// CMFCD3DDynaView.cpp : CMFCD3DDynaView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include <afxdlgs.h>
#include "MFCD3DDyna.h"
#include "MFCD3DDynaDoc.h"
#include "MFCD3DDynaView.h"

#include <Physics/PHSolid.h>
#include <Graphics/GRCamera.h>
#include <ImpD3D/D3Render.h>
#include <GraphicsGL/GLRender.h>
#include <FrameWork/FWFileIOD3D.h>
#include <FrameWork/FWPointer.h>
#include <WinBasis/WBMMTimer.h>

#include <Physics/PHContactEngine.h>
#include <Physics/PHForceField.h>
#include <Graphics/GRMaterial.h>
#include <Graphics/GRSphere.h>
#include <Collision/CDSphere.h>
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#else

//	If you need stable update rate for haptic interfaces, uncomment this
//	and use multi media timer.
//	#define USE_MMTIMER

#endif


volatile int stepCount;

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaView

IMPLEMENT_DYNCREATE(CMFCD3DDynaView, CView)

BEGIN_MESSAGE_MAP(CMFCD3DDynaView, CView)
	//{{AFX_MSG_MAP(CMFCD3DDynaView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_RENDER_D3D, OnRenderD3d)
	ON_COMMAND(ID_RENDER_GL, OnRenderGl)
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_COMMAND(ID_FILE_INSERT, OnFileInsert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaView �N���X�̍\�z/����

CMFCD3DDynaView::CMFCD3DDynaView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	bMinForce = true;
	bUpdate = false;

	// �}�E�X�̏�����
	mouse = new HIMouse;
	mouse->Init();
}

CMFCD3DDynaView::~CMFCD3DDynaView()
{
}

BOOL CMFCD3DDynaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaView �N���X�̕`��

void CMFCD3DDynaView::OnDraw(CDC* pDC){
	CMFCD3DDynaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
	if (!render) return;
	render->Render(scene);			//	�V�[���̕`��
	render->EndScene();				//	�`��I��
	render->Present();				//	�\��
	render->SetViewport(m_hWnd);	//	Viewport�̐ݒ�
	render->ClearBuffer();			//	�o�b�t�@�N���A
	render->BeginScene();			//	�`��J�n
}

void CMFCD3DDynaView::Import(std::string filename){
	//	���[�h
	static FWFileIOD3D fileio;
	fileio.Load(filename.c_str(), scene);
}
void CMFCD3DDynaView::Load(std::string filename){
	if (!scene){		//	������������ł��Ȃ��ꍇ�́C��Ń��[�h���邱�Ƃɂ��āC�t�@�C���������o���Ă����D
		loadFile=filename;
		return;
	}
#ifdef USE_MMTIMER	//	�}���`���f�B�A�^�C�}�[�̒�~
	timer.Release();
	Sleep(100);
#endif
	//  �����[�h�p�Ƀt�@�C�������o���Ă���
	loadFileName = filename;

	//	�ڐG�͕`��t���O��ۑ�
	bool bDrawForce = false;
	if (contactEngine) bDrawForce = contactEngine->bDraw;
	
	//	�V�[���̏�����
	scene->Clear();									//	�V�[���O���t�̃N���A
	scene->GetRenderers().Add(render);				//	�����_����ǉ�
	//	���[�h
	static FWFileIOD3D fileio;
	fileio.Load(filename.c_str(), scene);
	//	�J�����̏�����Ԃ��L�^
	initialCamera = render->camera->data;	
	//	�ڐG�͕`��t���O�̍Đݒ�
	contactEngine = NULL;
	scene->GetBehaviors().Find(contactEngine);
	if (contactEngine) contactEngine->bDraw = bDrawForce;
	
	//	�͊o�|�C���^���V�[���Ɛڑ�
	pointer.Init(mouse);
	pointer.Connect(scene);
	if (pointer.GetSolid()){
		// �}�E�X�f�o�C�X�̐ݒ�
		mouse->SetMass((float)pointer.GetSolid()->GetMass());
		mouse->SetPos(pointer.GetSolid()->GetFramePosition());
		mouse->SetOri(pointer.GetSolid()->GetOrientation());
	}
	mouse->SetAxis(render->camera->data.view);
	mouse->SetAxisInit(render->camera->data.view);

#ifdef USE_MMTIMER	//	�}���`���f�B�A�^�C�}�[�̋N��
	timer.Resolution(UINT(scene->GetTimeStep()*1000));
	timer.Interval(UINT(scene->GetTimeStep()*1000));
	timer.Create();
#else
	if (KillTimer(simTimerID))
		simTimerID = SetTimer(1, UINT(scene->GetTimeStep()*200), NULL);
#endif
}
void CMFCD3DDynaView::Save(std::string filename){
	if (!scene->GetWorld()) return;
	static FWFileIOD3D fileio;
	fileio.Save(filename.c_str(), scene);
}

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaView �N���X�̃��b�Z�[�W �n���h��
void CMFCD3DDynaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	Invalidate();
	static SGBehaviorStates states;
	switch(nChar){
#ifdef USE_MMTIMER
	case 'K':
		timer.Release();					//	�A�����s�̒�~
		break;
	case 'T':	
		timer.Create();						//	�A�����s�̍ĊJ
		break;
	case 'H':
		timer.Thread();						//	�A�����s�̍ĊJ
		break;
#else
	case 'K':
		KillTimer(simTimerID);				//	�A�����s�̒�~
		break;
	case 'T':								//	�A�����s�̍ĊJ
		simTimerID = SetTimer(1, (UINT)(scene->GetTimeStep()*200), NULL);
		break;
#endif
	case VK_SPACE:
		Step();								//	1�X�e�b�v���s
		break;
	case 'O':
		bMinForce = !bMinForce;				//	�͊o��ON/OFF
		break;
	case 'D':								//	�ڐG�͕`���ON/OFF
		{PHContactEngine* ce=NULL;
		scene->GetBehaviors().Find(ce);
		if (ce) ce->bDraw = !ce->bDraw;
		}break;
	case 'R':								// �t�@�C�������[�h
		Load(loadFileName.c_str());
		return;
	case 'F':								//	�t���X�N���[��
		render->EndScene();
		render->Fullscreen();
		render->BeginScene();
		break;
	case 'G':								//	�E�B���h�E���[�h
		render->EndScene();
		render->Window();
		render->BeginScene();
		break;
	case 'P':
		scene->GetWorld()->Print(DSTR);
		break;
	case '1':
		scene->SaveState(states);
		break;
	case '2':
		scene->LoadState(states);
		break;
	//	����ł��o��
	case '8':
		ThrowSphere(0.2f);
		break;
	case '9':
		ThrowSphere(0.3f);
		break;
	case '0':
		ThrowSphere(0.4f);
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CMFCD3DDynaView::ThrowSphere(float r){
	static UTRef<PHSolid> solids[10];
	static int pos=0;
	UTRef<PHSolid> solid = solids[pos];
	if (!solid) solid = new PHSolid;
	solid->SetMass(r*r*20);
	Matrix3f in = Matrix3f::Unit();
	in *= 0.4f* solid->GetMass() *r*r;
	solid->SetInertia(in);

	UTRef<SGFrame> frame = solid->GetFrame();
	UTRef<GRSphere> sphere;
	UTRef<CDSphere> csphere;
	if (frame){
		frame->FindObject(csphere);
		frame->FindObject(sphere);
		frame->Clear();
	}else{
		frame = new SGFrame;
		solid->SetFrame(frame);
	}
	CDCollisionEngine* ce;
	scene->GetBehaviors().Find(ce);
	if (ce){
		SGFrame* frCol = ce->GetFrame(0);
		if (frCol && frCol->GetParent()) frCol->GetParent()->AddChildObject(frame, scene);
	}else{
		scene->GetWorld()->AddChildObject(frame, scene);
	}
	Affinef afSphere = mouse->GetAxis().inv();
	afSphere.Ey().X() = 0;
	afSphere.Ey().Z() = 0;
	afSphere.Ey().unitize();
	afSphere.Ex().Y() = 0;
	afSphere.Ex().unitize();
	afSphere.Ez().Y() = 0;
	afSphere.Ez().unitize();
	afSphere.Pos() -= 3*afSphere.Ez();
	frame->SetPosture(afSphere);

	if (!sphere){
		sphere = new GRSphere;
		UTRef<GRMaterial> mat = new GRMaterial;
		mat->diffuse = Vec4f(0, 0.1f, 1.0f, 1.0f);
		mat->specular= Vec4f(0, 0.1f, 1.0f, 1.0f);
		mat->ambient = Vec4f(0, 0.1f, 0.1f, 1.0f);
		mat->emissive= Vec4f(0, 0.1f, 0.1f, 1.0f);
		sphere->AddChildObject(mat, scene);
	}
	sphere->radius = r;
	sphere->slices = 16;
	sphere->stacks = 8;
	frame->AddChildObject(sphere, scene);
	render->InitTree(frame, scene);

	if (!csphere) csphere = new CDSphere;
	csphere->radius = r;
	csphere->pmaterial = new CDPhysicalMaterial;
	csphere->pmaterial->pMatData.reflexDamper = 0.3f;
	csphere->pmaterial->pMatData.reflexSpring = 3.0f;
	frame->AddChildObject(csphere, scene);

	PHSolidContainer* sc;
	scene->GetBehaviors().Find(sc);
	if (sc && !sc->solids.Find(solid)) sc->AddChildObject(solid, scene);
	PHGravityEngine* ge;
	scene->GetBehaviors().Find(ge);
	if (ge && !ge->solids.Find(solid)) ge->AddChildObject(solid, scene);

	solid->SetVelocity(-afSphere.Ez() * 8.0f);
	solid->SetAngularVelocity(Vec3f());

	PHContactEngine* pce;
	scene->GetBehaviors().Find(pce);
	if (pce && !pce->solids.Find(solid)){
		pce->AddChildObject(solid, scene);
		pce->Init(scene);
	}
	solids[pos] = solid;
	pos++;
	if (pos >= 10) pos = 0;
}

BOOL CMFCD3DDynaView::OnEraseBkgnd(CDC* pDC){ return true; }

void TimerFunc(void* arg){
	CMFCD3DDynaView* view = (CMFCD3DDynaView*) arg;
	view->Step();
}

void CMFCD3DDynaView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	if (!render){
		render = new D3Render;
		render->Create(m_hWnd);
		scene = new SGScene();
		scene->GetRenderers().Add(render);
		mouse->SetAxis(render->camera->data.view);
		scene->SetTimeStep(0.005f);
	}
	timer.Resolution(UINT(scene->GetTimeStep()*1000));
	timer.Interval(UINT(scene->GetTimeStep()*1000));
#ifdef USE_MMTIMER
	timer.Set(TimerFunc, this);
	timer.Thread();
#else
	simTimerID = SetTimer(1, UINT(scene->GetTimeStep()*200), NULL);
#endif
	drawTimerID = SetTimer(2, 30, NULL);
	
	if (loadFile.length()){	//	���[�h�����������t�@�C��������΂����Ń��[�h
		Load(loadFile);
		loadFile = "";
	}
}
void CMFCD3DDynaView::UpdateMouse(){
	if (mouse->IsGood()){
		pointer.Step(scene->GetTimeStep());
		// �J�������}�E�X�̊���W���ɍX�V
		render->camera->data.view = mouse->GetAxis();
	}
}

void CMFCD3DDynaView::Step(){
	stepCount ++;
	scene->Step();
	UpdateMouse();
	bUpdate = true;
}

void CMFCD3DDynaView::OnTimer(UINT nIDEvent){
	if (nIDEvent == 1){
		for(int i=0; i<5; ++i) Step();
	}else{
		if (bUpdate){
			bUpdate = false;
			Invalidate();
		}
	}
}
void CMFCD3DDynaView::OnDestroy(){
	CView::OnDestroy();	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	timer.Release();
}

/////////////////////////////////////////////////////////////////////////////
// CMFCD3DDynaView �N���X�̐f�f

#ifdef _DEBUG
void CMFCD3DDynaView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCD3DDynaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCD3DDynaDoc* CMFCD3DDynaView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCD3DDynaDoc)));
	return (CMFCD3DDynaDoc*)m_pDocument;
}
#endif //_DEBUG


void CMFCD3DDynaView::OnRenderD3d() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if (render) render->EndScene();
	render = new D3Render;
	render->Create(m_hWnd);
	render->BeginScene();
	Load(loadFileName.c_str());
}

void CMFCD3DDynaView::OnRenderGl() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	if (render) render->EndScene();
	render = new GLRender;
	render->Create(m_hWnd);	
	render->BeginScene();
	Load(loadFileName.c_str());
}

BOOL CMFCD3DDynaView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if (mouse->PreviewMessage(pMsg)){
		UpdateMouse();
	}
	return CView::PreTranslateMessage(pMsg);
}

void CMFCD3DDynaView::OnFileInsert() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CFileDialog dlg(true);
	if (dlg.DoModal() == IDOK){
		Import((const char*)dlg.GetPathName());
	}
}
