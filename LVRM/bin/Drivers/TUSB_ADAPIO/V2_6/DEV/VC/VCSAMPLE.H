// vcsample.h : VCSAMPLE アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_VCSAMPLE_H__09A46B05_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
#define AFX_VCSAMPLE_H__09A46B05_B6F5_11D4_935E_006008B03B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CVcsampleApp:
// このクラスの動作の定義に関しては vcsample.cpp ファイルを参照してください。
//

class CVcsampleApp : public CWinApp
{
public:
	CVcsampleApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CVcsampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CVcsampleApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VCSAMPLE_H__09A46B05_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
