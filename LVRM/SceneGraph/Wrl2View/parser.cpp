
/*  A Bison parser, made from wrl2view/parser.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ID	257
#define	DEF	258
#define	EXTERNPROTO	259
#define	IS	260
#define	NULL_NODE	261
#define	ROUTE	262
#define	PROTO	263
#define	TO	264
#define	USE	265
#define	EVENTIN	266
#define	EVENTOUT	267
#define	FIELD	268
#define	EXPOSEDFIELD	269
#define	SFBOOL	270
#define	SFCOLOR	271
#define	SFFLOAT	272
#define	SFIMAGE	273
#define	SFINT32	274
#define	SFNODE	275
#define	SFROTATION	276
#define	SFSTRING	277
#define	SFTIME	278
#define	SFVEC2F	279
#define	SFVEC3F	280
#define	MFCOLOR	281
#define	MFFLOAT	282
#define	MFINT32	283
#define	MFNODE	284
#define	MFROTATION	285
#define	MFSTRING	286
#define	MFVEC2F	287
#define	MFVEC3F	288

#line 2 "wrl2view/parser.y"

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

#line 124 "wrl2view/parser.y"
typedef union {
	char	*string;
	TField	*field;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		137
#define	YYFLAG		-32768
#define	YYNTBASE	40

#define YYTRANSLATE(x) ((unsigned)(x) <= 288 ? yytranslate[x] : 71)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,    35,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    36,     2,    37,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    38,     2,    39,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     5,     8,    10,    12,    14,    17,    19,
    23,    26,    28,    30,    39,    40,    41,    52,    53,    54,
    63,    64,    67,    71,    75,    76,    82,    83,    89,    90,
    93,    97,   101,   105,   109,   110,   116,   117,   120,   121,
   125,   127,   129,   130,   136,   140,   144,   150,   156,   158,
   160,   162,   164,   166,   168,   170,   172,   174,   176,   178,
   180,   182,   184,   186,   188,   190,   191,   195,   196,   200,
   203,   207,   211,   213,   215,   219,   221,   222
};

static const short yyrhs[] = {    -1,
    41,    42,     0,     0,    42,    43,     0,    44,     0,    45,
     0,    46,     0,     1,    43,     0,    59,     0,     4,     3,
    59,     0,    11,     3,     0,    47,     0,    50,     0,     8,
     3,    35,     3,    10,     3,    35,     3,     0,     0,     0,
     9,     3,    48,    36,    53,    37,    49,    38,    42,    39,
     0,     0,     0,     5,     3,    51,    36,    57,    37,    52,
    65,     0,     0,    53,    54,     0,    12,     3,     3,     0,
    13,     3,     3,     0,     0,    14,     3,     3,    55,    65,
     0,     0,    15,     3,     3,    56,    65,     0,     0,    57,
    58,     0,    12,     3,     3,     0,    13,     3,     3,     0,
    14,     3,     3,     0,    15,     3,     3,     0,     0,     3,
    60,    38,    61,    39,     0,     0,    61,    62,     0,     0,
     3,    63,    65,     0,    46,     0,    45,     0,     0,    14,
     3,     3,    64,    65,     0,    12,     3,     3,     0,    13,
     3,     3,     0,    12,     3,     3,     6,     3,     0,    13,
     3,     3,     6,     3,     0,    16,     0,    17,     0,    27,
     0,    18,     0,    28,     0,    19,     0,    20,     0,    29,
     0,    22,     0,    31,     0,    23,     0,    32,     0,    24,
     0,    25,     0,    33,     0,    26,     0,    34,     0,     0,
    21,    66,    68,     0,     0,    30,    67,    69,     0,     6,
     3,     0,    21,     6,     3,     0,    30,     6,     3,     0,
     7,     0,    44,     0,    36,    70,    37,     0,    44,     0,
     0,    70,    44,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   132,   133,   137,   138,   142,   143,   144,   145,   149,   150,
   157,   162,   163,   167,   172,   173,   174,   178,   179,   180,
   184,   185,   189,   190,   191,   192,   193,   194,   198,   199,
   203,   204,   205,   206,   211,   212,   216,   217,   221,   222,
   223,   224,   225,   226,   227,   228,   229,   230,   234,   235,
   236,   237,   238,   239,   240,   241,   242,   243,   244,   245,
   246,   247,   248,   249,   250,   251,   252,   253,   254,   258,
   259,   260,   264,   265,   269,   270,   274,   275
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","ID","DEF",
"EXTERNPROTO","IS","NULL_NODE","ROUTE","PROTO","TO","USE","EVENTIN","EVENTOUT",
"FIELD","EXPOSEDFIELD","SFBOOL","SFCOLOR","SFFLOAT","SFIMAGE","SFINT32","SFNODE",
"SFROTATION","SFSTRING","SFTIME","SFVEC2F","SFVEC3F","MFCOLOR","MFFLOAT","MFINT32",
"MFNODE","MFROTATION","MFSTRING","MFVEC2F","MFVEC3F","'.'","'['","']'","'{'",
"'}'","vrmlScene","@1","sts","st","nodeSt","protoSt","routeSt","proto","@2",
"@3","externProto","@4","@5","i_fs","i_f","@6","@7","externI_fs","externI_f",
"node","@8","nodeElms","nodeElm","@9","@10","field","@11","@12","sfnode","mfnode",
"nodeSts", NULL
};
#endif

static const short yyr1[] = {     0,
    41,    40,    42,    42,    43,    43,    43,    43,    44,    44,
    44,    45,    45,    46,    48,    49,    47,    51,    52,    50,
    53,    53,    54,    54,    55,    54,    56,    54,    57,    57,
    58,    58,    58,    58,    60,    59,    61,    61,    63,    62,
    62,    62,    64,    62,    62,    62,    62,    62,    65,    65,
    65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
    65,    65,    65,    65,    65,    66,    65,    67,    65,    65,
    65,    65,    68,    68,    69,    69,    70,    70
};

static const short yyr2[] = {     0,
     0,     2,     0,     2,     1,     1,     1,     2,     1,     3,
     2,     1,     1,     8,     0,     0,    10,     0,     0,     8,
     0,     2,     3,     3,     0,     5,     0,     5,     0,     2,
     3,     3,     3,     3,     0,     5,     0,     2,     0,     3,
     1,     1,     0,     5,     3,     3,     5,     5,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     0,     3,     0,     3,     2,
     3,     3,     1,     1,     3,     1,     0,     2
};

static const short yydefact[] = {     1,
     3,     0,     0,    35,     0,     0,     0,     0,     0,     4,
     5,     6,     7,    12,    13,     9,     8,     0,     0,    18,
     0,    15,    11,    37,    10,     0,     0,     0,     0,    29,
     0,    21,    39,     0,     0,     0,    36,    42,    41,    38,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    19,    30,     0,     0,     0,     0,     0,    16,    22,
     0,    49,    50,    52,    54,    55,    66,    57,    59,    61,
    62,    64,    51,    53,    56,    68,    58,    60,    63,    65,
    40,    45,    46,    43,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    70,     0,     0,     0,     0,
     0,     0,     0,    31,    32,    33,    34,    20,    14,    23,
    24,    25,    27,     3,    71,    73,    74,    67,    72,    77,
    76,    69,    47,    48,    44,     0,     0,     0,     0,    26,
    28,    17,    75,    78,     0,     0,     0
};

static const short yydefgoto[] = {   135,
     1,     2,    10,    11,    12,    13,    14,    28,    95,    15,
    26,    89,    43,    60,   126,   127,    41,    53,    16,    18,
    29,    40,    44,   103,    81,    98,   100,   118,   122,   129
};

static const short yypact[] = {-32768,
-32768,    87,    98,-32768,    -2,     3,     6,     9,    11,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,   -23,    15,-32768,
    -8,-32768,-32768,-32768,-32768,    -7,    29,     5,     8,-32768,
    24,-32768,-32768,    32,    36,    42,-32768,-32768,-32768,-32768,
    67,    43,    71,    44,    45,    46,    48,    49,    50,    51,
    52,-32768,-32768,    21,    86,    90,    91,    94,-32768,-32768,
    97,-32768,-32768,-32768,-32768,-32768,    99,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,   104,-32768,-32768,-32768,-32768,
-32768,   105,   106,-32768,   110,   111,   112,   113,    44,   114,
   115,   116,   117,   118,    84,-32768,   120,    33,   121,    22,
   122,   123,    44,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,    44,    44,    -1,    20,-32768,
-32768,-32768,-32768,-32768,   127,   128,-32768
};

static const short yypgoto[] = {-32768,
-32768,    16,   126,   -70,   102,   103,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,   119,-32768,
-32768,-32768,-32768,-32768,   -84,-32768,-32768,-32768,-32768,-32768
};


#define	YYLAST		138


static const short yytable[] = {     3,
    19,     4,     5,     6,   108,    20,     7,     8,    21,     9,
    33,    22,     6,    23,    24,     7,     8,     4,   125,    34,
    35,    36,     4,     5,     4,     5,    27,   117,    30,   121,
     9,    31,     9,    42,    45,     4,     5,   132,    46,   116,
    32,   130,   131,     9,    47,    54,    37,    82,    83,    61,
    84,    85,    86,    87,    88,    90,   133,   120,   134,    62,
    63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    48,    49,
    50,    51,    55,    56,    57,    58,    -2,     3,    91,     4,
     5,     6,    92,    93,     7,     8,    94,     9,     3,    96,
     4,     5,     6,    52,    97,     7,     8,    59,     9,    99,
   101,   102,   104,   105,   106,   107,   109,   110,   111,   112,
   113,   114,   115,   119,   123,   124,   136,   137,    17,   128,
    38,    39,     0,     0,     0,     0,     0,    25
};

static const short yycheck[] = {     1,
     3,     3,     4,     5,    89,     3,     8,     9,     3,    11,
     3,     3,     5,     3,    38,     8,     9,     3,   103,    12,
    13,    14,     3,     4,     3,     4,    35,    98,    36,   100,
    11,     3,    11,    10,     3,     3,     4,    39,     3,     7,
    36,   126,   127,    11,     3,     3,    39,     3,     3,     6,
     3,     3,     3,     3,     3,    35,    37,    36,   129,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    12,    13,
    14,    15,    12,    13,    14,    15,     0,     1,     3,     3,
     4,     5,     3,     3,     8,     9,     3,    11,     1,     3,
     3,     4,     5,    37,     6,     8,     9,    37,    11,     6,
     6,     6,     3,     3,     3,     3,     3,     3,     3,     3,
     3,    38,     3,     3,     3,     3,     0,     0,     3,   114,
    29,    29,    -1,    -1,    -1,    -1,    -1,    19
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 132 "wrl2view/parser.y"
{ StIn(); ;
    break;}
case 2:
#line 133 "wrl2view/parser.y"
{ g_sts = StOut(); ;
    break;}
case 10:
#line 150 "wrl2view/parser.y"
{ 
									mapNode[yyvsp[-1].string] = lSts.top()->back();
									TField f;
									f.push_back(new TFieldBaseString(strdup(yyvsp[-1].string)));
									lSts.top()->back()->SetElm("nodeID", &f);
									free(yyvsp[-1].string);
								;
    break;}
case 11:
#line 157 "wrl2view/parser.y"
{ if(mapNode.find(yyvsp[0].string) != mapNode.end())
									lSts.top()->push_back(mapNode[yyvsp[0].string]); free(yyvsp[0].string); ;
    break;}
case 14:
#line 167 "wrl2view/parser.y"
{ free(yyvsp[-6].string); free(yyvsp[-4].string); free(yyvsp[-2].string); free(yyvsp[0].string); ;
    break;}
case 15:
#line 172 "wrl2view/parser.y"
{ ProtoIn(yyvsp[0].string); ;
    break;}
case 16:
#line 173 "wrl2view/parser.y"
{ StIn(); ;
    break;}
case 17:
#line 174 "wrl2view/parser.y"
{ ProtoOut(StOut()); free(yyvsp[-8].string); ;
    break;}
case 18:
#line 178 "wrl2view/parser.y"
{ ProtoIn(yyvsp[0].string); ;
    break;}
case 19:
#line 179 "wrl2view/parser.y"
{ expect = MFSTRING; ;
    break;}
case 20:
#line 180 "wrl2view/parser.y"
{ ProtoOut(0); free(yyvsp[-6].string); ;
    break;}
case 23:
#line 189 "wrl2view/parser.y"
{ protos.top()->SetEventIn(yyvsp[0].string, Type(yyvsp[-1].string)); free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 24:
#line 190 "wrl2view/parser.y"
{ protos.top()->SetEventOut(yyvsp[0].string, Type(yyvsp[-1].string)); free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 25:
#line 191 "wrl2view/parser.y"
{ expect = Type(yyvsp[-1].string); ;
    break;}
case 26:
#line 192 "wrl2view/parser.y"
{ protos.top()->SetField(yyvsp[-2].string, Type(yyvsp[-3].string), yyvsp[0].field); free(yyvsp[-3].string); free(yyvsp[-2].string); ;
    break;}
case 27:
#line 193 "wrl2view/parser.y"
{ expect = Type(yyvsp[-1].string); ;
    break;}
case 28:
#line 194 "wrl2view/parser.y"
{ protos.top()->SetExposedField(yyvsp[-2].string, Type(yyvsp[-3].string), yyvsp[0].field); free(yyvsp[-3].string); free(yyvsp[-2].string); ;
    break;}
case 31:
#line 203 "wrl2view/parser.y"
{ protos.top()->SetEventIn(yyvsp[0].string, Type(yyvsp[-1].string)); free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 32:
#line 204 "wrl2view/parser.y"
{ protos.top()->SetEventOut(yyvsp[0].string, Type(yyvsp[-1].string)); free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 33:
#line 205 "wrl2view/parser.y"
{ protos.top()->SetField(yyvsp[0].string, Type(yyvsp[-1].string)); free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 34:
#line 206 "wrl2view/parser.y"
{ protos.top()->SetExposedField(yyvsp[0].string, Type(yyvsp[-1].string)); free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 35:
#line 211 "wrl2view/parser.y"
{ if(NodeIn(yyvsp[0].string)) YYERROR; ;
    break;}
case 36:
#line 212 "wrl2view/parser.y"
{ NodeOut(); free(yyvsp[-4].string); ;
    break;}
case 39:
#line 221 "wrl2view/parser.y"
{ FieldIn(yyvsp[0].string); ;
    break;}
case 40:
#line 222 "wrl2view/parser.y"
{ FieldOut(yyvsp[-2].string, yyvsp[0].field); free(yyvsp[-2].string); ;
    break;}
case 43:
#line 225 "wrl2view/parser.y"
{ expect = Type(yyvsp[-1].string); ;
    break;}
case 44:
#line 226 "wrl2view/parser.y"
{ free(yyvsp[-3].string); free(yyvsp[-2].string); ;
    break;}
case 45:
#line 227 "wrl2view/parser.y"
{ free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 46:
#line 228 "wrl2view/parser.y"
{ free(yyvsp[-1].string); free(yyvsp[0].string); ;
    break;}
case 47:
#line 229 "wrl2view/parser.y"
{ free(yyvsp[-3].string); free(yyvsp[-2].string); free(yyvsp[0].string); ;
    break;}
case 48:
#line 230 "wrl2view/parser.y"
{ free(yyvsp[-3].string); free(yyvsp[-2].string); free(yyvsp[0].string); ;
    break;}
case 66:
#line 251 "wrl2view/parser.y"
{ StIn(); ;
    break;}
case 67:
#line 252 "wrl2view/parser.y"
{ yyval.field = MakeField(StOut()); ;
    break;}
case 68:
#line 253 "wrl2view/parser.y"
{ StIn(); ;
    break;}
case 69:
#line 254 "wrl2view/parser.y"
{ yyval.field = MakeField(StOut()); ;
    break;}
case 70:
#line 258 "wrl2view/parser.y"
{ yyval.field = MakeField(yyvsp[0].string); ;
    break;}
case 71:
#line 259 "wrl2view/parser.y"
{ yyval.field = MakeField(yyvsp[0].string); ;
    break;}
case 72:
#line 260 "wrl2view/parser.y"
{ yyval.field = MakeField(yyvsp[0].string); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 278 "wrl2view/parser.y"


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
