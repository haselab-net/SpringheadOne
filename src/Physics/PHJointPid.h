#ifndef PH_JOINTPID_H
#define PH_JOINTPID_H

#include <Physics/PHJoint1D.h>
#include <Physics/PHJointMulti.h>

namespace Spr{;

class PHJointPid:public SGBehaviorEngine{
protected:
	///	�O��̖ڕW�l
	float lastGoal;
	///	�ϕ�
	float integratedError;
public:
	SGOBJECTDEF(PHJointPid);
	///	����Ώۊ֐�
	UTRef<PHJointBase> joint;
	///	�Ώۂ̎�
	int axis;
	///	�ڕW�l		Step()���ĂԑO�ɐݒ肷��D
	float goal;
	///	�ڕW�̑��x	Step()���ĂԑO�ɐݒ肷��D
	float dGoal;
	///	PID�̌W��
	float proportional, differential, integral;
	///	�ڕW�̃^�C�v 0:�ʒu����	1�F���x����
	int type;
	/// proportional,differential,integral�ɂ��g���N(PID����)
	float p_torque, d_torque, i_torque;

	/// p_torque, d_torque,i_torque�̎擾
	float GetProportionalTorque(){ return p_torque; }
	float GetDifferentialTorque(){ return d_torque; }
	float GetIntegralTorque(){ return i_torque; }

	///	�ڕW�l,�ڕW�̑��x	Step()���ĂԑO�ɐݒ肷��
	void SetPGoal(float p_goal){ goal = p_goal;}
	void SetDGoal(float d_goal){ dGoal = d_goal;}
	
	///	
	PHJointPid():type(0), proportional(0), differential(0), integral(0), goal(0), dGoal(FLT_MAX), lastGoal(0), integratedError(0), axis(0){}
	///
	static PHJointPid* Find(PHJoint1D* j, SGScene* scene);
	///	�v���C�I���e�B
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	�V�[�����N���A����Ƃ��̏���
	virtual void Clear(SGScene* s){ joint=NULL; }

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
	///	PID�����������D
	void Step(SGScene* s);	

	///
	double GetJointTorque(){ return joint->GetJointTorque(axis); }

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
protected:
};



/*	����ɂ���
	�j���[�g�������܂������Ƃ��āC
	- �ƁE�ӂ̃O���t�ŉ����\���D�܂���ߎ��D
	- �Ō�̎���,�萔 or ���S�� �ƁE�ӂ̐ݒ�_�ɂ��Đݒ�D
	- �ƁE�ӂ͈ᔽ������C�Ŋ�̃ƁE�ӂɌ�������PID����D
�R�[���ŗǂ����D
�R�[���̒��S�̌���Vec3f �Ɗp�x�Ƃ����ł悢�D
*/
class PHJointBallPid:public SGBehaviorEngine{
protected:
	///	�O��̃S�[��
	Quaterniond lastGoal;
	///	�ϕ�
	Vec3f integratedError;
public:
	SGOBJECTDEF(PHJointBallPid);
	///	����Ώۊ֐�
	UTRef<PHJointBall> joint;
	///	�ڕW�l			Step()���ĂԑO�ɐݒ肷��D
	Quaternionf goal;
	///	�ڕW�l�̔���	Step()���ĂԑO�ɐݒ肷��D�ݒ肵�Ȃ��Ă��ǂ��D
	Vec3f dGoal;
	///	PID�̌W��
	float proportional, differential, integral;
	/// proportional,differential,integral�ɂ��g���N(PID����)
	Vec3f p_torque, d_torque, i_torque;

	/// p_torque, d_torque,i_torque�̎擾
	Vec3f GetProportionalTorque(){ return p_torque; }
	Vec3f GetDifferentialTorque(){ return d_torque; }
	Vec3f GetIntegralTorque(){ return i_torque; }
	
	///	
	PHJointBallPid():proportional(0), differential(0), integral(0), dGoal(FLT_MAX,0,0){}
	///
	static PHJointBallPid* Find(PHJointBall* j, SGScene* scene);
	///	�v���C�I���e�B
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	�V�[�����N���A����Ƃ��̏���
	virtual void Clear(SGScene* s){ joint=NULL; }

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
	///	PID�����������D
	void Step(SGScene* s);

	///
	Vec3f GetJointTorque(){ return joint->GetTorque(); }

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);	
};


}

#endif