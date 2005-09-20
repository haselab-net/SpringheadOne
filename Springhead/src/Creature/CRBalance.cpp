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
	spring_noV		= Vec3f();
//	spring_no		= 0.0f;
	dampa_no		= 0.0f;
	targetZmp		= Vec3f();
	targetAccel		= Vec3f();
	jointEngine		= NULL;
	gravityEngine	= NULL;
	springCog_no	= 0.0f;
	dampaCog_no		= 0.0f;
}

CRBalance::~CRBalance()
{

}

//------------------　基本的なインタフェース　------------------//

void CRBalance::Step(SGScene* scene,CRHuman* crHuman){

	Measure(scene,crHuman);
	ModifyBody(scene,crHuman);

}
void CRBalance::Measure(SGScene* scene,CRHuman* crHuman){
	CalcBalanceTorque(scene,crHuman);			// バランスに必要なトルクを計算
	CalcBalanceForce(scene,crHuman);			// バランスに必要な力を計算
	CalcTargetZMP(scene,crHuman);				// 目標ZMPの計算
	ZmpIncludeCheck(crHuman->supportArea);		// 目標ZMPが安定領域に入ってるいるか否か
}

void CRBalance::ModifyBody(SGScene* scene,CRHuman* crHuman){
	ControlBody(crHuman);
}

void CRBalance::SetBalanceParameter(CRHuman* crHuman){

	// 重心の目標加速度(バランスが取れた状態の）
	targetAccel = Vec3f(0.0,0.0,0.0);
	//目標姿勢の設定
	targetPostureQ = Quaterniond(1.0,0.0,0.0,0.0);

	// バランストルク計算のバネ・ダンパ設定
	dampa_no = 0.0003f*19;
	for(int i = 0; i < 3; i++)
		spring_noV[i] = 0.0001f*0.3f;

	// 重心を安定領域内にする力のバネ・ダンパの設定
	springCog_no	= 0.000005f*0.3f;
	dampaCog_no		= 0.000015f*19;

}

void CRBalance::Load(SGScene* scene,CRHuman* crHuman){
	if(crHuman->IsLoaded()){
		Init(scene,crHuman);
		scene->GetBehaviors().Find(jointEngine);	// jointEngineの取得
		scene->GetBehaviors().Find(gravityEngine);	// gravityEngineの取得
		SetBalanceParameter(crHuman);
	}
}

void CRBalance::Init(SGScene* scene,CRHuman* crHuman){
//	spring_no		= 0.0f;
	spring_noV		= Vec3f();
	dampa_no		= 0.0f;
	targetZmp		= Vec3f();
	targetAccel		= Vec3f();
	jointEngine		= NULL;
	gravityEngine	= NULL;
	springCog_no	= 0.0f;
	dampaCog_no		= 0.0f;
}

void CRBalance::OnKeyDown(UINT &nChar,CRHuman* crHuman){

}

void CRBalance::Draw(GRRender* render,CRSupportArea* supportArea){
	//目標ZMPの描画
	DrawZMP(render);
	//安定領域の描画
	supportArea->DrawCollisionVetxs(render);
}

void CRBalance::DrawZMP(GRRender* render){
	render->SetModelMatrix(Affinef());
	render->SetMaterial(GRMaterialData(Spr::Vec4f(1,1,0,1)));
	render->SetLineWidth(10);
	Vec3f v = targetZmp;
	render->DrawDirect(Spr::GRRender::POINTS, &v , &v+1);
}

void CRBalance::ControlBody(CRHuman* crHuman){

	//　姿勢に応じてトルクをかける方法
	//足首の制御
	if(crHuman->joints[27] != NULL){	//右足首
		Vec3f rightTorque = (crHuman->solids[11]->GetRotation()*balanceTorque)/2;	//足首の座標へ変換
		crHuman->joints[27]->SetJointTorque(-rightTorque.x, 0);
		crHuman->joints[28]->SetJointTorque(-rightTorque.z, 0);
		crHuman->joints[29]->SetJointTorque(-rightTorque.y, 0);
	}
	if(crHuman->joints[35] != NULL){	//左足首
		Vec3f leftTorque = (crHuman->solids[15]->GetRotation()*balanceTorque)/2;
		crHuman->joints[35]->SetJointTorque(-leftTorque.x, 0);
		crHuman->joints[36]->SetJointTorque(leftTorque.z, 0);
		crHuman->joints[37]->SetJointTorque(leftTorque.y, 0);
	}
}

void CRBalance::CalcTargetZMP(SGScene* scene,CRHuman* crHuman){

	// 重力 + 重心を安定領域の中心に近づける力
	Vec3d cogForce = balanceForce + crHuman->GetTotalMass()*(targetAccel-gravityEngine->accel);
	//Vec3d cogTorque = balanceTorque + crHuman->GetCOG()%cogForce;
	Vec3d cogTorque = balanceTorque;

	//目標ZMPの計算
	targetZmp = -(cogForce % (-cogTorque)) / cogForce.square();
	//targetZmp = (balanceForce % balanceTorque) / balanceForce.square();
	targetZmp += cogPos;
	targetZmp.Y() = 0;

	/*
	if(!ZmpIncludeCheck(crHuman->supportArea)){
		// 最近傍点を求める
		targetZmp=crHuman->supportArea.GetSupportArea().ClosestPoint(targetZmp);
		balanceTorque = -targetZmp%cogForce;
	}
	*/
}

void CRBalance::CalcBalanceForce(SGScene* scene,CRHuman* crHuman){
	//targetForce = crHuman->GetTotalMass()*(targetAccel-gravityEngine->accel);
	std::vector<Vec3f> supportVetxs = crHuman->supportArea.GetSupportArea();
	Vec3f center = Vec3f();
	for(int i = 0; i < supportVetxs.size(); i++){
		center += supportVetxs[i];
	}
	center /= supportVetxs.size();
	float dt = scene->GetTimeStep();
	float k = springCog_no*(2*crHuman->GetTotalMass()/(dt*dt));
	float b = dampaCog_no*(crHuman->GetTotalMass()/dt);
	balanceForce = k*(center - crHuman->GetCOG()) - b*crHuman->GetCogVelocity();
	balanceForce.y = 0.0f;
}

void CRBalance::CalcBalanceTorque(SGScene* scene, CRHuman* crHuman){

	//　バランスをとるものの重心
	cogPos = crHuman->GetCOG();

	//　重心まわりの全身の慣性テンソルを計算(ダンパ係数を無次元化のため)
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
	double timeStep = scene->GetTimeStep();
	/*
	spring.x = (2*spring_no*cog_all_inartia.xx)/(timeStep*timeStep);
	spring.y = (2*spring_no*cog_all_inartia.yy)/(timeStep*timeStep);
	spring.z = (2*spring_no*cog_all_inartia.zz)/(timeStep*timeStep);
	*/
	Vec3f spring = (2*spring_noV*cog_all_inartia)/(timeStep*timeStep);
	float dampa = dampa_no/timeStep;

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
	// バランストルク
	balanceTorque = TMatrix3<float>::Diag(spring.x,spring.y,spring.z)*quatd.rotation() - dampa*L;
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