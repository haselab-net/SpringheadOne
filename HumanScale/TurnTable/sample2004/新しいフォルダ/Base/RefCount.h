#ifndef VR_BASE_REFCOUNT
#define VR_BASE_REFCOUNT
#include <assert.h>
/**	@file RefCount.h �Q�ƃJ�E���^�N���X�̒�`.	*/
namespace LVRM {
///	�Q�ƃJ�E���^�D
class CRefCount{
	int refCount;
	public:
	CRefCount(){refCount = 1;}
	virtual ~CRefCount(){assert(refCount == 0);}
	int AddRef(){refCount++; return refCount;}
	int Release(){
		refCount--;
		int ref = refCount;
		if (ref == 0) delete this;
		return ref;
	}
};
}	//	namespace LVRM
#endif
