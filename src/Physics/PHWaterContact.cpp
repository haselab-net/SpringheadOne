#include "Physics.h"
#pragma hdrstop

namespace Spr{;

//----------------------------------------------------------------------------
//PHWSolid
PHWSolid::Init(){
	frame = solid->GetFrame();
	//SGFrame::contentsからCDGeometryの派生クラスオブジェクトを探す
	EnumGeometries(frame);
}

PHWSolid::EnumGeometries(SGFrame* f){
	CDMesh* g;
	for(SGObjects::iterator ic = f->contents.begin(); ic != f->contents.end(); ic++){
		g = DCAST(CDMesh, *ic);
		if(g){
			geometries.push_back(new PHWGeometry);
			geometries.back()->Set(f, g);
		}
	}
	for(SGFrames::iterator i = f->Children().begin(); i != f->Children().end(); i++)
		EnumGeometries(*i);
}

//----------------------------------------------------------------------------
//PHWGeometry
void PHWGeometry::Set(SGFrame* f, CDMesh* g){
	frame = f;
	conveces = g->conveces;
}


//----------------------------------------------------------------------------
//	PHWaterContactEngine
//
SGOBJECTIMP(PHWaterContactEngine, SGBehaviorEngine);

PHWaterContactEngine::PHWaterContactEngine(){

}

bool PHWaterContactEngine::AddChildObject(SGObject* o, SGScene* scene){
	if(DCAST(PHSolid, o)){
		solids.push_back(new PHWSolidInfo);
		solids.back()->solid = DCAST(PHSolid, o);
		return true;
	}
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
	//Solidの形状データを吸い出す
	PHWSolid* s;
	CDGeometry* g;
	for(PHWSolids::iterator is = solids.begin(); is != solids.end(); is++)
		(*is)->Init();
}

void PHWaterContactEngine::Step(SGScene* s){
	
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
