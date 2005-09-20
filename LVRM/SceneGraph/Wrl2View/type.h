// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#ifndef TYPE_H_
#define TYPE_H_
#pragma warning(disable:4786)

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "node.h"

namespace LVRM {
using namespace std;


///	base class of node type
class TType {
public:
	bool bEnumNode;
	static vector<class TNode*> protoStack;
	class TListener{
	public:
		virtual void FoundNamed(TNode *pn){}
		virtual bool FoundTypeNode(TNode *pn){return true;}
		virtual void FoundTransformBefore(TNode *pn){}
		virtual void FoundTransformAfter(TNode *pn){}
	};
	static TListener* listener;
	void ClearEnumFlags();
	enum TNodeClass {SHAPE=0, TRANSLATE=1};
	void SetEnumFlags(TNodeClass c);
	virtual ~TType() { }

	virtual const char* GetName() const = 0;
	virtual bool IsEvent(const char *name) const = 0;
	virtual int GetField(const char *name) const = 0;

	//	treverse tree
	virtual void Traverse(void(TType::* action)(TNode*), TNode *pn)=0;

	// actions
	virtual void Init(TNode *pn) = 0;
	virtual void Draw(TNode *pn) = 0;
	virtual void EnumTypeNode(TNode* pn) = 0;
	virtual void EnumNamedNode(TNode *pn) = 0;
	
	static void ScopeIn();
	static void ScopeOut();
	static TType *Find(const char *name);
	static bool SetupType();

 protected:
	void SetScope();

 private:
	class TTypes: public vector<TType*>{
	public:
		~TTypes(){ TType::ScopeOut(); }
	};
	static TTypes typeList;
};


class TTypeProto : public TType {
 public:
	struct ProtoInit{
		int mode;
		int type;
		const char *name;
	};
	TTypeProto(const char *name) : m_name(name), pSts(0) { SetScope(); }

	const char* GetName() const { return m_name.c_str(); }
	bool IsEvent(const char *name) const;
	int GetField(const char *name) const;
	void Init(TNode *pn){ Traverse(TType::Init, pn); }
	void Draw(TNode *pn){ Traverse(TType::Draw, pn); }
	void EnumNamedNode(TNode *pn){
		char* nodeID = NULL;
		if (CopyElm(pn, "nodeID", &nodeID)){
			if (listener) listener->FoundNamed(pn);
		}else{
			Traverse(&TType::EnumNamedNode, pn);
		}
	}
	void EnumTypeNode(TNode *pn){
		if (listener){
			if (listener->FoundTypeNode(pn)) Traverse(TType::EnumTypeNode, pn);
		}
	}

	void SetSts(TSts *_p) { pSts = _p; }
	void SetEventIn(const char *name, int type) { eventIns[name] = type; }
	void SetEventOut(const char *name, int type) { eventOuts[name] = type; }
	void SetField(const char *name, int type) {
		fields[name] = type;
	}
	void SetField(const char *name, int type, TField *f) {
		SetField(name,type);
		defaults[name].clear();
		defaults[name] = *f;
	}
	void SetExposedField(const char *name, int type);
	void SetExposedField(const char *name, int type, TField *f) {
		SetExposedField(name,type);
		defaults[name].clear();
		defaults[name] = *f;
	}
	void SetFields(const ProtoInit **ppi);
 protected:
	void SetFields(int mode, int type, const char *name);
	void Traverse(void(TType::* action)(TNode*), TNode *pn);
 private:
	
	string	m_name;

	typedef map<string, int>	TFieldNames;
	TFieldNames eventIns;
	TFieldNames eventOuts;
	TFieldNames fields;

 public:
	TNodeElms	defaults;
	UTRef<TSts>	pSts;				/* PROTO statements */
};


class TTypeBase : public TTypeProto {
 public:
	TTypeBase(const char *name) : TTypeProto(name) {}
	void Traverse(void(TType::* action)(TNode*), TNode *pn);
	void Init(TNode *pn) { Traverse(&TType::Init, pn); }
	void Draw(TNode *pn) { Traverse(&TType::Draw, pn); }
};

}	//	namespace LVRM
#endif	// TYPE_H_
