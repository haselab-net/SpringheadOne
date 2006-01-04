#ifndef D3DOCNODE_H
#define D3DOCNODE_H

#include <FileIO/FIDocScene.h>
#include <WinBasis/WBUtility.h>
#include <WinDx/D3D/WXD3D.h>
#include <dxfile.h>

namespace Spr {;
/**	DirectX�t�@�C���� Document Object�D*/
class SPR_DLL D3DocNode:public FIDocNodeBase, public UTTreeNode<D3DocNode>{
protected:
	UTString typeName;
	mutable UTString name;
public:
	typedef UTTreeNode<D3DocNode> Tree;

	std::vector<char> data;
	
	///	DirectXFileData
	WXINTF(DirectXFileData) xfileLoad;
	WXINTF(DirectXFileData) xfileSave;
	WXINTF(DirectXFileDataReference) xfileRef;
	WXINTF(DirectXFileBinary) xfileBin;

	//	�c���[�̑���
	virtual FIDocNodeBase* GetParent();
	virtual int NChildren() const;
	virtual FIDocNodeBase* Child(int i);
	virtual void AddChild(FIDocNodeBase*);
	virtual void SetParent(FIDocNodeBase*);
	virtual void ClearChildren();
	virtual FIDocNodeBase* FindChild(UTString type);
	
	virtual void SetType(UTString t);
	//@name	�����̑���
	//@{	
	virtual void AddDataImp(const void* data, size_t sz, FIString key, FIString type);
	virtual void AddNode(FIDocNodeBase* node, FIString key);
	virtual FIDocNodeBase* SetNewNode(FIString id, FIString type);
	virtual FIIterator FirstAttr() const ;
	virtual FIIterator LastAttr() const ;
	virtual FIIterator FindAttr(FIString key, size_t pos=0) const;
	virtual bool IsLast(const FIIterator& it) const ;
	//@}
	
	
	///	���[�h�p�̃R���X�g���N�^
	D3DocNode(IDirectXFileData* x, IDirectXFileDataReference* xRef):xfileLoad(x), xfileRef(xRef){
		if (xfileRef) typeName = "REF";
	}
	D3DocNode(IDirectXFileBinary* x):xfileBin(x), typeName("Binary"){}
	///	�Z�[�u�p�̃R���X�g���N�^
	D3DocNode(FIString t):typeName(t){}
	///	
	///	���̃m�[�h��ID�̎擾
	virtual const GUID* GetId();
	///	���̃m�[�h�̖��O�̎擾
	virtual UTString GetName() const;

	///	���̃m�[�h�̌^��ID�̎擾
	virtual const GUID* GetTypeId();
	///	���̃m�[�h�̌^���̎擾
	UTString GetType() const{ return typeName; }
	///	�^�̏��̎擾
	virtual FITypeDesc* GetTypeDesc();
	///	���̃m�[�h�̖��O�̐ݒ�
	void SetName(UTString n);
};

///	D3D�p�̃Z�[�o�[
class SPR_DLL D3SaveScene:public FISaveScene{
public:
	virtual UTRef<FIDocNodeBase> CreateDocNodeImp(UTString type){
		return new D3DocNode(type);
	}
};
void RegisterD3DLoader(FILoadScene* l);
void RegisterD3DSaver(FISaveScene* l);


}

#endif
