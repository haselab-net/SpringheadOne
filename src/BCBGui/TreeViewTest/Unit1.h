//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SHBox.h"
#include "SHFrame.h"
#include "SHScene.h"
#include <ExtCtrls.hpp>
#include "SHComponent.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
    TSHBox *SHBox1;
    TButton *btnTree;
    TSHScene *SHScene1;
    TTimer *Timer1;
    TButton *btnStep;
    TButton *btnRun;
    TButton *btnReset;
    TButton *btnLoad;
    TOpenDialog *OpenDialog1;
    void __fastcall btnTreeClick(TObject *Sender);
    void __fastcall btnStepClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall btnRunClick(TObject *Sender);
    void __fastcall btnResetClick(TObject *Sender);
    void __fastcall btnLoadClick(TObject *Sender);
private:	// ユーザー宣言
public:		// ユーザー宣言
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 