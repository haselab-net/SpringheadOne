//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "scrgl.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TTimer *Timer1;
    TCheckBox *x3dfx;
    TCheckBox *xIgnoreBpp;
        TListBox *ModeList;
        TMemo *Log;
        TSglPanel *SglPanel;
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall SglPanelGLInit(TObject *Sender);
    void __fastcall SglPanelGLPaint(TObject *Sender);
    void __fastcall x3dfxClick(TObject *Sender);
    void __fastcall xIgnoreBppClick(TObject *Sender);
        void __fastcall SglPanelGLError(TObject *Sender, AnsiString err);
        void __fastcall ModeListClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
    __fastcall ~TMainForm();
    void __fastcall GenerateModeList();
    void __fastcall PrepareProjection();
    double RotX,RotY,RotZ;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
