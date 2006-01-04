// DDrawSDIExView.cpp : WXDDrawSDIExView �N���X�̓���̒�`���s���܂��B
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
// WXDDrawSDIExView �N���X�̍\�z/����

WXDDrawSDIExView::WXDDrawSDIExView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	bFlip = false;
}

WXDDrawSDIExView::~WXDDrawSDIExView()
{
}

BOOL WXDDrawSDIExView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView �N���X�̕`��

void WXDDrawSDIExView::OnDraw(CDC* pDC)
	{
	WXDDrawSDIExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
	}

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView �N���X�̐f�f

#ifdef _DEBUG
void WXDDrawSDIExView::AssertValid() const
{
	CView::AssertValid();
}

void WXDDrawSDIExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

WXDDrawSDIExDoc* WXDDrawSDIExView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(WXDDrawSDIExDoc)));
	return (WXDDrawSDIExDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WXDDrawSDIExView �N���X�̃��b�Z�[�W �n���h��

void WXDDrawSDIExView::OnInitialUpdate(){
	CView::OnInitialUpdate();
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if (surfFlip.pDDS)
		{
		surfFlip.Release();
		surfFlip.Create();
		}
	}
