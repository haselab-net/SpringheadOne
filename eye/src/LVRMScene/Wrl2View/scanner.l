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
#pragma optimize ("awgity", on)

#include "node.h"
namespace Spr{
#include "tokendef.h"
extern void yyerror(const char *);
}
#include <string.h>
#include <io.h>

using namespace Spr;

int expect = 0;

extern "C" int yywrap() { yylineno = 1; expect = 0; return 1; }

void yy_delete_buffer YY_PROTO(( YY_BUFFER_STATE b ));
void YYCleanup(){
	yy_delete_buffer(YY_CURRENT_BUFFER);
}



%}
%option	yylineno
%x		SFB SFI MFI SFF MFF SFS MFS IN_STR

ws		([ \t\r\n,]|(#.*))+
int32	([+\-]?(([0-9]+)|(0[xX][0-9a-fA-F]+)))
float	([+\-]?((([0-9]+(\.)?)|([0-9]*\.[0-9]+))([eE][+\-]?[0-9]+)?))
idFirst	([^\x30-\x39\x00-\x20\x22\x23\x27\x2b-\x2e\x5b-\x5d\x7b\x7d\x7f])
idRest	([^\x00-\x20\x22\x23\x27\x2c\x2e\x5b-\x5d\x7b\x7d\x7f])

%%
%{
	int inMfNode = 0, fieldCnt = 1, procCnt = 1;

	static TField	s_field;
	s_field.clear();
	yylval.field = &s_field;	/* default value */

	int bk_expect = expect;
	expect = 0;
	switch(bk_expect) {
	case 0:		BEGIN INITIAL;	break;
	case SFBOOL:	BEGIN SFB;	break;
	case SFSTRING:	BEGIN SFS;	break;
	case MFSTRING:	BEGIN MFS;	break;
	case SFIMAGE:		procCnt = 2;
	case SFINT32:	BEGIN SFI;	break;
	case MFINT32:	BEGIN MFI;	break;

	case SFROTATION:	++procCnt;
	case SFCOLOR:
	case SFVEC3F:		++procCnt;
	case SFVEC2F:		++procCnt;
	case SFFLOAT:
	case SFTIME:	BEGIN SFF;	break; /* SFTime is double. but we ignore it */
	case MFROTATION:	++procCnt;	++fieldCnt;
	case MFCOLOR:
	case MFVEC3F:		++procCnt;	++fieldCnt;
	case MFVEC2F:		++procCnt;	++fieldCnt;
	case MFFLOAT:	BEGIN MFF;	break;

	case MFNODE:
	case SFNODE:	return bk_expect;
	}
%}


<SFS,MFS>\"				{ BEGIN IN_STR; }

<IN_STR>\\[\\\"]		{ yymore(); }
<IN_STR>[^\"]			{ yymore(); }
<IN_STR>\"				{	yytext[strlen(yytext) - 1] = 0;
							s_field.push_back(new TFieldBaseString(strdup(yytext)));
							if(!inMfNode) return bk_expect;
							BEGIN MFS;
						}

<MFI,MFF,MFS>\[			{	if(inMfNode) yyerror("Redundant '['");
							inMfNode = 1;
						}
<MFI,MFF,MFS>\]			{	if(!inMfNode) yyerror("Unexpected ']'");
							return bk_expect;
						}

<SFB>(TRUE)|(FALSE)		{ s_field.push_back(new TFieldBaseInt(yytext[0] == 'T')); return bk_expect; }

<SFI,MFI>{int32}		{	int i;
							if(yytext[1] == 'x' || yytext[1] == 'X')
								sscanf(yytext, "%x", &i);
							else
								i = atoi(yytext);
							s_field.push_back(new TFieldBaseInt(i));
							if(--procCnt == 0){
								if(bk_expect == SFIMAGE && s_field.size() == 2){
									procCnt = GetData<int>(s_field.front()) * i + 1;
								}else{
									if(!inMfNode) return bk_expect;
									procCnt = fieldCnt;
								}
							}
						}
<SFF,MFF>{float}		{	s_field.push_back(new TFieldBaseFloat(atof(yytext)));
							if(--procCnt == 0){
								if(!inMfNode) return bk_expect;
								procCnt = fieldCnt;
							}
						}

<INITIAL>DEF			{ return DEF; }
<INITIAL>EXTERNPROTO	{ return EXTERNPROTO; }
<INITIAL>NULL			{ return NULL_NODE; }
<INITIAL>ROUTE			{ return ROUTE; }
<INITIAL>PROTO			{ return PROTO; }
<INITIAL>TO				{ return TO; }
<INITIAL>USE			{ return USE; }
<INITIAL>eventIn		{ return EVENTIN; }
<INITIAL>eventOut		{ return EVENTOUT; }
<INITIAL>exposedField	{ return EXPOSEDFIELD; }
<INITIAL>field			{ return FIELD; }

<INITIAL,SFB,SFI,MFI,SFF,MFF,SFS,MFS>IS		{ return IS; }

<INITIAL>{idFirst}{idRest}*		{ yylval.string = strdup(yytext); return ID; }

<INITIAL,SFB,SFI,MFI,SFF,MFF,SFS,MFS>{ws}
<INITIAL,SFB,SFI,MFI,SFF,MFF,SFS,MFS>.		{ return yytext[0]; }

%%
