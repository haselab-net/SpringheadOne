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
	s->names.Del(this);
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
	return true;
}
void SGObjectNames::Print(std::ostream& os) const{
	for(const_iterator it = begin(); it != end(); ++it){
		os << (*it)->GetName() << " : " << (*it)->GetTypeInfo()->ClassName();
		os << std::endl;
	}
}

}

