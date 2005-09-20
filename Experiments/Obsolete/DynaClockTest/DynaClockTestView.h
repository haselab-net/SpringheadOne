// DynaClockTestView.h : CDynaClockTestView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaClockTestVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_DynaClockTestVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBPreciseTimer.h>
#include <Physics/PhSolid.h>
#include <ImpD3D/D3Render.h>

class CDynaClockTestView : public CView
{
	std::string initialLoadFile;
	std::string loadFileName;
	int height, side;
	bool execFlag;
protected: // シリアライズ機能のみから作成します。
	CDynaClockTestView();
	DECLARE_DYNCREATE(CDynaClockTestView)

// アトリビュート
public:

	CDynaClockTestDoc* GetDocument();
	Spr::UTRef<Spr::D3Render> render;
	Spr::UTRef<Spr::SGScene> scene;
	Spr::Affinef afBody, afHead;
	float dt;
	/// 現在のカメラ
	Spr::UTRef<Spr::GRCamera> camera;

	Spr::PHSolid* solid;

	Spr::WBPreciseTimer ptimer;

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDynaClockTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void OnIdle();
	void Step();
	virtual ~CDynaClockTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Load(std::string filename);
	void Save(std::string filename);
	bool CreateBox();
	bool CreateBoxAuto();

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CDynaClockTestView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DynaClockTestView.cpp ファイルがデバッグ環境の時使用されます。
inline CDynaClockTestDoc* CDynaClockTestView::GetDocument()
   { return (CDynaClockTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DynaClockTestVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
