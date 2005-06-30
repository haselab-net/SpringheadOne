#include "Physics.h"
#pragma hdrstop

namespace Spr{;

//----------------------------------------------------------------------------
//	PHWaterContactEngine
//
SGOBJECTIMP(PHWaterContactEngine, SGBehaviorEngine);

PHWaterContactEngine::PHWaterContactEngine(){

}

bool PHWaterContactEngine::AddChildObject(SGObject* o, SGScene* scene){
	if(DCAST(PHSolid, o)){
		solids.push_back((PHSolid*)o);
		return true;
	}
	//SGFrame‚Í–Ú‰º–¢‘Î‰ž
	/*if (DCAST(SGFrame, o)){
		solids.push_back(new PHSolid);
		solids.back()->SetMass(FLT_MAX);
        Matrix3d in = Matrix3d::Unit();
		solids.back()->SetInertia(in * 1e200);
		solids.back()->SetFrame((SGFrame*)o);
		return true;
	}*/
	if(DCAST(PHWater, o)){
		water = DCAST(PHWater, o);
		return true;
	}
	return false;
}

void PHWaterContactEngine::Clear(SGScene* s){
	water = 0;
	solids.clear();
}
void PHWaterContactEngine::Loaded(SGScene* scene){
	Init(scene);
}

void PHWaterContactEngine::Init(SGScene* scene){
	
}

//----------------------------------------------------------------------------
//	PHWaterContactEngineLoader
//
class PHWaterContactEngineLoader:public FIObjectLoader<PHWaterContactEngine>{
	virtual bool LoadData(FILoadScene* ctx, PHWaterContactEngine* pc){
		ctx->objects.Push(pc);
		return true;
	}
	virtual void Loaded(FILoadScene* ctx){
		ctx->objects.Pop();
	}
};

class PHWaterContactEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterContactEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWaterContactEngine* pc = (PHWaterContactEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("WaterContactEngine", pc);
		ctx->docs.back()->AddChild(doc);
		for(PHSolids::iterator it = pc->solids.begin(); it != pc->solids.end(); ++it){
			doc->AddChild(ctx->CreateDocNode("REF", *it));
		}
		doc->AddChild(ctx->CreateDocNode("REF", water));
	}
};
DEF_REGISTER_BOTH(PHWaterContactEngine);

}
