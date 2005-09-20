// BeetleView.cpp : CBeetleView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "Beetle.h"

#include "BeetleDoc.h"
#include "BeetleView.h"
#include "ConsoleDlg.h"
#include "Controller.h"
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <Physics/PHJoint.h>
#include <Collision/CDMesh.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <ImpD3D/D3Mesh.h>
#include <Graphics/GRCamera.h>
#include <fstream>
#include <sstream>


#include <Physics/PHContactEngine.h>
#include <Graphics/GRMaterial.h>
#include <Graphics/GRSphere.h>
#include <Collision/CDSphere.h>
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//グローバル変数の定義
UTRef<Spr::SGScene> g_Scene;
Beetle		g_theBeetle;
bool		g_bShowingConsole = false;

/////////////////////////////////////////////////////////////////////////////
// CBeetleView
IMPLEMENT_DYNCREATE(CBeetleView, CView)

BEGIN_MESSAGE_MAP(CBeetleView, CView)
	//{{AFX_MSG_MAP(CBeetleView)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CONSOLE, OnShowConsole)
END_MESSAGE_MAP()

void timerFunc(void* arg){
	CBeetleView* view = (CBeetleView*)arg;
	view->Step();	
}

/////////////////////////////////////////////////////////////////////////////
// CBeetleView クラスの構築/消滅

	BeetleState keyInst;

CBeetleView::CBeetleView()
{
	keyInst = STOP;
	beetle = NULL;
	execFlag = true;

//	SPIDAR周り
	bMinForce = true;
	dt = 0.01f;
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.RPool().Register(new DRUsb20Sh4(0));
	devMan.Init();
	DSTR << devMan;
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	(0.390f/2)		//	x方向の辺の長さ/2
	#define PY	(0.395f/2)		//	y方向の辺の長さ/2
	#define PZ	(-0.405f/2)		//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f( PX, PY,-PZ),	Vec3f(-PX, PY, PZ), Vec3f( PX,-PY, PZ), Vec3f(-PX,-PY,-PZ)},
		{Vec3f(-PX, PY,-PZ),	Vec3f( PX, PY, PZ), Vec3f(-PX,-PY, PZ), Vec3f( PX,-PY,-PZ)}
	};
	spidar[0].Init(devMan, motorPos[0], 0.3f, 2.924062107079e-5f, 0.5f, 6.0f);
	spidar[1].Init(devMan, motorPos[1], 0.3f, -2.924062107079e-5f, 0.5f, 6.0f);

	spidar[0].Calib();
	spidar[1].Calib();

	timer.Resolution(7);
	timer.Interval(7);
	timer.Set(timerFunc, this);
}

CBeetleView::~CBeetleView()
{
}

BOOL CBeetleView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBeetleView クラスの描画

void CBeetleView::OnDraw(CDC* pDC)
{
	if (!render) return;
	render->Render(g_Scene);
	render->EndScene();
	render->Present();
	render->SetViewport(m_hWnd);
	render->ClearBuffer();
//	render->device->SetRenderState(D3DRS_AMBIENT, 0x808080);
	render->BeginScene();
}
/////////////////////////////////////////////////////////////////////////////
// CBeetleView クラスの診断

#ifdef _DEBUG
void CBeetleView::AssertValid() const
{
	CView::AssertValid();
}

void CBeetleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBeetleDoc* CBeetleView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBeetleDoc)));
	return (CBeetleDoc*)m_pDocument;
}
#endif //_DEBUG

///////////////////////////////////////////////////////////////////////////
//読み込みと保存
Spr::PHContactEngine* contactEngine;

void CBeetleView::Load(std::string fn)
{
	if(!g_Scene){
		initialLoadFile = fn;
		return;
	}
	bool timerFlag = timer.IsCreated();
	if (timerFlag) timer.Release();
	loadFileName = fn;
	beetle = NULL;
	g_Scene->Clear();
	//シーンにレンダラを関連付ける
	g_Scene->GetRenderers().Add(render);

	//読み込み
	FILoadScene loader;
	REGISTER_LOADER(&loader, SceneGraph);
	REGISTER_LOADER(&loader, ImpD3D);
	REGISTER_LOADER(&loader, Collision);
	REGISTER_LOADER(&loader, Physics);
	REGISTER_LOADER(&loader, Graphics);

	D3FileDoc fileDoc;
	fileDoc.Load(loadFileName);
	loader.Load(g_Scene, &fileDoc);

	//シーンからカメラオブジェクトを取得
	GRCamera* cam = NULL;
	g_Scene->GetRenderers().GetObjects().Find(cam);
	//無ければ作成してセット
	if (!cam) g_Scene->GetRenderers().Set(new GRCamera);
	
	//シーンとコントローラを連動
	if(!g_theBeetle.ConnectToSpringhead())
		MessageBox("神経接続に失敗しました");
	//	spidarとの接続
	for(int i=0; i<2; ++i){
		std::ostringstream oss;
		oss << "Pointer" << i+1;
		pointer[i] = DCAST(PHSolid, g_Scene->FindObject(oss.str()));
	}
	//	AIのためのSolidの接続
	beetle = NULL;
	atracters.clear();
	fpRecords.clear();
	PHSolid* solid=NULL;
	char* names[] = { "soWaterMelon", "soBlock1", "soBlock2", "soBlock3"};
	for(int i=0; i<sizeof(names)/sizeof(names[0]); ++i){
		PHSolid* solid = DCAST(PHSolid, g_Scene->FindObject(names[i]));
		if (solid){
			atracters.push_back(solid);
		}else{
			DSTR << "Can not connect to " << names[i] << std::endl;
		}
	}
	beetle = DCAST(PHSolid, g_Scene->FindObject("soBody"));

	CDCollisionEngine* ce;
	g_Scene->GetBehaviors().Find(ce);
	g_Scene->GetBehaviors().Find(contactEngine);
	std::vector<SGFrame*> frames;
	char* fnames[] = {"frHead", "frTail"};
	for(int i=0; i<sizeof(fnames)/sizeof(fnames[0]); ++i){
		SGFrame* fr = DCAST(SGFrame, g_Scene->FindObject(fnames[i]));
		if (fr) frames.push_back(fr);
	}
	if (ce && contactEngine){
		for(unsigned i=0; i<frames.size(); ++i){
			for(int j=0; j<2; ++j){
				PHContactEngine::FramePairRecord* fpr = contactEngine->GetFramePairRecord(frames[i], pointer[j]->GetFrame());
				if (!fpr){
					fpr = contactEngine->GetFramePairRecord(pointer[j]->GetFrame(), frames[i]);
				}
				fpRecords.push_back(fpr);
			}
		}
	}
	if (timerFlag) timer.Create();
}

void CBeetleView::Save(std::string fn)
{
	if (!g_Scene->GetWorld()) return;
	D3SaveScene saver;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	REGISTER_SAVER(&saver, SceneGraph);
	REGISTER_SAVER(&saver, ImpD3D);
	REGISTER_SAVER(&saver, Collision);
	REGISTER_SAVER(&saver, Physics);
	REGISTER_SAVER(&saver, Graphics);
	saver.Save(doc, g_Scene);
	D3FileDoc fileDoc;
	for (int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(fn.c_str(), doc->Child(i));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBeetleView クラスのメッセージ ハンドラ

void CBeetleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	Affinef afCam = render->camera->data.view.inv();

	float df = 1.0f ; //移動変化量
	if ( nChar == 'T' ) execFlag = !execFlag;				//	連続実行の再開
	switch(nChar){
	//1ステップ実行
	case VK_SPACE: Step(); break;	
    //虫の操作(vi バインド)
	case 'H':keyInst =(LEFTTURN);	break;	//左旋回
	case 'J': keyInst =(FORWARD);	break;	//前進
	case 'K': keyInst =(BACKWARD);	break;	//後進
	case 'L': keyInst =(RIGHTTURN);	break;	//右旋回

	case 'Q': afCam.Pos() = afCam.Pos() - afCam.Ex() * df; break;
	case 'W': afCam.Pos() = afCam.Pos() + afCam.Ex() * df; break;
	case 'Z': afCam.Pos() = afCam.Pos() - afCam.Ey() * df; break;
	case 'A': afCam.Pos() = afCam.Pos() + afCam.Ey() * df; break;
	//ズーム
	case VK_UP : afCam.Pos() = afCam.Pos() - Vec3f(afCam.EzX(),0,afCam.EzZ()) * df; break;
	case VK_DOWN: afCam.Pos() = afCam.Pos() + Vec3f(afCam.EzX(),0,afCam.EzZ()) * df; break;
	//シーン回転
	case VK_RIGHT: afCam.Rot() = Matrix3f::Rot((float)Rad(-5),'y') * afCam.Rot(); break;
	case VK_LEFT: afCam.Rot() = Matrix3f::Rot((float)Rad(5),'y') * afCam.Rot(); break;
	case 'S': afCam = Affinef::Rot((float)Rad(-5),'x') * afCam; break;
	case 'X': afCam = Affinef::Rot((float)Rad(5),'x') * afCam; break;
	//再読み込み
	case 'R': Load(loadFileName); return;
	//	MMTimer
	case 'M':
		KillTimer(simTimerID);
		timer.Create();
		break;
	case 'N':
		timer.Release();
		simTimerID = SetTimer(1, UINT(dt*200), NULL);
		break;
	//フルスクリーン
	case 'F':
		render->EndScene();
		render->device.Fullscreen();
		render->Setup(render->device.GetSurfaceSize());
		render->device->ShowCursor(false);
		render->BeginScene();
		break;
	//ウィンドウ表示
	case 'G':{
		render->EndScene();
		render->device.Window();
		RECT rc;
		GetClientRect(&rc);
		render->Setup( Vec2f(rc.right, rc.bottom) );
		render->device->ShowCursor(true);
		render->BeginScene();
		break;
		}
	//	SPIDAR関係
	case 'C':{	//	Calibration
		bool timerFlag = timer.IsCreated();
		if (timerFlag) timer.Release();
		for(int i=0; i<2; ++i) spidar[i].BeforeCalib();
		Sleep(100);
		for(int i=0; i<2; ++i) spidar[i].Calib();
		if (timerFlag) timer.Create();
		}break;
	case VK_RETURN:	//	力覚ON・OFF
		bMinForce = !bMinForce;
		break;
	//	球を打ち出す
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
	render->camera->data.view = afCam.inv();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

std::vector< UTRef<PHSolid> > bullets;
void CBeetleView::ThrowSphere(float r){
	bullets.resize(10);
	static int pos=0;
	UTRef<PHSolid> solid = bullets[pos];
	if (!solid) solid = new PHSolid;
	solid->SetMass(r*r*5);
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
	g_Scene->GetBehaviors().Find(ce);
	if (ce){
		SGFrame* frCol = ce->GetFrame(0);
		if (frCol && frCol->GetParent()) frCol->GetParent()->AddChildObject(frame, g_Scene);
	}else{
		g_Scene->GetWorld()->AddChildObject(frame, g_Scene);
	}
	Affinef afSphere = render->camera->data.view.inv();
	Vec3f dir = -afSphere.Ez();
	afSphere.Ey().X() = 0;
	afSphere.Ey().Z() = 0;
	afSphere.Ey().unitize();
	afSphere.Ex().Y() = 0;
	afSphere.Ex().unitize();
	afSphere.Ez().Y() = 0;
	afSphere.Ez().unitize();
	frame->SetPosture(afSphere);
	afSphere.Pos() += 3*dir;

	if (!sphere){
		sphere = new GRSphere;
		UTRef<GRMaterial> mat = new GRMaterial;
		mat->diffuse = Vec4f(0, 0.1f, 1.0f, 1.0f);
		mat->specular= Vec4f(0, 0.1f, 1.0f, 1.0f);
		mat->ambient = Vec4f(0, 0.1f, 0.1f, 1.0f);
		mat->emissive= Vec4f(0, 0.1f, 0.1f, 1.0f);
		sphere->AddChildObject(mat, g_Scene);
	}
	sphere->radius = r;
	sphere->slices = 16;
	sphere->stacks = 8;
	frame->AddChildObject(sphere, g_Scene);
	render->InitTree(frame, g_Scene);

	if (!csphere) csphere = new CDSphere;
	csphere->radius = r;
	csphere->pmaterial = new PHPhysicalMaterial;
	csphere->pmaterial->pMatData.reflexDamper = 0.3f;
	csphere->pmaterial->pMatData.reflexSpring = 3.0f;
	frame->AddChildObject(csphere, g_Scene);

	PHSolidContainer* sc;
	g_Scene->GetBehaviors().Find(sc);
	if (sc && !sc->solids.Find(solid)) sc->AddChildObject(solid, g_Scene);
	PHGravityEngine* ge;
	g_Scene->GetBehaviors().Find(ge);
	if (ge && !ge->solids.Find(solid)) ge->AddChildObject(solid, g_Scene);

	solid->SetVelocity(dir * 20.0f);
	solid->SetAngularVelocity(Vec3f());

	PHContactEngine* pce;
	g_Scene->GetBehaviors().Find(pce);
	if (pce && !pce->solids.Find(solid)){
		pce->AddChildObject(solid, g_Scene);
		pce->Init(g_Scene);
	}
	bullets[pos] = solid;
	pos++;
	if (pos >= 10) pos = 0;
}

void CBeetleView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//操縦キーを離したら虫を止める
	switch(nChar){
//		case 'H': case 'J': case 'K': case 'L':
	case VK_LEFT: case VK_UP: case VK_DOWN: case VK_RIGHT: 

		
//		g_theBeetle.SetState(NEUTRAL);
		keyInst = STOP;
	}
}

BOOL CBeetleView::OnEraseBkgnd(CDC* pDC){return true;}

void CBeetleView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	//レンダラとシーンの作成
	if (!render){
		render = new D3Render;
		render->device.Create(m_hWnd);
		g_Scene = new SGScene;
		g_Scene->SetTimeStep(dt);
	}
	//初期ロードファイルの指定があるならロードする
	if(!initialLoadFile.empty()){
		Load(initialLoadFile);
		initialLoadFile = "";
	}
	drawTimerID = SetTimer(2, 30, NULL);
	simTimerID = SetTimer(1, UINT(dt*200), NULL);
}

void CBeetleView::OnTimer(UINT nIDEvent){
	if (nIDEvent == 1){
		if (execFlag){
			for(int i=0; i<2; ++i) Step();
		}
	}else{
		if (bUpdate){
			bUpdate = false;
			Invalidate();
		}
	}
}

//コンソールを表示する
void CBeetleView::OnShowConsole()
{
	if(!g_bShowingConsole){
		if(!pdlgConsole)
			pdlgConsole = new CConsoleDlg;
		pdlgConsole->Create(IDD_CONSOLE, this);
		g_bShowingConsole = true;
	}
}


//////////////////////////////////////////////////////////////////////////////

WBPreciseTimer ptimer;
void CBeetleView::Step()
{
	//	シミュレータ
	ptimer.Start();
	g_theBeetle.Step(dt);
	g_Scene->ClearForce();
	g_Scene->GenerateForce();
	g_theBeetle.CheckForce();

	g_Scene->Integrate();
	ptimer.Stop();

#if 1	//	処理時間の出力
	static CDCollisionEngine* ce=NULL;
	if (!ce) g_Scene->GetBehaviors().Find(ce);
	if (ce){
//		DSTR << " GJK:" << ce->timerGjk.Clear();
//		DSTR << " Qhull:" << ce->timerQhull.Clear();
//		DSTR << " Normal:" << ce->timerNormal.Clear();
		DSTR << ce->timerGjk.Clear() << "\t";
		DSTR << ce->timerQhull.Clear() << "\t";
		DSTR << ce->timerNormal.Clear() << "\t";
	}
	static PHJointEngine* je=NULL;
	if (!je) g_Scene->GetBehaviors().Find(je);
	if (je){
//		DSTR << " Featherstone:" << je->timer.Clear();
		DSTR << je->timer.Clear() << "\t";
	}
//	DSTR << "  Total:" << ptimer.Clear() << std::endl;
	DSTR << ptimer.Clear() << std::endl;
#endif

	//	SPIDAR周り
	for(int i=0; i<2; ++i){
		if (spidar[i].IsGood() && pointer[i]){
			spidar[i].Update(dt);
			Vec3f pos = spidar[i].GetPos();
			static Vec3f lastPos[2];
			static Vec3f lastVel[2];
			Vec3f vel = pos - lastPos[i];
			const float maxVel = 0.02f;
			if (vel.norm() > maxVel && lastVel[i].norm() > maxVel){
				bMinForce = true;
				lastVel[i] = vel = Vec3f();
			}
			if (pos.Z() > 0.22){
				bMinForce = false;
			}
			Vec3f force, torque;
			force = pointer[i]->GetForce() * 0.2f;
			if (bMinForce) spidar[i].SetMinForce();
			else spidar[i].SetForce(force);

			pointer[i]->SetFramePosition(pos*30.0f + Vec3f(0,2.0f,0));
			pointer[i]->SetOrientation(Quaternionf());
			pointer[i]->SetAngularVelocity(Vec3f());
			pointer[i]->SetVelocity( Vec3f() );

			Affinef posture;
			Matrix3d rot;
			pointer[i]->GetOrientation().to_matrix(rot);
			posture.Rot() = rot;
			posture.Pos() = pointer[i]->GetFramePosition();
			pointer[i]->GetFrame()->SetPosture(posture);
			lastPos[i] = pos;
			lastVel[i] = vel;
		}
	}

	//	AI
	static int sleep;
	sleep++;
	if (beetle && sleep > 40){
		sleep = 0;
		static int count;
		count ++;

		//	一番近くのアトラクターを探す。
		float minDist = 1e10f;
		PHSolid* nearest=NULL;
		for(unsigned i=0; i<atracters.size(); ++i){
			float dist = (float)(beetle->GetFramePosition() - atracters[i]->GetFramePosition()).norm();
			if (dist < minDist){
				minDist = dist;
				nearest = atracters[i];
			}
		}
		//	一番近くのアトラクターに向かう。
		BeetleState aiInst = STOP;
		BeetleState walk = STOP;
		static bool bMove;
		if (bMove && rand() < 0.001*RAND_MAX){
			bMove = !bMove;
		}
		if (!bMove && rand() < 0.005*RAND_MAX){
			bMove = !bMove;
		}
		if (bMove && nearest){
			Affinef afBeetle = beetle->GetFrame()->GetPosture();
			Affinef afGoal = afBeetle.inv() * nearest->GetFrame()->GetPosture();
			Vec3f v3Goal = afGoal.Pos();
			if (v3Goal.norm() > 1.0f){
				float angGoal = atan2(v3Goal.X(), v3Goal.Z());
				if (abs(angGoal) > Rad(180-10)){
					walk = FORWARD;
				}else{
					if (angGoal > 0){
						walk = RIGHTTURN;
					}else{
						walk = LEFTTURN;
					}
				}
			}
		}
		//	触られているときの処理（最優先）
		for(unsigned i=0; i<fpRecords.size(); ++i){
			if (fpRecords[i]->reflexForce.norm() > 1e-5){
				if (i<2) walk = BACKWARD;
				else walk = FORWARD;
				fpRecords[i]->Clear();
			}
		}
		//	指令
		if (walk){
			aiInst = walk;
		}else{	//	ないときはランダム
			if (rand() < 0.2*RAND_MAX){
				int r = rand();
				if (r < 0.2*RAND_MAX){
					aiInst = LEFTTURN;
				}else if (r < 0.4*RAND_MAX){
					aiInst = RIGHTTURN;
				}else if (r < 0.6*RAND_MAX){
					aiInst = FORWARD;
				}else{
					aiInst = BACKWARD;
				}
			}else{
				aiInst = STOP;
			}
		}
		if (keyInst == STOP){
			g_theBeetle.SetState(aiInst);
		}else{
			g_theBeetle.SetState(keyInst);
		}
	}
	bUpdate = true;
}
