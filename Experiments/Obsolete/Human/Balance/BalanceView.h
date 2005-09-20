// BalanceView.h : CBalanceView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BalanceVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_BalanceVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Human.h"
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <Collision/CDFramePair.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBMMTimer.h>
#include <Physics/PHSolid.h>
#include <HIS/HIMouse.h>

class CBalanceView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CBalanceView();
	DECLARE_DYNCREATE(CBalanceView)

	bool bMinForce;

// アトリビュート
public:
	Spr::UTRef<Spr::D3Render> render;
	Spr::UTRef<Spr::SGScene> scene;
	float dt;
	std::string loadFile;
	std::string loadFileName;			//リロード用に記憶しておくファイル名
	UINT wmTimerID;
	Spr::WBMMTimer timer;
	Spr::UTRef<Spr::PHSolid> pointer;
	Spr::HIMouse mouse;
	CHuman human;

	CBalanceDoc* GetDocument();
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CBalanceView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void OnIdle();
	void Step();
	virtual ~CBalanceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Load(std::string filename);
	void Save(const char* filename);

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CBalanceView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // BalanceView.cpp ファイルがデバッグ環境の時使用されます。
inline CBalanceDoc* CBalanceView::GetDocument()
   { return (CBalanceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BalanceVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
