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
	pGraph = NULL;
	pMediaControl = NULL;
	pVidWin = NULL;
}
//---------------------------------------------------------------------------

void TForm1::PlayStart()
{
	wchar_t filename[256];
	HRESULT hr;

	if( OpenDialog1->Execute() ){
		OpenDialog1->FileName.WideChar( filename,256 );

		// フィルタグラフマネージャの作成
		//   IGraphBuilderには、フィルタをすべて削除という関数はないようなので、
		//   すべて消す代わりに新規に作る。
		//   そうしないと過去にRenderFileした映像も同時に別ウィンドウで流れる。
		CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC,
			IID_IGraphBuilder,(void**)&pGraph );
		if( !pGraph ) Application->MessageBox( "pGraph is NULL ","",MB_OK );
		// IMediaControlは再生・停止に利用。
		pGraph->QueryInterface( IID_IMediaControl,(void**)&pMediaControl );
		// IVideoWindowはビデオ表示画面の設定に利用。
		pGraph->QueryInterface( IID_IVideoWindow,(void**)&pVidWin );
		// IBasicVideoはビデオの縦横の大きさを得るのに利用。
		pGraph->QueryInterface( IID_IBasicVideo,(void**)&pBasicVideo );

		// グラフを構築する
		pGraph->RenderFile( filename,NULL );

		// オーディオのみのファイルかを判断
		if( !IsAudioOnly() ){
			// ビデオウィンドウを構築する
			if( S_OK != pVidWin->put_Owner( (OAHWND)Handle ) ){
				Application->MessageBox("put_Owner failed","",MB_OK );
			}
			if( S_OK != pVidWin->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS ) )
			{
				Application->MessageBox( "put_WindowStyle failed","",MB_OK );
			}
			if( S_OK != pVidWin->put_Visible( OATRUE ) ){
				Application->MessageBox( "put_Visible failed","",MB_OK );
			}

			// ビデオの大きさを得て、ウィンドウサイズを変更
			long width,height;
			pBasicVideo->get_SourceWidth( &width );
			pBasicVideo->get_SourceHeight( &height );
			Width = (width < Screen->Width) ? width : Screen->Width;
			Height = (height < Screen->Height) ? height : Screen->Height;
			Left = ( Screen->Width - Width ) / 2;
			Top = ( Screen->Height - Height ) / 2;

			// ポジションをセット
			pVidWin->SetWindowPosition( 0,0,ClientWidth,ClientHeight );
		}

		// グラフを実行する
		pMediaControl->Run();
	}
}

void TForm1::PlayStop()
{
	if( pGraph ){
		// 再生の停止
		pMediaControl->Stop();
		pVidWin->put_Visible(OAFALSE); //忘れると画面描画がおかしくなる
		pVidWin->put_Owner(NULL); //忘れるとウィンドウにメッセージが送り続けられる

		// インターフェイスを開放
		KT_RELEASE( pBasicVideo );
		KT_RELEASE( pVidWin );
		KT_RELEASE( pMediaControl );
		KT_RELEASE( pGraph );
	}
}

// ファイルがオーディオのみかをチェック
// playwndサンプルのvoid CheckVisibility()を参考にした。
bool TForm1::IsAudioOnly()
{
	// オーディオのみのファイルを読み込んだときは
	// VideoWindowインターフェイスが存在しないことがあるっぽい。
	if( !pVidWin ) return true;

	// VideoWindowインターフェイスが存在するときでも
	// オーディオのみのファイルはビデオレンダラが接続されないので
	// メンバ関数を呼んでも VFW_E_NOT_CONNECTED を返す。
	// (追記)E_NOINTERFACE を返すこともあるようだ。
	// ここでは適当にget_Width関数を呼んでみる。
	long temp;
	HRESULT hr = pVidWin->get_Width( &temp );
	return hr == VFW_E_NOT_CONNECTED || hr == E_NOINTERFACE;

/*	// 本当はこのようにエラー内容をチェックした方が良いと思われ。
	hr = pVidWin->get_Visible( &temp );
	if( hr == S_OK )
		Application->MessageBox( "S_OK","",MB_OK );
	else if( hr == VFW_E_NOT_CONNECTED )
		Application->MessageBox( "VFW_E_NOT_CONNECTED","",MB_OK );
	else if( hr == E_NOINTERFACE )
		Application->MessageBox( "E_NOINTERFACE","",MB_OK );
	else
		Application->MessageBox(
			(AnsiString("Unknown Error:")+AnsiString(hr)).c_str(),"",MB_OK );

	//return VFW_E_NOT_CONNECTED == pVidWin->get_Width( &temp );
	return S_OK != pVidWin->get_Width( &temp );
*/
}

//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	CoInitialize( NULL );
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	CoUninitialize();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenO1Click(TObject *Sender)
{
	PlayStop();
	PlayStart();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StopS1Click(TObject *Sender)
{
	PlayStop();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	PlayStop();
}
//---------------------------------------------------------------------------

