// VirtualHumanView.cpp : CVirtualHumanView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "VirtualHuman.h"
#include "VirtualHumanDoc.h"
#include "VirtualHumanView.h"

#include <Physics/PHSolid.h>
#include <Physics/PHContactEngine.h>
#include <Graphics/GRCamera.h>
#include <Graphics/GRMaterial.h>
#include <ImpD3D/D3Render.h>
#include <GraphicsGL/GLRenderWGL.h>
#include <WinBasis/WBMMTimer.h>
#include <fstream>
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;

#else

//	If you need stable update rate for haptic interfaces, uncomment this
//	and use multi media timer.
//#define USE_MMTIMER

#endif


volatile int stepCount;
static int waitCount;

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView

IMPLEMENT_DYNCREATE(CVirtualHumanView, CView)

BEGIN_MESSAGE_MAP(CVirtualHumanView, CView)
	//{{AFX_MSG_MAP(CVirtualHumanView)
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
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView クラスの構築/消滅

CVirtualHumanView::CVirtualHumanView()
{
	// TODO: この場所に構築用のコードを追加してください。
	dt = 0.0005f;
	appState = INVALID;
	bMinForce = true;

	// マウスの初期化
	mouse.Init();
	// 更新周期とマウスのスケールを設定
	mouse.SetScale(0.002f, 0.008f, 0.040f, 0.008f);	// SetScale(ポインタの並進，回転，カメラの並進，回転)
}

CVirtualHumanView::~CVirtualHumanView()
{
}

BOOL CVirtualHumanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView クラスの描画

void CVirtualHumanView::OnDraw(CDC* pDC){
	CVirtualHumanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	if (!render) return;
	render->Render(scene);			//	シーンの描画
    if (render->CanDraw()){			//	diff fileの表示テスト
		if (render->camera) render->SetViewMatrix(render->camera->data.view);
		static Affinef afw;
		render->SetModelMatrix(afw);
		diff.Draw(render, human, time);
	}
	render->EndScene();				//	描画終了
	render->Present();				//	表示
	render->SetViewport(m_hWnd);	//	Viewportの設定
	render->ClearBuffer();			//	バッファクリア
	render->BeginScene();			//	描画開始
}

void CVirtualHumanView::Load(std::string filename){
	if (!scene){		//	初期化がすんでいない場合は，後でロードすることにして，ファイル名だけ覚えておく．
		loadFile=filename;
		return;
	}
	//  リロード用にファイル名を覚えておく
	loadFileName = filename;
	PHContactEngine* ce=NULL;
	scene->GetBehaviors().Find(ce);
	bool bDraw = true;
	if (ce) bDraw = ce->bDraw;

#ifdef USE_MMTIMER	//	マルチメディアタイマーの停止
	timer.Release();
	Sleep(100);
#endif
	GRCameraData oldCamData = render->camera->data;
	
	//	シーンの初期化
	scene->Clear();									//	シーングラフのクリア
	scene->GetRenderers().Add(render);				//	レンダラを追加
	// ローダーの初期化
	FILoadScene loader;
	REGISTER_LOADER(&loader, SceneGraph);			//	シーングラフのローダを登録
	REGISTER_LOADER(&loader, ImpD3D);				//	D3Dのノードのローダを登録
	REGISTER_LOADER(&loader, Collision);			//	衝突判定ノードのローダを登録
	REGISTER_LOADER(&loader, Physics);				//	物理シミュレータのローダを登録
	REGISTER_LOADER(&loader, Graphics);				//	グラフィックスのローダを登録
	//	ファイルからドキュメントオブジェクトをロード
	D3FileDoc fileDoc;			//	ファイルをドキュメントオブジェクトに変換するオブジェクト
	fileDoc.Load(filename);		//	ファイルのロード
	//	ドキュメントオブジェクトからシーングラフを作成
	loader.Load(scene, &fileDoc);
	//	カメラの初期状態を記録
	static bool bFirst=true;
	if (bFirst){
		initialCamera = render->camera->data;
		bFirst = false;
	}else{
		render->camera->data = oldCamData;
		if (render->camera->frPosture) render->camera->frPosture->SetPosture(oldCamData.view);
	}
	scene->GetBehaviors().Find(ce);
	if (ce) ce->bDraw = bDraw;
	
	//	力覚ポインタをシーンと接続
	pointer = (PHSolid*)scene->FindObject("Pointer");
	if (!pointer) pointer = (PHSolid*)scene->FindObject("Pointer1");
	if (pointer){
		pointer->SetFramePosition(Vec3f());
		pointer->SetOrientation(Quaternionf());
		pointer->SetAngularVelocity(Vec3f());
		pointer->SetVelocity(Vec3f());
		pointer->GetFrame()->SetPosture(Affinef());	
		// マウスデバイスの設定
		mouse.SetMass(pointer->GetMass());
		mouse.SetPos(pointer->GetFramePosition());
		mouse.SetOri(pointer->GetOrientation());
	}
	mouse.SetAxis(render->camera->data.view);
	mouse.SetAxisInit(render->camera->data.view);

	//	Virtual Human をシーンと接続
	human.Connect(scene);
	if (human.IsLoaded()){
		//	diffのロード
		if (!diff.IsLoaded()){
			std::ifstream file("Test.DA");
			if (file.good()) diff.Load(file);
		}
		//	VHの大きさをあわせる．
		diff.SetHumanScale(human);
		//	ジョイントエンジンの再初期化
		PHJointEngine* je;
		scene->GetBehaviors().Find(je);
		je->Loaded(scene);
		//	アプリケーションの状態
		time = 0;
		if (appState == INVALID){
			appState = MAKE_ANGLE;
			human.angles.clear();
//			scene->GetBehaviors().Find(collision);
//			if (collision) scene->GetBehaviors().Del(collision);
//			scene->GetBehaviors().Find(gravity);
//			if (gravity) scene->GetBehaviors().Del(gravity);
		}else if (appState == MAKE_ANGLE){
			appState = MAKE_FORCE;
		}else if (appState == MAKE_FORCE){
			appState = SET_TORQUE;
			//force.txtのロード
			if (!fFile.IsLoaded()){
				std::ifstream infile("force.txt");
				if (infile.good()) fFile.Load(infile);
			}
		}
	}else{
		appState = INVALID;
	}

	//////////////////////////////////////////
	///////////   HeadStand  /////////////////
	//////////////////////////////////////////
	scene->FindObject(hStand.joHeadStand1, "joPole01");
	scene->FindObject(hStand.joHeadStand2, "joPole02");
	scene->FindObject(hStand.soHeadStand, "soPole01");


	if(hStand.soHeadStand){
		appState = hoge;
		time = 0;
		if(hStand.joHeadStand1){
			appState = OUT_TORQUE;
			DSTR << "state = OUTTORQUE" << std::endl;
		}
	}
	if(hStand.joHeadStand2){
		appState = START_TORQUE;
		//appState = NON_CONT;
		time = 0;
		DSTR << "state = START_TORQUE" << std::endl;
	}
	scene->Print(DSTR);
	waitCount = 0;

#ifdef USE_MMTIMER	//	マルチメディアタイマーの起動
	timer.Create();
#endif
}

void CVirtualHumanView::Save(const char* filename){
	if (!scene->GetWorld()) return;
	D3SaveScene saver;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	REGISTER_SAVER(&saver, SceneGraph);				//	シーングラフのローダを登録
	REGISTER_SAVER(&saver, ImpD3D);					//	D3Dのノードのローダを登録
	REGISTER_SAVER(&saver, Collision);				//	衝突判定ノードのローダを登録
	REGISTER_SAVER(&saver, Physics);				//	物理シミュレータのローダを登録
	REGISTER_SAVER(&saver, Graphics);				//	グラフィックスのローダを登録
	saver.Save(doc, scene);
	D3FileDoc fileDoc;
	for (unsigned int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(filename, doc->Child(i));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView クラスのメッセージ ハンドラ
void CVirtualHumanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	Invalidate();
#ifdef USE_MMTIMER
	if ( nChar == 'K' ) timer.Release();					//	連続実行の停止
	if ( nChar == 'T' )	timer.Create();						//	連続実行の再開
	if ( nChar == 'H' )	timer.Thread();						//	連続実行の再開
#else
	if ( nChar == 'K' ) KillTimer(simTimerID);				//	連続実行の停止
	if ( nChar == 'T' )	simTimerID = SetTimer(1, 10, NULL);	//	連続実行の再開
#endif
	if ( nChar == VK_SPACE ) bMinForce = !bMinForce;		//	力覚のON/OFF
	if ( nChar == 'D' ){									//	接触力描画のON/OFF
		PHContactEngine* ce=NULL;
		scene->GetBehaviors().Find(ce);
		if (ce) ce->bDraw = !ce->bDraw;
	}
	if ( nChar == 'R' ){									// ファイルリロード
		Load(loadFileName.c_str());
		return;
	}

	if ( nChar == 'N' ){									//	更新周期の変更
		if (dt > 0.01f) dt -= 0.01f;
		else dt -= 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	if ( nChar == 'M' ){									//	更新周期の変更
		if (dt >= 0.01f) dt += 0.01f;
		else dt += 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	if (nChar == 'F'){										//	フルスクリーン
		render->EndScene();
		render->Fullscreen();
	}
	if (nChar == 'G'){										//	ウィンドウモード
		render->EndScene();
		render->Window();
		render->BeginScene();
	}
	//	視点移動関係
	Affinef afBody = mouse.GetAxis().inv();
	float df = 1.0f ; //移動変化量
	if ( nChar == VK_SPACE ) Step();						//	1ステップ実行
	// カメラの平行移動
	if ( nChar == 'W' ) afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	if ( nChar == 'Q' ) afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	if ( nChar == 'A' ) afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	if ( nChar == 'Z' ) afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	if ( nChar == VK_DOWN ) afBody.Pos() = afBody.Pos() + afBody.Ez() * df;
	if ( nChar == VK_UP ) afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	// カメラの回転
	if ( nChar == VK_LEFT ) afBody = Affinef::Rot(Radf(5),'y') * afBody;
	if ( nChar == VK_RIGHT ) afBody = Affinef::Rot(Radf(-5),'y') * afBody;
	if ( nChar == 'X' ) afBody = Affinef::Rot(Radf(5),afBody.Ex()) * afBody;
	if ( nChar == 'S' ) afBody = Affinef::Rot(Radf(-5),afBody.Ex()) * afBody;
	if ( nChar == 'C' ){									// ポインタとカメラをリセット
		afBody = initialCamera.view.inv();
		if (pointer){
			pointer->SetFramePosition(Vec3f());
			pointer->SetOrientation(Quaternionf());
			pointer->SetAngularVelocity(Vec3f());
			pointer->SetVelocity(Vec3f());
			pointer->GetFrame()->SetPosture(Affinef());	
		}
	}
	afBody.Ex().unitize();
	afBody.Ey().unitize();
	afBody.Ez().unitize();
	mouse.SetAxis(afBody.inv());
	UpdateMouse();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CVirtualHumanView::OnEraseBkgnd(CDC* pDC){ return true; }

void TimerFunc(void* arg){
	CVirtualHumanView* view = (CVirtualHumanView*) arg;
	view->Step();
}

void CVirtualHumanView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	if (!render){
		render = new GLRenderWgl;
		render->Create(m_hWnd);
		scene = new SGScene();
		scene->GetRenderers().Add(render);
		mouse.SetAxis(render->camera->data.view);
	}
	scene->SetTimeStep(dt);
	timer.Resolution(dt*1000);
	timer.Interval(dt*1000);
#ifdef USE_MMTIMER
	timer.Set(TimerFunc, this);
	timer.Thread();
#else
	simTimerID = SetTimer(1, dt*200, NULL);
#endif
	drawTimerID = SetTimer(2, 30, NULL);
	
	if (loadFile.length()){	//	ロードしたかったファイルがあればここでロード
		Load(loadFile);
		loadFile = "";
	}
}
void CVirtualHumanView::UpdateMouse(){
	if (mouse.IsGood()){
//		mouse.Update();		// 押されているボタンの状態から位置・姿勢を更新

		// カメラをマウスの基準座標軸に更新
		render->camera->data.view = mouse.GetAxis();
		
		//	ポインタの位置と姿勢をmouseから取得
		Vec3f pos = mouse.GetPos();
		Quaternionf ori = mouse.GetOri();

		if (pointer){
			//	力とトルクをシミュレータのノード(pointer)から取得
			Vec3f force, torque;
			force = pointer->GetForce() ;
			torque = pointer->GetTorque() ;
			
			//DSTR << "force = " << force << std::endl;

			//	提示力の設定
			if (bMinForce) mouse.SetMinForce();
			else mouse.SetForce(force, torque);
			
			//	solidに位置と向きを設定．
			pointer->SetFramePosition(pos*15.0f);
			pointer->SetOrientation(ori);
			//	速度は0にしている．(本当はマウスの速度を入れた方が良い)
			pointer->SetAngularVelocity(Vec3f());
			pointer->SetVelocity(Vec3f());
			//	シミュレータノードの位置を更新
			Affinef posture;
			Matrix3d rot;
			pointer->GetOrientation().to_matrix(rot);
			posture.Rot() = rot;
			posture.Pos() = pointer->GetFramePosition();
			pointer->GetFrame()->SetPosture(posture);
		}
	}
}
void CVirtualHumanView::Step(){
	scene->ClearForce();
	if (appState == MAKE_ANGLE){
		if (waitCount++ < 300) time = 0;
		bool rv = diff.AddSpringForce(human, time, dt);
		if (time > 0) human.SaveJointAngle();
		if (!rv){	//	最後のデータだった場合，もう一度ロードしてappStateを進める．
			Load(loadFileName.c_str());
			waitCount = 0;
		}
	}else if (appState == MAKE_FORCE){
		if (waitCount++ < 500){
			time = 0.0f;
			diff.AddSpringForce(human, time, dt);
			human.SetSpring(dt);
			human.LoadJointAngle(time, dt);
		}else if (waitCount++ < 2000){
			time = 0.0f;
			human.LoadJointAngle(time, dt);
		}else{
			bool rv = human.LoadJointAngle(time, dt);
			if (!rv){	//	最後のデータだった場合，もう一度ロードしてappStateを進める．
				std::ofstream of("force.txt");
				for(float t=0.001f; t<time; t+= 0.001f){
					int c = t / dt;
					TDiffRecord pos, vel;
					diff.frf.GetRecord(pos, vel, t);
					if (c < human.angles.size()){
						of << t << "\t";
						of << pos.data[1].X() << "\t";
						of << pos.data[1].Y() << "\t";
						of << pos.data[1].Z() << "\t";
						of << human.angles[c].force.X() << "\t";
						of << human.angles[c].force.Y() << "\t";
						of << human.angles[c].force.Z() << "\t";
						for(int i=0; i<human.joints.size(); ++i){
							of << human.angles[c].jointTorque[i] << "\t";
						}
						of << std::endl;
					}
				}
				Load(loadFileName.c_str());
				waitCount = 0;
			}
		}

	}else if(appState == SET_TORQUE){
		DSTR << fFile.jointTorque.size() << std::endl;
		DSTR << fFile.jointTorque[0].torqueData[0] << std::endl;
		if (waitCount++ < 500){
			time = 0.0f;
			diff.AddSpringForce(human, time, dt);
		}else if (waitCount++ < 2000){
			time = 0.0f;
			fFile.SetVHTorque(human,time,dt);
		}else{
			bool rv = fFile.SetVHTorque(human,time,dt);
			if (!rv){	//	最後のデータだった場合，もう一度ロードしてappStateを進める．
				Load(loadFileName.c_str());
				waitCount = 0;
			}
		}
/*
		bool rv = diff.AddSpringForce(human, time, dt);
		if (time > 0) human.SaveJointAngle();
		if (!rv){	//	最後のデータだった場合，もう一度ロードしてappStateを進める．
			//ファイルへの書き出し
			std::ofstream off("force1.txt");
			for(float t=0; t<time; t+= 0.001f){
				int c = t / dt;
				off << fFile.jointTorque[c].time << "\t";
				for(int j = 0; j < 30; j++){
					off << fFile.jointTorque[c].torqueData[j] << "\t";
				}
				off << std::endl;
			}
			Load(loadFileName.c_str());
			waitCount = 0;
		}
*/		
	}else if(appState == OUT_TORQUE){
		hStand.SaveHSTorque();
		
//		DSTR << "時間:" << time << std::endl;
//		DSTR << "ジョイントの角度：" << hStand.joHeadStand1->GetPosition() <<std::endl;
//		DSTR << "速度：" << hStand.joHeadStand1->GetVelocity() << std::endl;
		//int c = (int)(time / dt);
		//DSTR << "トルク：" << hStand.HeadStandTorque[c] <<std::endl;
				
		//if(abs(hStand.joHeadStand1->GetPosition()) < 0.0001 
		//	&& abs(hStand.joHeadStand1->GetTorque())< 0.001){
		if(abs(hStand.joHeadStand1->GetVelocity()) < 0.001 ){
			if(hStand.checkZero == true){
				hStand.checkCount++;
			}
			hStand.checkZero = true;
		}
		else{
			hStand.checkZero = false;
		}
		if(hStand.checkCount > 1000){
			hStand.checkCount = 0.0;
			hStand.checkZero = false;
			time = 0;
			std::ofstream of("torque.txt");
			for(int i=0; i<hStand.HeadStandTorque.size(); ++i){
				of << i << "\t";
				of << hStand.HeadStandTorque[i] << "\n";
			}
			//Load("data/Two1.x");
			Load("data/OneLink2.x");
		}

	}else if(appState == START_TORQUE){

		DSTR << "時間:"<<time << std::endl;
		DSTR << "ジョイントの角度：" << hStand.joHeadStand2->GetPosition() <<std::endl;
		DSTR << "トルク:" << hStand.joHeadStand2->GetTorque() << std::endl;
			
		bool rv = hStand.SetHSTorque(time,dt);

		//if(abs(hStand.joHeadStand2->GetPosition()) < 0.0001 
			//&&abs(hStand.joHeadStand2->GetTorque()) < 0.001){
		if(abs(hStand.joHeadStand2->GetVelocity()) < 0.001 ){
			if(hStand.checkZero == true){
				hStand.checkCount++;
			}
			hStand.checkZero = true;
		}
		else{
			hStand.checkZero = false;
		}

		if(hStand.checkCount > 1000 || !rv){
			hStand.checkCount = 0.0;
			hStand.checkZero = false;
			hStand.HeadStandTorque.clear();
			time = 0;
			//Load("data/Two.x");
			appState = OUT_TORQUE;
			Load("data/OneLink1.x");
		}
	}else if(appState == hoge){
		hStand.soHeadStand->AddForce(Vec3f(0.0,9.8,0.0));
		DSTR << hStand.soHeadStand->GetForce() << std::endl;
	}else if(appState == NON_CONT){
		hStand.joHeadStand2->SetTorque(1.0);
	}

	scene->GenerateForce();
	if(appState == NON_CONT){
		DSTR << hStand.joHeadStand2->GetTorque() << std::endl;
	}else if (appState == MAKE_FORCE && time > 0){
		human.SaveForce(time, dt);
		human.SaveTorque(time, dt);
	}else if (appState == OUT_TORQUE && time > 0){
		//hStand.SaveHSTorque();
	}
	scene->Integrate();
	UpdateMouse();
	stepCount ++;
	time += dt;
	//DSTR << "PoleAngle:" << fFile.jointTorque[0].torqueData[0] << std::endl;
}

void CVirtualHumanView::OnTimer(UINT nIDEvent){
	static bool bUpdate;
	if (nIDEvent == 1){
		for(int i=0; i<0.01/dt; ++i) Step();
		bUpdate = true;
	}else{
		if (bUpdate){
			bUpdate = false;
			Invalidate();
		}
	}
}
void CVirtualHumanView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	timer.Release();
}

//-------- マウスのイベントの取得 ---------------------------

void CVirtualHumanView::OnMouseMove(UINT nFlags, CPoint point) 
{
	mouse.OnButtonMove(nFlags, point.x, point.y);
	CView::OnMouseMove(nFlags, point);
}

void CVirtualHumanView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	mouse.OnButtonDown(point.x, point.y);
	CView::OnLButtonDown(nFlags, point);
}

void CVirtualHumanView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	mouse.OnButtonDown(point.x, point.y);
	CView::OnMButtonDown(nFlags, point);
}

void CVirtualHumanView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	mouse.OnButtonDown(point.x, point.y);
	CView::OnRButtonDown(nFlags, point);
}

void CVirtualHumanView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	mouse.OnButtonUp();
	CView::OnLButtonUp(nFlags, point);
}

void CVirtualHumanView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	mouse.OnButtonUp();
	CView::OnMButtonUp(nFlags, point);
}

void CVirtualHumanView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	mouse.OnButtonUp();
	CView::OnRButtonUp(nFlags, point);
}


void CVirtualHumanView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (nFlags == MK_SHIFT + MK_LBUTTON ){
		mouse.OnShiftLButtonDClick();
	}
	else if (nFlags == MK_LBUTTON) {
		mouse.OnLButtonDClick();
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CVirtualHumanView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if ( nFlags == MK_RBUTTON) {
		mouse.OnRButtonDClick();
	}
	CView::OnRButtonDblClk(nFlags, point);
}

BOOL CVirtualHumanView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	mouse.OnWheel(nFlags, zDelta/10);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView クラスの診断

#ifdef _DEBUG
void CVirtualHumanView::AssertValid() const
{
	CView::AssertValid();
}

void CVirtualHumanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVirtualHumanDoc* CVirtualHumanView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVirtualHumanDoc)));
	return (CVirtualHumanDoc*)m_pDocument;
}
#endif //_DEBUG

