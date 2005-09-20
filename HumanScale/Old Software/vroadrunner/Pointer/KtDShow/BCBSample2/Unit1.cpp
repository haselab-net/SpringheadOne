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
	dshow = new KtDShowCapture();
	dshow->SetOutputWindow( NULL );
	dshow->SetAddToRot( true );

	Graphics::TBitmap *imagebm = new Graphics::TBitmap();
	imagebm->PixelFormat = pf24bit;
	imagebm->HandleType = bmDIB;
	imagebm->Width = ClientWidth;
	imagebm->Height = ClientHeight;
	Image1->Picture->Graphic = imagebm;
	delete imagebm;
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
	dshow->FindCaptureDevices();
	if( dshow->GetNumOfCaptureDevices() ){
		Caption = dshow->GetCaptureDevice(0)->GetName();
	}else{
		Caption = "NoCaptureDevice";
		return;
	}
	dshow->Render();
	if( dshow->HaveVideoData() ){
		ClientWidth = dshow->GetVideoWidth();
		ClientHeight = dshow->GetVideoHeight();
	}
	dshow->Run();
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
	// Imageのサイズが変わっても、
	// 保持しているBitmapのサイズは変わらないようなので変える。
	Image1->Picture->Bitmap->Width = ClientWidth;
	Image1->Picture->Bitmap->Height = ClientHeight;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::CaptureC1Click(TObject *Sender)
{
	BYTE *image = NULL;
//	dshow->GetCurrentBuffer();
	HBITMAP bitmap = dshow->GetCurrentBitmap();

	long width = dshow->GetVideoWidth();
	long height = dshow->GetVideoHeight();

	HDC hdcSrc = CreateCompatibleDC( NULL );
	HGDIOBJ hobj;
	hobj = SelectObject( hdcSrc,bitmap );
	BitBlt( Form1->Image1->Canvas->Handle,0,0,width,height,hdcSrc,0,0,SRCCOPY );
	Image1->Repaint();

	// ビットマップを解放
	SelectObject( hdcSrc,hobj );
	DeleteDC( hdcSrc );

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveF1Click(TObject *Sender)
{
	if( SavePictureDialog1->Execute() ){
		Image1->Picture->Bitmap->SaveToFile( SavePictureDialog1->FileName );
	}
}
//---------------------------------------------------------------------------

