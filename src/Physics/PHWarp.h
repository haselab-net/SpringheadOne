#ifndef PHWARP_H
#define PHWARP_H

#include <Collision/CDFramePair.h>
#include <Collision/CDCollisionEngine.h>
#include "PHSolid.h"

namespace Spr{;

/**	�w�肵�����́i�����w��j�����[�v�]�[���ɏՓ˂����Ƃ����[�v������D
    �Ⴆ�΁C���[�v�]�[���t���[�������̉��ɂЂ�[������Ă����΁C
    �����痎�����I�u�W�F�N�g�����̏���Ƀ��[�v������Ƃ������Ƃ��\�D
	PHWarpEngine �����D	*/
class PHWarp:public SGObject{
	SGOBJECTDEF(PHWarp);
protected:

public:
	Vec3f position;					///<	���[�v��̈ʒu								
	Vec3f velocity;					///<	���[�v��ł̑��x
	Vec3f angVel;					///<	���[�v��ł̊p���x
	UTRef<SGFrame> warpZoneFrame;	///<	���[�v�]�[��
	PHSolids solids;				///<	���[�v�����鍄�̂���

	///	�N���A
	void Clear();
	///	
	bool AddChildObject(SGObject* o, SGScene* s);

};

///	PHWarp�̔z��
class PHWarps:public std::vector< UTRef<PHWarp> >{
};


/** ���[�v�G���W�� */
class PHWarpEngine:public SGBehaviorEngine, CDCollisionListener {
	SGOBJECTDEF(PHWarpEngine);

public:
	/// �t���[���΂��Ƃ̃��[�v�f�[�^
	class FramePairRecord:public CDUserRecord{
	public:
		Vec3f position;				///<	���[�v��̈ʒu								
		Vec3f velocity;				///<	���[�v��ł̑��x
		Vec3f angVel;				///<	���[�v��ł̊p���x
		UTRef<PHSolid> solid[2];	///<	���[�v�����鍄��
		int frameID[2];				///<	�t���[���΂�ID
	};


public:
	/// PHWarpEngine �̌Ă΂��D�揇��
	int GetPriority() const { return SGBP_WARPENGINE; }
	///
	bool AddChildObject(SGObject* o, SGScene* s);
	/// �ڐG���ɌĂ΂��R�[���o�b�N�D���[�v�������鏈�����s���D
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	///	���Ԃ� dt �i�߂�i�������Ȃ��CListener �� Collide �����ׂĂ̏���������j
	virtual void Step(SGScene* s){}
	/// �N���A
	virtual void Clear(SGScene* s);
	/// ���[�h�������̏����CCollisionEngine �ɃR�[���o�b�N��o�^����DCollisionEngine ���Ȃ���΍��D
	virtual void Loaded(SGScene* scene);
	/// FramePairRecord �̓o�^
	void AddFramePairRecord(PHWarp* w, int f1, int f2, PHSolid* s1, PHSolid* s2=NULL);

	PHWarps warps;				///<	���[�v�̏��
protected:
	int listenerPos;			///< ���X�i�̈ʒu
	int framePairRecordPos;		///< ���[�v�G���W���p�� FramePairRecord �̈ʒu
	std::vector< UTRef<FramePairRecord> > framePairRecords;	///< �t���[���΂��Ƃ̃��[�v�f�[�^
};


};
#endif
