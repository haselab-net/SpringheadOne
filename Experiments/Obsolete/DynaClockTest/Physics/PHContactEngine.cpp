#include "Physics.h"
#pragma hdrstop

extern bool bUseContactVolume;

namespace Spr{;

const float SPRING = 0.1f;
const float DAMPER = 0.002f;

const float STATIC_FRICTION = 0.6f;
const float DYNAMIC_FRICTION = 0.5f;


//----------------------------------------------------------------------------
//	PHContactEngine
//
SGOBJECTIMP(PHContactEngine, SGBehaviorEngine);

void PHContactEngine::FrameRecord::UpdateCache(int c){
	if ((unsigned)c == count) return;
	assert(solid);
	count = c;
	vel = solid->GetVelocity();
	angVel = solid->GetAngularVelocity();
	lastPos = pos;
	pos = solid->GetPosition();
	lastOri = ori;
	ori = solid->GetOrientation();
	cog = ori * solid->GetCenter() + pos;
}
PHContactEngine::PHContactEngine():firstLoadedCall(true){}

bool PHContactEngine::AddChildObject(SGObject* o, SGScene* scene){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	if (DCAST(SGFrame, o)){
		solids.push_back(new PHSolid);
		solids.back()->SetMass(FLT_MAX);
		solids.back()->SetInertiaInv(Matrix3d::Zero());
		solids.back()->frame = (SGFrame*)o;
		return true;
	}
	return false;
}

void PHContactEngine::AddInactive(int f1, int f2){
	inactiveList.push_back(std::make_pair(f1,f2));
}

extern WBPreciseTimer detectTimer, analyzeTimer, normalTimer, reflexTimer, frictionTimer;
void PHContactEngine::Before(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fp)
{
	FrameRecord* fr[2] = {
		(FrameRecord*)&*fp->frame[0]->records[frameRecordPos],
		(FrameRecord*)&*fp->frame[1]->records[frameRecordPos]
	};
	area=0;	//	衝突部分の面積
	//	運動に関するパラメータを取ってくる
	fr[0]->UpdateCache(count);
	fr[1]->UpdateCache(count);

	FramePairRecord* fpr = UTRef<FramePairRecord>(fp->records[framePairRecordPos]);
	fpr->cocog = 0.5f * (fr[0]->cog+fr[1]->cog);

	//	換算質量の計算
	float m=1.0f;
	if (fr[0]->solid->GetMass() < 1e10f && fr[1]->solid->GetMass() < 1e10f){
		float m0 = (float)fr[0]->solid->GetMass();
		float m1 = (float)fr[1]->solid->GetMass();
		m = m0 * m1 / (m0+m1);
	}else if (fr[0]->solid->GetMass() < 1e10f){
		m = (float)fr[0]->solid->GetMass();
	}else if (fr[1]->solid->GetMass() < 1e10f){
		m = (float)fr[1]->solid->GetMass();
	}
	convertedMass = m;
}

//	各交差について，ペナルティ，速度，面積を積分する．
void PHContactEngine::Analyzed(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fp, CDGeometryPair* gp, CDConvexPairWithRecord* cp, CDContactAnalysis* analyzer)
{
	FrameRecord* fr[2] = {
		(FrameRecord*)&*fp->frame[0]->records[frameRecordPos],
		(FrameRecord*)&*fp->frame[1]->records[frameRecordPos]
	};
#ifdef _DEBUG	//	交差部を描画
	Affinef afw;
	if (fr[0]->solid->frame->GetParent()) afw = fr[0]->solid->frame->GetParent()->GetWorldPosture();
	analyzer->Draw(*cp, afw, scene);
#endif

	//	衝突の法線を求める．
	analyzer->CalcNormal(*cp, gp->posture);	
	//	バネ係数とダンパ係数を決める
	float rs[2], rd[2], fs[2], fd[2], sf[2], df[2];
	for(int i=0; i<2; ++i){
		if (cp->convex[i]->pmaterial != NULL){
			rs[i] = cp->convex[i]->pmaterial->pMatData.reflexSpring * SPRING;
			rd[i] = cp->convex[i]->pmaterial->pMatData.reflexDamper * DAMPER;
			fs[i] = cp->convex[i]->pmaterial->pMatData.frictionSpring * SPRING;
			fd[i] = cp->convex[i]->pmaterial->pMatData.frictionDamper * DAMPER;
			sf[i] = cp->convex[i]->pmaterial->pMatData.staticFriction;
			df[i] = cp->convex[i]->pmaterial->pMatData.dynamicFriction;
		}else{
			rs[i] = fs[i] = SPRING;
			rd[i] = fd[i] = DAMPER;
			sf[i] = STATIC_FRICTION;
			df[i] = DYNAMIC_FRICTION;
		}
	}
	reflexSpring = (rs[0]+rs[1])/2 * convertedMass / (dt*dt);
	reflexDamper = (rd[0]+rd[1])/2 * convertedMass / (dt*dt);
	frictionSpring = (fs[0]+fs[1])/2 * convertedMass / (dt*dt);
	frictionDamper = (fd[0]+fd[1])/2 * convertedMass / (dt*dt);
	staticFriction = (sf[0]+sf[1])/2;
	dynamicFriction = (df[0]+df[1])/2;

	reflexSpring = 1000;
	reflexDamper = 50;

	//	衝突の面積，抗力を求める
	ConvexPairRecord* rec = (ConvexPairRecord*)&*cp->records[convexPairRecordPos];
	timerReflex.Start();
	CalcReflexForce(fr, cp, rec, analyzer);
	timerReflex.Stop();
	area += rec->area;
}

void PHContactEngine::After(SGScene* scene, float dt, unsigned count, CDFramePairWithRecord* fp){
	FrameRecord* fr[2] = {
		(FrameRecord*)&*fp->frame[0]->records[frameRecordPos],
		(FrameRecord*)&*fp->frame[1]->records[frameRecordPos]
	};
	FramePairRecord* fpr = (FramePairRecord*)&*fp->records[framePairRecordPos];
	fpr->Clear();
	//	抗力とその作用点を求め，摩擦を計算し，抗力と摩擦力を物体に加える．
	for(CDIntersections::iterator it = fp->intersections.begin(); it != fp->intersections.end(); ++it){
		CDConvexPairWithRecord* cp = (CDConvexPairWithRecord*) it->convexPair;
		ConvexPairRecord* rec = (ConvexPairRecord*)&*cp->records[convexPairRecordPos];
		//	積分したペナルティと速度を面積で割る
		rec->reflexSpringForce /= area;
		rec->reflexDamperForce /= area;
		rec->reflexSpringTorque /= area;
		rec->reflexDamperTorque /= area;
		rec->dynaFric /= area;
		rec->dynaFricMom /= area;
				
		DEBUG_EVAL(
			if ( !_finite( rec->reflexSpringForce.norm() )
				|| !_finite( rec->frictionForce.norm() )
				|| !_finite( rec->frictionTorque.norm() ) ){
				DSTR << "Error: forces: " << rec->reflexSpringForce << rec->frictionForce << rec->frictionTorque << std::endl;
			}
		)
		//	摩擦力を計算する
		timerFriction.Start();
		CalcFriction(fr, fpr, cp, rec);
		timerFriction.Stop();
		//	力を加える．
		Vec3f reflexForce = rec->reflexSpringForce + rec->reflexDamperForce;
		Vec3f reflexTorque = rec->reflexSpringTorque + rec->reflexDamperTorque
			- ((fpr->cocog - cp->commonPoint)^reflexForce);
		fpr->reflexForce += reflexForce;
		fpr->reflexTorque += reflexTorque;
		fpr->frictionForce += rec->frictionForce;
		fpr->frictionTorque += rec->frictionTorque
			- ((fpr->cocog - (rec->reflexForcePoint+cp->commonPoint)) ^ rec->frictionForce);
	}
	//	力を制限する．
//	fpr->LimitForces();
	// 力を加える．
	fpr->frictionForce = Vec3f();
	fpr->frictionTorque = Vec3f();
	fr[0]->solid->AddForce(fpr->reflexForce + fpr->frictionForce, fpr->cocog);
	fr[0]->solid->AddTorque(fpr->reflexTorque + fpr->frictionTorque);
	fr[1]->solid->AddForce(-(fpr->reflexForce + fpr->frictionForce), fpr->cocog);
	fr[1]->solid->AddTorque(-(fpr->reflexTorque + fpr->frictionTorque));
	DEBUG_EVAL( Draw(scene) );
}

//	凸形状対に発生する反力の計算と最大摩擦力の計算
//	すべて commonPoint を原点とした座標系で計算する．
void PHContactEngine::CalcReflexForce(PHContactEngine::FrameRecord** fr, CDConvexPairWithRecord* cp, PHContactEngine::ConvexPairRecord* rec, CDContactAnalysis* analyzer){
	rec->area = 0;
	rec->reflexSpringForce.clear();
	rec->reflexDamperForce.clear();
	rec->reflexSpringTorque.clear();
	rec->reflexDamperTorque.clear();
	rec->dynaFric.clear();
	rec->dynaFricMom.clear();

	Vec3f cog[2] = {fr[0]->cog - cp->commonPoint, fr[1]->cog - cp->commonPoint};
	if (bUseContactVolume){
		if (analyzer->isValid){	//	ポリゴン-ポリゴンの場合
			for(CDContactAnalysis::Vtxs::iterator it = analyzer->vtxs.begin(); it != analyzer->planes.vtxBegin; ++it){
				CDFace& face = **it;
				if (face.NCommonVtx() < 3) continue;
				int curID;
				if (((CDPolyhedron*)&*cp->convex[0])->HasFace(&face)){
					curID = 0;
				}else{
					curID = 1;
					assert(((CDPolyhedron*)&*cp->convex[1])->HasFace(&face));
				}
				Vec3f p0 = face.CommonVtx(0);
				Vec3f p1;
				Vec3f p2 = face.CommonVtx(1);
				for(int i=2; i<face.NCommonVtx(); ++i){
					p1 = p2;
					p2 = face.CommonVtx(i);
					CalcTriangleReflexForce(cp, rec, p0, p1, p2, cog[curID], fr[curID]->vel, fr[curID]->angVel);
	#ifdef _DEBUG	//	hase
					if (rec->reflexSpringForce.norm() > 10000 || !_finite(rec->reflexSpringForce.norm()) ){
						DSTR << "CalcTriangleReflexForce returned very large force." << std::endl;
						CalcTriangleReflexForce(cp, rec, p0, p1, p2, cog[curID], fr[curID]->vel, fr[curID]->angVel);
					}
	#endif
				}
			}
			/*	ap: 作用点とすると
				torque = ap ^ force,
				ap * force = 0 とすると ap ⊥ torque,  force ⊥ torque, ap ⊥ force 
				となり，3つのベクトルは直行する．
				そのため，
				|torque| = |ap|*|force| となる．

				ap = ((force^torque) / (|force|*|torque|)) * (|torque|/|force|)
				= (force^torque) / force^2
				
				(ap + k*normal) * normal = interior * normal
				k = (interior-ap) * normal 
			*/
			if (rec->reflexSpringForce.square() > 1e-8){
				rec->reflexForcePoint = (rec->reflexSpringForce ^ rec->reflexSpringTorque) / (rec->reflexSpringForce*rec->reflexSpringForce);
				rec->reflexForcePoint -= rec->reflexForcePoint * cp->normal * cp->normal;
			}else{
				rec->reflexForcePoint = cp->center - cp->commonPoint;
			}
		}else{	//	プリミティブの場合
			//	面積∝深さ^2と仮定
			rec->area = (float)(cp->depth*cp->depth);
			//	抗力
			rec->reflexSpringForce = - rec->area * reflexSpring * cp->depth * cp->normal;
			Vec3f cvel[2] = {
				fr[0]->vel + (fr[0]->angVel^(cp->center-fr[0]->cog)),
				fr[1]->vel + (fr[1]->angVel^(cp->center-fr[1]->cog)),
			};
			Vec3f dVel = cvel[1]-cvel[0];
			Vec3f dVelN = dVel*cp->normal*cp->normal;
			Vec3f dVelT = dVel - dVelN;
			rec->reflexDamperForce = rec->area * reflexDamper * dVelN;
			//	作用点は決まっている．
			rec->reflexForcePoint = cp->center - cp->commonPoint;
			//	抗力によるトルク
			rec->reflexSpringTorque = rec->reflexForcePoint ^ rec->reflexSpringForce;
			rec->reflexDamperTorque = rec->reflexForcePoint ^ rec->reflexDamperForce;
			//	最大摩擦力
			if (dVelT.norm() > 1e-6f){
				rec->dynaFric = -dVelT.unit() * (rec->reflexSpringForce + rec->reflexDamperForce).norm();
				Vec3f dAngVel = fr[1]->angVel - fr[0]->angVel;
				Vec3f dAngVelN = dAngVel*cp->normal*cp->normal;
				rec->dynaFricMom = - (float)cp->depth * rec->dynaFric.norm() * dAngVelN.unit()
					+ (rec->reflexForcePoint^rec->dynaFric);
			}else{
				rec->dynaFric = Vec3f();
				rec->dynaFricMom = Vec3f();
			}
		}
	}else{
		Vec3d closest[2] = {
			fr[0]->ori * cp->closestPoint[0] + fr[0]->pos,
			fr[1]->ori * cp->closestPoint[1] + fr[1]->pos};
		rec->reflexSpringForce = reflexSpring * (closest[1] - closest[0]);
		Vec3f colPos = (closest[0]+closest[1])/2;
		rec->reflexSpringTorque = (colPos-cp->commonPoint) ^ rec->reflexSpringForce;
		Vec3f vel[2];
		vel[0] = fr[0]->vel + (fr[0]->angVel ^ (colPos - fr[0]->cog));
		vel[1] = fr[1]->vel + (fr[1]->angVel ^ (colPos - fr[1]->cog));
		Vec3f rvel = vel[1] - vel[0];
		rec->reflexDamperForce = reflexDamper * rvel;
		rec->reflexDamperTorque = (colPos-cp->commonPoint) ^ rec->reflexDamperForce;
		rec->area = 10.0f;
	}
#if 0	//	hase
	if (!_finite(rec->reflexForcePoint.norm()) || !_finite(rec->reflexSpringForce.norm()) || rec->reflexForcePoint.norm() > 100){
		DSTR << "reflexForcePoint:" << rec->reflexForcePoint << std::endl;
		DSTR << "reflexSpringForce:"  << rec->reflexSpringForce << std::endl;
		DSTR << "reflexTorque:"  << rec->reflexSpringTorque << std::endl;
		DSTR << "commonPoint:"  << cp->commonPoint << std::endl;
	}
	if (rec->area<=0){
		DSTR << "Area:" << rec->area << std::endl;
	}
	if (rec->reflexSpringForce.norm() > 10000 || !_finite(rec->reflexSpringForce.norm()) ){
		int i=0;
		DSTR << "Error: Too large reflexForce: " << rec->reflexSpringForce << std::endl;
		DSTR << "solidName:" << fr[0]->solid->GetName() << " - " << fr[1]->solid->GetName() << "  ";
		DSTR << "commonPoint:" << cp->commonPoint << std::endl;
		for(CDContactAnalysis::Vtxs::iterator it = analyzer->vtxs.begin(); it != analyzer->planes.vtxBegin; ++it){
			CDFace& face = **it;
			DSTR << "p" << i++ << "\t d:" << face.dist << " n:" << face.normal << std::endl;
		}
		i = 1;
		for(CDQHPlane<CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end; ++it){
			if (it->deleted) continue;
			DSTR << "v" << i++ << "\t " << it->normal/it->dist << std::endl;
		}
	}
#endif
}
void PHContactEngine::CalcTriangleReflexForce(CDConvexPairWithRecord* cp, PHContactEngine::ConvexPairRecord* rec, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel){
	//---------------------------------------------------------------
	//	ばねモデルの計算：各頂点の侵入深さの計算
	float depth0 = p0 * cp->normal;
 	float depth1 = p1 * cp->normal;
 	float depth2 = p2 * cp->normal;
	//	ダンパモデルの計算：各頂点の速度とその法線方向成分を求める
	angVel *= 1;	/*	角速度をダンパの計算の前に増やす．
						ここで角速度のダンパを強くすると安定するけど，
						転がらなくなる．
						抗力は出しても，引力は出さないような工夫をすれば
						安定でかつ転がるようにできるのではないかと思う．
						by hase
					*/
	p0 -= depth0 * cp->normal;
	p1 -= depth1 * cp->normal;
	p2 -= depth2 * cp->normal;
	Vec3f vel0 = (angVel^(p0-cog)) + vel;
	Vec3f vel1 = (angVel^(p1-cog)) + vel;
	Vec3f vel2 = (angVel^(p2-cog)) + vel;
	float vel0_normal = vel0 * cp->normal;
	float vel1_normal = vel1 * cp->normal;
	float vel2_normal = vel2 * cp->normal;

	//	抗力を計算
	float refSp0 = reflexSpring * depth0;
	float refSp1 = reflexSpring * depth1;
	float refSp2 = reflexSpring * depth2;

	float refDa0 = reflexDamper * vel0_normal;
	float refDa1 = reflexDamper * vel1_normal;
	float refDa2 = reflexDamper * vel2_normal;

	//---------------------------------------------------------------
	//	三角形について積分
	Vec3f a = p1 - p0;
	Vec3f b = p2 - p0;
	Vec3f a_b = a^b;
	float a_b_normal = a_b * cp->normal;
	Vec3f triRefSp = (1.0f/6.0f) * (refSp0 + refSp1 + refSp2) * a_b;
	Vec3f triRefMomSp = (
				((1.0f/12.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p0
			+	((1.0f/24.0f)*refSp0 + (1.0f/12.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p1
			+	((1.0f/24.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/12.0f)*refSp2) * p2
		  ) ^ a_b;
	Vec3f triRefDa = (1.0f/6.0f) * (refDa0 + refDa1 + refDa2) * a_b;
	Vec3f triRefMomDa = (
				((1.0f/12.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p0
			+	((1.0f/24.0f)*refDa0 + (1.0f/12.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p1
			+	((1.0f/24.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/12.0f)*refDa2) * p2
		  ) ^ a_b;
#ifdef _DEBUG
	if (refSp0 > 1000 || refSp1 > 1000 || refSp2 > 1000 || !_finite(triRefSp.norm()) ){
		DSTR << "Error: The reflection spring force is too large: " << triRefSp << std::endl;
	}
#endif
	assert(_finite(triRefMomSp.norm()));

	//	3角形の面積の計算
	float triArea = a_b_normal / 4;
	if (triArea < 0) triArea = -triArea;

	//---------------------------------------------------------------
	//	動摩擦力を求める
	Vec3f velTan0 = vel0 - vel0_normal * cp->normal;
	Vec3f velTan1 = vel1 - vel1_normal * cp->normal;
	Vec3f velTan2 = vel2 - vel2_normal * cp->normal;
	Vec3f fric0, fric1, fric2;
	if (velTan0.square() > 1e-8) fric0 = velTan0.unit() * (refSp0+refDa0);
	if (velTan1.square() > 1e-8) fric1 = velTan1.unit() * (refSp1+refDa1);
	if (velTan2.square() > 1e-8) fric2 = velTan2.unit() * (refSp2+refDa2);

	Vec3f triFric = (1.0f/6.0f) * (fric0 + fric1 + fric2) * a_b_normal;
	Vec3f triFricMom = (
				(p0 ^ ((1.0f/12.0f)*fric0 + (1.0f/24.0f)*fric1 + (1.0f/24.0f)*fric2))
			+	(p1 ^ ((1.0f/24.0f)*fric0 + (1.0f/12.0f)*fric1 + (1.0f/24.0f)*fric2))
			+	(p2 ^ ((1.0f/24.0f)*fric0 + (1.0f/24.0f)*fric1 + (1.0f/12.0f)*fric2))
		  ) * a_b_normal;
	assert(_finite(triFric.norm()));
	assert(_finite(triFricMom.norm()));
	//---------------------------------------------------------------
	//	結果を加える
	rec->area += triArea;
	rec->reflexSpringForce += triRefSp;
	rec->reflexSpringTorque += triRefMomSp;
	rec->reflexDamperForce += triRefDa;
	rec->reflexDamperTorque += triRefMomDa;
	rec->dynaFric += triFric;
	rec->dynaFricMom += triFricMom;
}

//	凸形状対に発生する摩擦力の計算
//	力の作用点を原点とした座標系で計算する．
void PHContactEngine::CalcFriction(PHContactEngine::FrameRecord** fr, PHContactEngine::FramePairRecord* fpr, CDConvexPairWithNormal* cp, PHContactEngine::ConvexPairRecord* rec){
	//	初めての接触の時
	Vec3f reflexForcePoint = rec->reflexForcePoint + cp->commonPoint;	//	力の作用点(絶対系)
	if (cp->state == CDConvexPair::NEW){
		//	バネモデルの始点を設定する．
		rec->transFrictionBase[0] = fr[0]->ori.inv() * (reflexForcePoint - fr[0]->pos);
		rec->transFrictionBase[1] = fr[1]->ori.inv() * (reflexForcePoint - fr[1]->pos);
		rec->rotSpring = 0;
		rec->frictionState = ConvexPairRecord::STATIC;
		return;
	}

	//	接触が継続している場合．
	
	//	並進バネの計算
	//	グローバル系に変換
	rec->transFrictionBase[0] = fr[0]->ori * rec->transFrictionBase[0] + fr[0]->pos;
	rec->transFrictionBase[1] = fr[1]->ori * rec->transFrictionBase[1] + fr[1]->pos;
	//	平面上に落とす
	rec->transFrictionBase[0] -= (rec->transFrictionBase[0]-cp->center) * cp->normal * cp->normal;
	rec->transFrictionBase[1] -= (rec->transFrictionBase[1]-cp->center) * cp->normal * cp->normal;
	//	差がバネの力
	Vec3f transSpring = rec->transFrictionBase[1] - rec->transFrictionBase[0];
	float transSpringNorm = transSpring.norm();
	float frictionSpringForce = frictionSpring * transSpringNorm;
	Vec3f frictionForceDirection;
	if (transSpringNorm>1e-10f) frictionForceDirection = transSpring / transSpringNorm;

	//	回転バネの計算
	Quaternionf delta[2] = { fr[0]->ori*fr[0]->lastOri.inv(), fr[1]->ori*fr[1]->lastOri.inv() };
	rec->rotSpring += delta[1].angle()*cp->normal - delta[0].angle()*cp->normal;
	float frictionSpringTorque = frictionSpring*rec->rotSpring;

	//	動摩擦の制約を加える
	float fricCoeff = (rec->frictionState == ConvexPairRecord::STATIC) ? staticFriction : dynamicFriction;
	float maxFric = fricCoeff * rec->dynaFric.norm();
	float maxRotFric = fricCoeff * (rec->dynaFricMom - (rec->reflexForcePoint^rec->dynaFric)).norm();
	assert(_finite(maxFric));
	DEBUG_EVAL( if(!_finite(maxRotFric)){ DSTR << "FricMax:" << maxFric << "  " << maxRotFric << std::endl; } )

	rec->frictionState = ConvexPairRecord::STATIC;
	if (frictionSpringForce > maxFric){
		frictionSpringForce = maxFric;
		rec->frictionState = ConvexPairRecord::DYNAMIC;
	}
	if (abs(frictionSpringTorque) > maxRotFric){
		frictionSpringTorque = sign(frictionSpringTorque) * maxRotFric;
		rec->frictionState = ConvexPairRecord::DYNAMIC;
	}
	rec->frictionForce = frictionSpringForce * frictionForceDirection;
	rec->frictionTorque = frictionSpringTorque * cp->normal;

	if (rec->frictionState == ConvexPairRecord::STATIC){
		///	ダンパによる静止摩擦力の計算
		Vec3f frictionVel = ((fr[1]->angVel ^ (reflexForcePoint-fr[1]->cog)) + fr[1]->vel) - ((fr[0]->angVel ^ (reflexForcePoint-fr[0]->cog)) + fr[0]->vel);
		frictionVel -= frictionVel * cp->normal * cp->normal;
		rec->frictionForce += frictionDamper * frictionVel;
		
		Vec3f frictionAngVel = fr[1]->angVel - fr[0]->angVel;
		DEBUG_EVAL( if (!_finite(frictionAngVel.norm())){ DSTR << "frictionAngVel: " << frictionAngVel << std::endl; } )
		

		frictionAngVel = (frictionAngVel * cp->normal) * cp->normal;
		rec->frictionTorque += frictionDamper * frictionAngVel;

		if (rec->frictionForce.norm() > maxFric){
			if (rec->frictionForce.norm() > 1e-10f) rec->frictionForce = rec->frictionForce.unit() * maxFric;
			else rec->frictionForce = Vec3f();
		}

		if (rec->frictionTorque.norm() > maxRotFric){
			if(rec->frictionTorque.norm() > 1e-10f) rec->frictionTorque = rec->frictionTorque.unit() * maxRotFric;
			else rec->frictionTorque = Vec3f();
		}
	}
#if 0
	   if (rec->frictionTorque.norm() > 100){
		   DSTR << "FT:" << rec->frictionTorque << area << std::endl;
		   DSTR << "rotFricSpring:" << rec->rotSpring;
		   DSTR << std::endl;
	   }
#endif

	//	ローカル系に変換して保存
	rec->transFrictionBase[0] = fr[0]->ori.inv() * (reflexForcePoint - 0.5f*frictionSpringForce/frictionSpring*frictionForceDirection - fr[0]->pos);
	rec->transFrictionBase[1] = fr[1]->ori.inv() * (reflexForcePoint + 0.5f*frictionSpringForce/frictionSpring*frictionForceDirection - fr[1]->pos);
	rec->rotSpring = frictionSpringTorque / frictionSpring;
}

void PHContactEngine::Draw(SGScene* scene){
}

void PHContactEngine::Clear(SGScene* s){
	listenerPos = -1;
	inactiveList.clear();
	solids.clear();
}
void PHContactEngine::Loaded(SGScene* scene){
	if (!firstLoadedCall) return;
	firstLoadedCall = false;
	Init(scene);
}

void PHContactEngine::Init(SGScene* scene){
	for(unsigned int i=0; i<solids.size(); ++i){
		if (DCAST(SGFrame, solids[i])){
			SGFrame* fr = UTRef<SGFrame>(solids[i]);
			solids[i] = new PHSolid;
			solids[i]->SetMass(FLT_MAX);
			solids[i]->SetInertiaInv(Matrix3d::Zero());
			solids[i]->frame = fr;
			solids[i]->SetName("_solid", scene);
		}
	}
	if (!collisionEngine){
		scene->GetBehaviors().Find(collisionEngine);
		if (!collisionEngine){
			collisionEngine = new CDCollisionEngine;
			collisionEngine->SetName("theCollisionEngine", scene);
			scene->GetBehaviors().Add(collisionEngine);
		}
		listenerPos = collisionEngine->AddListener(this);
		framePairRecordPos = collisionEngine->ReserveFramePairRecord();
		convexPairRecordPos = collisionEngine->ReserveConvexPairRecord();
		frameRecordPos = collisionEngine->ReserveFrameRecord();
	}
	for(unsigned i=0; i<solids.size(); ++i){
		FrameRecord* fr = new FrameRecord;
		fr->solid = solids[i];
		collisionEngine->AddFrame(solids[i]->frame, fr, frameRecordPos);
	}
	for(unsigned i=0; i<solids.size(); ++i){
		for(unsigned j=i+1; j<solids.size(); ++j){
			int f1 = collisionEngine->GetFrameID(solids[i]->frame);
			int f2 = collisionEngine->GetFrameID(solids[j]->frame);
			collisionEngine->AddActive(f1, f2, listenerPos);
		}
	}
	for(unsigned i=0; i<inactiveList.size(); ++i){
		SGFrame* f1 = solids[inactiveList[i].first]->frame;
		SGFrame* f2 = solids[inactiveList[i].second]->frame;
		collisionEngine->AddInactive(f1, f2, listenerPos);
	}
	collisionEngine->Init();
	for(int i=0; i<collisionEngine->NFrame(); ++i){
		for(int j=i+1; j<collisionEngine->NFrame(); ++j){
			CDFramePairWithRecord& fp = collisionEngine->GetFramePair(i, j);
			fp.records[framePairRecordPos] = new FramePairRecord;
			for(CDGeometryPairs::iterator itg = fp.geometryPairs.begin();
				itg != fp.geometryPairs.end(); ++itg){
				for(CDConvexPairs::iterator itc = (*itg)->convexPairs.begin(); itc != (*itg)->convexPairs.end(); ++itc){
					CDConvexPairWithRecord* cp = (CDConvexPairWithRecord*)&**itc;
					cp->records[convexPairRecordPos] = new ConvexPairRecord;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
//	PHContactEngineLoader
//
class PHContactEngineLoader:public FIObjectLoader<PHContactEngine>{
	virtual void LoadData(FILoadScene* ctx, PHContactEngine* pc){
		ctx->objects.Push(pc);
	}
	virtual void Loaded(FILoadScene* ctx){
		ctx->objects.Pop();
	}
};
class PHContactEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHContactEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHContactEngine* pc = (PHContactEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("ContactEngine", pc);
		ctx->docs.back()->AddChild(doc);
		for(PHSolids::iterator it = pc->solids.begin(); it != pc->solids.end(); ++it){
			if (strlen((*it)->GetName())){
				doc->AddChild(ctx->CreateDocNode("Solid", *it, true));
			}else{
				doc->AddChild(ctx->CreateDocNode("Frame", (*it)->frame, true));
			}
		}
	}
};
DEF_REGISTER_BOTH(PHContactEngine);


#pragma pack(push, 4)

DEF_RECORD(XContactInactive, {
	DWORD nFrames;
	FIVVector<DWORD> frames VSIZE(nFrames);
})

#pragma pack(pop)

class PHContactInactiveLoader:public FIBaseLoader{
public:
	virtual UTString GetType() const{ return "ContactInactive"; }
	UTRef<FITypeDescDb> db;
	PHContactInactiveLoader(){
		db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_RECORD(XContactInactive);
	}
	virtual void Load(FILoadScene* ctx){
		FIDocNodeBase* doc = ctx->docs.Top();
		FITypeDesc* desc = db->Find("ContactInactive");
		XContactInactive inactive;
		desc->Read(&inactive, doc);
		PHContactEngine* ce = ACAST(PHContactEngine, ctx->objects.back());
		for(unsigned i=0; i<inactive.frames.size(); ++i){
			for(unsigned j=i+1; j<inactive.frames.size(); ++j){
				ce->AddInactive(inactive.frames[i], inactive.frames[j]);
			}
		}
	}
};

DEF_REGISTER_LOADER(PHContactInactive);

}
