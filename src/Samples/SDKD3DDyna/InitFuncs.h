#ifndef	_INITFUNCS_H_
#define	_INITFUNCS_H_

#include <windows.h>
//#include <tchar.h>

typedef struct tagWindowParams {
	// ウインドウシステム系
	HWND		hWnd;				// ウインドウ ハンドル
	HINSTANCE	hInst;				// インスタンス
	LPTSTR		lpszCmdParam;		// コマンドライン
	int			nCmdShow;			// 表示モード
	char		szWinName[128];		// ウインドウクラス名
	char		szWinTitle[128];	// ウインドウのタイトル

} WindowParams;

// アプリケーション初期化系
void	InitParams( WindowParams *wp,
				   HINSTANCE hInst,
				   LPTSTR lpszCmdParam,
				   int nCmdShow);	// 構造体のメンバの初期化
BOOL	RegWinClass( WindowParams *wp );	// ウインドウクラスの登録
BOOL	WindowSetup( WindowParams *wp );	// ウインドウの作成等
int		MesLoop();					// メッセージループ

#ifndef _MESFUNCS_H_
LRESULT CALLBACK
MainWndProc( HWND hwnd, UINT uMsg,
			WPARAM wParam, LPARAM lParam); // メインのウインドウ関数
#endif

#endif // _INITFUNCS_H_
