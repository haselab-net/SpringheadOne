#ifndef FIDOCNODE_H
#define FIDOCNODE_H

#include "FIIterator.h"
#include "FITypeDesc.h"
#include <Base/BaseUtility.h>
#include <list>
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




/*	DocNode�̐݌v�ɂ���
	D3D�̏ꍇ�C�f�[�^�t�@�C���DVRML�̓X�N���v�g�D
	���Ƃ��΁CVRML�̏ꍇ�Corientation �� ���x���o�Ă���悤�� Transform �m�[�h
	�����肦��D
	X�̏ꍇ�C���Ԃ��Œ�ŁC�f�[�^�����������Ă���D

	X��DocNode�́C�������O�̃t�B�[���h��1�������̂ŁCC++�̃N���X��1��1�ɑΉ�����D
	VRML��DocNode�͕������肦�āC���������蓖����D

	����IF�ɂ���̂͂悢�̂��H
	���悢


	�C�^���[�^�́C�m�[�h�����񂷂邯��ǁC�g�ݗ��Č^�͂ǂ����邩�H
	X�͑������g�ݗ��Č^�ɂȂ邱�Ƃ�����D
	VMRL�͒P���^�����D
	X����
		MeshFace{ int n; int vtx[]; }
		Mesh{ MeshFace face; MeshMaterialList{} }
	������D
	�����DocNode�ŏ����ɂ́H
		Mesh{
			face -> {
				int n;
				int vtx[];
			}
			children -> MeshMatrialList{
			}
		}


	�g�ݗ��Č^�̒������񂷂�Ȃ瑮���̒l�Ƃ��āC�m�[�h�^������ɂ��Ȃ���΂Ȃ�Ȃ��D
	�������m�[�h��
	�m�[�h��Children�ɓ���Ă���
	�Ƃ���H
*/

class FITypeDesc;
/**	�t�@�C�����o�̓m�[�h�D
	�V�[���O���t���t�@�C�����烍�[�h������C�t�@�C���ɃZ�[�u����ۂɁC
	�t�@�C���𒼐ڈ��킸�ɁC�h�L�������g�I�u�W�F�N�g����ăZ�[�u�E
	���[�h����ƁC�t�@�C���̈������h�L�������g�I�u�W�F�N�g�ɔC����
	���Ƃ��ł���̂ŁC�Z�[�u�E���[�h�̃R�[�h�����炷���Ƃ��ł���D
	���̃N���X�́C�h�L�������g�I�u�W�F�N�g���\������m�[�h�̊�{�N���X.

	�h�L�������g�m�[�h�ł́C�����l�Ƃ��āC�m�[�h����邱�Ƃ��o����悤�ɂ��Ă���D
	�q�m�[�h�Ƒ����l�̗����Ƀm�[�h�����邱�Ƃ����蓾��D*/
class SPR_DLL FIDocNodeBase:public UTRefCount{
public:
	friend class FISaveScene;
	///	���z�f�X�g���N�^��p��
	virtual ~FIDocNodeBase(){}

	//@name	�m�[�h�̊�{����
	//@{
	///	���̃m�[�h�̖��O�̎擾
	virtual UTString GetName() const =0;
	///	���̃m�[�h�̖��O�̐ݒ�
	virtual void SetName(UTString s)=0;
	/**	���̃m�[�h�̌^���̎擾.
		���t�@�����X�ɂ��āF
		���̃m�[�h�����̃m�[�h�ւ̃��t�@�����X�������ꍇ�C
		GetType() �� "REF" ��Ԃ��CGetName()�͎Q�Ɛ�̃m�[�h����Ԃ��D*/
	virtual UTString GetType() const =0;
	/**	���̃m�[�h�̌^���̐ݒ�.
		���t�@�����X�̏ꍇ�́C "REF"��ݒ肷��D*/
	virtual void SetType(UTString t){ assert(0); }
	///	�^�L�q�q�̐ݒ�
	virtual void SetTypeDesc(FITypeDesc* desc){ assert(0); }
	bool IsReference() const { return GetType().compare("REF") == 0; }
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
	//@}

	//@name	�����̑���
	//@{
	/**	�S������ݒ� */
	template <class T> bool SetWholeData(const T& t){ return SetWholeData(&t, sizeof(t)); }
	virtual bool SetWholeData(const void* data, size_t sz);
	///	�����S�̂��擾�D
	template <class T> bool GetWholeData(T& t){ return GetWholeData(&t, sizeof(t)); }
	virtual bool GetWholeData(void* data, size_t sz);	
	
	///	�w��̃f�[�^�����̐ݒ�
	template <class T> bool SetData(const T& t, FIString id, FIString type){ return FindAttr(id).SetData(t, type); }
	///	�w��̃f�[�^�����̐ݒ�(�z��̈ꊇ�ݒ��)
	bool SetData(const void* data, size_t& dataSize, size_t elementSize, FIString id, FIString type){ return FindAttr(id).SetDataImp(data, dataSize, elementSize, id, type); }
	///	�w��̃f�[�^�����̎擾
	template <class T> bool GetData(T& t, FIString id){ return FindAttr(id).GetData(t); }
	///	�w��̃f�[�^�����̎擾
	bool GetData(void* data, size_t sz, FIString id){ return FindAttr(id).GetDataImp(data, sz); }
	//@}

	//@name	��������̎���
	//@{
	///	���݂̑����ɐV�����m�[�h���Z�b�g����D
	virtual FIDocNodeBase* SetNewNode(FIString id, FIString type)=0;
	
	///	�擪�̑����l���w���C�^���[�^���擾
	virtual FIIterator FirstAttr() const =0;
	///	�Ō�̎��̑����l���w���C�^���[�^���擾
	virtual FIIterator LastAttr() const =0;
	///	�C�^���[�^���Ō�ɗ����� true
	virtual bool IsLast(const FIIterator& it) const { return it == LastAttr(); }
	///	�w��̃m�[�h���w���C�^���[�^���擾
	virtual FIIterator FindAttr(FIString id, size_t pos=0) const =0;
	//@}

	///	�m�[�h�̃c���[��\��
	virtual void Print(std::ostream& os, int indent=0) const;
	///	������\��
	virtual void PrintAttr(std::ostream& os, int indent=0) const;
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
public:
	struct TData{
		typedef std::vector<char> container;
		FIString type;							///<	�l�̌^
		size_t elementSize;						///<	�v�f�̃T�C�Y
		container data;							///<	�o�b�t�@
		TData(){ elementSize=0; }
		///	�f�[�^���Ō�ɒǉ�
		void PushBack(const void* d){
			data.resize(data.size()+elementSize);
			container::iterator it = data.end()-elementSize;
			memcpy(&*it, d, elementSize);
		}
		///	�Ō�̃f�[�^���폜
		void PopBack(){
			data.resize(data.size() - elementSize);
		}
		///	1�v�f�̃T�C�Y
		size_t ElementSize() const { return elementSize; }
		void ElementSize(size_t s){ elementSize = s; }
		///	�f�[�^��
		void Size(size_t n){ data.resize(n*elementSize); }
		size_t Size() const { return elementSize ? data.size()/elementSize : 0; }
		///	�S�f�[�^�̃o�C�g��
		size_t DataSize(){ return data.size(); }
		///	n�Ԗڂ̃f�[�^�ւ̃|�C���^
		char* Data(size_t i){ return &*data.begin() + i*elementSize; }
		///	�ŏ��̃f�[�^�ւ̃|�C���^
		char* Begin(){ return &*data.begin(); }
		///	�ŏ��̃f�[�^�ւ̃|�C���^
		const char* Begin() const { return &*data.begin(); }
		///	�ŏ��̃f�[�^�̎��ւ̃|�C���^
		char* End(){ return &*data.end(); }
		///	�ŏ��̃f�[�^�̎��ւ̃|�C���^
		const char* End() const { return &*data.end(); }
		///	�T�C�Y�̐ݒ�
		void Resize(size_t s){ data.resize(s*elementSize); }
	};
	///	�m�[�h���o�C�i���f�[�^�̔z��
	struct TValue{
		///	�m�[�h�l(�o�C�i���l������ꍇ�͋�)
		std::vector< UTRef<FIDocNode> > nodes;
		///	�o�C�i���f�[�^
		TData data;
		///	�m�[�h�܂��̓f�[�^�̐��D
		size_t Size() const {
			return nodes.size() ? nodes.size() : data.Size();
		}
	};
	struct TAttr: public TValue{
		UTString id;				///<	������
		TAttr(){}
		TAttr(FIString i):id(i){}
		bool operator < (const TAttr& b) const { return id.compare(b.id) < 0; }
		bool operator == (const TAttr& b) const { return id.compare(b.id)==0; }
	};
	class TAttrs:public std::vector<TAttr>{
	public:
		typedef std::vector<TAttr> base;
		TAttrs(){}
		TAttrs(size_t sz, const TAttr a):base(sz,a){}
		iterator Find(FIString id) const;
	};
	class TIteratorImp:public FIIteratorImpBase{
	public:
		TAttrs::iterator it;	//	�����ւ̃|�C���^
		size_t pos;				//	�������ł̈ʒu
		FIDocNode* node;

		TIteratorImp(FIDocNode* n, TAttrs::const_iterator i, size_t p):node(n), it((TAttrs::iterator&)i), pos(p){}
		virtual FIIteratorImpBase* Clone() const { return new TIteratorImp(*this); }
		virtual void Next(){
			if (pos < it->Size()) { ++pos; }
			else {
				if (Validate()) ++pos;
			}
		}
		bool Equal(const FIIteratorImpBase* bBase) const {
			const TIteratorImp* a = this;
			const TIteratorImp* b = (TIteratorImp*) bBase;
			return a->it == b->it && a->node == b->node && a->pos == b->pos;			
		}
		bool Validate() const;
//		bool ValidateNode();
//		bool ValidateData();
		//@{	���g�ւ̃A�N�Z�X
		///	id���擾
		virtual FIString GetId() const;
		///	�f�[�^�����̌^���擾
		virtual FIString GetType() const;
		///	�f�[�^������1�v�f�̃T�C�Y���擾
		virtual size_t GetElementSize() const;
		///	�f�[�^�����̗v�f�����擾
		virtual size_t GetNElement() const;
		///	�f�[�^������ݒ肷��Dsz�ɂ́C���ۂɃZ�b�g�����T�C�Y���Ԃ�D
		virtual bool SetDataImp(const void* data, size_t& sz, size_t esz, FIString id, FIString type);
		///	�f�[�^�������擾����Dsz�ɂ́C���ۂɓǂݏo�����T�C�Y���Ԃ�D
		virtual bool GetDataImp(void* data, size_t& sz);
		///	�m�[�h��ݒ肷��
		virtual bool SetNode(FIDocNodeBase* node);
		///	�m�[�h���擾����
		virtual FIDocNodeBase* GetNode();
		//@}
	};

	///	��{�N���X
	typedef UTTreeNode<FIDocNode> FITree;

public:
	TAttrs attrs;
	FIString name;
	FIString type;
public:
	///
	FIDocNode(){}
	///
	~FIDocNode(){}
	//@name	�m�[�h�̑���
	//@{
	///	���̃m�[�h�̖��O�̎擾
	virtual UTString GetName() const { return name; }
	///	���̃m�[�h�̖��O�̐ݒ�
	virtual void SetName(UTString s){ name = s; }
	///	���̃m�[�h�̌^���̂����Ă�
	virtual void SetType(FIString t);
	///	���̃m�[�h�̌^���̎擾
	virtual FIString GetType() const;
	//@}
	
	//@name	�؍\���̑���
	//@{
	///	�e�m�[�h
	virtual FIDocNodeBase* GetParent(){ return FITree::GetParent(); }
	///	�q�m�[�h�̐�
	virtual int NChildren() const { return (int)FITree::Children().size(); }
	///	�e�m�[�h�̐ݒ�
	virtual void SetParent(FIDocNodeBase* n){ FITree::SetParent((FIDocNode*)n); }
	///	�q�m�[�h�����ׂč폜
	virtual void ClearChildren(){ FITree::ClearChildren(); }
	///	�q�m�[�h��Ԃ�
	virtual FIDocNodeBase* Child(int i){ return FITree::Children()[i]; }
	//@}
	
	//@name	�����̑���
	//@{	
	virtual FIDocNodeBase* SetNewNode(FIString id, FIString type);
	virtual FIIterator FirstAttr() const;
	virtual FIIterator LastAttr() const;
	virtual FIIterator FindAttr(FIString id, size_t pos=0) const;
	virtual bool IsLast(const FIIterator& it) const;
	//@}
};
}
#endif
