// D3DRMSDIExView.h : WXD3RMSDIExView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
#define AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_

#include <LVRM/DDraw/WXDDraw.h>
#include <LVRM/WXD3D/WXRM.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class WXD3RMSDIExView : public CView
{
protected: // シリアライズ機能のみから作成します。
	WXD3RMSDIExView();
	DECLARE_DYNCREATE(WXD3RMSDIExView)

// アトリビュート
public:
	WXD3RMSDIExDoc* GetDocument();

// オペレーション
public:
	WXRM rm;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(WXD3RMSDIExView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	WXDDraws ddraws;
	WXD3Device d3dDev;
	WXD3Vec2 mouseLDrag, mouseRDrag;
	Affinef now, head;
	Vec3f vel, accel;
	void BuildScene();
	void OnIdle();
	virtual ~WXD3RMSDIExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(WXD3RMSDIExView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // D3DRMSDIExView.cpp ファイルがデバッグ環境の時使用されます。
inline WXD3RMSDIExDoc* WXD3RMSDIExView::GetDocument()
   { return (WXD3RMSDIExDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SDIDDRAWVIEW_H__D98D602C_B10C_11D1_B2EE_00C06C00E6E7__INCLUDED_)
