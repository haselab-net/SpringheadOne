#ifndef VR_DRREALDEVICEPOOL_H
#define VR_DRREALDEVICEPOOL_H
#include "DRRealDevice.h"

namespace LVRM {

/**	���f�o�C�X�̃v�[��. 
	���[�U��ISA�J�[�h��PC�J�[�h�Ȃǂ̎��f�o�C�X�������ɓo�^����D
	�f�o�C�X�͏��������ɁC���z�f�o�C�X(DVVirtualDevices)�ɓo�^�����D	*/
class DRRealDevicePool: std::vector<DRRealDevice*>{
public:
	DRRealDevicePool();
	~DRRealDevicePool();
	///	�o�^
	void Register(DRRealDevice* dev);
	///	������
	void Init(DVVirtualDevicePool& vpool);
	///	�_���v�o��
	void Print(std::ostream& o) const;
};
inline std::ostream& operator << (std::ostream& o, const DRRealDevicePool& d){
	d.Print(o);
	return o;
}
}	//	namespace LVRM

#endif
