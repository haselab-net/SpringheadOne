#include "InitFuncs.h"
//#include <crtdbg.h>

int WINAPI WinMain(	HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPSTR lpCmdLine,
						int nCmdShow )
{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);

	int		iRet = -1;
	BOOL	bRet = FALSE;
	WindowParams objWp;

	// �\���̂̃����o�̏�����
	InitParams( &objWp, hInstance, lpCmdLine, nCmdShow );

	// �E�C���h�E�N���X�̍쐬
	bRet = RegWinClass( &objWp );
	if( !bRet ) goto WinMain_End;

	// �E�C���h�E�̍쐬��
	bRet = WindowSetup( &objWp );
	if( !bRet ) goto WinMain_End;

	iRet = 0;
	// ���b�Z�[�W���[�v
	iRet = MesLoop();

WinMain_End :

	return iRet;
}
