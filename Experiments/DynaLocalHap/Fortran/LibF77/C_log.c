#include "f2c.h"

#ifdef KR_headers
extern double log(), f__cabs(), atan2();
void c_log(complex *,complex *);

void c_log(complex *r, complex *z)
{
r->i = atan2(z->i, z->r);
r->r = log( f__cabs(z->r, z->i) );
}
#else
#undef abs
#include "math.h"
extern double f__cabs(double, double);



#endif