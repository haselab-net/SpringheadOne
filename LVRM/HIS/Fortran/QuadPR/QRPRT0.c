/* QRPRT0.F -- translated by f2c (version 19960711).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "headrs.h"
/* Common Block Declarations */

#define qprcbi_1 qprcbi_
/*
struct {
    real tols[2];
} qprcbr_;
*/
#define qprcbr_1 qprcbr_

#define qprcbd_1 qprcbd_

#define qprcbc_1 qprcbc_

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int qrprt0_0_(n__, a, lda, kt, rhs, cost, quad, ldq, obj, x, 
	rc, dual, slk)
int n__;
real *a;
integer *lda, *kt;
real *rhs, *cost, *quad;
integer *ldq;
real *obj, *x, *rc, *dual, *slk;
{
    /* Initialized data */

    static char iam[64+1] = "MADISON ACADEMIC COMPUTING CENTER - QUADPR VERS\
ION 92.05        ";
    static char blank[56+1] = "                                             \
           ";
    static char heding[40*4+1] = "  P R O B L E M    P A R A M E T E R S    \
      P R O B L E M    D A T A         I N T E R M E D I A T E    O U T P U \
T      S O L U T I O N    R E P O R T     ";
    static char txtinp[52*17+1] = "ROW DIMENSION OF A                       \
           ROW DIMENSION OF QUAD                               NUMBER OF CON\
STRAINTS                               NUMBER OF VARIABLES                  \
               0 IMPLIES MINIMIZE, 1 IMPLIES MAXIMIZE              SIZE OF W\
ORKSPACE ARRAY                             ITERATION LIMIT                  \
                   0 IMPLIES DO NOT COMPUTE OBJECTIVE VALUE            1 IMP\
LIES OUTPUT PROBLEM PARAMETERS                 1 IMPLIES OUTPUT PROB DATA-A \
AND QUAD IN DENSE FORM 1 IMPLIES OUTPUT PIVOT INFORMATION                  1\
 IMPLIES OUTPUT SOLUTION REPORT WITHOUT RC,DUAL,SLK1 IMPLIES ALL OUTPUT BUT \
PROB PAR AND SOL REP FILED MAXIMUM OUTPUT COLUMN WIDTH                      \
   2 IMPLIES OUTPUT PROB DATA-A AND QUAD IN SPARSE FORM2 IMPLIES OUTPUT SOLU\
TION REPORT WITH RC,DUAL,SLK   2 IMPLIES ALL OUTPUT FILED                   \
       ";
    static char txttol[28*2+1] = "ROUND-OFF OR ZERO TOLERANCE PIVOT TOLERANC\
E             ";
    static char iv[24+1] = "     INDEX     VALUE    ";
    static char rcv[24+1] = "  ROW  COL     VALUE    ";
    static char status[24*6+1] = "SOLUTION IS OPTIMAL     NO SOLUTION       \
      PIVOT LIMIT REACHED     INVALID QUAD MATRIX     PROBLEM DATA ERROR    \
  ALGORITHM ERROR         ";
    static char contyp[1*3+1] = "GEL";
    static char opttyp[3*2+1] = "MINMAX";

    /* Format strings */
    static char fmt_101[] = "(/\0021\002//a,a64,\002CALL\002,i4/a,\002TITLE \
- \002,a64)";
    static char fmt_102[] = "(//a,16x,a40/)";
    static char fmt_105[] = "(/a,\002INPUT VALUES\002)";
    static char fmt_111[] = "(a,\002INPUT(\002,i2,\002) =\002,i6,3x,a52)";
    static char fmt_112[] = "(a,20x,a52)";
    static char fmt_140[] = "(/a,\002TOL VALUES\002)";
    static char fmt_151[] = "(a,\002TOL(\002,i1,\002) =\002,1pe14.5,3x,a28)";
    static char fmt_155[] = "(/)";
    static char fmt_202[] = "(/1x,a3,\002IMIZATION\002)";
    static char fmt_203[] = "(//1x,\002VARIABLES\002)";
    static char fmt_208[] = "(1x,9(i10,4x))";
    static char fmt_216[] = "(/1x,\002COST\002)";
    static char fmt_220[] = "(1x,1p9e14.5e2)";
    static char fmt_230[] = "(//1x,\002QUAD-MATRIX\002)";
    static char fmt_232[] = "(/1x,\002COLUMNS\002)";
    static char fmt_235[] = "(1x,\002ROW\002,i4)";
    static char fmt_241[] = "(/1x,\002NON-ZERO ELEMENTS\002/)";
    static char fmt_242[] = "(1x,5a24)";
    static char fmt_245[] = "(1x,5(0p2i5,1pe14.5e2))";
    static char fmt_252[] = "(//1x,\002CONSTRAINTS\002)";
    static char fmt_258[] = "(1x,9(i6,\002  ( \002,a1,\002 ) \002))";
    static char fmt_260[] = "(/1x,\002RHS\002)";
    static char fmt_270[] = "(//1x,\002A-MATRIX\002)";
    static char fmt_410[] = "(/6x,\002PIVOT\002,5x,\002INDEX\002,5x,\002PIVO\
T ROW\002,3x,\002PIVOT COLUMN\002,7x,\002PIVOT VALUE\002)";
    static char fmt_440[] = "(1x,2i10,3x,i10,5x,i10,1pe19.5e2)";
    static char fmt_510[] = "(1x,2i10,5x,\002PRINCIPAL PIVOT COMPLETED\002)";
    static char fmt_605[] = "(/a,18x,a24)";
    static char fmt_610[] = "(a,18x,a3,\002IMUM OBJECTIVE VALUE\002,2x,1pe15\
.6e2)";
    static char fmt_615[] = "(a,18x,\002NUMBER OF PIVOTS\002,5x,i8)";
    static char fmt_620[] = "(///a,28x,\002SOLUTION VECTOR\002)";
    static char fmt_622[] = "(/a,5a24)";
    static char fmt_628[] = "(a,5(2x,0pi8,1pe14.5e2))";
    static char fmt_630[] = "(//a,25x,\002REDUCED COSTS VECTOR\002)";
    static char fmt_640[] = "(//a,25x,\002DUAL SOLUTION VECTOR\002)";
    static char fmt_650[] = "(//a,24x,\002SLACK VARIABLES VECTOR\002)";
    static char fmt_695[] = "(//)";

    /* System generated locals */
    integer a_dim1, a_offset, quad_dim1, quad_offset, i__1, i__2, i__3, i__4, 
	    i__5;
    real r__1;
    static integer equiv_19[16];
    static real equiv_21[8];

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
#define kobj ((integer *)&qprcbi_1 + 7)
    static integer jmax;
#define ierr ((integer *)&qprcbi_1 + 14)
#define jsol ((integer *)&qprcbi_1 + 11)
#define jout ((integer *)&qprcbi_1 + 12)
#define tpiv ((real *)&qprcbr_1 + 1)
    static integer i__, j;
#define jdata ((integer *)&qprcbi_1 + 9)
#define ibuff (equiv_19)
#define rbuff (equiv_21)
#define itcnt ((integer *)&qprcbi_1 + 15)
#define maxit ((integer *)&qprcbi_1 + 6)
    static integer items;
#define lenws ((integer *)&qprcbi_1 + 5)
    static integer i1, l1;
#define tzero ((real *)&qprcbr_1)
#define ibuff1 (equiv_19)
#define ibuff2 (equiv_19 + 8)
#define rbuff1 (equiv_21)
#define rbuff2 (equiv_21 + 4)
    static integer items2, it2prt;
#define ml ((integer *)&qprcbi_1)
#define nl ((integer *)&qprcbi_1 + 1)
#define mo ((integer *)&qprcbi_1 + 2)
#define no ((integer *)&qprcbi_1 + 3)
    static integer margin, indent;
#define minmax ((integer *)&qprcbi_1 + 4)
#define jwidth ((integer *)&qprcbi_1 + 13)
#define jpivot ((integer *)&qprcbi_1 + 10)
#define jit ((integer *)&qprcbi_1 + 8)

    /* Fortran I/O blocks */
    static cilist io___36 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___37 = { 0, 0, 0, fmt_102, 0 };
    static cilist io___38 = { 0, 0, 0, fmt_105, 0 };
    static cilist io___40 = { 0, 0, 0, fmt_111, 0 };
    static cilist io___41 = { 0, 0, 0, fmt_112, 0 };
    static cilist io___42 = { 0, 0, 0, fmt_111, 0 };
    static cilist io___43 = { 0, 0, 0, fmt_112, 0 };
    static cilist io___44 = { 0, 0, 0, fmt_111, 0 };
    static cilist io___45 = { 0, 0, 0, fmt_112, 0 };
    static cilist io___46 = { 0, 0, 0, fmt_111, 0 };
    static cilist io___47 = { 0, 0, 0, fmt_140, 0 };
    static cilist io___48 = { 0, 0, 0, fmt_151, 0 };
    static cilist io___49 = { 0, 0, 0, fmt_155, 0 };
    static cilist io___50 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___51 = { 0, 0, 0, fmt_102, 0 };
    static cilist io___52 = { 0, 0, 0, fmt_202, 0 };
    static cilist io___53 = { 0, 0, 0, fmt_203, 0 };
    static cilist io___57 = { 0, 0, 0, fmt_208, 0 };
    static cilist io___58 = { 0, 0, 0, fmt_216, 0 };
    static cilist io___59 = { 0, 0, 0, fmt_220, 0 };
    static cilist io___60 = { 0, 0, 0, fmt_230, 0 };
    static cilist io___61 = { 0, 0, 0, fmt_232, 0 };
    static cilist io___62 = { 0, 0, 0, fmt_208, 0 };
    static cilist io___64 = { 0, 0, 0, fmt_235, 0 };
    static cilist io___65 = { 0, 0, 0, fmt_220, 0 };
    static cilist io___66 = { 0, 0, 0, fmt_241, 0 };
    static cilist io___68 = { 0, 0, 0, fmt_242, 0 };
    static cilist io___70 = { 0, 0, 0, fmt_245, 0 };
    static cilist io___71 = { 0, 0, 0, fmt_245, 0 };
    static cilist io___72 = { 0, 0, 0, fmt_252, 0 };
    static cilist io___73 = { 0, 0, 0, fmt_258, 0 };
    static cilist io___74 = { 0, 0, 0, fmt_260, 0 };
    static cilist io___75 = { 0, 0, 0, fmt_220, 0 };
    static cilist io___76 = { 0, 0, 0, fmt_270, 0 };
    static cilist io___77 = { 0, 0, 0, fmt_232, 0 };
    static cilist io___78 = { 0, 0, 0, fmt_208, 0 };
    static cilist io___79 = { 0, 0, 0, fmt_235, 0 };
    static cilist io___80 = { 0, 0, 0, fmt_220, 0 };
    static cilist io___81 = { 0, 0, 0, fmt_241, 0 };
    static cilist io___82 = { 0, 0, 0, fmt_242, 0 };
    static cilist io___83 = { 0, 0, 0, fmt_245, 0 };
    static cilist io___84 = { 0, 0, 0, fmt_245, 0 };
    static cilist io___85 = { 0, 0, 0, fmt_155, 0 };
    static cilist io___86 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___87 = { 0, 0, 0, fmt_102, 0 };
    static cilist io___89 = { 0, 0, 0, fmt_410, 0 };
    static cilist io___90 = { 0, 0, 0, fmt_440, 0 };
    static cilist io___91 = { 0, 0, 0, fmt_510, 0 };
    static cilist io___92 = { 0, 0, 0, fmt_101, 0 };
    static cilist io___93 = { 0, 0, 0, fmt_102, 0 };
    static cilist io___94 = { 0, 0, 0, fmt_605, 0 };
    static cilist io___95 = { 0, 0, 0, fmt_610, 0 };
    static cilist io___96 = { 0, 0, 0, fmt_615, 0 };
    static cilist io___97 = { 0, 0, 0, fmt_620, 0 };
    static cilist io___99 = { 0, 0, 0, fmt_622, 0 };
    static cilist io___100 = { 0, 0, 0, fmt_628, 0 };
    static cilist io___101 = { 0, 0, 0, fmt_630, 0 };
    static cilist io___102 = { 0, 0, 0, fmt_622, 0 };
    static cilist io___103 = { 0, 0, 0, fmt_628, 0 };
    static cilist io___104 = { 0, 0, 0, fmt_640, 0 };
    static cilist io___105 = { 0, 0, 0, fmt_622, 0 };
    static cilist io___106 = { 0, 0, 0, fmt_628, 0 };
    static cilist io___107 = { 0, 0, 0, fmt_650, 0 };
    static cilist io___108 = { 0, 0, 0, fmt_622, 0 };
    static cilist io___109 = { 0, 0, 0, fmt_628, 0 };
    static cilist io___110 = { 0, 0, 0, fmt_695, 0 };



/*     OUTPUT REPORT ROUTINE FOR QUADPR */

/*     QRPRT1 OUTPUTS PROBLEM PARAMETERS */
/*     QRPRT2 OUTPUTS PROBLEM DATA */
/*     QRPRT3 OUTPUTS HEADER FOR INTERMEDIATE (PIVOT) OUTPUT */
/*     QRPRT4 OUTPUTS INFORMATION AFTER A PIVOT */
/*     QRPRT5 OUTPUTS INFORMATION AFTER A COMPLETE PRINICPAL PIVOT */
/*     QRPRT6 OUTPUTS SOLUTION REPORT */

/*     QRPRT0 IS A DUMMY ENTRY TO DEFINE ALL POSSIBLE PARAMETERS */







    /* Parameter adjustments */
    if (a) {
	a_dim1 = *lda;
	a_offset = a_dim1 + 1;
	a -= a_offset;
	}
    if (kt) {
	--kt;
	}
    if (rhs) {
	--rhs;
	}
    if (cost) {
	--cost;
	}
    if (quad) {
	quad_dim1 = *ldq;
	quad_offset = quad_dim1 + 1;
	quad -= quad_offset;
	}
    if (x) {
	--x;
	}
    if (rc) {
	--rc;
	}
    if (dual) {
	--dual;
	}
    if (slk) {
	--slk;
	}

    /* Function Body */
    switch(n__) {
	case 1: goto L_qrprt1;
	case 2: goto L_qrprt2;
	case 3: goto L_qrprt3;
	case 4: goto L_qrprt4;
	case 5: goto L_qrprt5;
	case 6: goto L_qrprt6;
	}



/* .......................................................................
 */


L_qrprt1:

/*     OUTPUT PROBLEM PARAMETERS */

/*     OUTPUT HEADING */
/* L100: */
    margin = *jwidth / 2 - 35;
    io___36.ciunit = qprcbi_1.io2;
    s_wsfe(&io___36);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, iam, 64L);
    do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, qprcbc_1.titles, 64L);
    e_wsfe();
    io___37.ciunit = qprcbi_1.io2;
    s_wsfe(&io___37);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, heding, 40L);
    e_wsfe();

/*     OUTPUT INPUT ARRAY VALUES */
    io___38.ciunit = qprcbi_1.io2;
    s_wsfe(&io___38);
    do_fio(&c__1, blank, margin);
    e_wsfe();
    for (i__ = 1; i__ <= 10; ++i__) {
	io___40.ciunit = qprcbi_1.io2;
	s_wsfe(&io___40);
	do_fio(&c__1, blank, margin);
	do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&qprcbi_1.inputs[i__ - 1], (ftnlen)sizeof(
		integer));
	do_fio(&c__1, txtinp + (i__ - 1) * 52, 52L);
	e_wsfe();
/* L110: */
    }
    io___41.ciunit = qprcbi_1.io2;
    s_wsfe(&io___41);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, txtinp + 728, 52L);
    e_wsfe();
    for (i__ = 11; i__ <= 12; ++i__) {
	io___42.ciunit = qprcbi_1.io2;
	s_wsfe(&io___42);
	do_fio(&c__1, blank, margin);
	do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&qprcbi_1.inputs[i__ - 1], (ftnlen)sizeof(
		integer));
	do_fio(&c__1, txtinp + (i__ - 1) * 52, 52L);
	e_wsfe();
/* L120: */
    }
    io___43.ciunit = qprcbi_1.io2;
    s_wsfe(&io___43);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, txtinp + 780, 52L);
    e_wsfe();
    i__ = 13;
    io___44.ciunit = qprcbi_1.io2;
    s_wsfe(&io___44);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&qprcbi_1.inputs[i__ - 1], (ftnlen)sizeof(integer));
    do_fio(&c__1, txtinp + (i__ - 1) * 52, 52L);
    e_wsfe();
    io___45.ciunit = qprcbi_1.io2;
    s_wsfe(&io___45);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, txtinp + 832, 52L);
    e_wsfe();
    i__ = 14;
    io___46.ciunit = qprcbi_1.io2;
    s_wsfe(&io___46);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&qprcbi_1.inputs[i__ - 1], (ftnlen)sizeof(integer));
    do_fio(&c__1, txtinp + (i__ - 1) * 52, 52L);
    e_wsfe();
    io___47.ciunit = qprcbi_1.io2;
    s_wsfe(&io___47);
    do_fio(&c__1, blank, margin);
    e_wsfe();
    for (i__ = 1; i__ <= 2; ++i__) {
	io___48.ciunit = qprcbi_1.io2;
	s_wsfe(&io___48);
	do_fio(&c__1, blank, margin);
	do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	do_fio(&c__1, (char *)&qprcbr_1.tols[i__ - 1], (ftnlen)sizeof(real));
	do_fio(&c__1, txttol + (i__ - 1) * 28, 28L);
	e_wsfe();
/* L150: */
    }
    io___49.ciunit = qprcbi_1.io2;
    s_wsfe(&io___49);
    e_wsfe();

    return 0;

/* .......................................................................
 */


L_qrprt2:

/*     OUTPUT PROBLEM DATA */

/*     OUTPUT HEADING */
/* L200: */
    margin = *jwidth / 2 - 35;
    io___50.ciunit = qprcbi_1.io3;
    s_wsfe(&io___50);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, iam, 64L);
    do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, qprcbc_1.titles, 64L);
    e_wsfe();
    io___51.ciunit = qprcbi_1.io3;
    s_wsfe(&io___51);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, heding + 40, 40L);
    e_wsfe();

/*     OUTPUT OPTIMIZATION */
    io___52.ciunit = qprcbi_1.io3;
    s_wsfe(&io___52);
    do_fio(&c__1, opttyp + *minmax * 3, 3L);
    e_wsfe();

/*     OUTPUT VARIABLES HEADER */
    io___53.ciunit = qprcbi_1.io3;
    s_wsfe(&io___53);
    e_wsfe();
    items = *jwidth / 14;
    i__1 = *no;
    i__2 = items;
    for (j = 1; i__2 < 0 ? j >= i__1 : j <= i__1; j += i__2) {
/* Computing MIN */
	i__3 = *no, i__4 = j + items - 1;
	jmax = min(i__3,i__4);
	io___57.ciunit = qprcbi_1.io3;
	s_wsfe(&io___57);
	i__3 = jmax;
	for (i__ = j; i__ <= i__3; ++i__) {
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	}
	e_wsfe();
/* L207: */
    }

/*     OUTPUT LINEAR COSTS */
/* L215: */
    io___58.ciunit = qprcbi_1.io3;
    s_wsfe(&io___58);
    e_wsfe();
    i__2 = *no;
    i__1 = items;
    for (j = 1; i__1 < 0 ? j >= i__2 : j <= i__2; j += i__1) {
/* Computing MIN */
	i__3 = *no, i__4 = j + items - 1;
	jmax = min(i__3,i__4);
	io___59.ciunit = qprcbi_1.io3;
	s_wsfe(&io___59);
	i__3 = jmax;
	for (i__ = j; i__ <= i__3; ++i__) {
	    do_fio(&c__1, (char *)&cost[i__], (ftnlen)sizeof(real));
	}
	e_wsfe();
/* L217: */
    }

/*     OUTPUT QUAD-MATRIX */
    io___60.ciunit = qprcbi_1.io3;
    s_wsfe(&io___60);
    e_wsfe();
    if (*jdata == 2) {
	goto L240;
    }
/*     DENSE FORM */
    io___61.ciunit = qprcbi_1.io3;
    s_wsfe(&io___61);
    e_wsfe();
    i__1 = *no;
    i__2 = items;
    for (j = 1; i__2 < 0 ? j >= i__1 : j <= i__1; j += i__2) {
/* Computing MIN */
	i__3 = *no, i__4 = j + items - 1;
	jmax = min(i__3,i__4);
	io___62.ciunit = qprcbi_1.io3;
	s_wsfe(&io___62);
	i__3 = jmax;
	for (i__ = j; i__ <= i__3; ++i__) {
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	}
	e_wsfe();
/* L234: */
    }
/* L235: */
    i__2 = *no;
    for (l1 = 1; l1 <= i__2; ++l1) {
	io___64.ciunit = qprcbi_1.io3;
	s_wsfe(&io___64);
	do_fio(&c__1, (char *)&l1, (ftnlen)sizeof(integer));
	e_wsfe();
	i__1 = *no;
	i__3 = items;
	for (j = 1; i__3 < 0 ? j >= i__1 : j <= i__1; j += i__3) {
/* Computing MIN */
	    i__4 = *no, i__5 = j + items - 1;
	    jmax = min(i__4,i__5);
	    io___65.ciunit = qprcbi_1.io3;
	    s_wsfe(&io___65);
	    i__4 = jmax;
	    for (i__ = j; i__ <= i__4; ++i__) {
		do_fio(&c__1, (char *)&quad[l1 + i__ * quad_dim1], (ftnlen)
			sizeof(real));
	    }
	    e_wsfe();
/* L237: */
	}
/* L238: */
    }
    goto L250;
/*     SPARSE FORM */
L240:
    io___66.ciunit = qprcbi_1.io3;
    s_wsfe(&io___66);
    e_wsfe();
    items2 = *jwidth / 24;
    io___68.ciunit = qprcbi_1.io3;
    s_wsfe(&io___68);
    i__2 = items2;
    for (i__ = 1; i__ <= i__2; ++i__) {
	do_fio(&c__1, rcv, 24L);
    }
    e_wsfe();
    i1 = 0;
    i__2 = *no;
    for (l1 = 1; l1 <= i__2; ++l1) {
	i__3 = *no;
	for (j = 1; j <= i__3; ++j) {
	    if ((r__1 = quad[l1 + j * quad_dim1], dabs(r__1)) < *tzero) {
		goto L247;
	    }
	    ++i1;
	    ibuff1[i1 - 1] = l1;
	    ibuff2[i1 - 1] = j;
	    rbuff[i1 - 1] = quad[l1 + j * quad_dim1];
	    if (i1 < items2) {
		goto L247;
	    }
	    io___70.ciunit = qprcbi_1.io3;
	    s_wsfe(&io___70);
	    i__1 = items2;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		do_fio(&c__1, (char *)&ibuff1[i__ - 1], (ftnlen)sizeof(
			integer));
		do_fio(&c__1, (char *)&ibuff2[i__ - 1], (ftnlen)sizeof(
			integer));
		do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	    }
	    e_wsfe();
	    i1 = 0;
L247:
	    ;
	}
/* L248: */
    }
    if (i1 > 0) {
	io___71.ciunit = qprcbi_1.io3;
	s_wsfe(&io___71);
	i__2 = i1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_fio(&c__1, (char *)&ibuff1[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&ibuff2[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
    }

L250:
    if (*mo == 0) {
	goto L290;
    }
/*     OUTPUT CONSTRAINT TYPES */
    io___72.ciunit = qprcbi_1.io3;
    s_wsfe(&io___72);
    e_wsfe();
    i__2 = *mo;
    i__3 = items;
    for (j = 1; i__3 < 0 ? j >= i__2 : j <= i__2; j += i__3) {
/* Computing MIN */
	i__1 = *mo, i__4 = j + items - 1;
	jmax = min(i__1,i__4);
	io___73.ciunit = qprcbi_1.io3;
	s_wsfe(&io___73);
	i__1 = jmax;
	for (i__ = j; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	    do_fio(&c__1, contyp + (kt[i__] + 1), 1L);
	}
	e_wsfe();
/* L257: */
    }

/*     OUTPUT RIGHT-HAND-SIDE */
    io___74.ciunit = qprcbi_1.io3;
    s_wsfe(&io___74);
    e_wsfe();
    i__3 = *mo;
    i__2 = items;
    for (j = 1; i__2 < 0 ? j >= i__3 : j <= i__3; j += i__2) {
/* Computing MIN */
	i__1 = *mo, i__4 = j + items - 1;
	jmax = min(i__1,i__4);
	io___75.ciunit = qprcbi_1.io3;
	s_wsfe(&io___75);
	i__1 = jmax;
	for (i__ = j; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&rhs[i__], (ftnlen)sizeof(real));
	}
	e_wsfe();
/* L261: */
    }

/*     OUTPUT A-MATRIX */
    io___76.ciunit = qprcbi_1.io3;
    s_wsfe(&io___76);
    e_wsfe();
    if (*jdata == 2) {
	goto L280;
    }
/*     DENSE FORM */
    io___77.ciunit = qprcbi_1.io3;
    s_wsfe(&io___77);
    e_wsfe();
    i__2 = *no;
    i__3 = items;
    for (j = 1; i__3 < 0 ? j >= i__2 : j <= i__2; j += i__3) {
/* Computing MIN */
	i__1 = *no, i__4 = j + items - 1;
	jmax = min(i__1,i__4);
	io___78.ciunit = qprcbi_1.io3;
	s_wsfe(&io___78);
	i__1 = jmax;
	for (i__ = j; i__ <= i__1; ++i__) {
	    do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
	}
	e_wsfe();
/* L274: */
    }
    i__3 = *mo;
    for (l1 = 1; l1 <= i__3; ++l1) {
	io___79.ciunit = qprcbi_1.io3;
	s_wsfe(&io___79);
	do_fio(&c__1, (char *)&l1, (ftnlen)sizeof(integer));
	e_wsfe();
	i__2 = *no;
	i__1 = items;
	for (j = 1; i__1 < 0 ? j >= i__2 : j <= i__2; j += i__1) {
/* Computing MIN */
	    i__4 = *no, i__5 = j + items - 1;
	    jmax = min(i__4,i__5);
	    io___80.ciunit = qprcbi_1.io3;
	    s_wsfe(&io___80);
	    i__4 = jmax;
	    for (i__ = j; i__ <= i__4; ++i__) {
		do_fio(&c__1, (char *)&a[l1 + i__ * a_dim1], (ftnlen)sizeof(
			real));
	    }
	    e_wsfe();
/* L277: */
	}
/* L278: */
    }
    goto L290;
/*     SPARSE FORM */
L280:
    io___81.ciunit = qprcbi_1.io3;
    s_wsfe(&io___81);
    e_wsfe();
    io___82.ciunit = qprcbi_1.io3;
    s_wsfe(&io___82);
    i__3 = items2;
    for (i__ = 1; i__ <= i__3; ++i__) {
	do_fio(&c__1, rcv, 24L);
    }
    e_wsfe();
    i1 = 0;
    i__3 = *mo;
    for (l1 = 1; l1 <= i__3; ++l1) {
	i__1 = *no;
	for (j = 1; j <= i__1; ++j) {
	    if ((r__1 = a[l1 + j * a_dim1], dabs(r__1)) < *tzero) {
		goto L287;
	    }
	    ++i1;
	    ibuff1[i1 - 1] = l1;
	    ibuff2[i1 - 1] = j;
	    rbuff[i1 - 1] = a[l1 + j * a_dim1];
	    if (i1 < items2) {
		goto L287;
	    }
	    io___83.ciunit = qprcbi_1.io3;
	    s_wsfe(&io___83);
	    i__2 = items2;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		do_fio(&c__1, (char *)&ibuff1[i__ - 1], (ftnlen)sizeof(
			integer));
		do_fio(&c__1, (char *)&ibuff2[i__ - 1], (ftnlen)sizeof(
			integer));
		do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	    }
	    e_wsfe();
	    i1 = 0;
L287:
	    ;
	}
/* L288: */
    }
    if (i1 > 0) {
	io___84.ciunit = qprcbi_1.io3;
	s_wsfe(&io___84);
	i__3 = i1;
	for (i__ = 1; i__ <= i__3; ++i__) {
	    do_fio(&c__1, (char *)&ibuff1[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&ibuff2[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
    }

L290:
    io___85.ciunit = qprcbi_1.io3;
    s_wsfe(&io___85);
    e_wsfe();

    return 0;

/* .......................................................................
 */


L_qrprt3:

/*     OUTPUT HEADER FOR INTERMEDIATE OUTPUT */

/*     OUTPUT HEADING */
/* L300: */
    margin = *jwidth / 2 - 35;
    io___86.ciunit = qprcbi_1.io3;
    s_wsfe(&io___86);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, iam, 64L);
    do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, qprcbc_1.titles, 64L);
    e_wsfe();
    io___87.ciunit = qprcbi_1.io3;
    s_wsfe(&io___87);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, heding + 80, 40L);
    e_wsfe();
    it2prt = 0;

    return 0;

/* .......................................................................
 */


L_qrprt4:

/*     OUTPUT INFORMATION AFTER A PIVOT */

/* L400: */
    if (it2prt > 0) {
	goto L420;
    }
    io___89.ciunit = qprcbi_1.io3;
    s_wsfe(&io___89);
    e_wsfe();
    it2prt = 19;
    goto L430;
L420:
    --it2prt;
L430:
    io___90.ciunit = qprcbi_1.io3;
    s_wsfe(&io___90);
    do_fio(&c__1, (char *)&(*itcnt), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&qprcbi_1.indx, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&qprcbi_1.row, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&qprcbi_1.col, (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&qprcbd_1.pivot, (ftnlen)sizeof(doublereal));
    e_wsfe();

    return 0;

/* .......................................................................
 */


L_qrprt5:

/*     OUTPUT INFORMATION AFTER A PRINCIPAL PIVOT */

/* L500: */
    --it2prt;
    io___91.ciunit = qprcbi_1.io3;
    s_wsfe(&io___91);
    do_fio(&c__1, (char *)&(*itcnt), (ftnlen)sizeof(integer));
    do_fio(&c__1, (char *)&qprcbi_1.indx, (ftnlen)sizeof(integer));
    e_wsfe();

    return 0;

/* .......................................................................
 */


L_qrprt6:

/*     OUTPUT SOLUTION REPORT */

/*     OUTPUT HEADING */
/* L600: */
    margin = *jwidth / 2 - 35;
    io___92.ciunit = qprcbi_1.io2;
    s_wsfe(&io___92);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, iam, 64L);
    do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, qprcbc_1.titles, 64L);
    e_wsfe();
    io___93.ciunit = qprcbi_1.io2;
    s_wsfe(&io___93);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, heding + 120, 40L);
    e_wsfe();

/*     OUTPUT STATUS */
    io___94.ciunit = qprcbi_1.io2;
    s_wsfe(&io___94);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, status + (*ierr - 1) * 24, 24L);
    e_wsfe();

/*     OUTPUT OBJECTIVE VALUE ONLY IF APPROPRIATE */
    if (*ierr == 1 && *kobj == 1) {
	io___95.ciunit = qprcbi_1.io2;
	s_wsfe(&io___95);
	do_fio(&c__1, blank, margin);
	do_fio(&c__1, opttyp + *minmax * 3, 3L);
	do_fio(&c__1, (char *)&(*obj), (ftnlen)sizeof(real));
	e_wsfe();
    }

/*     OUTPUT NO. PIVOTS */
    io___96.ciunit = qprcbi_1.io2;
    s_wsfe(&io___96);
    do_fio(&c__1, blank, margin);
    do_fio(&c__1, (char *)&(*itcnt), (ftnlen)sizeof(integer));
    e_wsfe();

/*     OUTPUT SOLUTION VECTOR */
    io___97.ciunit = qprcbi_1.io2;
    s_wsfe(&io___97);
    do_fio(&c__1, blank, margin);
    e_wsfe();
/* Computing MIN */
    i__3 = *no, i__1 = *jwidth / 24;
    items = min(i__3,i__1);
    indent = (*jwidth - items * 24) / 2 + 1;
    io___99.ciunit = qprcbi_1.io2;
    s_wsfe(&io___99);
    do_fio(&c__1, blank, indent);
    i__3 = items;
    for (i__ = 1; i__ <= i__3; ++i__) {
	do_fio(&c__1, iv, 24L);
    }
    e_wsfe();
    i__3 = *no;
    i__1 = items;
    for (j = 1; i__1 < 0 ? j >= i__3 : j <= i__3; j += i__1) {
/* Computing MIN */
	i__2 = *no, i__4 = j + items - 1;
	jmax = min(i__2,i__4);
	i1 = 0;
	i__2 = jmax;
	for (l1 = j; l1 <= i__2; ++l1) {
	    ++i1;
	    ibuff[i1 - 1] = l1;
	    rbuff[i1 - 1] = x[l1];
/* L624: */
	}
	io___100.ciunit = qprcbi_1.io2;
	s_wsfe(&io___100);
	do_fio(&c__1, blank, indent);
	i__2 = i1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_fio(&c__1, (char *)&ibuff[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
/* L626: */
    }
/*     OUTPUT REMAINDER ONLY IF REQUESTED */
    if (*jsol == 1) {
	goto L690;
    }

/*     OUTPUT REDUCED COSTS VECTOR */
    io___101.ciunit = qprcbi_1.io2;
    s_wsfe(&io___101);
    do_fio(&c__1, blank, margin);
    e_wsfe();
    io___102.ciunit = qprcbi_1.io2;
    s_wsfe(&io___102);
    do_fio(&c__1, blank, indent);
    i__1 = items;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_fio(&c__1, iv, 24L);
    }
    e_wsfe();
    i__1 = *no;
    i__3 = items;
    for (j = 1; i__3 < 0 ? j >= i__1 : j <= i__1; j += i__3) {
/* Computing MIN */
	i__2 = *no, i__4 = j + items - 1;
	jmax = min(i__2,i__4);
	i1 = 0;
	i__2 = jmax;
	for (l1 = j; l1 <= i__2; ++l1) {
	    ++i1;
	    ibuff[i1 - 1] = l1;
	    rbuff[i1 - 1] = rc[l1];
/* L634: */
	}
	io___103.ciunit = qprcbi_1.io2;
	s_wsfe(&io___103);
	do_fio(&c__1, blank, indent);
	i__2 = i1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_fio(&c__1, (char *)&ibuff[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
/* L636: */
    }
    if (*mo == 0) {
	goto L690;
    }

/*     OUTPUT DUAL SOLUTION VECTOR */
    io___104.ciunit = qprcbi_1.io2;
    s_wsfe(&io___104);
    do_fio(&c__1, blank, margin);
    e_wsfe();
/* Computing MIN */
    i__3 = *mo, i__1 = *jwidth / 24;
    items = min(i__3,i__1);
    indent = (*jwidth - items * 24) / 2 + 1;
    io___105.ciunit = qprcbi_1.io2;
    s_wsfe(&io___105);
    do_fio(&c__1, blank, indent);
    i__3 = items;
    for (i__ = 1; i__ <= i__3; ++i__) {
	do_fio(&c__1, iv, 24L);
    }
    e_wsfe();
    i__3 = *mo;
    i__1 = items;
    for (j = 1; i__1 < 0 ? j >= i__3 : j <= i__3; j += i__1) {
/* Computing MIN */
	i__2 = *mo, i__4 = j + items - 1;
	jmax = min(i__2,i__4);
	i1 = 0;
	i__2 = jmax;
	for (l1 = j; l1 <= i__2; ++l1) {
	    ++i1;
	    ibuff[i1 - 1] = l1;
	    rbuff[i1 - 1] = dual[l1];
/* L644: */
	}
	io___106.ciunit = qprcbi_1.io2;
	s_wsfe(&io___106);
	do_fio(&c__1, blank, indent);
	i__2 = i1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_fio(&c__1, (char *)&ibuff[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
/* L646: */
    }

/*     OUTPUT SLACK VARIABLES VECTOR */
    io___107.ciunit = qprcbi_1.io2;
    s_wsfe(&io___107);
    do_fio(&c__1, blank, margin);
    e_wsfe();
    io___108.ciunit = qprcbi_1.io2;
    s_wsfe(&io___108);
    do_fio(&c__1, blank, indent);
    i__1 = items;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_fio(&c__1, iv, 24L);
    }
    e_wsfe();
    i__1 = *mo;
    i__3 = items;
    for (j = 1; i__3 < 0 ? j >= i__1 : j <= i__1; j += i__3) {
/* Computing MIN */
	i__2 = *mo, i__4 = j + items - 1;
	jmax = min(i__2,i__4);
	i1 = 0;
	i__2 = jmax;
	for (l1 = j; l1 <= i__2; ++l1) {
	    ++i1;
	    ibuff[i1 - 1] = l1;
	    rbuff[i1 - 1] = slk[l1];
/* L654: */
	}
	io___109.ciunit = qprcbi_1.io2;
	s_wsfe(&io___109);
	do_fio(&c__1, blank, indent);
	i__2 = i1;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    do_fio(&c__1, (char *)&ibuff[i__ - 1], (ftnlen)sizeof(integer));
	    do_fio(&c__1, (char *)&rbuff[i__ - 1], (ftnlen)sizeof(real));
	}
	e_wsfe();
/* L656: */
    }

L690:
    io___110.ciunit = qprcbi_1.io2;
    s_wsfe(&io___110);
    e_wsfe();

    return 0;

} /* qrprt0_ */

#undef jit
#undef jpivot
#undef jwidth
#undef minmax
#undef no
#undef mo
#undef nl
#undef ml
#undef rbuff2
#undef rbuff1
#undef ibuff2
#undef ibuff1
#undef tzero
#undef lenws
#undef maxit
#undef itcnt
#undef rbuff
#undef ibuff
#undef jdata
#undef tpiv
#undef jout
#undef jsol
#undef ierr
#undef kobj


/* Subroutine */ int qrprt0_(a, lda, kt, rhs, cost, quad, ldq, obj, x, rc, 
	dual, slk)
real *a;
integer *lda, *kt;
real *rhs, *cost, *quad;
integer *ldq;
real *obj, *x, *rc, *dual, *slk;
{
    return qrprt0_0_(0, a, lda, kt, rhs, cost, quad, ldq, obj, x, rc, dual, 
	    slk);
    }

/* Subroutine */ int qrprt1_()
{
    return qrprt0_0_(1, (real *)0, (integer *)0, (integer *)0, (real *)0, (
	    real *)0, (real *)0, (integer *)0, (real *)0, (real *)0, (real *)
	    0, (real *)0, (real *)0);
    }

/* Subroutine */ int qrprt2_(a, lda, kt, rhs, cost, quad, ldq)
real *a;
integer *lda, *kt;
real *rhs, *cost, *quad;
integer *ldq;
{
    return qrprt0_0_(2, a, lda, kt, rhs, cost, quad, ldq, (real *)0, (real *)
	    0, (real *)0, (real *)0, (real *)0);
    }

/* Subroutine */ int qrprt3_()
{
    return qrprt0_0_(3, (real *)0, (integer *)0, (integer *)0, (real *)0, (
	    real *)0, (real *)0, (integer *)0, (real *)0, (real *)0, (real *)
	    0, (real *)0, (real *)0);
    }

/* Subroutine */ int qrprt4_()
{
    return qrprt0_0_(4, (real *)0, (integer *)0, (integer *)0, (real *)0, (
	    real *)0, (real *)0, (integer *)0, (real *)0, (real *)0, (real *)
	    0, (real *)0, (real *)0);
    }

/* Subroutine */ int qrprt5_()
{
    return qrprt0_0_(5, (real *)0, (integer *)0, (integer *)0, (real *)0, (
	    real *)0, (real *)0, (integer *)0, (real *)0, (real *)0, (real *)
	    0, (real *)0, (real *)0);
    }

/* Subroutine */ int qrprt6_(x, rc, dual, slk, obj)
real *x, *rc, *dual, *slk, *obj;
{
    return qrprt0_0_(6, (real *)0, (integer *)0, (integer *)0, (real *)0, (
	    real *)0, (real *)0, (integer *)0, obj, x, rc, dual, slk);
    }

