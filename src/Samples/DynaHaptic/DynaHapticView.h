// DynaHapticView.h : CDynaHapticView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FrameWork/FWAppD3D.h>
#include <FrameWork/FWPointer.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <WinBasis/WBMMTimer.h>
#include "HisConfig.h"


class CDynaHapticView : public CView {
protected: // シリアライズ機能のみから作成します。
	CDynaHapticView();
	DECLARE_DYNCREATE(CDynaHapticView)
// アトリビュート
public:
	CHisConfig dlConfig;
	Spr::UTRef<Spr::FWAppD3D> app;
	int drawTimerId;

	CDynaHapticDoc* GetDocument();

// オペレーション
public:
	void InitHis(std::string hisList, float ps, float fs);
	void ReadRegistry();
	void WriteRegistry();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDynaHapticView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDynaHapticView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	void ThrowSphere(float r);
	//{{AFX_MSG(CDynaHapticView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnD3d();
	afx_msg void OnGL();
	afx_msg void OnHisConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // DynaHapticView.cpp ファイルがデバッグ環境の時使用されます。
inline CDynaHapticDoc* CDynaHapticView::GetDocument()
   { return (CDynaHapticDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DynaHapticVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
