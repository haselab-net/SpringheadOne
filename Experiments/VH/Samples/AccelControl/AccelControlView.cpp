// AccelControlView.cpp : CAccelControlView クラスの動作の定義を行います。
//

#include "stdafx.h"

#include "AccelControl.h"
#include "AccelControlDoc.h"
#include "AccelControlView.h"
#include "HisConfig.h"

#include <Physics/PHSolid.h>
#include <Physics/PHJoint1D.h>
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
#include <Physics/PHJoint.h>

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

/////////////////////////////////////////////////////////////////////////////
// CAccelControlView

IMPLEMENT_DYNCREATE(CAccelControlView, CView)

BEGIN_MESSAGE_MAP(CAccelControlView, CView)
	//{{AFX_MSG_MAP(CAccelControlView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(RENDER_D3D, OnD3d)
	ON_COMMAND(WGL_RENDER, OnGL)
	ON_COMMAND(ID_HIS_CONFIG, OnHisConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

class CDynaApp: public Spr::FWAppD3D{
public:
	UTRef<PHJointEngine> je;
	UTRef<PHJoint1D> j9;
	std::vector<PHJoint1D*> joints;
	virtual void LoadImp(UTString fn){
		joints.clear();
		je=NULL;
		j9=NULL;
		FWAppD3D::LoadImp(fn);
		je = DCAST(PHJointEngine, scene->FindObject("jeChain"));
		j9 = DCAST(PHJoint1D,  scene->FindObject("jnt910"));
		if (je){
			je->EnumJoint(joints);
		}
		joints.erase(joints.begin());
	}
	void SetPattern(PHJoint1D* j9, PHJointEngine* je, int n){
		for(int i=0; i<joints.size(); ++i){
			joints[i]->SetTorque(0);
		}
		j9->solid->ClearForce();

		if (n<0) return;
		if (n<10){
			joints[n]->SetTorque(1);
			return;
		}
		Vec3d force;
		force[n-10] = 1;
		j9->solid->AddForce(force);
	}
	virtual void Step(){
		if (bBusy) return;
		cs.Enter();
		scene->ClearForce();
		scene->GenerateForce();
		if (je && j9){
			j9->solid->ClearForce();
			//	状態保存
			std::vector<double> torques;
			for(int i=0; i<joints.size(); ++i){
				torques.push_back(joints[i]->torque);
			}
			
			//	伝播行列計算
			double dt = scene->GetTimeStep();
			je->root->CompCoriolisAccelRecursive(dt);
			PTM::TMatrixRow<3, 10, double> K;
			Matrix3d D;
			Vec3d c;
			//	accel = K t + D f + c
			for(int i=-1; i<13; ++i){
				SetPattern(j9, je, i);
				je->root->CompArticulatedInertia(dt);
				j9->CalcAccel(dt);
				Vec3d accel = j9->GetSolidAccel();
				if (i==-1){
					c = accel;
				}else if (i<10){
					K.col(i) = accel - c;
				}else{
					D.col(i-10) = accel - c;
				}
			}
			/*	accel = K t + D f + c
				Df = - (Kt + c)
				f = - D^-1 (Kt + c)
			*/

			//	状態復元
			for(int i=0; i<joints.size(); ++i){
				joints[i]->torque = torques[i];
			}
			j9->solid->ClearForce();
#if 0
			//	とりあえず， t = 0の場合．
			Vec3d f = - D.inv() * c;
			DSTR << "f:" << f << std::endl;
			j9->solid->AddForce(f);
#else
			//	f=0の場合
			//	Kt = - (c)
			//	t = -K^-1 c
			PTM::TVector<10, double> tq;
			PTM::TMatrixRow<2,2,double> kk = (K * K.trans()).sub_matrix(0,0,PTM::TMatDim<2,2>());
			PTM::TMatrixRow<2,2,double> kkinv = kk.inv();
			PTM::TMatrixRow<10,2,double> kinv = K.trans().sub_matrix(0,0, PTM::TMatDim<10, 2>()) * kkinv;
			tq = - kinv * c.sub_vector(0, PTM::TVecDim<2>());
			for(int i=0; i<joints.size(); ++i){
				joints[i]->torque = tq[i];
			}
//			DSTR << K;
//			DSTR << kk;
//			DSTR << kk.inv();
//			DSTR << kinv;
			DSTR << tq << std::endl;

#endif
		}
		scene->Integrate();
		pointers.Step(scene->GetTimeStep());
		cs.Leave();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CAccelControlView クラスの構築/消滅

CAccelControlView::CAccelControlView()
{
	app = new CDynaApp;
	// SPIDARの初期化を行う
	ReadRegistry();
	WriteRegistry();
	InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
}

void CAccelControlView::ReadRegistry(){
	dlConfig.hisList = AfxGetApp()->GetProfileString("his_config", "his_list", "Spidar4\r\nSpidar4 reverse\r\nSpidarG6C\r\nSpidarG6C\r\nMouse");
	std::string hisPs(AfxGetApp()->GetProfileString("his_config", "his_posScale", "1.0"));
	std::string hisFs(AfxGetApp()->GetProfileString("his_config", "his_forceScale", "1.0"));
	std::string timeStep(AfxGetApp()->GetProfileString("his_config", "his_timeStep", "0.005"));
	std::istringstream(hisPs) >> dlConfig.posScale;
	std::istringstream(hisFs) >> dlConfig.forceScale;
	std::istringstream(timeStep) >> dlConfig.timeStep;
}
void CAccelControlView::WriteRegistry(){
	AfxGetApp()->WriteProfileString("his_config", "his_list", dlConfig.hisList);
	char buf[256];
	std::ostrstream(buf, sizeof(buf)) << dlConfig.posScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_posScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.forceScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_forceScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.timeStep << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_timeStep", buf);
}

CAccelControlView::~CAccelControlView(){
}

BOOL CAccelControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAccelControlView クラスの描画

void CAccelControlView::OnDraw(CDC* pDC){
	CAccelControlDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	app->Draw();
}
/////////////////////////////////////////////////////////////////////////////
// CAccelControlView クラスの診断

#ifdef _DEBUG
void CAccelControlView::AssertValid() const
{
	CView::AssertValid();
}

void CAccelControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAccelControlDoc* CAccelControlView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAccelControlDoc)));
	return (CAccelControlDoc*)m_pDocument;
}
#endif //_DEBUG

void CAccelControlView::InitHis(std::string hisList, float ps, float fs){
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
// CAccelControlView クラスのメッセージ ハンドラ
void CAccelControlView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch(nChar){
	case '8':	//	球を打ち出す
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
void CAccelControlView::ThrowSphere(float r){
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
	csphere->pmaterial = new PHPhysicalMaterial;
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

BOOL CAccelControlView::OnEraseBkgnd(CDC* pDC) 
{
	// MFCはデフォルトで背景を白く塗る．ちらつくので何もしない関数でオーバーライド．
	return true;
}


void CAccelControlView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	app->Init(m_hWnd);
	//	描画用タイマーの設定
	drawTimerId = SetTimer(FWApp::TIMER_DRAW, 33, NULL);
}

//	タイマーに呼ばれる関数．シミュレーションと力覚提示を行う．
void CAccelControlView::Step(){
	//	ここでシミュレーションのステップをすべて実行している．
	//	SGBehaviorEngine の派生オブジェクトが順番に呼ばれてシミュレーションを実行する．
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

void CAccelControlView::OnTimer(UINT nIDEvent){
	if (nIDEvent == FWApp::TIMER_DRAW){
		Invalidate();
	}
}

void CAccelControlView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	app = NULL;
}

void CAccelControlView::OnD3d(){
	app->CreateD3DRender();
}

void CAccelControlView::OnGL(){
	app->CreateGLRender();
}

BOOL CAccelControlView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	app->PreviewMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}

void CAccelControlView::OnHisConfig(){
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ReadRegistry();
	if (dlConfig.DoModal() == IDOK){
		WriteRegistry();
		InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
		if (app->scene) app->scene->SetTimeStep(dlConfig.timeStep);
		app->ConnectHis();
	}
}



