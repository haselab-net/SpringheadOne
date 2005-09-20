#ifndef VR_DRREALDEVICEBASE_H
#define VR_DRREALDEVICEBASE_H

#include "DVVirtualDevicePool.h"

namespace LVRM {

///	���f�o�C�X�̊�{�N���X�DISA�J�[�h1���Ȃǂɑ�������.
class DRRealDevice{
public:
	DRRealDevice(){}
	virtual ~DRRealDevice(){}
	///	�f�o�C�X�̖��O
	virtual const char* Name() const=0;
	///	������
	virtual bool Init()=0;
	///	���z�f�o�C�X�̓o�^
	virtual void Register(DVVirtualDevicePool& vpool)=0;
	///	��Ԃ̍X�V
	virtual void Update(){}
	///	�_���v�o��
	virtual void Print(std::ostream& o) const;
};
inline std::ostream& operator << (std::ostream& o, const DRRealDevice& d){
	d.Print(o);
	return o;
}
}	//	namespace LVRM

#endif
