// D3DDPExView.cpp : WXD3DPExView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "D3DDPEx.h"

#include "D3DDPExDoc.h"
#include "D3DDPExView.h"
#include "math.h" 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExView

IMPLEMENT_DYNCREATE(WXD3DPExView, CView)

BEGIN_MESSAGE_MAP(WXD3DPExView, CView)
	//{{AFX_MSG_MAP(WXD3DPExView)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExView クラスの構築/消滅

WXD3DPExView::WXD3DPExView()
{
	// TODO: この場所に構築用のコードを追加してください。
}

WXD3DPExView::~WXD3DPExView()
{
}

BOOL WXD3DPExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExView クラスの描画

void WXD3DPExView::OnDraw(CDC* pDC)
	{
	WXD3DPExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	}

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExView クラスの診断

#ifdef _DEBUG
void WXD3DPExView::AssertValid() const
{
	CView::AssertValid();
}

void WXD3DPExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

WXD3DPExDoc* WXD3DPExView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(WXD3DPExDoc)));
	return (WXD3DPExDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WXD3DPExView クラスのメッセージ ハンドラ

void WXD3DPExView::OnInitialUpdate() 
	{
	CView::OnInitialUpdate();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	for(int i=0; i<ddraws.Size(); i++) ddraws[i].HWnd(m_hWnd);
	d3ddev.DDraw(ddraws[0]);
	d3ddev.HWnd(m_hWnd);
	d3ddev.Create();
	

	SetTimer(10, 1, NULL);
	}

void WXD3DPExView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
 	if ('0' <= nChar && nChar <= '9'){
		int x = nChar-'0';
		ddraws[x].Fullscreen(640,480,16);
		d3ddev.DDraw(ddraws[x]);
		d3ddev.FitWindow(CPoint(100,100), CSize(100,100));
	}
 	if (nChar == (UINT)'F'){
		int i;
		for(i=0; i<ddraws.Size(); i++){
			if (ddraws[i].IsFullscreen())break;
		}	
		if (i==ddraws.Size()){
			ddraws[0].Fullscreen(640,480,16);
		}else{
			ddraws[0].Window();
			d3ddev.DDraw(ddraws[0]);
		}
	}
}

void WXD3DPExView::OnTimer(UINT nIDEvent){
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	d3ddev.Clear(0x000000ff);

	d3ddev.Viewport(WXD3Viewport(Vec3f(0,0,0)));

    WXD3Affine afW;
    d3ddev->SetTransform( D3DTRANSFORMSTATE_WORLD, afW );

    // The view matrix defines the position and orientation of the camera.
    // Here, we are just moving it back along the z-axis by 10 units.
    WXD3Affine afCam;
	afCam.Pos() = WXD3Vec3(500, 0, 500);
	afCam.LookAtGL(Vec3f());
    d3ddev->SetTransform( D3DTRANSFORMSTATE_VIEW, WXD3Affine(afCam.inv()) );

    // Enable z-buffering.
    d3ddev->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );

	d3ddev->LightEnable(0, true);
	WXD3Light l;
	d3ddev->SetLight(0, &l);
    d3ddev->SetRenderState( D3DRENDERSTATE_LIGHTING, TRUE );
	
    D3DMATERIAL7 mtrl;
    ZeroMemory( &mtrl, sizeof(mtrl) );
    mtrl.diffuse.r = mtrl.diffuse.g = mtrl.diffuse.b = 1.0f;
    mtrl.ambient.r = mtrl.ambient.g = mtrl.ambient.b = 1.0f;
    d3ddev->SetMaterial( &mtrl );

	float vtxs[][6] = {
		{0,    0, 0,   0, 0, -1},
		{0,  100, 0,   0, 1, -5},
		{100,  0, 0,   1, 0, -5},
	};

	WXCHECK(d3ddev->BeginScene());
	WXCHECK(d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_XYZ | D3DFVF_NORMAL , vtxs, sizeof(vtxs) / sizeof(vtxs[0]), D3DDP_WAIT));
	WXCHECK(d3ddev->EndScene());
	d3ddev.Flip();
	CView::OnTimer(nIDEvent);
}

void WXD3DPExView::OnSize(UINT nType, int cx, int cy){
	CView::OnSize(nType, cx, cy);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if (d3ddev.pDDS) d3ddev.Release();
}
