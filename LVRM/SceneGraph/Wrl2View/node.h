// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#ifndef NODE_H_
#define NODE_H_

#pragma warning(disable:4786)

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <Base/Utilities.h>
#include <Base/VRBase.h>

namespace LVRM {
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//	Field types
class TNode;
class TSts:public vector< UTRef<TNode> >, public UTRefCount{
public:
};
UTRef<TSts> LoadWrl(char* fname);
class TField;
class TFieldBase:public UTRefCount{
public:
	virtual ~TFieldBase(){}
	virtual int GetInt(){ return 0; }
	virtual float GetFloat(){ return 0; }
	virtual char* GetString(){ return NULL; }
	virtual TField* GetRefer(int nest){ return NULL; }
	virtual TSts* GetSts(){ return NULL; }
	operator int(){ return GetInt(); }
	operator float(){ return GetFloat(); }
	operator char*(){ return GetString(); }
	operator TSts*(){ return GetSts(); }
};
class TFieldBaseInt:public TFieldBase{
	int value;
public:
	TFieldBaseInt(int v=0){ value = v; }
	virtual int GetInt(){ return value; }
};
class TFieldBaseFloat:public TFieldBase{
	float value;
public:
	TFieldBaseFloat(float v=0){ value = v; }
	virtual float GetFloat(){ return value; }
};
class TFieldBaseString:public TFieldBase{
	char* value;
public:
	TFieldBaseString(char* v=NULL){ value = v; }
	virtual ~TFieldBaseString(){ free(value); }
	virtual char* GetString(){ return value; }
};
class TFieldBaseRefer:public TFieldBase{
	char* value;
public:
	TFieldBaseRefer(char* v=NULL){ value = v; }
	virtual ~TFieldBaseRefer(){ free(value); }
	virtual TField* GetRefer(int nest);
};
class TFieldBaseSts:public TFieldBase{
	UTRef<TSts> value;
public:
	TFieldBaseSts(TSts* v){ value = v; }
	virtual TSts* GetSts(){ return value; }
};


template<class _T>
_T GetData(TFieldBase *_node) {return (_T)*_node;}
template<class _T>
void GetData(TFieldBase *_node, _T* p) { *p = (_T)*_node;}

////////////////////////////////////////////////////////////////////////////////
//	Node types

class TType;
class TField:public vector< UTRef<TFieldBase> >{
};
typedef map<string, TField>			TNodeElms;

class TNode: public UTRefCount {
public:
	string		typeName;
	TType*		type;
	TNodeElms	nodeElms;
protected:
	TNode* parent;
public:
	TNode(const char* tname, TType *tp) : typeName(tname), type(tp), parent(NULL) {}
	TType* GetType() { return type; }
	void SetElm(const char *fieldName, TField *f);
	void SetParent(TNode* p){ parent = p; }
	TNode* GetParent(){ return parent; }
	void GetPosture(Affinef& posture);
	template<class T> friend bool CopyElm(TNode *pn, const char *fieldName, T itb);
	template<class _T, class _A> friend bool PushbackElm(TNode *pn, const char *fieldName, vector<_T, _A> &c);
	TNodeElms& NodeElms(){ return nodeElms; }
	TField* GetField(const char* fieldName);
	friend class TTypeBase;
	void DoAction(void(TType::* action)(TNode*));
};

template<class T> bool CopyElm(TNode *pn, const char *fieldName, T itb)
{
	TField* field = pn->GetField(fieldName);
	if (field){
		for(TField::iterator itf = field->begin(); itf != field->end(); ++itf){
			GetData(*itf, itb++);
		}
		return true;
	}
	return false;
}

template<class _T, class _A> bool PushbackElm(TNode *pn, const char *fieldName, vector<_T, _A> &c)
{
	TField* field = pn->GetField(fieldName);
	if (field){
		c.reserve(field->size());
		for(TField::iterator itf = field->begin(); itf != field->end(); ++itf)
			c.push_back(GetData<_T>(*itf));
		return true;
	}
	return false;
}


}	//	namespace LVRM
#endif	// NODE_H_
