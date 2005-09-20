// test3Dlg.h : ヘッダー ファイル
//

#if !defined(AFX_TEST3DLG_H__A529982D_BCB4_4313_A4AF_440313197711__INCLUDED_)
#define AFX_TEST3DLG_H__A529982D_BCB4_4313_A4AF_440313197711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../KtDShow.h"
/////////////////////////////////////////////////////////////////////////////
// CTest3Dlg ダイアログ

class CTest3Dlg : public CDialog
{
// 構築
public:
	CTest3Dlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTest3Dlg)
	enum { IDD = IDD_TEST3_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTest3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTest3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonCapture();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonStart();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	KtDShowCapture* dshow;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TEST3DLG_H__A529982D_BCB4_4313_A4AF_440313197711__INCLUDED_)
