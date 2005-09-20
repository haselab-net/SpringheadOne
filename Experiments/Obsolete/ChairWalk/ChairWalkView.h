// ChairWalkView.h : CChairWalkView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ChairWalkVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
#define AFX_ChairWalkVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ImpD3D/SGD3D.h>
#include <Collision/CDDetector.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinDX/DInput/WXDInput.h> 

using namespace Spr;

#if _WIN32_WINNT >= 0x501
class WBRawInputDevice:public RAWINPUT{
public:
	UTString name;
};
inline bool operator < (WBRawInputDevice& d1, WBRawInputDevice& d2){
	if (d1.header.dwType < d2.header.dwType) return true;
	if (d1.name.compare(d2.name) < 0) return true;
	return false;
}

class WBRawInput{
public:
	class WBRawInputDevices: public std::vector<WBRawInputDevice>{
	public:
	};
	WBRawInputDevices devices;
	struct HandleLess{
		bool operator() (WBRawInputDevice* d1, WBRawInputDevice* d2) const{
			return (DWORD)d1->header.hDevice < (DWORD)d2->header.hDevice;
		}
	};
	typedef std::set<WBRawInputDevice*, HandleLess> Index;
	Index index;

	void Enum();
	void Register(HWND hWnd);
	int Get(LPARAM lParam);
	WBRawInputDevice* Device(HANDLE hD);
};

#endif

class CChairWalkView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CChairWalkView();
	DECLARE_DYNCREATE(CChairWalkView)

// アトリビュート
public:
	bool bFullscreen;
	CChairWalkDoc* GetDocument();
	Spr::WXDInput dinput;
	Spr::UTRef<Spr::D3Render> render;
	Spr::UTRef<Spr::SGScene> scene;
	Spr::Affinef afBody, afHead;
	UINT timerID;
	float dt;
	/// 現在のカメラ
	Spr::UTRef<Spr::SGCamera> camera;

	//	RawInput
	WBRawInput rawInput;
	//	マウスの値
	Vec3f angleMouse;		//	背もたれの角度
	Vec3f floorMouse[2];	//	床のマウス
	Vec3f rotMouse;			//	座席の回転角
	//	椅子の状態
	Affinef afLower;
	Affinef afUpper;

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CChairWalkView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL
	LRESULT AFX_MSG_CALL OnInput(WPARAM w, LPARAM l);
// インプリメンテーション
public:
	void Step();
	virtual ~CChairWalkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Load(const char* filename);
	void Save(const char* filename);

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CChairWalkView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ChairWalkView.cpp ファイルがデバッグ環境の時使用されます。
inline CChairWalkDoc* CChairWalkView::GetDocument()
   { return (CChairWalkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ChairWalkVIEW_H__2DC375FA_13A6_4A8D_ABC1_344D836C16E0__INCLUDED_)
