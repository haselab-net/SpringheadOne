// VirtualHumanView.h : CMFCD3DDynaView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinBasis/WBMMTimer.h>
#include <Graphics/GRRender.h>
#include <Graphics/GRCamera.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <Physics/PHSolid.h>
#include <Physics/PHContactEngine.h>
#include <HIS/HIMouse.h>
#include <Framework/FWPointer.h>

///	View
class CMFCD3DDynaView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CMFCD3DDynaView();
	DECLARE_DYNCREATE(CMFCD3DDynaView)
// アトリビュート
public:
	float dt;							///<	更新周期
	Spr::UTRef<Spr::GRRender> render;	///<	レンダラー
	Spr::UTRef<Spr::SGScene> scene;		///<	シーングラフ
	std::string loadFile;				///<	OnInitialUpdateでロードすべきファイル名
	std::string loadFileName;			///<	リロード用に記憶しておくファイル名
	Spr::GRCameraData initialCamera;	///<	ファイルロード時のカメラデータ
	Spr::UTRef<Spr::PHContactEngine> contactEngine;
										///<	接触エンジン．接触力の描画に使用
	Spr::WBMMTimer timer;				///<	マルチメディアタイマー
	UINT simTimerID;					///<	シミュレーション用のWM_TIMERのタイマーID
	UINT drawTimerID;					///<	描画用のWM_TIMERのタイマーID
	volatile bool bUpdate;				///<	更新フラグ

	Spr::UTRef<Spr::HIMouse> mouse;		///<	マウスデバイス
	Spr::FWPointer6D pointer;			///<	力覚ポインタ
	bool bMinForce;						///<	最小張力にするかどうかのフラグ

	void Import(std::string filename);	///<	データファイルのインポート
	void Load(std::string filename);	///<	データファイルのロード
	void Save(std::string filename);	///<	データファイルのセーブ
	void Step();						///<	シミュレーションのステップ
	void UpdateMouse();					///<	マウスデバイスの更新
	void ThrowSphere(float r);			///<	ボールを投げる

	CMFCD3DDynaDoc* GetDocument();
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMFCD3DDynaView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMFCD3DDynaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMFCD3DDynaView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRenderD3d();
	afx_msg void OnRenderGl();
	afx_msg void OnFileInsert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // VirtualHumanView.cpp ファイルがデバッグ環境の時使用されます。
inline CMFCD3DDynaDoc* CMFCD3DDynaView::GetDocument()
   { return (CMFCD3DDynaDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VirtualHumanVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
