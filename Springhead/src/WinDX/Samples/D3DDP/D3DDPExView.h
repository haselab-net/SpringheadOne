// D3DDPExView.h : WXD3DPExView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
#define AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_

#include <WinDX/D3D/WXD3DPDev.h>


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class WXD3DPExView : public CView
{
protected: // シリアライズ機能のみから作成します。
	WXD3DPExView();
	DECLARE_DYNCREATE(WXD3DPExView)

// アトリビュート
public:
	WXD3DPExDoc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(WXD3DPExView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	WXDDraws ddraws;
	WXD3DPDevice d3ddev;
	virtual ~WXD3DPExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(WXD3DPExView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // D3DDPExView.cpp ファイルがデバッグ環境の時使用されます。
inline WXD3DPExDoc* WXD3DPExView::GetDocument()
   { return (WXD3DPExDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
