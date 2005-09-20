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

	// 構造体のメンバの初期化
	InitParams( &objWp, hInstance, lpCmdLine, nCmdShow );

	// ウインドウクラスの作成
	bRet = RegWinClass( &objWp );
	if( !bRet ) goto WinMain_End;

	// ウインドウの作成等
	bRet = WindowSetup( &objWp );
	if( !bRet ) goto WinMain_End;

	iRet = 0;
	// メッセージループ
	iRet = MesLoop();

WinMain_End :

	return iRet;
}
