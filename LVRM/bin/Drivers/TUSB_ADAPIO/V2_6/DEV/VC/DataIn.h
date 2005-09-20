#if !defined(AFX_DATAIN_H__09A46B0F_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
#define AFX_DATAIN_H__09A46B0F_B6F5_11D4_935E_006008B03B0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataIn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDataIn ダイアログ

class CDataIn : public CDialog
{
// コンストラクション
public:
	int DataNum;
	BOOL HexCheck(const char * str);
	CDataIn(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDataIn)
	enum { IDD = IDD_DATAIN };
	CString	m_Data;
	CString	m_Message;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDataIn)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDataIn)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DATAIN_H__09A46B0F_B6F5_11D4_935E_006008B03B0A__INCLUDED_)
