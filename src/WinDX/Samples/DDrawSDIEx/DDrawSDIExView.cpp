// DDrawSDIExView.cpp : WXDDrawSDIExView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "DDrawSDIEx.h"

#include "DDrawSDIExDoc.h"
#include "DDrawSDIExView.h"
#include "math.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView

IMPLEMENT_DYNCREATE(WXDDrawSDIExView, CView)

BEGIN_MESSAGE_MAP(WXDDrawSDIExView, CView)
	//{{AFX_MSG_MAP(WXDDrawSDIExView)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView クラスの構築/消滅

WXDDrawSDIExView::WXDDrawSDIExView()
{
	// TODO: この場所に構築用のコードを追加してください。
	bFlip = false;
}

WXDDrawSDIExView::~WXDDrawSDIExView()
{
}

BOOL WXDDrawSDIExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView クラスの描画

void WXDDrawSDIExView::OnDraw(CDC* pDC)
	{
	WXDDrawSDIExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	}

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView クラスの診断

#ifdef _DEBUG
void WXDDrawSDIExView::AssertValid() const
{
	CView::AssertValid();
}

void WXDDrawSDIExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

WXDDrawSDIExDoc* WXDDrawSDIExView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(WXDDrawSDIExDoc)));
	return (WXDDrawSDIExDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView クラスのメッセージ ハンドラ

void WXDDrawSDIExView::OnInitialUpdate(){
	CView::OnInitialUpdate();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	for(int i=0; i<ddraws.Size(); i++) ddraws[i].HWnd(m_hWnd);
	ddraws[0].Create();
	surfFront.DDraw(ddraws[0]);
	surfFlip.DDraw(ddraws[0]);
	ddraws[0].HWnd();
	surfFront.HWnd(m_hWnd);
	surfFlip.HWnd(m_hWnd);
	SetTimer(10, 1, NULL);
}

void WXDDrawSDIExView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
 	if ('0' <= nChar && nChar <= '9')
		{
		int x = nChar-'0';
		ddraws[x].Fullscreen(640,480,16);
		surfFlip.DDraw(ddraws[x]);
		surfFront.DDraw(ddraws[x]);
		surfFlip.FitWindow(CPoint(100,100), CSize(100,100));
		surfFront.FitWindow(CPoint(100,100), CSize(100,100));
		}
 	if (nChar == (UINT)'F')
		{
		int i;
		for(i=0; i<ddraws.Size(); i++)
			{
			if (ddraws[i].IsFullscreen())break;
			}
		if (i==ddraws.Size())
			{
			ddraws[0].Fullscreen(640,480,16);
			}
		else
			{
			ddraws[0].Window();
			surfFlip.DDraw(ddraws[0]);
			surfFront.DDraw(ddraws[0]);
			}
		}
 	if (nChar == (UINT)'D') bFlip = !bFlip;
	}

void WXDDrawSDIExView::OnTimer(UINT nIDEvent) 
	{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	Spr::WXDDSurfaceBase* pSurf;
	if (bFlip) pSurf = &surfFlip;
	else pSurf = &surfFront;
	pSurf->Clear(pSurf->ColorToPixel(RGB(0,100,120)));
	static double theta=0;
	theta += 5.0 * 3.14 / 360.0;
	CPoint pt;
	pt.x = cos(theta) * 90 + 100;
	pt.y = sin(theta) * 90 + 100;
	if (pSurf->Lock(pt,CSize(50,50)))
		{
		for(int i=0; i<pSurf->NLine(); i++)
			{
			memset(pSurf->Line(i), 0x75, pSurf->LineLen());
			}
		pSurf->Unlock();
		}
	if (bFlip) surfFlip.Flip();
	CView::OnTimer(nIDEvent);
	}

void WXDDrawSDIExView::OnSize(UINT nType, int cx, int cy) 
	{
	CView::OnSize(nType, cx, cy);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if (surfFlip.pDDS)
		{
		surfFlip.Release();
		surfFlip.Create();
		}
	}
