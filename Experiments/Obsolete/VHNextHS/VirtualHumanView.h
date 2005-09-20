// VirtualHumanView.h : CVirtualHumanView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "THuman.h"
#include "TDiffFile.h"
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <ImpD3D/D3Render.h>
#include <Collision/CDFramePair.h>
#include <Physics/PHForceField.h>
#include <Physics/PHSolid.h>
#include <Physics/PHContactEngine.h>
#include <Graphics/GRCamera.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBMMTimer.h>
#include <HIS/HIMouse.h>

class CVirtualHumanView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CVirtualHumanView();
	DECLARE_DYNCREATE(CVirtualHumanView)
// アトリビュート
public:
	bool bMinForce;						//	最小張力にするかどうかのフラグ
	float time;							//	現在時刻
	Spr::UTRef<Spr::GRRender> render;	//	レンダラー
	Spr::UTRef<Spr::SGScene> scene;		//	シーングラフ
	std::string loadFile;				//	OnInitialUpdateでロードすべきファイル名
	std::string loadFileName;			//	リロード用に記憶しておくファイル名
	UINT simTimerID;					//	シミュレーション用のWM_TIMERのタイマーID
	UINT drawTimerID;					//	描画用のWM_TIMERのタイマーID
	Spr::WBMMTimer timer;				//	マルチメディアタイマー
	Spr::HIMouse mouse;					//	マウスデバイス
	Spr::UTRef<Spr::PHSolid> pointer;	//	力覚ポインタ
	Spr::GRCameraData initialCamera;	//	ファイルロード時のカメラデータ
	Spr::UTRef<Spr::PHContactEngine> contactEngine;	//	衝突判定エンジン

	THuman human;						//	Virtual Human
	TDiffFile diff;						//	Diff ファイル
	enum TAppState{						//	アプリケーションの状態
		INVALID,						//	無効
		MAKE_ANGLE,						//	DIFFファイルの頂点情報から角度ファイルを生成
		MAKE_FORCE,						//	床反力計算
		SET_TORQUE,						//　トルクデータファイルの情報をセットする
		NON_GRAVITY						//	腰固定のトルク入力
	} appState;
	void Load(std::string filename);
	void Save(const char* filename);
	void UpdateMouse();
	void Step();

	CVirtualHumanDoc* GetDocument();
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CVirtualHumanView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CVirtualHumanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CVirtualHumanView)
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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // VirtualHumanView.cpp ファイルがデバッグ環境の時使用されます。
inline CVirtualHumanDoc* CVirtualHumanView::GetDocument()
   { return (CVirtualHumanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
