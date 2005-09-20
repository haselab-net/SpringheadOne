#if !defined(AFX_ETCDIALOG_H__F8625196_1E0A_4E54_BF1C_44DAD4E3A399__INCLUDED_)
#define AFX_ETCDIALOG_H__F8625196_1E0A_4E54_BF1C_44DAD4E3A399__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtcDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// EtcDialog ダイアログ

class EtcDialog : public CDialog
{
// コンストラクション
public:
	EtcDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(EtcDialog)
	enum { IDD = IDD_DIALOG_NEW };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(EtcDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(EtcDialog)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ETCDIALOG_H__F8625196_1E0A_4E54_BF1C_44DAD4E3A399__INCLUDED_)
