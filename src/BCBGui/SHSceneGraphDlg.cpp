#include "BCBGuiPCH.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include <vcl.h>
#include "SHSceneGraphDlg.h"
#include "SHLight.h"
#include "SHScene.h"
#include "SHBox.h"
#include "SHCamera.h"
#include <algorithm>

using namespace Spr;
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHTreeView"
#pragma resource "*.dfm"
TSHSceneGraphDlg *SceneGraphDlg;
//---------------------------------------------------------------------------
__fastcall TSHSceneGraphDlg::TSHSceneGraphDlg(TComponent* Owner, TSHScene* s)
	: TForm(Application), scene(s), tmpLight(NULL){
	Tree->Scene = scene;
    scene->dlg = this;
}
//---------------------------------------------------------------------------
__fastcall TSHSceneGraphDlg::~TSHSceneGraphDlg(){
	if (scene) scene->dlg = NULL;
}
//---------------------------------------------------------------------------
TSHLight* TSHSceneGraphDlg::GetTmpLight()
{
	if (!tmpLight){
		tmpLight= new TSHLight(this);
		tmpLight->Scene= scene;
	}
	return tmpLight;
}

//---------------------------------------------------------------------------
_di_IDesigner __fastcall TSHSceneGraphDlg::GetDesigner(){
	if (!designer && scene){
		TForm* form = (TForm*)scene->Owner;
		if (form->Designer){
			form->Designer->Supports(designer);
		}
	}
	return designer;
}

//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::InsertClick(TObject *Sender)
{
//	if (Tree->Selected) Tree->Selected->InsertFrame();
}
//---------------------------------------------------------------------------


void __fastcall TSHSceneGraphDlg::DeleteClick(TObject *Sender){
	if (Tree->Selected) Tree->Selected->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::ImportClick(TObject *Sender)
{
	if (!Tree->Selected) return;
	OpenDlg->Execute();
	for(int i=0; i<OpenDlg->Files->Count; ++i){
		Tree->Selected->Import(OpenDlg->Files->Strings[i].c_str());
	}
    Tree->Selected->Update();
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::ExportClick(TObject *Sender){
	if (!Tree->Selected) return;
	SaveDlg->Execute();
	if (SaveDlg->Files->Count){
		Tree->Selected->Export(SaveDlg->Files->Strings[0].c_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::AddLightClick(TObject *Sender)
{
	if (!Tree->Selected) return;
	SGFrame* fr = DCAST(SGFrame, Tree->SelectedComponent->Imp);
	if (!fr) return;
	//ライトを作成
	GRLight* light= new GRLight;
	light->frPosture = fr;
	//位置フレームを対象フレームに設定（仮）
	SGObjects::iterator it;
	for (it=fr->contents.begin(); it!=fr->contents.end(); ++it){
		if(DCAST(GRLight, (*it))) break;
	}
	fr->contents.insert(it, light);
	light->SetName("Light", scene->Imp);
	Tree->Selected->Update();
}
//---------------------------------------------------------------------------
//オブジェクト名の変更を伝える
void TSHSceneGraphDlg::NotifyChangeName(Spr::SGObject* o)
{
	if (!o) return;
	for(int i= 0; i<Tree->Items->Count; ++i){
		TSHTreeNode* node = (TSHTreeNode*)Tree->Items->Item[i];
		if (node->Object == o){
			node->Text = o->GetName();
		}
	}
}

//---------------------------------------------------------------------------
void TSHSceneGraphDlg::SelectComponent(TPersistent* comp)
{
	if (!Designer) return;
	Designer->SelectComponent(comp);
	Designer->Modified();
}

//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::ComponentClick(TObject *Sender)
{
	if (!Tree->Selected) return;
	Tree->CreateComponent(Tree->Selected->Object);
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::FormCreate(TObject *Sender)
{
    Tree->HideSelection = false;
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::TreeChange(TObject *Sender,
			TTreeNode *Node)
{
	TSHTreeNode* node= Tree->Selected;
	if (!node || !node->Object) return;

    TSHComponent* comp = Tree->GetComponent(node->Object);
    SelectComponent(comp);

    // 選択ノードがSGFrameの場合
    if( node->Object->GetTypeInfo() == SGFrame::GetTypeInfoStatic() ){
        FramePanel->Show();
        LightPanel->Hide();
        SolidPanel->Hide();
        PHMaterialPanel->Hide();

        SGFrame* fr = DCAST(SGFrame, node->Object);
        Spr::Affinef afNow = fr->GetPosture();
        UpdateStringGridAffine(afNow);
        UpdateEuler(afNow);
        UpdateStringGridScale(afNow);
    }
    // 選択ノードがGRLightの場合
    else if ( node->Object->GetTypeInfo() == GRLight::GetTypeInfoStatic() )
    {
        FramePanel->Hide();
        LightPanel->Show();
        SolidPanel->Hide();
        PHMaterialPanel->Hide();
        UpdateLightPanel(comp);
    }
    // 選択ノードがPHSolidの場合
    else if ( node->Object->GetTypeInfo() == PHSolid::GetTypeInfoStatic() )
    {
        FramePanel->Hide();
        LightPanel->Hide();
        SolidPanel->Show();
        PHMaterialPanel->Hide();
        UpdateSolidPanel(comp);
    }
    // 選択ノードがPHPhysicalMaterialの場合
    else if ( node->Object->GetTypeInfo() == PHPhysicalMaterial::GetTypeInfoStatic() )
    {
        FramePanel->Hide();
        LightPanel->Hide();
        SolidPanel->Hide();
        PHMaterialPanel->Show();
        UpdatePHMaterialPanel(comp);
    }
    // 選択ノードがその他の場合
    else {
        FramePanel->Hide();
        LightPanel->Hide();
        SolidPanel->Hide();
        PHMaterialPanel->Hide();
    }

}
//---------------------------------------------------------------------------
//
//以下、マウスドラッグによるフレームの操作
//
void __fastcall TSHSceneGraphDlg::ButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    lastX = X;
	lastY = Y;
    LabelFrameState->Caption = "Drag!";
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::ButtonMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    LabelFrameState->Caption = "Ready";
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::ButtonMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    if (!Tree->Selected) return;
	if (Shift.Contains(ssLeft)){
        SGFrame* fr = DCAST(SGFrame, Tree->Selected->Object);
        if (!fr) return;

        int DeltaX = X - lastX;
		int DeltaY = Y - lastY;
		Spr::Affinef afNow = fr->GetPosture();

        //Buttonにより、動作を指定（移動、回転）
        if(Sender == ButtonFrameX){
            afNow.Trn() = afNow.Trn() + Vec3f(float(-DeltaY * 0.05), 0.0f, 0.0f);
        }
        else if(Sender == ButtonFrameY){
            afNow.Trn() = afNow.Trn() + Vec3f(0.0f, float(-DeltaY * 0.05), 0.0f);
        }
        else if(Sender == ButtonFrameZ){
            afNow.Trn() = afNow.Trn() + Vec3f(0.0f, 0.0f, float(-DeltaY * 0.05));
        }
        else if(Sender == ButtonFrameXY){
            afNow.Trn() = afNow.Trn() + Vec3f(float(DeltaX * 0.05), float(-DeltaY * 0.05), 0.0f);
        }
        else if(Sender == ButtonFrameYZ){
            afNow.Trn() = afNow.Trn() + Vec3f(0.0f, float(-DeltaY * 0.05), float(DeltaX * 0.05));
        }
        else if(Sender == ButtonFrameZX){
            afNow.Trn() = afNow.Trn() + Vec3f(float(DeltaX * 0.05), 0.0f, float(-DeltaY * 0.05));
		}
        else if(Sender == ButtonFrameRotX){
            afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(-DeltaY), 'X') * afNow.Rot();
        }
        else if(Sender == ButtonFrameRotY){
            afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(-DeltaY), 'Y') * afNow.Rot();
        }
        else if(Sender == ButtonFrameRotZ){
            afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(-DeltaY), 'Z') * afNow.Rot();
        }
		fr->SetPosture(afNow);
		scene->NotifyChange();
        lastX = X;
		lastY = Y;

        TButton* theButton = (TButton*)Sender;
        LabelFrameState->Caption = theButton->Caption + " Move";
        UpdateStringGridAffine(afNow);
        UpdateEuler(afNow);
    }
}
//---------------------------------------------------------------------------
//
//視点の操作
//
void __fastcall TSHSceneGraphDlg::ImageViewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    lastX = X;
    lastY = Y;
	LabelFrameState->Caption = "Drag!";
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::ImageViewMouseMove(TObject *Sender,
	  TShiftState Shift, int X, int Y)
{
	if (Shift.Contains(ssLeft)){
/*		TDesignerSelections* sels = new TDesignerSelections;
		_di_IDesignerSelections selIf;
		sels->GetInterface(selIf);
		if (GetDesigner()) GetDesigner()->GetSelections(selIf);
		std::vector<TSHBox*> selBoxes;
		for(int i=0; i>scene->BoxCount; ++i){
			for(int j=0; j<selIf->Count; ++j){
				if (selIf->Items[j] == scene->Boxes[i]){
					selBoxes.push_back(scene->Boxes[i]);
					break;
				}
			}
		}
		if (selBoxes.size() == 0) selBoxes.push_back(scene->Boxes[0]);
 */
		int DeltaX = X - lastX;
		int DeltaY = Y - lastY;
		TSHBox* box = scene->Boxes[0];
		for(int i=0; i<scene->BoxCount; ++i){
			if (scene->Boxes[i]->Camera->Imp == Tree->SelectedComponent->Imp){
				box = scene->Boxes[i];
				break;
			}
		}
		if(Sender == ImageFrameViewTrans){
			box->Camera->Imp->data.view.PosX() += DeltaX * 0.2;
			box->Camera->Imp->data.view.PosY() -= DeltaY * 0.2;
			LabelFrameState->Caption = "ViewTrans";
		}
		else if(Sender == ImageFrameViewRotate){
			Spr::Affinef afNow = box->Camera->Imp->data.view;
			afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(-DeltaX * 0.3), afNow.Ey().unit()) * afNow.Rot();
			afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(-DeltaY * 0.3), Vec3f(1, 0, 0)) * afNow.Rot();
			box->Camera->Imp->data.view = afNow;
			LabelFrameState->Caption = "ViewRotate";
		}
		else if(Sender == ImageFrameViewZoom){
			box->Camera->Imp->data.view.PosZ() -= DeltaY * 0.2;
			LabelFrameState->Caption = "ViewZoom";
		}
		scene->NotifyChange();
		lastX = X;
		lastY = Y;
	}
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::ImageViewMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    LabelFrameState->Caption = "Ready";
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::StringGridFrameAffineSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    SGFrame* fr = DCAST(SGFrame, Tree->Selected->Object);
    if (!fr) return;
    Spr::Affinef afNow = fr->GetPosture();
    try{
        afNow[ARow][ACol] = StringGridFrameAffine->Cells[ACol][ARow].ToDouble();
    }
    catch(...){}
    fr->SetPosture(afNow);
    scene->NotifyChange();
    UpdateStringGridScale(afNow);
    UpdateEuler(afNow);
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::LabeledEditRotEnter(TObject *Sender)
{
/*    LabelFrameState->Caption = "Input Euler Angle";

    TLabeledEdit* theLEdit = (TLabeledEdit*)Sender;
    theLEdit->SelectAll();
*/}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::LabeledEditRotExit(TObject *Sender)
{
    LabelFrameState->Caption = "Ready";
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::LabeledEditRotChange(TObject *Sender)
{
    SGFrame* fr = DCAST(SGFrame, Tree->Selected->Object);
    if (!fr) return;
    Spr::Affinef afNow = fr->GetPosture();

    float normX = afNow.Ex().norm();
    float normY = afNow.Ey().norm();
    float normZ = afNow.Ez().norm();

    try{
        afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(LabeledEditFrameRotX->Text.ToDouble()), 'X');
        afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(LabeledEditFrameRotY->Text.ToDouble()), 'Y') * afNow.Rot();
        afNow.Rot() = Spr::Matrix3f::Rot(Spr::Rad(LabeledEditFrameRotZ->Text.ToDouble()), 'Z') * afNow.Rot();
    }
    catch(...){}

    afNow.Ex() *= normX;
    afNow.Ey() *= normY;
    afNow.Ez() *= normZ;

    fr->SetPosture(afNow);
    UpdateStringGridAffine(afNow);
    scene->NotifyChange();
}
//---------------------------------------------------------------------------
//アフィン行列の更新
void TSHSceneGraphDlg::UpdateStringGridAffine(Spr::Affinef afNow){
    StringGridFrameAffine->Cells[0][0] = afNow.Ex().X();
    StringGridFrameAffine->Cells[0][1] = afNow.Ex().Y();
    StringGridFrameAffine->Cells[0][2] = afNow.Ex().Z();
    StringGridFrameAffine->Cells[1][0] = afNow.Ey().X();
    StringGridFrameAffine->Cells[1][1] = afNow.Ey().Y();
    StringGridFrameAffine->Cells[1][2] = afNow.Ey().Z();
    StringGridFrameAffine->Cells[2][0] = afNow.Ez().X();
    StringGridFrameAffine->Cells[2][1] = afNow.Ez().Y();
    StringGridFrameAffine->Cells[2][2] = afNow.Ez().Z();
    StringGridFrameAffine->Cells[3][0] = afNow.Trn().X();
    StringGridFrameAffine->Cells[3][1] = afNow.Trn().Y();
    StringGridFrameAffine->Cells[3][2] = afNow.Trn().Z();
}
//---------------------------------------------------------------------------
//オイラー角の更新

/* アフィン行列の中身（回転部分）
    | CE BDE-AF  ADE+BF |
    | CF BDF+AE  ADF-BE |
    | -D   BC      AC   |
    where A,B are the cosine and sine of the X-axis rotation axis,
          C,D are the cosine and sine of the Y-axis rotation axis,
          E,F are the cosine and sine of the Z-axis rotation axis.

    if(D == 0)
        | 0  -F  DE |
        | 0   E  DF |
        |-D   0  0  |
        where X-axis rotation axis is zero (A = 1, B = 0).

        D = 0の場合は、X軸周りの回転とZ軸周りの回転は同じ回転なので
        　　　　　　　 どちらかの回転を0と定めることで容易に求まる
        　　　　　　   ここではX軸周りの回転を0とした
*/
void TSHSceneGraphDlg::UpdateEuler(Spr::Affinef afNow){
    float PI = 3.1415926535;
    float NEZERO = 0.00005;
    float angleX, angleY, angleZ;
    float cosX, sinX, cosY, sinY, cosZ, sinZ;

    afNow.Ex() /= afNow.Ex().norm();
    afNow.Ey() /= afNow.Ey().norm();
    afNow.Ez() /= afNow.Ez().norm();

    sinY   = -afNow.Ex().Z();
    angleY = asin(sinY);
    cosY   = cos(angleY);

    if(fabs(cosY) > Rad(NEZERO)){
        cosX = afNow.Ez().Z() / cosY;
        sinX = afNow.Ey().Z() / cosY;
        angleX = atan2(sinX, cosX);

        cosZ = afNow.Ex().X() / cosY;
        sinZ = afNow.Ex().Y() / cosY;
        angleZ = atan2(sinZ, cosZ);
    }
    else {
        angleX = 0;

        cosZ =  afNow.Ey().Y();
        sinZ = -afNow.Ey().X();
        angleZ = atan2(sinZ, cosZ);
    }

    LabeledEditFrameRotX->Text = angleX * 180 / PI;
    LabeledEditFrameRotY->Text = angleY * 180 / PI;
    LabeledEditFrameRotZ->Text = angleZ * 180 / PI;
}
//---------------------------------------------------------------------------
void TSHSceneGraphDlg::UpdateStringGridScale(Spr::Affinef afNow){
    StringGridFrameScale->Cells[0][0] = afNow.Ex().norm();
    StringGridFrameScale->Cells[1][0] = afNow.Ey().norm();
    StringGridFrameScale->Cells[2][0] = afNow.Ez().norm();
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::LabeledEditRotMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TLabeledEdit* theLEdit = (TLabeledEdit*)Sender;
    theLEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::ButtonFrameInitRotClick(TObject *Sender)
{
    SGFrame* fr = DCAST(SGFrame, Tree->Selected->Object);
    if (!fr) return;
    Spr::Affinef afNow = fr->GetPosture();
    afNow.Ex() = Vec3f(1, 0, 0) * StringGridFrameScale->Cells[0][0].ToDouble();
    afNow.Ey() = Vec3f(0, 1, 0) * StringGridFrameScale->Cells[1][0].ToDouble();
    afNow.Ez() = Vec3f(0, 0, 1) * StringGridFrameScale->Cells[2][0].ToDouble();
    fr->SetPosture(afNow);
    scene->NotifyChange();
    UpdateStringGridAffine(afNow);
    UpdateEuler(afNow);
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::StringGridFrameScaleSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    SGFrame* fr = DCAST(SGFrame, Tree->Selected->Object);
    if (!fr) return;
    Spr::Affinef afNow = fr->GetPosture();
    try{
        if(StringGridFrameScale->Cells[ACol][ARow].ToDouble() == 0){ return; }

        if(ACol == 0){
            afNow.Ex() /= afNow.Ex().norm();
            afNow.Ex() *= StringGridFrameScale->Cells[ACol][ARow].ToDouble();
        }
        else if(ACol == 1){
            afNow.Ey() /= afNow.Ey().norm();
            afNow.Ey() *= StringGridFrameScale->Cells[ACol][ARow].ToDouble();
        }
        else if(ACol == 2){
            afNow.Ez() /= afNow.Ez().norm();
            afNow.Ez() *= StringGridFrameScale->Cells[ACol][ARow].ToDouble();
        }
    }
    catch(...){}
    fr->SetPosture(afNow);
    scene->NotifyChange();
    UpdateStringGridAffine(afNow);
}
//---------------------------------------------------------------------------
void __fastcall TSHSceneGraphDlg::ButtonMeshCtrlClick(TObject *Sender)
{
    // フレームの取得
    SGFrame* fr = DCAST(SGFrame, Tree->Selected->Object);
    if (!fr) return;
    Spr::Affinef afOld = fr->GetPosture();
    Spr::Affinef afNew;
    Spr::Affinef afChange;

    if(Sender == ButtonFrameMeshCtrlScale){
        afNew.Ex() = afOld.Ex() / afOld.Ex().norm();
        afNew.Ey() = afOld.Ey() / afOld.Ey().norm();
        afNew.Ez() = afOld.Ez() / afOld.Ez().norm();
        afNew.Pos() = afOld.Pos();
    }
    else if(Sender == ButtonFrameMeshCtrlRot){
        afNew.Ex() = Vec3f(1, 0, 0) * afOld.Ex().norm();
        afNew.Ey() = Vec3f(0, 1, 0) * afOld.Ey().norm();
        afNew.Ez() = Vec3f(0, 0, 1) * afOld.Ez().norm();
        afNew.Pos() = afOld.Pos();
    }
    else if(Sender == ButtonFrameMeshCtrlPos){
        LabelFrameState->Caption = "HOGE";
        afNew.Rot() = afOld.Rot();
        afNew.Pos() = Vec3f(0, 0, 0);
    }
    Matrix3f for_bcb = afNew.Rot();
    afChange.Rot() = for_bcb.inv() * afOld.Rot();
    afChange.Pos() = -afNew.Pos() + afOld.Pos();

    if(CheckBoxFrameMeshCheck->Checked){
        // フレームのメッシュを取得
        D3Mesh* mesh;
        bool flag = false;
        for(unsigned int i = 0; i < Tree->Selected->Object->NChildObjects(); i++){
            mesh = DCAST(D3Mesh, Tree->Selected->Object->ChildObject(i));
            if(mesh) { flag = true; break; }
        }
        if(!flag) return;
        //　メッシュの情報の取得
        DWORD nVertex = mesh->intf->GetNumVertices();
        DWORD fvf = mesh->intf->GetFVF();

        if(fvf != (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)){
            struct VtxFVF{
               Vec3f pos;
               Vec3f normal;
            };
			VtxFVF* vtxs;
			mesh->LockVertexBuffer(0, (void**)&vtxs);
			for(unsigned int i = 0; i < nVertex; i++){
                vtxs[i].pos    = afChange.Rot() * vtxs[i].pos;
                vtxs[i].pos    = afChange.Pos() + vtxs[i].pos;
                vtxs[i].normal = afChange.Rot() * vtxs[i].normal;
                vtxs[i].normal /= vtxs[i].normal.norm();
            }
        }
        else{
            struct VtxFVF{
                Vec3f pos;
                Vec3f normal;
                Vec2f tex;
            };
            VtxFVF* vtxs;
            mesh->LockVertexBuffer(0, (void**)&vtxs);
            for(unsigned int i = 0; i < nVertex; i++){
                vtxs[i].pos    = afChange.Rot() * vtxs[i].pos;
                vtxs[i].normal = afChange.Rot() * vtxs[i].normal;
                vtxs[i].normal /= vtxs[i].normal.norm();
                vtxs[i].tex    = afChange.Rot() * vtxs[i].tex;
            }
        }
        mesh->intf->UnlockVertexBuffer();
        mesh->intf->OptimizeInplace(D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);
    }

    fr->SetPosture(afNew);
    UpdateStringGridAffine(afNew);
    UpdateStringGridScale(afNew);
    UpdateEuler(afNew);
    scene->NotifyChange();
}
//---------------------------------------------------------------------------


void __fastcall TSHSceneGraphDlg::TreeKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    if( Key == VK_DELETE ){
        if (Tree->Selected) Tree->Selected->Delete();
    }    
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::StringGridDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
    if( ((TStringGrid *)Sender)->Focused( ) != true ){
        if( ((TStringGrid *)Sender)->Row == ARow && ((TStringGrid *)Sender)->Col == ACol ){
            ((TStringGrid *)Sender)->Canvas->Brush->Color = ((TStringGrid *)Sender)->Color;
            ((TStringGrid *)Sender)->Canvas->FillRect( Rect );
            ((TStringGrid *)Sender)->Canvas->Font->Color = ((TStringGrid *)Sender)->Font->Color;
            ((TStringGrid *)Sender)->Canvas->TextOut( Rect.Left + 2, Rect.Top + 2, ((TStringGrid *)Sender)->Cells[ACol][ARow] );
        }
    }
}

//***********************************************************
//********* GRLight *****************************************
//***********************************************************

void TSHSceneGraphDlg::UpdateLightPanel(TSHComponent* comp)
{
    TSHLight* ltComp = (TSHLight*)comp;

    // ライトタイプ
    switch(ltComp->Type){
        case ltPoint:
            RadioButtonLightPoint->Checked = true;
            LabeledEditLightRange->Enabled = true;
            LabeledEditLightFalloff->Enabled = false;
            LabelLightAttenuation->Enabled = true;
            StringGridLightAttenuation->Enabled = true;
            StringGridLightAttenuation->Font->Color = clWindowText;
            LabeledEditLightTheta->Enabled = false;
            LabeledEditLightPhi->Enabled = false;
            break;
	   	case ltSpot:
            RadioButtonLightSpot->Checked = true;
            LabeledEditLightRange->Enabled = true;
            LabeledEditLightFalloff->Enabled = true;
            LabelLightAttenuation->Enabled = true;
            StringGridLightAttenuation->Enabled = true;
            StringGridLightAttenuation->Font->Color = clWindowText;
            LabeledEditLightTheta->Enabled = true;
            LabeledEditLightPhi->Enabled = true;
            break;
	  	case ltDirectional:
            RadioButtonLightDirectional->Checked = true;
            LabeledEditLightRange->Enabled = false;
            LabeledEditLightFalloff->Enabled = false;
            LabelLightAttenuation->Enabled = false;
            StringGridLightAttenuation->Enabled = false;
            StringGridLightAttenuation->Font->Color = clInactiveCaption;
            LabeledEditLightTheta->Enabled = false;
            LabeledEditLightPhi->Enabled = false;
            break;
    }

    // Diffuse, Specular, Ambient
	Vec4f diff;
	Vec4f spec;
    Vec4f ambi;
    diff = ltComp->DColor(ltComp->Diffuse);
    spec = ltComp->DColor(ltComp->Specular);
    ambi = ltComp->DColor(ltComp->Ambient);
	StringGridLightDiffuse->Cells[0][0] = diff.x;
	StringGridLightDiffuse->Cells[1][0] = diff.y;
	StringGridLightDiffuse->Cells[2][0] = diff.z;
	StringGridLightDiffuse->Cells[3][0] = diff.w;
	StringGridLightSpecular->Cells[0][0] = spec.x;
	StringGridLightSpecular->Cells[1][0] = spec.y;
	StringGridLightSpecular->Cells[2][0] = spec.z;
	StringGridLightSpecular->Cells[3][0] = spec.w;
	StringGridLightAmbient->Cells[0][0] = ambi.x;
	StringGridLightAmbient->Cells[1][0] = ambi.y;
	StringGridLightAmbient->Cells[2][0] = ambi.z;
	StringGridLightAmbient->Cells[3][0] = ambi.w;
	PanelLightDiffuse->Color = ltComp->Diffuse;
    PanelLightSpecular->Color = ltComp->Specular;
    PanelLightAmbient->Color = ltComp->Ambient;

    // Range, Falloff
    LabeledEditLightRange->Text = ltComp->Range;
    LabeledEditLightFalloff->Text = ltComp->Falloff;

    // Attenuation
    StringGridLightAttenuation->Cells[0][0] = ltComp->Attenuation0;
    StringGridLightAttenuation->Cells[1][0] = ltComp->Attenuation1;
    StringGridLightAttenuation->Cells[2][0] = ltComp->Attenuation2;

    // Theta, Phi
    LabeledEditLightTheta->Text = ltComp->Theta;
    LabeledEditLightPhi->Text = ltComp->Phi;
}

//----------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::RadioButtonLightPointClick(
      TObject *Sender)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    light->position.W() = 1;

    scene->NotifyChange();
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    UpdateLightPanel(comp);
}

void __fastcall TSHSceneGraphDlg::RadioButtonLightSpotClick(
      TObject *Sender)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

	light->position.w = 1;
	light->spotCutoff = M_PI/4;

    scene->NotifyChange();
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    UpdateLightPanel(comp);
}

void __fastcall TSHSceneGraphDlg::RadioButtonLightDirectionalClick(
      TObject *Sender)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

	light->position.w = 0;

	scene->NotifyChange();
	TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
	UpdateLightPanel(comp);
}

void __fastcall TSHSceneGraphDlg::StringGridLightDiffuseSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    double v;
    try{
        v = Value.ToDouble();
    }
    catch(...){}

    // 各要素は0以上1以下
    if(v > 1){
        StringGridLightDiffuse->Cells[ACol][ARow] = 1;
        v = 1;
    }
    else if(v < 0){
        StringGridLightDiffuse->Cells[ACol][ARow] = 0;
        v = 0;
    }

	if(ACol == 0) light->diffuse.x = v;
	else if(ACol == 1) light->diffuse.y = v;
	else if(ACol == 2) light->diffuse.z = v;
	else if(ACol == 3) light->diffuse.w = v;

    scene->NotifyChange();

    // カラーパネルの色を変更
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    TSHLight* ltComp = (TSHLight*)comp;
    PanelLightDiffuse->Color = ltComp->Diffuse;
}

void __fastcall TSHSceneGraphDlg::StringGridLightSpecularSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    double v;
    try{
        v = Value.ToDouble();
    }
    catch(...){}

    // 各要素は0以上1以下
    if(v > 1){
        StringGridLightSpecular->Cells[ACol][ARow] = 1;
        v = 1;
    }
    else if(v < 0){
        StringGridLightSpecular->Cells[ACol][ARow] = 0;
        v = 0;
    }

	if(ACol == 0) light->specular.x = v;
	else if(ACol == 1) light->specular.y = v;
	else if(ACol == 2) light->specular.x = v;
	else if(ACol == 3) light->specular.w = v;

    scene->NotifyChange();

    // カラーパネルの色を変更
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    TSHLight* ltComp = (TSHLight*)comp;
    PanelLightSpecular->Color = ltComp->Specular;
}

void __fastcall TSHSceneGraphDlg::StringGridLightAmbientSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    double v;
    try{
        v = Value.ToDouble();
    }
    catch(...){}

    // 各要素は0以上1以下
    if(v > 1){
        StringGridLightAmbient->Cells[ACol][ARow] = 1;
        v = 1;
    }
    else if(v < 0){
        StringGridLightAmbient->Cells[ACol][ARow] = 0;
        v = 0;
    }

	if(ACol == 0) light->ambient.x = v;
	else if(ACol == 1) light->ambient.y = v;
	else if(ACol == 2) light->ambient.z = v;
	else if(ACol == 3) light->ambient.w = v;

    scene->NotifyChange();

    // カラーパネルの色を変更
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    TSHLight* ltComp = (TSHLight*)comp;
    PanelLightAmbient->Color = ltComp->Ambient;
}

void __fastcall TSHSceneGraphDlg::LabeledEditLightRangeChange(
      TObject *Sender)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    try{
        double r = LabeledEditLightRange->Text.ToDouble();
        // Rangeは0より大きい
        if(r <= 0){
            LabeledEditLightRange->Text = 0.1;
            r = 0.1;
        }
        light->range = r;
    }
    catch(...){}

    scene->NotifyChange();
}

void __fastcall TSHSceneGraphDlg::LabeledEditLightFalloffChange(
      TObject *Sender)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    try{
        double f = LabeledEditLightFalloff->Text.ToDouble();
        // Falloffは0以上1以下
        if(f < 0){
            LabeledEditLightFalloff->Text = 0;
            f = 0;
        }
        else if(f > 1){
            LabeledEditLightFalloff->Text = 1;
            f = 1;
        }
        light->spotFalloff = f;
    }
    catch(...){}

    scene->NotifyChange();
}

void __fastcall TSHSceneGraphDlg::StringGridLightAttenuationSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    double a;
    try{
        a = Value.ToDouble();
        // Attenuationは0以上
        if(a < 0){
            StringGridLightAttenuation->Cells[ACol][ARow] = 0;
            a = 0;
        }
        // ただし，全部0でもダメ
        if(StringGridLightAttenuation->Cells[0][0].ToDouble() == StringGridLightAttenuation->Cells[1][0].ToDouble() == StringGridLightAttenuation->Cells[2][0].ToDouble() == 0){
            a = 1;
            return;
        }
    }
    catch(...){}

	if(ACol == 0) light->attenuation0 = a;
	else if(ACol == 1) light->attenuation1 = a;
    else if(ACol == 2) light->attenuation2 = a;

    scene->NotifyChange();
}

void __fastcall TSHSceneGraphDlg::LabeledEditLightThetaChange(
      TObject *Sender)
{
    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    try{
        double t = LabeledEditLightTheta->Text.ToDouble();
        // Thetaは0以上Phi以下
        if(t < 0){
            LabeledEditLightTheta->Text = 0;
            t = 0;
        }
		else if(t > light->spotCutoff){
			LabeledEditLightTheta->Text = light->spotCutoff;
			t = light->spotCutoff;
		}
        light->spotInner = t;
    }
    catch(...){}

    scene->NotifyChange();
}

void __fastcall TSHSceneGraphDlg::LabeledEditLightPhiChange(
      TObject *Sender)
{
    const float PI = 3.1415926535;

    GRLight* light = DCAST(GRLight, Tree->Selected->Object);
    if(!light) return;

    try{
        double p = LabeledEditLightPhi->Text.ToDouble();
        // PhiはTheta以上π以下
        if(p < light->spotInner){
            LabeledEditLightPhi->Text = light->spotCutoff;
            p = light->spotCutoff;
        }
        else if(p > PI){
            LabeledEditLightPhi->Text = PI;
            p = PI;
        }
        light->spotCutoff = p;
    }
    catch(...){}

    scene->NotifyChange();
}

void __fastcall TSHSceneGraphDlg::PanelLightDiffuseClick(TObject *Sender)
{
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    TSHLight* ltComp = (TSHLight*)comp;

    if(ColorDialogLight->Execute() == true){
        ltComp->Diffuse = ColorDialogLight->Color;
        UpdateLightPanel(ltComp);
    }
    scene->NotifyChange();
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::PanelLightSpecularClick(TObject *Sender)
{
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    TSHLight* ltComp = (TSHLight*)comp;

    if(ColorDialogLight->Execute() == true){
        ltComp->Specular = ColorDialogLight->Color;
        UpdateLightPanel(ltComp);
    }
    scene->NotifyChange();
}
//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::PanelLightAmbientClick(TObject *Sender)
{
    TSHComponent* comp = Tree->GetComponent(Tree->Selected->Object);
    TSHLight* ltComp = (TSHLight*)comp;

    if(ColorDialogLight->Execute() == true){
        ltComp->Ambient = ColorDialogLight->Color;
        UpdateLightPanel(ltComp);
    }
    scene->NotifyChange();
}

//***********************************************************
//********* PHSolid *****************************************
//***********************************************************

void TSHSceneGraphDlg::UpdateSolidPanel(TSHComponent* comp)
{
    TSHSolid* soComp = (TSHSolid*)comp;

    LabeledEditSolidMass->Text = soComp->Mass;
    LabeledEditSolidMassMin->Text = 0.1;
    LabeledEditSolidMassMax->Text = soComp->Mass * 3;
    UpdateTrackBarSolidMass(soComp->Mass);


    TSHMatrix3f inertia = soComp->Inertia;
    StringGridSolidInertia->Cells[0][0] = inertia.Ex.X;
    StringGridSolidInertia->Cells[0][1] = inertia.Ex.Y;
    StringGridSolidInertia->Cells[0][2] = inertia.Ex.Z;
    StringGridSolidInertia->Cells[1][0] = inertia.Ey.X;
    StringGridSolidInertia->Cells[1][1] = inertia.Ey.Y;
    StringGridSolidInertia->Cells[1][2] = inertia.Ey.Z;
    StringGridSolidInertia->Cells[2][0] = inertia.Ez.X;
    StringGridSolidInertia->Cells[2][1] = inertia.Ez.Y;
    StringGridSolidInertia->Cells[2][2] = inertia.Ez.Z;

    StringGridSolidCenter->Cells[0][0] = soComp->Center.X;
    StringGridSolidCenter->Cells[1][0] = soComp->Center.Y;
    StringGridSolidCenter->Cells[2][0] = soComp->Center.Z;

    StringGridSolidVelocity->Cells[0][0] = "Current";
    StringGridSolidVelocity->Cells[0][1] = "Set";
    StringGridSolidVelocity->Cells[1][1] = 0.0;
    StringGridSolidVelocity->Cells[2][1] = 0.0;
    StringGridSolidVelocity->Cells[3][1] = 0.0;
    StringGridSolidVelocity->Row = 1;
    StringGridSolidVelocity->Col = 1;

    StringGridSolidAngularVelocity->Cells[0][0] = "Current";
    StringGridSolidAngularVelocity->Cells[0][1] = "Set";
    StringGridSolidAngularVelocity->Cells[1][1] = 0.0;
    StringGridSolidAngularVelocity->Cells[2][1] = 0.0;
    StringGridSolidAngularVelocity->Cells[3][1] = 0.0;
    StringGridSolidAngularVelocity->Row = 1;
    StringGridSolidAngularVelocity->Col = 1;

}

//------ Mass -------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::LabeledEditSolidMassChange(
      TObject *Sender)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;

    double m;
    try{
        m = LabeledEditSolidMass->Text.ToDouble();
    }
    catch(...){}
    UpdateSolidMass(m);
    UpdateTrackBarSolidMass(m);
    scene->NotifyChange();
}

void TSHSceneGraphDlg::UpdateSolidMass(double m){
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;

    // Massは0.1以上
    if(m < 0.1){
        LabeledEditSolidMass->Text = 0.1;
        m = 0.1;
    } else {
        LabeledEditSolidMass->Text = m;
    }
    solid->SetMass(m);
}

void __fastcall TSHSceneGraphDlg::TrackBarSolidMassChange(TObject *Sender)
{
    double mass, min, max;
    try{
        min = LabeledEditSolidMassMin->Text.ToDouble();
        max = LabeledEditSolidMassMax->Text.ToDouble();
        mass = min + (max-min)/(TrackBarSolidMass->Max - TrackBarSolidMass->Min) * (TrackBarSolidMass->Position - TrackBarSolidMass->Min);
    }
    catch(...){}
    UpdateSolidMass(mass);
    scene->NotifyChange();
}

// TrackBarのポジションを現在の質量に合わせる
void TSHSceneGraphDlg::UpdateTrackBarSolidMass(double m){
    double pos, min, max;
    try{
        min = LabeledEditSolidMassMin->Text.ToDouble();
        max = LabeledEditSolidMassMax->Text.ToDouble();
        // 質量がMinより小さい，Maxより大きい場合はMin,Maxを修正
        if (m < min) {
            min = m;
            LabeledEditSolidMassMin->Text = m;
        } else if (m > max) {
            max = m;
            LabeledEditSolidMassMax->Text = m;
        }
        // Min=Max の場合はトラックバーのポジションをとりあえず真中にセット
        if (min == max) {
            pos = (TrackBarSolidMass->Max - TrackBarSolidMass->Min)/2;
        } else {
            pos = (m - min) * (TrackBarSolidMass->Max - TrackBarSolidMass->Min)/(max-min) + TrackBarSolidMass->Min;
        }
    }
    catch(...){}
    TrackBarSolidMass->Position = (int)pos;
}

void __fastcall TSHSceneGraphDlg::LabeledEditSolidMassMinChange(
      TObject *Sender)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;
    try{
        double min = LabeledEditSolidMassMin->Text.ToDouble();
        // Minは0.1以上Max以下
        if(min < 0.1){
            LabeledEditSolidMassMin->Text = 0.1;
        } else if ( min > LabeledEditSolidMassMax->Text.ToDouble() ){
            LabeledEditSolidMassMin->Text = LabeledEditSolidMassMax->Text;
        }
    }
    catch(...){}
    UpdateTrackBarSolidMass(solid->GetMass());
}
void __fastcall TSHSceneGraphDlg::LabeledEditSolidMassMaxChange(TObject *Sender)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;
    try{
        double max = LabeledEditSolidMassMax->Text.ToDouble();
        // MaxはMin以上
        if ( max < LabeledEditSolidMassMin->Text.ToDouble() ){
            LabeledEditSolidMassMax->Text = LabeledEditSolidMassMin->Text;
        }
    }
    catch(...){}
    UpdateTrackBarSolidMass(solid->GetMass());
}

//------ Inertia ------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::StringGridSolidInertiaSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;

    Matrix3f inertia = solid->GetInertia();
    Vec3f vec = inertia[ACol];
    try{
        vec[ARow] = Value.ToDouble();
    }
    catch(...){}
    //軸に対するノルムは0より大きい
    if(vec.norm() > 1.0e-10){
        inertia[ACol] = vec;
        solid->SetInertia(inertia);
    } else {
        StringGridSolidInertia->Cells[ACol][ARow] = inertia[ARow][ACol];
    }
    scene->NotifyChange();
}
//------ Center ---------------------------------------------------------

void __fastcall TSHSceneGraphDlg::StringGridSolidCenterSetEditText(
      TObject *Sender, int ACol, int ARow, const AnsiString Value)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;

    double v;
    try{
        v = Value.ToDouble();
    }
    catch(...){}

    Vec3d center = solid->GetCenter();
    if(ACol == 0) center.X() = v;
    else if(ACol == 1) center.Y() = v;
    else if(ACol == 2) center.Z() = v;
    solid->SetCenter(center);

    scene->NotifyChange();
}
//------ Velocity, AngularVelocity ------------------------------------------

// Velocity, AngularVelocityをセット
void __fastcall TSHSceneGraphDlg::ButtonSolidSetClick(TObject *Sender)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;

    SHVec3f vel = solid->GetVelocity();
    SHVec3f angVel = solid->GetAngularVelocity();

    try{
        vel.SetX(StrToFloat(StringGridSolidVelocity->Cells[1][1]));
        vel.SetY(StrToFloat(StringGridSolidVelocity->Cells[2][1]));
        vel.SetZ(StrToFloat(StringGridSolidVelocity->Cells[3][1]));
        angVel.SetX(StrToFloat(StringGridSolidAngularVelocity->Cells[1][1]));
        angVel.SetY(StrToFloat(StringGridSolidAngularVelocity->Cells[2][1]));
        angVel.SetZ(StrToFloat(StringGridSolidAngularVelocity->Cells[3][1]));
    }
    catch(...){}

    solid->SetVelocity(vel);
    solid->SetAngularVelocity(angVel);
    scene->NotifyChange();
}

// Velocity, AngularVelocityの変化を表示
void __fastcall TSHSceneGraphDlg::TimerSolidObserverTimer(TObject *Sender)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;

    StringGridSolidVelocity->Cells[1][0] = solid->GetVelocity().X();
    StringGridSolidVelocity->Cells[2][0] = solid->GetVelocity().Y();
    StringGridSolidVelocity->Cells[3][0] = solid->GetVelocity().Z();
    StringGridSolidAngularVelocity->Cells[1][0] = solid->GetAngularVelocity().X();
    StringGridSolidAngularVelocity->Cells[2][0] = solid->GetAngularVelocity().Y();
    StringGridSolidAngularVelocity->Cells[3][0] = solid->GetAngularVelocity().Z();
}

void __fastcall TSHSceneGraphDlg::StringGridSolidVelocitySelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    if(ARow == 0) CanSelect = false;
    else CanSelect = true;
}

//---------------------------------------------------------------------------

// 対応frameへジャンプ
void __fastcall TSHSceneGraphDlg::ButtonSolidToFrameClick(TObject *Sender)
{
    PHSolid* solid = DCAST(PHSolid, Tree->Selected->Object);
    if(!solid) return;

    SGFrame* frame = solid->GetFrame();

	for(int i= 0; i<Tree->Items->Count; ++i){
		TSHTreeNode* node = (TSHTreeNode*)Tree->Items->Item[i];
		if (node->Object == (SGObject*)frame){
            Tree->Selected = node;
            return;
		}
	}
}


//***********************************************************
//******* PHPhysicalMaterial ********************************
//***********************************************************

void TSHSceneGraphDlg::UpdatePHMaterialPanel(TSHComponent* comp)
{
    TSHPhysicalMaterial* pmComp = (TSHPhysicalMaterial*)comp;

    LabeledEditPHMaterialReflexSpring->Text = pmComp->ReflexSpring;
    LabeledEditPHMaterialReflexDamper->Text = pmComp->ReflexDamper;
    LabeledEditPHMaterialFrictionSpring->Text = pmComp->FrictionSpring;
    LabeledEditPHMaterialFrictionDamper->Text = pmComp->FrictionDamper;
    LabeledEditPHMaterialStaticFriction->Text = pmComp->StaticFriction;
    LabeledEditPHMaterialDynamicFriction->Text = pmComp->DynamicFriction;
    UpdatePHMaterialTrackBar(pmComp->ReflexSpring, 0);
    UpdatePHMaterialTrackBar(pmComp->ReflexDamper, 1);
    UpdatePHMaterialTrackBar(pmComp->FrictionSpring, 2);
    UpdatePHMaterialTrackBar(pmComp->FrictionDamper, 3);
    UpdatePHMaterialTrackBar(pmComp->StaticFriction, 4);
    UpdatePHMaterialTrackBar(pmComp->DynamicFriction, 5);
}

void TSHSceneGraphDlg::UpdatePHMaterial(float value, int property){
    PHPhysicalMaterial* pmat = DCAST(PHPhysicalMaterial, Tree->Selected->Object);
    if(!pmat) return;

    switch(property){
        case 0:
            pmat->pMatData.reflexSpring = value;
            break;
        case 1:
            pmat->pMatData.reflexDamper = value;
            break;
        case 2:
            pmat->pMatData.frictionSpring = value;
            break;
        case 3:
            pmat->pMatData.frictionDamper = value;
            break;
        case 4:
            pmat->pMatData.staticFriction = value;
            break;
        case 5:
            pmat->pMatData.dynamicFriction = value;
            break;
    }
    scene->NotifyChange();
}


void __fastcall TSHSceneGraphDlg::TrackBarPHMaterialChange(
      TObject *Sender)
{
    TTrackBar* barSender = (TTrackBar*)Sender;
    float power, value, pos, min, max, maxPower;
    pos = barSender->Position;
    min = barSender->Min;
    max = barSender->Max;
    maxPower = 4.0f;
    try{
        // TrackBarのPositionを Min～Max で動かすとpowerは -maxPower～maxPower の間で動く
        if(pos != min) {
            power = ((pos-(max+min)/2) / ((max-min)/(maxPower*2)));
            value = pow(10, power);
        } else value = 0.0f;
    }
    catch(...){}

    int property;
    if(barSender == TrackBarPHMaterialReflexSpring){
        property = 0;
    } else if(barSender == TrackBarPHMaterialReflexDamper){
        property = 1;
    } else if(barSender == TrackBarPHMaterialFrictionSpring){
        property = 2;
    } else if(barSender == TrackBarPHMaterialFrictionDamper){
        property = 3;
    } else if(barSender == TrackBarPHMaterialStaticFriction){
        property = 4;
        value *= 0.6f;
    } else if(barSender == TrackBarPHMaterialDynamicFriction){
        property = 5;
        value *= 0.3f;
    }

    // ラベル付きエディットボックスの値をトラックバーに合わせる
    UpdatePHMaterialLabeledEdit(value, property);
    // 結果を世界に反映
    UpdatePHMaterial(value, property);
}

// ラベル付きエディットボックスの値を設定する
void TSHSceneGraphDlg::UpdatePHMaterialLabeledEdit(float value, int property){
    switch(property){
        case 0:
            LabeledEditPHMaterialReflexSpring->Text = value;
            break;
        case 1:
            LabeledEditPHMaterialReflexDamper->Text = value;
            break;
        case 2:
            LabeledEditPHMaterialFrictionSpring->Text = value;
            break;
        case 3:
            LabeledEditPHMaterialFrictionDamper->Text = value;
            break;
        case 4:
            LabeledEditPHMaterialStaticFriction->Text = value;
            break;
        case 5:
            LabeledEditPHMaterialDynamicFriction->Text = value;
            break;
    }
}

//---------------------------------------------------------------------------

void __fastcall TSHSceneGraphDlg::LabeledEditPHMaterialChange(
      TObject *Sender)
{
    TLabeledEdit* LEditSender = (TLabeledEdit*)Sender;

    float value;
    try{
        value = StrToFloat(LEditSender->Text);
        // valueは0以上
        if(value < 0) {
            value = 0.0f;
            LEditSender->Text = 0;
        }
    }
    catch(...){}

    int property;
    if(LEditSender == LabeledEditPHMaterialReflexSpring){
        property = 0;
    } else if(LEditSender == LabeledEditPHMaterialReflexDamper){
        property = 1;
    } else if(LEditSender == LabeledEditPHMaterialFrictionSpring){
        property = 2;
    } else if(LEditSender == LabeledEditPHMaterialFrictionDamper){
        property = 3;
    } else if(LEditSender == LabeledEditPHMaterialStaticFriction){
        property = 4;
    } else if(LEditSender == LabeledEditPHMaterialDynamicFriction){
        property = 5;
    }

    // トラックバーをラベル付きエディットボックスの値に合わせる
    UpdatePHMaterialTrackBar(value, property);
    // 結果を世界に反映
    UpdatePHMaterial(value, property);

}

// トラックバーの位置を合わせる
void TSHSceneGraphDlg::UpdatePHMaterialTrackBar(float value, int property){

    float maxPower = 4.0f;
    float basis = 1.0f;       // トラックバーが中心のときの値
    TTrackBar* tBar;          // 編集するトラックバー
    switch(property){
        case 0:
            tBar = TrackBarPHMaterialReflexSpring;
            break;
        case 1:
            tBar = TrackBarPHMaterialReflexDamper;
            break;
        case 2:
            tBar = TrackBarPHMaterialFrictionSpring;
            break;
        case 3:
            tBar = TrackBarPHMaterialFrictionDamper;
            break;
        case 4:
            tBar = TrackBarPHMaterialStaticFriction;
            basis = 0.6f;
            break;
        case 5:
            tBar = TrackBarPHMaterialDynamicFriction;
            basis = 0.3f;
            break;
    }

    float pos;
    pos = (log10(value/basis))*(tBar->Max-tBar->Min)/(maxPower*2) + (tBar->Max+tBar->Min)/2;
    pos = (int)pos;
    if(pos < tBar->Min) pos = tBar->Min;
    else if(pos > tBar->Max) pos = tBar->Max;
    tBar->Position = pos;
}

//---------------------------------------------------------------------------


