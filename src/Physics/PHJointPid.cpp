#include "Physics.h"
#pragma hdrstop
#include "PHJointPid.h"

using namespace PTM;
namespace Spr{;

SGOBJECTIMP(PHJointPid, SGBehaviorEngine);

size_t PHJointPid::NReferenceObjects(){
	return joint ? 1 : 0;
}
SGObject* PHJointPid::ReferenceObject(size_t i){
	if (i == 0) return joint;
	return NULL;
}
bool PHJointPid::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHJoint1D, o)){
		joint = (PHJoint1D*)o;
		return true;
	}
	return false;
}
bool PHJointPid::DelChildObject(SGObject* o, SGScene* s){
	if (joint == o){
		joint = NULL;
		return true;
	}
	return false;
}
extern void LimitCycle(float& t);
void PHJointPid::Step(SGScene* s){
	float now = type ? joint->GetVelocity() : joint->GetPosition();
	float e = goal - now;
	if (type==0 && DCAST(PHJointHinge, joint)){
		LimitCycle(e);
	}
	float ed = (e - error) / s->GetTimeStep();
	integratedError += e * s->GetTimeStep();
	float tq = proportional*e + integral*integratedError + differential*ed;
	joint->AddTorque(tq);
	error = e;
}


class PHJointPidState:public SGBehaviorState{
public:
	SGOBJECTDEF(PHJointPidState);
	float error;
	float integratedError;
	float goal;
};
SGOBJECTIMP(PHJointPidState, SGBehaviorState);

void PHJointPid::LoadState(const SGBehaviorStates& states){
	PHJointPidState* pState = DCAST(PHJointPidState, states.GetNext());
	error = pState->error;
	integratedError = pState->integratedError;
	goal = pState->goal;	
}
void PHJointPid::SaveState(SGBehaviorStates& states) const{
	UTRef<PHJointPidState> state = new PHJointPidState;
	states.push_back(state);
	state->error = error;
	state->integratedError = integratedError;
	state->goal = goal;	
}

typedef float FLOAT;
DEF_RECORD(XJointPid,{
	GUID Guid(){ return WBGuid("395862A3-064C-445c-A969-72488CDD6A78"); }
	FLOAT prop;		//	PID ‚Ì P ”ä—áŒW”
	FLOAT inte;		//	PID ‚Ì I Ï•ªŒW”
	FLOAT diff;		//	PID ‚Ì D ”÷•ªŒW”
	FLOAT goal;		//	PID‚Ì–Ú•W’l
});


class PHJointPidLoader : public FIObjectLoader<PHJointPid>
{
public:
	PHJointPidLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XJointPid);
	}
	bool LoadData(FILoadScene* ctx, PHJointPid* pid){		
		XJointPid x;
		ctx->docs.Top()->GetWholeData(x);
		pid->proportional = x.prop;
		pid->integral = x.inte;
		pid->differential = x.diff;
		pid->goal = x.goal;
		return true;
	}
};

class PHJointPidSaver : public FIObjectSaver<PHJointPid>
{
protected:
	void SaveData(class FISaveScene* ctx, FIDocNodeBase* doc, PHJointPid* pid){
		XJointPid x;
		ctx->docs.Top()->GetWholeData(x);
		x.prop = pid->proportional;
		x.inte = pid->integral;
		x.diff =  pid->differential;;
		x.goal = pid->goal;
		doc->SetWholeData(x);
	}
};
DEF_REGISTER_BOTH(PHJointPid);

PHJointPid* PHJointPid::Find(PHJoint1D* j, SGScene* scene){
	if (!j || !scene) return NULL;
	for(SGBehaviorEngines::const_iterator itb = scene->GetBehaviors().begin(); itb != scene->GetBehaviors().end(); ++itb){
		PHJointPid* pid = DCAST(PHJointPid, (*itb));
		if (pid && pid->joint == j){
			return pid;
		}
	}
	return NULL;
}

}
