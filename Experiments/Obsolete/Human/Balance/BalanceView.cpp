// BalanceView.cpp : CBalanceView クラスの動作の定義を行います。
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
// CBalanceView クラスの構築/消滅

CBalanceView::CBalanceView()
{
	// TODO: この場所に構築用のコードを追加してください。
	dt = 0.01f;
	bMinForce = true;

	// マウスの初期化
	mouse.Init();
	// 更新周期とマウスのスケールを設定
	mouse.Setdt(dt);
	mouse.SetScale(0.002f, 0.008f, 0.006f, 0.008f);	// SetScale(ポインタの並進，回転，カメラの並進，回転)

}

CBalanceView::~CBalanceView()
{
}

BOOL CBalanceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBalanceView クラスの描画

void CBalanceView::OnDraw(CDC* pDC){
	CBalanceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	if (!render) return;

	render->Render(scene);
	render->EndScene();
	render->Present();
	render->SetViewport(m_hWnd);
	render->ClearBuffer();
	render->BeginScene();
}
/////////////////////////////////////////////////////////////////////////////
// CBalanceView クラスの診断

#ifdef _DEBUG
void CBalanceView::AssertValid() const
{
	CView::AssertValid();
}

void CBalanceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBalanceDoc* CBalanceView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBalanceDoc)));
	return (CBalanceDoc*)m_pDocument;
}
#endif //_DEBUG

void CBalanceView::Load(std::string filename){
	if (!scene){		//	初期化がすんでいない場合は，後でロードすることにして，ファイル名だけ覚えておく．
		loadFile=filename;
		return;
	}
	//  リロード用にファイル名を覚えておく
	loadFileName = filename;

	render->EndScene();
#ifdef USE_MMTIMER
	timer.Release();
	Sleep(100);
#endif
	scene->Clear();
	scene->GetRenderers().Add(render);

//	doc->Print(DSTR);
	// ローダーの初期化
	FILoadScene loader;
	REGISTER_LOADER(&loader, SceneGraph);			//	シーングラフのローダを登録
	REGISTER_LOADER(&loader, ImpD3D);				//	D3Dのノードのローダを登録
	REGISTER_LOADER(&loader, Collision);			//	衝突判定ノードのローダを登録
	REGISTER_LOADER(&loader, Physics);				//	物理シミュレータのローダを登録
	REGISTER_LOADER(&loader, Graphics);				//	グラフィックスのローダを登録
	//	ファイルからドキュメントオブジェクトをロード
	D3FileDoc fileDoc;			//	ファイルをドキュメントオブジェクトに変換するオブジェクト
	fileDoc.renderer = (D3Render*)render;
	fileDoc.Load(filename);		//	ファイルのロード
	//	ドキュメントオブジェクトからシーングラフを作成
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
		// ロードするファイルごとに異なるデータを設定
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
// CBalanceView クラスのメッセージ ハンドラ
void CBalanceView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	Affinef afBody = mouse.GetAxis().inv();
	float df = 1.0f ; //移動変化量
	if ( nChar == VK_SPACE ) Step();						//	1ステップ実行
#ifdef USE_MMTIMER
	if ( nChar == 'K' ) timer.Release();					//	連続実行の停止
	if ( nChar == 'T' )	timer.Create();						//	連続実行の再開
	if ( nChar == 'H' )	timer.Thread();						//	連続実行の再開
#else
	if ( nChar == 'K' ) KillTimer(wmTimerID);				//	連続実行の停止
	if ( nChar == 'T' )	wmTimerID = SetTimer(1, 5, NULL);	//	連続実行の再開
#endif
	// カメラの平行移動
	if ( nChar == 'Q' ) afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	if ( nChar == 'W' ) afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	if ( nChar == 'Z' ) afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	if ( nChar == 'A' ) afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	if ( nChar == VK_DOWN ) afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	if ( nChar == VK_UP ) afBody.Pos() = afBody.Pos() + afBody.Ez() * df;

	// カメラの回転
	if ( nChar == VK_LEFT ) afBody = Affinef::Rot(Radf(-5),afBody.Ey()) * afBody;
	if ( nChar == VK_RIGHT ) afBody = Affinef::Rot(Radf(5),afBody.Ey()) * afBody;
	if ( nChar == 'S' ) afBody = Affinef::Rot(Radf(-5),afBody.Ex()) * afBody;
	if ( nChar == 'X' ) afBody = Affinef::Rot(Radf(5),afBody.Ex()) * afBody;

	// ファイルリロード
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
	// ポインタとカメラをリセット
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
	mouse.Update();		// 押されているボタンの状態から位置・姿勢を更新
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
	
	if (loadFile.length()){	//	ロードしたかったファイルがあればここでロード
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
		mouse.Update();		// 押されているボタンの状態から位置・姿勢を更新

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
			
			DSTR << "force = " << force << std::endl;

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
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	timer.Release();
}

void CBalanceView::OnIdle()
{
	Invalidate();
}


//-------- マウスのイベントの取得 ---------------------------

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



