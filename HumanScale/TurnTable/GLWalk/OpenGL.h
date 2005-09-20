// OpenGL.h : OPENGL アプリケーションのメイン ヘッダー ファイル
//

#if !defined(AFX_OPENGL_H__1BF8B864_8A1C_11D5_BE31_0800460222F0__INCLUDED_)
#define AFX_OPENGL_H__1BF8B864_8A1C_11D5_BE31_0800460222F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// COpenGLApp:
// このクラスの動作の定義に関しては OpenGL.cpp ファイルを参照してください。
//

class COpenGLApp : public CWinApp
{
public:
	COpenGLApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COpenGLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション
	//{{AFX_MSG(COpenGLApp)
	afx_msg void OnAppAbout();
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPENGL_H__1BF8B864_8A1C_11D5_BE31_0800460222F0__INCLUDED_)
