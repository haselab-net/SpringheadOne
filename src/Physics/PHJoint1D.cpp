#include "Physics.h"
#pragma hdrstop
#include "PHJoint1D.h"
#include <iomanip>

using namespace PTM;
namespace Spr{;
SGOBJECTIMPABST(PHJoint1D, PHJointBase);

void PHJoint1D::Reset(){
	accel = 0;
	torque = 0;

	//非ルートノード
	if(GetParent()){	
		PropagateState();
		solid->SetCenterPosition(p);
		solid->SetRotation(R);
		solid->UpdateFrame();
	}
	//ルートノード
	else{
		quat.from_matrix(R);
		v = R.trans() * v_abs;
		w = R.trans() * w_abs;
	}
}

////////////////////////////////////////////////////////
//・Ia: Articulated Inertia計算
//・Za: Articulated 0-accel force計算
void PHJoint1D::CompArticulatedInertia(double dt)
{
	//	Ia, Zaをクリア
	Ia.clear();
	Za.clear();

	//子ノードから先に計算
	for(array_type::const_iterator it = Children().end(); it != Children().begin();)
		(*--it)->CompArticulatedInertia(dt);

	//よく使う出てくる式をキャッシュ
	rcross = Matrix3d::Cross(prc);
	rpcross = Matrix3d::Cross(pRc * prc);
	rcross_cRp = rcross * cRp;
	rpcross_pRc = rpcross * pRc;
		
	//Za
	if(solid){
		svitem(Za, 0) += -R.trans() * solid->GetForce();
		svitem(Za, 1) += -R.trans() * solid->GetTorque() + Vec3d(
			(I[2][2] - I[1][1]) * w.Y() * w.Z(),
			(I[0][0] - I[2][2]) * w.Z() * w.X(),
			(I[1][1] - I[0][0]) * w.X() * w.Y());
	}
	//Ia
	Ia += Ii;

	//これもキャッシュ
	smat = svmat(s, s);
	Ia_c = Ia * c;
	Ia_s = Ia * s;
	Z_plus_Ia_c = Za + Ia_c;
	dot_s_Ia_s = svdot(s, Ia_s);
	dot_s_Z_plus_Ia_c = svdot(s, Z_plus_Ia_c);
	
/*	static bReport;
	if (abs(Ia.det()) < 1e-2f || bReport || abs(accel)>1e4){
		DSTR << GetName() << "a:" << accel << "v:" << info.velocity << " Ia.det:" << Ia.det();
		DSTR << " dot_s_Ia_s:" << dot_s_Ia_s;
		DSTR << " Ii.det:" << Ii.det();
		DSTR << " Za:" << Za.norm() << std::endl;
		bReport = true;
	}
*/
	//親ノードのZa,Iaに自分のZa,Iaを積み上げる
	if(GetParent()){
/*		if (abs(dot_s_Ia_s) < 1e-1){
			DSTR << Ia << (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) << std::endl;
		}
*/
		//Ia
		OfParent(&PHJointBase::Ia) += pXc_Mat_cXp(Ia - (Ia * smat * Ia) * (1 / dot_s_Ia_s));
		//Za
		OfParent(&PHJointBase::Za) += pXc_Vec(Z_plus_Ia_c + (Ia_s * (torque - dot_s_Z_plus_Ia_c)) / dot_s_Ia_s);
	}
}

void PHJoint1D::CalcAccel(double dt){
	//非ルートノードの場合
	if(GetParent()){
		GetParent()->CalcAccel(dt);	//	親の加速度を計算
		a_p = cXp_Vec(OfParent(&PHJointBase::a));
		//加速度を計算
		accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
		//重心周りの加速度(子ノードの積分で使用する)
		a = a_p + c + accel * s;
	}else{	//ルートノードの場合
		if(solid){	//	ルートノードが剛体の場合
			//加速度を計算
			a = (Ia.inv() * Za) * -1;
		}else{	//if(frame)
			a.clear();
		}
	}
}
void PHJoint1D::ClearTorqueRecursive(){
	PHJointBase::ClearTorqueRecursive();
	torque = 0;
}

////////////////////////////////////////////////////////
//・関節加速度計算・数値積分
//・加速度計算
void PHJoint1D::Integrate(double dt)
{
	//非ルートノードの場合
	if(GetParent()){
		a_p = cXp_Vec(OfParent(&PHJointBase::a));
		bool bOutOfRange = false;
		//可動範囲制限が有効な場合
		if(!(maxPosition == 0.0 && minPosition == 0.0)){
			if(maxPosition > minPosition){
				if(position >= maxPosition && velocity > 0.0){
					accel = 0;
					velocity *= -0.2f;				//速度を０に
					position = maxPosition;
					bOutOfRange = true;
				}
				else if(position <= minPosition && velocity < 0.0){
					accel = 0;
					velocity *= -0.2f;
					position = minPosition;
					bOutOfRange = true;
				}
			}
		}
		if(!bOutOfRange){
			//加速度を計算
			accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
			//位置を積分
			position += float ( (velocity + 0.5 * accel * dt) * dt );
			//回転関節の場合は[-π,π]
			LimitAngle(position);
			//速度を積分
			velocity += float(accel * dt);
		}
		//重心周りの加速度(子ノードの積分で使用する)
		a = a_p + c + accel * s;
		
		//位置・速度の伝播
		PropagateState();		
	}
	//ルートノードの場合
	else{
		//physical
		if(solid){
			//加速度を計算
			a = (Ia.inv() * Za) * -1;
			//速度変化量
			Vec3d dv_abs = R * svitem(a, 1) * dt;
			//角速度変化量
			Vec3d dw_abs = R * svitem(a, 0) * dt;

			//位置を更新
			p += (v_abs + 0.5 * dv_abs) * dt;
			//回転量を更新
			quat += quat.derivative(w_abs + 0.5 * dw_abs) * dt;
			quat.unitize();
			quat.to_matrix(R);

			//速度を更新
			v_abs += dv_abs;
			w_abs += dw_abs;
			v = quat.conjugated() * v_abs;
			w = quat.conjugated() * w_abs;

			Vec3d a_rot = R * svitem(a, 0);
			Vec3d a_trn = R * svitem(a, 1);

			/* 旧形式
			//加速度を積分して新しい速度を求める
			Vec3d w_new = (w_abs + a_rot * dt);
			Vec3d v_new = (v_abs + a_trn * dt);

			//速度を積分して位置を求める
			p += (v_abs + (0.5 * dt) * (R * a_trn)) * dt;

			double wnorm = w.norm();
			//クヲータニオンを微小回転
			if(wnorm > 1.0e-10){
				quat = Quaterniond::Rot(wnorm * dt, w_abs / wnorm) * quat;
				quat.unitize();
			}
			//これを回転行列に変換
			quat.to_matrix(R);

			//速度を更新
			v_abs = v_new;
			w_abs = w_new;
			v = R.trans() * v_abs;
			w = R.trans() * w_abs;
			*/
		}
		//non-physical
		else{	//if(frame)
			a.clear();
		}
	}

	//関連コンポーネントの位置、速度、関節変位、関節速度を更新
	if(solid){
#if 0	//	hase
		if (pos.norm() > 100){
			DSTR << "Strange position:" << std::endl;
			DSTR << p << std::endl;
			DSTR << R << solid->GetCenter() << std::endl;
			DSTR << GetParent()->p << std::endl;
		}
#endif
		solid->SetCenterPosition(p);
		solid->SetRotation(R);
		solid->SetVelocity(v_abs);
		solid->SetAngularVelocity(w_abs);
		solid->SetIntegrationMode(PHINT_NONE);
	}
	
	for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->Integrate(dt);
}

/////////////////////////////////////////////////////////////////////

class PHJointState: public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointState);
	//非ルートノードの状態
	float position;
	float velocity;
	//ルートノードの状態
	Vec3d p;
	Matrix3d R;
	Vec3d v_abs;
	Vec3d w_abs;
};
SGOBJECTIMP(PHJointState, SGBehaviorState);

void PHJoint1D::LoadState(const SGBehaviorStates& states){
	PHJointState* js = DCAST(PHJointState, states.GetNext());	
	//ロード
	if(GetParent()){
		position = js->position;
		velocity = js->velocity;
	}
	else{
		p = js->p;
		R = js->R;
		v_abs = js->v_abs;
		w_abs = js->w_abs;
	}
	PHJointBase::LoadState(states);
}

void PHJoint1D::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointState> js = new PHJointState;
	states.push_back(js);
	if(GetParent()){
		js->position = position;
		js->velocity = velocity;
	}
	else{
		js->p = p;
		js->R = R;
		js->v_abs = v_abs;
		js->w_abs = w_abs;
	}
	PHJointBase::SaveState(states);
}

typedef float FLOAT;
typedef Matrix3f Matrix3x3;
typedef Vec3f Vector;
DEF_RECORD(XJoint1D, {
	FLOAT minPosition;
	FLOAT maxPosition;
	FLOAT position;
	FLOAT velocity;
	GUID Guid(){ return WBGuid("AC150CC8-72CC-4f71-8BD3-72AAF2B31E97"); }
});

void PHJoint1D::SaveX(XJoint1D& x) const{
	x.minPosition = minPosition;
	x.maxPosition = maxPosition;
	x.position = position;
	x.velocity = velocity;
}
void PHJoint1D::LoadX(const XJoint1D& x){
	minPosition = x.minPosition;
	maxPosition = x.maxPosition;
	position = x.position;
	velocity = x.velocity;
}

//////////////////////////////////////////////////////////////////////
//PHJointSlider
SGOBJECTIMP(PHJointSlider, PHJoint1D);
void PHJointSlider::CompJointAxis()
{
	svitem(s, 0).clear();
	svitem(s, 1) = m3fRotationChild.Ez();
}

void PHJointSlider::CompRelativePosition(){
	TMatrixRow<3,3,float,float,float> work_around_for_bcb;
	pRc = m3fRotationParent * m3fRotationChild.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid)cp = GetParent()->solid->GetCenter();
	prc = (cRp * (v3fPositionParent - cp)) +
		m3fRotationChild.Ez() * position - (v3fPositionChild - solid->GetCenter());
}

void PHJointSlider::CompRelativeVelocity()
{
	pvc = m3fRotationChild.Ez() * velocity;
	pwc.clear();
}

void PHJointSlider::CompCoriolisAccel()
{
	Vec3d wp = cRp * OfParent(&PHJointBase::w);
	svitem(c, 0).clear();
	svitem(c, 1) = cross(wp, cross(wp, prc)) + 2.0 * cross(wp, m3fRotationChild.Ez() * velocity);
}

//////////////////////////////////////////////////////////////////////
//PHJointHinge
SGOBJECTIMP(PHJointHinge, PHJoint1D);
void PHJointHinge::CompJointAxis()
{
	svitem(s, 0) = m3fRotationChild.Ez();
	svitem(s, 1) = cross(m3fRotationChild.Ez(), -(v3fPositionChild - solid->GetCenter()));
}

void PHJointHinge::CompRelativePosition()
{
	pRc = m3fRotationParent * Matrix3d::Rot(position, 'z') * m3fRotationChild.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid) GetParent()->solid->GetCenter();
	prc = (cRp * (v3fPositionParent - cp)) - (v3fPositionChild - solid->GetCenter());
}

void PHJointHinge::CompRelativeVelocity()
{
	pvc = velocity * svitem(s, 1);
	pwc = velocity * svitem(s, 0);
}

void PHJointHinge::CompCoriolisAccel()
{
	Vec3d ud = m3fRotationChild.Ez() * velocity;
	Vec3d wp = cRp * OfParent(&PHJointBase::w);
	Vec3d tmp = cross(ud, (v3fPositionChild - solid->GetCenter()));
	svitem(c, 0) = cross(wp, ud);
	svitem(c, 1) = cross(wp, cross(wp, prc)) - 2.0 * cross(wp, tmp) - cross(ud, tmp);
}

/*			//ボールジョイントは回転ジョイント３軸で実現する
			UTRef<AC3Joint> joint[3];
			AC3JointInfo i;

			joint[0] = new AC3RevolutiveJoint(0, BallJoint.v3PositionParent, Vec3d(), Vec3d(1.0, 0.0, 0.0));
			joint[1] = new AC3RevolutiveJoint(0, Vec3d(), Vec3d(), Vec3d(0.0, 1.0, 0.0));
			joint[2] = new AC3RevolutiveJoint(pChild, Vec3d(), BallJoint.v3PositionChild, Vec3d(0.0, 0.0, 1.0));
			joint[0]->AddChild(joint[1]);
			joint[1]->AddChild(joint[2]);
			return _Connect(pParent, joint[0]);		
*/

DEF_RECORD(XJointHinge, {
	GUID Guid(){ return WBGuid("F0FEE14B-9F53-44b2-815A-93503C471474"); }
	XJointBase jointBase;
	XJoint1D joint1D;
});

class PHJointHingeLoader : public FIObjectLoader<PHJointHinge>
{
public:
	PHJointHingeLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Vector);
		db->REG_FIELD(Matrix3x3);
		db->REG_RECORD_PROTO(XJointBase);
		db->REG_RECORD_PROTO(XJoint1D);
		db->REG_RECORD_PROTO(XJointHinge);
	}
	bool LoadData(FILoadScene* ctx, PHJointHinge* j){
		XJointHinge x;
		ctx->docs.Top()->GetWholeData(x);
		j->PHJointBase::LoadX(x.jointBase);
		j->PHJoint1D::LoadX(x.joint1D);
		return true;
	}
};
class PHJointHingeSaver : public FIObjectSaver<PHJointHinge>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointHinge* jc){
		XJointHinge x;
		jc->PHJointBase::SaveX(x.jointBase);
		jc->PHJoint1D::SaveX(x.joint1D);
		doc->SetWholeData(x);
	}
};
DEF_REGISTER_BOTH(PHJointHinge);


DEF_RECORD(XJointSlider, {
	GUID Guid(){ return WBGuid("9A0DC32A-720C-415b-9EA5-0D5E79E7DC22"); }
	XJointBase jointBase;
	XJoint1D joint1D;
});
class PHJointSliderLoader : public FIObjectLoader<PHJointSlider>
{
public:
	PHJointSliderLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_FIELD(Vector);
		db->REG_FIELD(Matrix3x3);
		db->REG_RECORD(XJointBase);
		db->REG_RECORD(XJoint1D);
		db->REG_RECORD_PROTO(XJointSlider);
	}
	bool LoadData(FILoadScene* ctx, PHJointSlider* j){
		XJointSlider x;
		ctx->docs.Top()->GetWholeData(x);
		j->PHJointBase::LoadX(x.jointBase);
		j->PHJoint1D::LoadX(x.joint1D);
		return true;
	}
};
class PHJointSliderSaver : public FIObjectSaver<PHJointSlider>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointSlider* jc){
		XJointSlider x;
		jc->PHJointBase::SaveX(x.jointBase);
		jc->PHJoint1D::SaveX(x.joint1D);
		doc->SetWholeData(x);
	}
};
DEF_REGISTER_BOTH(PHJointSlider);

}
