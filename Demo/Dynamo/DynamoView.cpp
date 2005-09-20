// DynaHapticView.cpp : CDynaHapticView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "Dynamo.h"

#include "DynamoDoc.h"
#include "DynamoView.h"
#include <ImpD3D/SGD3D.h>
#include <Collision/CDLoader.h>
#include <Physics/PHSolid.h>
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
// CDynaHapticView

IMPLEMENT_DYNCREATE(CDynaHapticView, CView)

BEGIN_MESSAGE_MAP(CDynaHapticView, CView)
	//{{AFX_MSG_MAP(CDynaHapticView)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView �N���X�̍\�z/����

CDynaHapticView::CDynaHapticView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	bMinForce = true;
	dt = 0.01f;
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();
	DSTR << devMan;
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
//	#define PX	(0.390f/2)		//	x�����̕ӂ̒���/2
//	#define PY	(0.395f/2)		//	y�����̕ӂ̒���/2
//	#define PZ	(0.405f/2)		//	z�����̕ӂ̒���/2
	#define PX	(0.250f/2)		//	x�����̕ӂ̒���/2�@������ς��Ă��ω��Ȃ��H�H
	#define PY	(0.250f/2)		//	y�����̕ӂ̒���/2
	#define PZ	(0.250f/2)		//	z�����̕ӂ̒���/2
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f( PX, PY,-PZ),	Vec3f(-PX, PY, PZ), Vec3f( PX,-PY, PZ), Vec3f(-PX,-PY,-PZ)},
		{Vec3f(-PX, PY,-PZ),	Vec3f( PX, PY, PZ), Vec3f(-PX,-PY, PZ), Vec3f( PX,-PY,-PZ)}
	};
/*
	#define GX	(0.065f/2)		//	x�����̕ӂ̒���/2
	#define GY	(0.065f/2)		//	y�����̕ӂ̒���/2
	Vec3f motorPos[8][2] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f( PX, PY,-PZ), Vec3f( GX,  0, 0)},
		{Vec3f(-PX, PY, PZ), Vec3f(  0, GY, 0)},
		{Vec3f( PX,-PY, PZ), Vec3f(  0,-GY, 0)},
		{Vec3f(-PX,-PY,-PZ), Vec3f(-GX,  0, 0)},
		{Vec3f(-PX, PY,-PZ), Vec3f(-GX,  0, 0)},
		{Vec3f( PX, PY, PZ), Vec3f(  0, GY, 0)},
		{Vec3f(-PX,-PY, PZ), Vec3f(  0,-GY, 0)},
		{Vec3f( PX,-PY,-PZ), Vec3f( GX,  0, 0)}
	};
*/
	spidar.Init(devMan, motorPos[0], 0.3f, 2.924062107079e-5f, 0.5f, 10.0f);
	spidar2.Init(devMan, motorPos[1], 0.3f, -2.924062107079e-5f, 0.5f, 10.0f);
//	for(int i=4; i<8; ++i){
//		spidar.motor[i].lengthPerPulse *= -1;
//	}
	spidar.Calib();
}

CDynaHapticView::~CDynaHapticView()
{
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
	static DWORD lastTime;
	static int lastStep;
	DWORD nowTime = GetTickCount();
	DWORD dwPeriod = (nowTime - lastTime);
	if (stepCount!=lastStep && dwPeriod){
		float period = dwPeriod / 1000.0f;
		lastTime = nowTime;
		int step = stepCount - lastStep;
		lastStep = stepCount;
		float dtMeasure = period / step;
//		DSTR << "Period: " << period << " dt: " << dtMeasure << " = " << 1.0f/dtMeasure << "Hz" <<  std::endl;
	}else{
		return;
	}

	CDynaHapticDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
	if (!render) return;

	static bool bDraw = false;

	if (bDraw){
		render->Render(scene->world, camera);
		render->EndScene();
		render->Present();
	}

	RECT rc;
	GetClientRect(&rc);
	bDraw = !render->Resize( Vec2f(rc.right, rc.bottom), m_hWnd, camera );
	render->Clear();
	render->BeginScene();
	render->device->SetRenderState(D3DRS_AMBIENT, 0x000000);
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

/*	�����ɂ��Ƃ��烁�b�V�������[�h����R�[�h�������Ă݂��D
	DocScene�ł�������������Ȃ̂ŁC�Ӗ��͂Ȃ����ǁC�V�[���O���t��
	�ł��Ă��烍�[�h���邱�Ƃ��ł���悤�ɂ����������D
void LoadMesh(SGFrame* frame, FIDocSceneD3D* arg){
	UTRef<D3DocNode> doc = UTRef<D3DocNode>(frame->doc);
	if (!doc) return ;
	for(FIDocNodes::iterator it = doc->Children().begin(); it != doc->Children().end(); ++it){
		UTRef<D3DocNode> doc = *it;
		if (doc && doc->GetType().compare("Mesh") == 0){
			FIDocSceneD3D* ctx = (FIDocSceneD3D*)arg;
		
			UTRef<SGVisualD3DMesh> mesh = ctx->CreateMesh();
			frame->visuals.push_back(mesh);
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
			//�@�����Ȃ��ꍇ�@������������
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

	void hoge1_func(int a)
{//case�ŏ���������������������
	;
	}

void hoge2_func(int a)
{//����
;
}

void CDynaHapticView::Load(const char* filename){
#ifdef USE_MMTIMER
	timer.Release();
#endif
	scene->Clear();
	//	std::ofstream file("SceneGraph.txt"); 	
	scene->cameras.clear();

	D3FileDoc fileDoc;
	fileDoc.render = (D3Render*)render;
	UTRef<FIDocNode> doc = fileDoc.Load(filename);
//	doc->Print(DSTR);
	FILoadScene loader;
	RegisterD3DLoader(&loader);
	RegisterXCDLoader(&loader);
	RegisterPHLoader(&loader);
	loader.Load(scene, doc);

	//�t�@�C�����`�F�b�N����
    char *pdest;
    int  result;
	char poem[] =    "poem.x";
	char venus[] =    "venus.x";

	RECT fullsc;
	fullsc.right = 1024;
	fullsc.bottom = 768;

	pdest = strstr( filename, poem );
	result = pdest - filename + 1;
	if( pdest != NULL ) {
		//poem
		//FullScreen
		render->EndScene();
		render->device.Fullscreen();
		render->Setup( Vec2f(fullsc.right, fullsc.bottom), camera );
		render->device->ShowCursor(false);
		render->BeginScene();
		
	} else {
		pdest = strstr( filename, venus );
		result = pdest - filename + 1;
		if( pdest != NULL ) {
			//venus
			//FullScreen
			render->EndScene();
			render->device.Fullscreen();
			render->Setup( Vec2f(fullsc.right, fullsc.bottom), camera );
			render->device->ShowCursor(false);
			render->BeginScene();
		}
	}



	//*************
	//*****************
	camera->data.InitData();
/* �f�t�H���g�J����
	void SGCameraData::InitData(){
	front = 0.4f;
	back = 4000.0f;
	size = Vec2f(front, front);
	center = Vec2f(0, 0);
	view.Pos() = Vec3f(0, 6, -12);
    view.LookAt(Vec3f(), Vec3f(0,1,0));
	view = view.inv();
}
		afBody = Affinef::Trn(0,0,-20);
		afBody.LookAt(Vec3f());
		afHead = Affinef();
*/
	camera->data.view.Pos() = Vec3f(0, 20, -1);
	camera->data.view.LookAt(Vec3f(), Vec3f(0,1,0));
	camera->data.view = camera->data.view.inv();

	/**/
	if (scene->cameras.size() == 0) {
		scene->cameras.push_back(camera);
	} else {
		camera = scene->cameras.back();
	}

	
	afBody = camera->data.view.inv();
	afHead = Affinef();


	//*************
	scene->world->Print(DSTR);

	pointer = (PHSolid*)scene->FindObject("Pointer");
	pointer2 = (PHSolid*)scene->FindObject("Pointer2");

#ifdef USE_MMTIMER
	timer.Create();
#endif
}
void CDynaHapticView::Save(const char* filename){
	if (!scene->world) return;
	//**************
	scene->cameras.back() = camera;
	//**************
	D3SaveScene saver;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	RegisterD3DSaver(&saver);
	RegisterPHSaver(&saver);
	saver.Save(doc, scene);
	D3FileDoc fileDoc;
	for (unsigned int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(filename, doc->Child(i));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDynaHapticView �N���X�̃��b�Z�[�W �n���h��
void CDynaHapticView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	float df = 1.0f ; //�ړ��ω���
	if ( nChar == VK_SPACE ) Step();						//	1�X�e�b�v���s
	if ( nChar == 'K' ) timer.Release();					//	�A�����s�̒�~
	if ( nChar == 'T' )	timer.Create();						//	�A�����s�̍ĊJ
	if ( nChar == 'H' )	timer.Thread();						//	�A�����s�̍ĊJ
	if ( nChar == 'Q' ) afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	if ( nChar == 'W' ) afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	if ( nChar == 'Z' ) afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	if ( nChar == 'A' ) afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	if ( nChar == VK_DOWN ) afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	if ( nChar == VK_UP ) afBody.Pos() = afBody.Pos() + afBody.Ez() * df;
	if ( nChar == VK_LEFT ) afBody = afBody * Affinef::Rot(Rad(-5),'y');
	if ( nChar == VK_RIGHT ) afBody = afBody * Affinef::Rot(Rad(5),'y');
	if ( nChar == VK_SPACE ){
		bMinForce = !bMinForce;
	}
	if ( nChar == 'S' ) afHead = Affinef::Rot(Rad(-5),'x') * afHead;
	if ( nChar == 'X' ) afHead = Affinef::Rot(Rad(5),'x') * afHead;
	if ( nChar == 'R' ){
		afBody = Affinef::Trn(0,20,-2);
		afBody.LookAt(Vec3f());
		afHead = Affinef();
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
		render->Setup( Vec2f(1024,768), camera );
//		render->Setup( Vec2f(rc.right, rc.bottom), camera );
		render->device->ShowCursor(false);
		render->BeginScene();
	}
	if (nChar == 'G'){
		render->EndScene();
		render->device.Window();

		RECT rc;
		GetClientRect(&rc);
		render->Setup( Vec2f(rc.right, rc.bottom), camera );
		render->device->ShowCursor(true);
		render->BeginScene();
	}
	if ( nChar == 'C' ){
		spidar.Calib();
		spidar2.Calib();
	}
	camera->data.view = (afBody*afHead).inv();
//	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDynaHapticView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	return true;
}

void TimerFunc(void* arg){
	CDynaHapticView* view = (CDynaHapticView*) arg;
	view->Step();
}
void CDynaHapticView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	Spr::CDFramePair::InitCommonPointMap();

	GetDocument()->view = this;

	if (!render){
		render = new D3Render;
		render->device.Create(m_hWnd);
		scene = new SGScene(render);
		if (!camera) camera = new SGCamera;
//		scene->cameras.push_back(camera);
	}
	timer.Resolution(5);
	timer.Interval(5);
#ifdef USE_MMTIMER
	timer.Set(TimerFunc, this);
	timer.Thread();
#else
	wmTimerID = SetTimer(1, 5, NULL);
#endif
}

void CDynaHapticView::Step(){
	stepCount ++;
	scene->Step(dt);
	if (spidar.IsGood() && pointer){
		spidar.Update();
//		Vec3f pos = spidar.GetPos();
		Vec3f pos = spidar.GetPos()*2.0f;
		static Vec3f lastPos;
		Vec3f force, torque;
		force = pointer->GetForce() * 0.15f;
		if (bMinForce) spidar.SetMinForce();
		else spidar.SetForce(force);

		pointer->SetPosition(pos*15.0f);
		pointer->SetOrientation(Quaternionf());
		pointer->SetAngularVelocity(Vec3f());
		pointer->SetVelocity( Vec3f() );

		Affinef posture;
		Matrix3d rot;
		pointer->GetOrientation().to_matrix(rot);
		posture.Rot() = rot;
		posture.Pos() = pointer->GetPosition();
		pointer->frame->SetPosture(posture);
		lastPos = pos;
	}
	if (spidar2.IsGood() && pointer2){
		spidar2.Update();
		Vec3f pos = spidar2.GetPos();
		static Vec3f lastPos;
		Vec3f force, torque;
		force = pointer2->GetForce() * 0.2f;
		if (bMinForce) spidar2.SetMinForce();
		else spidar2.SetForce(force);

		pointer2->SetPosition(pos*15.0f);
		pointer2->SetOrientation(Quaternionf());
		pointer2->SetAngularVelocity(Vec3f());
		pointer2->SetVelocity( Vec3f() );

		Affinef posture;
		Matrix3d rot;
		pointer2->GetOrientation().to_matrix(rot);
		posture.Rot() = rot;
		posture.Pos() = pointer2->GetPosition();
		pointer2->frame->SetPosture(posture);
		lastPos = pos;
	}
#ifndef USE_MMTIMER
	Invalidate();
#endif
}

void CDynaHapticView::OnTimer(UINT nIDEvent){
	Step();
}


void CDynaHapticView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	timer.Release();
}

void CDynaHapticView::OnIdle()
{
	Invalidate();
}
