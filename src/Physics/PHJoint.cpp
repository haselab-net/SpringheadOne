#include "Physics.h"
#pragma hdrstop
#include "PHJointPid.h"
#include <iomanip>

using namespace PTM;
namespace Spr{;

////////////////////////////////////////////////////////////////////////////////
//�c���[����
PHJointBase* PHJointBase::Search(PHSolid* s)
{
	if(solid == s)return this;
	PHJointBase* pNode;
	array_type::const_iterator it;
	for(it = Children().begin(); it != Children().end(); it++){
		pNode = (*it)->Search(s);
		if(pNode)return pNode;
	}
	return 0;
}
//////////////////////////////////////////////////////////
//	������
SGOBJECTIMPABST(PHJointBase, SGObject);

PHJointBase::PHJointBase():solid(0), frame(0){
	intType = SYMPLETIC;
//	intType = ANALYTIC;
}

void PHJointBase::Loaded(SGScene* scene)
{
	//solid��frame�����w��̏ꍇ�̏���
	if(!solid && !frame){
		solid = new PHSolid;
		solid->SetMass(0.1f);
		solid->SetInertia(0.1f * Matrix3d());
		solid->SetFrame(new SGFrame);
	}
	if(solid)solid->Loaded(scene);
	
	//���ϐ��̏�����
	UpdateSolid();
	if(GetParent()){
		CompJointAxis();
	}else{
		if(solid){
			p = solid->GetCenterPosition();
			R = solid->GetRotation();
			v_abs = solid->GetVelocity();
			w_abs = solid->GetAngularVelocity();
		}else{
			p = frame->GetPosition();
			R = frame->GetRotation();
			v_abs.clear();
			w_abs.clear();
		}		
	}
	Ia.clear();
	Za.clear();

	Reset();

	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->Loaded(scene);
	}
}

//////////////////////////////////////////////////////////
//�E�؍\��
SGObject* PHJointBase::ChildObject(size_t i){
	return Children()[i];
}
SGObject* PHJointBase::ReferenceObject(size_t i){
	if (i!=0) return NULL;
	if (solid) return solid;
	return frame;
}
size_t PHJointBase::NReferenceObjects(){
	if (frame || solid) return 1; 
	return 0;
}
size_t PHJointBase::NChildObjects(){
	return Children().size();
}

bool PHJointBase::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		solid = (PHSolid*)o;
		return true;
	}
	if (DCAST(PHJointBase, o)){
		((PHJointBase*)o)->SetParent(this);
		return true;
	}
	return s->AddChildObject(o, s);
}

bool PHJointBase::DelChildObject(SGObject* o, SGScene* s){
	PHJointBase* j = DCAST(PHJointBase, o);
	for(unsigned i=0; i<Children().size(); ++i){
		if (Children()[i] == j){
			j->SetParent(NULL);
			return true;
		}
	}
	return false;
}

const UTTypeInfo** PHJointBase::ChildCandidates(){
	static const UTTypeInfo* rv[] = {
		PHJointBase::GetTypeInfoStatic(), NULL
	};
	return rv;
}

void PHJointBase::UpdateSolid(){
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
void PHJointBase::UpdateJointPosture(){
	CompJointAxis();
	CompRelativePosition();
	//TODO:�����ŉ��ʃm�[�h�̈ʒu�����ׂčX�V
}
void PHJointBase::CompArticulatedInertia(double dt){
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
	Ia_c = Ia * c;
	Z_plus_Ia_c = Za + Ia_c;
}
void PHJointBase::CompCoriolisAccelRecursive(double dt){
	if (GetParent()) CompCoriolisAccel();
	for(array_type::const_iterator it = Children().end(); it != Children().begin();){
		(*--it)->CompCoriolisAccelRecursive(dt);
	}
}

void PHJointBase::ClearTorqueRecursive(){
	for(array_type::iterator it = Children().begin(); it != Children().end(); ++it){
		(*it)->ClearTorqueRecursive();
	}
}

void PHJointBase::Integrate(double dt){
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

void PHJointBase::PropagateState()
{
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
void PHJointBase::Reset(){
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

void PHJointBase::LoadState(const SGBehaviorStates& states){
	Reset();
	//	�q�W���C���g�����[�h
	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->LoadState(states);
	}
}

void PHJointBase::SaveState(SGBehaviorStates& states) const{
	for(unsigned i=0; i<Children().size(); ++i){
		Children()[i]->SaveState(states);
	}
}
void PHJointBase::SaveX(XJointBase& x) const{
	x.v3fPositionChild = v3fPositionChild;
	x.v3fPositionParent = v3fPositionParent;
	x.m3fRotationChild = m3fRotationChild;
	x.m3fRotationParent = m3fRotationParent;
}
void PHJointBase::LoadX(const XJointBase& x){
	v3fPositionChild = x.v3fPositionChild;
	v3fPositionParent = x.v3fPositionParent;
	m3fRotationChild = x.m3fRotationChild;
	m3fRotationParent = x.m3fRotationParent;
}


//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointRoot, PHJointBase);
void PHJointRoot::Reset(){
	PHJointBase::Reset();
}

class PHJointStateRoot: public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointStateRoot);
	//���[�g�m�[�h�̏��
	Vec3d p;
	Matrix3d R;
	Vec3d v_abs;
	Vec3d w_abs;
};
SGOBJECTIMP(PHJointStateRoot, SGBehaviorState);
void PHJointRoot::LoadState(const SGBehaviorStates& states){
	PHJointStateRoot* js = DCAST(PHJointStateRoot, states.GetNext());	
	//���[�h
	p = js->p;
	R = js->R;
	v_abs = js->v_abs;
	w_abs = js->w_abs;
	PHJointBase::LoadState(states);
}
void PHJointRoot::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointStateRoot> js = new PHJointStateRoot;
	states.push_back(js);
	js->p = p;
	js->R = R;
	js->v_abs = v_abs;
	js->w_abs = w_abs;
	PHJointBase::SaveState(states);
}
void PHJointRoot::CalcAccel(double dt){
	if(solid){	//	���[�g�m�[�h�����̂̏ꍇ
		//�����x���v�Z
		a = (Ia.inv() * Za) * -1;
	}else{	//if(frame)
		a.clear();
	}
}
void PHJointRoot::Integrate(double dt){
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
	}else{		//non-physical
		a.clear();
	}
	//�֘A�R���|�[�l���g�̈ʒu�A���x�A�֐ߕψʁA�֐ߑ��x���X�V
	PHJointBase::Integrate(dt);
}

//-----------------------------------------------------------------------------
SGOBJECTIMP(PHJointClearForce, SGBehaviorEngine);
void PHJointClearForce::Step(SGScene* s){
	je->root->ClearTorqueRecursive();
}


SGOBJECTIMP(PHJointEngine, SGBehaviorEngine);
void PHJointEngineEnumSolid(PHJointBase* j, PHJointEngine* e){
	e->solids.insert(j->solid);
}
void PHJointEngine::Loaded(SGScene* scene){
	root->Loaded(scene);
	root->Traverse(&PHJointEngineEnumSolid, this);
}
void PHJointEngine::Step(SGScene* scene)
{
	timer.Start();
	//	featherstone's algorithm
	double dt = scene->GetTimeStep();
	root->CompCoriolisAccelRecursive(dt);
	root->CompArticulatedInertia(dt);
	root->Integrate(dt);
	timer.Stop();
}

bool PHJointEngine::Has(SGObject* o){
	PHSolid* solid = DCAST(PHSolid, o);
	if (solid){
		if (solids.find(solid) != solids.end()) return true;
	}
	PHJointBase* joint = DCAST(PHJointBase, o);
	while (joint){
		if (joint == root) return true;
		joint = joint->GetParent();
	}
	return false;
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
	if (DCAST(PHJointBase, o)){
		((PHJointBase*)o)->SetParent(root);
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
		PHJointBase::GetTypeInfoStatic(), NULL
	};
	return rv;
}
void PHJointEngine::LoadState(const SGBehaviorStates& states){
	root->LoadState(states);
}
void PHJointEngine::SaveState(SGBehaviorStates& states) const{
	root->SaveState(states);
}

//////////////////////////////////////////////////////////////////////
class PHJointEngineLoader : public FIObjectLoader<PHJointEngine>{
public:
	bool LoadData(FILoadScene* ctx, PHJointEngine* engine){
		engine->root = new PHJointRoot;
		PHJointClearForce* jcf = new PHJointClearForce;
		jcf->je = engine;
		ctx->scene->AddChildObject(jcf, ctx->scene);
		return true;
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
			doc->AddChild(ctx->CreateDocNode("REF", je->root->solid));
		}else if(je->root->frame){
			doc->AddChild(ctx->CreateDocNode("REF", je->root->frame));
		}

		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(unsigned i=0; i<je->root->Children().size(); ++i){
			ctx->SaveRecursive(je->root->Children()[i]);
		}
		ctx->docs.pop_back();
	}
};


//�֐߂̎��
enum PHJointType{
	TYPE_HINGE,			//��]
	TYPE_SLIDER,		//����
	//TYPE_BALLJOINT,	//�{�[���W���C���g
};
//���͂̎��
enum PHInputType{
	TYPE_TORQUE,		//�g���N
	TYPE_ACCEL,			//�����x
	TYPE_VELOCITY,		//���x
	TYPE_POSITION		//�ʒu
};
///�֐ߏ��D���̂܂܃t�@�C����Joint�^�O�̃G���g���Ɠ���
struct PHJointInfo{
	DWORD		nType;				///<	�֐ߎ�� @see PHJointType
	Vec3f		v3fPositionParent;	///<	�e���̂̃t���[�����猩���֐߈ʒu
	Matrix3f	m3fRotationParent;	///<	�e���̂̃t���[�����猩���֐ߎp��
	Vec3f		v3fPositionChild;	///<	�q���̂̃t���[�����猩���֐߈ʒu
	Matrix3f	m3fRotationChild;	///<	�q���̂̃t���[�����猩���֐ߎp��

	float		fPosition;			///<	�ψ�
	float		fVelocity;			///<	���x
	float		fMaxTorque;			///<	�ő�g���N
	float		fMinPosition;		///<	���͈�(�ő�l)
	float		fMaxPosition;		///<	���͈�(�ŏ��l)
	DWORD		nInputType;			///<	���͂̎�� @see PHJointInputType
	float		fInput;				///<	���͒l�̏����l
	float		fPValue;			///<	PID�����P�̒l
	float		fIValue;			///<	PID�����I�̒l
	float		fDValue;			///<	PID�����D�̒l
};

class PHJointLoader: public FIBaseLoader
{
protected:
	virtual UTString GetNodeType() const {
		return "Joint";
	}
	void Load(FILoadScene* ctx){
		//�֐߂̎�ނƃp�����[�^
		PHJointInfo info;
		ctx->docs.Top()->GetWholeData(info);
		//	����n���E��n�ϊ�
		info.m3fRotationChild.ExZ() *= -1;
		info.m3fRotationChild.EyZ() *= -1;
		info.m3fRotationChild.EzX() *= -1;
		info.m3fRotationChild.EzY() *= -1;
		info.v3fPositionChild.Z() *= -1;
		info.m3fRotationParent.ExZ() *= -1;
		info.m3fRotationParent.EyZ() *= -1;
		info.m3fRotationParent.EzX() *= -1;
		info.m3fRotationParent.EzY() *= -1;
		info.v3fPositionParent.Z() *= -1;

		UTRef<PHJoint1D> joint=NULL;
		if (info.nType == TYPE_HINGE){
			joint = new PHJointHinge;
		}else if(info.nType == TYPE_SLIDER){
			joint = new PHJointSlider;
		}else{
			DSTR << "Unknown joint type " << info.nType << std::endl;
		}
		joint->SetName(ctx->docs.Top()->GetName().c_str(), ctx->scene);
		ctx->AddObject(joint);
		ctx->AddContainer(joint);
		
		joint->m3fRotationChild = info.m3fRotationChild;
		joint->m3fRotationParent = info.m3fRotationParent;
		joint->v3fPositionChild = info.v3fPositionChild;
		joint->v3fPositionParent = info.v3fPositionParent;
		joint->position = info.fPosition;
		joint->velocity = info.fVelocity;
		joint->maxPosition = info.fMaxPosition;
		joint->minPosition = info.fMinPosition;
		
		//	PID�̕���
		UTRef<PHJointPid> pid = new PHJointPid;
		std::string name = joint->GetName();
		if (name[0] == 'j') name = name.substr(1);
		if (name[0] == 'o') name = name.substr(1);
		name = std::string("pid").append(name);
		pid->SetName(name.c_str(), ctx->scene);
		ctx->scene->AddChildObject(pid, ctx->scene);
		pid->proportional = info.fPValue;
		pid->differential = info.fDValue;
		pid->integral = info.fIValue;
		pid->goal = info.fInput;
		pid->joint = joint;
		pid->type = (info.nInputType == TYPE_VELOCITY) ? 1 : 0;
	}
};

DEF_REGISTER_LOADER(PHJoint);
DEF_REGISTER_BOTH(PHJointEngine);

}
