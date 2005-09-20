/* QUADPR.F -- translated by f2c (version 19960711).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "headrs.h"

/* Common Block Declarations */
#define qprcbi_1 qprcbi_
#define qprcbr_1 qprcbr_
#define qprcbd_1 qprcbd_
#define qprcbc_1 qprcbc_

/* Table of constant values */
static integer c__1 = 1;

//extern int quadpr_(real *, int * ,real *, real *, real * , int *, real *, char *, char *, real *, real *, real *, real *,real *,int *, real *,ftnlen,ftnlen);

/* Subroutine */ int quadpr_(a, kt, rhs, cost, quad, input, tol, title, pfile,
	 obj, x, rc, dual, slk, iout, ws, title_len, pfile_len)
real *a;
integer *kt;
real *rhs, *cost, *quad;
integer *input;
real *tol;
char *title, *pfile;
real *obj, *x, *rc, *dual, *slk;
integer *iout;
real *ws;
ftnlen title_len;
ftnlen pfile_len;
{
    /* Initialized data */

    static integer ncall0 = 0;
    static integer luprnt = 6;
    static integer lufile = 92;

    /* Format strings */
    static char fmt_70[] = "(////5x,\002QUADPR  CALL\002,i4//1x,\002*** UNAB\
LE TO OPEN THE FILE \002,a)";
    static char fmt_72[] = "(\002QDPR\002,i4.4,\002.LIS\002,52x)";
    static char fmt_74[] = "(/5x,\002OUTPUT WILL BE FILED ON \002,a12)";
    static char fmt_78[] = "(/5x,\002CANNOT OPEN THIS FILE EITHER.  CHECK YO\
UR FILES \002,\002FOR THE ABOVE NAME\002/5x,\002AND MAKE SURE LOGICAL UNI\
T\002,i4,\002 IS CLOSED.\002/)";

    /* System generated locals */
    integer i__1;
    olist o__1;
    cllist cl__1;

    /* Builtin functions */
    /* Subroutine */ int s_copy();
    integer i_len(), f_open(), s_wsfe(), do_fio(), e_wsfe(), s_wsfi(), e_wsfi(
	    ), f_clos();

    /* Local variables */
#define kobj ((integer *)&qprcbi_1 + 7)
#define ierr ((integer *)&qprcbi_1 + 14)
#define jsol ((integer *)&qprcbi_1 + 11)
#define jout ((integer *)&qprcbi_1 + 12)
#define tpiv ((real *)&qprcbr_1 + 1)
    static integer i__;
#define jdata ((integer *)&qprcbi_1 + 9)
#define maxit ((integer *)&qprcbi_1 + 6)
#define itcnt ((integer *)&qprcbi_1 + 15)
#define lenws ((integer *)&qprcbi_1 + 5)
    static integer l1, l2, l3;
#define tzero ((real *)&qprcbr_1)
    static integer l4;
#define ml ((integer *)&qprcbi_1)
#define nl ((integer *)&qprcbi_1 + 1)
    extern /* Subroutine */ int qrprt1_();
#define mo ((integer *)&qprcbi_1 + 2)
#define no ((integer *)&qprcbi_1 + 3)
    extern /* Subroutine */ int qrprt2_(), qrprt3_(), qrprt6_(), qdcomp_();
#define minmax ((integer *)&qprcbi_1 + 4)
#define jwidth ((integer *)&qprcbi_1 + 13)
    extern /* Subroutine */ int qdprep_(), qdsoln_(), qdrver_();
#define jpivot ((integer *)&qprcbi_1 + 10)
#define jit ((integer *)&qprcbi_1 + 8)

    /* Fortran I/O blocks */
    static cilist io___26 = { 0, 0, 0, fmt_70, 0 };
    static icilist io___27 = { 0, qprcbc_1.pfiles, 0, fmt_72, 64, 1 };
    static cilist io___28 = { 0, 0, 0, fmt_74, 0 };
    static cilist io___29 = { 0, 0, 0, fmt_78, 0 };



/*           MADISON ACADEMIC COMPUTING CENTER */
/*           SUBROUTINE QUADPR FOR QUADRATIC PROGRAMMING PROGRAMS */
/*           FORTRAN 77 VERSION 92.05 */

/*           QUADPR MINIMIZES OR MAXIMIZES  COST*X + X*QUAD*X */
/*                  SUBJECT TO     A*X + KT*'SLACK' = RHS */
/*                                               X .GE. 0. */
/*                  (KT(I)=-1,0,+1 IF I-TH CONSTRAINT IS .GE.,.EQ.,.LE.) 
*/
/*                  THE OBJECTIVE FUNCTION SHOULD BE CONVEX (CONCAVE) */
/*                  FOR MINIMIZATION (MAXIMIZATION) PROBLEMS. */
/*                  THIS MEANS THE SYMMETRIC MATRIX QUAD+QUAD(TRANSPOSE) 
*/
/*                  SHOULD BE NONNEGATIVE (NONPOSITIVE) DEFINITE */
/*                  FOR MINIMIZATION (MAXIMIZATION) PROBLEMS. */

/*     CALLING SEQUENCE ... */
/*     CALL QUADPR (A,KT,RHS,COST,QUAD,INPUT,TOL,TITLE,PFILE, */
/*    *   OBJ,X,RC,DUAL,SLK,IOUT,WS) */

/*     IN WHAT FOLLOWS, MO = NO. OF CONSTRAINTS, NO = NO. OF VARIABLES, */
/*     DIMENSION SIZES GIVEN ARE THE MINIMUM REQUIRED. */

/* A     --- MO X NO - CONSTRAINT MATRIX */
/* KT    --- MO-VECTOR OF CONSTRAINT TYPES,  -1,0,+1 MEAN .GE.,.EQ.,.LE. 
*/
/*           RESPECTIVELY */
/* RHS   --- MO-VECTOR OF RIGHT-HAND-SIDE VALUES */
/* COST  --- NO-VECTOR OF LINEAR COSTS */
/* QUAD  --- NO X NO MATRIX OF QUADRATIC COSTS */
/* INPUT --- FIXED CONSTANTS VECTOR OF LENGTH 21 CONTAINING ... */
/* ML      1 ROW DIMENSION OF A IN CALLING PROGRAM.  MINIMUM IS MO. */
/* NL      2 ROW DIMENSION OF QUAD IN CALLING PROGRAM.  MINIMUM IS NO. */
/* MO      3 NUMBER OF CONSTRAINTS. */
/* NO      4 NUMBER OF VARIABLES . */
/* MINMAX  5 =0 IF OBJECTIVE IS TO BE MINIMIZED, */
/*           =1 IF OBJECTIVE IS TO BE MAXIMIZED. */
/* LENWS   6 DIMENSION OF WS IN CALLING PROGRAM.  MINIMUM IS */
/*           2*(MO+NO)**2 + 12*(MO+NO) + 16 IF ANY EQUALITY CONSTRAINTS */
/*           ARE PRESENT OR  2*(MO+NO)**2 +  8*(MO+NO) + 6 */
/*           IF ALL CONSTRAINTS ARE INEQUALITIES. */
/* MAXIT   7 PIVOT LIMIT. */
/* KOBJ    8 COMPUTE OBJECTIVE AT SOLUTION OPTION.  =0 NO, =1 YES. */
/* JIT     9 OUTPUT PROBLEM PARAMETERS OPTION.   =0 NO, =1 YES. */
/* JDATA  10 OUTPUT PROBLEM DATA OPTION.  =0 NO, */
/*           =1 OUTPUT WITH A AND QUAD MATRICES IN DENSE FORM, */
/*           =2 OUTPUT WITH A AND QUAD MATRIX IN SPARSE FORM. */
/* JPIVOT 11 OUTPUT PIVOT INFORMATION.  =0 NO, =1 YES. */
/* JSOL   12 OUTPUT SOLUTION REPORT OPTION.  =0 NO, */
/*           =1 OUTPUT BRIEF REPORT,  =2 OUTPUT FULL REPORT. */
/* JOUT   13 OPTION TO PRINT OR FILE OUTPUT. */
/*           =0 PRINT ALL OUTPUT,  =1 PRINT PROBLEM PARAMETERS AND FINAL 
*/
/*           OUTPUT AND FILE OTHER OUTPUT, =2 FILE ALL OUTPUT. */
/* JWIDTH 14 MAXIMUM WIDTH OF OUTPUT LINES.  A MINIMUM OF 72 AND A */
/*           MAXIMUM OF 132 WILL BE USED. */

/* TOL   --- TOLERANCE VECTOR OF SIZE 2 CONTAINING... */
/* TZERO   1 ROUND-OFF OR ZERO TOLERANCE.  IF .LE. 0 RESET TO 1.E-7 */
/* TPIV    2 PIVOT TOLERANCE.  IF .LE. 0 RESET TO 1.E-6 */

/* TITLE --- TITLE PRINTED IN OUTPUT.  TYPE CHARACTER. */
/*           MAY CONTAIN UP TO 64 CHARACTERS. */
/*           TRUNCATED TO 64 CHARACTERS IF LONGER. */
/* PFILE --- FILE NAME QUADPR WRITES OUTPUT ONTO IF REQUESTED. */
/*           TYPE CHARACTER.  MAY CONTAIN UP TO 64 CHARACTERS. */
/*           IF FILE CANNOT BE OPENED, A FILE WILL BE CREATED */
/*           AND A MESSAGE PRINTED.  IF THE NAME IS ALL BLANKS, */
/*           OUTPUT TO BE FILED WILL BE DISCARDED. */
/* OBJ   --- OBJECTIVE VALUE. */
/* X     --- NO-VECTOR THAT WILL CONTAIN THE SOLUTION. */
/* RC    --- NO-VECTOR THAT WILL CONTAIN THE REDUCED COSTS. */
/* DUAL  --- MO-VECTOR THAT WILL CONTAIN THE DUAL SOLUTION. */
/* SLK   --- MO-VECTOR THAT WILL CONTAIN THE SLACK VALUES. */
/* IOUT  --- VECTOR OF SIZE 2, THAT WILL CONTAIN... */
/* IERR    1 STATUS CODE FROM 1 TO 6. */
/*           1 - SOLUTION IS OPTIMAL. */
/*           2 - NO SOLUTION. */
/*           3 - PIVOT LIMIT REACHED. */
/*           4 - INVALID QUAD MATRIX. */
/*           5 - PROBLEM DATA ERROR. */
/*           6 - ALGORITHM ERROR. */
/* ITCNT   2 NUMBER OF PIVOTS. */

/* WS    --- WORK SPACE ARRAY OF SIZE AT LEAST */
/*           2*(MO+NO)**2 + 12*(MO+NO) + 16 IF ANY EQUALITY CONSTRAINTS */
/*           ARE PRESENT OR  2*(MO+NO)**2 +  8*(MO+NO) + 6 */
/*           IF ALL CONSTRAINTS ARE INEQUALITIES. */






/*     QUADPR CALLS COUNTER */
    /* Parameter adjustments */
    --ws;
    --iout;
    --slk;
    --dual;
    --rc;
    --x;
    --tol;
    --input;
    --quad;
    --cost;
    --rhs;
    --kt;
    --a;

    /* Function Body */

/*     OUTPUT UNITS FOR LINE PRINTER (TERMINAL) AND OUTPUT FILE */


/*     INITIALIZATIONS */

    qprcbi_1.io1 = luprnt;
    *ierr = 0;
    *itcnt = 0;
    *tzero = tol[1];
    *tpiv = tol[2];
    ++ncall0;
    qprcbi_1.ncall = ncall0;
    for (i__ = 1; i__ <= 14; ++i__) {
	qprcbi_1.inputs[i__ - 1] = input[i__];
/* L10: */
    }
    s_copy(qprcbc_1.titles, "                                               \
                 ", 64L, 64L);
    l1 = i_len(title, title_len);
    s_copy(qprcbc_1.titles, title, l1, l1);

/*     INTERNAL TABLEAU IS (MO+NO)*(MO+NO+1) IF ALL INEQUALITIES AND */
/*     (MO+NO+1)*(MO+NO+2) IF ANY EQUALITIES SINCE 1 MORE ROW IS ADDED. */
    if (*mo <= 0) {
	goto L22;
    }
    i__1 = *mo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (kt[i__] == 0) {
	    goto L24;
	}
/* L20: */
    }
L22:
    qprcbi_1.more = 0;
    goto L26;
L24:
    qprcbi_1.more = 1;
L26:
    qprcbi_1.p = *mo + *no + qprcbi_1.more;
    qprcbi_1.p1 = qprcbi_1.p + 1;
/* Computing 2nd power */
    i__1 = qprcbi_1.p;
    qprcbi_1.psq = i__1 * i__1;

/*     SET OUTPUT UNITS AND OPEN PRINT FILE IF REQUIRED. */
    if (*jout != 1 && *jout != 2) {
	goto L80;
    }
    s_copy(qprcbc_1.pfiles, "                                               \
                 ", 64L, 64L);
    l1 = i_len(pfile, pfile_len);
    l2 = 0;
    l3 = 0;
    i__1 = l1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (*(unsigned char *)&pfile[i__ - 1] == ' ') {
	    goto L30;
	}
	++l2;
	*(unsigned char *)&qprcbc_1.pfiles[l2 - 1] = *(unsigned char *)&pfile[
		i__ - 1];
L30:
	;
    }
    if (l2 > 0) {
	goto L40;
    }
/*     BLANK FILE NAME, DISCARD OUTPUT TO BE FILED. */
    *jdata = 0;
    *jpivot = 0;
    if (*jout == 2) {
	goto L90;
    }
    *jit = 0;
    *jsol = 0;
    goto L50;
/*     OPEN PRINT FILE */
L40:
    o__1.oerr = 1;
    o__1.ounit = lufile;
    o__1.ofnmlen = 64;
    o__1.ofnm = qprcbc_1.pfiles;
    o__1.orl = 0;
    o__1.osta = "UNKNOWN";
    o__1.oacc = 0;
    o__1.ofm = 0;
    o__1.oblnk = 0;
    i__1 = f_open(&o__1);
    if (i__1 != 0) {
	goto L60;
    }
    qprcbi_1.io3 = lufile;
    if (*jout == 1) {
	goto L50;
    }
    qprcbi_1.io2 = lufile;
    goto L90;
L50:
    qprcbi_1.io2 = luprnt;
    goto L90;
/*     CANNOT OPEN FILE. MAKE ONE UP. */
L60:
    if (l3 == 1) {
	goto L76;
    }
    l3 = 1;
    io___26.ciunit = qprcbi_1.io1;
    s_wsfe(&io___26);
    do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
    do_fio(&c__1, qprcbc_1.pfiles, l2);
    e_wsfe();
    s_wsfi(&io___27);
    do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
    e_wsfi();
/*     FORM FILE NAME. */
    io___28.ciunit = qprcbi_1.io1;
    s_wsfe(&io___28);
    do_fio(&c__1, qprcbc_1.pfiles, 12L);
    e_wsfe();
    goto L40;
/*     CANNOT OPEN THIS FILE EITHER. */
L76:
    io___29.ciunit = qprcbi_1.io1;
    s_wsfe(&io___29);
    do_fio(&c__1, (char *)&lufile, (ftnlen)sizeof(integer));
    e_wsfe();
    *ierr = 5;
    goto L200;
L80:
    qprcbi_1.io2 = luprnt;
    qprcbi_1.io3 = luprnt;
L90:

/*     PARTITION WORKSPACE ARRAY. */
    l1 = (qprcbi_1.psq + qprcbi_1.p << 1) + 1;
    l2 = l1 + (qprcbi_1.p1 << 1);
    l3 = l2 + (qprcbi_1.p1 << 1);
    l4 = l3 + qprcbi_1.p1;
    qprcbi_1.lenreq = l4 + qprcbi_1.p1 - 1;

/*     CHECK INPUT DATA PARAMETERS FOR CONSISTENCY */
    qdrver_(&kt[1]);

    if (*ierr == 5) {
	goto L200;
    }
/*     QUIT IF BAD VALUES (IERR = 5) */

/*     OUTPUT PROBLEM PARAMETERS AND PROBLEM DATA IF REQUESTED. */
    if (*jit != 0) {
	qrprt1_();
    }
    if (*jdata != 0) {
	qrprt2_(&a[1], ml, &kt[1], &rhs[1], &cost[1], &quad[1], nl);
    }

/*     OUTPUT HEADER FOR INTERMEDIATE OUTPUT, IF ANY EXPECTED */
    if (*jpivot != 0) {
	qrprt3_();
    }

/*     CONSTRUCT INITIAL TABLEAU FOR QUADRATIC PROGRAMMING. */
    qdprep_(&a[1], ml, &kt[1], &rhs[1], &cost[1], &quad[1], nl, &ws[1], &
	    qprcbi_1.p);

/*     PERFORM PRINCIPAL PIVOTING TO FINAL TABLEAU. */
    qdcomp_(&ws[1], &ws[l1], &ws[l2], &ws[l3], &ws[l4]);

/*     GET SOLUTION AND OBJECTIVE VALUE IF REQUESTED. */
    qdsoln_(&kt[1], &cost[1], &quad[1], nl, &ws[1], &qprcbi_1.p, &ws[l4], &x[
	    1], &rc[1], &dual[1], &slk[1], &ws[l1], obj);

/*     OUTPUT SOLUTION REPORT IF REQUESTED. */
    if (*jsol != 0) {
	qrprt6_(&x[1], &rc[1], &dual[1], &slk[1], obj);
    }

/*     SET EXIT VALUES, IOUT. */
L200:
    iout[1] = *ierr;
    iout[2] = *itcnt;

/*     CLOSE PRINT FILE IF REQUIRED. */
    if (*jout > 0) {
	cl__1.cerr = 0;
	cl__1.cunit = lufile;
	cl__1.csta = "KEEP";
	f_clos(&cl__1);
    }

    return 0;

} /* quadpr_ */

#undef jit
#undef jpivot
#undef jwidth
#undef minmax
#undef no
#undef mo
#undef nl
#undef ml
#undef tzero
#undef lenws
#undef itcnt
#undef maxit
#undef jdata
#undef tpiv
#undef jout
#undef jsol
#undef ierr
#undef kobj


