#include "Physics.h"
#pragma hdrstop

using namespace PTM;
namespace Spr{

///////////////////////////////////////////////////////////////////
//	PHSolid
SGOBJECTIMP(PHSolid, SGObject);
PHSolid::PHSolid(){
	mass = 1.0;
	inertia = Matrix3d::Unit();
	integrationMode = PHINT_SIMPLETIC;
}
bool PHSolid::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(SGFrame, o)){
		frame = (SGFrame*)o;
		return true;
	}
	return false;
}
size_t PHSolid::NReferenceObjects(){
	if (frame) return 1;
	return 0;
}
SGObject* PHSolid::ReferenceObject(size_t i){
	if (i != 0) return NULL;
	return frame;
}
/*inline void Rotate(Quaterniond& quat, const Vec3d& drot){
	double norm = drot.norm();
	if(norm > 1.0e-10){
		quat = Quaterniond::Rot(norm, drot) * quat;
		quat.unitize();
	}
}*/

void PHSolid::Step(double dt){
#ifdef _DEBUG
	if (velocity.norm() > 100 || angVelocity.norm() > 100){	
		DSTR << "Warning: solid '" << GetName() << "' has very fast velocity." << velocity << angVelocity << std::endl;
	}
#endif
	//k1 = f(y);
	//k2 = f(y + k1 * h / 2);
	//k3 = f(y + k2 * h / 2);
	//k4 = f(y + k3);
	//y += (k1 + 2 * k2 + 2 * k3 + k4) * h / 6;

	if(GetIntegrationMode() != PHINT_NONE){
		//	�ϕ��v�Z
		Vec3d dv, dw;
		switch(GetIntegrationMode()){
		case PHINT_EULER:
			//���s�ړ��ʂ̐ϕ�
			SetFramePosition(GetFramePosition() + velocity * dt);
			velocity += force * (dt / mass);
			//�p���x����N�E�H�[�^�j�I���̎��Ԕ��������߁A�ϕ��A���K��
			quat += quat.derivative(angVelocity) * dt;
			quat.unitize();
			torque		= quat.conjugated() * torque;			//�g���N�Ɗp���x�����[�J�����W��
			angVelocity = quat.conjugated() * angVelocity;
			angVelocity += Euler(inertia, torque, angVelocity) * dt;	//�I�C���[�̉^��������
			torque = quat * torque;						//�g���N�Ɗp���x�����[���h��
			angVelocity = quat * angVelocity;
			break;
		case PHINT_ARISTOTELIAN:{
			SetFramePosition(GetFramePosition() + velocity * dt);
			velocity = force / mass;		//���x�͗͂ɔ�Ⴗ��
			Vec3d tq = quat.conjugated() * torque;	//�g���N�����[�J����
			angVelocity = quat * (inertia_inv * tq);	//�p���x�̓g���N�ɔ�Ⴗ��
			//�N�E�H�[�^�j�I����ϕ��A���K��
			quat += quat.derivative(angVelocity) * dt;
			quat.unitize();
			}break;
		case PHINT_SIMPLETIC:
			//���s�ړ��ʂ̐ϕ��i��͉��Ɉ�v�j
			dv = force * (dt / mass);									//���x�ω���
			SetFramePosition(GetFramePosition() + (velocity + 0.5 * dv) * dt);	//�ʒu�̐ϕ�
			velocity += dv;												//���x�̐ϕ�
			//��]�ʂ̐ϕ�
			//��]�͉�͓I�ɐϕ��ł��Ȃ��̂ŁA�`���I�Ɂ��̌�������]�̏ꍇ�ɓ��Ă͂߂�
			torque		= quat.conjugated() * torque;					//�g���N�Ɗp���x�����[�J�����W��
			angVelocity = quat.conjugated() * angVelocity;
			dw = Euler(inertia, torque, angVelocity) * dt;			//�p���x�ω���
			quat += quat.derivative(quat * (angVelocity + 0.5 * dw)) * dt;
			quat.unitize();
			angVelocity += dw;										//�p���x�̐ϕ�
			torque = quat * torque;								//�g���N�Ɗp���x�����[���h��
			angVelocity = quat * angVelocity;
			break;
		case PHINT_RUNGEKUTTA2:
			//���s�ړ��ʂ̐ϕ�(�܂��߂Ƀ����Q�N�b�^�����ɏ]���Ă����ʂ͓�������͉�)
			dv = force * (dt / mass);
			SetFramePosition(GetFramePosition() + (velocity + 0.5 * dv) * dt);
			velocity += dv;
			//��]�ʂ̌v�Z
			//��]�͉�͓I�ɐϕ��ł��Ȃ��̂ŁA�����Q�N�b�^�������g��
			torque		= quat.conjugated() * torque;					//�g���N�Ɗp���x�����[�J�����W��
			angVelocity = quat.conjugated() * angVelocity;
			_angvel[0]	= angVelocity;
			_angacc[0]	= Euler(inertia, torque, _angvel[0]);
			_angvel[1]	= _angvel[0] + _angacc[0] * dt;
			_angacc[1]	= Euler(inertia, torque, _angvel[1]);
			quat += quat.derivative(quat * (_angvel[0] + _angvel[1]) / 2.0) * dt;
			quat.unitize();
			angVelocity = quat * (angVelocity + ((_angacc[0] + _angacc[1]) * (dt / 2.0)));
			torque = quat * torque;
			break;
		case PHINT_RUNGEKUTTA4:
			//���s�ړ��ʂ̐ϕ�(�܂��߂Ƀ����Q�N�b�^�����ɏ]���Ă����ʂ͓�������͉�)
			dv = force * (dt / mass);
			SetFramePosition(GetFramePosition() + (velocity + 0.5 * dv) * dt);
			velocity += dv;
			//��]�ʂ̌v�Z
			_angvel[0]	= angVelocity;
			_angacc[0]	= Euler(inertia, torque, _angvel[0]);
			_angvel[1]	= _angvel[0] + _angacc[0] * (dt / 2.0);
			_angacc[1]	= Euler(inertia, torque, _angvel[1]);
			_angvel[2]	= _angvel[0] + _angacc[1] * (dt / 2.0); 
			_angacc[2]	= Euler(inertia, torque, _angvel[2]);
			_angvel[3]	= _angvel[0] + _angacc[2] * dt;
			_angacc[3]	= Euler(inertia, torque, _angvel[3]);
			quat += quat.derivative(quat * (_angvel[0] + 2.0 * (_angvel[1] + _angvel[2]) + _angvel[3]) / 6.0) * dt;
			quat.unitize();
			angVelocity = quat * (angVelocity + (_angacc[0] + 2.0 * (_angacc[1] + _angacc[2]) + _angacc[3]) * (dt / 6.0));
			torque = quat * torque;
			
			break;
		}
	}
	UpdateFrame();
}

void PHSolid::Loaded(SGScene* scene){
	quat.from_matrix(frame->GetRotation());
}

void PHSolid::AddForce(Vec3d f)
{
	force += f;
}

void PHSolid::AddForce(Vec3d f, Vec3d r){
	torque += (r - frame->GetPosture() * center) ^ f;
	force += f;
}

/*void PHSolid::AddForceLocal(Vec3d f, Vec3d r){
	torque += cross(quat * (r - center), f);
	force += f;
}*/

void PHSolid::ClearForce(){
	force.clear();
	torque.clear();
}

void PHSolid::UpdateFrame(){
	Matrix3f rot;
	quat.to_matrix(rot);
	frame->SetRotation(rot);
}


//----------------------------------------------------------------------------
//	PHSolidContainer
//
SGOBJECTIMP(PHSolidContainer, SGBehaviorEngine);
bool PHSolidContainer::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	return false;
}

void  PHSolidContainer::Step(SGScene* s){
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); ++it){
		(*it)->Step(s->GetTimeStep());
	}
}

void  PHSolidContainer::Loaded(SGScene* scene){
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); ++it){
		(*it)->Loaded(scene);
	}
}

class PHSolidState{
public:
	PHSolid* solid;
	Vec3d pos;
	Quaterniond ori;
	Vec3d vel;
	Vec3d angVel;
	Vec3d force;
	Vec3d torque;
};
class PHSolidContainerState: public SGBehaviorState, public std::vector<PHSolidState>{
public:
	SGOBJECTDEF(PHSolidContainerState);
};
SGOBJECTIMP(PHSolidContainerState, SGBehaviorState);

void PHSolidContainer::LoadState(const SGBehaviorStates& states){
	PHSolidContainerState* pState = DCAST(PHSolidContainerState, states.GetNext());
	assert(pState);
	PHSolidContainerState& state = *pState;
	for(unsigned i=0; i<state.size(); ++i){
		solids[i]->SetFramePosition( state[i].pos );
		solids[i]->SetOrientation( state[i].ori );
		solids[i]->SetVelocity( state[i].vel);
		solids[i]->SetAngularVelocity( state[i].angVel);
		solids[i]->SetForce( state[i].force);
		solids[i]->SetTorque( state[i].torque);
		solids[i]->UpdateFrame();
	}
}
void PHSolidContainer::SaveState(SGBehaviorStates& states) const{
	UTRef<PHSolidContainerState> state = new PHSolidContainerState;
	states.push_back(state);
	for(PHSolids::const_iterator it = solids.begin(); it != solids.end(); ++it){
		state->push_back(PHSolidState());
		state->back().pos = (*it)->GetFramePosition();
		state->back().ori = (*it)->GetOrientation();
		state->back().vel = (*it)->GetVelocity();
		state->back().angVel = (*it)->GetAngularVelocity();
		state->back().force = (*it)->GetForce();
		state->back().torque = (*it)->GetTorque();
	}
}

//----------------------------------------------------------------------------
//	PHSolidClearForce
//
SGOBJECTIMP(PHSolidClearForce, SGBehaviorEngine);
void  PHSolidClearForce::Step(SGScene* s){
	for(PHSolids::iterator it = cont->solids.begin(); it != cont->solids.end(); ++it){
		(*it)->ClearForce();
	}
}


class PHSolidContainerLoader:public FIObjectLoader<PHSolidContainer>{
	virtual bool LoadData(FILoadScene* ctx, PHSolidContainer* sc){
		UTRef<PHSolidClearForce> clearForce = new PHSolidClearForce;
		clearForce->cont = sc;
		ctx->scene->GetBehaviors().Add(clearForce);
		return true;
	}
};

class PHSolidContainerSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHSolidContainer"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHSolidContainer* sc = (PHSolidContainer*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("SolidContainer", sc);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(PHSolids::iterator it = sc->solids.begin(); it != sc->solids.end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHSolidContainer);

class PHSolidLoader:public FIObjectLoader<PHSolid>{
	virtual bool LoadData(FILoadScene* ctx, PHSolid* s){
		SolidInfo info;
		ctx->docs.Top()->GetWholeData(info);
		s->SetMass				(info.mass);
		s->SetInertia			(info.inertia);
		s->SetCenter			(info.center);
		s->SetVelocity			(info.velocity);
		s->SetAngularVelocity	(info.angularVelocity);
		return true;
	}
};

class PHSolidSaver:public FIBaseSaver{
	virtual UTString GetType() const{return "PHSolid";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHSolid* s = (PHSolid*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("Solid", s);
		ctx->docs.back()->AddChild(doc);
		SolidInfo info;
		info.mass				= (float)s->GetMass();
		info.inertia			= s->GetInertia();
		info.velocity			= s->GetVelocity();
		info.angularVelocity	= s->GetAngularVelocity();
		info.center				= s->GetCenter();
		doc->SetWholeData(info);
		if (s->GetFrame()){
			doc->AddChild(ctx->CreateDocNode("REF", s->GetFrame()));
		}
	}
};
DEF_REGISTER_BOTH(PHSolid);


}
