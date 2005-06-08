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
 - Ia: Articulated Inertia�v�Z
 - Za: Articulated 0-accel force�v�Z
*/
void PHJoint1D::CompArticulatedInertia(double dt){
	//	�q�W���C���g��Ia,Za�̌v�Z�D
	PHJointBase::CompArticulatedInertia(dt);
	//	�L���b�V��
	Ia_s = Ia * s;
	dot_s_Ia_s = svdot(s, Ia_s);
	dot_s_Z_plus_Ia_c = svdot(s, Z_plus_Ia_c);
	
	//�e�m�[�h��Za,Ia�Ɏ�����Za,Ia��ςݏグ��
	//Ia
	OfParent(&PHJoint1D::Ia) += pXc_Mat_cXp(Ia - (Ia * svmat(s, s) * Ia) * (1 / dot_s_Ia_s));
	//Za
	OfParent(&PHJoint1D::Za) += pXc_Vec(Z_plus_Ia_c + (Ia_s * (torque - dot_s_Z_plus_Ia_c)) / dot_s_Ia_s);

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
	GetParent()->CalcAccel(dt);	//	�e�̉����x���v�Z
	a_p = cXp_Vec(OfParent(&PHJoint1D::a));
	//�����x���v�Z
	accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
	//�d�S����̉����x(�q�m�[�h�̐ϕ��Ŏg�p����)
	a = a_p + c + accel * s;
}

void PHJoint1D::ClearTorqueRecursive(){
	PHJointBase::ClearTorqueRecursive();	//	�q�W���C���g���N���A
	torque = 0;
}

////////////////////////////////////////////////////////
//�E�֐߉����x�v�Z�E���l�ϕ�
//�E�����x�v�Z
void PHJoint1D::Integrate(SGScene* scene){
	double dt = scene->GetTimeStep();
	a_p = cXp_Vec(OfParent(&PHJoint1D::a));
	//���͈͐������L���ȏꍇ
	if(!(maxPosition == 0.0 && minPosition == 0.0)){
		bool bOutOfRange = false;
		double prop;
		double K=.2;
		double B=.2;
		double massFactor = MassFactor();
		K *= massFactor/(dt*dt);
		B *= massFactor/dt;
		if(maxPosition > minPosition){
			if(position >= maxPosition && velocity > 0.0){
				prop = maxPosition - position;
				bOutOfRange = true;
			}
			else if(position <= minPosition && velocity < 0.0){
				prop = minPosition - position;
				bOutOfRange = true;
			}
		}
		if(bOutOfRange){
			AddTorque(K*prop -B*velocity);
		}
	}
	//�����x���v�Z
	accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
	if (intType == SYMPLETIC){
		//���x��ϕ�
		velocity += float(accel * dt);
		//�ʒu��ϕ�
		position += float (velocity * dt);
		//��]�֐߂̏ꍇ��[-��,��]
		LimitAngle(position);
	}else{
		//�ʒu��ϕ�
		position += float ( (velocity + 0.5 * accel * dt) * dt );
		//��]�֐߂̏ꍇ��[-��,��]
		LimitAngle(position);
		//���x��ϕ�
		velocity += float(accel * dt);
	}
	//�d�S����̉����x(�q�m�[�h�̐ϕ��Ŏg�p����)
	a = a_p + c + accel * s;

	velocity *= scene->GetVelocityLossPerStep();
	if (velocity > PHJOINT_MAX_VELOCITY){
		velocity = PHJOINT_MAX_VELOCITY;
		DSTR << "The velocity of " << GetName() << ":" << velocity << "was limited";
	}
	if (velocity < -PHJOINT_MAX_VELOCITY){
		velocity = -PHJOINT_MAX_VELOCITY;
		DSTR << "The velocity of " << GetName() << ":" << velocity << "was limited";
	}

/*	if (abs(velocity) > 2*M_PI*30){
		DSTR << "Joint " << GetName() << " has velocity of " << velocity << std::endl;
		DebugBreak();
	}
*/	//�ʒu�E���x�̓`�d
	PropagateState();
	//�֘A�R���|�[�l���g�̈ʒu�A���x�A�֐ߕψʁA�֐ߑ��x���X�V
	PHJointBase::Integrate(scene);
}

/////////////////////////////////////////////////////////////////////

class PHJointState1D: public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointState1D);
	//�񃋁[�g�m�[�h�̏��
	double position;
	double velocity;
	double torque;
	double accel;
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
	x.minPosition = (float)minPosition;
	x.maxPosition = (float)maxPosition;
	x.position = (float)position;
	x.velocity = (float)velocity;
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
	svitem(s, 1) = cRj.Ez();
}

void PHJointSlider::CompRelativePosition(){
	TMatrixRow<3,3,float,float,float> work_around_for_bcb;
	pRc = pRj * cRj.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid)cp = GetParent()->solid->GetCenter();
	prc = (cRp * (prj - cp)) +
		cRj.Ez() * (float)position - (crj - solid->GetCenter());
}

void PHJointSlider::CompRelativeVelocity()
{
	pvc = cRj.Ez() * (float)velocity;
	pwc.clear();
}

void PHJointSlider::CompCoriolisAccel()
{
	Vec3d wp = cRp * OfParent(&PHJointSlider::w);
	svitem(c, 0).clear();
	svitem(c, 1) = cross(wp, cross(wp, prc)) + 2.0 * cross(wp, cRj.Ez() * velocity);
}
void PHJointSlider::Loaded(SGScene* scene){
	PHJoint1D::Loaded(scene);
	double pMass = 1e10;
	if (GetParent()->solid){
		pMass = GetParent()->solid->GetMass();
	}
	double m1 = pMass;
	double m2 = solid->GetMass();
	massFactor = (m1*m2)/(m1+m2);
}
double PHJointSlider::MassFactor(){
	return massFactor;
}

//////////////////////////////////////////////////////////////////////
//PHJointHinge
SGOBJECTIMP(PHJointHinge, PHJoint1D);
void PHJointHinge::CompJointAxis()
{
	svitem(s, 0) = cRj.Ez();
	svitem(s, 1) = cross(cRj.Ez(), -(crj - solid->GetCenter()));
}

void PHJointHinge::CompRelativePosition()
{
	pRc = pRj * Matrix3d::Rot(position, 'z') * cRj.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid) cp = GetParent()->solid->GetCenter();
	prc = (cRp * (prj - cp)) - (crj - solid->GetCenter());
}

void PHJointHinge::CompRelativeVelocity()
{
	pvc = velocity * svitem(s, 1);
	pwc = velocity * svitem(s, 0);
}

void PHJointHinge::CompCoriolisAccel()
{
	Vec3d ud = cRj.Ez() * (float)velocity;
	Vec3d wp = cRp * OfParent(&PHJointHinge::w);
	Vec3d tmp = cross(ud, (crj - solid->GetCenter()));
	svitem(c, 0) = cross(wp, ud);
	svitem(c, 1) = cross(wp, cross(wp, prc)) - 2.0 * cross(wp, tmp) - cross(ud, tmp);
}
void PHJointHinge::Loaded(SGScene* s){
	PHJoint1D::Loaded(s);
	Vec3d axis = pRj.Ez();
	Matrix3d pInertia = Matrix3d::Unit() * 1e10;
	double pMass = 1e10;
	Vec3d pCenter;
	if (GetParent()->solid){
		pMass = GetParent()->solid->GetMass();
		pInertia = GetParent()->solid->GetInertia();
		pCenter = GetParent()->solid->GetCenter();
	}
	Vec3d psrj = -pCenter+prj;
	double i1 = (psrj - axis*psrj*axis).square() * pMass
		+ (pRj * pInertia * pRj.inv())[2][2];
	axis = cRj;
	Vec3d csrj = -solid->GetCenter()+crj;
	double i2 = (csrj - axis*csrj*axis).square() * solid->GetMass()
		+ (cRj * solid->GetInertia() * cRj.inv())[2][2];
	massFactor = (i1*i2)/(i1+i2);
}
double PHJointHinge::MassFactor(){
	return massFactor;
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
