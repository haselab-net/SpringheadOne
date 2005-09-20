#include "Physics.h"
#pragma hdrstop

using namespace PTM;
namespace Spr{;

void LimitCycle(float& t){
	if (!_finite(t) || t > 1e6 || t < -1e6){
		if (t>0) t = (float)M_PI;
		else t= - (float)M_PI;
		return;
	}
	int times;
	if (t > M_PI){
		times = int((t+M_PI) / (2*M_PI));
	}else if (t < -M_PI){
		times = int((t-M_PI) / (2*M_PI));
	}else{
		return;
	}
	t -= (float)(times*(2*M_PI));
	assert(-M_PI < t && t < M_PI);
}

////////////////////////////////////////////////////////////////////////////////
//�c���[����
PHJoint* PHJoint::Search(PHSolid* s)
{
	if(solid == s)return this;
	PHJoint* pNode;
	array_type::const_iterator it;
	for(it = Children().begin(); it != Children().end(); it++){
		pNode = (*it)->Search(s);
		if(pNode)return pNode;
	}
	return 0;
}

int PHJoint::NumNodes()
{
	//fixed link�̃��[�g�m�[�h�͐����Ȃ�
	int nNodes = solid ? 1 : 0;
	for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
		nNodes += (*it)->NumNodes();
	return nNodes;
}
int PHJoint::NumJoints()
{
	int nNodes = GetParent() ? 1 : 0;
	for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
		nNodes += (*it)->NumJoints();
	return nNodes;
}

//////////////////////////////////////////////////////////
//	������
SGOBJECTIMP(PHJoint, SGObject);

PHJoint::PHJoint():solid(0), frame(0){}
PHJoint::PHJoint(PHSolid* s):solid(s), frame(0){}
PHJoint::PHJoint(SGFrame* f):solid(0), frame(f){}
PHJoint::PHJoint(PHSolid* s, const PHJointInfo& i):solid(s), frame(0), info(i){}

void PHJoint::Loaded(SGScene* scene)
{
	if(solid)solid->Loaded(scene);
	Reset();
	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->Loaded(scene);
	}
}

void PHJoint::Reset(){
	//solid�̏���ǂݍ���
	SetInfo(INFO_SOLID);
	
	//�񃋁[�g�m�[�h
	if(GetParent()){
		CompJointAxis();
		CompRelativePosition();
		CompRelativeVelocity();
		error = error_diff = error_int = 0;
		torque = 0;
	}
	//���[�g�m�[�h
	else{
		if(solid){
			p = solid->GetPosition();
			R = solid->GetRotation();
			v_abs = solid->GetVelocity();
			w_abs = solid->GetAngularVelocity();
			//PHJoint�͏d�S�����W���_�Ƃ��Ă���̂ł��̕ϊ�
			p += R * solid->GetCenter();
			v_abs += cross(w_abs, R * solid->GetCenter());
		}
		else{
			p = frame->GetPosition();
			R = frame->GetOrientation();
			v_abs.clear();
			w_abs.clear();
		}
		
		quat.from_matrix(R);
		v = R.trans() * v_abs;
		w = R.trans() * w_abs;
	}

	Ia.clear();
	Za.clear();

	//for(array_type::const_iterator it = Children().begin(); it != Children().end();)
	//	(*it++)->Reset();
}

//////////////////////////////////////////////////////////
//�E�؍\��
SGObject* PHJoint::ChildObject(size_t i){
	return Children()[i];
}

size_t PHJoint::NChildObjects(){
	return Children().size();
}

bool PHJoint::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		solid = (PHSolid*)o;
		return true;
	}
	if (DCAST(PHJoint, o)){
		((PHJoint*)o)->SetParent(this);
		return true;
	}
	return false;
}

bool PHJoint::DelChildObject(SGObject* o, SGScene* s){
	PHJoint* j = DCAST(PHJoint, o);
	for(unsigned i=0; i<Children().size(); ++i){
		if (Children()[i] == j){
			j->SetParent(NULL);
			return true;
		}
	}
	return false;
}

const UTTypeInfo** PHJoint::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHJoint::GetTypeInfoStatic(), NULL
	};
	return rv;
}

//////////////////////////////////////////////////////////
//�E���̍X�V
void PHJoint::SetInfo(PHJointInfoEntry entry, bool recursive, void* value)
{
	//solid�̏���ǂݍ���
	if(entry == INFO_SOLID){
		if(solid){
			m = solid->GetMass();
			I = solid->GetInertia();
		}
		else{
			m = 0.0;
			I.clear();
		}
		Ii.clear();
		smitem(Ii, 0, 1) = Matrix3d::Diag(m, m, m);
		smitem(Ii, 1, 0) = I;
	}

	if(!GetParent() || !value)return;

	switch(entry){
	case INFO_POSITIONPARENT: info.v3fPositionParent = *(Vec3f*)value; break;
	case INFO_ROTATIONPARENT: info.m3fRotationParent = *(Matrix3f*)value; break;
	case INFO_POSITIONCHILD: info.v3fPositionChild = *(Vec3f*)value; break;
	case INFO_ROTATIONCHILD: info.m3fRotationChild = *(Matrix3f*)value; break;
	case INFO_MAXTORQUE: info.fMaxTorque = max(0.0f, *(float*)value); break;
	case INFO_MINPOSITION: info.fMinPosition = *(float*)value; break;
	case INFO_MAXPOSITION: info.fMaxPosition = *(float*)value; break;
	case INFO_INPUTTYPE: info.nInputType = *(DWORD*)value; break;
	case INFO_INPUT: info.fInput = *(float*)value; break;
	case INFO_PVALUE: info.fPValue = max(0.0f, *(float*)value); break;
	case INFO_IVALUE: info.fIValue = max(0.0f, *(float*)value); break;
	case INFO_DVALUE: info.fDValue = max(0.0f, *(float*)value); break;
	default:;
	}

	//�֐߈ʒu��ύX�����ꍇ
	switch(entry){
	case INFO_POSITIONPARENT:
	case INFO_ROTATIONPARENT:
	case INFO_POSITIONCHILD:
	case INFO_ROTATIONCHILD:
		CompJointAxis();
		CompRelativePosition();
		//TODO:�����ŉ��ʃm�[�h�̈ʒu�����ׂčX�V
	}

	//�ċA�w��̏ꍇ
	if(recursive)
		for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
			(*it)->SetInfo(entry, recursive, value);
}

/////////////////////////////////////////////////////////////////////
//����

void PHJoint::SetPosition(double pos)
{
	info.nInputType = TYPE_POSITION;
	info.fInput = (float)pos;
	//��]�֐߂̏ꍇ�͓��͒l��[-��,��]�͈̔͂ɐ�������
	if(info.nType == TYPE_HINGE){
		LimitCycle(info.fInput);
	}
}
void PHJoint::SetVelocity(double vel)
{
	info.nInputType = TYPE_VELOCITY;
	info.fInput = (float)vel;
}
void PHJoint::SetAccel(double accel)
{
	info.nInputType = TYPE_ACCEL;
	info.fInput = (float)accel;
}
void PHJoint::SetTorque(double torque)
{
	info.nInputType = TYPE_TORQUE;
	info.fInput = (float)torque;
}

void PHJoint::CompTorque(double dt)
{
	float e;
	switch(info.nInputType){
	case TYPE_TORQUE:
		torque = info.fInput;
		break;
	case TYPE_ACCEL:
		//����̉����x���o�����߂̃g���N���v�Z
		torque = info.fInput * dot_s_Ia_s + dot_s_Z_plus_Ia_c + svdot(s, Ia * a_p);
		break;
	case TYPE_VELOCITY:
		//���x��PID����
		e = info.fInput - info.fVelocity;
		error_diff = (e - error) / dt;
		error_int += e;
		error = e;
		torque = info.fPValue * error + info.fIValue * error_int + info.fDValue * error_diff;
		break;
	case TYPE_POSITION:
		//�ʒu��PID����
		e = info.fInput - info.fPosition;
		if(info.nType == TYPE_HINGE){
			LimitCycle(e);
		}
		error_diff = (e - error) / dt;
		error_int += e;
		error = e;
		torque = info.fPValue * error + info.fIValue * error_int + info.fDValue * error_diff;
		break;
	}

	//�ő�g���N�ŃN�����v
	if(info.fMaxTorque >= 0.0){
		if(torque >  info.fMaxTorque)torque =  info.fMaxTorque;
		if(torque < -info.fMaxTorque)torque = -info.fMaxTorque;
	}
}

/////////////////////////////////////////////////////////////////////
//Featherstone's algorithm

void PHJoint::Dynamics(double dt)
{
	CompArticulatedInertia(dt);
	Integrate(dt);
}

////////////////////////////////////////////////////////
//�EArticulated Inertia�v�Z
//�EArticulated 0-accel force�v�Z
void PHJoint::CompArticulatedInertia(double dt)
{
	//�q�m�[�h�����Ɍv�Z
	for(array_type::const_iterator it = Children().end(); it != Children().begin();)
		(*--it)->CompArticulatedInertia(dt);

	//�悭�g���o�Ă��鎮���L���b�V��
	rcross = crossmat(prc);
	rpcross = crossmat(pRc * prc);
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

	if(GetParent()){
		//�R���I�������x
		CompCoriolisAccel();
		//�g���N
		CompTorque(dt);
	}

	//������L���b�V��
	smat = svmat(s, s);
	Ia_c = Ia * c;
	Ia_s = Ia * s;
	Z_plus_Ia_c = Za + Ia_c;
	dot_s_Ia_s = svdot(s, Ia_s);
	dot_s_Z_plus_Ia_c = svdot(s, Z_plus_Ia_c);
		
	//�e�m�[�h��Za,Ia�Ɏ�����Za,Ia��ςݏグ��
	if(GetParent()){
		//Ia
		GetParent()->Ia += pXc_Mat_cXp(Ia - (Ia * smat * Ia) * (1 / dot_s_Ia_s));
		//Za
		GetParent()->Za += pXc_Vec(Z_plus_Ia_c + (Ia_s * (torque - dot_s_Z_plus_Ia_c)) / dot_s_Ia_s);
	}
}

////////////////////////////////////////////////////////
//�E�֐߉����x�v�Z�E���l�ϕ�
//�E�����x�v�Z
void PHJoint::Integrate(double dt)
{
	//�񃋁[�g�m�[�h�̏ꍇ
	if(GetParent()){
		a_p = cXp_Vec(GetParent()->a);
		bool bOutOfRange = false;
		//���͈͐������L���ȏꍇ
		/*
		if(info.fMaxPosition > info.fMinPosition){
			if(info.fPosition >= info.fMaxPosition && info.fVelocity > 0.0){
				accel = 0;
				info.fVelocity = 0;				//���x���O��
				info.fPosition = info.fMaxPosition;
				bOutOfRange = true;
			}
			else if(info.fPosition <= info.fMinPosition && info.fVelocity < 0.0){
				accel = 0;
				info.fVelocity = 0;
				info.fPosition = info.fMinPosition;
				bOutOfRange = true;
			}
		}*/
		if(!bOutOfRange){
			//�����x���v�Z
			accel = (torque - svdot(s, Ia * a_p) - dot_s_Z_plus_Ia_c) / dot_s_Ia_s;	
			//�ʒu��ϕ�
			info.fPosition += float ( (info.fVelocity + 0.5 * accel * dt) * dt );
			//��]�֐߂̏ꍇ��[-��,��]
			if(info.nType == TYPE_HINGE){
				LimitCycle(info.fPosition);
			}
			//���x��ϕ�
			info.fVelocity += float(accel * dt);
		}
		//�d�S����̉����x
		a = a_p + c + accel * s;
		
		//�e�m�[�h�Ƃ̑��Έʒu�A�p�����v�Z
		CompRelativePosition();
		
		//���Έʒu�����Έʒu���v�Z
		R = GetParent()->R * pRc;
		p = GetParent()->p + R * prc;
		
		//�e�m�[�h�̑��x�E�p���x�{�֐ߑ��x�@���@���x�E�p���x
		w = cRp * GetParent()->w;
		v = cRp * GetParent()->v + cross(w, prc);
		CompRelativeVelocity();
		v += pvc;
		w += pwc;
		v_abs = R * v;
		w_abs = R * w;
	}
	//���[�g�m�[�h�̏ꍇ
	else{
		//physical
		if(solid){
			//�����x���v�Z
			a = (Ia.inv() * Za) * -1;
			Vec3d a_rot = R * svitem(a, 0);
			Vec3d a_trn = R * svitem(a, 1);

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
		}
		//non-physical
		else{	//if(frame)
			a.clear();
		}
	}

	//�֘A�R���|�[�l���g�̈ʒu�A���x�A�֐ߕψʁA�֐ߑ��x���X�V
	if(solid){
		Vec3f pos = p - R * solid->GetCenter();
#if 0	//	hase
		if (pos.norm() > 100){
			DSTR << "Strange position:" << pos << std::endl;
			DSTR << p << std::endl;
			DSTR << R << solid->GetCenter() << std::endl;
			DSTR << GetParent()->p << std::endl;
		}
#endif
		solid->SetPosition(pos);
		solid->SetRotation(R);
		solid->SetVelocity(v_abs - cross(w_abs, R * solid->GetCenter()));
		solid->SetAngularVelocity(w_abs);
		solid->IntegrationMode = PHINT_NONE;

		R = solid->GetRotation();
	}

	//���̃��[�v�̂��߂ɃN���A
	Ia.clear();
	Za.clear();
	
	for(array_type::const_iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->Integrate(dt);
}

//////////////////////////////////////////////////////////////////////
//PHSliderJoint
PHSliderJoint::PHSliderJoint(PHSolid* solid, const PHJointInfo& info):
	PHJoint(solid, info)
{
}

void PHSliderJoint::CompJointAxis()
{
	svitem(s, 0).clear();
	svitem(s, 1) = info.m3fRotationChild.Ez();
}

void PHSliderJoint::CompRelativePosition(){
	TMatrixRow<3,3,float,float,float> work_around_for_bcb;
	pRc = info.m3fRotationParent * info.m3fRotationChild.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid)cp = GetParent()->solid->GetCenter();
	prc = (cRp * (info.v3fPositionParent - cp)) +
		info.m3fRotationChild.Ez() * info.fPosition - (info.v3fPositionChild - solid->GetCenter());
}

void PHSliderJoint::CompRelativeVelocity()
{
	pvc = info.m3fRotationChild.Ez() * info.fVelocity;
	pwc.clear();
}

void PHSliderJoint::CompCoriolisAccel()
{
	Vec3d wp = cRp * ((PHSliderJoint*)GetParent())->w;
	svitem(c, 0).clear();
	svitem(c, 1) = cross(wp, cross(wp, prc)) + 2.0 * cross(wp, info.m3fRotationChild.Ez() * info.fVelocity);
}

//////////////////////////////////////////////////////////////////////
//PHHingeJoint
PHHingeJoint::PHHingeJoint(PHSolid* solid, const PHJointInfo& info):
	PHJoint(solid, info)
{
}

void PHHingeJoint::CompJointAxis()
{
	svitem(s, 0) = info.m3fRotationChild.Ez();
	svitem(s, 1) = cross(info.m3fRotationChild.Ez(), -(info.v3fPositionChild - solid->GetCenter()));
}

void PHHingeJoint::CompRelativePosition()
{
	pRc = info.m3fRotationParent * Matrix3d::Rot(info.fPosition, 'z') * info.m3fRotationChild.trans();
	cRp = pRc.trans();
	Vec3d cp;
	if(GetParent()->solid)GetParent()->solid->GetCenter();
	prc = (cRp * (info.v3fPositionParent - cp)) - (info.v3fPositionChild - solid->GetCenter());
}

void PHHingeJoint::CompRelativeVelocity()
{
	pvc = info.fVelocity * svitem(s, 1);
	pwc = info.fVelocity * svitem(s, 0);
}

void PHHingeJoint::CompCoriolisAccel()
{
	Vec3d ud = info.m3fRotationChild.Ez() * info.fVelocity;
	Vec3d wp = cRp * ((PHHingeJoint*)GetParent())->w;
	Vec3d tmp = cross(ud, (info.v3fPositionChild - solid->GetCenter()));
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

////////////////////////////////////////////////////////////////////

SGOBJECTIMP(PHJointEngine, SGBehaviorEngine);
void PHJointEngine::Loaded(SGScene* scene){
	root->Loaded(scene);
}
void PHJointEngine::Step(SGScene*, float dt, unsigned int count)
{
	root->Dynamics(dt);
}

SGObject* PHJointEngine::ChildObject(size_t i){
	return root;
}
size_t PHJointEngine::NChildObjects(){
	return root ? 1 : 0;
}
bool PHJointEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		root->solid = (PHSolid*)o;
		return true;
	}
	if (DCAST(SGFrame, o)){
		root->frame= (SGFrame*)o;
		return true;
	}
	if (DCAST(PHJoint, o)){
		((PHJoint*)o)->SetParent(root);
		return true;
	}
	return false;
}
bool PHJointEngine::DelChildObject(SGObject* o, SGScene* s){
	if (root == o){
		root = NULL;
		return true;
	}
	return false; 
}
const UTTypeInfo** PHJointEngine::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHJoint::GetTypeInfoStatic(), NULL
	};
	return rv;
}

//////////////////////////////////////////////////////////////////////
class PHJointEngineLoader : public FIObjectLoader<PHJointEngine>{
public:
	virtual void LoadData(FILoadScene* ctx, UTRef<PHJointEngine>& je){
		je = new PHJointEngine;
		je->root = new PHJoint;
	}
};

class PHJointEngineSaver : public FIBaseSaver
{
public:
	UTString GetType() const{ return "PHJointEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHJointEngine* je = (PHJointEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("JointEngine", je);
		//�ڑ��Ώۂ�ۑ�
		if (je->root->solid){
			doc->AddChild(ctx->CreateDocNode("Solid", je->root->solid, true));
		}else if(je->root->frame){
			doc->AddChild(ctx->CreateDocNode("Frame", je->root->frame, true));
		}

		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(unsigned i=0; i<je->root->Children().size(); ++i){
			ctx->SaveRecursive(je->root->Children()[i]);
		}
		ctx->docs.pop_back();
	}
};

///////////////////////////////////////////////////////////////////////

//����ł�Joint�^�O�̓��e�ɂ����PHJoint�̔h���N���X��new����݌v�Ȃ̂ŁA
//FIObjectLoader�͎g�p�ł��Ȃ�
class PHJointLoader : public FIObjectLoader<PHJoint>
{
protected:
	void LoadData(FILoadScene* ctx, UTRef<PHJoint>& joint){
		FIDocNodeBase* doc = ctx->docs.Top();
		
		//�֐߂̎�ނƃp�����[�^
		PHJointInfo info;
		doc->GetData(info);
		
		switch((int)(info.nType)){
		case TYPE_HINGE:	joint = new PHHingeJoint(0, info);	break;
		case TYPE_SLIDER:	joint = new PHSliderJoint(0, info);	break;
		default: ;
		}
	}
};

class PHJointSaver : public FIBaseSaver
{
protected:
	UTString GetType()const{return "PHJoint";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHJoint* joint = (PHJoint*)arg;
		FIDocNodeBase* docJoint = ctx->CreateDocNode("Joint", joint);
		docJoint->SetData(joint->info);
		ctx->docs.back()->AddChild(docJoint);

		//�ڑ��Ώۂ�ۑ�
		if (joint->solid){
			docJoint->AddChild(ctx->CreateDocNode("Solid", joint->solid, true));
		}else if(joint->frame){
			docJoint->AddChild(ctx->CreateDocNode("Frame", joint->frame, true));
		}
		
		//	�q�W���C���g�̃Z�[�u
		ctx->docs.push_back(docJoint);
		for(PHJoint::array_type::const_iterator it = joint->Children().begin(); it != joint->Children().end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};

DEF_REGISTER_BOTH(PHJoint);
DEF_REGISTER_BOTH(PHJointEngine);

}
