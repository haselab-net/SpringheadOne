#include "Physics.h"
#pragma hdrstop
#include "PHForceField.h"

using namespace std;
namespace Spr{

/////////////////////////////////////////////////////////////
//PHGravityEngine/Loader/Saver
SGOBJECTIMP(PHGravityEngine, SGBehaviorEngine);

void PHGravityEngine::Step(SGScene*, float, unsigned)
{
	ApplyForce();
}

bool PHGravityEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		targets.push_back((PHSolid*)o);
		return true;
	}
	return false;
}

void PHGravityEngine::ApplyForce()
{
	list<UTRef<PHSolid> >::iterator it;
	PHSolid* solid;
	for(it = targets.begin(); it != targets.end(); it++){
		solid = *it;
		solid->AddForceLocal((float)solid->GetMass() * accel, solid->GetCenter());
	}
}


///////////////////////////////////////////////////////////////////
class PHGravityEngineLoader : public FIObjectLoader<PHGravityEngine>{
public:
	virtual void LoadData(FILoadScene* ctx, UTRef<PHGravityEngine>& ge){
		ge = new PHGravityEngine;
		//d—Í‰Á‘¬“x‚ðŽæ“¾
		ctx->docs.Top()->GetData(ge->accel);
	}
};

class PHGravityEngineSaver : public FIBaseSaver{
public:
	UTString GetType() const{ return "PHGravityEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHGravityEngine* gravity= (PHGravityEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("GravityEngine", gravity);
		ctx->docs.back()->AddChild(doc);
		doc->SetData(gravity->accel);
		for(list<UTRef<PHSolid> >::iterator it = gravity->targets.begin(); it != gravity->targets.end(); it++)
			doc->AddChild(ctx->CreateDocNode("Solid", *it, true));
	}
};
DEF_REGISTER_BOTH(PHGravityEngine);

}