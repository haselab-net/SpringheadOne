//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "SHSceneGraphDlg.h"

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHBox"
#pragma link "SHFrame"
#pragma link "SHScene"
#pragma link "SHSceneGraphDlg"
#pragma link "SHComponent"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
TSHSceneGraphDlg* dlg;

void __fastcall TForm1::btnTreeClick(TObject *Sender)
{
	if(!dlg) dlg = new TSHSceneGraphDlg(this, SHScene1);
    dlg->Show();
}
//---------------------------------------------------------------------------
static bool bRun;
void __fastcall TForm1::btnStepClick(TObject *Sender)
{
    SHScene1->Step();
    SHScene1->NotifyChange();
    bRun = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
    if (bRun){
        for(int i=0; i<10;++i) SHScene1->Step();
        SHScene1->NotifyChange();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnRunClick(TObject *Sender)
{
    bRun = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnResetClick(TObject *Sender)
{
    SHScene1->Reset();
    bRun = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnLoadClick(TObject *Sender)
{
    if(OpenDialog1->Execute()){
        SHScene1->Load(OpenDialog1->FileName.c_str());
        SHScene1->Reset();
        bRun = false;
	}
}
//---------------------------------------------------------------------------

