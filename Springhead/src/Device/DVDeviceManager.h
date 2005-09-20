#ifndef VR_DVDEVICEMANAGER_H
#define VR_DVDEVICEMANAGER_H

#include "DVVirtualDevicePool.h"
#include "DRRealDevicePool.h"
#include <iosfwd>

namespace Spr {
///	�f�o�C�X�̊Ǘ��N���X�D
class SPR_DLL DVDeviceManager{
	DVVirtualDevicePool vpool;
	DRRealDevicePool rpool;
public:
	DVVirtualDevicePool& VPool() { return vpool; }
	const DVVirtualDevicePool& VPool() const { return vpool; }
	DRRealDevicePool& RPool() { return rpool; }
	const DRRealDevicePool& RPool() const { return rpool; }
	void Init();
	void Clear();
	void Print(std::ostream& o) const;
};
inline std::ostream& operator << (std::ostream& o, const DVDeviceManager& m){
	m.Print(o);
	return o;
}

}	//	namespace Spr

#endif
