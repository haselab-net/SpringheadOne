// LocalDynamicsView.cpp : CLocalDynamicsView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"

#include "LocalDynamics.h"
#include "LocalDynamicsDoc.h"
#include "LocalDynamicsView.h"
#include "HisConfig.h"
#ifdef USE_IHC
#include <Device/DRUsb20Sh4.h>
#endif

#include <Physics/PHSolid.h>
#include <Graphics/GRCamera.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <GraphicsGL/GLRenderWgl.h>
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

#include <string.h>
#include <fstream>
#include <sstream>
#include <strstream>
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

enum {
	TIMER_SIMU = 1,
	TIMER_DRAW,
	TIMER_LD,
};

enum{
	TEN_TO_ONE = 1,
	ONE_TO_ONE,
};

static float simuTimerPeriod = 0.01f;
static float ldTimerPeriod   = 0.001f;
float timer_interval = 0.01f;

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsView

IMPLEMENT_DYNCREATE(CLocalDynamicsView, CView)

BEGIN_MESSAGE_MAP(CLocalDynamicsView, CView)
	//{{AFX_MSG_MAP(CLocalDynamicsView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(RENDER_D3D, OnD3d)
	ON_COMMAND(WGL_RENDER, OnGL)
	ON_COMMAND(ID_HIS_CONFIG, OnHisConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsView �N���X�̍\�z/����

CLocalDynamicsView::CLocalDynamicsView()
{
	// SPIDAR�̏��������s��
	bPause = false;
	ReadRegistry();
	WriteRegistry();
	InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
	timer.Resolution(dlConfig.timeStep*1000);
	timer.Interval(dlConfig.timeStep*1000);

	iCalculateMode = TEN_TO_ONE;
#ifndef USE_IHC
//	app.SetFWApp(&app);
	app.SetLDRenderer(&ldRenderer);
//	app.SetDT(app.scene->GetTimeStep());

	ldRenderer.SetLDDT(ldTimerPeriod);
//	ldRenderer.SetSimuDT(app.scene->GetTimeStep());
#endif
}

void CLocalDynamicsView::ReadRegistry(){
	dlConfig.hisList = AfxGetApp()->GetProfileString("his_config", "his_list", "Spidar4\r\nSpidar4 reverse\r\nSpidarG6C\r\nSpidarG6C\r\nMouse");
	std::string hisPs(AfxGetApp()->GetProfileString("his_config", "his_posScale", "1.0"));
	std::string hisFs(AfxGetApp()->GetProfileString("his_config", "his_forceScale", "1.0"));
	std::string timeStep(AfxGetApp()->GetProfileString("his_config", "his_timeStep", "0.005"));
	std::istringstream(hisPs) >> dlConfig.posScale;
	std::istringstream(hisFs) >> dlConfig.forceScale;
	std::istringstream(timeStep) >> dlConfig.timeStep;
}
void CLocalDynamicsView::WriteRegistry(){
	AfxGetApp()->WriteProfileString("his_config", "his_list", dlConfig.hisList);
	char buf[256];
	std::ostrstream(buf, sizeof(buf)) << dlConfig.posScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_posScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.forceScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_forceScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.timeStep << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_timeStep", buf);
}

CLocalDynamicsView::~CLocalDynamicsView(){
}

BOOL CLocalDynamicsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsView �N���X�̕`��

void CLocalDynamicsView::OnDraw(CDC* pDC){
	CLocalDynamicsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	app.Draw();
}
/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsView �N���X�̐f�f

#ifdef _DEBUG
void CLocalDynamicsView::AssertValid() const
{
	CView::AssertValid();
}

void CLocalDynamicsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLocalDynamicsDoc* CLocalDynamicsView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLocalDynamicsDoc)));
	return (CLocalDynamicsDoc*)m_pDocument;
}
#endif //_DEBUG

void CLocalDynamicsView::InitHis(std::string hisList, float ps, float fs){
	app.ClearHis();
	std::istringstream istr(hisList);
	while(istr.good()){
		char line[1024];
		istr.getline(line, sizeof(line));
		app.AddHis(line);
	}
	for(int i=0; i<app.pointers.size(); ++i){
		app.pointers[i]->SetScale(ps, fs);
	}
#ifdef USE_IHC
		app.SetIHC(UTRef<DRUsb20Sh4>(app.devMan.RPool()[0]));
		app.device = UTRef<FWPointer6D>(app.pointers[0]);
#else
		//ldRenderer.device = UTRef<HIForceDevice6D>(app.pointers[0]);
		ldRenderer.device = UTRef<FWPointer6D>(app.pointers[0]);
#endif
}

void CLocalDynamicsView::LoadList(){
	if (fileListCur<0) fileListCur = 0;
	if (fileListCur>=fileList.size()) fileListCur = 0;
	if (fileList.size()){
		DSTR << fileListCur << ": " << fileList[fileListCur].c_str() << std::endl;
		Load(fileList[fileListCur]);
	}
	fileListCur ++;
	if (fileListCur>=fileList.size()) fileListCur = 0;
}
//	�t�@�C���̃��[�h
void CLocalDynamicsView::Load(std::string fn){
	WBPath path;
	path.Path(fn);
	//	�t�@�C�����X�g�t�@�C���̏ꍇ
	if (strcmpi(path.Ext().c_str(), ".lst") == 0){
		std::ifstream fs(fn.c_str());
		while(fs.good()){
			char line[1024];
			fs.getline(line, sizeof(line));
			if (strlen(line)){
				fileList.push_back(line);
			}
		}
		path.SetCwd(path.FullDir());
		fileListCur = 0;
		LoadList();
	//	���ʂ̃V�[���t�@�C���̏ꍇ
	}else{
		if (app.scene) app.scene->SetTimeStep(dlConfig.timeStep);
		app.Load(fn);
		if (app.scene){
//			timer.Resolution(app.scene->GetTimeStep()*1000);
//			timer.Interval(app.scene->GetTimeStep()*1000);
			timer.Resolution(ldTimerPeriod);
			timer.Interval(ldTimerPeriod);
		}
	}
	/*
	app.AddListener();
	PHContactEngine* pc;
	app.scene->GetBehaviors().Find(pc);
	if (pc){
		pc->Init(app.scene);
	}
	app.FindDSFrameRecord();
	*/
}

void CLocalDynamicsView::Save(std::string fn){	//	�t�@�C���̃Z�[�u
	bool bTimerCreated = timer.IsCreated();
	bool bTimerThread = timer.IsThread();
	timer.Release();
	app.Save(fn);
	if (bTimerCreated) timer.Create();
	if (bTimerThread) timer.Thread();
}

/////////////////////////////////////////////////////////////////////////////
// CLocalDynamicsView �N���X�̃��b�Z�[�W �n���h��
void CLocalDynamicsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch(nChar){
	case 'H':
		timer.Thread();				//	�X���b�h
#ifndef USE_IHC
		KillTimer(ldTimerId);
#endif
		KillTimer(simuTimerId);
		break;
	case 'L':		
		app.SetCalcLD(!app.GetCalcLD());
		break;
	case 'M':		//	multi media timer		
		if (app.scene){
#ifdef USE_IHC
//			timer.Resolution(app.scene->GetTimeStep()*1000);
//			timer.Interval(app.scene->GetTimeStep()*1000);
			float hoge = 0.01f;
			timer.Resolution(hoge*1000);
			timer.Interval(hoge*1000);
			app.scene->SetTimeStep(hoge);
			app.SetSimuDT(hoge);
#else
//			timer.Resolution(ldTimerPeriod*1000);
//			timer.Interval(ldTimerPeriod*1000);
//			timer.Resolution(simuTimerPeriod*1000);
//			timer.Interval(simuTimerPeriod*1000);
//			app.scene->SetTimeStep(simuTimerPeriod);
//			app.SetSimuDT(simuTimerPeriod);
			timer.Resolution(timer_interval*1000);
			timer.Interval(timer_interval*1000);
			app.scene->SetTimeStep(timer_interval);
			app.SetSimuDT(timer_interval);
#endif
		}
		timer.Create();				//	�}���`���f�B�A�^�C�}�[
#ifndef USE_IHC
		KillTimer(ldTimerId);
#endif
		KillTimer(simuTimerId);
		break;
	case 'P':		//	pause
		timer.Release();
		KillTimer(ldTimerId);
		KillTimer(simuTimerId);
		Step();
		break;
	case 'T':		//	message timer
		timer.Release();
#ifndef USE_IHC
		SetTimer(TIMER_LD, ldTimerPeriod*1000, NULL);
#endif
		SetTimer(TIMER_SIMU, simuTimerPeriod*1000, NULL);
		break;
	case VK_RETURN:
		if (fileList.size()){
			LoadList();
		}
		break;
	case 'O': // �I�[
		iCalculateMode = TEN_TO_ONE;
		app.scene->SetTimeStep(timer_interval);
		app.SetSimuDT(timer_interval);
		ldRenderer.SetSimuDT(timer_interval);
		ldRenderer.SetLDDT(timer_interval/10);
		ldRenderer.SetBExperiment(false);
		app.SetBExperiment(false);
		break;
	case '0': // �[��
		iCalculateMode = ONE_TO_ONE;
		app.scene->SetTimeStep(timer_interval/10);
		app.SetSimuDT(timer_interval/10);
		ldRenderer.SetSimuDT(timer_interval/10);
		ldRenderer.SetLDDT(timer_interval/10);
		ldRenderer.SetBExperiment(true);
		app.SetBExperiment(true);
		break;
/*
	case '8':	//	����ł��o��
		ThrowSphere(0.2f);
		break;
	case '9':
		ThrowSphere(0.3f);
		break;
	case '0':
		ThrowSphere(0.4f);
		break;
*/
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CLocalDynamicsView::ThrowSphere(float r){
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
	if (app.scene) app.scene->GetBehaviors().Find(ce);
	if (ce){
		SGFrame* frCol = ce->GetFrame(0);
		if (frCol && frCol->GetParent()) frCol->GetParent()->AddChildObject(frame, app.scene);
	}else{
		app.scene->GetWorld()->AddChildObject(frame, app.scene);
	}
	Affinef afSphere = app.mouse->GetAxis().inv();
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
		sphere->AddChildObject(mat, app.scene);
	}
	sphere->radius = r;
	sphere->slices = 16;
	sphere->stacks = 8;
	frame->AddChildObject(sphere, app.scene);
	app.render->InitTree(frame, app.scene);

	if (!csphere) csphere = new CDSphere;
	csphere->radius = r;
	csphere->pmaterial = new PHPhysicalMaterial;
	csphere->pmaterial->pMatData.reflexDamper = 0.1f;
	csphere->pmaterial->pMatData.reflexSpring = 1.0f;
	frame->AddChildObject(csphere, app.scene);
	frame->CalcBBox();

	PHSolidContainer* sc;
	app.scene->GetBehaviors().Find(sc);
	if (sc && !sc->solids.Find(solid)) sc->AddChildObject(solid, app.scene);
	PHGravityEngine* ge;
	app.scene->GetBehaviors().Find(ge);
	if (ge && !ge->solids.Find(solid)) ge->AddChildObject(solid, app.scene);

	solid->SetVelocity(-afSphere.Ez() * 8.0f);
	solid->SetAngularVelocity(Vec3f());

	PHContactEngine* pce;
	app.scene->GetBehaviors().Find(pce);
	if (pce && !pce->solids.Find(solid)){
		pce->AddChildObject(solid, app.scene);
		pce->Init(app.scene);
	}
	solids[pos] = solid;
	pos++;
	if (pos >= 10) pos = 0;
}

BOOL CLocalDynamicsView::OnEraseBkgnd(CDC* pDC) 
{
	// MFC�̓f�t�H���g�Ŕw�i�𔒂��h��D������̂ŉ������Ȃ��֐��ŃI�[�o�[���C�h�D
	return true;
}

///	�}���`���f�B�A�^�C�}�[�ɌĂяo�����֐�
void TimerFunc(void* arg){
	CLocalDynamicsView* view = (CLocalDynamicsView*) arg;
	view->Step();
}

void CLocalDynamicsView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if (!app.scene) app.Init(m_hWnd);
	//	�`��p�^�C�}�[�̐ݒ�
	drawTimerId = SetTimer(TIMER_DRAW, 33, NULL);
	//	�V�~�����[�V�����p�̕��ʂ̃^�C�}�[�̐ݒ�
	simuTimerId = SetTimer(TIMER_SIMU, simuTimerPeriod*1000, NULL);
	//	�V�~�����[�V�����p�̃}���`���f�B�A�^�C�}�[�̐ݒ�
	timer.Set(TimerFunc, this);
#ifndef USE_IHC
	//  �Ǐ������v�Z�p�̕��ʂ̃^�C�}�[
	ldTimerId   = SetTimer(TIMER_LD, ldTimerPeriod*1000, NULL);
#endif
}

//	�^�C�}�[�ɌĂ΂��֐��D�V�~�����[�V�����Ɨ͊o�񎦂��s���D
void CLocalDynamicsView::Step(){
	//	�����ŃV�~�����[�V�����̃X�e�b�v�����ׂĎ��s���Ă���D
	//	SGBehaviorEngine �̔h���I�u�W�F�N�g�����ԂɌĂ΂�ăV�~�����[�V���������s����D
	detectTime = 0;
	analyzeTime = 0;
	refTime = 0;
	fricTime = 0;
	totalTime = 0;
	static WBPreciseTimer ptimer;
	ptimer.CountUS();

#ifndef USE_IHC
	if(iCalculateMode == TEN_TO_ONE){
		app.Step();
		for(int i = 0; i < 10; i++){
			ldRenderer.Step();
		}
	}
	if(iCalculateMode == ONE_TO_ONE){
		for(int i = 0; i < 10; i++){
			app.Step();
			ldRenderer.Step();
		}
	}
#else
	app.Step();
#endif
	totalTime = ptimer.CountUS();
}

//  �^�C�}�[�ɌĂ΂��֐��@�Ǐ��I���͊w�v�Z�Ɨ͊o��
void CLocalDynamicsView::LDStep(){
#ifndef USE_IHC
	ldRenderer.Step();
#endif
}

void CLocalDynamicsView::OnTimer(UINT nIDEvent){
	if (nIDEvent == TIMER_LD){
/*		int times = 1;
		if (app.scene){
			times = ldTimerPeriod / app.scene->GetTimeStep();
		}
#ifndef USE_IHC
//		for(int i=0; i<times; ++i) LDStep();
#endif
*/
	}
	else if (nIDEvent == TIMER_SIMU){
		int times = 1;
		if (app.scene){
			times = simuTimerPeriod / app.scene->GetTimeStep();
		}
#ifdef USE_IHC
		Step();
#else
		for(int i=0; i<times; ++i) Step();
#endif
	}else if (nIDEvent == TIMER_DRAW){
		Invalidate();
	}
}

void CLocalDynamicsView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	timer.Release();
}

void CLocalDynamicsView::OnD3d(){
	app.CreateD3DRender();
}

void CLocalDynamicsView::OnGL(){
	app.CreateGLRender();
}

BOOL CLocalDynamicsView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	app.PreviewMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}

void CLocalDynamicsView::OnHisConfig(){
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	ReadRegistry();
	if (dlConfig.DoModal() == IDOK){
		WriteRegistry();
		InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
		if (app.scene) app.scene->SetTimeStep(dlConfig.timeStep);
		app.ConnectHis();
		timer.Resolution(dlConfig.timeStep*1000);
		timer.Interval(dlConfig.timeStep*1000);
	}
}



