#ifndef SGBEHAVIORENGINE_H
#define SGBEHAVIORENGINE_H

#include "SGEngine.h"

namespace Spr {;
///	�V�[���̐U�����i�r�w�C�r�A�j���Ǘ�����G���W��
/// �r�w�C�r�A�̕��ނƗD�揇��
///	100�̔{���͕��ނ�\���A�ȉ��̈ʂ͂��̒��ł̎��s�������K�肷��
enum SGBehaviorPriority{
	//���������W���[��
	SGBP_INITIALIZER			= 100,
	SGBP_CLEARFORCE,
	//�͐������W���[��
	SGBP_FORCEGENERATOR			= 200,
		SGBP_GRAVITYENGINE,
		SGBP_COLLISIONENGINE,
		SGBP_PENALTYENGINE,
		SGBP_CREATURE,
	//�ϕ��̑Ώ�
	SGBP_DYNAMICALSYSTEM		= 300,
		SGBP_JOINTENGINE,
		SGBP_SOLIDCONTAINER,
	//
	SGBP_MOTION					= 400,
		SGBP_CHANGEOBJECT,
		SGBP_CONSTRAINT,
	//���̃G���W���̃��X�i
	SGBP_LISTENER				= 500,
		SGBP_CONTACTENGINE,
		SGBP_STICKENGINE,
		SGBP_WARPENGINE,
};
class GRRender;
class SGBehaviorStates;
///	����G���W���̊�{�N���X�D
class SGBehaviorEngine:public SGEngine{
public:
	SGOBJECTDEF(SGBehaviorEngine);
	///	���s���������߂�v���C�I���e�B�l�D�������قǑ���
	virtual int GetPriority() const { return SGBP_FORCEGENERATOR; }
	///	���Ԃ� dt �i�߂�
	virtual void Step(SGScene* s){}
	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states){}
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const {}
	///	�����_�����O
	virtual void Render(GRRender* r, SGScene* s){}
};

///	SGBehaviorEngine�̔z��
class SGBehaviorEngines: public SGEngines{
protected:
	iterator initializer;
	iterator forceGenerator;
	iterator dynamicalSystem;
	iterator motion;
	iterator listener;
public:
	SGBehaviorEngines();
	///@name �V�~�����[�V������i�߂郁�\�b�h
	//@{	
	///	�͂̃N���A�D
	virtual void ClearForce(SGScene* s);
	///	�͂𐶐�(�v�Z)����
	virtual void GenerateForce(SGScene* s);
	///	ODE(�ϕ�����)�D�́����x�C���x���ʒu �̌v�Z���s���D
	virtual void Integrate(SGScene* s);
	//@}
	void Add(SGEngine* e);
	bool Del(SGEngine* e);
	void Sort();
	///	���[�h��̏���
	virtual void Loaded(SGScene* scene);
	///	�V�[���O���t�̃N���A���̏���
	virtual void Clear(SGScene* s);

	///	��Ԃ̓ǂݏo��
	virtual void LoadState(const SGBehaviorStates& states);
	///	��Ԃ̕ۑ�
	virtual void SaveState(SGBehaviorStates& states) const;
	///	�����_�����O
	virtual void Render(GRRender* r, SGScene* s){
		for(iterator it = begin2(); it != end2(); ++it){
			((SGBehaviorEngine*)&**it)->Render(r,s);
		}
	}
};

/**	BehaviorEngine�̏�ԕۑ��p	*/
class SGBehaviorState: public UTRefCount{
public:
	SGOBJECTDEF(SGBehaviorState);
};
///	�SBehaviorEngine�̏�Ԃ�ێ����邽�߂̃N���X
class SGBehaviorStates:public std::vector< UTRef<SGBehaviorState> >{
public:
	typedef std::vector< UTRef<SGBehaviorState> > base;
	mutable unsigned cur;
	SGBehaviorStates():cur(0){}
	void clear(){ base::clear(); cur=0; }
	void SetCursorFirst() const { cur = 0; }
	const SGBehaviorState* GetNext() const { return cur<size() ? &*at(cur++) : NULL; }
	SGBehaviorState* GetNext(){ return cur<size() ? &*at(cur++) : NULL; }
};
///	
class SGBehaviorStateStack: public std::vector<SGBehaviorStates>{
};
}
#endif
