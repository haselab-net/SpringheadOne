#include "f2c.h"

#ifdef KR_headers
extern double exp(), cos(), sin();

void c_exp(complex *r,complex *z);

void c_exp(complex *r, complex *z)
{
double expx;

expx = exp(z->r);
r->r = expx * cos(z->i);
r->i = expx * sin(z->i);
}
#else
#undef abs
#include "math.h"


#endif