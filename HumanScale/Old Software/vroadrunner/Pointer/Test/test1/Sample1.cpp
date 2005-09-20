//---------------------------------------------------------------------------
#include <windows.h>
#pragma hdrstop
#include <dshow.h>

//---------------------------------------------------------------------------

#pragma argsused
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	IGraphBuilder *pGraph;
	IMediaControl *pMediaControl;
	IMediaEvent *pEvent;
	CoInitialize(NULL);

	MessageBox(NULL,"Start","",MB_OK);

	// フィルタグラフマネージャを作成し、
	// インターフェイスをクエリする。
	CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,(void**)&pGraph );
	pGraph->QueryInterface( IID_IMediaControl,(void**)&pMediaControl );
	pGraph->QueryInterface( IID_IMediaEvent,(void**)&pEvent );

	// グラフを作成
	pGraph->RenderFile( L"..\\test.avi",NULL );

	// グラフの表示
	pMediaControl->Run();

	// 終了を待つ
	long evCode;
	pEvent->WaitForCompletion( INFINITE,&evCode);

	// クリーンアップ
	pMediaControl->Release();
	pEvent->Release();
	pGraph->Release();
	CoUninitialize();

	MessageBox(NULL,"End","",MB_OK);

	return 0;
}
//---------------------------------------------------------------------------
