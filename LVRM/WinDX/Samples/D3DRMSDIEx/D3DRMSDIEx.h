// D3DRMSDIEx.h : SDIDDRAW アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_SDIDDRAW_H__D98D6024_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
#define AFX_SDIDDRAW_H__D98D6024_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExApp:
// このクラスの動作の定義に関しては D3DRMSDIEx.cpp ファイルを参照してください。
//
class WXD3RMSDIExView;

class WXD3RMSDIExApp : public CWinApp
{
public:
	WXD3RMSDIExApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(WXD3RMSDIExApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// インプリメンテーション
	WXD3RMSDIExView* view;

	//{{AFX_MSG(WXD3RMSDIExApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SDIDDRAW_H__D98D6024_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
