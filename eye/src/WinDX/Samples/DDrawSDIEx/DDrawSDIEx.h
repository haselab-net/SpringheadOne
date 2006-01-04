// DDrawSDIEx.h : SDIDDRAW アプリケーションのメイン ヘッダー ファイル
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
// WXDDrawSDIExApp:
// このクラスの動作の定義に関しては DDrawSDIEx.cpp ファイルを参照してください。
//

class WXDDrawSDIExApp : public CWinApp
{
public:
	WXDDrawSDIExApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(WXDDrawSDIExApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(WXDDrawSDIExApp)
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
