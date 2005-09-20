// DynaClockTestView.cpp : CDynaClockTestView クラスの動作の定義を行います。
//


#include "stdafx.h"
#include "DynaClockTest.h"

#include "DynaClockTestDoc.h"
#include "DynaClockTestView.h"
#include <Physics/PHSolid.h>
#include <Physics/PHPenalty.h>
#include <Physics/PHForceField.h>
#include <WinBasis/WBPreciseTimer.h>
#include <Collision/CDMesh.h>
#include <ImpD3D/D3FileDoc.h>
#include <ImpD3D/D3DocNode.h>
#include <ImpD3D/D3Mesh.h>
#include <Graphics/GRCamera.h>
#include <fstream>
#include <mmsystem.h>
using namespace Spr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//	ライブラリに影響を与えるフラグ
bool bUseContactVolume = false;


//
static int count;
//#define SPEEDTEST
#ifdef SPEEDTEST
static int stepCount;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestView


IMPLEMENT_DYNCREATE(CDynaClockTestView, CView)

BEGIN_MESSAGE_MAP(CDynaClockTestView, CView)
	//{{AFX_MSG_MAP(CDynaClockTestView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestView クラスの構築/消滅

CDynaClockTestView::CDynaClockTestView()
{
	// TODO: この場所に構築用のコードを追加してください。
	dt = 0.01f;
	execFlag = true;
}

CDynaClockTestView::~CDynaClockTestView()
{
}

BOOL CDynaClockTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestView クラスの描画

void CDynaClockTestView::OnDraw(CDC* pDC){
#ifdef SPEEDTEST
	stepCount ++;
	static int nBox=1;
	static bool first;
	if (!first){
		if (initialLoadFile.length()){
			Load(initialLoadFile.c_str());
			initialLoadFile="";
		}
		first = true;

		for(int i=0; i<14; ++i) CreateBoxAuto();
	}
	if (stepCount < 0){
		DSTR << stepCount << std::endl;
		ptimer.Clear();
	}

	if (stepCount > 20){
		float period = (float)ptimer.Clear() / 200.0f;
		DSTR << period << "\t" << nBox << std::endl;
		stepCount = 0;
		if (CreateBoxAuto()) nBox++;
	}
	for(int i=0; i<10; ++i) Step();
#endif

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	if (!render) return;
	render->Render(scene);
	render->EndScene();
	render->Present();
	render->SetViewport(m_hWnd);
	render->ClearBuffer();
	render->device->SetRenderState(D3DRS_AMBIENT, 0x000000);
//	render->device->SetRenderState(D3DRS_AMBIENT, 0x808080);
	render->BeginScene();
}
/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestView クラスの診断

#ifdef _DEBUG
void CDynaClockTestView::AssertValid() const
{
	CView::AssertValid();
}

void CDynaClockTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynaClockTestDoc* CDynaClockTestView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynaClockTestDoc)));
	return (CDynaClockTestDoc*)m_pDocument;
}
#endif //_DEBUG

/*	試しにあとからメッシュをロードするコードを書いてみた．
	DocSceneでやった方が高速なので，意味はないけど，シーングラフが
	できてからロードすることもできるようにしたかった．
void LoadMesh(SGFrame* frame, FIDocSceneD3D* arg){
	UTRef<D3DocNode> doc = UTRef<D3DocNode>(frame->doc);
	if (!doc) return ;
	for(FIDocNodes::iterator it = doc->Children().begin(); it != doc->Children().end(); ++it){
		UTRef<D3DocNode> doc = *it;
		if (doc && doc->GetType().compare("Mesh") == 0){
			FIDocSceneD3D* ctx = (FIDocSceneD3D*)arg;
		
			UTRef<SGVisualD3DMesh> mesh = ctx->CreateMesh();
			frame->contents.push_back(mesh);
			WXINTF(D3DXBuffer) adjacency, xbufMaterials;
			DWORD nMaterials=0;
			WXCHECK(D3DXLoadMeshFromXof(doc->xfile, D3DXMESH_MANAGED, ctx->render->device,
				&adjacency.Intf(), &xbufMaterials.Intf(), &nMaterials, &mesh->intf.Intf()));
			D3DXMATERIAL* d3dxMaterials = NULL;
			if (nMaterials)	d3dxMaterials = (D3DXMATERIAL*)xbufMaterials->GetBufferPointer();
			mesh->materials.resize(nMaterials);
			mesh->textures.resize(nMaterials);
			for(unsigned int i=0; i<nMaterials; ++i){
				mesh->materials[i] = d3dxMaterials[i];
				mesh->materials[i].MatD3D.Ambient = mesh->materials[i].MatD3D.Diffuse;
				mesh->materials[i].MatD3D.Ambient.a= mesh->materials[i].MatD3D.Diffuse.a;
				if (d3dxMaterials[i].pTextureFilename) mesh->textures[i] = ctx->render->textureManager.Get(d3dxMaterials[i].pTextureFilename);
			}
			//法線がない場合法線を書き込む
			if(mesh->intf->GetFVF() != (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)){
				WXINTF(D3DXMesh) meshTmp;
				mesh->intf->CloneMeshFVF(mesh->intf->GetOptions(),D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
				ctx->render->device, &meshTmp.Intf());
				mesh->intf = meshTmp;
				D3DXComputeNormals(mesh->intf, (DWORD*)adjacency->GetBufferPointer());
			}
		}
	}
	frame->Traverse(LoadMesh, arg);
}
*/
static int nCreate;

void CDynaClockTestView::Load(std::string filename){
#ifdef SPEEDTEST
	stepCount = -1000;
#endif


	if (!scene){
		initialLoadFile = filename;
		return;
	}
	loadFileName = filename;
	scene->Clear();
	scene->GetRenderers().Add(render);

	D3FileDoc fileDoc;
	fileDoc.renderer = (D3Render*)render;
	fileDoc.Load(loadFileName);
	FILoadScene loader;
	REGISTER_LOADER(&loader, SceneGraph);
	REGISTER_LOADER(&loader, ImpD3D);
	REGISTER_LOADER(&loader, Collision);
	REGISTER_LOADER(&loader, Physics);
	REGISTER_LOADER(&loader, Graphics);
	loader.Load(scene, &fileDoc);
	GRCamera* cam = NULL;
	scene->GetRenderers().GetObjects().Find(cam);
	if (!cam) scene->GetRenderers().Set(new GRCamera);

//	scene->GetWorld()->Print(DSTR);
	nCreate = 0;
	height = 0;
	side = 0;	
}
void CDynaClockTestView::Save(std::string fn){
	if (!scene->GetWorld()) return;
	D3SaveScene saver;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	REGISTER_SAVER(&saver, SceneGraph);
	REGISTER_SAVER(&saver, ImpD3D);
	REGISTER_SAVER(&saver, Collision);
	REGISTER_SAVER(&saver, Physics);
	REGISTER_SAVER(&saver, Graphics);
	saver.Save(doc, scene);
	D3FileDoc fileDoc;
	for (int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(fn, doc->Child(i));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDynaClockTestView クラスのメッセージ ハンドラ
bool CDynaClockTestView::CreateBox(){
	SGFrame* frTop = (SGFrame*)scene->FindObject("frTop");
	D3Mesh* d3mesh = (D3Mesh*)scene->FindObject("meshBlock");
	PHPenaltyEngine* pens = NULL;
	scene->GetBehaviors().Find(pens);
	if (!frTop || !d3mesh || !pens) return false;
	SGFrame* fr = new SGFrame;
	frTop->AddChild(fr);
	assert(d3mesh);
	fr->contents.push_back((SGObject*)d3mesh);
	
	CDMesh* mesh = new CDMesh;
	fr->contents.push_back(mesh);
	int nVtxs = 0;
	d3mesh->doc->GetData(nVtxs, "nVertices");
	mesh->vertices.resize(nVtxs);
	d3mesh->doc->GetDataImp(&*mesh->vertices.begin(), sizeof(Vec3f)*nVtxs, "vertices");
	mesh->CreateTree();
	
	PHSolidContainer* solids;
	scene->GetBehaviors().Find(solids);
	PHSolid* solid = new PHSolid;
	solid->frame = fr;
	solid->SetPosition(Vec3f(0, 6, 0));
	solid->SetMass(1.0f);
	solid->SetInertia(Matrix3f::Unit());
	solid->SetVelocity(Vec3f());
	solid->SetAngularVelocity(Vec3f());
	solid->Step(0);
	solids->solids.push_back(solid);
	
	for(int i=0; i<solids->solids.size()-1; ++i){
		PHPenalty* pen = new PHPenalty;
		pen->Set(solid, solids->solids[i]);
		pen->CalcConvertedMass();
		pens->penalties.push_back(pen);
	}	
	PHPenalty* pen = new PHPenalty;
	PHSolid* soFloor = new PHSolid;
	soFloor->SetMass(FLT_MAX);
	soFloor->SetInertiaInv(Matrix3d::Zero());
	soFloor->frame = (SGFrame*) scene->FindObject("frFloor");
	pen->Set(solid, soFloor);
	pen->CalcConvertedMass();
	pens->penalties.push_back(pen);
	
	PHGravityEngine* ff;
	scene->GetBehaviors().Find(ff);
	ff->AddChildObject(solid, scene);
	return true;
}

bool CDynaClockTestView::CreateBoxAuto(){
	SGFrame* frTop = (SGFrame*)scene->FindObject("frTop");
	D3Mesh* d3mesh = (D3Mesh*)scene->FindObject("meshBlock");
	PHContactEngine* pens = NULL;
	scene->GetBehaviors().Find(pens);
	if (!frTop || !d3mesh || !pens) return false;
	SGFrame* fr = new SGFrame;
	frTop->AddChild(fr);
	assert(d3mesh);
	fr->contents.push_back(d3mesh);
	
	CDMesh* mesh = new CDMesh;
	fr->contents.push_back(mesh);
	int nVtxs = 0;
	d3mesh->doc->GetData(nVtxs, "nVertices");
	mesh->vertices.resize(nVtxs);
	d3mesh->doc->GetDataImp(&*mesh->vertices.begin(), sizeof(Vec3f)*nVtxs, "vertices");
	mesh->CreateTree();
	fr->CalcBBox();
	
	if (side < height){
		side++;
	}else{
		height++;
		side = 0;
	}
	Vec3f pos = Vec3f(1,0,0)*height + Vec3f(-0.5f,1,0)*side;	// * 1.2f;
	
	PHSolidContainer* solids = NULL;
	scene->GetBehaviors().Find(solids);
	PHSolid* solid = new PHSolid;
	solid->frame = fr;
	solid->SetPosition(pos);
	solid->SetMass(1.0f);
	solid->SetInertia(Matrix3f::Unit());
	solid->SetVelocity(Vec3f());
	solid->SetAngularVelocity(Vec3f());
	solid->Step(0);
	solids->AddChildObject(solid, scene);
	
	pens->AddChildObject(solid, scene);
	pens->Init(scene);

	PHGravityEngine* ff = NULL;
	scene->GetBehaviors().Find(ff);
	ff->AddChildObject(solid, scene);
	return true;
}

void CDynaClockTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	Affinef afCam = render->camera->data.view.inv();

	float df = 1.0f ; //移動変化量
	switch(nChar){
	//1ステップ実行
	case VK_SPACE: Step(); break;	
	case 'Q': afCam.Pos() = afCam.Pos() - afCam.Ex() * df; break;
	case 'W': afCam.Pos() = afCam.Pos() + afCam.Ex() * df; break;
	case 'Z': afCam.Pos() = afCam.Pos() - afCam.Ey() * df; break;
	case 'A': afCam.Pos() = afCam.Pos() + afCam.Ey() * df; break;
	case 'T': execFlag = !execFlag; break;	//	連続実行の再開
	//ズーム
	case VK_DOWN : afCam.Pos() = afCam.Pos() - afCam.Ez() * df; break;
	case VK_UP: afCam.Pos() = afCam.Pos() + afCam.Ez() * df; break;
	//シーン回転
	case VK_LEFT: afCam = Affinef::Rot((float)Rad(-5),'y') * afCam; break;
	case VK_RIGHT: afCam = Affinef::Rot((float)Rad(5),'y') * afCam; break;
	case 'S': afCam = Affinef::Rot((float)Rad(-5),'x') * afCam; break;
	case 'X': afCam = Affinef::Rot((float)Rad(5),'x') * afCam; break;
	//再読み込み
	case 'R': Load(loadFileName.c_str()); return;
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
	case 'B':	CreateBoxAuto(); break;
	case 'C':	count ++; break;
	}
	render->camera->data.view = afCam.inv();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDynaClockTestView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	return true;
}

void CDynaClockTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if (!render){
		render = new D3Render;
		render->device.Create(m_hWnd);
		scene = new SGScene;
	}
	if (initialLoadFile.length()){
		Load(initialLoadFile.c_str());
		initialLoadFile = "";
	}
}

void CDynaClockTestView::Step(){
//	if (count % 250 == 0) return;
//	count ++;
	ptimer.Start();
	scene->ClearForce(dt);
	scene->GenerateForce(dt);
	scene->Integrate(dt);
	ptimer.Stop();
#if 0	//	計算時間の計測
	DSTR << "total:\t" << ptimer.Clear();
	static CDCollisionEngine* ce;
	if (!ce) scene->GetBehaviors().Find(ce);
	if (ce){
		DSTR << "\tgjk:\t" << ce->timerGjk.Clear() << " n:" << ce->timerNormal.Clear();
		DSTR << "\tqh:\t" << ce->timerQhull.Clear();
	}
	static PHContactEngine* pe;
	if (!pe) scene->GetBehaviors().Find(pe);
	if (pe){
		DSTR << "\tr:\t" << pe->timerReflex.Clear();
		DSTR << "\tf:\t" << pe->timerFriction.Clear();
		DSTR << std::endl;
	}
#endif
#if 1	//	立方体の角運動量の計測
	static PHSolid* cube;
	if (!cube) cube = DCAST(PHSolid, scene->FindObject("soCube"));
	if (cube){
		Vec3f am = cube->GetRotation()*cube->GetInertia()*cube->GetRotation().inv() * cube->GetAngularVelocity();
		DSTR << am.Z() << std::endl;
		Vec3f ang = cube->GetOrientation().angle();
		ang -= ang * Vec3f(0,1,0) * Vec3f(0,1,0);
		cube->SetOrientation(Quaterniond::Rot(ang.norm(), ang.unit()));
	}
#endif
}

void CDynaClockTestView::OnIdle(){
#ifdef SPEEDTEST
	Invalidate();
#else
	if (execFlag) Step();
	static int sleep;	
	sleep++;
	if (sleep>10){
		Invalidate();
		sleep=0;
	}
#endif
}
