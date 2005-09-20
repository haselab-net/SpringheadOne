// BalanceView.cpp : CBalanceView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "Balance.h"
#include "BalanceDoc.h"
#include "BalanceView.h"
#include <ImpD3D/D3Render.h>
#include <Physics/PHSolid.h>
#include <Graphics/GRCamera.h>
#include <fstream>
#include <WinBasis/WBMMTimer.h>
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#else

#define USE_MMTIMER

#endif


volatile int stepCount;

/////////////////////////////////////////////////////////////////////////////
// CBalanceView

IMPLEMENT_DYNCREATE(CBalanceView, CView)

BEGIN_MESSAGE_MAP(CBalanceView, CView)
	//{{AFX_MSG_MAP(CBalanceView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBalanceView �N���X�̍\�z/����

CBalanceView::CBalanceView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	dt = 0.01f;
	bMinForce = true;

	// �}�E�X�̏�����
	mouse.Init();
	// �X�V�����ƃ}�E�X�̃X�P�[����ݒ�
	mouse.Setdt(dt);
	mouse.SetScale(0.002f, 0.008f, 0.006f, 0.008f);	// SetScale(�|�C���^�̕��i�C��]�C�J�����̕��i�C��])

}

CBalanceView::~CBalanceView()
{
}

BOOL CBalanceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBalanceView �N���X�̕`��

void CBalanceView::OnDraw(CDC* pDC){
	CBalanceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
	if (!render) return;

	render->Render(scene);
	render->EndScene();
	render->Present();
	render->SetViewport(m_hWnd);
	render->ClearBuffer();
	render->BeginScene();
}
/////////////////////////////////////////////////////////////////////////////
// CBalanceView �N���X�̐f�f

#ifdef _DEBUG
void CBalanceView::AssertValid() const
{
	CView::AssertValid();
}

void CBalanceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBalanceDoc* CBalanceView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBalanceDoc)));
	return (CBalanceDoc*)m_pDocument;
}
#endif //_DEBUG

void CBalanceView::Load(std::string filename){
	if (!scene){		//	������������ł��Ȃ��ꍇ�́C��Ń��[�h���邱�Ƃɂ��āC�t�@�C���������o���Ă����D
		loadFile=filename;
		return;
	}
	//  �����[�h�p�Ƀt�@�C�������o���Ă���
	loadFileName = filename;

	render->EndScene();
#ifdef USE_MMTIMER
	timer.Release();
	Sleep(100);
#endif
	scene->Clear();
	scene->GetRenderers().Add(render);

//	doc->Print(DSTR);
	// ���[�_�[�̏�����
	FILoadScene loader;
	REGISTER_LOADER(&loader, SceneGraph);			//	�V�[���O���t�̃��[�_��o�^
	REGISTER_LOADER(&loader, ImpD3D);				//	D3D�̃m�[�h�̃��[�_��o�^
	REGISTER_LOADER(&loader, Collision);			//	�Փ˔���m�[�h�̃��[�_��o�^
	REGISTER_LOADER(&loader, Physics);				//	�����V�~�����[�^�̃��[�_��o�^
	REGISTER_LOADER(&loader, Graphics);				//	�O���t�B�b�N�X�̃��[�_��o�^
	//	�t�@�C������h�L�������g�I�u�W�F�N�g�����[�h
	D3FileDoc fileDoc;			//	�t�@�C�����h�L�������g�I�u�W�F�N�g�ɕϊ�����I�u�W�F�N�g
	fileDoc.renderer = (D3Render*)render;
	fileDoc.Load(filename);		//	�t�@�C���̃��[�h
	//	�h�L�������g�I�u�W�F�N�g����V�[���O���t���쐬
	loader.Load(scene, &fileDoc);
	GRCamera* cam = NULL;
	if (scene->GetRenderers().GetObjects().Find(cam)){
		scene->GetRenderers().Set(new GRCamera);	
	}
	render->camera->loadData = render->camera->data;

//	scene->GetWorld()->Print(DSTR);

	pointer = (PHSolid*)scene->FindObject("Pointer");
	if (pointer){
		pointer->SetFramePosition(Vec3f());
		pointer->SetOrientation(Quaternionf());
		pointer->SetAngularVelocity(Vec3f());
		pointer->SetVelocity(Vec3f());
		pointer->GetFrame()->SetPosture(Affinef());	
		// ���[�h����t�@�C�����ƂɈقȂ�f�[�^��ݒ�
		mouse.SetMass(pointer->GetMass());
		mouse.SetPos(pointer->GetFramePosition());
		mouse.SetOri(pointer->GetOrientation());
	}
	mouse.SetAxis(render->camera->data.view);

	human.Connect(scene);

#ifdef USE_MMTIMER
	timer.Create();
#endif
}
void CBalanceView::Save(const char* filename){
	if (!scene->GetWorld()) return;
	D3SaveScene saver;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	REGISTER_SAVER(&saver, SceneGraph);				//	�V�[���O���t�̃��[�_��o�^
	REGISTER_SAVER(&saver, ImpD3D);					//	D3D�̃m�[�h�̃��[�_��o�^
	REGISTER_SAVER(&saver, Collision);				//	�Փ˔���m�[�h�̃��[�_��o�^
	REGISTER_SAVER(&saver, Physics);				//	�����V�~�����[�^�̃��[�_��o�^
	REGISTER_SAVER(&saver, Graphics);				//	�O���t�B�b�N�X�̃��[�_��o�^
	saver.Save(doc, scene);
	D3FileDoc fileDoc;
	for (unsigned int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(filename, doc->Child(i));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBalanceView �N���X�̃��b�Z�[�W �n���h��
void CBalanceView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	Affinef afBody = mouse.GetAxis().inv();
	float df = 1.0f ; //�ړ��ω���
	if ( nChar == VK_SPACE ) Step();						//	1�X�e�b�v���s
#ifdef USE_MMTIMER
	if ( nChar == 'K' ) timer.Release();					//	�A�����s�̒�~
	if ( nChar == 'T' )	timer.Create();						//	�A�����s�̍ĊJ
	if ( nChar == 'H' )	timer.Thread();						//	�A�����s�̍ĊJ
#else
	if ( nChar == 'K' ) KillTimer(wmTimerID);				//	�A�����s�̒�~
	if ( nChar == 'T' )	wmTimerID = SetTimer(1, 5, NULL);	//	�A�����s�̍ĊJ
#endif
	// �J�����̕��s�ړ�
	if ( nChar == 'Q' ) afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	if ( nChar == 'W' ) afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	if ( nChar == 'Z' ) afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	if ( nChar == 'A' ) afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	if ( nChar == VK_DOWN ) afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	if ( nChar == VK_UP ) afBody.Pos() = afBody.Pos() + afBody.Ez() * df;

	// �J�����̉�]
	if ( nChar == VK_LEFT ) afBody = Affinef::Rot(Radf(-5),afBody.Ey()) * afBody;
	if ( nChar == VK_RIGHT ) afBody = Affinef::Rot(Radf(5),afBody.Ey()) * afBody;
	if ( nChar == 'S' ) afBody = Affinef::Rot(Radf(-5),afBody.Ex()) * afBody;
	if ( nChar == 'X' ) afBody = Affinef::Rot(Radf(5),afBody.Ex()) * afBody;

	// �t�@�C�������[�h
	if ( nChar == 'R' ){
		Load(loadFileName.c_str());
		return;
	}

	if ( nChar == 'N' ){
		if (dt > 0.01f) dt -= 0.01f;
		else dt -= 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	if ( nChar == 'M' ){
		if (dt >= 0.01f) dt += 0.01f;
		else dt += 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	if (nChar == 'F'){
		render->EndScene();
		render->device.Fullscreen();
		
		RECT rc;
		GetClientRect(&rc);
		render->Setup( Vec2f(rc.right, rc.bottom) );
		render->device->ShowCursor(false);
		render->BeginScene();
	}
	if (nChar == 'G'){
		render->EndScene();
		render->device.Window();

		RECT rc;
		GetClientRect(&rc);
		render->Setup( Vec2f(rc.right, rc.bottom) );
		render->device->ShowCursor(true);
		render->BeginScene();
	}
	// �|�C���^�ƃJ���������Z�b�g
	if ( nChar == 'C' ){
		afBody = render->camera->loadData.view.inv();
		pointer->SetFramePosition(Vec3f());
		pointer->SetOrientation(Quaternionf());
		pointer->SetAngularVelocity(Vec3f());
		pointer->SetVelocity(Vec3f());
		pointer->GetFrame()->SetPosture(Affinef());	
	}
	if ( nChar == VK_SPACE ) bMinForce = !bMinForce;

	mouse.SetAxis(afBody.inv());
	mouse.Update();		// ������Ă���{�^���̏�Ԃ���ʒu�E�p�����X�V
	render->camera->data.view = mouse.GetAxis();
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CBalanceView::OnEraseBkgnd(CDC* pDC) 
{

	
	return true;
}

void TimerFunc(void* arg){
	CBalanceView* view = (CBalanceView*) arg;
	view->Step();
}
void CBalanceView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	if (!render){
		render = new D3Render;
		render->device.Create(m_hWnd);
		scene = new SGScene();
		scene->GetRenderers().Add(render);
		scene->GetRenderers().Set(new GRCamera);
		timer.Resolution(2);
		timer.Interval(2);
#ifdef USE_MMTIMER
		timer.Set(TimerFunc, this);
		timer.Thread();
#else
		wmTimerID = SetTimer(1, 5, NULL);
#endif
	}
	
	if (loadFile.length()){	//	���[�h�����������t�@�C��������΂����Ń��[�h
		Load(loadFile);
		loadFile = "";
	}
}

void CBalanceView::Step(){
	stepCount ++;

	human.Step();
	scene->ClearForce(dt);
	scene->GenerateForce(dt);
	scene->Integrate(dt);

	if (mouse.IsGood()){
		mouse.Update();		// ������Ă���{�^���̏�Ԃ���ʒu�E�p�����X�V

		// �J�������}�E�X�̊���W���ɍX�V
		render->camera->data.view = mouse.GetAxis();
		
		//	�|�C���^�̈ʒu�Ǝp����mouse����擾
		Vec3f pos = mouse.GetPos();
		Quaternionf ori = mouse.GetOri();

		if (pointer){
			//	�͂ƃg���N���V�~�����[�^�̃m�[�h(pointer)����擾
			Vec3f force, torque;
			force = pointer->GetForce() ;
			torque = pointer->GetTorque() ;
			
			DSTR << "force = " << force << std::endl;

			//	�񎦗͂̐ݒ�
			if (bMinForce) mouse.SetMinForce();
			else mouse.SetForce(force, torque);
			
			//	solid�Ɉʒu�ƌ�����ݒ�D
			pointer->SetFramePosition(pos*15.0f);
			pointer->SetOrientation(ori);
			//	���x��0�ɂ��Ă���D(�{���̓}�E�X�̑��x����ꂽ�����ǂ�)
			pointer->SetAngularVelocity(Vec3f());
			pointer->SetVelocity(Vec3f());
			//	�V�~�����[�^�m�[�h�̈ʒu���X�V
			Affinef posture;
			Matrix3d rot;
			pointer->GetOrientation().to_matrix(rot);
			posture.Rot() = rot;
			posture.Pos() = pointer->GetFramePosition();
			pointer->GetFrame()->SetPosture(posture);
		}
	}


#ifndef USE_MMTIMER
	Invalidate();
#endif

}

void CBalanceView::OnTimer(UINT nIDEvent){
	Step();
}


void CBalanceView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	timer.Release();
}

void CBalanceView::OnIdle()
{
	Invalidate();
}


//-------- �}�E�X�̃C�x���g�̎擾 ---------------------------

void CBalanceView::OnMouseMove(UINT nFlags, CPoint point) 
{
	mouse.OnButtonMove(nFlags, point.x, point.y);

	CView::OnMouseMove(nFlags, point);
}

void CBalanceView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	mouse.OnButtonDown(point.x, point.y);

	CView::OnLButtonDown(nFlags, point);
}

void CBalanceView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	mouse.OnButtonDown(point.x, point.y);
	
	CView::OnMButtonDown(nFlags, point);
}

void CBalanceView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	mouse.OnButtonDown(point.x, point.y);
	
	CView::OnRButtonDown(nFlags, point);
}

void CBalanceView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	mouse.OnButtonUp();

	CView::OnLButtonUp(nFlags, point);
}

void CBalanceView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	mouse.OnButtonUp();
	
	CView::OnMButtonUp(nFlags, point);
}

void CBalanceView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	mouse.OnButtonUp();

	CView::OnRButtonUp(nFlags, point);
}


void CBalanceView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (nFlags == MK_SHIFT + MK_LBUTTON ){
		mouse.OnShiftLButtonDClick();
	}
	else if (nFlags == MK_LBUTTON) {
		mouse.OnLButtonDClick();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CBalanceView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if ( nFlags == MK_RBUTTON) {
		mouse.OnRButtonDClick();
	}

	CView::OnRButtonDblClk(nFlags, point);
}



