#include "Physics.h"
#pragma hdrstop
#include "PHForceField.h"

using namespace std;
namespace Spr{

/////////////////////////////////////////////////////////////
//PHGravityEngine/Loader/Saver
SGOBJECTIMP(PHGravityEngine, SGBehaviorEngine);

void PHGravityEngine::Step(SGScene*)
{
	ApplyForce();
}

bool PHGravityEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	return false;
}

void PHGravityEngine::ApplyForce()
{
	PHSolids::iterator it;
	PHSolid* solid;
	for(it = solids.begin(); it != solids.end(); it++){
		solid = *it;
		solid->AddForce((float)solid->GetMass() * accel);
	}
}


///////////////////////////////////////////////////////////////////
class PHGravityEngineLoader : public FIObjectLoader<PHGravityEngine>{
public:
	bool LoadData(FILoadScene* ctx, PHGravityEngine* gravity){		
		//�d�͉����x���擾
		ctx->docs.Top()->GetWholeData(gravity->accel);
		return true;
	}
};

class PHGravityEngineSaver : public FIBaseSaver{
public:
	UTString GetType() const{ return "PHGravityEngine";}
	void Save(FISaveScene* ctx, SGObject* arg){
		PHGravityEngine* gravity= (PHGravityEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("GravityEngine", gravity);
		ctx->docs.back()->AddChild(doc);
		doc->SetWholeData(gravity->accel);
		for(PHSolids::iterator it = gravity->solids.begin(); it != gravity->solids.end(); it++)
			doc->AddChild(ctx->CreateDocNode("REF", *it));
	}
};
DEF_REGISTER_BOTH(PHGravityEngine);

}