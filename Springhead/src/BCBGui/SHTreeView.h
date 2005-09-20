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
	///	このノードだけを更新する．
	void __fastcall UpdateSelf();
	///	このノードと子ノードを更新する．孫ノードは作らない(消える)．
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
    /// 対応するコンポーネントを返す．ない場合は新しいコンポーネントを作る．
    TSHComponent* GetComponent(TSHScene* scene, TComponent* owner, Spr::SGObject* obj);
};

//---------------------------------------------------------------------------
class PACKAGE TSHTreeView: public TCustomTreeView{
	friend TSHTreeNode;
private:
	///	デザイナー(BCB.exeのGUIのインタフェース)
	_di_IDesigner designer;
	///	対象シーン
	TSHScene* scene;
	/**	コンポーネントを持たないシーングラフのオブジェクトを
		プロパティエディタに表示するために使う一時コンポーネント*/
    TSHComponentPool componentPool;

	TSHTreeNode* __fastcall GetSelection(){ return (TSHTreeNode*)TCustomTreeView::Selected; };
	TSHTreeNode* __fastcall GetSelections(int i){ return (TSHTreeNode*)TCustomTreeView::Selections[i]; };
	TSHComponent* __fastcall GetSelectedComponent();
	void __fastcall SetSelection(TSHTreeNode* n){TCustomTreeView::Selected=n;}
	_di_IDesigner __fastcall GetDesigner();
	void __fastcall SetScene(TSHScene* s);

protected:
	///	ノード作成関数
	virtual TTreeNode* __fastcall CreateNode(void){
		return new TSHTreeNode(Items);
	}
    /// ノードの追加
    void __fastcall AddChild(TSHTreeNode* parent, Spr::SGObject* obj);

    /// 展開前に呼ばれる．このときに子ノードを作る．
//    DYNAMIC bool __fastcall CanExpand(TTreeNode* Node);
    /// 折りたたみ時に呼ばれる．このときに子ノードを更新する．
    DYNAMIC bool __fastcall CanCollapse(TTreeNode* Node);
    /// 編集後に呼ばれる．SGObjectの名前を更新する．
    DYNAMIC void __fastcall Edit(const tagTVITEMA &Item);
    /// 型名を後ろに表示する．
	virtual bool __fastcall CustomDrawItem(TTreeNode* Node, TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages);
    /// カスタム表示イベントを要求する．
	virtual bool __fastcall IsCustomDrawn(TCustomDrawTarget Target, TCustomDrawStage Stage);
    /// 削除前に呼ばれる．objectをこのノードからはずす．
    DYNAMIC void __fastcall Delete(TTreeNode* Node);
public:
	///	コンストラクタ
	__fastcall TSHTreeView(TComponent* Owner);
	///	SGObjectに対応するコンポーネントをフォーム上から探し出す．
	TSHComponent* FindComponent(Spr::SGObject* obj);
	/**	コンポーネントを作成する．TSHScene の CreateObject と違って，
		ヘッダファイルに宣言を加える．	*/
	TSHComponent* __fastcall CreateComponent();
	TSHComponent* CreateComponent(Spr::SGObject* obj);
	///	SGObjectに対応するコンポーネント(ない場合は仮のコンポーネント)を返す．
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
