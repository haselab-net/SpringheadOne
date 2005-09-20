// DynaHapticDoc.h : CDynaHapticDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaHapticDOC_H__B6A17084_79C6_4EB1_814D_77A09AE84C67__INCLUDED_)
#define AFX_DynaHapticDOC_H__B6A17084_79C6_4EB1_814D_77A09AE84C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDynaHapticView;
class CDynaHapticDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CDynaHapticDoc();
	DECLARE_DYNCREATE(CDynaHapticDoc)

// アトリビュート
public:
// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDynaHapticDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDynaHapticDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDynaHapticDoc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DynaHapticDOC_H__B6A17084_79C6_4EB1_814D_77A09AE84C67__INCLUDED_)
