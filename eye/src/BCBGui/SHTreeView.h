//---------------------------------------------------------------------------

#ifndef TSHTreeViewH
#define TSHTreeViewH
#include "SHScene.h"
#include "SHFrame.h"
#include "SHSolid.h"
#include "SHPhysicalMaterial.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <DesignIntf.hpp>

//---------------------------------------------------------------------------
class __declspec(delphiclass) TSHTreeView;

class PACKAGE TSHTreeNode: public TTreeNode{
private:
	Spr::UTRef<Spr::SGObject> object;
	void __fastcall SetObject(Spr::SGObject* obj);
    Spr::SGObject* GetObject(){ return object; }

	TSHTreeView* __fastcall GetTreeView(){
		return (TSHTreeView*)TTreeNode::TreeView;
	}
	TSHTreeNode* __fastcall GetParent(){
		return (TSHTreeNode*) TTreeNode::Parent;
	}

public:
	__fastcall TSHTreeNode(TTreeNodes* AOwner):TTreeNode(AOwner){}
	///	���̃m�[�h�������X�V����D
	void __fastcall UpdateSelf();
	///	���̃m�[�h�Ǝq�m�[�h���X�V����D���m�[�h�͍��Ȃ�(������)�D
	void __fastcall Update();
	void __fastcall Import(const char* filename);
	void __fastcall Export(const char* filename);
	__property Spr::SGObject* Object={read=GetObject, write=SetObject};
	__property TSHTreeNode* Parent = {read=GetParent};
__published:
	__property TSHTreeView* TreeView = {read=GetTreeView};

};


TMetaClass* GetBCBClassID(Spr::UTTypeInfo* sprTypeInfo);

class PACKAGE TSHComponentPool{
protected:
    std::map<const Spr::UTTypeInfo*, TSHComponent*> objects;
public:
    /// �Ή�����R���|�[�l���g��Ԃ��D�Ȃ��ꍇ�͐V�����R���|�[�l���g�����D
    TSHComponent* GetComponent(TSHScene* scene, TComponent* owner, Spr::SGObject* obj);
};

//---------------------------------------------------------------------------
class PACKAGE TSHTreeView: public TCustomTreeView{
	friend TSHTreeNode;
private:
	///	�f�U�C�i�[(BCB.exe��GUI�̃C���^�t�F�[�X)
	_di_IDesigner designer;
	///	�ΏۃV�[��
	TSHScene* scene;
	/**	�R���|�[�l���g�������Ȃ��V�[���O���t�̃I�u�W�F�N�g��
		�v���p�e�B�G�f�B�^�ɕ\�����邽�߂Ɏg���ꎞ�R���|�[�l���g*/
    TSHComponentPool componentPool;

	TSHTreeNode* __fastcall GetSelection(){ return (TSHTreeNode*)TCustomTreeView::Selected; };
	TSHTreeNode* __fastcall GetSelections(int i){ return (TSHTreeNode*)TCustomTreeView::Selections[i]; };
	TSHComponent* __fastcall GetSelectedComponent();
	void __fastcall SetSelection(TSHTreeNode* n){TCustomTreeView::Selected=n;}
	_di_IDesigner __fastcall GetDesigner();
	void __fastcall SetScene(TSHScene* s);

protected:
	///	�m�[�h�쐬�֐�
	virtual TTreeNode* __fastcall CreateNode(void){
		return new TSHTreeNode(Items);
	}
    /// �m�[�h�̒ǉ�
    void __fastcall AddChild(TSHTreeNode* parent, Spr::SGObject* obj);

    /// �W�J�O�ɌĂ΂��D���̂Ƃ��Ɏq�m�[�h�����D
//    DYNAMIC bool __fastcall CanExpand(TTreeNode* Node);
    /// �܂肽���ݎ��ɌĂ΂��D���̂Ƃ��Ɏq�m�[�h���X�V����D
    DYNAMIC bool __fastcall CanCollapse(TTreeNode* Node);
    /// �ҏW��ɌĂ΂��DSGObject�̖��O���X�V����D
    DYNAMIC void __fastcall Edit(const tagTVITEMA &Item);
    /// �^�������ɕ\������D
	virtual bool __fastcall CustomDrawItem(TTreeNode* Node, TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages);
    /// �J�X�^���\���C�x���g��v������D
	virtual bool __fastcall IsCustomDrawn(TCustomDrawTarget Target, TCustomDrawStage Stage);
    /// �폜�O�ɌĂ΂��Dobject�����̃m�[�h����͂����D
    DYNAMIC void __fastcall Delete(TTreeNode* Node);
public:
	///	�R���X�g���N�^
	__fastcall TSHTreeView(TComponent* Owner);
	///	SGObject�ɑΉ�����R���|�[�l���g���t�H�[���ォ��T���o���D
	TSHComponent* FindComponent(Spr::SGObject* obj);
	/**	�R���|�[�l���g���쐬����DTSHScene �� CreateObject �ƈ���āC
		�w�b�_�t�@�C���ɐ錾��������D	*/
	TSHComponent* __fastcall CreateComponent();
	TSHComponent* CreateComponent(Spr::SGObject* obj);
	///	SGObject�ɑΉ�����R���|�[�l���g(�Ȃ��ꍇ�͉��̃R���|�[�l���g)��Ԃ��D
	TSHComponent* __fastcall GetComponent(Spr::SGObject* obj);
	
	__property TSHScene* Scene={read=scene, write=SetScene};
	__property _di_IDesigner Designer = {read=GetDesigner};
	__property TSHTreeNode* Selections[int Index] = {read=GetSelections};
	__property TSHTreeNode* Selected = {read=GetSelection, write=SetSelection};
	__property TSHComponent* SelectedComponent = {read=GetSelectedComponent};

	
__published:
	__property Align;
	__property BorderStyle;
	__property ChangeDelay;
	__property DockOrientation;
	__property DockSite;
	__property DragKind;
	__property DragMode;
	__property Floating;
    __property HideSelection;
	__property Items;
    __property Images;
	__property PopupMenu;

	__property OnKeyDown;
	__property OnKeyPress;
	__property OnKeyUp;
	__property OnClick;
	__property OnContextPopup;
	__property OnDblClick;
	__property OnDragDrop;
	__property OnDragOver;
	__property OnEndDock;
	__property OnEndDrag;
	__property OnMouseDown;
	__property OnMouseMove;
	__property OnMouseUp;
	__property OnStartDock;
	__property OnStartDrag;
	__property OnChange;
};
//---------------------------------------------------------------------------
#endif
