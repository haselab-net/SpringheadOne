/* QDCOMP.F -- translated by f2c (version 19960711).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "headrs.h"
/* Common Block Declarations */

#define qprcbi_1 qprcbi_

#define qprcbr_1 qprcbr_

#define qprcbd_1 qprcbd_

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int qdcomp_(b, temp, u, jz, jw)
doublereal *b, *temp, *u;
integer *jz, *jw;
{
    /* Initialized data */

    static char msg[48*4+1] = "ALGORITHM ERROR, INDEX VALUE NON-DECREASING. \
   NO SOLUTION, INVALID QUADRATIC COSTS MATRIX.    NO SOLUTION, INFEASIBLE O\
R UNBOUNDED PROBLEM.   ERROR, ITERATION LIMIT REACHED.                 ";

    /* Format strings */
    static char fmt_470[] = "(/1x,a48,i6,\002 PIVOTS PERFORMED.\002/)";

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
#define kobj ((integer *)&qprcbi_1 + 7)
#define ierr ((integer *)&qprcbi_1 + 14)
#define jsol ((integer *)&qprcbi_1 + 11)
    static real tmmp;
#define jout ((integer *)&qprcbi_1 + 12)
#define tpiv ((real *)&qprcbr_1 + 1)
    static integer i__, j, k;
#define jdata ((integer *)&qprcbi_1 + 9)
    static integer jflag, nflag;
    static doublereal z__;
    static integer isave, itemp;
#define itcnt ((integer *)&qprcbi_1 + 15)
#define maxit ((integer *)&qprcbi_1 + 6)
#define lenws ((integer *)&qprcbi_1 + 5)
#define tzero ((real *)&qprcbr_1)
    static integer kc, ii, ij, jj;
    static doublereal bs;
    static integer kj, ll;
#define ml ((integer *)&qprcbi_1)
#define nl ((integer *)&qprcbi_1 + 1)
#define mo ((integer *)&qprcbi_1 + 2)
#define no ((integer *)&qprcbi_1 + 3)
    extern /* Subroutine */ int qrprt5_(), qrprt4_();
#define minmax ((integer *)&qprcbi_1 + 4)
#define jwidth ((integer *)&qprcbi_1 + 13)
#define jpivot ((integer *)&qprcbi_1 + 10)
    static integer iii, ict, lll;
#define jit ((integer *)&qprcbi_1 + 8)
    static doublereal bst;

    /* Fortran I/O blocks */
    static cilist io___38 = { 0, 0, 0, fmt_470, 0 };
    static cilist io___39 = { 0, 0, 0, fmt_470, 0 };



/*     COMPUTE THE FINAL TABLEAU FOR QUADRATIC PROGRAMMING GIVEN */
/*     THE INITIAL TABLEAU USING THE METHOD OF PRINCIPAL PIVOTING. */






    /* Parameter adjustments */
    --jw;
    --jz;
    --u;
    --temp;
    --b;

    /* Function Body */


/* *****INITIALIZATIONS */
    *itcnt = 0;
    jflag = 0;
    qprcbi_1.indx = qprcbi_1.p1;
/* *****BASIC VARIABLES GET NEGATIVE VALUES. */
/* *****NONBASIC VARIABLES(JZ) GET POSITIVE VALUES. */
    i__1 = qprcbi_1.p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	jw[i__] = -i__;
	jz[i__] = i__;
/* L30: */
    }
L40:
    itemp = 0;
/* *****CHECK FOR NONDECREASING INDEX (ALLOW UP TO 5 EQUAL INDEXES FOR */
/* *****TOLERANCE PURPOSES). */
    ij = qprcbi_1.psq;
    i__1 = qprcbi_1.p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++ij;
	if (b[ij] < -(*tzero)) {
	    ++itemp;
	}
/* L50: */
    }
    if (itemp != qprcbi_1.indx) {
	ict = 0;
    }
    if (itemp == qprcbi_1.indx) {
	++ict;
    }
    if (itemp > qprcbi_1.indx || ict == 5) {
	goto L400;
    }
/* *****IF THE INDEX IS ZERO YOU ARE DONE. */
    if (itemp == 0) {
	goto L300;
    }
    qprcbi_1.indx = itemp;
    ij = qprcbi_1.psq;
    i__1 = qprcbi_1.p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++ij;
	if (b[ij] < -(*tzero)) {
	    goto L70;
	}
/* L60: */
    }
L70:
    ij = qprcbi_1.p * (i__ - 1) + i__;
/* *****CHECK FOR NEGATIVE DIAGONAL ELEMENT (NOT ALLOWED IF POS. SEMIDEF.)
 */
    if (b[ij] < -(*tpiv)) {
	goto L410;
    }
    if (b[ij] > *tpiv) {
	goto L100;
    }
/* *****IF POTENTIAL PIVOT ELEMENT IS NEAR ZERO, USE CODE BELOW */
/* *****TO DETERMINE THE BLOCKING VARIABLE. */
    ii = i__;
L80:
    qprcbi_1.row = 0;
    tmmp = (float)1e6;
    ij = qprcbi_1.psq;
    kj = qprcbi_1.p * (ii - 1);
    i__1 = qprcbi_1.p;
    for (j = 1; j <= i__1; ++j) {
	++ij;
	++kj;
	if (b[ij] < -(*tzero)) {
	    goto L90;
	}
/* ********IF ALL COL. ELEMENTS ARE NONNEGATIVE, THEN THE DRIVING VARI
ABLE */
/* ********IS UNBLOCKED AND THERE IS NO SOLUTION. */
	if (b[kj] >= -(*tpiv)) {
	    goto L90;
	}
	z__ = -b[ij] / b[kj];
	if (z__ >= tmmp) {
	    goto L90;
	}
	tmmp = z__;
	qprcbi_1.row = j;
L90:
	;
    }
    if (qprcbi_1.row == 0) {
	goto L420;
    }
    qprcbi_1.col = ii;
    goto L150;
/* *****COMPUTE Z = -Q(R)/B(R,R), THE INCREASE IN THE DRIVING VARIABLE. */
L100:
    ij = qprcbi_1.psq + i__;
    kj = qprcbi_1.p * (i__ - 1) + i__;
    z__ = -b[ij] / b[kj];
    qprcbi_1.col = i__;
L110:
    bs = (float)0.;
    qprcbi_1.row = 0;
/* *****FIND THE NEW BLOCKING VARIABLE, BASED ON WHICH BASIC VARIABLE IS 
*/
/* *****MOST AFFECTED BY THE CHANGE IN THE DRIVING VARIABLE. */
    ij = (qprcbi_1.col - 1) * qprcbi_1.p;
    kj = qprcbi_1.psq;
    i__1 = qprcbi_1.p;
    for (j = 1; j <= i__1; ++j) {
	++ij;
	++kj;
	bst = b[ij] * z__ + b[kj];
	if (b[kj] < -(*tzero) || bst >= -(*tpiv)) {
	    goto L140;
	}
	if ((d__1 = b[kj], abs(d__1)) < *tzero && b[ij] < -(*tzero)) {
	    goto L120;
	}
	if (b[kj] != 0.) {
	    goto L130;
	} else {
	    goto L140;
	}
L120:
	qprcbi_1.row = j;
	goto L150;
L130:
	bst = -bst / b[kj];
	if (bst <= bs) {
	    goto L140;
	}
	bs = bst;
	qprcbi_1.row = j;
L140:
	;
    }
    if (qprcbi_1.row == 0) {
	goto L210;
    }
/* *****NONPRINCIPAL PIVOT ON B(S,R). */
/* *****AT THIS POINT THE BLOCKING VARIABLE IS B(NS,I). */
L150:
    iii = -jw[qprcbi_1.row];
    nflag = 1;
    goto L500;
L160:
    if (*itcnt >= *maxit) {
	goto L440;
    }
    ++(*itcnt);
    jflag = 0;
/* *****PUT TOLERANCE ON B(I,P1) AFTER A NONPRINCIPAL PIVOT. */
    ij = qprcbi_1.psq + i__;
    if (b[ij] < -(*tzero)) {
	goto L170;
    }
    goto L40;
/* *****THE NEW DRIVING VARIABLE IS THE COMPLEMENT OF THE OLD BLOCKING */
/* *****VARIABLE */
L170:
    i__1 = qprcbi_1.p;
    for (jj = 1; jj <= i__1; ++jj) {
	if (jz[jj] == iii) {
	    goto L190;
	}
/* L180: */
    }
/* *****NO COMPLEMENT VARIABLE (NOT ALLOWED IF POS. SEMIDEF.) */
    goto L410;
L190:
    qprcbi_1.col = jj;
/* *****THE DISTINGUISHED VARIABLE IS STILL THE I-TH ROW. */
    ij = qprcbi_1.p * (qprcbi_1.col - 1) + i__;
    if (b[ij] > *tpiv) {
	goto L200;
    }
    ii = qprcbi_1.col;
    goto L80;
L200:
    kj = qprcbi_1.psq + i__;
    z__ = -b[kj] / b[ij];
    goto L110;
/* *****PRINCIPAL PIVOT ON B(I,I). */
L210:
    ll = qprcbi_1.row;
    qprcbi_1.row = i__;
    nflag = 2;
    goto L500;
L220:
    if (jflag == 1) {
	goto L290;
    }
    jflag = 1;
/* *****MUST REARRANGE THE ROWS AND COLUMNS FOR COMPLEMENTARITY */
/* *****EXCHANGE ROWS */
L230:
    lll = 0;
    i__1 = qprcbi_1.p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k = (i__2 = jw[i__], abs(i__2));
	if (k == i__) {
	    goto L250;
	}
	ij = k;
	kj = i__;
	i__2 = qprcbi_1.p1;
	for (j = 1; j <= i__2; ++j) {
	    z__ = b[ij];
	    b[ij] = b[kj];
	    b[kj] = z__;
	    ij += qprcbi_1.p;
	    kj += qprcbi_1.p;
/* L240: */
	}
	ii = jw[i__];
	jw[i__] = jw[k];
	jw[k] = ii;
	++lll;
L250:
	;
    }
    if (lll >= 2) {
	goto L230;
    }
/* *****EXCHANGE COLUMNS */
L260:
    lll = 0;
    i__1 = qprcbi_1.p;
    for (j = 1; j <= i__1; ++j) {
	k = (i__2 = jz[j], abs(i__2));
	if (k == j) {
	    goto L280;
	}
	ij = qprcbi_1.p * (k - 1);
	kj = qprcbi_1.p * (j - 1);
	i__2 = qprcbi_1.p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ++ij;
	    ++kj;
	    z__ = b[ij];
	    b[ij] = b[kj];
	    b[kj] = z__;
/* L270: */
	}
	ii = jz[j];
	jz[j] = jz[k];
	jz[k] = ii;
	++lll;
L280:
	;
    }
    if (lll >= 2) {
	goto L260;
    }
L290:
    qprcbi_1.row = ll;
    ++(*itcnt);
    if (*jpivot == 1) {
	qrprt5_();
    }
    if (qprcbi_1.indx > 1) {
	goto L40;
    }

/* *****NORMAL TERMINATION */
L300:
    *ierr = 1;
    goto L600;

/* *****ERROR TERMINATION */
L400:
    k = 1;
    *ierr = 6;
    goto L460;
L410:
    k = 2;
    *ierr = 4;
    goto L460;
L420:
    k = 3;
    *ierr = 2;
    goto L460;
L440:
    k = 4;
    *ierr = 3;
L460:
    io___38.ciunit = qprcbi_1.io1;
    s_wsfe(&io___38);
    do_fio(&c__1, msg + (k - 1) * 48, 48L);
    do_fio(&c__1, (char *)&(*itcnt), (ftnlen)sizeof(integer));
    e_wsfe();
    if (*jout != 0) {
	io___39.ciunit = qprcbi_1.io3;
	s_wsfe(&io___39);
	do_fio(&c__1, msg + (k - 1) * 48, 48L);
	do_fio(&c__1, (char *)&(*itcnt), (ftnlen)sizeof(integer));
	e_wsfe();
    }
    goto L600;

/* *****PIVOT CODE. */
/* *****QUADPP USES A NORMAL JORDAN PIVOT. */
/* *****HOWEVER, WE CAN SAVE MEMORY BY USING THE ELEMENTARY MATRIX. */
/* *****STORE PIVOT ELEMENT */
L500:
    kc = qprcbi_1.p * (qprcbi_1.col - 1);
    kj = kc + qprcbi_1.row;
    qprcbd_1.pivot = b[kj];
    z__ = 1. / qprcbd_1.pivot;
/* *****STORE ELEMENTARY-VECTOR AND U-VECTOR. */
    ij = qprcbi_1.row;
    isave = i__;
    i__1 = qprcbi_1.p1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	u[i__] = b[ij];
	ij += qprcbi_1.p;
/* L510: */
    }
    ij = kc;
    i__1 = qprcbi_1.p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++ij;
	temp[i__] = -b[ij] * z__;
/* L520: */
    }
    temp[qprcbi_1.row] = -z__ - 1.;
/* *****COMPUTE ALL COLUMNS EXCEPT PIVOT COLUMN. */
    ij = 0;
    i__1 = qprcbi_1.p1;
    for (j = 1; j <= i__1; ++j) {
	if (j != qprcbi_1.col) {
	    goto L530;
	}
	ij += qprcbi_1.p;
	goto L550;
L530:
	i__2 = qprcbi_1.p;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ++ij;
	    b[ij] += u[j] * temp[i__];
/* L540: */
	}
L550:
	;
    }
/* *****COMPUTE PIVOT COLUMN. */
    ij = kc;
    i__1 = qprcbi_1.p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	++ij;
	b[ij] = -temp[i__];
/* L560: */
    }
/* *****COMPUTE NEW PIVOT ELEMENT. */
    b[kj] = z__;
/* *****KEEP TRACK OF BASIC AND NONBASIC VARIABLES */
    k = jw[qprcbi_1.row];
    jw[qprcbi_1.row] = jz[qprcbi_1.col];
    jz[qprcbi_1.col] = k;
    if (*jpivot == 1) {
	qrprt4_();
    }
    i__ = isave;
    switch ((int)nflag) {
	case 1:  goto L160;
	case 2:  goto L220;
    }

/* *****FINISHED */
L600:
    return 0;
} /* qdcomp_ */

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
#undef maxit
#undef itcnt
#undef jdata
#undef tpiv
#undef jout
#undef jsol
#undef ierr
#undef kobj


