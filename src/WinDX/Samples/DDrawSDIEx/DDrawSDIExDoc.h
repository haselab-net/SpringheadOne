// DDrawSDIExDoc.h : WXDDrawSDIExDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIDDRAWDOC_H__D98D602A_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
#define AFX_SDIDDRAWDOC_H__D98D602A_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class WXDDrawSDIExDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	WXDDrawSDIExDoc();
	DECLARE_DYNCREATE(WXDDrawSDIExDoc)

// アトリビュート
public:

// オペレーション
public:

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(WXDDrawSDIExDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~WXDDrawSDIExDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(WXDDrawSDIExDoc)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SDIDDRAWDOC_H__D98D602A_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
