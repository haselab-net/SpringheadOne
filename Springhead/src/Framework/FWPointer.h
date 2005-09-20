#ifndef FWPOINTER_H
#define FWPOINTER_H
#include <Physics/PHSolid.h>
#include <Physics/PHJointPid.h>
#include <HIS/HIHapticDevice.h>
#include <HIS/HIRuiBase.h>

namespace Spr{;
/**	�͊o�C���^�t�F�[�X�ƃV�[���O���t�̌����D
	+ �V�[�����̕��̂��A�o�^�[�Ƃ���D
		- �o�[�`�����J�b�v�����O�F�A�o�^�[���_�C�i�~�N�X�������C�o�l�_���p�łȂ��D
		- �v���L�V�F�_�C�i�~�N�X���������C�ʒu�E�p���𒼐ڎw��C�͂��
	+ �}�E�X�� drag and drop �̂悤�ɐ؂�ւ����ɂ���D

	�@�\�Ƃ��ẮC
	���_�R���g���[���E���̑���̐؂�ւ�
	�����_���[�C�J�����Ƃ̐ڑ�
	�V�[���O���t���̕��̂Ƃ̐ڑ��C�X�V
*/
class FWPointerBase: public UTRefCount{
public:
	float posScale, forceScale;
	bool bOutForce;
	UTRef<HIHapticDevice> device;
	UTRef<SGObject> obj;
	
	FWPointerBase(HIHapticDevice* d):device(d), posScale(1.0f), forceScale(1.0f), bOutForce(false){}
	virtual ~FWPointerBase(){}
	virtual void Step(float dt)=0;					///<	�X�V
	virtual void Connect(SGScene* scene){}
	virtual void SetOutForce(bool f){ bOutForce = f; }
	virtual void SetScale(float ps, float fs){posScale=ps; forceScale=fs;}
};
///
class FWPointer6D: public FWPointerBase{
protected:
	Vec3f pos;
	Quaternionf ori;
	Quaternionf qtOffset;
	Quaternionf qtOffsetInv;
	Vec3f v3Offset;
public:
	HIForceDevice6D*& Device(){ return (HIForceDevice6D*&)device; }
	FWPointer6D(HIForceDevice6D* d=NULL):FWPointerBase(d){
		posScale=1.0f; forceScale=1.0f;
		bOutForce = false;
	}
	virtual void Init(HIForceDevice6D*);	///<	������
	virtual void Connect(SGScene* s);
	virtual void Step(float dt);
	virtual void Update(float dt);

	virtual Vec3f GetPos();
	virtual Quaternionf GetOri();
	virtual Vec3f GetVel();
	virtual Vec3f GetAngVel();

	Quaternionf GetQTOffset()   { return qtOffset;    }
	Quaternionf GetQTOffsetInv(){ return qtOffsetInv; }
	Vec3f       GetV3Offset()   { return v3Offset;    }

	virtual void  SetForce(Vec3f, Vec3f);

	PHSolid* GetSolid(){ return UTRef<PHSolid>(obj); }
protected:
	UTRef<PHSolid>& Solid(){ return (UTRef<PHSolid>&)obj; }
};

class FWPointerRui: public FWPointerBase{
protected:
public:
	std::vector< UTRef<PHJointPid> > jointPids;
	HIRuiBase*& Device(){ return (HIRuiBase*&)device; }
	FWPointerRui(HIRuiBase* d=NULL):FWPointerBase(d){Init(d);}
	virtual void Init(HIRuiBase*);		///<	������
	virtual void Connect(SGScene* s);
	virtual void Step(float dt);
};


class FWPointers:public std::vector< UTRef<FWPointerBase> >{
public:
	void Connect(SGScene* s);
	void Step(float dt);
	FWPointerBase* Find(HIHapticDevice* dev);
};

}	//	namespace Spr
#endif
