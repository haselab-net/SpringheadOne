#ifndef HIS_RUIBASE_H
#define HIS_RUIBASE_H
#include "HIHapticDevice.h"

namespace Spr {

///	RUI�̊��N���X
class SPR_DLL HIRuiBase: public HIHapticDevice{
protected:
public:
	HIOBJECTDEFABST(HIRuiBase);
	///	�W���C���g��
	virtual int NJoint()=0;
	virtual float GetJointAngle(int jn)=0;
	virtual void SetTorque(int jn, float toeque)=0;
//	virtual void Update(float dt);
};


}	//	namespace Spr
#endif
