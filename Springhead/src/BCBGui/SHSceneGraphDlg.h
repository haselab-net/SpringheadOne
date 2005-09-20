//---------------------------------------------------------------------------

#ifndef TSHSceneGraphDlgH
#define TSHSceneGraphDlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <DesignIntf.hpp>
#include <DesignEditors.hpp>
#include "SHTreeView.h"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Grids.hpp>
#include <Base/BaseUtility.h>
#include <ImpD3D/D3Mesh.h>
#include <Graphics/GRLight.h>

namespace Spr{
	class SGFrame;
}
class __declspec(delphiclass) TSHScene;
class __declspec(delphiclass) TSHFrame;
class __declspec(delphiclass) TSHLight;
//---------------------------------------------------------------------------
class PACKAGE TSHSceneGraphDlg : public TForm
{
protected:
	_di_IDesigner designer;
	_di_IDesigner __fastcall GetDesigner();
	TSHScene* scene;
	int lastX;
	int lastY;

	TSHLight* GetTmpLight();

__published:	// IDE 管理のコンポーネント
	TPopupMenu *TreeMenu;
	TMenuItem *Insert;
	TMenuItem *Delete;
    TMenuItem *Import;
    TMenuItem *Export;
	TOpenDialog *OpenDlg;
	TSaveDialog *SaveDlg;
	TMenuItem *Component;
	TMenuItem *AddLight;
    TPanel *FramePanel;
    TBevel *BevelFrameViewCtrl;
    TImageList *ImageListForTree;
    TLabel *LabelFrameState;
    TLabel *LabelFrameRot;
    TLabel *LabelFrameTrans;
    TLabel *LabelFrameViewCtrl;
    TLabel *LabelFrameAffine;
    TButton *ButtonFrameX;
    TButton *ButtonFrameY;
    TButton *ButtonFrameZ;
    TButton *ButtonFrameXY;
    TButton *ButtonFrameYZ;
    TButton *ButtonFrameZX;
    TButton *ButtonFrameRotX;
    TButton *ButtonFrameRotY;
    TButton *ButtonFrameRotZ;
    TImage *ImageFrameViewTrans;
    TImage *ImageFrameViewRotate;
    TImage *ImageFrameViewZoom;
    TStringGrid *StringGridFrameAffine;
    TLabeledEdit *LabeledEditFrameRotX;
    TLabeledEdit *LabeledEditFrameRotY;
    TLabeledEdit *LabeledEditFrameRotZ;
    TLabel *LabelFrameScale;
    TStringGrid *StringGridFrameScale;
    TButton *ButtonFrameInitRot;
    TPanel *LightPanel;
    TLabeledEdit *LabeledEditLightRange;
    TLabeledEdit *LabeledEditLightFalloff;
    TLabeledEdit *LabeledEditLightTheta;
    TStringGrid *StringGridLightAmbient;
    TLabel *LabelLightAmbient;
    TStringGrid *StringGridLightSpecular;
    TLabel *LabelLightSpecular;
    TStringGrid *StringGridLightDiffuse;
    TLabel *LabelLightDiffuse;
    TRadioButton *RadioButtonLightPoint;
    TRadioButton *RadioButtonLightSpot;
    TRadioButton *RadioButtonLightDirectional;
    TLabel *LabelLightType;
    TLabeledEdit *LabeledEditLightPhi;
    TStringGrid *StringGridLightAttenuation;
    TLabel *LabelLightAttenuation;
    TSplitter *Splitter2;
    TSHTreeView *Tree;
    TPanel *ParentPanel;
    TButton *ButtonFrameMeshCtrlPos;
    TButton *ButtonFrameMeshCtrlRot;
    TButton *ButtonFrameMeshCtrlScale;
    TCheckBox *CheckBoxFrameMeshCheck;
    TBevel *BevelFrameInit;
    TLabel *LabelFrameInit;
    TColorDialog *ColorDialogLight;
    TPanel *PanelLightSpecular;
    TPanel *PanelLightDiffuse;
    TPanel *PanelLightAmbient;
    TPanel *SolidPanel;
    TLabeledEdit *LabeledEditSolidMass;
    TLabel *LabelSolidVelocity;
    TButton *ButtonSolidSet;
    TStringGrid *StringGridSolidVelocity;
    TStringGrid *StringGridSolidAngularVelocity;
    TLabel *LabelSolidAngVel1;
    TLabel *LabelSolidAngVel2;
    TStringGrid *StringGridSolidCenter;
    TStringGrid *StringGridSolidInertia;
    TLabel *LabelSolidInertia;
    TLabel *LabelSolidCenter;
    TTimer *TimerSolidObserver;
    TButton *ButtonSolidToFrame;
    TTrackBar *TrackBarSolidMass;
    TLabeledEdit *LabeledEditSolidMassMin;
    TLabeledEdit *LabeledEditSolidMassMax;
    TGroupBox *GroupBoxSolidMass;
    TPanel *PHMaterialPanel;
    TTrackBar *TrackBarPHMaterialReflexSpring;
    TLabeledEdit *LabeledEditPHMaterialReflexSpring;
    TTrackBar *TrackBarPHMaterialReflexDamper;
    TLabeledEdit *LabeledEditPHMaterialReflexDamper;
    TTrackBar *TrackBarPHMaterialFrictionSpring;
    TLabeledEdit *LabeledEditPHMaterialFrictionSpring;
    TTrackBar *TrackBarPHMaterialFrictionDamper;
    TLabeledEdit *LabeledEditPHMaterialFrictionDamper;
    TTrackBar *TrackBarPHMaterialStaticFriction;
    TLabeledEdit *LabeledEditPHMaterialStaticFriction;
    TTrackBar *TrackBarPHMaterialDynamicFriction;
    TLabeledEdit *LabeledEditPHMaterialDynamicFriction;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
	void __fastcall InsertClick(TObject *Sender);
    void __fastcall DeleteClick(TObject *Sender);
	void __fastcall ImportClick(TObject *Sender);
	void __fastcall ExportClick(TObject *Sender);
	void __fastcall ComponentClick(TObject *Sender);
    void __fastcall ButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ButtonMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
    void __fastcall ButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ImageViewMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ImageViewMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ImageViewMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall AddLightClick(TObject *Sender);
	void __fastcall TreeChange(TObject *Sender, TTreeNode *Node);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall StringGridFrameAffineSetEditText(TObject *Sender, int ACol, int ARow, const AnsiString Value);
    void __fastcall LabeledEditRotEnter(TObject *Sender);
    void __fastcall LabeledEditRotExit(TObject *Sender);
    void __fastcall LabeledEditRotChange(TObject *Sender);
    void __fastcall LabeledEditRotMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall ButtonFrameInitRotClick(TObject *Sender);
    void __fastcall StringGridFrameScaleSetEditText(TObject *Sender, int ACol, int ARow, const AnsiString Value);
    void __fastcall ButtonMeshCtrlClick(TObject *Sender);
    void __fastcall TreeKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall RadioButtonLightPointClick(TObject *Sender);
    void __fastcall RadioButtonLightSpotClick(TObject *Sender);
    void __fastcall RadioButtonLightDirectionalClick(TObject *Sender);
    void __fastcall StringGridLightDiffuseSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
    void __fastcall StringGridLightSpecularSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
    void __fastcall StringGridLightAmbientSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
    void __fastcall LabeledEditLightRangeChange(TObject *Sender);
    void __fastcall LabeledEditLightFalloffChange(TObject *Sender);
    void __fastcall StringGridLightAttenuationSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
    void __fastcall LabeledEditLightThetaChange(TObject *Sender);
    void __fastcall LabeledEditLightPhiChange(TObject *Sender);
    void __fastcall PanelLightDiffuseClick(TObject *Sender);
    void __fastcall PanelLightSpecularClick(TObject *Sender);
    void __fastcall PanelLightAmbientClick(TObject *Sender);
    void __fastcall StringGridDrawCell(TObject *Sender,
          int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall LabeledEditSolidMassChange(TObject *Sender);
    void __fastcall ButtonSolidSetClick(TObject *Sender);
    void __fastcall StringGridSolidCenterSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
    void __fastcall TimerSolidObserverTimer(TObject *Sender);
    void __fastcall TrackBarSolidMassChange(TObject *Sender);
    void __fastcall LabeledEditSolidMassMinChange(TObject *Sender);
    void __fastcall LabeledEditSolidMassMaxChange(TObject *Sender);
    void __fastcall ButtonSolidToFrameClick(TObject *Sender);
    void __fastcall StringGridSolidInertiaSetEditText(TObject *Sender,
          int ACol, int ARow, const AnsiString Value);
    void __fastcall StringGridSolidVelocitySelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect);
    void __fastcall TrackBarPHMaterialChange(TObject *Sender);
    void __fastcall LabeledEditPHMaterialChange(
          TObject *Sender);
//////	void __fastcall ObjectViewSelectItem(TObject *Sender, TListItem *Item, bool Selected);
//////	void __fastcall ObjectViewDeleteClick(TObject *Sender);
//////	void __fastcall ObjectViewComponentClick(TObject *Sender);
private:	// ユーザー宣言
	TSHLight* tmpLight;

	void SelectComponent(TPersistent* comp);
    void UpdateStringGridAffine(Spr::Affinef afNow);
    void UpdateEuler(Spr::Affinef afNow);
    void UpdateStringGridScale(Spr::Affinef afNow);
    void UpdateLightPanel(TSHComponent* comp);
    void UpdateSolidPanel(TSHComponent* comp);
    void UpdateSolidMass(double m);
    void UpdateTrackBarSolidMass(double m);
    void UpdatePHMaterialPanel(TSHComponent* comp);
    void UpdatePHMaterialLabeledEdit(float value, int property);
    void UpdatePHMaterialTrackBar(float value, int property);
    void UpdatePHMaterial(float value, int property);

    protected:
public:		// ユーザー宣言
	__fastcall TSHSceneGraphDlg(TComponent* Owner, TSHScene* s);
	__fastcall ~TSHSceneGraphDlg();
	/*オブジェクトビューを更新。
		ライトなどの追加・削除・名前変更時に呼び出す事。*/
	void UpdateObjectView();
	//オブジェクト名の変更を伝える
	void NotifyChangeName(Spr::SGObject* o);

	__property _di_IDesigner Designer={read = GetDesigner};
};
/*
class TSHSceneGetDesigner: public TComponentProperty{
public:
	__fastcall TSHSceneGetDesigner(const _di_IDesigner ADesigner, int APropCount):
		TComponentProperty(ADesigner, APropCount){
		designer = (IDesigner*)Designer;
	}
	static _di_IDesigner designer;
};
*/
//---------------------------------------------------------------------------
extern PACKAGE TSHSceneGraphDlg *SceneGraphDlg;
//---------------------------------------------------------------------------
#endif
