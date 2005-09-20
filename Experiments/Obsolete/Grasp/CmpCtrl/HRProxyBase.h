#ifndef HRPROXYBASE_H
#define HRPROXYBASE_H
#include <Base/ringbuf.h>
#include <Base/Affine.h>
#include <Base/utilities.h>

namespace  LVRM{

class HRProxyBase:public UTRefCount{
public:
	static float dt;
};

}
#endif
