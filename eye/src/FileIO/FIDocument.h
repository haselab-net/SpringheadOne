#ifndef FIDOCUMENT_H
#define FIDOCUMENT_H

#include "FITypeDesc.h"
#include <Base/BaseUtility.h>
#include <set>
#include <map>
#include <algorithm>

namespace Spr {;

///	������̌^
typedef std::string FIString;
///	����0�̕�����
extern FIString zeroLengthString;
///	������̔�r
struct FIStringLess{
	bool operator() (const FIString& s1, const FIString& s2) const { return s1.compare(s2) < 0;}
};
///	������̕\��
inline std::ostream& operator << (std::ostream& os, FIString s){
	os << s.c_str();
	return os;
}

/**	�h�L�������g�m�[�h�̓ǂݏo���E�������݈ʒu��ێ�����R���e�L�X�g	*/
class FIDocNodePos{
public:
	virtual ~FIDocNodePos(){}
};
/**	�t�@�C�����o�̓m�[�h�D
	�V�[���O���t���t�@�C�����烍�[�h������C�t�@�C���ɃZ�[�u����ۂɁC
	�t�@�C���𒼐ڈ��킸�ɁC�h�L�������g�I�u�W�F�N�g����ăZ�[�u�E
	���[�h����ƁC�t�@�C���̈������h�L�������g�I�u�W�F�N�g�ɔC����
	���Ƃ��ł���̂ŁC�Z�[�u�E���[�h�̃R�[�h�����炷���Ƃ��ł���D
	���̃N���X�́C�h�L�������g�I�u�W�F�N�g���\������m�[�h�̊�{�N���X.
*/
class SPR_DLL FIDocNodeBase:public UTRefCount{
public:
	friend class FISaveScene;
	///	���z�f�X�g���N�^��p��
	virtual ~FIDocNodeBase(){}
	//@name	�m�[�h�̑���
	//@{
	///	���̃m�[�h�̖��O�̎擾
	virtual UTString GetName()=0;
	///	���̃m�[�h�̖��O�̐ݒ�
	virtual void SetName(UTString s)=0;
	///	���̃m�[�h�̌^���̎擾
	virtual UTString GetType()=0;
	///	���̃m�[�h�̌^���̐ݒ�
	virtual void SetType(UTString t){}
	///	���̃m�[�h�̌^�f�[�^�̎擾
	virtual FITypeDesc* GetTypeDesc()=0;
	///	���̃m�[�h���V�[���O���t���̑��̃m�[�h�ւ̎Q�Ƃ��ǂ���
	virtual bool IsReference() = 0;
	//@}
	
	//@name	�؍\���̑���
	//@{
	///	�e�m�[�h
	virtual FIDocNodeBase* GetParent()=0;
	///	�q�m�[�h�̐�
	virtual int NChildren() const =0;
	///	�q�m�[�h
	virtual FIDocNodeBase* GetChild(int i){ return Child(i); }
	///	�q�m�[�h(Const��)
	virtual const FIDocNodeBase* GetChild(int i) const { return ((FIDocNodeBase*)this)->Child(i); }
	///	�q�m�[�h�̒ǉ�
	virtual void AddChild(FIDocNodeBase* n){ n->SetParent(this); }
	///	�e�m�[�h�̐ݒ�
	virtual void SetParent(FIDocNodeBase*)=0;
	///	�q�m�[�h�����ׂč폜
	virtual void ClearChildren()=0;
	///	�q�m�[�h���^���Ō���
	virtual FIDocNodeBase* FindChild(UTString type)=0;
	//@}

	///	�f�[�^��ݒ�
	virtual void SetDataImp(const void* data, size_t sz);
	///	�f�[�^���擾
	virtual void GetDataImp(void* data, int sz, FIString id);
	virtual void GetDataImp(void* data, int sz){ GetDataImp(data, sz,""); }
	///	�f�[�^��ݒ�(�e���v���[�g��)
	template <class T> void SetData(const T& t){ SetDataImp(&t, sizeof(t)); }
	///	�f�[�^���擾(�e���v���[�g��)
	template <class T> void GetData(T& t){ GetDataImp(&t, sizeof(t), ""); }
	template <class T> void GetData(T& t, UTString id){ GetDataImp(&t, sizeof(t), id); }
	///	�ǂݏo���R���e�L�X�g�̍쐬
protected:
	virtual FIDocNodePos* ReadContextImp(FIString id)=0;
public:
	FIDocNodePos* ReadContext(){ return ReadContextImp(""); }
	FIDocNodePos* ReadContext(FIString id){ return ReadContextImp(id); }
	///	�������݃R���e�L�X�g�̍쐬
protected:
	virtual FIDocNodePos* WriteContextImp(FIString id){ return ReadContext(id); }
public:
	FIDocNodePos* WriteContext(FIString id){ return WriteContextImp(id); }
	FIDocNodePos* WriteContext(){ return WriteContextImp(""); }
	///	�R���e�L�X�g�̊J��
	virtual void ReleaseContext(FIDocNodePos* c){ delete c; }
	///	���̃t�B�[���h��ǂݏo���֐�
	virtual bool Read(FIDocNodePos* ctx, char* ptr, int sz)=0;
	///	���̃t�B�[���h�̃f�[�^����������	
	virtual void Write(FIDocNodePos* ctx, const char* ptr, int sz, FIString fn, FIString type)=0;
	///	�t�B�[���h�̃O���[�v�ɓ���		
	virtual void EnterGroup(FIString fn){}
	///	�O���[�v���甲����D				
	virtual void ExitGroup(){}
	
	///	�m�[�h�̃c���[��\��
	virtual void Print(std::ostream& os, int indent=0) const;
	///	�q�m�[�h�̃c���[��\��
	virtual void PrintChildren(std::ostream& os, int indent=0) const;
protected:
	///	�q�m�[�h��Ԃ��֐��̎���
	virtual FIDocNodeBase* Child(int i) = 0;
};

///	�h�L�������g�m�[�h�̊�{�N���X��vector
class SPR_DLL FIDocNodes:public UTStack< UTRef<FIDocNodeBase> >{
public:
	virtual void Print(std::ostream& os, int indent=0) const;
};

///	�h�L�������g�m�[�h
class SPR_DLL FIDocNode:public FIDocNodeBase, public UTTreeNode<FIDocNode>{
	UTString name;
	UTRef<FITypeDesc> typeDesc;
	bool bReference;
	std::vector<char> data;
public:
	typedef UTTreeNode<FIDocNode> Tree;
	///
	FIDocNode(): bReference(false){}
	///
	~FIDocNode(){}
	///	�^�̐ݒ�
	void SetType(FITypeDesc* t);
	void SetType(FIString s){ FIDocNodeBase::SetType(s); }
	//@name	�m�[�h�̑���
	//@{
	///	���̃m�[�h�̖��O�̎擾
	virtual UTString GetName(){ return name; }
	///	���̃m�[�h�̖��O�̐ݒ�
	virtual void SetName(UTString s){ name = s; }
	///	���̃m�[�h�̌^���̎擾
	virtual UTString GetType();
	///	���̃m�[�h�̌^�f�[�^�̎擾
	virtual FITypeDesc* GetTypeDesc(){ return typeDesc; }
	///	���̃m�[�h���V�[���O���t���̑��̃m�[�h�ւ̎Q�Ƃ��ǂ���
	virtual bool IsReference(){ return bReference; }
	//@}
	
	//@name	�؍\���̑���
	//@{
	///	�e�m�[�h
	virtual FIDocNodeBase* GetParent(){ return Tree::GetParent(); }
	///	�q�m�[�h�̐�
	virtual int NChildren() const { return (int)Tree::Children().size(); }
	///	�e�m�[�h�̐ݒ�
	virtual void SetParent(FIDocNodeBase* n){ Tree::SetParent((FIDocNode*)n); }
	///	�q�m�[�h�����ׂč폜
	virtual void ClearChildren(){ Tree::ClearChildren(); }
	///	�q�m�[�h���^���Ō���
	virtual FIDocNodeBase* FindChild(UTString type);
	//@}

	///	�ǂݏo���R���e�L�X�g�̍쐬
	virtual FIDocNodePos* ReadContextImp(FIString id);
	///	���̃t�B�[���h��ǂݏo���֐�
	virtual bool Read(FIDocNodePos* ctx, char* ptr, int sz);
	///	���̃t�B�[���h�̃f�[�^����������	
	virtual void Write(FIDocNodePos* ctxBase, const char* ptr, int sz, FIString fn, FIString type);

protected:
	///	�q�m�[�h��Ԃ��֐��̎���
	virtual FIDocNodeBase* Child(int i){ return Tree::Children()[i]; }
};

/**	�h�L�������g�ƃt�@�C���̕ϊ��̂��߂̃R���e�L�X�g�D	*/
class SPR_DLL FIFileDoc:public UTRefCount{
protected:
	///	�t�@�C����
	UTString fileName;
	///	�h�L�������g�m�[�h�̃X�^�b�N
	FIDocNodes docs;
public:
	///	�t�@�C������h�L�������g�I�u�W�F�N�g���쐬
	virtual bool Load(UTString filename)=0;
	///	�h�L�������g�I�u�W�F�N�g�ɕۑ�
	virtual bool Save(UTString filename, FIDocNodeBase* doc)=0;
	///	�h�L�������g�̃��[�g�m�[�h
	virtual FIDocNodes& GetRoots(){ return docs; }
	///	�t�@�C����
	UTString GetFileName(){ return fileName; }
};

}
#endif
