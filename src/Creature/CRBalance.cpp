// CRBalance.cpp: CRBalance クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "CRBalance.h"
#include "CRHuman.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace Spr{;

CRBalance::CRBalance()
{
	bCalc = false;
	bReload = false;
	bControl = true;
	spring_no = 0.0005f;
	dampa_no = 0.0015f; 
	gravity = Vec3f(0.0,-9.8,0.0);
	targetZmp = Vec3f();
	spring = Vec3f();
	dampa = 0.0f;

	contactEngine = NULL;
	jointEngine = NULL;
	//目標姿勢の設定
	targetPostureQ = Quaterniond(1.0,0.0,0.0,0.0);

}

CRBalance::~CRBalance()
{

}

//------------------　基本的なインタフェース　------------------//

void CRBalance::Step(SGScene* scene,GRRender* render,CRHuman* crHuman){

	if(bCalc){
		CalcBodyTorque(scene,crHuman);				// バランスに必要なトルクを計算
		CalcTargetZMP(scene,crHuman);				// 目標ZMPの計算

		//ControlBody(crHuman);						// 前の足首だけの方法で体のバランスをとる。
		AppControl(scene,crHuman);					// アプリケーション

		ZmpIncludeCheck(supportArea);				// 目標ZMPが安定領域に入ってるいるか否か
		supportArea.Step();							// 安定領域の計算

		// 二次計画法
		balanceQuadPR.Step(crHuman);

		Draw(render);								// 描画
	}
}

void CRBalance::Load(SGScene* scene,CRHuman* crHuman){

	bCalc = false;
	bReload = false;
	bControl = true;
	targetZmp = Vec3f();
	spring = Vec3f();
	dampa = 0.0f;
	contactEngine = NULL;
	jointEngine = NULL;

	if(crHuman->IsLoaded()){
		bCalc = true;
		scene->GetBehaviors().Find(contactEngine);	// contactEngineの取得
		scene->GetBehaviors().Find(jointEngine);	// jointEngineの取得
		supportArea.Load(scene,crHuman);			// SupportAreaのLoad
	
		// 二次計画法
		balanceQuadPR.Init(scene,crHuman);
	}

}

void CRBalance::OnKeyDown(UINT &nChar,CRHuman* crHuman){
	if(nChar == 'E'){
		crHuman->jointPids[9]->goal += 3*M_PI/180;
		crHuman->jointPids[16]->goal += 3*M_PI/180;
	}
}

void CRBalance::Draw(GRRender* render){
	//目標ZMPの描画
	DrawZMP(render);
	//安定領域の描画
	supportArea.DrawCollisionVetxs(render);
}


void CRBalance::DrawZMP(GRRender* render){
	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(1,1,0,1)));
	render->SetLineWidth(10);
	Vec3f v = targetZmp;
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
}


void CRBalance::ControlBody(CRHuman* crHuman){

	if(bControl){
		//　姿勢に応じてトルクをかける方法
		//足首の制御
		if(crHuman->joints[27] != NULL){	//右足首
			Vec3f rightTorque = crHuman->solids[11]->GetRotation()*targetTorque;	//足首の座標へ変換
			crHuman->joints[27]->SetTorque(-rightTorque.x);
			crHuman->joints[28]->SetTorque(-rightTorque.z);
			crHuman->joints[29]->SetTorque(-rightTorque.y);
		}
		if(crHuman->joints[35] != NULL){	//左足首
			Vec3f leftTorque = crHuman->solids[15]->GetRotation()*targetTorque;
			crHuman->joints[35]->SetTorque(-leftTorque.x);
			crHuman->joints[36]->SetTorque(leftTorque.z);
			crHuman->joints[37]->SetTorque(leftTorque.y);
		}

	}else{ //足首にかけるトルクをなくす
		if(crHuman->joints[27] != NULL){	//右足首
			crHuman->joints[27]->SetTorque(0.0f);
			crHuman->joints[28]->SetTorque(0.0f);
			crHuman->joints[29]->SetTorque(0.0f);
		}
		if(crHuman->joints[35] != NULL){	//左足首
			crHuman->joints[35]->SetTorque(0.0f);
			crHuman->joints[36]->SetTorque(0.0f);
			crHuman->joints[37]->SetTorque(0.0f);
		}
	}
	//	つま先の制御
	/*if(torque.x > 0){
		if(crHuman->joints[30] != NULL){	//右足つま先
			Vec3f rightToeTorque = crHuman->solids[12]->GetRotation()*torque/5;
			crHuman->joints[30]->SetTorque(rightToeTorque.x);
		}
		if(crHuman->joints[38] != NULL){	//左足つま先
			Vec3f leftToeTorque = crHuman->solids[16]->GetRotation()*torque/5;
			crHuman->joints[38]->SetTorque(leftToeTorque.x);
		}
	}*/
		
}


void CRBalance::CalcTargetZMP(SGScene* scene,CRHuman* crHuman){

	//重心の目標加速度
	targetAccel = Vec3f(0.0,0.0,0.0);

	//目標ZMPの計算
	targetForce = crHuman->GetTotalMass()*(targetAccel-gravity);
	targetZmp = (targetForce % targetTorque) / targetForce.square();
	targetZmp += cogPos;
	targetZmp.Y() = 0;

}

void CRBalance::CalcBodyTorque(SGScene* scene, CRHuman* crHuman){
	//体全体の重心の位置
	cogPos = crHuman->GetCOG();

	//totalMass = crHuman->GetTotalMass();

	//　重心まわりの全身の慣性テンソルを求める
	Matrix3d cog_all_inartia = Matrix3d::Zero();
	for(int i = 0; i < crHuman->solids.size(); i++){
		if(crHuman->solids[i] != NULL){
			Vec3d temp = crHuman->solids[i]->GetCenterPosition() - cogPos;
			Matrix3d X = Matrix3d::Zero();
			X.xx = temp.y*temp.y + temp.z*temp.z;
			X.yx = -temp.x*temp.y;
			X.zx = -temp.x*temp.z;
			X.xy = -temp.x*temp.y;
			X.yy = temp.x*temp.x + temp.z*temp.z;
			X.zy = -temp.z*temp.y;
			X.xz = -temp.x*temp.z;
			X.yz = -temp.z*temp.y;
			X.zz = temp.x*temp.x + temp.y*temp.y;
			
			cog_all_inartia += crHuman->solids[i]->GetRotation()*crHuman->solids[i]->GetInertia()*crHuman->solids[i]->GetRotation().trans()
									+ crHuman->solids[i]->GetMass()*X;
		}
	}

	//バネ係数、ダンパ係数の計算
	spring.x = (2*spring_no*cog_all_inartia.xx)/(scene->GetTimeStep()*scene->GetTimeStep());
	spring.y = (2*spring_no*cog_all_inartia.yy)/(scene->GetTimeStep()*scene->GetTimeStep());
	spring.z = (2*spring_no*cog_all_inartia.zz)/(scene->GetTimeStep()*scene->GetTimeStep());
	dampa = dampa_no/scene->GetTimeStep();

	///////   トルクの計算（PD制御による）  ///////
	Quaterniond quatd;
	//　バネ係数の項（基準となるsolidの姿勢を戻す)
	for(int i = 0; i < crHuman->solids.size(); i++){
		if(crHuman->solids[i] != NULL){
			quatd = targetPostureQ * crHuman->solids[i]->GetOrientation().inv();
			break;
		}
	}
	//　ダンパ係数の項(全角運動量を0にする)
	Vec3f L;
	for(int j = 0; j < crHuman->solids.size(); j++){
		if(crHuman->solids[j] != NULL){
			L += (crHuman->solids[j]->GetCenterPosition() - cogPos) % (crHuman->solids[j]->GetMass()*crHuman->solids[j]->GetVelocity());
		}
	}

	targetTorque.x = spring.x*quatd.rotation().x - dampa*L.x;
	targetTorque.y = spring.y*quatd.rotation().y - dampa*L.y;
	targetTorque.z = spring.z*quatd.rotation().z - dampa*L.z;
}

void CRBalance::AppControl(SGScene* scene,CRHuman* crHuman){

	// 体に力をかけるか否か
	if(bControl){
		//if(abs((cogPos - targetZmp).norm()) > 0.9){
		Vec2f comp;
		comp.x = cogPos.x - targetZmp.x;
		comp.y = cogPos.z - targetZmp.z;
		if(abs(comp.norm()) > 0.3){
			bControl = false;
		}
	}
	
	// リロードするか否か
	SGFrame* floor;
	scene->FindObject(floor,"frFloor");
	PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(crHuman->solids[0]->GetFrame(),floor);
	if(fpr->GetReflexForce().norm() > 0.1){
		bReload = true;
	}else{
		bReload = false;
	}
}


bool CRBalance::ZmpIncludeCheck(CRSupportArea& supportArea){

	bool zmp_in = false;
	std::vector<Vec3f> supportVetxs = supportArea.GetSupportArea();
	if(supportVetxs.size() > 3){
		for(int l = 0; l < supportVetxs.size() - 2  && !zmp_in; ++l){
			for(int m = l+1; m < supportVetxs.size() - 1 && !zmp_in; ++m){
				for(int n = m+1; n < supportVetxs.size() && !zmp_in; ++n){
					zmp_in = PosIncludeTriangle(supportVetxs[l],
						supportVetxs[m], supportVetxs[n], targetZmp);
				}
			}
		}
	}
	return zmp_in;
}

bool CRBalance::PosIncludeTriangle(Vec3f t_a, Vec3f t_b, Vec3f t_c, Vec3f zmp){

	//t_a,t_b,t_cの三点の中にzmpが内包されているかどうか

	float p_a, p_b, a_b, b_a, t, s;
	Spr::Vec3f a_v, b_v;

	a_v = t_b - t_a;
	b_v = t_c - t_a;
	zmp -= t_a;

	p_a = (zmp*a_v)/a_v.norm();
	p_b = (zmp*b_v)/b_v.norm();
	a_b = (a_v*b_v)/b_v.norm();
	b_a = (a_v*b_v)/a_v.norm();

	t = (p_a*a_b - p_b*a_v.norm())/(a_b*b_a - a_v.norm()*b_v.norm());
	s = (p_a*b_v.norm() - p_b*b_a)/(a_v.norm()*b_v.norm() - a_b*b_a);

	if(t >= 0 && t <= 1 && s >= 0 && s <= 1 && s + t >= 0 && s + t <= 1)
		return true;	//内包している場合
	else
		return false;	//内包していない場合
}


}	// end of namespace