#include "ImpD3D.h"
#pragma hdrstop

namespace Spr{;

//----------------------------------------------------------------------------
//	D3DocNode
FIDocNodeBase* D3DocNode::GetParent(){
	return Tree::GetParent();
}
int D3DocNode::NChildren() const{
	return ((D3DocNode*)this)->Tree::Children().size();
}
FIDocNodeBase* D3DocNode::Child(int i){
	return Tree::Children()[i];
}
void D3DocNode::AddChild(FIDocNodeBase* d){
	Tree::AddChild((D3DocNode*)d);
}
void D3DocNode::SetParent(FIDocNodeBase* p){
	Tree::SetParent((D3DocNode*)p);
}
void D3DocNode::ClearChildren(){
	Tree::ClearChildren();
}
FIDocNodeBase* D3DocNode::FindChild(UTString type){
	for(int i=0; i<NChildren(); ++i){
		if (type.compare(Child(i)->GetType()) == 0){
			return Child(i);
		}
	}
	return NULL;
}
FITypeDesc* D3DocNode::GetTypeDesc(){
	return NULL;
}
void D3DocNode::SetType(UTString t){
	typeName = t;
}

const GUID* D3DocNode::GetId(){
	if (!xfileLoad) return NULL;
	static GUID guid;
	xfileLoad->GetId(&guid);
	return &guid;
}
UTString D3DocNode::GetName() const{
	IDirectXFileObject* x = NULL;
	if (xfileLoad){
		x = xfileLoad.Intf();
	}else if (xfileBin){
		x = xfileBin.Intf();
	}else if (xfileRef){
		x = xfileRef;
		xfileRef->Resolve((IDirectXFileData**)&x);
	}
	if (x && name.length()==0){
		DWORD len=0;
		WXCHECK(x->GetName(NULL, &len));
		if (len > 0){
			char* buf = new char [len];
			WXCHECK(x->GetName(buf, &len));
			if (buf[len-1] == '\0') len--;
			name = UTString(buf, len);
			delete buf;
		}else{
     	name="";
		}
	}
	return name;
}
void D3DocNode::SetName(UTString n){
	name=n;
}
class D3IteratorImp: public FIIteratorImpBase{
public:
	long pos;
	DWORD len;
	char* ptr;
	D3DocNode* node;
	D3IteratorImp(){
		pos = len = 0;
		ptr = NULL;
		node = NULL;
	}
	D3IteratorImp(D3DocNode* n, int po){
		pos = po;
		node = n;
		if (node->xfileLoad){
			node->xfileLoad->GetData(NULL, &len, (void**)&ptr);
		}else{
			len=0;
			ptr=NULL;
		}
	}
	virtual FIIteratorImpBase* Clone() const {
		return new D3IteratorImp(*this);
	}
	virtual void Next(){
		assert(0);
		exit(-1);
	}
	bool Equal(const FIIteratorImpBase* bBase) const {
		const D3IteratorImp* a = this;
		const D3IteratorImp* b = (D3IteratorImp*) bBase;
		return a->ptr == b->ptr && a->len== b->len && a->pos == b->pos;			
	}
	//@{	���g�ւ̃A�N�Z�X
	///	id���擾
	virtual FIString GetId() const { return node->GetName(); }
	///	�f�[�^�����̌^���擾
	virtual FIString GetType() const { return node->GetType(); }
	///	�f�[�^������1�v�f�̃T�C�Y���擾
	virtual size_t GetElementSize() const { assert(0); return 0; }
	///	�f�[�^�����̗v�f�����擾
	virtual size_t GetNElement() const { assert(0); return 0; }
	///	�f�[�^������ݒ肷��Dsz�ɂ́C���ۂɃZ�b�g�����T�C�Y���Ԃ�D
	virtual bool SetDataImp(const void* src, size_t& sz, size_t esz=0, FIString id="", FIString type=""){
		if (esz){
			int n = sz / esz;
			sz = esz * n;
		}
		if (node->data.size() < unsigned(pos+sz)) node->data.resize(pos+sz);
		memcpy(&*(node->data.begin()+pos), src, sz);
		pos += sz;
		return true;
	}
	///	�f�[�^�������擾����Dsz�ɂ́C���ۂɓǂݏo�����T�C�Y���Ԃ�D
	virtual bool GetDataImp(void* dest, size_t& sz){
		if (unsigned(pos + sz) > len) return false;
		memcpy(dest, ptr + pos, sz);
		pos += sz;
		return true;
	}
	///	�m�[�h��ݒ肷��
	virtual bool SetNode(FIDocNodeBase* node){
		assert(0);	//	�܂����C���v���D�ł��ł���̂ł��\��D
		return false;
	}
	///	�m�[�h���擾����
	virtual FIDocNodeBase* GetNode(){
		return NULL;
	}
	//@}
};
FIIterator D3DocNode::FirstAttr() const {
	D3IteratorImp* rv = new D3IteratorImp;
	if (xfileLoad){
		xfileLoad->GetData(NULL, &rv->len, (void**)&rv->ptr);
	}else{
		rv->ptr=0;
		rv->len=0;
	}
	rv->pos=0;
	rv->node = (D3DocNode*)this;
	return FIIterator(rv);
}
FIIterator D3DocNode::LastAttr() const {
	D3IteratorImp* rv = new D3IteratorImp;
	if (xfileLoad){
		xfileLoad->GetData(NULL, &rv->len, (void**)&rv->ptr);
	}else{
		rv->ptr=0;
		rv->len=0;
	}
	rv->pos=rv->len;
	return FIIterator(rv);
}
FIIterator D3DocNode::FindAttr(FIString key, size_t pos) const {
	assert(pos==0);
	D3IteratorImp* rv = new D3IteratorImp;
	if (xfileLoad){
		xfileLoad->GetData(key.c_str(), &rv->len, (void**)&rv->ptr);
	}else{
		assert(0);	//	typedesc�����Ȃ��Əꏊ���킩��Ȃ��D
	}
	return FIIterator(rv);
}
bool D3DocNode::IsLast(const FIIterator& it) const {
	D3IteratorImp* imp = (D3IteratorImp*)it.imp;
	return imp->pos >= (int)imp->len;
}
FIDocNodeBase* D3DocNode::SetNewNode(FIString id, FIString type){
	return NULL;
}
void D3DocNode::AddNode(FIDocNodeBase* node, FIString key){
	assert(0);	//	����͖����ł��D�^�����܂��ĂȂ��m�[�h���Ȃ�����D
}
void D3DocNode::AddDataImp(const void* data, size_t sz, FIString key, FIString type){
	assert(0);	//	����������ł��D
}



const GUID* D3DocNode::GetTypeId(){
	const GUID* rv=NULL;
	if(xfileLoad) xfileLoad->GetType(&rv);
	return rv;
}


}

