// D3DRMSDIExView.cpp : WXD3RMSDIExView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "D3DRMSDIEx.h"

#include "D3DRMSDIExDoc.h"
#include "D3DRMSDIExView.h"
#include "LVRM/WXD3D/WXRMVisual.h"
#include "LVRM/WXD3D/WXRM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExView

IMPLEMENT_DYNCREATE(WXD3RMSDIExView, CView)

BEGIN_MESSAGE_MAP(WXD3RMSDIExView, CView)
	//{{AFX_MSG_MAP(WXD3RMSDIExView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExView �N���X�̍\�z/����

WXD3RMSDIExView::WXD3RMSDIExView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
}

WXD3RMSDIExView::~WXD3RMSDIExView()
{
}

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExView �N���X�̕`��

void WXD3RMSDIExView::OnDraw(CDC* pDC){
	WXD3RMSDIExDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
	rm.Step();
}

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExView �N���X�̐f�f

#ifdef _DEBUG
void WXD3RMSDIExView::AssertValid() const
{
	CView::AssertValid();
}

void WXD3RMSDIExView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

WXD3RMSDIExDoc* WXD3RMSDIExView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(WXD3RMSDIExDoc)));
	return (WXD3RMSDIExDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// WXD3RMSDIExView �N���X�̃��b�Z�[�W �n���h��

void WXD3RMSDIExView::OnInitialUpdate(){
	CView::OnInitialUpdate();
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	ddraws.HWnd(m_hWnd);
	d3dDev.HWnd(m_hWnd);
	rm.Surf(d3dDev);
	rm.Create();
	if (rm.Create()) BuildScene();
	head = Affinef(Rad(10), 'x');
	((WXD3RMSDIExApp*) AfxGetApp())->view = this;
}
void WXD3RMSDIExView::BuildScene(){
	//	����
	WXRMFrame lights;
	lights.Add(WXRMLight(D3DRMLIGHT_AMBIENT, WXD3DRGB(0.6, 0.6, 0.6)));
	lights.Add(WXRMLight(D3DRMLIGHT_DIRECTIONAL, WXD3DRGB(0.5, 0.5, 0.5)));
	rm.Scene().Add(lights);

	//	�n�`
	WXRMFrame fFrontier;
	fFrontier.Load("island.x");
	Affinef afScale;
	afScale.Rot() *= 30;
	fFrontier.AfParent(afScale);
	rm.Scene().Add(fFrontier);

	rm.ViewFrustum(WXRMViewFrustum(3, 5000.0));
}

const REAL groundHeight	= REAL(100);
const REAL rebound		= REAL(0.7);
const REAL gravity		= REAL(0.2);		//	�d�͉����x(���̒l)
const REAL delta		= REAL(0.2);
const Vec3f airResist= Vec3f(REAL(0.02), REAL(0.02), REAL(0.02));

void WXD3RMSDIExView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
	{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	if (nChar == VK_NUMPAD8 || nChar == VK_UP)   accel = now.Rot() * Vec3f(0,0,delta);
	if (nChar == VK_NUMPAD2 || nChar == VK_DOWN) accel = now.Rot() * Vec3f(0,0,-delta);
	if (nChar == VK_NUMPAD1) now = now * Affinef(-1,0,0);
	if (nChar == VK_NUMPAWXD3) now = now * Affinef(1,0,0);
	if (nChar == VK_NUMPAD4 || nChar == VK_LEFT)  now = now * Affinef(Rad(-5), 'y');
	if (nChar == VK_NUMPAD6 || nChar == VK_RIGHT) now = now * Affinef(Rad(5), 'y');
	if (nChar == VK_NUMPAD5) head = head*Affinef(Rad(-5), 'x');
	if (nChar == VK_NUMPAD0) head = head*Affinef(Rad(5), 'x');
	if (nChar == VK_NUMPAD7) head = head*Affinef(Rad(-5), 'z');
	if (nChar == VK_NUMPAD9) head = head*Affinef(Rad(5), 'z');
	if (nChar == VK_DECIMAL) vel = Vec3f();
	if (nChar == VK_SPACE) accel = Vec3f(0, 10, 0);
	if (nChar == 'F'){
		if (ddraws.Primary().IsFullscreen()){
			ddraws.Primary().Window();
			d3dDev.RestoreWindowPlace();
		}else{
			d3dDev.SaveWindowPlace();
			ddraws.Primary().Fullscreen(640, 480, 16);
		}
	}
	if (('1' <= nChar && nChar <= '9') && ddraws.Size() > nChar - '0'){
		int i= nChar - '0';
		if (ddraws.Primary().IsFullscreen()){
			ddraws.Primary().Window();
			d3dDev.DDraw(ddraws.Primary());
		}else if (ddraws[i].IsFullscreen()){
			ddraws.Primary().Window();
			ddraws[i].Window();
			d3dDev.DDraw(ddraws.Primary());
		}else{
			ddraws[i].Fullscreen(640,480,16);
			d3dDev.DDraw(ddraws[i]);
		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void WXD3RMSDIExView::OnIdle(){
	//---------------------------------------------------------------
	//	���͏���
	//
	now = now * Affinef(Rad(mouseLDrag.X() * 0.1) ,'y');
	now = now * Affinef(0, 0, -mouseLDrag.Y() * 0.01);
	now	= now * Affinef(Rad(mouseRDrag.X() * 0.1) ,'z');
	now	= now * Affinef(Rad(mouseRDrag.Y() * 0.1) ,'x');
	//---------------------------------------------------------------
	//	���_�̈ړ�����
	//
	//	�ʒu���X�V
	vel += accel;
	accel = Vec3f();
	now.Pos() = now.Pos() + vel;
	if (now.Pos().Y() < groundHeight){
		now.PosY() = groundHeight;
		if (-vel.Y() > 4*gravity) vel.Y() = - rebound * vel.Y();
		else if (abs(vel.Y()) < 4*gravity) vel.Y() = REAL(0);
	}
	//	�d�͉���
	vel += Vec3f(REAL(0), -gravity, REAL(0));
	//	��C��R
	vel.X() -= vel.X() * airResist.X();
	vel.Y() -= vel.Y() * airResist.Y();
	vel.Z() -= vel.Z() * airResist.Z();
	//	�v���C���[�̈ʒu��ݒ�
	rm.Camera().AfParent(now);
	rm.Camera().AfParent(now * head);
	Invalidate();
}

BOOL WXD3RMSDIExView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	return true;
}

void WXD3RMSDIExView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	static UINT lastFlags;
	static CPoint pLStart, pRStart;
	if (!(lastFlags&MK_LBUTTON) && (nFlags&MK_LBUTTON)){
		pLStart = point;
		SetCapture();
	}else if (nFlags&MK_LBUTTON){
		CSize delta = point - pLStart;
		mouseLDrag.X() = delta.cx;
		mouseLDrag.Y() = delta.cy;
	}else{
		mouseLDrag = WXD3Vec2();
	}

	if (!(lastFlags&MK_RBUTTON) && (nFlags&MK_RBUTTON)){
		pRStart = point;
		SetCapture();
	}else if (nFlags&MK_RBUTTON){
		CSize delta = point - pRStart;
		mouseRDrag.X() = delta.cx;
		mouseRDrag.Y() = delta.cy;
	}else{
		mouseRDrag = WXD3Vec2();
	}
	if (nFlags == 0) ReleaseCapture();
	lastFlags = nFlags;

	CView::OnMouseMove(nFlags, point);
}
