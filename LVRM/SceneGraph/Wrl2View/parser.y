/* -*- c -*- */
%{
/*
// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.
*/
#pragma warning(disable:4786)

#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>

#include "type.h"
#include "node.h"

#ifndef	_MSC_VER
#include <string.h>				// for strrchr()
#include <unistd.h>				// for chdir()
#define GetLastSlash(fname) strrchr(fname, '/')
#else
#include <windows.h>
#include <winuser.h>			// for CharNext()
#include <direct.h>				// for _chdir()
#define chdir(path) _chdir(path)

extern int yylex(void);
void YYCleanup();
void yyrestart(FILE *input_file);
extern FILE *yyin;
extern int yylineno;
extern int expect;

namespace LVRM {

extern int yyparse();
extern void yyerror(const char *msg);
static void ReportError(const char *fmt, const char *msg);
static int Type(const char *type);
static TField* MakeField(TSts *st);
static TField* MakeField(char* refer);

static void StIn();
static UTRef<TSts> StOut();
static void ProtoIn(const char *protoName);
static void ProtoOut(TSts *_p);
static int NodeIn(const char *typeName);
static void NodeOut();
static void FieldIn(const char *fieldName);
static void FieldOut(const char *fieldName, TField *);

static std::stack< UTRef<TSts> >		lSts;
static std::stack<TTypeProto*>		protos;
static std::map<string,TNode*>		mapNode;
static UTRef<TSts> g_sts;		/* VRML-File Statement Tree */


////////////////////////////////////////////////////////////////////////////////
//	pathname functions
char* GetLastSlash(char *p)
{
	char	*ret = 0;
	for( ; *p; p = CharNext(p)){
		if(*p == '/' || *p == '\\')
			ret = p;
	}
	return ret;
}
#endif	// _MSC_VER

/// chdir to open external file.(texture image file etc.)
static char* SetBaseDir(char *fname){
	char buf[4096];
	strcpy(buf, fname);
	char	*p = GetLastSlash(buf);
	if(p){
		*p = 0;
		chdir(buf);
		return p+1;
	}
	return fname;
}
///	load wrl file.
static char *fnwrl;
UTRef<TSts> LoadWrl(char* url){
	g_sts = NULL;
	yyin = fopen(url, "r");
	if (!yyin){
		fprintf(stderr, "cannot open %s\n", url);
		return NULL;
	}
	fnwrl = SetBaseDir(url);
	static const bool isSetuped = TType::SetupType();
	yyrestart(yyin);
	yyparse();
	YYCleanup();
	fclose(yyin);
	UTRef<TSts> rv = g_sts;
	g_sts = NULL;
	return rv;
}
map<string, TNode*>& NodeIDMap(){
	return mapNode;
}
void yyerror(const char *msg)
{
	fprintf(stderr, "Error in file %s line %d: %s\n", fnwrl, yylineno, msg);
}
%}

%type	<field> field
%token	<string> ID
%token	DEF EXTERNPROTO IS NULL_NODE ROUTE PROTO TO USE
%token	EVENTIN EVENTOUT FIELD EXPOSEDFIELD
%token	<field> SFBOOL SFCOLOR SFFLOAT SFIMAGE SFINT32 SFNODE
%token	<field> SFROTATION SFSTRING SFTIME SFVEC2F SFVEC3F
%token	<field> MFCOLOR MFFLOAT MFINT32 MFNODE
%token	<field> MFROTATION MFSTRING MFVEC2F MFVEC3F

%union {
	char	*string;
	TField	*field;
};

%%

vrmlScene
	:							{ StIn(); }
		sts						{ g_sts = StOut(); }
	;

sts
	:	/* Empty */
	|	sts st
	;

st
	:	nodeSt
	|	protoSt
	|	routeSt
	|	error st
	;

nodeSt
	:	node
	|	DEF ID node				{ 
									mapNode[$2] = lSts.top()->back();
									TField f;
									f.push_back(new TFieldBaseString(strdup($2)));
									lSts.top()->back()->SetElm("nodeID", &f);
									free($2);
								}
	|	USE ID					{ if(mapNode.find($2) != mapNode.end())
									lSts.top()->push_back(mapNode[$2]); free($2); }
	;

protoSt
	:	proto
	|	externProto
	;

routeSt
	:	ROUTE ID '.' ID TO ID '.' ID	{ free($2); free($4); free($6); free($8); }
	;


proto
	:	PROTO ID				{ ProtoIn($2); }
			'[' i_fs ']'		{ StIn(); }
			'{' sts '}'			{ ProtoOut(StOut()); free($2); }
	;

externProto
	:	EXTERNPROTO ID			{ ProtoIn($2); }
			'[' externI_fs ']'	{ expect = MFSTRING; }
			field				{ ProtoOut(0); free($2); }
	;

i_fs
	:	/* Empty */
	|	i_fs i_f
	;

i_f
	:	EVENTIN ID ID			{ protos.top()->SetEventIn($3, Type($2)); free($2); free($3); }
	|	EVENTOUT ID ID			{ protos.top()->SetEventOut($3, Type($2)); free($2); free($3); }
	|	FIELD ID ID				{ expect = Type($2); }
			field				{ protos.top()->SetField($3, Type($2), $5); free($2); free($3); }
	|	EXPOSEDFIELD ID	ID		{ expect = Type($2); }
			field				{ protos.top()->SetExposedField($3, Type($2), $5); free($2); free($3); }
	;

externI_fs
	:	/* Empty */
	|	externI_fs externI_f
	;

externI_f
	:	EVENTIN ID ID			{ protos.top()->SetEventIn($3, Type($2)); free($2); free($3); }
	|	EVENTOUT ID ID			{ protos.top()->SetEventOut($3, Type($2)); free($2); free($3); }
	|	FIELD ID ID				{ protos.top()->SetField($3, Type($2)); free($2); free($3); }
	|	EXPOSEDFIELD ID ID		{ protos.top()->SetExposedField($3, Type($2)); free($2); free($3); }
	;


node
	:	ID						{ if(NodeIn($1)) YYERROR; }
			'{' nodeElms '}'	{ NodeOut(); free($1); }
	;

nodeElms
	:	/* Empty */
	|	nodeElms nodeElm
	;

nodeElm
	:	ID						{ FieldIn($1); }
			field				{ FieldOut($1, $3); free($1); }
	|	routeSt
	|	protoSt
	|	FIELD ID ID				{ expect = Type($2); }
			field				{ free($2); free($3); }
	|	EVENTIN ID ID			{ free($2); free($3); }
	|	EVENTOUT ID ID			{ free($2); free($3); }
	|	EVENTIN ID ID IS ID		{ free($2); free($3); free($5); }
	|	EVENTOUT ID ID IS ID	{ free($2); free($3); free($5); }
	;

field
	:	SFBOOL
	|	SFCOLOR
	|	MFCOLOR
	|	SFFLOAT
	|	MFFLOAT
	|	SFIMAGE
	|	SFINT32
	|	MFINT32
	|	SFROTATION
	|	MFROTATION
	|	SFSTRING
	|	MFSTRING
	|	SFTIME
	|	SFVEC2F
	|	MFVEC2F
	|	SFVEC3F
	|	MFVEC3F
	|	SFNODE					{ StIn(); }
			sfnode				{ $$ = MakeField(StOut()); }
	|	MFNODE					{ StIn(); }
			mfnode				{ $$ = MakeField(StOut()); }
/*	|	IS ID					{ $$ = 0; free($2); }
	|	SFNODE IS ID			{ $$ = 0; free($3); }
	|	MFNODE IS ID			{ $$ = 0; free($3); }
*/	|	IS ID					{ $$ = MakeField($2); }
	|	SFNODE IS ID			{ $$ = MakeField($3); }
	|	MFNODE IS ID			{ $$ = MakeField($3); }
	;

sfnode
	:	NULL_NODE
	|	nodeSt
	;

mfnode
	:	'[' nodeSts ']'
	|	nodeSt
	;

nodeSts
	:	/* Empty */
	|	nodeSts nodeSt
	;

%%

static void ReportError(const char *fmt, const char *msg)
{
	char	tmp[512];
	sprintf(tmp, fmt, msg);
	yyerror(tmp);
}

static int Type(const char *type)
{
	typedef struct {
		char		name[12];
		int			id;
	}TFType;
	static const TFType	ts[] = {
		{"MFColor",	MFCOLOR	},
		{"MFFloat",	MFFLOAT	},
		{"MFInt32",	MFINT32	},
		{"MFNode",	MFNODE	},
		{"MFRotation",	MFROTATION	},
		{"MFString",	MFSTRING	},
		{"MFVec2f",	MFVEC2F	},
		{"MFVec3f",	MFVEC3F	},
		{"SFBool",	SFBOOL	},
		{"SFColor",	SFCOLOR	},
		{"SFFloat",	SFFLOAT	},
		{"SFImage",	SFIMAGE	},
		{"SFInt32",	SFINT32	},
		{"SFNode",	SFNODE	},
		{"SFRotation",	SFROTATION	},
		{"SFString",	SFSTRING	},
		{"SFTime",	SFTIME	},
		{"SFVec2f",	SFVEC2F	},
		{"SFVec3f",	SFVEC3F	},
	};
	typedef int (CMPFUNC)(const void *, const void *);
	TFType	*p = (TFType*)bsearch(type, ts, sizeof(ts)/sizeof(*ts),
								  sizeof(*ts), (CMPFUNC*)strcmp);
	if(p)
		return p->id;

	ReportError("Wrong field type '%s'", type);
	return 0;
}

static TField* MakeField(char* refer){
	static TField	fv;
	fv.clear();
	fv.push_back(new TFieldBaseRefer(refer));
	return &fv;
}

static TField* MakeField(TSts *st)
{
	static TField	fv;
	fv.clear();
	fv.push_back(new TFieldBaseSts(st));
	return &fv;
}


static void StIn()
{
	lSts.push(new TSts());
}
static UTRef<TSts> StOut()
{
	UTRef<TSts>	st = lSts.top();
	lSts.pop();
	return st;
}

static void ProtoIn(const char *protoName)
{
	protos.push(new TTypeProto(protoName));
	TType::ScopeIn();
}
static void ProtoOut(TSts *_p)
{
	TType::ScopeOut();
	protos.top()->SetSts(_p);
	protos.pop();
}

static int NodeIn(const char *typeName)
{
	TType	*t = TType::Find(typeName);
	if(t){
		lSts.top()->push_back(new TNode(typeName, t));
	}else{
		ReportError("Undefined node type '%s'", typeName);
		return 1;
	}
	return 0;
}
static void NodeOut()
{
}

static void FieldIn(const char *fieldName)
{
	TType	*nodeType = lSts.top()->back()->GetType();

	if(nodeType->IsEvent(fieldName))
		return;

	expect = nodeType->GetField(fieldName);
	if(expect == 0)
		ReportError("Undefined fields name '%s'", fieldName);
}
static void FieldOut(const char *fieldName, TField *fv)
{
	if(fv){
		lSts.top()->back()->SetElm(fieldName, fv);
	}
}

}	//	namespace LVRM
