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

	//�񃋁[�g�m�[�h
	if(GetParent()){	
		PropagateState();
		solid->SetCenterPosition(p);
		solid->SetRotation(R);
		solid->UpdateFrame();
	}
	//���[�g�m�[�h
	else{
		quat.from_matrix(R);
		v = R.trans() * v_abs;
		w = R.trans() * w_abs;
	}
}

////////////////////////////////////////////////////////
//�EIa: Articulated Inertia�v�Z
//�EZa: Articulated 0-accel force�v�Z
void PHJoint1D::CompArticulatedInertia(double dt)
{
	//	Ia, Za���N���A
	Ia.clear();
	Za.clear();

	//�q�m�[�h�����Ɍv�Z
	for(array_type::const_iterator it = Children().end(); it != Children().begin();)
		(*--it)->CompArticulatedInertia(dt);

	//�悭�g���o�Ă��鎮���L���b�V��
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

	//������L���b�V��
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
	//�e�m�[�h��Za,Ia�Ɏ�����Za,Ia��ςݏグ��
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
	//�񃋁[�g�m�[�h�̏ꍇ
	if(GetParent()){
		GetParent()->CalcAccel(dt);	//	�e�̉����x���v�Z
		a_p = cXp_Vec(OfParent(&PHJointBase::a));
		//�����x���v�Z
		accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
		//�d�S����̉����x(�q�m�[�h�̐ϕ��Ŏg�p����)
		a = a_p + c + accel * s;
	}else{	//���[�g�m�[�h�̏ꍇ
		if(solid){	//	���[�g�m�[�h�����̂̏ꍇ
			//�����x���v�Z
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
//�E�֐߉����x�v�Z�E���l�ϕ�
//�E�����x�v�Z
void PHJoint1D::Integrate(double dt)
{
	//�񃋁[�g�m�[�h�̏ꍇ
	if(GetParent()){
		a_p = cXp_Vec(OfParent(&PHJointBase::a));
		bool bOutOfRange = false;
		//���͈͐������L���ȏꍇ
		if(!(maxPosition == 0.0 && minPosition == 0.0)){
			if(maxPosition > minPosition){
				if(position >= maxPosition && velocity > 0.0){
					accel = 0;
					velocity *= -0.2f;				//���x���O��
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
			//�����x���v�Z
			accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
			//�ʒu��ϕ�
			position += float ( (velocity + 0.5 * accel * dt) * dt );
			//��]�֐߂̏ꍇ��[-��,��]
			LimitAngle(position);
			//���x��ϕ�
			velocity += float(accel * dt);
		}
		//�d�S����̉����x(�q�m�[�h�̐ϕ��Ŏg�p����)
		a = a_p + c + accel * s;
		
		//�ʒu�E���x�̓`�d
		PropagateState();		
	}
	//���[�g�m�[�h�̏ꍇ
	else{
		//physical
		if(solid){
			//�����x���v�Z
			a = (Ia.inv() * Za) * -1;
			//���x�ω���
			Vec3d dv_abs = R * svitem(a, 1) * dt;
			//�p���x�ω���
			Vec3d dw_abs = R * svitem(a, 0) * dt;

			//�ʒu���X�V
			p += (v_abs + 0.5 * dv_abs) * dt;
			//��]�ʂ��X�V
			quat += quat.derivative(w_abs + 0.5 * dw_abs) * dt;
			quat.unitize();
			quat.to_matrix(R);

			//���x���X�V
			v_abs += dv_abs;
			w_abs += dw_abs;
			v = quat.conjugated() * v_abs;
			w = quat.conjugated() * w_abs;

			Vec3d a_rot = R * svitem(a, 0);
			Vec3d a_trn = R * svitem(a, 1);

			/* ���`��
			//�����x��ϕ����ĐV�������x�����߂�
			Vec3d w_new = (w_abs + a_rot * dt);
			Vec3d v_new = (v_abs + a_trn * dt);

			//���x��ϕ����Ĉʒu�����߂�
			p += (v_abs + (0.5 * dt) * (R * a_trn)) * dt;

			double wnorm = w.norm();
			//�N���[�^�j�I���������]
			if(wnorm > 1.0e-10){
				quat = Quaterniond::Rot(wnorm * dt, w_abs / wnorm) * quat;
				quat.unitize();
			}
			//�������]�s��ɕϊ�
			quat.to_matrix(R);

			//���x���X�V
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

	//�֘A�R���|�[�l���g�̈ʒu�A���x�A�֐ߕψʁA�֐ߑ��x���X�V
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
	//�񃋁[�g�m�[�h�̏��
	float position;
	float velocity;
	//���[�g�m�[�h�̏��
	Vec3d p;
	Matrix3d R;
	Vec3d v_abs;
	Vec3d w_abs;
};
SGOBJECTIMP(PHJointState, SGBehaviorState);

void PHJoint1D::LoadState(const SGBehaviorStates& states){
	PHJointState* js = DCAST(PHJointState, states.GetNext());	
	//���[�h
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

/*			//�{�[���W���C���g�͉�]�W���C���g�R���Ŏ�������
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
