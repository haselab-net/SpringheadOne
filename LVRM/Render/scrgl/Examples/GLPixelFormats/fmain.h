//---------------------------------------------------------------------------
#ifndef fmainH
#define fmainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------


class TMainForm : public TForm
{
__published:	// IDE-managed Components
    TListBox *ListBox;
    TPanel *Frame;
    TTimer *Timer;
    TButton *btnList;
    TButton *btnCreate;
    TButton *btnDestroy;
    void __fastcall btnListClick(TObject *Sender);

    void __fastcall btnCreateClick(TObject *Sender);
    void __fastcall TimerTimer(TObject *Sender);
    void __fastcall btnDestroyClick(TObject *Sender);
    void __fastcall ListBoxClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TMainForm(TComponent* Owner);
    __fastcall ~TMainForm();
    double RotX, RotY, RotZ;
    void __fastcall PrepareProjection();
    void __fastcall DrawGL();
    void __fastcall InitGL();
    HDC hdc;
    TWinControl *render;
    void __fastcall WndProc(TMessage &msg);
    void __fastcall PanelMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall RenderMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
