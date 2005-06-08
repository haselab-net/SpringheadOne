// DynaHapticView.cpp : CDynaHapticView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"

#include "DynaHaptic.h"
#include "DynaHapticDoc.h"
#include "DynaHapticView.h"
#include "HisConfig.h"

#include <Physics/PHSolid.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <GraphicsGL/GLRender.h>
#include <WinBasis/WBPath.h>
//	for haptics
#include <WinBasis/WBMMTimer.h>
#include <WinBasis/WBPreciseTimer.h>
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>

//	for shooting
#include <Graphics/GRSphere.h>
#include <Collision/CDSphere.h>
#include <Collision/CDCollisionEngine.h>
#include <Physics/PHForceField.h>
#include <Physics/PHContactEngine.h>
#include <Physics/PHJoint.h>

#include <string.h>
#include <fstream>
#include <sstream>
#include <strstream>
#include ".\dynahapticview.h"
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int totalTime;
namespace Spr{
extern int detectTime;
extern int analyzeTime;
extern int refTime;
extern int fricTime;
};

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView

IMPLEMENT_DYNCREATE(CDynaHapticView, CView)

BEGIN_MESSAGE_MAP(CDynaHapticView, CView)
	//{{AFX_MSG_MAP(CDynaHapticView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(RENDER_D3D, OnD3d)
	ON_COMMAND(WGL_RENDER, OnGL)
	ON_COMMAND(ID_HIS_CONFIG, OnHisConfig)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()


class CDynaApp: public Spr::FWAppD3D{
};

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView �N���X�̍\�z/����

CDynaHapticView::CDynaHapticView()
{
	app = new CDynaApp;
	// SPIDAR�̏��������s��
	ReadRegistry();
	WriteRegistry();
	InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
}

void CDynaHapticView::ReadRegistry(){
	dlConfig.hisList = AfxGetApp()->GetProfileString("his_config", "his_list", "Spidar4\r\nSpidar4 reverse\r\nSpidarG6C\r\nSpidarG6C\r\nMouse");
	std::string hisPs(AfxGetApp()->GetProfileString("his_config", "his_posScale", "1.0"));
	std::string hisFs(AfxGetApp()->GetProfileString("his_config", "his_forceScale", "1.0"));
	std::string timeStep(AfxGetApp()->GetProfileString("his_config", "his_timeStep", "0.005"));
	std::istringstream(hisPs) >> dlConfig.posScale;
	std::istringstream(hisFs) >> dlConfig.forceScale;
	std::istringstream(timeStep) >> dlConfig.timeStep;
}
void CDynaHapticView::WriteRegistry(){
	AfxGetApp()->WriteProfileString("his_config", "his_list", dlConfig.hisList);
	char buf[256];
	std::ostrstream(buf, sizeof(buf)) << dlConfig.posScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_posScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.forceScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_forceScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.timeStep << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_timeStep", buf);
}

CDynaHapticView::~CDynaHapticView(){
}

BOOL CDynaHapticView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView �N���X�̕`��

void CDynaHapticView::OnDraw(CDC* pDC){
	CDynaHapticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	app->Draw();
}
/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView �N���X�̐f�f

#ifdef _DEBUG
void CDynaHapticView::AssertValid() const
{
	CView::AssertValid();
}

void CDynaHapticView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynaHapticDoc* CDynaHapticView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynaHapticDoc)));
	return (CDynaHapticDoc*)m_pDocument;
}
#endif //_DEBUG

void CDynaHapticView::InitHis(std::string hisList, float ps, float fs){
	app->ClearHis();
	std::istringstream istr(hisList);
	while(istr.good()){
		char line[1024];
		istr.getline(line, sizeof(line));
		app->AddHis(line);
	}
	for(unsigned i=0; i<app->pointers.size(); ++i){
		app->pointers[i]->SetScale(ps, fs);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView �N���X�̃��b�Z�[�W �n���h��
void CDynaHapticView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch(nChar){
	case '8':	//	����ł��o��
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
void CDynaHapticView::ThrowSphere(float r){
	static UTRef<PHSolid> solids[10];
	static int pos=0;
	UTRef<PHSolid> solid = solids[pos];
	if (!solid) solid = new PHSolid;
	solid->SetMass(r*r*20);
	Matrix3f in = Matrix3f::Unit();
	in *= float(0.4f* solid->GetMass() *r*r);
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
	if (app->scene) app->scene->GetBehaviors().Find(ce);
	if (ce){
		SGFrame* frCol = ce->GetFrame(0);
		if (frCol && frCol->GetParent()) frCol->GetParent()->AddChildObject(frame, app->scene);
	}else{
		app->scene->GetWorld()->AddChildObject(frame, app->scene);
	}
	Affinef afSphere = app->mouse->GetAxis().inv();
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
		sphere->AddChildObject(mat, app->scene);
	}
	sphere->radius = r;
	sphere->slices = 16;
	sphere->stacks = 8;
	frame->AddChildObject(sphere, app->scene);
	app->render->InitTree(frame, app->scene);

	if (!csphere) csphere = new CDSphere;
	csphere->radius = r;
	csphere->pmaterial = new CDPhysicalMaterial;
	csphere->pmaterial->pMatData.reflexDamper = 0.1f;
	csphere->pmaterial->pMatData.reflexSpring = 1.0f;
	frame->AddChildObject(csphere, app->scene);
	frame->CalcBBox();

	PHSolidContainer* sc;
	app->scene->GetBehaviors().Find(sc);
	if (sc && !sc->solids.Find(solid)) sc->AddChildObject(solid, app->scene);
	PHGravityEngine* ge;
	app->scene->GetBehaviors().Find(ge);
	if (ge && !ge->solids.Find(solid)) ge->AddChildObject(solid, app->scene);

	solid->SetVelocity(-afSphere.Ez() * 8.0f);
	solid->SetAngularVelocity(Vec3f());

	PHContactEngine* pce;
	app->scene->GetBehaviors().Find(pce);
	if (pce && !pce->solids.Find(solid)){
		pce->AddChildObject(solid, app->scene);
		pce->Init(app->scene);
	}
	solids[pos] = solid;
	pos++;
	if (pos >= 10) pos = 0;
}

BOOL CDynaHapticView::OnEraseBkgnd(CDC* pDC) 
{
	// MFC�̓f�t�H���g�Ŕw�i�𔒂��h��D������̂ŉ������Ȃ��֐��ŃI�[�o�[���C�h�D
	return true;
}


void CDynaHapticView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	app->Init(m_hWnd);
	//	�`��p�^�C�}�[�̐ݒ�
	drawTimerId = SetTimer(FWApp::TIMER_DRAW, 33, NULL);
}

//	�^�C�}�[�ɌĂ΂��֐��D�V�~�����[�V�����Ɨ͊o�񎦂��s���D
void CDynaHapticView::Step(){
	//	�����ŃV�~�����[�V�����̃X�e�b�v�����ׂĎ��s���Ă���D
	//	SGBehaviorEngine �̔h���I�u�W�F�N�g�����ԂɌĂ΂�ăV�~�����[�V���������s����D
	detectTime = 0;
	analyzeTime = 0;
	refTime = 0;
	fricTime = 0;
	totalTime = 0;
	static WBPreciseTimer ptimer;
	ptimer.CountUS();
	app->Step();
	totalTime = ptimer.CountUS();
#if 0
	if (app->scene){
		PHJointEngine* e;
		app->scene->GetBehaviors().Find(e);
		if (e){
			DWORD t = e->timer.Clear();
			
			DSTR << t << " " << e->timer.CPUFrequency() << std::endl;
		}
	}
#endif
}

void CDynaHapticView::OnTimer(UINT nIDEvent){
	if (nIDEvent == FWApp::TIMER_DRAW){
		Invalidate();
	}
}

void CDynaHapticView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	app = NULL;
}

void CDynaHapticView::OnD3d(){
	app->CreateD3DRender();
}

void CDynaHapticView::OnGL(){
	app->CreateGLRender();
}

BOOL CDynaHapticView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	app->PreviewMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}

void CDynaHapticView::OnHisConfig(){
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	ReadRegistry();
	if (dlConfig.DoModal() == IDOK){
		WriteRegistry();
		InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
		if (app->scene) app->scene->SetTimeStep(dlConfig.timeStep);
		app->ConnectHis();
	}
}


//-> �֌��ɂ��ύX (2005/1/05)
void CDynaHapticView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	app->mouse->SetViewSize(cx, cy);
}
//<-
