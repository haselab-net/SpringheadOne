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
	PHJointBase::Reset();
}
/*
 - Ia: Articulated Inertia計算
 - Za: Articulated 0-accel force計算
*/
void PHJoint1D::CompArticulatedInertia(double dt){
	//	子ジョイントのIa,Zaの計算．
	PHJointBase::CompArticulatedInertia(dt);
	//	キャッシュ
	Ia_s = Ia * s;
	dot_s_Ia_s = svdot(s, Ia_s);
	dot_s_Z_plus_Ia_c = svdot(s, Z_plus_Ia_c);
	
	//親ノードのZa,Iaに自分のZa,Iaを積み上げる
	//Ia
	OfParent(&PHJointBase::Ia) += pXc_Mat_cXp(Ia - (Ia * svmat(s, s) * Ia) * (1 / dot_s_Ia_s));
	//Za
	OfParent(&PHJointBase::Za) += pXc_Vec(Z_plus_Ia_c + (Ia_s * (torque - dot_s_Z_plus_Ia_c)) / dot_s_Ia_s);

/*	if (abs(dot_s_Ia_s) < 1e-1){
		DSTR << Ia << (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) << std::endl;
	}
*/
/*	static bReport;
	if (abs(Ia.det()) < 1e-2f || bReport || abs(accel)>1e4){
		DSTR << GetName() << "a:" << accel << "v:" << info.velocity << " Ia.det:" << Ia.det();
		DSTR << " dot_s_Ia_s:" << dot_s_Ia_s;
		DSTR << " Ii.det:" << Ii.det();
		DSTR << " Za:" << Za.norm() << std::endl;
		bReport = true;
	}
*/
}

void PHJoint1D::CalcAccel(double dt){
	GetParent()->CalcAccel(dt);	//	親の加速度を計算
	a_p = cXp_Vec(OfParent(&PHJointBase::a));
	//加速度を計算
	accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
	//重心周りの加速度(子ノードの積分で使用する)
	a = a_p + c + accel * s;
}

void PHJoint1D::ClearTorqueRecursive(){
	PHJointBase::ClearTorqueRecursive();	//	子ジョイントをクリア
	torque = 0;
}

////////////////////////////////////////////////////////
//・関節加速度計算・数値積分
//・加速度計算
void PHJoint1D::Integrate(double dt){
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
#if 0
		//	for DEBUG
		if (accel > 100){
			DSTR << GetName() << " accel:" << accel << "vel: " << velocity << std::endl;
		}
#endif
	}
	//重心周りの加速度(子ノードの積分で使用する)
	a = a_p + c + accel * s;
	
	//位置・速度の伝播
	PropagateState();
	//関連コンポーネントの位置、速度、関節変位、関節速度を更新
	PHJointBase::Integrate(dt);
}

/////////////////////////////////////////////////////////////////////

class PHJointState1D: public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointState1D);
	//非ルートノードの状態
	float position;
	float velocity;
	float torque;
	float accel;
};
SGOBJECTIMP(PHJointState1D, SGBehaviorState);
void PHJoint1D::LoadState(const SGBehaviorStates& states){
	PHJointState1D* js = DCAST(PHJointState1D, states.GetNext());	
	position = js->position;
	velocity = js->velocity;
	accel = js->accel;
	torque = js->torque;
	PHJointBase::LoadState(states);
}

void PHJoint1D::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointState1D> js = new PHJointState1D;
	states.push_back(js);
	js->position = position;
	js->velocity = velocity;
	js->accel = accel;
	js->torque = torque;
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
	if(GetParent()->solid) cp = GetParent()->solid->GetCenter();
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
