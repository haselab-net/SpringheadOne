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

void WXD3DPExView::OnDraw(CDC* pDC){
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
struct CUSTOMVERTEX
{
    FLOAT x, y, z, rhw; // The transformed position for the vertex
    DWORD color;        // The vertex color
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

void WXD3DPExView::OnInitialUpdate(){
	CView::OnInitialUpdate();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	d3ddev.Create(m_hWnd, d3dpp);
	SetTimer(10, 1, NULL);

    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX g_Vertices[] =
    {
        { 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
        { 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
        {  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
    };

    // Create the vertex buffer. Here we are allocating enough memory
    // (from the default pool) to hold all our 3 custom vertices. We also
    // specify the FVF, so the vertex buffer knows what data it contains.
    if( FAILED( d3ddev->CreateVertexBuffer( 3*sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &pVB ) ) ){
        return;
    }

    // Now we fill the vertex buffer. To do this, we need to Lock() the VB to
    // gain access to the vertices. This mechanism is required becuase vertex
    // buffers may be in device memory.
    VOID* pVertices;
    if( FAILED( pVB->Lock( 0, sizeof(g_Vertices), (BYTE**)&pVertices, 0 ) ) ) return;
    memcpy( pVertices, g_Vertices, sizeof(g_Vertices) );
    pVB->Unlock();
}

void WXD3DPExView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
}
LPDIRECT3DVERTEXBUFFER8 pVB        = NULL; // Buffer to hold vertices


void WXD3DPExView::OnTimer(UINT nIDEvent){
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CView::OnTimer(nIDEvent);
    static bool cf;
	if (cf)	d3ddev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
    else d3ddev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,200,255), 1.0f, 0 );
    cf = !cf;

    // Begin the scene
    d3ddev->BeginScene();
    
    // Rendering of scene objects can happen here
    d3ddev->SetStreamSource( 0, pVB, sizeof(CUSTOMVERTEX) );
    d3ddev->SetVertexShader( D3DFVF_CUSTOMVERTEX );
    d3ddev->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );

    
    // End the scene
    d3ddev->EndScene();
    
    // Present the backbuffer contents to the display
    d3ddev->Present( NULL, NULL, NULL, NULL );
}

void WXD3DPExView::OnSize(UINT nType, int cx, int cy){
	CView::OnSize(nType, cx, cy);
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
}
