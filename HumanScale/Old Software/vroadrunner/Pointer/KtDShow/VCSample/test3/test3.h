// test3.h : TEST3 アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_TEST3_H__569A8EEB_7077_4757_9D04_5AF7DC60A2F6__INCLUDED_)
#define AFX_TEST3_H__569A8EEB_7077_4757_9D04_5AF7DC60A2F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CTest3App:
// このクラスの動作の定義に関しては test3.cpp ファイルを参照してください。
//

class CTest3App : public CWinApp
{
public:
	CTest3App();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTest3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CTest3App)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
	

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TEST3_H__569A8EEB_7077_4757_9D04_5AF7DC60A2F6__INCLUDED_)
