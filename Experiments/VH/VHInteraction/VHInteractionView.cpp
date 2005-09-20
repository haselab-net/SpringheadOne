// VHInteractionView.cpp : CVHInteractionView クラスの動作の定義を行います。
//

#include "stdafx.h"

#include "VHInteraction.h"
#include "VHInteractionDoc.h"
#include "VHInteractionView.h"
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
// CVHInteractionView

IMPLEMENT_DYNCREATE(CVHInteractionView, CView)

BEGIN_MESSAGE_MAP(CVHInteractionView, CView)
	//{{AFX_MSG_MAP(CVHInteractionView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(RENDER_D3D, OnD3d)
	ON_COMMAND(WGL_RENDER, OnGL)
	ON_COMMAND(ID_HIS_CONFIG, OnHisConfig)
	ON_COMMAND(ID_VH_DIALOG, OnVhDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CVHInteractionView クラスの構築/消滅

CVHInteractionView::CVHInteractionView()
{
	// SPIDARの初期化を行う
	ReadRegistry();
	WriteRegistry();
	InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
}

void CVHInteractionView::ReadRegistry(){
	dlConfig.hisList = AfxGetApp()->GetProfileString("his_config", "his_list", "Spidar4\r\nSpidar4 reverse\r\nSpidarG6C\r\nSpidarG6C\r\nMouse");
	std::string hisPs(AfxGetApp()->GetProfileString("his_config", "his_posScale", "1.0"));
	std::string hisFs(AfxGetApp()->GetProfileString("his_config", "his_forceScale", "1.0"));
	std::string timeStep(AfxGetApp()->GetProfileString("his_config", "his_timeStep", "0.005"));
	std::istringstream(hisPs) >> dlConfig.posScale;
	std::istringstream(hisFs) >> dlConfig.forceScale;
	std::istringstream(timeStep) >> dlConfig.timeStep;
}
void CVHInteractionView::WriteRegistry(){
	AfxGetApp()->WriteProfileString("his_config", "his_list", dlConfig.hisList);
	char buf[256];
	std::ostrstream(buf, sizeof(buf)) << dlConfig.posScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_posScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.forceScale << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_forceScale", buf);
	std::ostrstream(buf, sizeof(buf)) << dlConfig.timeStep << '\0';
	AfxGetApp()->WriteProfileString("his_config", "his_timeStep", buf);
}

CVHInteractionView::~CVHInteractionView(){
}

BOOL CVHInteractionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionView クラスの描画

void CVHInteractionView::OnDraw(CDC* pDC){
	CVHInteractionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	app.Draw();

	// 文字列描画
	if(app.crPuppet.IsLoaded() && app.crUser.IsLoaded()){
		CFont font;
		font.CreatePointFont(300, "ＭＳゴシック");
		pDC->SelectObject(font);
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		GetClientRect(rc);

		// VH の状態表示
		if(app.bDrawState){
			// VH 目標行動
			char* statStr[] = {
				"stand by", "attack", "guard", "dodge"
			};
			for(int i = 0; i < 3; ++i){
				char* str = statStr[app.crPuppet.reaching[0][i].state];
				int x = rc.Width() - pDC->GetTextExtent(str).cx;
				x = x/2+100;
				if (i==0) pDC->TextOut(x-200, 50, str);
				if (i==1) pDC->TextOut(x, 50, str);
				if (i==2) pDC->TextOut(x-100, 0, str);
			}

			// VH 運動予測
			if(app.crPlanner.bPlanner){
				if(!app.crPlanner.bPrediction) pDC->SetTextColor(RGB(100,100,100));
				char* str = "prediction";
				int x = rc.Width() - 150 - pDC->GetTextExtent(str).cx / 2;
				int y = rc.Height() - 50;
				pDC->TextOut(x, y, str);
				pDC->SetTextColor(RGB(255,255,255));
			}

			// VH 行動パターン
			char* statStr2[] = {
				"Sandbag", "Dodge", "Guard", "Defence", "Attak", "Dodge & Attak", "Guard & Attak", "Defence & Attak",
			};
			int action = 0;
			if(app.crPlanner.bPlanner) action  = 1;
			if(app.crPuppet.bGuard)    action += 2;
			if(app.crPuppet.bAttack)   action += 4;
			char* str = statStr2[action];

			//int x = rc.Width() - 300;
			int x = rc.Width() - 150 - pDC->GetTextExtent(str).cx / 2;
			int y = rc.Height() - 100;
			pDC->TextOut(x, y, str);

			// 力覚
/*			pDC->TextOut(0,  0, "Force : ");
			if(app.crUser.bSpidarForce){
				pDC->TextOut(120, 0, "ON");
			}
			else{
				pDC->TextOut(120, 0, "OFF");
			}*/
		}

		// 攻撃が当たった回数
		if(app.bDrawCount){
			char count[255];
			int y = rc.Height()-100;

			itoa(app.crPuppet.hittingCount, count, 10);
			pDC->TextOut(0,   y, "You : ");
			pDC->TextOut(100, y, strcat(count, " hits!"));

			itoa(app.crUser.hittingCount, count, 10);
			y += 50;
			pDC->TextOut(0,   y, "VH  : ");
			pDC->TextOut(100, y, strcat(count, " hits!"));
		}
/*		// 評価実験用
		if(app.bDrawTest){
			char count[255];
			int y = rc.Height()-100;

			if(!app.crPlanner.bPlanner){
				pDC->TextOut(0, y, "Part.1");
			}
			else pDC->TextOut(0, y, "Part.2");

			itoa(app.crUser.atc, count, 10);
			pDC->TextOut(0,  y+50, "No.");
			pDC->TextOut(60, y+50, count);
		}*/
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionView クラスの診断

#ifdef _DEBUG
void CVHInteractionView::AssertValid() const
{
	CView::AssertValid();
}

void CVHInteractionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVHInteractionDoc* CVHInteractionView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVHInteractionDoc)));
	return (CVHInteractionDoc*)m_pDocument;
}
#endif //_DEBUG

void CVHInteractionView::InitHis(std::string hisList, float ps, float fs){
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
}

/////////////////////////////////////////////////////////////////////////////
// CVHInteractionView クラスのメッセージ ハンドラ
void CVHInteractionView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
/*	switch(nChar){
	case '8':	//	球を打ち出す
		ThrowSphere(0.2f);
		break;
	case '9':
		ThrowSphere(0.3f);
		break;
	case '0':
		ThrowSphere(0.4f);
		break;
	}*/
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CVHInteractionView::ThrowSphere(float r){
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
//	csphere->pmaterial = new PHPhysicalMaterial;
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

BOOL CVHInteractionView::OnEraseBkgnd(CDC* pDC) 
{
	// MFCはデフォルトで背景を白く塗る．ちらつくので何もしない関数でオーバーライド．
	return true;
}

void CVHInteractionView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	app.Init(m_hWnd);
	//	描画用タイマーの設定
	drawTimerId = SetTimer(FWApp::TIMER_DRAW, 33, NULL);
}

void CVHInteractionView::OnTimer(UINT nIDEvent){
	if (nIDEvent == FWApp::TIMER_DRAW){
		Invalidate();
	}
}

void CVHInteractionView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
}

void CVHInteractionView::OnD3d(){
	app.CreateD3DRender();
}

void CVHInteractionView::OnGL(){
	app.CreateGLRender();
}

BOOL CVHInteractionView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	app.PreviewMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}

void CVHInteractionView::OnHisConfig(){
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ReadRegistry();
	if (dlConfig.DoModal() == IDOK){
		WriteRegistry();
		InitHis(std::string(dlConfig.hisList), dlConfig.posScale, dlConfig.forceScale);
		if (app.scene) app.scene->SetTimeStep(dlConfig.timeStep);
		app.ConnectHis();
	}
}




void CVHInteractionView::OnVhDialog() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	app.vhDlg.Create(IDD_VH_CHECK);
	app.vhDlg.ShowWindow(SW_SHOWNORMAL);	
}
