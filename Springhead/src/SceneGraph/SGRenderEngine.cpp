#include "SceneGraph.h"
#pragma hdrstop
#include <sstream>
#include <FLOAT.H>

namespace Spr{;
//----------------------------------------------------------------------------
//	SGBehaviorEngine
SGOBJECTIMP(SGRenderEngine, SGEngine);

//----------------------------------------------------------------------------
void SGRenderEngines::Render(SGScene* s){
	for(iterator it = begin2(); it != end2(); ++it) UTRef<SGRenderEngine>(*it)->Render(s);
}

}
