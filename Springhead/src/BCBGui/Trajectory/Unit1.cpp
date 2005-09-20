//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <fstream>

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SHBox"
#pragma link "SHComponent"
#pragma link "SHScene"
#pragma link "SHPathGraph"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	path = new Spr::PHControlPath(this);
	Spr::PHJointEngine* e=NULL;
	SHScene1->Imp->GetBehaviors().Find(e);
	path->Init(e->root);
}
//---------------------------------------------------------------------------
bool bRun;
void __fastcall TForm1::btStepClick(TObject *Sender)
{
	bRun = false;
	SHScene1->Step();
	path->Step(0.005f);
	SHBox1->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (bRun){
		for(int i=0; i<3; ++i){
			SHScene1->Step();
			path->Step(0.005f);
		}
		static int count;
		count ++;
		if (count > 4){
			count = 0;
            SHBox1->Invalidate();
            SHBox2->Invalidate();
		}
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::btRunClick(TObject *Sender)
{
    bRun = true;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::btLoadClick(TObject *Sender)
{
	std::ifstream is("data.txt");
	path->Input(is);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btSaveClick(TObject *Sender)
{
	std::ofstream os("data.txt");
	path->Print(os);
}
//---------------------------------------------------------------------------

