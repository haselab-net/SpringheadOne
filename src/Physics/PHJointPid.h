#ifndef PH_JOINTPID_H
#define PH_JOINTPID_H

#include <Physics/PHJoint1D.h>

namespace Spr{;

class PHJointPid:public SGBehaviorEngine{
protected:
	///	�O��̌덷
	float error;
	///	�ϕ�
	float integratedError;
public:
	SGOBJECTDEF(PHJointPid);
	///	����Ώۊ֐�
	UTRef<PHJoint1D> joint;
	///	�ڕW�l
	float goal;
	///	PID�̌W��
	float proportional, differential, integral;
	///	�ڕW�̃^�C�v 0:�ʒu����	1�F���x����
	int type;
	
	///	
	PHJointPid():type(0), proportional(0), differential(0), integral(0), goal(0), error(0), integratedError(0){}
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

	size_t NReferenceObjects();
	SGObject* ReferenceObject(size_t i);
	bool AddChildObject(SGObject* o, SGScene* s);
	bool DelChildObject(SGObject* o, SGScene* s);
protected:
};


}

#endif