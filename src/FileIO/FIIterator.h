#ifndef FIITERATOR_H
#define FIITERATOR_H
#include <Base/BaseDebug.h>
#include <string>
#include <vector>

namespace Spr{;

///	FileIO�Ŏg��������^
typedef std::string FIString;
class FIDocNodeBase;
/**	���ۃc���[(FIDocNodeBase)�̂��߂̃C�^���[�^�̎����̊�{�N���X	*/
class FIIteratorImpBase{
public:
	///	�����̍쐬
	virtual FIIteratorImpBase* Clone() const=0;
	///	�C�^���[�^��i�߂�D
	virtual void Next()=0;
	///	��r
	virtual bool Equal(const FIIteratorImpBase* imp) const =0;

	//@{	���g�ւ̃A�N�Z�X
	///	id���擾
	virtual FIString GetId() const = 0;
	///	�f�[�^�����̌^���擾
	virtual FIString GetType() const =0;
	///	�f�[�^������1�v�f�̃T�C�Y���擾
	virtual size_t GetElementSize() const =0;
	///	�f�[�^�����̗v�f�����擾
	virtual size_t GetNElement() const =0;
	///	�f�[�^������ݒ肷��Dsz�ɂ́C���ۂɃZ�b�g�����T�C�Y���Ԃ�D
	virtual bool SetDataImp(const void* data, size_t& sz, size_t esz, FIString id, FIString type)=0;
	///	�f�[�^�������擾����Dsz�ɂ́C���ۂɓǂݏo�����T�C�Y���Ԃ�D
	virtual bool GetDataImp(void* data, size_t& sz)=0;
	///	�m�[�h��ݒ肷��
	virtual bool SetNode(FIDocNodeBase* node)=0;
	///	�m�[�h���擾����
	virtual FIDocNodeBase* GetNode()=0;
	//@}
};

/**	���ۃc���[(FIDocNodeBase)�̂��߂̃C�^���[�^
	�m�[�h�́C�����̑��������D�����̓L�[�ƒl����Ȃ�D
	�l�̓f�[�^���m�[�h�̔z������D
	�C�^���[�^�͔z����P�v�f���i�ށD
*/
class FIIterator{
public:
	///	����
	FIIteratorImpBase* imp;
	///
	FIIterator(FIIteratorImpBase* i){ imp=i; }
	FIIterator(){ imp=NULL; }
	~FIIterator(){ delete imp; }
	FIIterator(const FIIterator& i){ imp=i.imp->Clone(); }
	FIIterator& operator =(const FIIterator& i){
		delete imp;
		imp=i.imp->Clone();
		return *this;
	}
	///@name �|�C���^���Z
	//@{
	///
	FIIterator operator ++(){
		FIIterator rv(imp->Clone());
		imp->Next();
		return rv;
	}
	///
	FIIterator& operator ++(int){
		imp->Next();
		return *this;
	}
	///
	FIIterator operator +(int n){
		assert(n >= 0);
		FIIterator rv(imp->Clone());
		for(int i=0; i<n; ++i) imp->Next();
		return rv;
	}
	///
	bool operator == (const FIIterator& it) const {
		return imp->Equal(it.imp);
	}
	///
	bool operator != (const FIIterator& it) const {
		return !imp->Equal(it.imp);
	}
	//@}

	//@{	���g�ւ̃A�N�Z�X
	///	������id���擾
	FIString GetId() const { return imp->GetId(); }
	///	�f�[�^�����̌^���擾
	FIString GetType() const { return imp->GetType(); }
	///	�f�[�^������1�v�f�̃T�C�Y���擾
	size_t GetElementSize() const { return imp->GetElementSize(); }
	///	�f�[�^�����̗v�f�����擾
	size_t GetNElement() const { return imp->GetNElement(); }
	///	�f�[�^������ݒ肷��Dsz�ɂ́C���ۂɃZ�b�g�����T�C�Y���Ԃ�D
	bool SetDataImp(const void* data, size_t& sz, size_t esz, FIString id, FIString type){ return imp->SetDataImp(data, sz, esz, id, type); }
	bool SetDataImp(const void* data, size_t& sz){ return imp->SetDataImp(data, sz, 0, "", ""); }
	template <class T>
	bool SetData(const T& t, size_t esz, FIString id, FIString type){ return imp->SetDataImp(&t, sizeof(T), esz, id, type); }
	template <class T>
	bool SetData(const T& t){ return imp->SetDataImp(&t, sizeof(T), 0, "", ""); }
	///	�f�[�^�������擾����Dsz�ɂ́C���ۂɓǂݏo�����T�C�Y���Ԃ�D
	bool GetDataImp(void* data, size_t& sz) const { return imp->GetDataImp(data, sz); }
	template <class T>
	bool GetData(T& t){
		unsigned sz = sizeof(T);
		bool rv = imp->GetDataImp(&t, sz);
		assert(sz == sizeof(T));
		return rv;
	}
	///	�m�[�h��ݒ肷��
	bool SetNode(FIDocNodeBase* node){ return imp->SetNode(node); }
	///	�m�[�h���擾����
	FIDocNodeBase* GetNode(){ return imp->GetNode(); }
	const FIDocNodeBase* GetNode() const{ return ((FIIteratorImpBase*)imp)->GetNode(); }
};


#if 0
/**	���ۃc���[�̂��߂̃C�^���[�^	
	�������R�s�[�����ɎQ�Ƃ���D	*/
struct FIRefIterator: public FIIterator{
	FIRefIterator(FIIteratorImpBase* i){ imp=i; }
	~FIRefIterator(){ imp=NULL; }
	FIRefIterator(const FIIterator& i){ imp=i.imp; }
	FIRefIterator& operator =(const FIIterator& i){
		imp=i.imp;
		return *this;
	}
};

/**	�m�[�h�̑����C�^���[�^	*/
template <class NODE>
class FIAttrIteratorImp:public FIIteratorImpBase{
public:
	typedef NODE::FIAttrIterator pos;
	void Next(){ ++pos; }
};

/**	�l�������������񂷂�C�^���[�^�̎���	*/
template <class NODE>
class FIValueIteratorImp:public FIIteratorImpBase{
public:
	struct FIStackFrame{
		NODE* node;
		NODE::FIAttrIterator pos;
		FIStackFrame(NODE* n):node(n){ pos = node->Begin(); }
	};
	typedef std::vector<FIStackFrame> FINodeStack;
protected:
	FINodeStack nodeStack;
public:
	virtual void Next(){
		++ nodeStack.back().pos;
		while(1){
			if (nodeStack.back().pos == nodeStack.back().node->End()){
				if (nodeStack->size()==0) return;	//	End() ���w���D
				nodeStack->pop_back();
				++ nodeStack.back().pos;
			}
			NODE* attrNode = nodeStack.back().node->GetAttrNode(FIRefIterator(this));
			if (attrNode){
				nodeStack.push_back(FIStackFrame(attrNode));
			}else{
				return;	//	�l�����������w��
			}
		}
	}
};
#endif

}	//	namespace Spr
#endif
