// DynaLocalHapView.cpp : CDynaLocalHapView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"

#include "DynaLocalHap.h"
#include "DynaLocalHapDoc.h"
#include "DynaLocalHapView.h"
#include "HisConfig.h"

#include <Physics/PHSolid.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <GraphicsGL/GLRenderWgl.h>
#include <WinBasis/WBPath.h>
//	for haptics
#include <WinBasis/WBMMTimer.h>
#include <WinBasis/WBPreciseTimer.h>
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include "Fortran/quadpr.h"

//	for shooting
#include <Graphics/GRSphere.h>
#include <Collision/CDSphere.h>
#include <Collision/CDCollisionEngine.h>
#include <Physics/PHForceField.h>
#include <Physics/PHContactEngine.h>
#include "../../ihc/src/IHCBase/IHUsbPacket.h"

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
};
static float simuTimerPeriod = 0.1f;



#define FR_DUMMYSPHERE "frDummySphere"
#define SO_DUMMYSPHERE "soDummySphere"

CLocalHapApp::CLocalHapApp(){
	pointerRadius = 0.01f;
}
CLocalHapApp::~CLocalHapApp(){}


void CLocalHapApp::SimulationStep(){
	static std::ofstream file("log.txt");
	file << pointer.X() << "\t" << pointer.Y() << "\t" << pointer.Z() << std::endl;
	//	����p���̈ʒu�̍X�V
	if(soNearSphere){
		soNearSphere->SetCenterPosition(pointer);
		soNearSphere->UpdateFrame();
	}
	planes.clear();
	planeNormal = Vec3f();
	minDist = 1e10f;
	
	//	�V�~�����[�V�����Ɣ���
	scene->ClearForce();

	//	�͐ς������鏈��
	if (planes.size()){
		for(int i=0; i<planes.size(); ++i){
			planes[i].solid->AddForce(receivedForce / scene->GetTimeStep() / planes.size(), planePoint - pointerRadius*planeNormal);
		}
	}else if (planeSolid) {
		planeSolid->AddForce(receivedForce / scene->GetTimeStep(), planePoint - pointerRadius*planeNormal);
	}

	scene->GenerateForce();
	scene->Integrate();
	IHPacketPlane::PlaneState state;
	if (planes.size()){	//	�ڐG�ʂ�����ꍇ
		//	�ړI�֐�: (r-pointer)^2 ���ŏ��ɂ���ʒu r �����߂�D
		Matrix3f quad;					//	�ړI�֐���2���̌W��
		Vec3f cost;						//	�ړI�֐���1���̌W��
		const Vec3f v3QPOffset(1000, 1000, 1000);
		cost = -2*v3QPOffset;

		PTM::VMatrixCol<float> matA;	//	�������̍���
		matA.resize(planes.size(),3);
		PTM::VVector<float> rhs;		//	�������̉E��
		rhs.resize(planes.size());
		PTM::VVector<long> kt;			//	�������̕s����
		kt.resize(planes.size());

		for(int i=0; i<planes.size(); ++i){
			rhs[i] = planes[i].normal * (planes[i].pos - pointer + v3QPOffset);
//			if (rhs[i] < 0){
				rhs[i] *= -1;
				matA.row(i) = -planes[i].normal;
				kt[i] = 1;
//			}else{
//				matA.row(i) = planes[i].normal;
//				kt[i] = -1;
//			}
		}
		
		int wsSize = 2 * Square(matA.height()+quad.height()) + 8*(matA.height()+quad.height()) + 6;
		long input[14]={
			matA.height(),
			quad.height(),
			matA.height(),
			quad.height(),
			0,
			wsSize,
			100,1,0,0,0,0,0,80};		//	�e�ݒ�ϐ�
		float tol[2]={0,0};				//	���e�덷

		char title[]="quadratic ploblem";
		char pfile[]="savefile";

		PTM::TVector<1,float> obj;
		Vec3f x, rc;					//	output
		PTM::VVector<float> dual, slk;	//	auxiliary storage
		dual.resize(planes.size());
		slk.resize(planes.size());
		PTM::TVector<2,long> iout;                    //	output
		PTM::VVector<float> ws;
		ws.resize(wsSize);				//	work space
		long title_len=0, pfile_len=0;
		quadpr_(matA, kt, rhs, cost, quad, input, tol, title, pfile,
		 obj, x, rc, dual, slk, iout, ws, title_len, pfile_len);
		if (iout[0] != 1){
			DSTR << "qpError:" << iout[0] << std::endl;
			DSTR << "matA:" << matA;
			DSTR << "rhs:" << rhs << std::endl;
			DSTR << "kt:" << kt << std::endl;
			quadpr_(matA, kt, rhs, cost, quad, input, tol, title, pfile,
			 obj, x, rc, dual, slk, iout, ws, title_len, pfile_len);
		}
	//	DSTR << "cost" << cost << "matA:" << matA << std::endl;
	//	DSTR << "obj:" << obj[0] << " pivot:" << iout[1];
		planePoint = x + pointer - v3QPOffset;
	//	DSTR << "planePt:" << planePoint << std::endl;
	//	DSTR << "rhs:" << rhs << "A:" << matA;
		planeNormal = planePoint - pointer;
		if (planeNormal.norm() > 1e-6f) planeNormal.unitize();
		else planeNormal = planes[0].normal;
		state = IHPacketPlane::CONTINUE;
//		DSTR << "The point:" << planePoint  << x << " Normal:" << planeNormal <<	std::endl;
	}else if (planeNormal != Vec3f()){
		state = IHPacketPlane::NEW;
	}else{
		state = IHPacketPlane::NONE;
	}

#ifdef USE_IHC
	char buf[512];
	IHPacketPlane plane;
	plane.posScale = posScale;
	plane.forceScale = forceScale;
	plane.offset = hapticOffset;
	plane.position = planePoint;
	plane.normal = planeNormal;
	plane.planeState = state;
	
	plane.EndianAll();
	memset(buf, 0, sizeof(buf));
	memcpy(buf, &plane, sizeof(plane));
	ihc->UsbSend((unsigned char*)buf);
	ihc->UsbRecv((unsigned char*)buf);
	IHPacketPlaneReturn ret;
	memcpy(&ret, buf, sizeof(ret));
	ret.EndianAll();
//	DSTR << "ret.Pos:" << ret.position << " ret.f:" << ret.force << std::endl;
	pointer = ret.position;
	receivedForce = ret.force;

#else
	//	�ʂ�ݒ�
	hapticRender.SetPlane(planePoint, planeNormal, state, posScale, forceScale, hapticOffset);
	//	���̎擾
	receivedForce = hapticRender.force;
	hapticRender.force = Vec3f();
	pointer = hapticRender.pointer;
#endif
}
const float epsilon = 1e-5f;
void CLocalHapApp::Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
	if (!pContactEngine) return;
	int iPlane=0;
	if (fr->frame[0]->frame == soNearSphere->GetFrame()) iPlane = 1;

	Vec3f n = iPlane==0 ? conv->normal : -conv->normal;
	Vec3f cp = geom->posture[iPlane] * conv->closestPoint[iPlane];
	cp += pointerRadius*n;
	float d = (pointer - cp)*n;
	PHContactEngine::FrameRecord* rec = UTRef<PHContactEngine::FrameRecord>(fr->frame[iPlane]->records[pContactEngine->GetFrameRecordPos()]);
	if (d < -epsilon){
		planes.push_back(TPlane(cp, n, rec->solid));
	}
	if (!planes.size()){
		if (d < minDist){
			minDist = d;
			planePoint = cp;
			planeNormal = n;
			planeSolid = rec->solid;
		}
	}
}

void CLocalHapApp::Load(std::string fn){
	soNearSphere = NULL;
	pContactEngine = NULL;
	FWAppD3D::Load(fn);
	planes.clear();
#ifndef USE_IHC
	hapticRender.hapticDt = scene->GetTimeStep()/10;
#endif
	// ���X�i�ɔ���p�̋���o�^
	scene->GetBehaviors().Find(pContactEngine);
	UTRef<CDCollisionEngine> ce;
	scene->GetBehaviors().Find(ce);
	if (ce){
		listenerPos = ce->AddListener(this);
		SGFrame* fr = DCAST(SGFrame, scene->FindObject(FR_DUMMYSPHERE));
		if (fr){
			for(int i = 0; i < ce->NFrame(); i++){
				SGFrame* sg = ce->GetFrame(i);
				if(sg != fr){
					ce->AddActive(fr, sg, listenerPos);
					if (pContactEngine){
						ce->AddInactive(fr, sg, pContactEngine->GetListenerPos());
					}
				}
			}	
		}
		if (pContactEngine){
			pContactEngine->Init(scene);
		}else{
			ce->Init();
		}
		//	����p�̋���FrameRecord���擾
		if(pContactEngine){
			soNearSphere = DCAST(PHSolid, scene->FindObject(SO_DUMMYSPHERE));
			hapticOffset = soNearSphere->GetFramePosition();
			pointerRadius = soNearSphere->GetFrame()->bbox.GetBBoxExtent().X() / 5;
		}
	}
}
void CLocalHapApp::Step(){
#ifdef USE_IHC
	SimulationStep();
#else
	static int count;
	count ++;
	if (count >= 10){
		count = 0;
		SimulationStep();
	}
	hapticRender.HapticStep();
#endif
}
void CLocalHapApp::Draw(){
	if (!render) return;
	render->Render(scene);			//	�V�[���̕`��
	Vec3f line[2];
	line[0] = planePoint - pointerRadius*planeNormal;
	line[1] = line[0] + receivedForce;
	render->SetDepthTest(false);
	render->SetModelMatrix(Affinef());
	GRMaterialData mat(Vec4f(1,1,1,1), Vec4f(1,1,1,1), Vec4f(1,1,1,1), Vec4f(1,1,1,1), 1.0f);
	render->SetMaterial(mat);
	render->DrawDirect(GRRender::LINES, line, line+2);
	render->SetDepthTest(true);
	render->EndScene();				//	�`��I��
	render->Present();				//	�\��
	render->SetViewport(hWnd);		//	Viewport�̐ݒ�
	render->ClearBuffer();			//	�o�b�t�@�N���A
	render->BeginScene();			//	�`��J�n	
}





/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapView

IMPLEMENT_DYNCREATE(CDynaLocalHapView, CView)

BEGIN_MESSAGE_MAP(CDynaLocalHapView, CView)
	//{{AFX_MSG_MAP(CDynaLocalHapView)
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
// CDynaLocalHapView �N���X�̍\�z/����

CDynaLocalHapView::CDynaLocalHapView()
{
	// SPIDAR�̏��������s��
	bPause = false;
	ReadRegistry();
	WriteRegistry();
	InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
	timer.Resolution(dlConfig.timeStep*1000);
	timer.Interval(dlConfig.timeStep*1000);
}

void CDynaLocalHapView::ReadRegistry(){
	dlConfig.hisList = AfxGetApp()->GetProfileString("his_config", "his_list", "Spidar4\r\nSpidar4 reverse\r\nSpidarG6C\r\nSpidarG6C\r\nMouse");
	std::string hisPs(AfxGetApp()->GetProfileString("his_config", "his_posScale", "1.0"));
	std::string hisFs(AfxGetApp()->GetProfileString("his_config", "his_forceScale", "1.0"));
	std::string timeStep(AfxGetApp()->GetProfileString("his_config", "his_timeStep", "0.005"));
	std::istringstream(hisPs) >> dlConfig.posScale;
	std::istringstream(hisFs) >> dlConfig.forceScale;
	std::istringstream(timeStep) >> dlConfig.timeStep;
}
void CDynaLocalHapView::WriteRegistry(){
	AfxGetApp()->WriteProfileString("his_config", "his_list", dlConfig.hisList);
	char buf[256];
	std::ostrstream(buf, sizeof(buf)) << dlConfig.posScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_posScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.forceScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_forceScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.timeStep << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_timeStep", buf);
}

CDynaLocalHapView::~CDynaLocalHapView(){
}

BOOL CDynaLocalHapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapView �N���X�̕`��

void CDynaLocalHapView::OnDraw(CDC* pDC){
	CDynaLocalHapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	app.Draw();
}
/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapView �N���X�̐f�f

#ifdef _DEBUG
void CDynaLocalHapView::AssertValid() const
{
	CView::AssertValid();
}

void CDynaLocalHapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynaLocalHapDoc* CDynaLocalHapView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynaLocalHapDoc)));
	return (CDynaLocalHapDoc*)m_pDocument;
}
#endif //_DEBUG

void CDynaLocalHapView::InitHis(std::string hisList, float ps, float fs){
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
	if (app.pointers.size() > 0){
		app.posScale = ps;
		app.forceScale = fs;
#ifdef USE_IHC
		app.ihc = UTRef<DRUsb20Sh4>(app.devMan.RPool()[0]);
#else
		app.hapticRender.device = UTRef<HIForceDevice6D>(app.pointers[0]->device);
#endif
	}
}

void CDynaLocalHapView::LoadList(){
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
void CDynaLocalHapView::Load(std::string fn){
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
			timer.Resolution(app.scene->GetTimeStep()*1000);
			timer.Interval(app.scene->GetTimeStep()*1000);
		}
	}
}

void CDynaLocalHapView::Save(std::string fn){	//	�t�@�C���̃Z�[�u
	bool bTimerCreated = timer.IsCreated();
	bool bTimerThread = timer.IsThread();
	timer.Release();
	app.Save(fn);
	if (bTimerCreated) timer.Create();
	if (bTimerThread) timer.Thread();
}

/////////////////////////////////////////////////////////////////////////////
// CDynaLocalHapView �N���X�̃��b�Z�[�W �n���h��
void CDynaLocalHapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch(nChar){
	case 'P':		//	pause
		timer.Release();
		KillTimer(simuTimerId);
		Step();
		break;
	case 'T':		//	message timer
		timer.Release();
		SetTimer(TIMER_SIMU, simuTimerPeriod*1000, NULL);
		break;
	case 'M':		//	multi media timer		
		if (app.scene){
			timer.Resolution(app.scene->GetTimeStep()*1000);
			timer.Interval(app.scene->GetTimeStep()*1000);
		}
		timer.Create();				//	�}���`���f�B�A�^�C�}�[
		KillTimer(simuTimerId);
		break;
	case 'H':
		timer.Thread();				//	�X���b�h
		KillTimer(simuTimerId);
		break;
	case VK_RETURN:
		if (fileList.size()){
			LoadList();
		}
		break;
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
void CDynaLocalHapView::ThrowSphere(float r){
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

BOOL CDynaLocalHapView::OnEraseBkgnd(CDC* pDC) 
{
	// MFC�̓f�t�H���g�Ŕw�i�𔒂��h��D������̂ŉ������Ȃ��֐��ŃI�[�o�[���C�h�D
	return true;
}

///	�}���`���f�B�A�^�C�}�[�ɌĂяo�����֐�
void TimerFunc(void* arg){
	CDynaLocalHapView* view = (CDynaLocalHapView*) arg;
	view->Step();
}

void CDynaLocalHapView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	app.Init(m_hWnd);
	//	�`��p�^�C�}�[�̐ݒ�
	drawTimerId = SetTimer(TIMER_DRAW, 33, NULL);
	//	�V�~�����[�V�����p�̕��ʂ̃^�C�}�[�̐ݒ�
	simuTimerId = SetTimer(TIMER_SIMU, simuTimerPeriod*1000, NULL);
	//	�V�~�����[�V�����p�̃}���`���f�B�A�^�C�}�[�̐ݒ�
	timer.Set(TimerFunc, this);
}

//	�^�C�}�[�ɌĂ΂��֐��D�V�~�����[�V�����Ɨ͊o�񎦂��s���D
void CDynaLocalHapView::Step(){
	//	�����ŃV�~�����[�V�����̃X�e�b�v�����ׂĎ��s���Ă���D
	//	SGBehaviorEngine �̔h���I�u�W�F�N�g�����ԂɌĂ΂�ăV�~�����[�V���������s����D
	detectTime = 0;
	analyzeTime = 0;
	refTime = 0;
	fricTime = 0;
	totalTime = 0;
	static WBPreciseTimer ptimer;
	ptimer.CountUS();
	app.Step();
	totalTime = ptimer.CountUS();
}

void CDynaLocalHapView::OnTimer(UINT nIDEvent){
	if (nIDEvent == TIMER_SIMU){
		int times = 1;
		if (app.scene){
			times = simuTimerPeriod / app.scene->GetTimeStep();
		}
#ifdef USE_IHC
		Step();
#else
		for(int i=0; i<times*10; ++i) Step();
#endif
	}else if (nIDEvent == TIMER_DRAW){
		Invalidate();
	}
}

void CDynaLocalHapView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	timer.Release();
}

void CDynaLocalHapView::OnD3d(){
	app.CreateD3DRender();
}

void CDynaLocalHapView::OnGL(){
	app.CreateGLRender();
}

BOOL CDynaLocalHapView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	app.PreviewMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}

void CDynaLocalHapView::OnHisConfig(){
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



