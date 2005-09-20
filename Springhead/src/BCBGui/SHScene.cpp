#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include <basepch.h>
#include <DesignEditors.hpp>
#include <DesignIntf.hpp>


#include "SHScene.h"
#include "SHBox.h"
#include "SHCamera.h"
#include "SHLight.h"
#include "SHSolid.h"
#include "SHPhysicalMaterial.h"
#include "SHSceneGraphDlg.h"
#include <WinBasis/WBPath.h>
#include <ImpD3D/D3Render.h>
#include <ImpD3D/D3FileDoc.h>
#include <Framework/FWFileIOD3D.h>
using namespace Spr;

#pragma resource "*.res"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck は、定義されたコンポーネントが純粋仮想関数を含む
// 抽象クラスではないことを確認するために定義されています。
//

static inline void ValidCtrCheck(TSHScene *)
{
	new TSHScene(NULL);
}

//---------------------------------------------------------------------------
__fastcall TSHScene::TSHScene(TComponent* Owner)
	: TSHComponent(Owner), dlg(NULL), showTree(false){
	Scene = (TSHScene*)this;
}
//---------------------------------------------------------------------------
__fastcall TSHScene::~TSHScene(){
    while(BoxCount) Boxes[BoxCount-1]->Scene = NULL;
	delete dlg;
}

void* TSHScene::CreateObjectOrClass(TComponent* owner, Spr::SGObject* o, bool bClass){
#define RETURN_OBJECT_OR_CLASS(name)	RETURN_OBJECT_OR_CLASS2(TSH##name, SG##name)
#define RETURN_OBJECT_OR_CLASS2(T, S)						\
	if (DCAST(S, o)){										\
		if (bClass){										\
			return __classid(T);							\
		}else{												\
			T* t = new T(owner);							\
			t->Scene = this;								\
			t->Imp = (S*)o;									\
			return t;										\
		}													\
	}
	RETURN_OBJECT_OR_CLASS(Frame);
	RETURN_OBJECT_OR_CLASS2(TSHLight, GRLight);
	RETURN_OBJECT_OR_CLASS2(TSHCamera, GRCamera);
	RETURN_OBJECT_OR_CLASS2(TSHSolid, PHSolid);
	RETURN_OBJECT_OR_CLASS2(TSHPhysicalMaterial, PHPhysicalMaterial);
	return NULL;
}

void __fastcall TSHScene::NotifyChange(TObject* o){
    for(int i=0; i<BoxCount; ++i){
    	Boxes[i]->Invalidate();
    }
}

void TSHScene::NotifyChangeName(Spr::SGObject* o){
	if (dlg) dlg->NotifyChangeName(o);
}

D3Render* __fastcall TSHScene::GetRenderer(){
	if (!app.render){
		TForm* form = (TForm*)Owner;
		app.Init(form->Handle);
	}
	return (D3Render*)&*app.render;
}
Spr::SGFrame* TSHScene::GetWorkFrame(const char* fn){
	static std::map<std::string, UTRef<SGFrame> > frames;
	if (!frames[fn] && scene){
		frames[fn] = new SGFrame;
		static FWFileIOD3D fio;
		AnsiString path = getenv("SPRINGHEAD");
		path += "\\src\\BCBGui\\";
        path += fn;
		fio.Load(path.c_str(), scene->Imp, frames[fn]);
        frames[fn]->Print(DSTR);
	}
	return frames[fn];
}
Spr::SGFrame* TSHScene::GetBBoxFrame(){
	return GetWorkFrame("BoundBox.x");
}
Spr::SGFrame* TSHScene::GetAxisFrame(){
	return GetWorkFrame("Axis.x");
}

TSHSceneGraphDlg* __fastcall TSHScene::GetDlg(){
	if (!dlg && (showTree || (ComponentState.Contains(csDesigning)))){
		dlg = new TSHSceneGraphDlg(Application, this);
	}
	return dlg;
}
void __fastcall TSHScene::SetShowTree(bool b){
	showTree = b;
	if (Dlg){
		if (showTree) Dlg->Show();
		else Dlg->Hide();
	}
}

void __fastcall TSHScene::Loaded(void){
}
void __fastcall TSHScene::WriteState(TWriter* Writer){
	TComponent::WriteState(Writer);
	Export(Imp, FileName.c_str());
}

void __fastcall TSHScene::SetFileName(AnsiString fn){
	if (fileName == fn) return;
    fileName = fn;
	//	ファイル名の変更
    app.Load(fileName.c_str());
}
AnsiString __fastcall TSHScene::GetFileName(){
	if (fileName.Length() == 0){
		fileName = Name + ".x";
	}
	return fileName;
}
//---------------------------------------------------------------------------
Spr::SGScene* __fastcall TSHScene::GetImp(){
	if (!GetObject() && BoxCount){
		app.Init(Boxes[0]->Handle);
	}
	SetObject(app.scene);
	return (Spr::SGScene*)GetObject();
}
TSHBox* __fastcall TSHScene::GetBoxes(int idx){
	if (idx < 0 || idx >= (int)boxes.size()) return NULL;
	return boxes[idx];
}
int __fastcall TSHScene::GetBoxCount(){
	return boxes.size();
}

bool TSHScene::Import(SGObject* obj, Spr::UTString filename){
	WBPath path;
	path.Path(filename);
	app.fileIO.Load(path.FullPath().c_str(), Imp, obj);
	return true;
}
bool TSHScene::Export(SGObject* obj, UTString filename){
    if (!obj) obj = Imp;
    if (!obj) return false;
	WBPath path;
	path.Path(filename);
	app.fileIO.Save(path.FullPath().c_str(), Imp, obj);
	return true;
}
void __fastcall TSHScene::Render(TSHBox* box){
	Renderer->Set(box->Camera->Imp);
	if (Renderer && Renderer->device){
		//	ビューポート(描画サイズ)の設定
		Renderer->SetViewport(box->Handle);
		//	画面クリア
		Renderer->ClearBuffer();
		//	シーングラフの描画
		Renderer->BeginScene();
		if (Imp->GetWorld()) Renderer->Render(Imp);
		//	選択フレームの表示
		if (dlg){
            for(unsigned int i=0; i<dlg->Tree->SelectionCount; ++i){
                SGFrame* fr = DCAST(SGFrame, dlg->Tree->Selections[i]->Object);
                if (!fr) continue;
	            fr->CalcBBox();
	        	Affinef afw = fr->GetWorldPosture();
				Vec3f bbMin = fr->bbox.GetBBoxMin();
				Vec3f bbMax = fr->bbox.GetBBoxMax();
				if (bbMin.X() <= bbMax.X()){
					Affinef afb(afw);
					Affinef afc(afw);
					afb.Pos() = afw * bbMin;
					afb.Ex() *= bbMax.X()-bbMin.X();
					afb.Ey() *= bbMax.Y()-bbMin.Y();
					afb.Ez() *= bbMax.Z()-bbMin.Z();
        			GetBBoxFrame()->SetPosture(afb);
					Renderer->RenderRecurse(GetBBoxFrame());
					GetAxisFrame()->SetPosture(afc);
					Renderer->RenderRecurse(GetAxisFrame());
				}
			}
		}
		//	シーンの描画終了
		Renderer->EndScene();
		//	表示
		Renderer->Present();
	}
}

bool TSHScene::Load(Spr::UTString filename){
	WBPath path;
	path.Path(filename);
    app.Load(path.FullPath().c_str());
	return true;
}

void TSHScene::Reset(){
    app.Reset();
    if(dlg){
        TSHTreeNode* node;
        node = dlg->Tree->Selected;
        while(node->Parent) node = node->Parent;
        node->Update();
    }
    NotifyChange();
}


//---------------------------------------------------------------------------
Spr::SGObject* TSHScene::FindObject(const Spr::UTString& key){
	return Imp->FindObject(key);
}
//---------------------------------------------------------------------------
//  TSHSceneEditor
//
class TSHSceneEditor: public TComponentEditor{
	__fastcall TSHSceneEditor(Classes::TComponent* AComponent, _di_IDesigner ADesigner);
	virtual __fastcall ~TSHSceneEditor();
	protected:
	virtual void __fastcall Edit();
};
__fastcall TSHSceneEditor::TSHSceneEditor(Classes::TComponent* AComponent, _di_IDesigner ADesigner):
	TComponentEditor(AComponent, ADesigner){
}
__fastcall TSHSceneEditor::~TSHSceneEditor(){
}
void __fastcall TSHSceneEditor::Edit(){
	TSHScene* scene = (TSHScene*)Component;
	if (scene && scene->Dlg){
		scene->Dlg->Show();
		scene->Dlg->Invalidate();
	}
}

//---------------------------------------------------------------------------
namespace Shscene
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TSHScene)};
		RegisterComponents("SpringHead", classes, 0);
		RegisterComponentEditor(__classid(TSHScene), __classid(TSHSceneEditor));
//		RegisterPropertyEditor(__typeinfo(TSHBox), __classid(TSHScene), "Box", __classid(TSHSceneGetDesigner));
	}
}
//---------------------------------------------------------------------------

