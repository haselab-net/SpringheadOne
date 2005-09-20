#include "F2c.h"

#ifdef __cplusplus 
extern "C" int quadpr_(real *, long int* ,real *, real *, real *  , long int *, real *, char *, char *, real *, real *, real *, real *,real *,long int *, real *,ftnlen,ftnlen);
#else
extern int quadpr_(real *, long int* ,real *, real *, real * , long int *, real *, char *, char *, real *, real *, real *, real *,real *,long int *, real *,ftnlen,ftnlen);
#endif
//int * 2:  int * 6:  int * 15:
struct {
    integer inputs[14], iouts[2], row, col, indx, more, p, p1, psq, ncall, 
	    lenreq, io1, io2, io3;
} qprcbi_;

struct {
    real tols[2];
} qprcbr_;

struct {
    char titles[64], pfiles[64];
} qprcbc_;

struct {
    doublereal pivot;
} qprcbd_;
