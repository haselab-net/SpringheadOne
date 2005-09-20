#if !defined(AFX_HISCONFIG_H__5D92FEB6_3366_465C_B6FC_FDDFEE6FB266__INCLUDED_)
#define AFX_HISCONFIG_H__5D92FEB6_3366_465C_B6FC_FDDFEE6FB266__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HisConfig.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CHisConfig ダイアログ

class CHisConfig : public CDialog
{
// コンストラクション
public:
	CHisConfig(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CHisConfig)
	enum { IDD = IDD_HIS_CONFIG };
	CString	hisList;
	float	forceScale;
	float	posScale;
	float	timeStep;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CHisConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CHisConfig)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_HISCONFIG_H__5D92FEB6_3366_465C_B6FC_FDDFEE6FB266__INCLUDED_)
