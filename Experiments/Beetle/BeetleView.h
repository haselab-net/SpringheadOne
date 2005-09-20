// BeetleView.h : CBeetleView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <WinBasis/WBCriticalSection.h>
#include <Physics/PhSolid.h>
#include <Physics/PhContactEngine.h>
#include <ImpD3D/D3Render.h>

#include <WinBasis/WBMMTimer.h>
#include <HIS/HISpidar4.h>


class CConsoleDlg;
class CBeetleView : public CView
{

	std::string		initialLoadFile;
	std::string		loadFileName;
	bool			execFlag;
	Spr::WBMMTimer	timer;
	UINT simTimerID;					///<	シミュレーション用のWM_TIMERのタイマーID
	UINT drawTimerID;					///<	描画用のWM_TIMERのタイマーID
	volatile bool bUpdate;				///<	更新フラグ
	CConsoleDlg*	pdlgConsole;		//コンソールダイアログ

	//	SPIDARまわり
	Spr::DVDeviceManager devMan;
	Spr::HISpidar4 spidar[2];
	Spr::UTRef<Spr::PHSolid> pointer[2];
	bool bMinForce;

	//	AI関係
	std::vector<Spr::PHSolid*> atracters;
	Spr::PHSolid* beetle;
	std::vector<Spr::PHContactEngine::FramePairRecord*> fpRecords;

protected:
	CBeetleView();
	DECLARE_DYNCREATE(CBeetleView)

public:
	CBeetleDoc* GetDocument();
	Spr::UTRef<Spr::D3Render> render;
	float dt;
	
	//{{AFX_VIRTUAL(CBeetleView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

	void Step();
	virtual ~CBeetleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Load(std::string fn);
	void Save(std::string fn);
	void ThrowSphere(float r);
	
protected:
	//{{AFX_MSG(CBeetleView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowConsole();
};

#ifndef _DEBUG  // BeetleView.cpp ファイルがデバッグ環境の時使用されます。
inline CBeetleDoc* CBeetleView::GetDocument()
   { return (CBeetleDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
