#ifndef _FIDOCONVERTER
#define _FIDOCONVERTER
#include "FIDocNode.h"
#include "FITypeDesc.h"
#include <set>

/**	@file FIDocConverter.h �h�L�������g��ʂ̃h�L�������g�ɕϊ�����	*/
namespace Spr{;

class FIDocConverter;

/**	�ϊ��̃e���v���[�g	*/
class FIDocConvBase: public UTRefCount{
public:
	//@name �m�[�h�E�t�B�[���h�̃}�b�`���O�D
	//@{
	///	�m�[�h�̌^��
	virtual UTString GetType() const = 0;
	//@}

	///	�O�����ϊ�����
	virtual void Pre(FIDocConverter* ctx){}
	///	�㏇���ϊ�����
	virtual void Post(FIDocConverter* ctx){}
};
inline bool operator < (const FIDocConvBase& d1, const FIDocConvBase& d2){
	return d1.GetType().compare(d2.GetType()) < 0;
}

/**	�����̈ړ�	*/
class FIDocConvMove: public FIDocConvBase{
public:
	///	�ړ���̃t�B�[���h��
	FIString fieldTo;
	///	�O�����ϊ�����
	virtual void Pre(FIDocConverter* ctx);
};

/**	�h�L�������g�̃R���o�[�^	*/
class FIDocConverter{
protected:
	UTRef<FIDocNodeBase> from, to, root;
	typedef std::multiset< UTRef<FIDocConvBase> , UTContentsLess< UTRef<FIDocConvBase> > > Convs;
	Convs convs;
	UTRef<FITypeDescDb> typeDb;
public:
	///	�^�C�vDB�̎擾
	FITypeDescDb* GetTypeDb(){ return typeDb; }
	///	�^�C�vDB�̐ݒ�	
	void SetTypeDb(FITypeDescDb* t) { typeDb = t; }
	///	�ϊ����̃��[�g�m�[�h��n���C�ϊ����s���D
	void Convert(FIDocNodeBase* r);
	///	�ϊ��t�B���^�̒ǉ�
	void Register(FIDocConvBase* f);
public:
	FIDocNodeBase* GetRoot(){ return root; }
	///	���ݕϊ����̕ϊ����m�[�h
	FIDocNodeBase* From();
	///	���ݕϊ����̕ϊ���m�[�h
	FIDocNodeBase* To();
	///	���ݕϊ����̕ϊ���m�[�h�̈ړ�
	void ChangeTo(FIString t);
	///	���ݕϊ����̕ϊ���m�[�h�̈ړ�
	void UpTo();
	///	�m�[�h�̍쐬
	virtual FIDocNodeBase* CreateNode(FIString t);
};






}	//	namespace Spr
#endif
