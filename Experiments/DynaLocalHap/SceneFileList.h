#if !defined(AFX_SCENEFILELIST_H__C4A3B4B2_8EB1_4854_B358_41FDAFF78917__INCLUDED_)
#define AFX_SCENEFILELIST_H__C4A3B4B2_8EB1_4854_B358_41FDAFF78917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SceneFileList.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSceneFileList ダイアログ

class CSceneFileList : public CDialog
{
// コンストラクション
public:
	CSceneFileList(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSceneFileList)
	enum { IDD = IDD_FileList };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSceneFileList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSceneFileList)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SCENEFILELIST_H__C4A3B4B2_8EB1_4854_B358_41FDAFF78917__INCLUDED_)
