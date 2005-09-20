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
	IntegrationMode = PHINT_SIMPLETIC;
//	IntegrationMode = PHINT_EULER;
}
bool PHSolid::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(SGFrame, o)){
		frame = (SGFrame*)o;
		return true;
	}
	return false;
}

inline void Rotate(Quaterniond& quat, const Vec3d& drot){
	double norm = drot.norm();
	if(norm > 1.0e-10){
		quat = Quaterniond::Rot(norm, drot) * quat;
		quat.unitize();
	}
}
void PHSolid::Step(double dt){
#ifdef _DEBUG
	if (velocity.norm() > 100 || angVelocity.norm() > 100){	
		DSTR << "Warning: Very fast solid Vel" << velocity << angVelocity << std::endl;
	}
#endif
	//k1 = f(y);
	//k2 = f(y + k1 * h / 2);
	//k3 = f(y + k2 * h / 2);
	//k4 = f(y + k3);
	//y += (k1 + 2 * k2 + 2 * k3 + k4) * h / 6;

	if(IntegrationMode != PHINT_NONE){

		//ƒgƒ‹ƒNAŠp‘¬“x‚ðƒ[ƒJƒ‹‚É•ÏŠ·
		torque		= quat.inv() * torque;
		angVelocity = quat.inv() * angVelocity;
		
		//	Ï•ªŒvŽZ
		Vec3d	dangvel, drot, accel;
		switch(IntegrationMode){
		case PHINT_EULER:
			//•½sˆÚ“®—Ê‚ÌÏ•ª
			SetPosition(GetPosition() + velocity*dt);
			accel = force / mass;
			velocity += accel * dt;
			//‰ñ“]—Ê‚ÌŒvŽZ
			dangvel		= Euler(inertia, torque, angVelocity) * dt;
			drot		= angVelocity * dt;
			Rotate(quat, quat * drot);
			angVelocity = quat * (angVelocity + dangvel);
			break;
		case PHINT_SIMPLETIC:
			//•½sˆÚ“®—Ê‚ÌÏ•ª
			accel = force / mass;
			velocity += accel * dt;
			SetPosition(GetPosition() + velocity*dt);
			//‰ñ“]—Ê‚ÌŒvŽZ
			dangvel		= Euler(inertia, torque, angVelocity) * dt;
			drot		= (angVelocity+dangvel) * dt;
			angVelocity = quat * (angVelocity + dangvel);
			Rotate(quat, quat * drot);
			break;
		case PHINT_RUNGEKUTTA2:
			//•½sˆÚ“®—Ê‚ÌÏ•ª
			accel = force / mass;
			velocity += accel * dt;
			SetPosition(GetPosition() + velocity*dt);
			//‰ñ“]—Ê‚ÌŒvŽZ
			_angvel[0]	= angVelocity;
			_angacc[0]	= Euler(inertia, torque, _angvel[0]);
			_angvel[1]	= _angvel[0] + _angacc[0] * dt;
			_angacc[1]	= Euler(inertia, torque, _angvel[1]);
			dangvel = (_angacc[0] + _angacc[1]) * (dt / 2.0);
			drot	= (_angvel[0] + _angvel[1]) * (dt / 2.0);
			Rotate(quat, quat * drot);
			angVelocity = quat * (angVelocity + dangvel);
			break;
		case PHINT_RUNGEKUTTA4:
			//•½sˆÚ“®—Ê‚ÌÏ•ª
			accel = force / mass;
			velocity += accel * dt;
			SetPosition(GetPosition() + velocity*dt);
			//‰ñ“]—Ê‚ÌŒvŽZ
			_angvel[0]	= angVelocity;
			_angacc[0]	= Euler(inertia, torque, _angvel[0]);
			_angvel[1]	= _angvel[0] + _angacc[0] * (dt / 2.0);
			_angacc[1]	= Euler(inertia, torque, _angvel[1]);
			_angvel[2]	= _angvel[0] + _angacc[1] * (dt / 2.0); 
			_angacc[2]	= Euler(inertia, torque, _angvel[2]);
			_angvel[3]	= _angvel[0] + _angacc[2] * dt;
			_angacc[3]	= Euler(inertia, torque, _angvel[3]);
			dangvel = (_angacc[0] + 2.0 * (_angacc[1] + _angacc[2]) + _angacc[3]) * (dt / 6.0);
			drot	= (_angvel[0] + 2.0 * (_angvel[1] + _angvel[2]) + _angvel[3]) * (dt / 6.0);
			Rotate(quat, quat * drot);
			angVelocity = quat * (angVelocity + dangvel);
			break;
		}
	}
	//ƒtƒŒ[ƒ€‚É”½‰f
	Matrix3f rot;
	quat.to_matrix(rot);
	frame->SetOrientation(rot);
}

void PHSolid::Loaded(SGScene* scene){
	quat.from_matrix(frame->GetOrientation());
}

void PHSolid::AddForce(Vec3d f)
{
	force += f;
}

void PHSolid::AddForce(Vec3d f, Vec3d r){
	torque += (r - frame->GetPosture() * center) ^ f;
	force += f;
}

void PHSolid::AddForceLocal(Vec3d f, Vec3d r){
	torque += cross(quat * (r - center), f);
	force += f;
}

void PHSolid::ClearForce(){
	force.clear();
	torque.clear();
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

void  PHSolidContainer::Step(SGScene* s, float dt, unsigned int count){
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); ++it){
		(*it)->Step(dt);
	}
}

void  PHSolidContainer::Loaded(SGScene* scene){
	for(PHSolids::iterator it = solids.begin(); it != solids.end(); ++it){
		(*it)->Loaded(scene);
	}
}

//----------------------------------------------------------------------------
//	PHSolidClearForce
//
SGOBJECTIMP(PHSolidClearForce, SGBehaviorEngine);
void  PHSolidClearForce::Step(SGScene* s, float dt, unsigned int count){
	for(PHSolids::iterator it = cont->solids.begin(); it != cont->solids.end(); ++it){
		(*it)->ClearForce();
	}
}


class PHSolidContainerLoader:public FIObjectLoader<PHSolidContainer>{
	virtual void LoadData(FILoadScene* ctx, UTRef<PHSolidContainer>& sc){
		sc = new PHSolidContainer;
		UTRef<PHSolidClearForce> clearForce = new PHSolidClearForce;
		clearForce->cont = sc;
		ctx->scene->GetBehaviors().Add(clearForce);
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
	virtual void LoadData(FILoadScene* ctx, UTRef<PHSolid>& s){
		s = new PHSolid;
		SolidInfo info;
		ctx->docs.Top()->GetData(info);
		s->SetMass				(info.mass);
		s->SetInertia			(info.inertia);
		s->SetCenter			(info.center);
		s->SetVelocity			(info.velocity);
		s->SetAngularVelocity	(info.angularVelocity);
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
		doc->SetData(info);
		if (s->frame){
			doc->AddChild(ctx->CreateDocNode("Frame", s->frame, true));
		}
	}
};
DEF_REGISTER_BOTH(PHSolid);


}
