#ifndef PHCHANGEOBJECT_H
#define PHCHANGEOBJECT_H

#include <Collision/CDFramePair.h>
#include <Collision/CDCollisionEngine.h>
#include "PHSolid.h"

namespace Spr{;

/** Solid�̓���ւ����s���N���X�̊�{�N���X�DPHChangeObjectContainer������ */
class PHChangeObject:public SGObject{
	SGOBJECTDEFABST(PHChangeObject);
protected:

public:
	UTRef<PHSolid> solid[2];		///<	����ւ��O[0]�Ɠ���ւ���[1]�̍���

	/// IsChange()�̌��ʂɉ�����Change()�����s����DPHChangeObjectContainer::Step()����Ă΂��
	void Step();
	/// Solid�̓���ւ����s��
	void Change();
	/// ����ւ��������s�����ǂ������f����
	virtual bool IsChange()=0;
	/// �N���A
	virtual void Clear()=0;
};

///	PHChangeObject�̔z��
class PHChangeObjects:public std::vector< UTRef<PHChangeObject> >{
};

/** �`�F���W�I�u�W�F�N�g�R���e�i�DBehaviorEngine�̈�� */
class PHChangeObjectContainer:public SGBehaviorEngine{
	SGOBJECTDEF(PHChangeObjectContainer);
	public:
		PHChangeObjects cos;
		/// 
		bool AddChildObject(SGObject* o, SGScene* s);
		///
		int GetPriority() const {return SGBP_CHANGEOBJECT;}
		virtual void Step(SGScene* s);
		virtual void Loaded(SGScene* scene);
		virtual void Clear(SGScene* s){ cos.clear(); }
		virtual size_t NChildObjects(){ return cos.size(); }
		virtual SGObject* ChildObject(size_t i){ return cos[i]; }
};

/** �w��t���[���ւ̏Փ˂̗L���ŁC����ւ��������s�������f */
class PHChangeObjectCollision:public PHChangeObject, CDCollisionListener {
	SGOBJECTDEF(PHChangeObjectCollision);

public:
	UTRef<SGFrame> detectorFrame;	///<	���o�p�t���[��
	bool bCollision;				///<	�ڐG�t���O
protected:
	int listenerPos;				///<	���X�i�̈ʒu

public:
	///
	bool AddChildObject(SGObject* o, SGScene* s);
	/// �ڐG����CDCollisionEngine::Step()����Ă΂��R�[���o�b�N�D�ڐG�t���O��TRUE�ɂ���
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	/// �N���A
	virtual void Clear();
	/// �ڐG�t���O�̌��ʂ�Ԃ��C���̃X�e�b�v�ɔ����ăt���O��FALSE�ɏ���������
	virtual bool IsChange();
	/// ���[�h�������̏����CCDCollisionEngine �ɃR�[���o�b�N��o�^����DCDCollisionEngine ���Ȃ���΍��D
	virtual void Loaded(SGScene* scene);
};

/** Solid�̎p���Ɣ�r�p�p���s����ׂāC����ւ��������s�������f */
class PHChangeObjectOrientation:public PHChangeObject {
	SGOBJECTDEF(PHChangeObjectOrientation);

public:
	Matrix3d comparativeOrientation;	///<	��r�p�p���s��
	UTRef<SGFrame> comparativeFrame;	///<	��r�p�t���[��(�f�[�^�t�@�C���ɋL�q����Ă���Ƃ������g��)
	Vec3d selectedAxis;					///<	��r���鎲�̑I��(x,y,z)�@���̎���Solid�̍��W�n
	Vec3d targetedInnerProduct;			///<	�ڕW���ϒl(x�̓���,y�̓���,����)
	Vec3d accuracy;						///<	�ڕW���ϒl�̐��x
	public:
	///
	bool AddChildObject(SGObject* o, SGScene* s);
	/// �N���A
	virtual void Clear();
	/// ����ւ����s�����ǂ����̔���
	virtual bool IsChange();
};

/** �f�[�^�̃��[�h�E�Z�[�u�p�̍\���� */
struct PHChangeObjectOrientationInfo{
	Vec3f selectedAxis;
	Matrix3f comparativeOrientation;
	Vec3f targetedInnerProduct;
	Vec3f accuracy;
};

};
#endif
