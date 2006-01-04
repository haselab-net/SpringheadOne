// MFCViewerView.cpp : CMFCViewerView �N���X�̓���̒�`���s���܂��B
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
// CMFCViewerView �N���X�̍\�z/����

CMFCViewerView::CMFCViewerView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
}

CMFCViewerView::~CMFCViewerView()
{
}

BOOL CMFCViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView �N���X�̕`��

void CMFCViewerView::OnDraw(CDC* pDC){
	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
	app.Draw();
}
/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView �N���X�̐f�f

#ifdef _DEBUG
void CMFCViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCViewerDoc* CMFCViewerView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCViewerDoc)));
	return (CMFCViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCViewerView �N���X�̃��b�Z�[�W �n���h��

BOOL CMFCViewerView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	return true;
}

void CMFCViewerView::OnInitialUpdate(){
	CView::OnInitialUpdate();
	
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
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
