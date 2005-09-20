#include "SceneGraph.h"
#pragma hdrstop
#include <sstream>
#include <FLOAT.H>

namespace Spr{;
//----------------------------------------------------------------------------
//	SGBehaviorEngine
SGOBJECTIMP(SGBehaviorEngine, SGEngine);

//----------------------------------------------------------------------------
//	SGBehaviorEngines
SGBehaviorEngines::SGBehaviorEngines(){
	Clear(NULL);
}

void SGBehaviorEngines::Clear(SGScene* s){
	SGEngines::Clear(s);
	initializer = forceGenerator = dynamicalSystem = motion = listener = end2();
}

void SGBehaviorEngines::ClearForce(SGScene* scene){
	for(iterator it = begin2(); it != forceGenerator && it!=end2(); ++it)
		UTRef<SGBehaviorEngine>(*it)->Step(scene);
}

void SGBehaviorEngines::GenerateForce(SGScene* scene){
	for(iterator it = forceGenerator; it != dynamicalSystem && it!=end2(); ++it){
//		UTRef<SGBehaviorEngine>(*it)->Print(DSTR);

		UTRef<SGBehaviorEngine>(*it)->Step(scene);
	}
}

void SGBehaviorEngines::Integrate(SGScene* scene){
//	for(iterator it = dynamicalSystem; it != motion && it!=end2(); ++it)	本当はmotionまでにすべきだけど，
	for(iterator it = dynamicalSystem; it!=end2(); ++it)
		UTRef<SGBehaviorEngine>(*it)->Step(scene);
}

void SGBehaviorEngines::Loaded(SGScene* scene){
	SGEngines::Loaded(scene);
}

void SGBehaviorEngines::Sort(){
	//エンジンをプライオリティに従ってソート
	UTContentsLess< UTRef<SGEngine> > less;
	std::sort(begin2(), end2(), less);

	//各カテゴリの先頭へのイテレータを記憶
	initializer = forceGenerator = dynamicalSystem = motion = listener = end2();
	for(iterator it = begin2(); it != end2(); ++it){
		if(initializer == end2() && (*it)->GetPriority() >= SGBP_INITIALIZER)
			initializer = it;

		if(forceGenerator == end2() && (*it)->GetPriority() >= SGBP_FORCEGENERATOR)
			forceGenerator = it;

		if (dynamicalSystem == end2() && (*it)->GetPriority() >= SGBP_DYNAMICALSYSTEM)
			dynamicalSystem = it;

		if (motion == end2() && (*it)->GetPriority() >= SGBP_MOTION)
			motion = it;

		if (listener == end2() && (*it)->GetPriority() >= SGBP_LISTENER)
			listener = it;
	}
}

void SGBehaviorEngines::Add(SGEngine* e){
	SGEngines::Add(e);
	Sort();
}
bool SGBehaviorEngines::Del(SGEngine* e){
	bool rv = SGEngines::Del(e);
	Sort();
	return rv;
}

void SGBehaviorEngines::LoadState(const SGBehaviorStates& states){
	for(const_iterator it = begin(); it != end(); ++it){
		((SGBehaviorEngine*) &**it)->LoadState(states);
	}
}
void SGBehaviorEngines::SaveState(SGBehaviorStates& states) const{
	for(const_iterator it = begin(); it != end(); ++it){
		((SGBehaviorEngine*) &**it)->SaveState(states);
	}
}

//----------------------------------------------------------------------------
//	SGBehaviorState
SGOBJECTIMPBASE(SGBehaviorState);

}
