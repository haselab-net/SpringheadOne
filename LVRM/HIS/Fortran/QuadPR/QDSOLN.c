/* QDSOLN.F -- translated by f2c (version 19960711).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "headrs.h"
/* Common Block Declarations */

#define qprcbi_1 qprcbi_

#define qprcbr_1 qprcbr_

/* Subroutine */ int qdsoln_(kt, cost, quad, ldq, b, ldb, jw, x, rc, dual, 
	slk, temp, obj)
integer *kt;
real *cost, *quad;
integer *ldq;
doublereal *b;
integer *ldb, *jw;
real *x, *rc, *dual, *slk;
doublereal *temp;
real *obj;
{
    /* System generated locals */
    integer quad_dim1, quad_offset, b_dim1, b_offset, i__1, i__2;

    /* Local variables */
#define kobj ((integer *)&qprcbi_1 + 7)
    static doublereal dart;
#define ierr ((integer *)&qprcbi_1 + 14)
    static integer kart;
#define jsol ((integer *)&qprcbi_1 + 11)
#define jout ((integer *)&qprcbi_1 + 12)
#define tpiv ((real *)&qprcbr_1 + 1)
    static integer i__, j, k;
#define jdata ((integer *)&qprcbi_1 + 9)
#define itcnt ((integer *)&qprcbi_1 + 15)
#define maxit ((integer *)&qprcbi_1 + 6)
#define lenws ((integer *)&qprcbi_1 + 5)
#define tzero ((real *)&qprcbr_1)
    static doublereal bb;
#define ml ((integer *)&qprcbi_1)
#define nl ((integer *)&qprcbi_1 + 1)
#define mo ((integer *)&qprcbi_1 + 2)
#define no ((integer *)&qprcbi_1 + 3)
#define minmax ((integer *)&qprcbi_1 + 4)
#define jwidth ((integer *)&qprcbi_1 + 13)
#define jpivot ((integer *)&qprcbi_1 + 10)
#define jit ((integer *)&qprcbi_1 + 8)


/*     RETURN SOLUTION TO QUADRATIC PROGRAMMING PROBLEM FROM FINAL */
/*     TABLEAU AND COMPUTE OBJECTIVE VALUE IF REQUESTED. */
/*     CONVERTING THE PROBLEM TO MINIMIZATION WITH ALL .GE. CONSTRAINTS. 
*/






/*     GET X, RC, DUAL, SLK */
    /* Parameter adjustments */
    --kt;
    --cost;
    quad_dim1 = *ldq;
    quad_offset = quad_dim1 + 1;
    quad -= quad_offset;
    b_dim1 = *ldb;
    b_offset = b_dim1 + 1;
    b -= b_offset;
    --jw;
    --x;
    --rc;
    --dual;
    --slk;
    --temp;

    /* Function Body */
/* L100: */
    kart = *mo + 1;
    i__1 = *no;
    for (j = 1; j <= i__1; ++j) {
	x[j] = (float)0.;
	rc[j] = (float)0.;
/* L110: */
    }
    i__1 = *mo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	dual[i__] = (float)0.;
	slk[i__] = (float)0.;
/* L120: */
    }
    dart = 0.;
    i__1 = qprcbi_1.p;
    for (i__ = 1; i__ <= i__1; ++i__) {
	bb = b[i__ + qprcbi_1.p1 * b_dim1];
	if (abs(bb) < *tzero) {
	    bb = 0.;
	}
	j = jw[i__];
	if (j < 0) {
	    goto L150;
	}
	if (j > *no) {
	    goto L130;
	}
	x[j] = bb;
	goto L170;
L130:
	k = j - *no;
	if (k == kart) {
	    goto L140;
	}
	dual[k] = bb;
	goto L170;
L140:
	dart = bb;
	goto L170;
L150:
	j = -j;
	if (j > *no) {
	    goto L160;
	}
	rc[j] = bb;
	goto L170;
L160:
	k = j - *no;
	if (k == kart) {
	    goto L170;
	}
	slk[k] = bb;
L170:
	;
    }

/*     ADJUST DUALS IF ANY EQUALITIES */
    if (qprcbi_1.more == 0) {
	goto L200;
    }
    i__1 = *mo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (kt[i__] != 0) {
	    goto L190;
	}
	if (*minmax == 1) {
	    goto L180;
	}
	dual[i__] -= dart;
	goto L190;
L180:
	dual[i__] = dart - dual[i__];
L190:
	;
    }

/*     COMPUTE OBJECTIVE VALUE ONLY IF REQUESTED. */
L200:
    if (*kobj == 0) {
	goto L250;
    }
    *obj = (float)0.;
    i__1 = *no;
    for (i__ = 1; i__ <= i__1; ++i__) {
	temp[i__] = 0.;
	i__2 = *no;
	for (j = 1; j <= i__2; ++j) {
	    temp[i__] += x[j] * quad[i__ + j * quad_dim1];
/* L210: */
	}
/* L220: */
    }
    i__1 = *no;
    for (j = 1; j <= i__1; ++j) {
	*obj += temp[j] * x[j];
/* L230: */
    }
    i__1 = *no;
    for (j = 1; j <= i__1; ++j) {
	*obj += cost[j] * x[j];
/* L240: */
    }

/*     FINISHED */
L250:
    return 0;
} /* qdsoln_ */

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


