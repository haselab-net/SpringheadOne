#include "SceneGraph.h"
#pragma hdrstop
#include <sstream>
#include <FLOAT.H>

namespace Spr{;
//----------------------------------------------------------------------------
//	SGEngine
SGOBJECTIMP(SGEngine, SGObject);

//----------------------------------------------------------------------------
//	SGEngines
SGEngines::SGEngines(){
}
void SGEngines::Add(SGEngine* e){
	push_back(e);
	for(SGObjects::iterator it = objs.begin(); it != objs.end(); ++it){
		e->Set(*it);
	}
}
bool SGEngines::Del(SGEngine* e){
	SGEngines::iterator it = std::find(begin2(), end2(), UTRef<SGEngine>(e));
	if (it != end()){
		erase(it);
		return true;
	}
	return false;
}
void SGEngines::Clear(SGScene* s){
	for(iterator it = base::begin(); it != base::end(); ++it) (*it)->Clear(s);
	clear();
	objs.clear();
}
void SGEngines::Loaded(SGScene* scene){
	for(unsigned i=0; i<size(); ++i){
		begin()[i]->Loaded(scene);
	}
}
SGEngine* SGEngines::FindInherit(const UTTypeInfo& ti) const {
	for(const_iterator it = base::begin(); it != base::end(); ++it){
//		DSTR << (*it)->GetTypeInfo()->className << std::endl;
		if ((*it)->GetTypeInfo()->Inherit(&ti)){
			return *it;
		}
	}
	return NULL;
}
SGEngine* SGEngines::FindInherit(const UTTypeInfo& ti, UTString name) const {
	for(const_iterator it = base::begin(); it != base::end(); ++it){
		if ((*it)->GetTypeInfo()->Inherit(&ti) && name.compare((*it)->GetName())==0){
			return *it;
		}
	}
	return NULL;
}
void SGEngines::Save(FISaveScene* ctx){
	for(const_iterator it = begin(); it != end(); ++it){
		ctx->SaveRecursive(*it);
	}
	for(SGObjects::iterator it = objs.begin(); it!= objs.end(); ++it){
		ctx->SaveRecursive(*it);
	}
}

}
