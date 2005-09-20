// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "type.h"
namespace LVRM{
#include "tokendef.h"
}

#include <iostream>
#include <cstdlib>

namespace LVRM {

////////////////////////////////////////////////////////////////////////////////
//	TType

TType::TTypes TType::typeList; // static
TType::TListener* TType::listener;
vector<class TNode*> TType::protoStack;


void TType::ScopeIn()
{
	typeList.push_back(0);
}

void TType::ScopeOut()
{
	TTypes::reverse_iterator	it;
	for(it = typeList.rbegin(); it != typeList.rend(); ++it) {
		typeList.pop_back();
		if(!*it)
			break;
		delete *it;
	}
}

void TType::SetScope()
{
	if(Find(GetName())) {
		cerr << GetName() << " is redefined." << endl;
		return;
	}
	typeList.push_back(this);
}

TType* TType::Find(const char *name)
{
	TTypes::iterator	it;
	for(it = typeList.begin(); it != typeList.end(); ++it) {
		if(*it && !strcmp((*it)->GetName(), name))
			return *it;
	}
	return 0;
}

void TType::ClearEnumFlags(){
	for(TTypes::iterator it = typeList.begin(); it != typeList.end(); ++it){
		(*it)->bEnumNode = false;
	}
}
void TType::SetEnumFlags(TNodeClass c){
	if (c & SHAPE){
		for(TTypes::iterator it = typeList.begin(); it != typeList.end(); ++it){
			if (strcmp((*it)->GetName(), "Sphere")==0){
				(*it)->bEnumNode = true;
			}
		}
			
		}
	}



////////////////////////////////////////////////////////////////////////////////
//	TTypeProto

void TTypeProto::SetFields(const ProtoInit **ppi)
{
	while((*ppi)->mode){
		SetFields((*ppi)->mode, (*ppi)->type, (*ppi)->name);
		++(*ppi);
	}
}

void TTypeProto::SetFields(int mode, int type, const char *name)
{
	switch(mode){
	case FIELD:
		SetField(name, type);
		break;
	case EVENTIN:
		SetEventIn(name, type);
		break;
	case EVENTOUT:
		SetEventOut(name, type);
		break;
	default:
		SetExposedField(name, type);
		break;
	};
};

void TTypeProto::SetExposedField(const char *name, int type)
{
	fields[name] = type;
	eventIns[string("set_") + name] = type;
	eventOuts[string(name) + "_changed"] = type;
};

bool TTypeProto::IsEvent(const char *name) const
{
	return(eventIns.find(name) != eventIns.end() ||
			eventOuts.find(name) != eventOuts.end());
}

int TTypeProto::GetField(const char *name) const
{
	TFieldNames::const_iterator it;
	if((it = fields.find(name)) != fields.end())
		return it->second;
	return 0;
}

void TTypeProto::Traverse(void(TType::* action)(TNode*), TNode *pn){
	protoStack.push_back(pn);
	if(pSts){
		for(TSts::iterator	itn = pSts->begin(); itn != pSts->end(); ++itn){
			(*itn)->DoAction(action);
		}
	}
	protoStack.pop_back();
}

////////////////////////////////////////////////////////////////////////////////
//	TTypeBase

void TTypeBase::Traverse(void(TType::* action)(TNode*), TNode *pn)
{								// Traverse tree and do action
	TNodeElms::iterator		it;
	for(it = pn->nodeElms.begin(); it != pn->nodeElms.end(); ++it){
		int	type = GetField(it->first.c_str());
		if(type == SFNODE || type == MFNODE){
			TField* field = pn->GetField(it->first.c_str());
			for(TField::iterator it = field->begin(); it != field->end(); ++it){
				TSts* sts = (*it)->GetSts();
				for(TSts::iterator	itt = sts->begin(); itt != sts->end(); ++itt){
					(*itt)->DoAction(action);
				}
			}
		}
	}
}



////////////////////////////////////////////////////////////////////////////////
//	TNode

void TNode::DoAction(void(TType::* action)(TNode*))
{
	(type->*action)(this);
}

}	//	namespace LVRM
