/* QDRVER.F -- translated by f2c (version 19960711).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"
#include "headrs.h"
/* Common Block Declarations */

#define qprcbi_1 qprcbi_

#define qprcbr_1 qprcbr_

/* Table of constant values */

static integer c__1 = 1;

/* Subroutine */ int qdrver_(kt)
integer *kt;
{
    /* Initialized data */

    static integer zerone[4] = { 5,8,9,11 };
    static integer zero12[3] = { 10,12,13 };

    /* Format strings */
    static char fmt_22[] = "(///5x,\002QUADPR  CALL\002,i4)";
    static char fmt_24[] = "(/1x,\002*** ERROR, ILLEGAL VALUES IN KT.  KT CO\
NTAINS\002/(6i12))";
    static char fmt_40[] = "(/1x,\002*** ERROR, INCONSISTENT DATA IN INPUT. \
 INPUT CONTAINS\002/5i12/5i12/4i12//5x,\002THE FOLLOWING ERRORS OCCURRED.\
\002/)";
    static char fmt_41[] = "(5x,\002INPUT(3) NEGATIVE OR EXCEEDS INPUT(1)\
\002)";
    static char fmt_42[] = "(5x,\002INPUT(4) NON-POISTIVE OR EXCEEDS INPUT\
(2)\002)";
    static char fmt_43[] = "(5x,\002INPUT(6) TOO SMALL.  FOR THIS PROBLEM\
,\002,\002 MUST BE AT LEAST\002,i8)";
    static char fmt_46[] = "(5x,\002INPUT(\002,i2,\002) NOT 0 OR 1\002)";
    static char fmt_48[] = "(5x,\002INPUT(\002,i2,\002) NOT 0, 1, OR 2\002)";

    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    integer s_wsfe(), do_fio(), e_wsfe();

    /* Local variables */
#define kobj ((integer *)&qprcbi_1 + 7)
#define ierr ((integer *)&qprcbi_1 + 14)
#define jsol ((integer *)&qprcbi_1 + 11)
#define jout ((integer *)&qprcbi_1 + 12)
#define tpiv ((real *)&qprcbr_1 + 1)
    static integer i__, j;
#define jdata ((integer *)&qprcbi_1 + 9)
#define itcnt ((integer *)&qprcbi_1 + 15)
#define maxit ((integer *)&qprcbi_1 + 6)
#define lenws ((integer *)&qprcbi_1 + 5)
    static logical error[10];
#define tzero ((real *)&qprcbr_1)
#define ml ((integer *)&qprcbi_1)
#define nl ((integer *)&qprcbi_1 + 1)
#define mo ((integer *)&qprcbi_1 + 2)
#define no ((integer *)&qprcbi_1 + 3)
#define minmax ((integer *)&qprcbi_1 + 4)
#define jwidth ((integer *)&qprcbi_1 + 13)
    static integer ititle;
#define jpivot ((integer *)&qprcbi_1 + 10)
#define jit ((integer *)&qprcbi_1 + 8)

    /* Fortran I/O blocks */
    static cilist io___23 = { 0, 0, 0, fmt_22, 0 };
    static cilist io___24 = { 0, 0, 0, fmt_24, 0 };
    static cilist io___27 = { 0, 0, 0, fmt_22, 0 };
    static cilist io___28 = { 0, 0, 0, fmt_40, 0 };
    static cilist io___29 = { 0, 0, 0, fmt_41, 0 };
    static cilist io___30 = { 0, 0, 0, fmt_42, 0 };
    static cilist io___31 = { 0, 0, 0, fmt_43, 0 };
    static cilist io___32 = { 0, 0, 0, fmt_46, 0 };
    static cilist io___33 = { 0, 0, 0, fmt_48, 0 };



/*     CHECK INPUT DATA FOR QUADPR */




    /* Parameter adjustments */
    --kt;

    /* Function Body */


    ititle = 0;

/*     DEFAULT VALUES */
    if (*maxit <= 0) {
	*maxit = 1000;
    }
    if (*tzero <= (float)0.) {
	*tzero = (float)1e-7;
    }
    if (*tpiv <= (float)0.) {
	*tpiv = (float)1e-6;
    }
    if (*jwidth < 72) {
	*jwidth = 72;
    }
    if (*jwidth > 132) {
	*jwidth = 132;
    }

    ititle = 0;

/*     CHECK KT */
    i__1 = *mo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if ((i__2 = kt[i__], abs(i__2)) > 1) {
	    goto L20;
	}
/* L10: */
    }
    goto L30;
L20:
    io___23.ciunit = qprcbi_1.io1;
    s_wsfe(&io___23);
    do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
    e_wsfe();
    io___24.ciunit = qprcbi_1.io1;
    s_wsfe(&io___24);
    i__1 = *mo;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_fio(&c__1, (char *)&kt[i__], (ftnlen)sizeof(integer));
    }
    e_wsfe();
    *ierr = 5;
    ititle = 1;

/*     CHECK INPUT */
L30:
    error[0] = *mo < 0 || *mo > *ml;
    error[1] = *no < 1 || *no > *nl;
    error[2] = *lenws < qprcbi_1.lenreq;
    for (i__ = 1; i__ <= 4; ++i__) {
	j = zerone[i__ - 1];
	error[i__ + 2] = qprcbi_1.inputs[j - 1] != 0 && qprcbi_1.inputs[j - 1]
		 != 1;
/* L33: */
    }
    for (i__ = 1; i__ <= 3; ++i__) {
	j = zero12[i__ - 1];
	error[i__ + 6] = qprcbi_1.inputs[j - 1] < 0 || qprcbi_1.inputs[j - 1] 
		> 2;
/* L34: */
    }
    for (i__ = 1; i__ <= 10; ++i__) {
	if (error[i__ - 1]) {
	    goto L38;
	}
/* L35: */
    }
    goto L50;

L38:
    *ierr = 5;
    if (ititle == 0) {
	io___27.ciunit = qprcbi_1.io1;
	s_wsfe(&io___27);
	do_fio(&c__1, (char *)&qprcbi_1.ncall, (ftnlen)sizeof(integer));
	e_wsfe();
    }
    io___28.ciunit = qprcbi_1.io1;
    s_wsfe(&io___28);
    for (i__ = 1; i__ <= 14; ++i__) {
	do_fio(&c__1, (char *)&qprcbi_1.inputs[i__ - 1], (ftnlen)sizeof(
		integer));
    }
    e_wsfe();
    if (error[0]) {
	io___29.ciunit = qprcbi_1.io1;
	s_wsfe(&io___29);
	e_wsfe();
    }
    if (error[1]) {
	io___30.ciunit = qprcbi_1.io1;
	s_wsfe(&io___30);
	e_wsfe();
    }
    if (error[2]) {
	io___31.ciunit = qprcbi_1.io1;
	s_wsfe(&io___31);
	do_fio(&c__1, (char *)&qprcbi_1.lenreq, (ftnlen)sizeof(integer));
	e_wsfe();
    }
    for (i__ = 1; i__ <= 4; ++i__) {
	j = zerone[i__ - 1];
	if (error[i__ + 2]) {
	    io___32.ciunit = qprcbi_1.io1;
	    s_wsfe(&io___32);
	    do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
	    e_wsfe();
	}
/* L45: */
    }
    for (i__ = 1; i__ <= 3; ++i__) {
	j = zero12[i__ - 1];
	if (error[i__ + 6]) {
	    io___33.ciunit = qprcbi_1.io1;
	    s_wsfe(&io___33);
	    do_fio(&c__1, (char *)&j, (ftnlen)sizeof(integer));
	    e_wsfe();
	}
/* L47: */
    }

L50:
    return 0;
} /* qdrver_ */

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


