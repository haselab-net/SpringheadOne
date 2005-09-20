#include "f2c.h"

#ifdef KR_headers
long int pow_qq(ap, bp) longint *ap, *bp;
#else
long int pow_qq(long int *ap, long int *bp)
#endif
{
	long int pow, x, n;
	unsigned long u;	/* system-dependent */

	x = *ap;
	n = *bp;

	if (n <= 0) {
		if (n == 0 || x == 1)
			return 1;
		if (x != -1)
			return x == 0 ? 1/x : 0;
		n = -n;
		}
	u = n;
	for(pow = 1; ; )
		{
		if(u & 01)
			pow *= x;
		if(u >>= 1)
			x *= x;
		else
			break;
		}
	return(pow);
	}
