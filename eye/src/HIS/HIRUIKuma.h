#ifndef HIS_RUIKUMA_H
#define HIS_RUIKUMA_H
#include "HIRuiBase.h"
#include <WinBasis/WBComStream.h>

namespace Spr {

///	RUI�̊��N���X
class SPR_DLL HIRuiKuma: public HIRuiBase{
private:
	int data[6];
	unsigned char receiveBuffer[50];
	int senddata[6];
	unsigned char sendBuffer[50];

protected:
	WBComStream com;
	WBComStatus comSt;

public:
	///	�f�o�C�X�̃^�C�v
	HIOBJECTDEF(HIRuiKuma);
	///	�W���C���g��

	bool Init();
	virtual int NJoint(){return 4;}
	virtual float GetJointAngle(int jn);
	virtual void SetTorque(int jn, float torque);
	virtual void Update(float dt);

};

}	//	namespace Spr
#endif
