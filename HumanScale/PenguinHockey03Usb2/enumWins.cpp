#include <windows.h>

#define	LENWINNAME	256

typedef struct tagRetWinHandles
{
	HWND	hWnd;
	char	szWinName[LENWINNAME];
	int		nHitWindowsNum;
} RetWinHandles;

BOOL CALLBACK EnumWndProc( HWND hWnd, LPARAM lParam )
{
	char			szGetWinName[LENWINNAME];
	RetWinHandles	*pretWinH;

	pretWinH = ( RetWinHandles * ) lParam;
	GetWindowText( hWnd, szGetWinName, LENWINNAME );

	if( lstrcmp( szGetWinName, pretWinH->szWinName ) == 0 ) {
		pretWinH->hWnd = hWnd;
		( pretWinH->nHitWindowsNum )++;

		OutputDebugString( "Window Hit!!\n" );
	}

	return TRUE;
}

HWND GetHWnd( const char *windowname )
{
	char	msgbuf[1024];
	RetWinHandles retWinH;

	retWinH.hWnd = NULL;
	retWinH.nHitWindowsNum = 0;
	wsprintf( retWinH.szWinName, "%s", windowname );

	wsprintf( msgbuf, "Name = \"%s\" start!!\n", windowname );
	OutputDebugString( msgbuf );

	EnumWindows( EnumWndProc, ( LPARAM ) &retWinH );

	if( retWinH.nHitWindowsNum != 1 ){
		wsprintf( msgbuf, "There is %d Windows \"%s\"!!",
			retWinH.nHitWindowsNum, windowname );
		MessageBox( NULL, msgbuf, "Error!!!", MB_ICONEXCLAMATION | MB_OK );
		return NULL;
	}

	wsprintf( msgbuf, "hWnd = %p\n", retWinH.hWnd );
	OutputDebugString( msgbuf );

	return retWinH.hWnd;
}
