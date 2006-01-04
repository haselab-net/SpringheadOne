#include "FWPointer.h"
namespace Spr{;

static std::vector<SGObject*> objs;
void FWPointer6D::Connect(SGScene* s){
	char* names[] = {
		"Pointer",
		"Pointer1",
		"Pointer2",
		"Pointer3",
		"Pointer4",
		"Pointer5",
		"Pointer6",
		"Pointer7",
		"Pointer8",
		"Pointer9",
		"Pointer10",
		NULL
	};
	for(int i=0; names[i]; ++i){
		s->FindObject(Solid(), names[i]);
		if (Solid() && std::find(objs.begin(), objs.end(), Solid()) == objs.end()){
			objs.push_back(Solid());
			qtOffset = Solid()->GetOrientation();
			v3Offset = Solid()->GetFramePosition();
			qtOffsetInv = qtOffset.inv();
			break;
		}
	}
}
void FWPointer6D::Init(HIForceDevice6D* d){
	device = d;
}

void FWPointer6D::Step(float dt){
	if (!Device() || !Solid()) return;
	if (!forceScale) forceScale = 1.0f;
	if (!posScale) posScale = 1.0f;
	if (bOutForce){
		Device()->SetForce(qtOffsetInv*Solid()->GetForce() / forceScale, qtOffsetInv*Solid()->GetTorque() / forceScale);
	}else{
		Device()->SetForce(Vec3f(), Vec3f());
	}
	Update(dt);
	Solid()->SetFramePosition(GetPos());
	Solid()->SetOrientation(GetOri());
	Solid()->SetVelocity(GetVel());
	Solid()->SetAngularVelocity(GetAngVel());
}

void FWPointer6D::Update(float dt){
	Device()->Update(dt);
}

Vec3f FWPointer6D::GetPos(){
	return qtOffset * (Device()->GetPos() * posScale) + v3Offset;
}

Quaternionf FWPointer6D::GetOri(){
	return qtOffset * Device()->GetOri();
}

Vec3f FWPointer6D::GetVel(){
	return qtOffset * (Device()->GetVel() * posScale);
}

Vec3f FWPointer6D::GetAngVel(){
	return qtOffset * (Device()->GetAngVel());
}

void FWPointer6D::SetForce(Vec3f force, Vec3f torque){
	Device()->SetForce(qtOffsetInv*force / forceScale, qtOffsetInv*torque / forceScale);
}

void FWPointerRui::Init(HIRuiBase* dev){
	jointPids.resize(dev->NJoint());
	for(unsigned i=0; i<jointPids.size(); ++i){
		jointPids[i] = NULL;
	}
}
void FWPointerRui::Connect(SGScene* scene){
	char* names[] = {
		"joHandR1",
		"joHandR2",
		"joHandL1",
		"joHandL2",
		NULL
	};
	for(unsigned ii=0; ii < jointPids.size(); ++ii){
		for(unsigned i=0; names[i]; ++i){
			PHJoint1D* joint;
			scene->FindObject(joint, names[i]);
			PHJointPid* pid = PHJointPid::Find(joint, scene);
			if (pid){
				jointPids[ii] = pid;
				break;
			}
			if (jointPids[ii] && std::find(objs.begin(), objs.end(), jointPids[ii]) == objs.end()){
				objs.push_back(jointPids[ii]);
				break;
			}
		}
	}
	if (jointPids.size()) obj = jointPids[0];
}
void FWPointerRui::Step(float dt){
	Device()->Update(dt);
	for(unsigned i=0; i<jointPids.size(); ++i){
		float angle = Device()->GetJointAngle(i);
		if (jointPids[i]){
			jointPids[i]->goal = angle;
			Device()->SetTorque(i, (float)jointPids[i]->GetJointTorque());
		}
	}
}

void FWPointers::Connect(SGScene* s){
	objs.clear();
	for(iterator it = begin(); it != end(); ++it){
		(*it)->Connect(s);
	}		
}
void FWPointers::Step(float dt){
	for(iterator it = begin(); it != end(); ++it){
		(*it)->Step(dt);
	}
}
FWPointerBase* FWPointers::Find(HIHapticDevice* dev){
	for(FWPointers::iterator it = begin(); it != end(); ++it){
		if ((*it)->device == dev){
			return *it;
		}
	}
	return NULL;
}

}	//	namespace Spr
