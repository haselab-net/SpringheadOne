// DynaLocalHapView.h : CDynaLocalHapView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynaLocalHapVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_DynaLocalHapVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <FrameWork/FWAppD3D.h>
#include <FrameWork/FWPointer.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <WinBasis/WBMMTimer.h>
#include <Collision/CDCollisionEngine.h>
#include <Physics/PHContactEngine.h>
#include "HapticRender.h"
#include "HisConfig.h"

#ifdef USE_IHC
#include <Device/DRUsb20Sh4.h>
#endif

class TPlane{
public:
	Vec3f pos;
	Vec3f normal;
	UTRef<PHSolid> solid;
	TPlane(Vec3f p, Vec3f n, PHSolid* s):pos(p), normal(n), solid(s){}
};
class TPlanes:public std::vector<TPlane>{
};

class CLocalHapApp:public Spr::FWAppD3D, public CDCollisionListener{
protected:
	UTRef<PHSolid> soNearSphere;
	UTRef<PHContactEngine> pContactEngine;
	int listenerPos;				///<	リスナの位置

public:
	float posScale, forceScale;
	Vec3f hapticOffset;
#ifdef USE_IHC
	DRUsb20Sh4* ihc;
#else
	HapticRender hapticRender;
#endif
	TPlanes planes;
	Vec3f pointer;					///<	ポインタの位置
	float pointerRadius;			///<	ポインタの半径
	Vec3f planePoint;
	Vec3f planeNormal;
	UTRef<PHSolid> planeSolid;
	float minDist;
	Vec3f receivedForce;

	CLocalHapApp();
	~CLocalHapApp();

	//毎回の更新処理（シミュレータ）
	void SimulationStep();
	void Step();
	/*衝突判定リスナ用関数
	  衝突物体の最近傍点とFrameRecordをベクターに登録  */
	void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	void Load(std::string fn);
	void Draw();

};

class CDynaLocalHapView : public CView {
protected: // シリアライズ機能のみから作成します。
	CHisConfig dlConfig;
	Spr::WBMMTimer timer;
	CLocalHapApp app;
	UINT simuTimerId, drawTimerId;
	bool bPause;

	std::vector<std::string> fileList;
	int fileListCur;

	CDynaLocalHapView();
	DECLARE_DYNCREATE(CDynaLocalHapView)
// アトリビュート
public:
	CDynaLocalHapDoc* GetDocument();

// オペレーション
public:
	void InitHis(std::string hisList, float ps, float fs);
	void ReadRegistry();
	void WriteRegistry();
	void Step();
	void Load(std::string filename);
	void Save(std::string filename);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDynaLocalHapView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDynaLocalHapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	void LoadList();
	void ThrowSphere(float r);
	//{{AFX_MSG(CDynaLocalHapView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnD3d();
	afx_msg void OnGL();
	afx_msg void OnHisConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // DynaLocalHapView.cpp ファイルがデバッグ環境の時使用されます。
inline CDynaLocalHapDoc* CDynaLocalHapView::GetDocument()
   { return (CDynaLocalHapDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DynaLocalHapVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
