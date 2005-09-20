#include "f2c.h"

#ifdef KR_headers
extern double sin(), cos(), sinh(), cosh();

void c_cos(complex *r, complex *z);

void c_cos(complex *r, complex *z)
{
	r->r = cos(z->r) * cosh(z->i);
	r->i = - sin(z->r) * sinh(z->i);
}
#else
#undef abs
#include "math.h"



#endif