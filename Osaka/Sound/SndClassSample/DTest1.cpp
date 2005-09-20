#include <windows.h>
#include <commdlg.h>
#include "SndClass.h"
#include <crtdbg.h>

unsigned int CALLBACK	HookFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main()
{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);

	CHOOSECOLOR cc;
	COLORREF custc[16];

	memset( &cc, 0, sizeof( CHOOSECOLOR ) );
	memset( &custc, 0, sizeof( COLORREF ) * 16 );
	cc.lStructSize = sizeof( CHOOSECOLOR );
	cc.hwndOwner = NULL;
	cc.rgbResult = 0x00ff0000;
	cc.lpCustColors = custc;
	cc.Flags = CC_RGBINIT | CC_ENABLEHOOK;
	cc.lpfnHook = HookFunc;

	ChooseColor( &cc );

	return 0;
}

unsigned int CALLBACK	HookFunc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static	SndSystem *objSndSystem;

	switch( uMsg )
	{
	case WM_INITDIALOG:
		objSndSystem = new SndSystem;
		if( !(objSndSystem->Init( hWnd )) ){
			MessageBox( hWnd, "InitializeSound Err!!", "Err!!", MB_OK );
		}
		return FALSE;
	case WM_CHAR:
		{
			int sw1 = wParam - '0';
			if( sw1 >= 0 && sw1 < NUMSNDFILES ){
				// 音スタート
				objSndSystem->GetSndFile(sw1)->Play( 0 );
				return TRUE;
			}
			sw1 -= NUMSNDFILES;
			if( sw1 >= 0 && sw1 < NUMSNDFILES ){
				// 音スタート
				objSndSystem->GetSndFile(sw1)->Play( -1000 );
				return TRUE;
			}
			sw1 = wParam - 'a';
			if( sw1 >= 0 && sw1 < NUMSNDFILES ){
				// 音ストップ
				objSndSystem->GetSndFile(sw1)->Stop();
				return TRUE;
			}
		}
		return TRUE;
	case WM_DESTROY :
		/* DirectSoundオブジェクトの開放 */
		delete( objSndSystem );
		return FALSE;
	}
	return FALSE;
}
