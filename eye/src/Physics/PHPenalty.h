#ifndef PHPENALTY_H
#define PHPENALTY_H
#include "Physics.h"

namespace Spr{;

/**	2�̃t���[���Ԃ̏Փ˂����o���C�y�i���e�B�����߁C�͂�������D
	PHPenaltyEngine �����D	*/
class PHPenalty:public SGObject, public CDFramePairWithNormal{
	SGOBJECTDEF(PHPenalty);
	float convertedMass;			///<	2���̂̊��Z���ʁD�o�l�W�������߂邽�߂Ɏg�p�D
	float area;						///<	2�̃t���[���Ԃ̐ڐG�̈�̖ʐ�
	float dt;						///<	delta t
protected:
public:
	UTRef<PHSolid> solid[2];		///<	���ʂ𔽉f�����鍄��

public:
	///
	PHPenalty(){ Init(); }
	///
	PHPenalty(PHSolid* s1, PHSolid* s2){ Init(); Set(s1, s2); }
	~PHPenalty(){
		solid[0] = NULL;
		solid[1] = NULL;
	}
	///	detector ������������D
	void Init();
	///	���[�h��C�Q�Ƃ��������Ă���CconvertedMass ���v�Z����D
	void Loaded(SGScene* scene);
	///	Solid��ݒ�
	void Set(PHSolid* s1, PHSolid* s2);
	///	dt �����V�~�����[�V������i�߂�D
	void Step(SGScene* s);
	///	���C�̃o�l�̕\��
	void Draw(SGScene* s);
	///	���Z���ʂ��v�Z
	void CalcConvertedMass();
	///	solid��detector��NULL�ŃN���A����D���������[�N�΍�D
	void Clear();
	///
	bool operator < (const PHPenalty& p) const {
		if (solid[0] < p.solid[0]) return true;
		if (p.solid[0] < solid[0]) return false;
		if (solid[1] < p.solid[1]) return true;
		return false;
	}
	bool AddChildObject(SGObject* o, SGScene* s);
protected:
	virtual CDConvexPair* CreateConvexPair(){
		return new PHConvexPair;
	}
	virtual CDGeometryPair* CreateGeometryPair(){
		return new PHGeometryPair;
	}
	friend class PHPenaltyLoader;
	friend class PHPenaltySaver;
};
///	PHPenalty�̔z��
class PHPenalties:public std::vector< UTRef<PHPenalty> >{
};
/**	�y�i���e�B�@�̃G���W���D
	�Փ˔�����s���C�y�i���e�B�����߁CPHSolid�ɗ͂�������D	*/
class PHPenaltyEngine:public SGBehaviorEngine{
	SGOBJECTDEF(PHPenaltyEngine);
public:
	///	2�̍��̂̃y�i���e�B�v�Z
	PHPenalties penalties;
	bool AddChildObject(SGObject* o, SGScene* s);
	int GetPriority() const { return SGBP_PENALTYENGINE; }
	///	���Ԃ� dt �i�߂�
	virtual void Step(SGScene* s);
	///	�y�i���e�B���N���A����D
	virtual void Clear(SGScene* s);
	virtual void Loaded(SGScene* scene){
		for(PHPenalties::iterator it = penalties.begin(); it != penalties.end(); ++it){
			(*it)->Loaded(scene);
		}
	}
};


}
#endif
