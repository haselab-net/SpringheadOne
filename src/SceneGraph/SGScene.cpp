#include "SceneGraph.h"
#pragma hdrstop
#include <sstream>
#include <FLOAT.H>

#pragma hdrstop


namespace Spr{;

UTTypeInfoImp<SGObjectNames::SGObjectKey> SGObjectNames::SGObjectKey::typeInfo("", NULL);
SGObjectNames::SGObjectKey SGObjectNames::key;
SGObjectNames::SGObjectKey::SGObjectKey(){
	AddRef();
}
SGObjectNames::SGObjectKey::~SGObjectKey(){
	DelRef();
}
bool SGObjectNames::Add(SGObject* obj){
	if (!obj->GetName() || !strlen(obj->GetName())) return false;
	std::pair<iterator, bool> rv = insert(obj);
	if (rv.second){
		return true;
	}else if (*rv.first == obj){
		return false;
	}
	UTString base = obj->name;
	int i=1;
	do{
		std::ostringstream ss;
		ss << "_" << i << '\0';
		obj->name = base + ss.str();
		rv = insert(obj);
		i++;
	} while(!rv.second);
	nameMap[base] = obj->name;
	DSTR << "name change: " << base << " -> " << obj->name << std::endl;
	return true;
}
void SGObjectNames::Print(std::ostream& os) const{
	for(const_iterator it = begin(); it != end(); ++it){
		os << (*it)->GetName() << " : " << (*it)->GetTypeInfo()->ClassName();
		os << std::endl;
	}
}
bool SGObjectNamesLess::operator () (SGObject* o1, SGObject* o2) const {
	int name = strcmp(o1->GetName(), o2->GetName());
	if (name < 0) return true;
	if (name == 0){
		int cls = 0;
		if (GETCLASSNAME(o1)[0] && GETCLASSNAME(o2)[0]){
			cls = strcmp(GETCLASSNAME(o1), GETCLASSNAME(o2));
			if (cls < 0) return true;
		}
		if (cls == 0){
			if (o1->GetNameSpace()[0] && o2->GetNameSpace()[0]){
				if (strcmp(o1->GetNameSpace(), o2->GetNameSpace()) < 0) return true;
			}
		}
	}
	return false;
}

//----------------------------------------------------------------------------
//	SGScene
SGOBJECTIMP(SGScene, SGObject);

void SGScene::Step(){
	ClearForce();
	GenerateForce();
	Integrate();
}
void SGScene::ClearForce(){
	behaviors.ClearForce(this);
}
#define FP_ERROR_MASK	(_EM_INEXACT|_EM_UNDERFLOW)
void SGScene::GenerateForce(){
//	_controlfp(FP_ERROR_MASK, _MCW_EM);	//	コメントをはずすと例外が起きる．要調査
	behaviors.GenerateForce(this);
//	_controlfp(_MCW_EM, _MCW_EM);
}
void SGScene::Integrate(){
//	_controlfp(FP_ERROR_MASK, _MCW_EM);
	behaviors.Integrate(this);
//	_controlfp(_MCW_EM, _MCW_EM);
//	time += timeStep;
	count++;
}

void SGScene::Loaded(SGScene* scene){
	names.Print(DSTR);
	behaviors.Loaded(scene);
	renderers.Loaded(scene);
	world->Loaded(scene);
	world->CalcBBox();
}
SGScene::SGScene(){
	timeStep = 0.005;
	velocityLossPerStep = 0.98;
	Clear();
}
void SGScene::Clear(){
	behaviors.Clear(this);
	renderers.Clear(this);
	if (world){
		world->Clear();
		world=NULL;
	}
	ClearName();
	Print(DSTR);
	world = new SGFrame;
	world->SetName("world", this);
	count = 0;
}
void SGScene::ClearName(){
	names.clear();
/*
	for(SGObjectNames::iterator it = names.begin(); it != names.end(); ++it){
		SGObject* obj = *it;
		DSTR << obj->RefCount() << ",";
		while(obj->RefCount() == 1){
			SGObjectNames::iterator next = it;
			++next;
			names.erase(it);
			it = next;
			if (it == names.end()) return;
			obj = *it;
		}
	}
	DSTR << "\n";
	*/
}
size_t SGScene::NChildObjects(){
	return 1 + behaviors.size() + renderers.size();
}
SGObject* SGScene::ChildObject(size_t i){
	if (i == 0) return world;
	i -= 1;
	if (i < behaviors.size()) return behaviors[i];
	i -= behaviors.size();
	if (i < renderers.size()) return renderers[i];
	return NULL;
}

static std::vector<const UTTypeInfo*> sceneChildCandidate;
const UTTypeInfo** SGScene::ChildCandidates(){
	if (sceneChildCandidate.size()==0 || sceneChildCandidate.back()){
		sceneChildCandidate.push_back(NULL);
	}
	return &*sceneChildCandidate.begin();
}
bool SGScene::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(SGBehaviorEngine, o)){
		SGBehaviorEngine* be = (SGBehaviorEngine*)o;
		behaviors.Add(be);
		return true;
	}
	GetWorld()->AddChildObject(o, s);
	return false;
}
bool SGScene::DelChildObject(SGObject* o, SGScene* s){
	if (DCAST(SGBehaviorEngine, o)){
		SGBehaviorEngine* be = (SGBehaviorEngine*)o;
		be->Clear(this);
		return behaviors.Del(be);
	}else if (DCAST(SGRenderEngine, o)){
		SGRenderEngine* re = (SGRenderEngine*)o;
		re->Clear(this);
		return renderers.Del(re);
	}
	return false;
}

class SGSceneState:public SGBehaviorState{
public:
	SGOBJECTDEF(SGSceneState);
	unsigned int count;	
};
SGOBJECTIMP(SGSceneState, SGBehaviorState);


void SGScene::LoadState(const SGBehaviorStates& states){
	states.SetCursorFirst();
	SGSceneState* st = DCAST(SGSceneState, states.GetNext());
	count = st->count;
	GetBehaviors().LoadState(states);
}
void SGScene::SaveState(SGBehaviorStates& states) const{
	states.clear();
	SGSceneState* st = new SGSceneState;
	st->count = count;
	states.push_back(st);
	GetBehaviors().SaveState(states);
}

void SGScene::SetTimeStep(double dt){
	velocityLossPerStep = pow(velocityLossPerStep, dt/timeStep);
	timeStep = dt;
}
void SGScene::SetVelocityLoss(double v){
	velocityLossPerStep = pow(v, timeStep);
}
double SGScene::GetVelocityLoss(){
	 return pow(velocityLossPerStep, 1/timeStep);
}

//----------------------------------------------------------------------------
//	DocIO
//
class SGSceneLoader:public FIBaseLoader{
public:
	virtual UTString GetNodeType() const{
		return "Scene";
	}
	virtual void Load(FILoadScene* ctx){
		FIDocNodeBase* doc = ctx->docs.Top();
		ctx->scene->SetName(doc->GetName().c_str(), ctx->scene);
		ctx->scene->doc = doc;
	}
	virtual void Loaded(FILoadScene* ctx){
	}
};
class SGSceneSaver:public FIObjectSaver<SGScene, true, false>{
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, SGScene* t){
		ctx->docs.Push(doc);
		for(unsigned i=1; i<t->NChildObjects(); ++i){
			ctx->SaveRecursive(t->ChildObject(i));
		}
		for(unsigned i=0; i<t->ChildObject(0)->NChildObjects(); ++i){
			ctx->SaveRecursive(t->ChildObject(0)->ChildObject(i));
		}
		ctx->docs.Pop();	
	}
};
DEF_REGISTER_BOTH(SGScene);

typedef float FLOAT;
DEF_RECORD(XSimulator,{
	GUID Guid(){ return WBGuid("A3AD84FC-CF04-4541-A284-D9267D58E0AA"); }
	FLOAT dt;
	FLOAT velocityLoss;
});
class SGSimulator:public SGObject{
	SGOBJECTDEF(SGSimulator);
};
SGOBJECTIMP(SGSimulator, SGObject);

class SGSimulatorLoader:public FIObjectLoader<SGSimulator>{
public:
	SGSimulatorLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XSimulator);
	}
	virtual bool LoadData(FILoadScene* ctx, SGSimulator* s){
		XSimulator sim;
		sim.dt = 0.005f;
		sim.velocityLoss = 0.98f;
		ctx->docs.Top()->GetData(sim.dt, "dt");
		ctx->docs.Top()->GetData(sim.velocityLoss, "velocityLoss");
		if (sim.velocityLoss<1e-20f || sim.velocityLoss > 1.0f){
			sim.velocityLoss = 0.98f;
		}
		ctx->scene->SetTimeStep(sim.dt);
		ctx->scene->SetVelocityLoss(sim.velocityLoss);
		return true;
	}
};

class SGSimulatorSaver:public FIObjectSaver<SGSimulator>{
	virtual UTString GetType() const{ return "SGSimulator"; }
	virtual void SaveData(FISaveScene* ctx, FIDocNodeBase* doc, SGSimulator* s){
		XSimulator sim;
		sim.dt = (float)ctx->scene->GetTimeStep();
		sim.velocityLoss = (float)ctx->scene->GetVelocityLoss();
		doc->SetWholeData(sim);
	}
};
DEF_REGISTER_BOTH(SGSimulator);
}
