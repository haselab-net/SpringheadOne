typedef union {
	char	*string;
	TField	*field;
} YYSTYPE;
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


extern YYSTYPE yylval;
