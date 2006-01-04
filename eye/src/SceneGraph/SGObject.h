#ifndef SG_OBJECT_H
#define SG_OBJECT_H

#include <Base/BaseUtility.h>
#include <Base/BaseTypeInfo.h>
#include <Base/Affine.h>
#include <FileIO/FIDocNode.h>
#include <WinBasis/WBWin.h>


namespace Spr {;

///	SGObject ���p�������N���X�̓����o�Ƃ��Ă��̃}�N�������D
#define SGOBJECTDEF(cls)				DEF_UTTYPEINFODEF(cls)
#define SGOBJECTDEFABST(cls)			DEF_UTTYPEINFOABSTDEF(cls)
#define SGOBJECTIMPBASE(cls)			DEF_UTTYPEINFO(cls)
#define SGOBJECTIMPBASEABST(cls)		DEF_UTTYPEINFOABST(cls)
#define SGOBJECTIMP(cls, base)			DEF_UTTYPEINFO1(cls, base)
#define SGOBJECTIMP2(cls, b1, b2)		DEF_UTTYPEINFO2(cls, b1, b2)
#define SGOBJECTIMP3(cls, b1, b2, b3)	DEF_UTTYPEINFO3(cls, b1, b2, b3)
#define SGOBJECTIMPABST(cls, base)		DEF_UTTYPEINFOABST1(cls, base)
#define SGOBJECTIMPABST2(cls, b1, b2)	DEF_UTTYPEINFOABST2(cls, b1,b2)
#define SGOBJECTIMPABST3(cls, b1, b2, b3)	DEF_UTTYPEINFOABST3(cls, b1,b2,b3)

class SGScene;

/**	�V�[���O���t�̃I�u�W�F�N�g�^�D
	�N���X����Ԃ��֐������D	*/
class SPR_DLL SGObject:public UTRefCount, UTTypeInfoBase{
	UTString name;			///<	���O
	UTString nameSpace;		///<	���O��ԁ��t�@�C����
protected:
	friend class SGObjectNames;
public:
	///	�N���X���̎擾�Ȃǂ̊�{�@�\�̎���
	SGOBJECTDEF(SGObject);
	///	���̃I�u�W�F�N�g�̌��ƂȂ����h�L�������g
	UTRef<FIDocNodeBase> doc;
	///	���[�U�����R�Ɏg����|�C���^
	void* userData;

	///
	SGObject(): userData(NULL) {}
	///	�f�X�g���N�^��virtual��
	virtual ~SGObject();

	///	���O�̎擾
	const char* GetName() const { return name.c_str(); }
	///	���O��Ԃ̎擾
	const char* GetNameSpace() const { return nameSpace.c_str(); }
	///	���O�̐ݒ�
	void SetName(const char* n, SGScene* s);
	///	�֘A�t����ꂽ�h�L�������g�m�[�h���J��
	virtual void ReleaseDoc(){ doc = NULL; }

	///	�h�L�������g����̃��[�h���C�Q�Ƃ������������ƂŌĂяo�����D
	virtual void Loaded(SGScene* scene){}
	///	�f�o�b�O�p�̕\��
	virtual void Print(std::ostream& os) const;

	///	���L���Ă���I�u�W�F�N�g�̐�
	virtual size_t NChildObjects(){ return 0; }
	///	���L���Ă���I�u�W�F�N�g
	virtual SGObject* ChildObject(size_t i){ return NULL; }
	///	�Q�Ƃ��Ă���I�u�W�F�N�g�̐�
	virtual size_t NReferenceObjects(){ return 0; }
	///	�Q�Ƃ��Ă���I�u�W�F�N�g
	virtual SGObject* ReferenceObject(size_t i){ return NULL; };
	///	�q�I�u�W�F�N�g�̒ǉ�(���L�E�Q�Ƃ���Ȃ�)
	virtual bool AddChildObject(SGObject* o, SGScene* s){ return false; }
	///	�q�I�u�W�F�N�g�̍폜
	virtual bool DelChildObject(SGObject* o, SGScene* s){ return false; }
	///	�q�ɂȂ肦��I�u�W�F�N�g�̌^���̔z��
	virtual const UTTypeInfo** ChildCandidates();
};

///	SGObject�̃|�C���^�̔z��
class SGObjects:public UTStack< UTRef<SGObject> >{
public:
	///	T�̔h���N���X���X�^�b�N��Top������T���D
	template <class T> bool Find(T*& t){
		for(iterator it = end(); it != begin();){
			--it;
			T* tmp = DCAST(T, *it);
			if (tmp){
				t = tmp;
				return true;
			}
		}
		return false;
	}
};

}
#endif SG_OBJECT_H
