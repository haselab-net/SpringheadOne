#ifndef PH_JOINT_CONTROL_H
#define PH_JOINT_CONTROL_H

#include <Physics/PHJointPid.h>

namespace Spr{;

DEF_RECORD(XJointControl,{
	GUID Guid(){ return WBGuid("5A6DCC8B-BAD2-4baf-8F20-0EE5FB625996"); }
	FLOAT start;	//	deg
	FLOAT end;		//	deg
	FLOAT phase;	//	0.0-1.0
	FLOAT vel;		//	multi
});
DEF_RECORD(XJointControlEngine,{
	GUID Guid(){ return WBGuid("99971AA7-9DCE-40fd-9B0E-3C65BFFC17D9"); }
	FLOAT period;	//	sec
	FLOAT vel;
});
class PHJointControl:public SGObject, public XJointControl{
	SGOBJECTDEF(PHJointControl);
public:
	class TJoints:public std::vector< UTRef<PHJointPid> >{
	public:
	};
	TJoints joints;
	virtual void Step(float pos);

	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NReferenceObjects();
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ReferenceObject(size_t i);
	///	�q�I�u�W�F�N�g�̒ǉ�
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	�q�I�u�W�F�N�g�̍폜
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	//	�d�l�ύX�ɔ������[�_�[��WA
	virtual void Loaded(SGScene* scene);
};
class PHJointControlEngine:public SGBehaviorEngine, public XJointControlEngine{
	SGOBJECTDEF(PHJointControlEngine);
public:
	float time;
	class TJointControls:public std::vector< UTRef<PHJointControl> >{
	};
	TJointControls jcs;

	PHJointControlEngine();
	///	
	void Step(SGScene* scene);
	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects();
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i);
	///	�q�I�u�W�F�N�g�̒ǉ�
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	�q�I�u�W�F�N�g�̍폜
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	virtual void Loaded(SGScene* s);
};


}

#endif