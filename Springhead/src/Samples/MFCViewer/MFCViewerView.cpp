// MFCViewerView.cpp : CMFCViewerView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "MFCViewer.h"

#include "MFCViewerDoc.h"
#include "MFCViewerView.h"
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define MAKEPHOTO

#ifdef MAKEPHOTO
static int count;
static int countMax = 10;
static int stepCount;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView


IMPLEMENT_DYNCREATE(CMFCViewerView, CView)

BEGIN_MESSAGE_MAP(CMFCViewerView, CView)
	//{{AFX_MSG_MAP(CMFCViewerView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView クラスの構築/消滅

CMFCViewerView::CMFCViewerView()
{
	// TODO: この場所に構築用のコードを追加してください。
}

CMFCViewerView::~CMFCViewerView()
{
}

BOOL CMFCViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView クラスの描画

void CMFCViewerView::OnDraw(CDC* pDC){
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	app.Draw();
}
/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView クラスの診断

#ifdef _DEBUG
void CMFCViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCViewerDoc* CMFCViewerView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCViewerDoc)));
	return (CMFCViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView クラスのメッセージ ハンドラ

BOOL CMFCViewerView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	return true;
}

void CMFCViewerView::OnInitialUpdate(){
	CView::OnInitialUpdate();
	
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	app.Init(m_hWnd);
}

void CMFCViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	app.OnKeyDown(nChar);
}

void CMFCViewerView::OnIdle(){
	app.Step();
	static int sleep;
	sleep++;
	if (sleep>10){
		Invalidate();
		sleep=0;
	}
}
