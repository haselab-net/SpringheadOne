//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	CoInitialize(NULL);
	dshow = new KtDShowBasic();
//	dshow->SetOutputWindow( Handle );
	dshow->SetOutputWindow( NULL );
	dshow->SetAddToRot( true );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	delete dshow;
	CoUninitialize();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenO1Click(TObject *Sender)
{
	if( OpenDialog1->Execute() ){
		dshow->SetInputFile( OpenDialog1->FileName.c_str() );
		dshow->Render();
		if( dshow->HaveVideoData() ){
			ClientWidth = dshow->GetVideoWidth();
			ClientHeight = dshow->GetVideoHeight();
		}
		dshow->Run();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Stop1Click(TObject *Sender)
{
	dshow->Stop();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
	dshow->SetDrawPosition();
}
//---------------------------------------------------------------------------

