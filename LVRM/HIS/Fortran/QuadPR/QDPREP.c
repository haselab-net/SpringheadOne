/* QDPREP.F -- translated by f2c (version 19960711).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "headrs.h"
/* Common Block Declarations */


#define qprcbi_1 qprcbi_

/* Subroutine */ int qdprep_(a, lda, kt, rhs, cost, quad, ldq, b, ldb)
real *a;
integer *lda, *kt;
real *rhs, *cost, *quad;
integer *ldq;
doublereal *b;
integer *ldb;
{
    /* System generated locals */
    integer a_dim1, a_offset, quad_dim1, quad_offset, b_dim1, b_offset, i__1, 
	    i__2;

    /* Local variables */
#define kobj ((integer *)&qprcbi_1 + 7)
#define ierr ((integer *)&qprcbi_1 + 14)
#define jsol ((integer *)&qprcbi_1 + 11)
#define jout ((integer *)&qprcbi_1 + 12)
    static integer i__, k, l;
#define jdata ((integer *)&qprcbi_1 + 9)
#define itcnt ((integer *)&qprcbi_1 + 15)
#define maxit ((integer *)&qprcbi_1 + 6)
#define lenws ((integer *)&qprcbi_1 + 5)
    static integer l1, n1;
#define ml ((integer *)&qprcbi_1)
#define nl ((integer *)&qprcbi_1 + 1)
#define mo ((integer *)&qprcbi_1 + 2)
#define no ((integer *)&qprcbi_1 + 3)
#define minmax ((integer *)&qprcbi_1 + 4)
#define jwidth ((integer *)&qprcbi_1 + 13)
#define jpivot ((integer *)&qprcbi_1 + 10)
#define jit ((integer *)&qprcbi_1 + 8)


/*     CONSTRUCT THE INITIAL TABLEAU FOR QUADRATIC PROGRAMMING. */
/*     IT HAS THE FORM */
/*             ......................................... */
/*             .           (T)    .     (T)     .      . */
/*             . QUAD'+QUAD'      .   -A'       . COST'. */
/*             .     [1]          .   [2]       . [3]  . */
/*     B    =  ......................................... */
/*             .     A'           .    0        . -RHS'. */
/*             .     [4]          .   [5]       . [6]  . */
/*             ......................................... */

/*     WHERE A',RHS',COST',QUAD' ARE OBTAINED FROM A,RHS,COST,QUAD BY */
/*     CONVERTING THE PROBLEM TO MINIMIZATION WITH ALL .GE. CONSTRAINTS. 
*/




/*     FILL LOWER TRIANGLE OF [1] AND [3]. */
    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;
    --kt;
    --rhs;
    --cost;
    quad_dim1 = *ldq;
    quad_offset = quad_dim1 + 1;
    quad -= quad_offset;
    b_dim1 = *ldb;
    b_offset = b_dim1 + 1;
    b -= b_offset;

    /* Function Body */
    n1 = *no - 1;
    if (*minmax == 1) {
	goto L40;
    }
/*     MIN. PROBLEM */
    b[b_dim1 + 1] = quad[quad_dim1 + 1] * (float)2.;
    b[qprcbi_1.p1 * b_dim1 + 1] = cost[1];
    if (*no == 1) {
	goto L110;
    }
    i__1 = *no;
    for (k = 2; k <= i__1; ++k) {
	b[k + k * b_dim1] = quad[k + k * quad_dim1] * (float)2.;
	b[k + qprcbi_1.p1 * b_dim1] = cost[k];
/* L10: */
    }
    i__1 = n1;
    for (l = 1; l <= i__1; ++l) {
	l1 = l + 1;
	i__2 = *no;
	for (k = l1; k <= i__2; ++k) {
	    b[k + l * b_dim1] = quad[k + l * quad_dim1] + quad[l + k * 
		    quad_dim1];
/* L20: */
	}
/* L30: */
    }
    goto L80;
/*     MAX. PROBLEM */
L40:
    b[b_dim1 + 1] = quad[quad_dim1 + 1] * (float)-2.;
    b[qprcbi_1.p1 * b_dim1 + 1] = -cost[1];
    if (*no == 1) {
	goto L110;
    }
    i__1 = *no;
    for (k = 2; k <= i__1; ++k) {
	b[k + k * b_dim1] = quad[k + k * quad_dim1] * (float)-2.;
	b[k + qprcbi_1.p1 * b_dim1] = -cost[k];
/* L50: */
    }
    i__1 = n1;
    for (l = 1; l <= i__1; ++l) {
	l1 = l + 1;
	i__2 = *no;
	for (k = l1; k <= i__2; ++k) {
	    b[k + l * b_dim1] = -quad[k + l * quad_dim1] - quad[l + k * 
		    quad_dim1];
/* L60: */
	}
/* L70: */
    }

/*     FILL UPPER TRAINGLE OF [1]. */
L80:
    i__1 = *no;
    for (l = 2; l <= i__1; ++l) {
	l1 = l - 1;
	i__2 = l1;
	for (k = 1; k <= i__2; ++k) {
	    b[k + l * b_dim1] = b[l + k * b_dim1];
/* L90: */
	}
/* L100: */
    }

/*     CHECK FOR CASE OF NO CONSTRAINTS. */
L110:
    if (*mo == 0) {
	goto L240;
    }

/*     FILL [5]. */
    n1 = *no + 1;
    i__1 = qprcbi_1.p;
    for (l = n1; l <= i__1; ++l) {
	i__2 = qprcbi_1.p;
	for (k = n1; k <= i__2; ++k) {
	    b[k + l * b_dim1] = (float)0.;
/* L120: */
	}
/* L130: */
    }

/*     IF THERE ARE R .EQ. CONSTRAINTS, EXPRESS AS R+1 .GE. CONSTRAINTS, 
*/
/*     THE LATTER BEING MINUS THE SUM OF THE OTHERS. */
    if (qprcbi_1.more == 0) {
	goto L150;
    }
    i__1 = *no;
    for (l = 1; l <= i__1; ++l) {
	b[qprcbi_1.p + l * b_dim1] = (float)0.;
/* L140: */
    }
    b[qprcbi_1.p + qprcbi_1.p1 * b_dim1] = (float)0.;

/*     FILL [4] AND [6]. */
L150:
    i__1 = *mo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k = *no + i__;
	if (kt[i__] < 0) {
	    goto L170;
	}
	if (kt[i__] > 0) {
	    goto L190;
	}
/*        .EQ. CONSTRAINT */
	i__2 = *no;
	for (l = 1; l <= i__2; ++l) {
	    b[k + l * b_dim1] = a[i__ + l * a_dim1];
	    b[qprcbi_1.p + l * b_dim1] -= b[k + l * b_dim1];
/* L160: */
	}
	b[k + qprcbi_1.p1 * b_dim1] = -rhs[i__];
	b[qprcbi_1.p + qprcbi_1.p1 * b_dim1] -= b[k + qprcbi_1.p1 * b_dim1];
	goto L210;
/*        .GE. CONSTRAINT */
L170:
	i__2 = *no;
	for (l = 1; l <= i__2; ++l) {
	    b[k + l * b_dim1] = a[i__ + l * a_dim1];
/* L180: */
	}
	b[k + qprcbi_1.p1 * b_dim1] = -rhs[i__];
	goto L210;
/*        .LE. CONSTRAINT */
L190:
	i__2 = *no;
	for (l = 1; l <= i__2; ++l) {
	    b[k + l * b_dim1] = -a[i__ + l * a_dim1];
/* L200: */
	}
	b[k + qprcbi_1.p1 * b_dim1] = rhs[i__];
L210:
	;
    }

/*     FILL [2]. */
    i__1 = *no;
    for (l = 1; l <= i__1; ++l) {
	i__2 = qprcbi_1.p;
	for (k = n1; k <= i__2; ++k) {
	    b[l + k * b_dim1] = -b[k + l * b_dim1];
/* L220: */
	}
/* L230: */
    }

/*     FINISHED */
L240:
    return 0;
} /* qdprep_ */

#undef jit
#undef jpivot
#undef jwidth
#undef minmax
#undef no
#undef mo
#undef nl
#undef ml
#undef lenws
#undef maxit
#undef itcnt
#undef jdata
#undef jout
#undef jsol
#undef ierr
#undef kobj


