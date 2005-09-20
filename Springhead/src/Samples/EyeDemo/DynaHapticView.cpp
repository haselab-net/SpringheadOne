// DynaHapticView.cpp : CDynaHapticView クラスの動作の定義を行います。
//

#include "stdafx.h"

#include "DynaHaptic.h"
#include "DynaHapticDoc.h"
#include "DynaHapticView.h"
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
	ON_COMMAND(ID_DEMO_1, OnDemo1)
END_MESSAGE_MAP()


class CDynaApp: public Spr::FWAppD3D{
};

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView クラスの構築/消滅

CDynaHapticView::CDynaHapticView()
{
//追加by hh 20050824
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fPosZ = 10.0f;
	m_fI = 0.0f;
	m_fAbsPosX = 0.0f;
	m_fAbsPosY = 0.0f;
	m_fAbsPosZ = 0.0f;
	m_Binocular.OcularL.ErrIntX = 0.0f;
	m_Binocular.OcularL.RelAngX = 0.0f;
	m_Binocular.OcularR.ErrIntX = 0.0f;
	m_Binocular.OcularR.RelAngX = 0.0f;

	m_Binocular.Sigma = 1.0f; //1.0;
	m_Binocular.V = 0.1f; //0.1;
	m_Binocular.K = 0.5f; //0.5;
	m_Binocular.N =	0.1f; //0.1;
	m_Binocular.P1 = 1.0f; //0.35;
	m_Binocular.P2 = 0.3f; //0.1;

	m_Deg0 = 0.0f;
	m_Deg1 = 0.0f;
	m_Deg2 = 0.0f;
	m_Deg3 = 0.0f;
	m_Saved = TRUE;

	m_LookAtX = 0.0f;
	m_LookAtY = 0.0f;
	m_LookAtZ = 1000.0f;
	m_LookAtRot = 0.0f;
	m_ShutOne = FALSE;

	m_LOE = 2.0f;

//End 20050824
	
	app = new CDynaApp;
	// SPIDARの初期化を行う
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
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView クラスの描画

void CDynaHapticView::OnDraw(CDC* pDC){
	CDynaHapticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	
	app->Draw();
}
/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView クラスの診断

#ifdef _DEBUG
void CDynaHapticView::AssertValid() const
{
	CView::AssertValid();
}

void CDynaHapticView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynaHapticDoc* CDynaHapticView::GetDocument() // 非デバッグ バージョンはインラインです。
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
// CDynaHapticView クラスのメッセージ ハンドラ
void CDynaHapticView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
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
	case '7': //Test
		//TransfrTest();
		break;
	case '6':
		//EyeTest1();
		break;
	case '5':
		//EyeTest();
		break;
	case '1':
		OnDemo1();
		break;	
	case '2':
		OnDemo2();
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
	// MFCはデフォルトで背景を白く塗る．ちらつくので何もしない関数でオーバーライド．
	return true;
}


void CDynaHapticView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	app->Init(m_hWnd);
	//	描画用タイマーの設定
	drawTimerId = SetTimer(FWApp::TIMER_DRAW, 33, NULL);
}

//	タイマーに呼ばれる関数．シミュレーションと力覚提示を行う．
/*
void CDynaHapticView::Step(){

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
*/
void CDynaHapticView::OnTimer(UINT nIDEvent)
{	
	if (nIDEvent == FWApp::TIMER_DRAW){		
		Invalidate();		
	} 
}

void CDynaHapticView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
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
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	app->PreviewMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}

void CDynaHapticView::OnHisConfig(){
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ReadRegistry();
	if (dlConfig.DoModal() == IDOK){
		WriteRegistry();
		InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
		if (app->scene) app->scene->SetTimeStep(dlConfig.timeStep);
		app->ConnectHis();
	}
}


//-> 関口による変更 (2005/1/05)
void CDynaHapticView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO : ここにメッセージ ハンドラ コードを追加します。
	app->mouse->SetViewSize(cx, cy);
}
//<-

//hh for test

BOOL CDynaHapticView::OnDemo1(){	
	

	PHSolid* target1;	
	PHSolid* OcularL;
	PHSolid* OcularR;

    //Quaternionf quat;
	//Quaternionf oriIni = Quaternionf();
	//Quaternionf ori = Quaternionf();
	Quaternionf oriL = Quaternionf();
	Quaternionf oriR = Quaternionf();	

	//div_t divResult;
	m_ShutOne = FALSE;
	m_Saved = FALSE;
	float tmp_IL;
	float tmp_IR;

	//targetの軌跡を指定する
	m_fPosX = 7.0f * sin(PI / 180.0f * m_fI);
	//m_fPosZ = 7.0f * sin(PI / 180.0f * m_fI);

//test
	//divResult = div((int)m_fI, 6);
	//if (divResult.rem == 0){
		m_fAbsPosX = m_fPosX;
		m_fAbsPosY = m_fPosY;
		m_fAbsPosZ = m_fPosZ;
	//}
	//if (divResult.rem == 3){
	{
		m_Binocular.OcularL.RelAngX = atan2(m_fAbsPosX - m_LOE, m_fAbsPosZ) / PI *180.0f - m_Binocular.OcularL.b;
		//if (m_ShutOne) m_Binocular.OcularL.RelAngX = 0.0f;		

		m_Binocular.OcularR.RelAngX = -(atan2(m_fAbsPosX + m_LOE, m_fAbsPosZ) / PI *180.0f - m_Binocular.OcularR.b);
		
		m_Binocular.OcularL.RelAngY = -atan2(m_fAbsPosY , 
			sqrt((m_fAbsPosX - m_LOE) * (m_fAbsPosX - m_LOE) + m_fAbsPosZ *m_fAbsPosZ))
			/ PI *180.0f - m_Binocular.OcularL.a;
		m_Binocular.OcularR.RelAngY = -atan2(m_fAbsPosY , 
			sqrt((m_fAbsPosX + m_LOE) * (m_fAbsPosX + m_LOE) + m_fAbsPosZ *m_fAbsPosZ))
			/ PI *180.0f - m_Binocular.OcularR.a;

		m_Binocular.TransFunc(&m_Deg0, &m_Deg1,&m_Deg2, &m_Deg3);
		
		m_Deg0 -= m_Binocular.OcularL.b;
		m_Deg1 -= m_Binocular.OcularR.b;	
		m_Deg2 -= m_Binocular.OcularL.a;
		m_Deg3 -= m_Binocular.OcularR.a;	
		
		m_Binocular.OcularL.b += m_Deg0; 
		m_Binocular.OcularR.b += m_Deg1; 
		m_Binocular.OcularL.a += m_Deg2; 
		m_Binocular.OcularR.a += m_Deg3; 

		/*m_Binocular.OcularL.b += m_Deg0 / 2.0f; 
		m_Binocular.OcularR.b += m_Deg1 / 2.0f; 
		m_Binocular.OcularL.a += m_Deg2 / 2.0f; 
		m_Binocular.OcularR.a += m_Deg3 / 2.0f; 
		*/
	}
	
	if  (m_fI < 900){
		m_Log[0][(int)m_fI][0] = atan2(m_fPosX - 2.0f, m_fPosZ) / PI *180.0f;
		m_Log[0][(int)m_fI][1] = m_Binocular.OcularL.b;
		m_Log[0][(int)m_fI][2] = m_Binocular.OcularL.RelAngX;
		m_Log[1][(int)m_fI][0] = atan2(m_fPosX + 2.0f, m_fPosZ) / PI *180.0f;
		m_Log[1][(int)m_fI][1] = m_Binocular.OcularR.b;
		m_Log[1][(int)m_fI][2] = m_Binocular.OcularR.RelAngX;
	} else if (!m_Saved){
	if (!m_Saved){
		FILE *LogFile;
		int i;
		LogFile = fopen("LogFile.csv", "w");
		for (i=0; i<900; i++){
			fprintf(LogFile, "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n", 
				m_Log[0][i][0], m_Log[0][i][1], m_Log[0][i][2],
				m_Log[1][i][0], m_Log[1][i][1], m_Log[1][i][2]);
		}
		fclose(LogFile);
		m_Saved = TRUE;
	}
	}

	//描画

	//target objectの取得
	if(app -> scene) app -> scene -> FindObject(target1,"soTarget1");
	//現時点、targetの位置を取得
	pos_target1 = target1->GetFramePosition();	
	pos_target1[0] = m_fPosX;
	pos_target1[1] = m_fPosY;
	pos_target1[2] = m_fPosZ;
	target1->SetFramePosition(pos_target1);
	target1->UpdateFrame();

	
	if(app -> scene) app -> scene -> FindObject(OcularL,"soOcularL");
	if(app -> scene) app -> scene -> FindObject(OcularR,"soOcularR");
	tmp_IL = m_Binocular.OcularL.b / 180.0f * PI;	
	tmp_IR = m_Binocular.OcularR.b / 180.0f * PI;

	//test
		//tmp_IL =   -11.3f / 180.0f * PI;	
		//tmp_IR =   11.3f / 180.0f * PI;			
	//end test
	
	oriL = Quaternionf::Rot((tmp_IL),Vec3f(0,1,0));
	oriR = Quaternionf::Rot((tmp_IR),Vec3f(0,1,0));	
	
	OcularL->SetOrientation(oriL);
	OcularL->UpdateFrame();	

	OcularR->SetOrientation(oriR);
	OcularR->UpdateFrame();	
//test end	
	m_fI++;

	return true;
}

BOOL CDynaHapticView::OnDemo2(){	
	

	PHSolid* target1;	
	PHSolid* OcularL;
	PHSolid* OcularR;

    //Quaternionf quat;
	//Quaternionf oriIni = Quaternionf();
	//Quaternionf ori = Quaternionf();
	Quaternionf oriL = Quaternionf();
	Quaternionf oriR = Quaternionf();	

	//m_ShutOne = FALSE;
//	m_Saved = FALSE;
	float tmp_IL;
	float tmp_IR;

	//targetの軌跡を指定する
	m_fPosX = 7.0f * sin(PI / 180.0f * m_fI);

		m_fAbsPosX = m_fPosX;
		m_fAbsPosY = m_fPosY;
		m_fAbsPosZ = m_fPosZ;
	
	{
		//m_Binocular.OcularL.RelAngX = atan2(m_fAbsPosX - m_LOE, m_fAbsPosZ) / PI *180.0f - m_Binocular.OcularL.b;
		//if (m_ShutOne) m_Binocular.OcularL.RelAngX = 0.0f;		
		m_Binocular.OcularL.RelAngX = 0.0f;

		m_Binocular.OcularR.RelAngX = -(atan2(m_fAbsPosX + m_LOE, m_fAbsPosZ) / PI *180.0f - m_Binocular.OcularR.b);
		
		m_Binocular.OcularL.RelAngY = -atan2(m_fAbsPosY , 
			sqrt((m_fAbsPosX - m_LOE) * (m_fAbsPosX - m_LOE) + m_fAbsPosZ *m_fAbsPosZ))
			/ PI *180.0f - m_Binocular.OcularL.a;
		m_Binocular.OcularR.RelAngY = -atan2(m_fAbsPosY , 
			sqrt((m_fAbsPosX + m_LOE) * (m_fAbsPosX + m_LOE) + m_fAbsPosZ *m_fAbsPosZ))
			/ PI *180.0f - m_Binocular.OcularR.a;

		m_Binocular.TransFunc(&m_Deg0, &m_Deg1,&m_Deg2, &m_Deg3);
		
		m_Deg0 -= m_Binocular.OcularL.b;
		m_Deg1 -= m_Binocular.OcularR.b;	
		m_Deg2 -= m_Binocular.OcularL.a;
		m_Deg3 -= m_Binocular.OcularR.a;	
		
		m_Binocular.OcularL.b += m_Deg0; 
		m_Binocular.OcularR.b += m_Deg1; 
		m_Binocular.OcularL.a += m_Deg2; 
		m_Binocular.OcularR.a += m_Deg3; 
	
	}
	
	

	//描画

	//target objectの取得
	if(app -> scene) app -> scene -> FindObject(target1,"soTarget1");
	//現時点、targetの位置を取得
	pos_target1 = target1->GetFramePosition();	
	pos_target1[0] = m_fPosX;
	pos_target1[1] = m_fPosY;
	pos_target1[2] = m_fPosZ;
	target1->SetFramePosition(pos_target1);
	target1->UpdateFrame();

	
	if(app -> scene) app -> scene -> FindObject(OcularL,"soOcularL");
	if(app -> scene) app -> scene -> FindObject(OcularR,"soOcularR");
	tmp_IL = m_Binocular.OcularL.b / 180.0f * PI;	
	tmp_IR = m_Binocular.OcularR.b / 180.0f * PI;	
	
	oriL = Quaternionf::Rot((tmp_IL),Vec3f(0,1,0));
	oriR = Quaternionf::Rot((tmp_IR),Vec3f(0,1,0));	
	
	OcularL->SetOrientation(oriL);
	OcularL->UpdateFrame();	

	OcularR->SetOrientation(oriR);
	OcularR->UpdateFrame();	

	m_fI++;
	return true;
}