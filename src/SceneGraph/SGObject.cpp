#include "SceneGraph.h"
#pragma hdrstop
#include <sstream>

namespace Spr {;

//----------------------------------------------------------------------------
//	SGObject
SGOBJECTIMPBASE(SGObject);

void SGObject::SetName(const char* n, SGScene* s){
	if (name.compare(n) == 0) return;
	assert(s);
	if (name.length()) s->names.Del(this);
	name = n;
	s->names.Add(this);
}
SGObject::~SGObject()
{
	if (doc){
		doc->ClearChildren();
		doc->SetParent(NULL);//ドキュメントを親から削除
	}
}
void SGObject::Print(std::ostream& os) const {
	int w = os.width();
	os.width(0);
	os << UTPadding(w);
	os << "<" << GetTypeInfo()->ClassName() << " " << name.c_str() << ">" << std::endl;
	os.width(w);
}
const UTTypeInfo** SGObject::ChildCandidates(){
	static const UTTypeInfo* rv [] = { NULL };
	return rv;
}


}

