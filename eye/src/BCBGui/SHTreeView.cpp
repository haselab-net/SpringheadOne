#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <vcl.h>

#include <algorithm>
#include "SHTreeView.h"
#include "SHLight.h"
#include "SHScene.h"
#include "SHBox.h"
#include "SHCamera.h"
using namespace Spr;
using namespace std;
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSHComponent* TSHComponentPool::GetComponent(TSHScene* scene, TComponent* owner, Spr::SGObject* obj){
	std::map<const Spr::UTTypeInfo*, TSHComponent*>::iterator it = objects.find(obj->GetTypeInfo());
	if (it != objects.end()){
        it->second->Imp = obj;
        return it->second;
    }
	TSHComponent* component = scene->CreateComponent(owner, obj);
    if(component){
    	objects[obj->GetTypeInfo()] = component;
    }
   	return component;
}

//---------------------------------------------------------------------------
void __fastcall TSHTreeNode::UpdateSelf(){
	if (object) Text = object->GetName();
}
void __fastcall TSHTreeNode::Update(){
	DeleteChildren();
	UpdateSelf();
	for(unsigned i=0; i<object->NChildObjects(); ++i){
		TreeView->AddChild(this, object->ChildObject(i));
    }
}
void __fastcall TSHTreeNode::SetObject(Spr::SGObject* o){
	if (object==o) return;
	object=o;
	Update();
}
void __fastcall TSHTreeNode::Import(const char* filename){
	TreeView->scene->Import(object, filename);
	std::set<SGObject*> oldObjs;
	for(unsigned i=0; i<object->NChildObjects(); ++i){
		oldObjs.insert(object->ChildObject(i));
	}
	for(unsigned i=0; i<object->NChildObjects(); ++i){
		SGObject* co = object->ChildObject(i);
		if (oldObjs.end() == oldObjs.find(co)){
			TreeView->AddChild(this, co);
		}
	}
}
void __fastcall TSHTreeNode::Export(const char* filename){
	TreeView->scene->Export(Object, filename);
}

//---------------------------------------------------------------------------
TSHComponent* TSHTreeView::FindComponent(Spr::SGObject* obj){
	TForm* form = assert_cast<TForm*>(scene->Owner);
	for(int i=0; i<form->ComponentCount; ++i){
		TSHComponent* comp = dynamic_cast<TSHComponent*>(form->Components[i]);
		if (comp && comp->Imp == obj){
			return comp;
		}
	}
	return NULL;
}
void __fastcall TSHTreeView::AddChild(TSHTreeNode* parent, Spr::SGObject* obj){
	TTreeNodes* nodes = Items;
	TSHTreeNode* node = (TSHTreeNode*)nodes->AddChild(parent, NULL);
	node->Object = obj;
	node->UpdateSelf();
}
TSHComponent* TSHTreeView::CreateComponent(SGObject* obj){
	if (!obj) return NULL;
	TForm* form = assert_cast<TForm*>(scene->Owner);
	if (!Designer) return NULL;

	TMetaClass* cls = Scene->GetMetaClass(obj);
	assert(cls);
	AnsiString s= cls->ClassName();
	TSHComponent* comp = (TSHComponent*)Designer->CreateComponent(cls, form, 0, 0, 0, 0);
	comp->Scene = scene;
	comp->Imp = obj;
	return comp;
}
TSHComponent* __fastcall TSHTreeView::GetComponent(Spr::SGObject* obj){
	TSHComponent* rv = FindComponent(obj);
	if (!rv) rv = componentPool.GetComponent(Scene, this, obj);
	return rv;
}
TSHComponent* __fastcall TSHTreeView::GetSelectedComponent(){
	if (Selected) return GetComponent(Selected->Object);
	return NULL;
}

_di_IDesigner __fastcall TSHTreeView::GetDesigner(){
	if (scene){
		TForm* form = (TForm*)scene->Owner;
		if (form->Designer){
			form->Designer->Supports(designer);
		}
	}
	return designer;
}

__fastcall TSHTreeView::TSHTreeView(TComponent* Owner)
	:TCustomTreeView(Owner),designer(NULL),scene(NULL){
}
void __fastcall TSHTreeView::SetScene(TSHScene* s){
	if (scene == s) return;
	scene = s;
    Items->Clear();
    AddChild(NULL, scene->Imp);
}

bool __fastcall TSHTreeView::CustomDrawItem(TTreeNode* baseNode, TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages){
    TSHTreeNode* node = (TSHTreeNode*)baseNode;
	if (Stage == cdPostPaint){
		TRect rc = node->DisplayRect(true);
        Canvas->Brush->Color = clWindow;
        Canvas->Font->Color = clDkGray;
        if (node->Object) Canvas->TextOut(rc.right+6, rc.top+2, node->Object->GetTypeInfo()->className);
	}
	if (Stage == cdPostErase){
		TRect rc = node->DisplayRect(true);
        if (node->Object){
            int len = Canvas->TextWidth(node->Object->GetTypeInfo()->className);
            TRect crc(rc.right+6, rc.top, rc.right+len, rc.bottom);
            Canvas->FillRect(crc);
        }
	}
    return true;
}

bool __fastcall TSHTreeView::IsCustomDrawn(TCustomDrawTarget Target, TCustomDrawStage Stage){
	return true;
}
void __fastcall TSHTreeView::Edit(const tagTVITEMA &Item){
    TCustomTreeView::Edit(Item);
    TSHTreeNode* node = (TSHTreeNode*)Items->GetNode(Item.hItem);
	AnsiString S;
    if (Item.mask & TVIF_TEXT){
        S = Item.pszText;
    	if (node->Object){
    		node->Object->SetName(S.c_str(), scene->Imp);
    		S = node->Object->GetName();
    	}
        node->Text = S;
    }
}
void __fastcall TSHTreeView::Delete(TTreeNode* Node){
    TCustomTreeView::Delete(Node);
    TSHTreeNode* node = (TSHTreeNode*)Node;
    if (node->Object && node->Parent && node->Parent->Object){
        node->Parent->Object->DelChildObject(node->Object, scene->Imp);
    }
}

/*
bool __fastcall TSHTreeView::CanExpand(TTreeNode* Node){
    SGObject* obj = ((TSHTreeNode*)Node)->Object;
    bool rv = obj && obj->NChildObjects();
//    ((TSHTreeNode*)Node)->Update();

	return rv;
}
*/

bool __fastcall TSHTreeView::CanCollapse(TTreeNode* Node){
    SGObject* obj = ((TSHTreeNode*)Node)->Object;
    bool rv = obj && obj->NChildObjects();
	((TSHTreeNode*)Node)->Update();
	return rv;
}

namespace Shtreeview
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSHTreeView)};
		RegisterComponents("SpringHead", classes, 0);
	}
}

