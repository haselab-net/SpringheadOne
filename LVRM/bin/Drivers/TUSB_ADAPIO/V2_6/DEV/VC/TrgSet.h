#if !defined(AFX_TRGSET_H__09A46B10_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
#define AFX_TRGSET_H__09A46B10_B6F5_11D4_935E_006008B03B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrgSet.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTrgSet ダイアログ

class CTrgSet : public CDialog
{
// コンストラクション
public:
	BOOL ShowFlg;
	BOOL HexCheck(const char * str);
	int ThLevelNum;
	CTrgSet(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTrgSet)
	enum { IDD = IDD_TRGSET };
	UINT	m_EndCh;
	CString	m_Message;
	CString	m_ThLevel;
	UINT	m_TrgCh;
	UINT	m_BufSize;
	int		m_UpDown;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTrgSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTrgSet)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TRGSET_H__09A46B10_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
