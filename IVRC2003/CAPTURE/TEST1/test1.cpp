// test1.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "../../KtDShow.h"

int main(int argc, char* argv[])
{
	CoInitialize(NULL);
	KtDShowBasic *dshow = new KtDShowBasic();
//	dshow->SetOutputWindow( Handle ); //既存のウィンドウに表示する場合
	dshow->SetOutputWindow( NULL ); //新規にウィンドウを作成する場合
	dshow->SetAddToRot( true );

	dshow->SetInputFile( "..\\..\\sample.avi" );
	dshow->Render();
	dshow->Run();

	int key;
	key = getchar();

	delete dshow;
	CoUninitialize();

	return 0;
}
