// EditorDlg.h: CEditorDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITORDLG_H__BABEA222_A1C5_4A8D_9797_C3201D9CDA2C__INCLUDED_)
#define AFX_EDITORDLG_H__BABEA222_A1C5_4A8D_9797_C3201D9CDA2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditorDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CEditorDlg ダイアログ

class CEditorDlg : public CDialog
{
// コンストラクション
public:
	CEditorDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CEditorDlg)
	enum { IDD = IDD_EDITOR };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditorDlg)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EDITORDLG_H__BABEA222_A1C5_4A8D_9797_C3201D9CDA2C__INCLUDED_)

