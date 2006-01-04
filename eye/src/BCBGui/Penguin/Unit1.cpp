//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SHSceneGraphDlg.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHFrame"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
float delta=0.02;
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Penguin->Posture->Pos->X += delta;
//	Penguin->Imp->Print(DSTR);
//	DSTR << Penguin->Posture->Pos->X << std::endl;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btLeftClick(TObject *Sender)
{
	delta = -0.02f;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btStopClick(TObject *Sender)
{
	delta = 0.0f;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::btRightClick(TObject *Sender)
{
	delta = 0.02f;
}
//---------------------------------------------------------------------------

TSHSceneGraphDlg* dlg;

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	if (!dlg) dlg = new TSHSceneGraphDlg(this, Scene);
	dlg->Show();
}
//---------------------------------------------------------------------------

