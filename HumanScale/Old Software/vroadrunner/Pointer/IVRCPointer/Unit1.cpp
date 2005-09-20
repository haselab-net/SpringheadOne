//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <math.h>
#include <IniFiles.hpp>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	Dshow = NULL;
	Calculating = false;
	IsMouseMove = false;
	IsPreviewWindow = false;
	IsPositionTransform = false;
	IsCalibrateMatrixMode = false;
	PreviewPerFrame = 10;
	NumOfScreenApex = 0;
	OffsetX = OffsetY = 0;
	MatrixA = MatrixB = MatrixC = MatrixD = 0;
	ScreenWidth = 1024;
	ScreenHeight = 768;
	IsCaptured = false;
	CapX = 0;
	CapY = 0;
	ScreenX = 0;
	ScreenY = 0;
	RMin = 0x00;
	GMax = 0xFF;
	BMax = 0xFF;
	XReverse = true;
}
//---------------------------------------------------------------------------

bool TMainForm::CalculatePoint()
{
	BYTE *buffer = Dshow->GetCurrentBuffer();

	if( !buffer ){
//		Application->MessageBox( "image is NULL","",MB_OK );
		return false;
	}

	long width = Dshow->GetVideoWidth();
	long height = Dshow->GetVideoHeight();

	int x,y;
	int r,g,b;

	int sum_x = 0;
	int sum_y = 0;
	int num_of_point = 0;

	for( y=height-1; y>=0; y-- ){
		for( x=0; x<width; x++ ){
			b = *buffer;
			g = *(buffer+1);
			r = *(buffer+2);
			buffer += 3;
			if( r >= RMin && g <= GMax && b <= BMax ){
				sum_x += x;
				sum_y += y;
				num_of_point++;
			}
		}
	}

	if( num_of_point == 0 ){
		IsCaptured = false;
		return false;
	}

	IsCaptured = true;
	CapX = sum_x / (double)num_of_point;
	CapY = sum_y / (double)num_of_point;

	ScreenX = ( MatrixA * ( CapX - OffsetX ) + MatrixB * ( CapY - OffsetY ) )
		* ScreenWidth;
	ScreenY = ( MatrixC * ( CapX - OffsetX ) + MatrixD * ( CapY - OffsetY ) )
		* ScreenHeight;

	if( XReverse ){
		ScreenX = ScreenWidth - ScreenX;
	}

	return true;
}

//---------------------------------------------------------------------------
bool TMainForm::CalculatePointWithPreview()
{
	BYTE *buffer = Dshow->GetCurrentBuffer();

	if( !buffer ){
//		Application->MessageBox( "image is NULL","",MB_OK );
		return false;
	}

	long width = Dshow->GetVideoWidth();
	long height = Dshow->GetVideoHeight();

	int x,y;
	int r,g,b;

	int sum_x = 0;
	int sum_y = 0;
	int num_of_point = 0;

	for( y=height-1; y>=0; y-- ){
		for( x=0; x<width; x++ ){
			b = *buffer;
			g = *(buffer+1);
			r = *(buffer+2);
			if( r >= RMin && g <= GMax && b <= BMax ){
				sum_x += x;
				sum_y += y;
				num_of_point++;
			}else{
				r = 0;
				g = 0;
				b = 0;
			}
			PreviewForm->Canvas->Pixels[x][y] = RGB(r,g,b);
			buffer += 3;
		}
	}
//	PreviewForm->Image1->Refresh();
//	Application->ProcessMessages();

	if( num_of_point == 0 ){
		IsCaptured = false;
		return false;
	}

	IsCaptured = true;
	CapX = sum_x / (double)num_of_point;
	CapY = sum_y / (double)num_of_point;

	ScreenX = ( MatrixA * ( CapX - OffsetX ) + MatrixB * ( CapY - OffsetY ) )
		* ScreenWidth;
	ScreenY = ( MatrixC * ( CapX - OffsetX ) + MatrixD * ( CapY - OffsetY ) )
		* ScreenHeight;

	ScreenX = 0;
	ScreenY = 0;
/*
	ScreenX = MatrixA * ( CapX - OffsetX ) / ScreenWidth +
		MatrixB * ( CapY - OffsetY ) / ScreenHeight;
	ScreenY = MatrixC * ( CapX - OffsetX ) / ScreenWidth +
		MatrixD * ( CapY - OffsetY ) / ScreenHeight;
*/
	return true;
}

//---------------------------------------------------------------------------
void TMainForm::MoveMouseToPoint()
{
	if( IsPositionTransform ){
		SetCursorPos( ScreenX,ScreenY );
	}else{
		int WindowX = CapX * ClientWidth / Dshow->GetVideoWidth();
		int WindowY = CapY * (ClientHeight-StatusBar1->Height) / Dshow->GetVideoHeight();
		TPoint WindowPos( WindowX,WindowY );
		TPoint ScreenPos = ClientToScreen( WindowPos );
		SetCursorPos( ScreenPos.x,ScreenPos.y );
	}
}

//---------------------------------------------------------------------------
void TMainForm::WritePointToStatusBar()
{
	AnsiString str;
	char c = IsCaptured ? '*' : ' ';

	if( IsPositionTransform ){
		str.printf( "%cX=%d , Y=%d",c,ScreenX,ScreenY );
	}else{
		str.printf( "%cX=%.2f , Y=%.2f",c,CapX,CapY );
	}
	StatusBar1->Panels->Items[1]->Text = str;
}
//---------------------------------------------------------------------------
bool TMainForm::CalculateMatrix()
{
	int a = ScreenApex[1][0] - ScreenApex[0][0];
	int b = ScreenApex[2][0] - ScreenApex[0][0];
	int c = ScreenApex[1][1] - ScreenApex[0][1];
	int d = ScreenApex[2][1] - ScreenApex[0][1];

	double D;

	D = a * d - b * c;
	if (D == 0){
		return false;
	}

//	ScreenHeight = sqrt(a*a + c*c);
//	ScreenWidth  = sqrt(b*b + d*d);

	MatrixA =   d / D;
	MatrixB = - b / D;
	MatrixC = - c / D;
	MatrixD =   a / D;
	OffsetX = ScreenApex[0][0];
	OffsetY = ScreenApex[0][1];

	SaveMatrix();
	return true;
}
//---------------------------------------------------------------------------
void TMainForm::SaveMatrix()
{
	TStringList *strlist = new TStringList;
	AnsiString str;
	str.printf( "%lg,%lg,%lg,%lg,%lg,%lg",
		OffsetX,OffsetY,MatrixA,MatrixB,MatrixC,MatrixD );
	strlist->Add( str );
	strlist->SaveToFile( "inverse.txt" );
	delete strlist;
}
//---------------------------------------------------------------------------
void TMainForm::LoadMatrix()
{
	TStringList *strlist = new TStringList;
	strlist->LoadFromFile( "inverse.txt" );
	if( strlist->Count > 0 ){
		Application->MessageBox( "行列を読み込みました。","",MB_OK );
		sscanf( strlist->Strings[0].c_str(),
			"%lg,%lg,%lg,%lg,%lg,%lg",
			&OffsetX,&OffsetY,&MatrixA,&MatrixB,&MatrixC,&MatrixD );
		OffsetX = 10;
	}
	delete strlist;
}

//---------------------------------------------------------------------------
void TMainForm::SaveSetting()
{
	TIniFile *inifile =
		new TIniFile( ExtractFilePath(Application->ExeName) + "setting.ini" );
	inifile->WriteInteger( "Screen","Width",ScreenWidth );
	inifile->WriteInteger( "Screen","Height",ScreenHeight );
	inifile->WriteString( "SendPoint","Host",UdpSocket1->RemoteHost );
	inifile->WriteString( "SendPoint","Port",UdpSocket1->RemotePort );
	inifile->WriteInteger( "SendPoint","WaitTime",SendWaitTime );
	delete inifile;
}

//---------------------------------------------------------------------------
void TMainForm::LoadSetting()
{
	TIniFile *inifile =
		new TIniFile( ExtractFilePath(Application->ExeName) + "setting.ini" );
	ScreenWidth = inifile->ReadInteger( "Screen","Width",1024 );
	ScreenHeight = inifile->ReadInteger( "Screen","Height",768 );
	UdpSocket1->RemoteHost =
		inifile->ReadString( "SendPoint","Host","localhost" );
	UdpSocket1->RemotePort = inifile->ReadString( "SendPoint","Port","10000" );
	SendWaitTime = inifile->ReadInteger( "SendPoint","WaitTime",100 );
	delete inifile;
}

//---------------------------------------------------------------------------
bool TMainForm::StartCapture()
{
	Dshow->FindCaptureDevices();
	if( Dshow->GetNumOfCaptureDevices() ){
		Caption = Dshow->GetCaptureDevice(0)->GetName();
	}else{
		Caption = "NoCaptureDevice";
		return false;
	}
	Dshow->Render();
	if( Dshow->HaveVideoData() ){
		ClientWidth = Dshow->GetVideoWidth();
		ClientHeight = Dshow->GetVideoHeight() + StatusBar1->Height;
		Dshow->SetDrawPosition( 0,0,ClientWidth,ClientHeight-StatusBar1->Height );
		PreviewForm->ClientWidth = Dshow->GetVideoWidth();
		PreviewForm->ClientHeight = Dshow->GetVideoHeight();
	}
	Dshow->Run();

	return true;
}

//---------------------------------------------------------------------------
void TMainForm::StopCapture()
{
	Calculating = false;
	Dshow->Stop();
	SetForegroundWindow( MainForm->Handle );
}

//---------------------------------------------------------------------------
bool TMainForm::CalculateLoop()
{
	Calculating = true;
	int count = 0;

	AnsiString str;
	unsigned int sum_x,sum_y,num_of_point;
	double average_x,average_y;

	UdpSocket1->Connect();

	DWORD time;
	while( Calculating ){
		time = timeGetTime();
		if( IsPreviewWindow ){
			CalculatePointWithPreview();
		}else{
			CalculatePoint();
		}
		if( IsMouseMove ){
			MoveMouseToPoint();
		}
		count++;
		WritePointToStatusBar();
		SendScreenPoint();
		do{
			Application->ProcessMessages();
		}while( timeGetTime() < time + SendWaitTime );
	}

	UdpSocket1->Disconnect();

	return true;
}

//---------------------------------------------------------------------------
bool TMainForm::StopCalculate()
{
	Calculating = false;
	return true;
}

//---------------------------------------------------------------------------
void TMainForm::SendScreenPoint()
{
	if( ! UdpSocket1->Connected ) return;

	AnsiString str;
	str.printf( "%d,%d,%d",ScreenX,ScreenY,(int)IsCaptured );
	UdpSocket1->SendBuf( str.c_str(),str.Length()+1,0 );
}

//---------------------------------------------------------------------------
void TMainForm::SetRMin(BYTE r_min)
{
	RMin = r_min;
	WriteRgbToStatusBar();
}

//---------------------------------------------------------------------------
void TMainForm::SetGMax(BYTE g_max)
{
	GMax = g_max;
	WriteRgbToStatusBar();
}

//---------------------------------------------------------------------------
void TMainForm::SetBMax(BYTE b_max)
{
	BMax = b_max;
	WriteRgbToStatusBar();
}

//---------------------------------------------------------------------------
void TMainForm::WriteRgbToStatusBar()
{
	AnsiString str;
	str.printf( "R:%d,G:%d,B:%d",RMin,GMax,BMax );
	StatusBar1->Panels->Items[0]->Text = str;
}

//---------------------------------------------------------------------------
void TMainForm::SaveRgb()
{
	// ここを書いている途中
	TStringList *strlist = new TStringList;
	strlist->Add( AnsiString(RMin) );
	strlist->Add( AnsiString(GMax) );
	strlist->Add( AnsiString(BMax) );
	strlist->SaveToFile("rgb.txt");
	delete strlist;

}

//---------------------------------------------------------------------------
void TMainForm::LoadRgb()
{
	// RGB閾値をファイルから読み込み
	bool success = false;
	TStringList *strlist = new TStringList;

	strlist->LoadFromFile("rgb.txt");
	if( strlist->Count > 2 ){
		try{
			SetRMin( strlist->Strings[0].ToInt() );
			SetGMax( strlist->Strings[1].ToInt() );
			SetBMax( strlist->Strings[2].ToInt() );
			success = true;
		}catch(...){
		}
	}

	if( ! success ){
		SetRMin( 0x80 );
		SetGMax( 0xFF );
		SetBMax( 0xFF );
	}

	delete strlist;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	// DirectShow初期設定
	CoInitialize(NULL);
	Dshow = new KtDShowCapture();
	Dshow->SetOutputWindow( Handle );
	Dshow->SetAddToRot( true );
	Dshow->SetGetSample( true );

	// RGB閾値をファイルから読み込み
	LoadRgb();

	// 行列ファイルを読み込み
	LoadMatrix();

	// 設定を読み込み
	LoadSetting();
	
/*
	// StatusBarの大きさを設定
	StatusBar1->ClientHeight = abs( StatusBar1->Font->Height );
	int fontWidth = 0;
	GetCharWidth32( StatusBar1->Handle,'w','w',&fontWidth );
	StatusBar1->Panels->Items[0]->Width = fontWidth * 8;
	StatusBar1->Panels->Items[1]->Width = fontWidth * 16;
	StatusBar1->Panels->Items[0]->Width = fontWidth * 3;
*/
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	delete Dshow;
	CoUninitialize();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject *Sender)
{
	Dshow->SetDrawPosition( 0,0,ClientWidth,ClientHeight-StatusBar1->Height );
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StartExecute(TObject *Sender)
{
	StartCapture();
	CalculateLoop();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::StopExecute(TObject *Sender)
{
	StopCalculate();
	StopCapture();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CaptureExecute(TObject *Sender)
{
	CalculateLoop();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RMinUpExecute(TObject *Sender)
{
	if( 0xFF - RMin < 0x08 ){
		SetRMin( 0xFF );
	}else{
		SetRMin( RMin+0x08 );
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RMinDownExecute(TObject *Sender)
{
	if( RMin < 0x08 ){
		SetRMin( 0x00 );
	}else{
		SetRMin( RMin-0x08 );
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ExitE1Click(TObject *Sender)
{
	Calculating = false;
	//Application->Terminate();
	MainForm->Close();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	StopCalculate();

	SaveRgb();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::CameraPropertyExecute(TObject *Sender)
{
	Dshow->ShowCaptureDeviceProperty();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OutputPropertyExecute(TObject *Sender)
{
	Dshow->ShowCaptureOutputProperty();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ToggleMoveMouseExecute(TObject *Sender)
{
	IsMouseMove = !IsMouseMove;
	((TAction*)Sender)->Checked = IsMouseMove;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ToggleShowPreviewExecute(TObject *Sender)
{
	IsPreviewWindow = !IsPreviewWindow;
	((TAction*)Sender)->Checked = IsPreviewWindow;
	PreviewForm->Visible = IsPreviewWindow;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::LoadTransformMatrixExecute(TObject *Sender)
{
	TStringList *strlist = new TStringList;
	strlist->LoadFromFile( "..\kurahashi\inverse.txt" );
	sscanf( strlist->Strings[0].c_str(),"%f,%f,%f,%f",
		&MatrixA,&MatrixB,&MatrixC,&MatrixD );
	delete strlist;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TogglePositionTransformExecute(TObject *Sender)
{
	// スクリーンの頂点を指定しているときは座標変換しない
	if( IsCalibrateMatrixMode ){
		return;
	}

	IsPositionTransform = !IsPositionTransform;
	((TAction*)Sender)->Checked = IsPositionTransform;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CalibrateMatrixExecute(TObject *Sender)
{
	// 座標変換が有効なときは無効に
	if( IsPositionTransform ){
		TogglePositionTransform->Execute();
	}

	IsCalibrateMatrixMode = true;
	NumOfScreenApex = 0;
	SetFocus();
//	SetForegroundWindow( Handle);
	SetCapture( Handle );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormMouseUp(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// スクリーン頂点指定モードでないときは何もしない
	if( !IsCalibrateMatrixMode ){
		return;
	}

//	Application->MessageBox( "Hoge","",MB_OK );

	ScreenApex[NumOfScreenApex][0] = X;
	ScreenApex[NumOfScreenApex][1] = Y;
	NumOfScreenApex++;

	SetFocus();
//	SetForegroundWindow( Handle );
	SetCapture( Handle );

	// 頂点を4つ指定したら行列を計算して頂点指定モードを終了。
	if( NumOfScreenApex == 4 ){
		AnsiString str;
		str.printf( "POINT1:%d,%d\nPOINT2:%d,%d\nPOINT3:%d,%d",
			ScreenApex[0][0],ScreenApex[0][1],
			ScreenApex[1][0],ScreenApex[1][1],
			ScreenApex[2][0],ScreenApex[2][1],
			ScreenApex[3][0],ScreenApex[3][1] );
		Application->MessageBox( str.c_str(),"頂点指定終了",MB_OK );

		if( ! CalculateMatrix() ){
			Application->MessageBox( "逆行列を求められませんでした。",
				"頂点指定終了",MB_OK );
		}
		IsCalibrateMatrixMode = false;
		ReleaseCapture();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ShowSettingFormExecute(TObject *Sender)
{
	SettingForm->EditWidth->Text = ScreenWidth;
	SettingForm->EditHeight->Text = ScreenHeight;
	SettingForm->EditHost->Text = UdpSocket1->RemoteHost;
	SettingForm->EditPort->Text = UdpSocket1->RemotePort;
	SettingForm->EditSendWaitTime->Text = SendWaitTime;
	int result = SettingForm->ShowModal();
	if( result == mrOk ){
		ScreenWidth = SettingForm->EditWidth->Text.ToInt();
		ScreenHeight = SettingForm->EditHeight->Text.ToInt();
		UdpSocket1->RemoteHost = SettingForm->EditHost->Text;
		UdpSocket1->RemotePort = SettingForm->EditPort->Text;
		SendWaitTime = SettingForm->EditSendWaitTime->Text.ToIntDef(0);
		SaveSetting();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GMaxUpExecute(TObject *Sender)
{
	if( 0xFF - GMax < 0x08 ){
		SetGMax( 0xFF );
	}else{
		SetGMax( GMax+0x08 );
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GMaxDownExecute(TObject *Sender)
{
	if( GMax < 0x08 ){
		SetGMax( 0x00 );
	}else{
		SetGMax( GMax-0x08 );
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BMaxUpExecute(TObject *Sender)
{
	if( 0xFF - BMax < 0x08 ){
		SetBMax( 0xFF );
	}else{
		SetBMax( BMax+0x08 );
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::BMaxDownExecute(TObject *Sender)
{
	if( BMax < 0x08 ){
		SetBMax( 0x00 );
	}else{
		SetBMax( BMax-0x08 );
	}
}
//---------------------------------------------------------------------------

