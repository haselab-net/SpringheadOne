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
	Device()->Update(dt);
	Solid()->SetFramePosition(qtOffset * (Device()->GetPos() * posScale) + v3Offset);
	Solid()->SetOrientation(qtOffset * Device()->GetOri());
	Solid()->SetVelocity(qtOffset * (Device()->GetVel() * posScale));
	Solid()->SetAngularVelocity(qtOffset * (Device()->GetAngVel()));
	Solid()->UpdateFrame();
}

void FWPointerRui::Init(HIRuiBase* dev){
	jointPids.resize(dev->NJoint());
	for(int i=0; i<jointPids.size(); ++i){
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
	for(int ii=0; ii < jointPids.size(); ++ii){
		for(int i=0; names[i]; ++i){
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
	for(int i=0; i<jointPids.size(); ++i){
		float angle = Device()->GetJointAngle(i);
		if (jointPids[i]){
			jointPids[i]->goal = angle;
			Device()->SetTorque(i, jointPids[i]->joint->GetTorque());
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
