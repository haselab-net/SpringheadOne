#ifndef PH_JOINT1D_H
#define PH_JOINT1D_H

#include "PHJoint.h"

namespace Spr{;

struct XJoint1D;

///	1���R�x�̊֐�
class PHJoint1D:public PHJointBase{
public:
	SGOBJECTDEFABST(PHJoint1D);
	double minPosition;						///<	���͈�(�ő�l)
	double maxPosition;						///<	���͈�(�ŏ��l)
	double torque;							///<	�g���N
	double accel;							///<	�֐߉����x
	double position;						///<	�ψ�
	double velocity;						///<	���x
	void AddTorque(double t){ torque+=t; }	///<	�g���N��ǉ�
	void SetTorque(double t){ torque=t; }	///<	�g���N��ݒ�
	double GetTorque(){ return torque; }	///<	�g���N���擾
	double GetPosition(){ return position; }///<	�֐ߊp�x���擾
	double GetVelocity(){ return velocity; }///<	�֐ߑ��x���擾
	virtual void Integrate(double dt);		///<	�ϕ�
	void CompArticulatedInertia(double dt);	///<	articulated inertia & ZA-force
	void CalcAccel(double dt);				///<	���̃W���C���g�̉����x�v�Z�D�ڍׂ͊�{�N���X�̃R�����g���Q�ƁD

	///	�֐߂̎��R�x
	virtual int GetJointDof(){ return 1; }
	///	�֐߈ʒu�̎擾
	virtual double GetJointPosition(int i){ return position; }
	///	�֐ߑ��x�̎擾
	virtual double GetJointVelocity(int i){ return velocity; }
	///	�֐߉����x�̎擾
	virtual double GetJointAccel(int i){ return accel; }
	///	�֐߃g���N�̎擾
	virtual double GetJointTorque(int i){ return torque; }
	///	�֐߃g���N�̐ݒ�
	virtual void SetJointTorque(double v, int i){ torque = v; }
	///	�֐߃g���N��������
	virtual void AddJointTorque(double v, int i){ torque += v; }

	void SaveX(XJoint1D&) const;
	void LoadX(const XJoint1D&);
protected:
	SpVec6d			s;					///<	spatial joint axis in Fc coord.
	//	s���܂ރL���b�V���ϐ�
	SpVec6d			Ia_s;
	double			dot_s_Ia_s, dot_s_Z_plus_Ia_c;

	virtual void Reset();
	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
	
	virtual void ClearTorqueRecursive();
	virtual void LimitAngle(double& d){}
};

///	���Ԋ֐�
class PHJointHinge:public PHJoint1D{
public:
	SGOBJECTDEF(PHJointHinge);
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();
protected:
	virtual void LimitAngle(double& d){ LimitCycle(d); }
	void Integrate(double dt);
};
///	�X���C�h�֐�
class PHJointSlider:public PHJoint1D{
public:
	SGOBJECTDEF(PHJointSlider);
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();
};

}

#endif