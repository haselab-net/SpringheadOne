// SLCSampleDlg.h : ヘッダー ファイル
//

#if !defined(AFX_SLCSAMPLEDLG_H__6E5C838D_8E85_462E_9B5E_4A37F26BAF25__INCLUDED_)
#define AFX_SLCSAMPLEDLG_H__6E5C838D_8E85_462E_9B5E_4A37F26BAF25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSLCSampleDlg ダイアログ

class CSLCSampleDlg : public CDialog
{
// 構築
public:
	CSLCSampleDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSLCSampleDlg)
	enum { IDD = IDD_SLCSAMPLE_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSLCSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSLCSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnButtonCamcfg();
	afx_msg void OnButtonVidcfg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hSLC_device;	//▼SLC API ハンドル
	BOOL OnSlcDataGet(WPARAM wParam,LPARAM lParam);	//▼SLC API イベントハンドラ
	BOOL OnSlcDataGetDeb(WPARAM wParam,LPARAM lParam);	//▼SLC API イベントハンドラ
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SLCSAMPLEDLG_H__6E5C838D_8E85_462E_9B5E_4A37F26BAF25__INCLUDED_)
