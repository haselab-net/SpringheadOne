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

	// �t�B���^�O���t�}�l�[�W�����쐬���A
	// �C���^�[�t�F�C�X���N�G������B
	CoCreateInstance( CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,(void**)&pGraph );
	pGraph->QueryInterface( IID_IMediaControl,(void**)&pMediaControl );
	pGraph->QueryInterface( IID_IMediaEvent,(void**)&pEvent );

	// �O���t���쐬
	pGraph->RenderFile( L"..\\test.avi",NULL );

	// �O���t�̕\��
	pMediaControl->Run();

	// �I����҂�
	long evCode;
	pEvent->WaitForCompletion( INFINITE,&evCode);

	// �N���[���A�b�v
	pMediaControl->Release();
	pEvent->Release();
	pGraph->Release();
	CoUninitialize();

	MessageBox(NULL,"End","",MB_OK);

	return 0;
}
//---------------------------------------------------------------------------
