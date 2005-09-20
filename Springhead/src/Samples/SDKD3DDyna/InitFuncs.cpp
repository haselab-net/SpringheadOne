#include "InitFuncs.h"

void InitParams( WindowParams *wp, HINSTANCE hInst, 
				LPTSTR lpszCmdParam, int nCmdShow)
{
	wp->hInst = hInst;
	wp->hWnd = NULL;
	wp->lpszCmdParam = lpszCmdParam;
	wp->nCmdShow = nCmdShow;
	lstrcpy( wp->szWinName, "TestClass" );
	lstrcpy( wp->szWinTitle, "TestWindow" );
}

BOOL RegWinClass( WindowParams *wp )
{
	WNDCLASSEX	wcl;

	wcl.cbSize			= sizeof( WNDCLASSEX );
	wcl.hInstance		= wp->hInst;
	wcl.lpszClassName	= wp->szWinName;
	wcl.lpfnWndProc		= MainWndProc;
	wcl.style			= 0;
	wcl.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wcl.hIconSm			= NULL;
	wcl.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wcl.lpszMenuName	= NULL;
	wcl.cbClsExtra		= 0;
	wcl.cbWndExtra		= 0;
	wcl.hbrBackground	= ( HBRUSH ) GetStockObject( WHITE_BRUSH );

	return RegisterClassEx( &wcl );
}

BOOL	WindowSetup( WindowParams *wp )
{
	wp->hWnd = CreateWindow( wp->szWinName, wp->szWinTitle,
		WS_OVERLAPPEDWINDOW, 0, 0, 640, 480,
		NULL, NULL, wp->hInst, NULL );

	if( !(wp->hWnd) ) return FALSE;

	ShowWindow( wp->hWnd, wp->nCmdShow );	// Window•\Ž¦
//	ShowWindow( wp->hWnd, SW_HIDE );		// Window”ñ•\Ž¦
	UpdateWindow( wp->hWnd );

	return TRUE;
}

int		MesLoop()
{
	MSG		msg;

	while( GetMessage( &msg, NULL, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return msg.wParam;
}
