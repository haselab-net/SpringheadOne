// VirtualHumanView.cpp : CVirtualHumanView �N���X�̓���̒�`���s���܂��B
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
#include <Device/DRUsb20Sh4.h>
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
	ON_COMMAND(ID_ZMP_DRAW, OnZmpDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView �N���X�̍\�z/����

CVirtualHumanView::CVirtualHumanView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	dt = 0.0005f;
	appState = INVALID;
	bMinForce = true;
	bLoaded = false;

	// �}�E�X�̏�����
	mouse.Init();
	// �X�V�����ƃ}�E�X�̃X�P�[����ݒ�
	mouse.SetScale(0.002f, 0.008f, 0.040f, 0.008f);	// SetScale(�|�C���^�̕��i�C��]�C�J�����̕��i�C��])

	devman.RPool().Register(new DRUsb20Sh4(0));
	devman.RPool().Register(new DRUsb20Sh4(1));
	devman.RPool().Register(new DRUsb20Sh4(2));
	devman.RPool().Register(new DRUsb20Sh4(3));
	devman.Init();
	DSTR << devman;
	//	SPIDAR�̏������D�{���́C�n�[�h�E�G�A���� HISpidar4 �N���X��h��������ׂ�
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	0.265f	//	x�����̕ӂ̒���/2
	#define PY	0.265f	//	y�����̕ӂ̒���/2
	#define PZ	0.265f	//	z�����̕ӂ̒���/2
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
/*		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}*/
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};
	spidar[0].Init(devman, motorPos[0], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
	spidar[1].Init(devman, motorPos[1], 0.3776f,-2.924062107079e-5f, 0.5f, 20.0f);

	checkNum = 0;
	// ���x�����O�J�����̏�����
	if(user.bTUser) user.ConnectSLCamera();
}

CVirtualHumanView::~CVirtualHumanView()
{
}

BOOL CVirtualHumanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView �N���X�̕`��

void CVirtualHumanView::OnDraw(CDC* pDC){
	CVirtualHumanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
	if (!render) return;
	render->Render(scene);			//	�V�[���̕`��
    if(render->CanDraw()){			//	diff file�̕\���e�X�g
		if (render->camera) render->SetViewMatrix(render->camera->data.view);
		static Affinef afw;
		render->SetModelMatrix(afw);
		diff.Draw(render, human, time);
	}
	render->EndScene();				//	�`��I��
	render->Present();				//	�\��

	DrawDlg(); //�E�B���h�E��ς��ĕ`�悷��ꍇ�A���̊֐��̒��ɏ���

	render->SetViewport(m_hWnd);	//	Viewport�̐ݒ�
	render->ClearBuffer();			//	�o�b�t�@�N���A
	render->BeginScene();			//	�`��J�n
}

void CVirtualHumanView::Load(std::string filename){
	if (!scene){		//	������������ł��Ȃ��ꍇ�́C��Ń��[�h���邱�Ƃɂ��āC�t�@�C���������o���Ă����D
		loadFile=filename;
		return;
	}
	//  �����[�h�p�Ƀt�@�C�������o���Ă���
	loadFileName = filename;
	PHContactEngine* ce=NULL;
	scene->GetBehaviors().Find(ce);
	bool bDraw = true;
	if (ce) bDraw = ce->bDraw;

#ifdef USE_MMTIMER	//	�}���`���f�B�A�^�C�}�[�̒�~
	timer.Release();
	Sleep(100);
#endif
	GRCameraData oldCamData = render->camera->data;
	
	//	�V�[���̏�����
	scene->Clear();									//	�V�[���O���t�̃N���A
	scene->GetRenderers().Add(render);				//	�����_����ǉ�
	// ���[�_�[�̏�����
	FILoadScene loader;
	REGISTER_LOADER(&loader, SceneGraph);			//	�V�[���O���t�̃��[�_��o�^
	REGISTER_LOADER(&loader, ImpD3D);				//	D3D�̃m�[�h�̃��[�_��o�^
	REGISTER_LOADER(&loader, Collision);			//	�Փ˔���m�[�h�̃��[�_��o�^
	REGISTER_LOADER(&loader, Physics);				//	�����V�~�����[�^�̃��[�_��o�^
	REGISTER_LOADER(&loader, Graphics);				//	�O���t�B�b�N�X�̃��[�_��o�^
	//	�t�@�C������h�L�������g�I�u�W�F�N�g�����[�h
	D3FileDoc fileDoc;			//	�t�@�C�����h�L�������g�I�u�W�F�N�g�ɕϊ�����I�u�W�F�N�g
	fileDoc.Load(filename);		//	�t�@�C���̃��[�h
	//	�h�L�������g�I�u�W�F�N�g����V�[���O���t���쐬
	loader.Load(scene, &fileDoc);
	//	�J�����̏�����Ԃ��L�^
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
	
	//	�͊o�|�C���^���V�[���Ɛڑ�
	pointer = (PHSolid*)scene->FindObject("Pointer");
	if (!pointer) pointer = (PHSolid*)scene->FindObject("Pointer1");
	if (pointer){
		pointer->SetFramePosition(Vec3f());
		pointer->SetOrientation(Quaternionf());
		pointer->SetAngularVelocity(Vec3f());
		pointer->SetVelocity(Vec3f());
		pointer->GetFrame()->SetPosture(Affinef());	
		// �}�E�X�f�o�C�X�̐ݒ�
		mouse.SetMass(pointer->GetMass());
		mouse.SetPos(pointer->GetFramePosition());
		mouse.SetOri(pointer->GetOrientation());
	}
	mouse.SetAxis(render->camera->data.view);
	mouse.SetAxisInit(render->camera->data.view);


	// VH�̃Z�b�g
	SetVH();

	DSTR << "Time step:" << scene->GetTimeStep() << std::endl;
	waitCount = 0;

#ifdef USE_MMTIMER	//	�}���`���f�B�A�^�C�}�[�̋N��
	timer.Create();
#endif

}

void CVirtualHumanView::Save(const char* filename){
	if (!scene->GetWorld()) return;
	D3SaveScene saver;
	UTRef<D3DocNode> doc = new D3DocNode("ROOT");
	REGISTER_SAVER(&saver, SceneGraph);				//	�V�[���O���t�̃��[�_��o�^
	REGISTER_SAVER(&saver, ImpD3D);					//	D3D�̃m�[�h�̃��[�_��o�^
	REGISTER_SAVER(&saver, Collision);				//	�Փ˔���m�[�h�̃��[�_��o�^
	REGISTER_SAVER(&saver, Physics);				//	�����V�~�����[�^�̃��[�_��o�^
	REGISTER_SAVER(&saver, Graphics);				//	�O���t�B�b�N�X�̃��[�_��o�^
	saver.Save(doc, scene);
	D3FileDoc fileDoc;
	for (unsigned int i=0; i<doc->NChildren(); ++i){
		fileDoc.Save(filename, doc->Child(i));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CVirtualHumanView �N���X�̃��b�Z�[�W �n���h��
void CVirtualHumanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	Invalidate();
#ifdef USE_MMTIMER
	if ( nChar == 'K' ) timer.Release();					//	�A�����s�̒�~
	if ( nChar == 'T' )	timer.Create();						//	�A�����s�̍ĊJ
	if ( nChar == 'H' )	timer.Thread();						//	�A�����s�̍ĊJ
#else
	if ( nChar == 'K' ) KillTimer(simTimerID);				//	�A�����s�̒�~
	if ( nChar == 'T' )	simTimerID = SetTimer(1, 10, NULL);	//	�A�����s�̍ĊJ
#endif
	if ( nChar == VK_SPACE ) bMinForce = !bMinForce;		//	�͊o��ON/OFF
	if ( nChar == 'D' ){									//	�ڐG�͕`���ON/OFF
		PHContactEngine* ce=NULL;
		scene->GetBehaviors().Find(ce);
		if (ce) ce->bDraw = !ce->bDraw;
	}
	if ( nChar == 'R' ){									// �t�@�C�������[�h
		Load(loadFileName.c_str());
		return;
	}

	if ( nChar == 'N' ){									//	�X�V�����̕ύX
		if (dt > 0.01f) dt -= 0.01f;
		else dt -= 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	if ( nChar == 'M' ){									//	�X�V�����̕ύX
		if (dt >= 0.01f) dt += 0.01f;
		else dt += 0.001f;
		DSTR << "dt:" << dt << std::endl;
	}
	if (nChar == 'F'){										//	�t���X�N���[��
		render->EndScene();
		render->Fullscreen();
	}
	if (nChar == 'G'){										//	�E�B���h�E���[�h
		render->EndScene();
		render->Window();
		render->BeginScene();
	}
	//	���_�ړ��֌W
	Affinef afBody = mouse.GetAxis().inv();
	float df = 1.0f ; //�ړ��ω���
	if ( nChar == VK_SPACE ) Step();						//	1�X�e�b�v���s
	// �J�����̕��s�ړ�
	if ( nChar == 'W' ) afBody.Pos() = afBody.Pos() + afBody.Ex() * df;
	if ( nChar == 'Q' ) afBody.Pos() = afBody.Pos() - afBody.Ex() * df;
	if ( nChar == 'A' ) afBody.Pos() = afBody.Pos() + afBody.Ey() * df;
	if ( nChar == 'Z' ) afBody.Pos() = afBody.Pos() - afBody.Ey() * df;
	if ( nChar == VK_DOWN ) afBody.Pos() = afBody.Pos() + afBody.Ez() * df;
	if ( nChar == VK_UP ) afBody.Pos() = afBody.Pos() - afBody.Ez() * df;
	// �J�����̉�]
	if ( nChar == VK_LEFT ) afBody = Affinef::Rot(Radf(5),'y') * afBody;
	if ( nChar == VK_RIGHT ) afBody = Affinef::Rot(Radf(-5),'y') * afBody;
	if ( nChar == 'X' ) afBody = Affinef::Rot(Radf(5),afBody.Ex()) * afBody;
	if ( nChar == 'S' ) afBody = Affinef::Rot(Radf(-5),afBody.Ex()) * afBody;
	if ( nChar == 'C' ){									// �|�C���^�ƃJ���������Z�b�g
		afBody = initialCamera.view.inv();
		if (pointer){
			pointer->SetFramePosition(Vec3f());
			pointer->SetOrientation(Quaternionf());
			pointer->SetAngularVelocity(Vec3f());
			pointer->SetVelocity(Vec3f());
			pointer->GetFrame()->SetPosture(Affinef());	
		}
		// Spidar�̃L�����u���[�V���� 'C'
		for(int i = 0; i < 2; ++i)	spidar[i].Calib();
	}

	// SceneGraph�F ��ԕۑ� 'P', ��ԓǂݍ��� 'O', �V�~�����[�^�X�V 'I'
	predictor.OnKeyDown(nChar, scene);
	if ( nChar == 'I' ){
		//predictor.MovementPrediction(phHuman, phUser, scene, dt);

		predictor.SaveState(scene);
		scene->SetTimeStep(dt*5);
		for(int i = 0; i < 10; i++){
			predictor.Step(phHuman, phUser, scene);

			if(render){
				render->Render(scene);			//	�V�[���̕`��
				render->EndScene();				//	�`��I��
				render->Present();				//	�\��

				DrawDlg(); //�E�B���h�E��ς��ĕ`�悷��ꍇ�A���̊֐��̒��ɏ���

				render->SetViewport(m_hWnd);	//	Viewport�̐ݒ�
				render->ClearBuffer();			//	�o�b�t�@�N���A
				render->BeginScene();			//	�`��J�n
			}
		}
		scene->SetTimeStep(dt);
		predictor.LoadState(scene);
	}
	
	// �X�e���I���x�����O�J�����ؒf�E�Đڑ� 'L'
	user.OnKeyDown(nChar);

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
//		render = new GLRenderWgl;
		render = new D3Render;
		render->Create(m_hWnd);
		scene = new SGScene();
		scene->GetRenderers().Add(render);
		mouse.SetAxis(render->camera->data.view);
		//collision = new CDCollisionEngine();
		//scene->GetBehaviors().Find(collision);

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
	
	if (loadFile.length()){	//	���[�h�����������t�@�C��������΂����Ń��[�h
		Load(loadFile);
		loadFile = "";
	}
}
void CVirtualHumanView::UpdateMouse(){
	if (mouse.IsGood()){
//		mouse.Update();		// ������Ă���{�^���̏�Ԃ���ʒu�E�p�����X�V

		// �J�������}�E�X�̊���W���ɍX�V
		render->camera->data.view = mouse.GetAxis();
		
		//	�|�C���^�̈ʒu�Ǝp����mouse����擾
		Vec3f pos = mouse.GetPos();
		Quaternionf ori = mouse.GetOri();

		if (pointer){
			//	�͂ƃg���N���V�~�����[�^�̃m�[�h(pointer)����擾
			Vec3f force, torque;
			force = pointer->GetForce() ;
			torque = pointer->GetTorque() ;
			
			//DSTR << "force = " << force << std::endl;

			//	�񎦗͂̐ݒ�
			if (bMinForce) mouse.SetMinForce();
			else mouse.SetForce(force, torque);
			
			//	solid�Ɉʒu�ƌ�����ݒ�D
			pointer->SetFramePosition(pos*15.0f);
			pointer->SetOrientation(ori);
			//	���x��0�ɂ��Ă���D(�{���̓}�E�X�̑��x����ꂽ�����ǂ�)
			pointer->SetAngularVelocity(Vec3f());
			pointer->SetVelocity(Vec3f());
			//	�V�~�����[�^�m�[�h�̈ʒu���X�V
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
	for(int i = 0; i < 2; ++i){
		spidar[i].Update(0.033f);
	}
	//DSTR << spidar[1].Motor()[2].GetLength() << std::endl;
	scene->ClearForce();

	//PHhuman�p
	if(phHuman.IsLoaded()){
		//VH��ZMP�A�ڐG�i����j�̈�̌v�Z�A�`��A�`�F�b�N
		balance.CheckBalance(render,&phHuman,dt);


		//�e�֐߂̊p���x���t�@�C���ɏ����o���B
		DataOut();
	}
	
	if(phUser.IsLoaded() && user.bTUser){
		user.InputUserPosition(phUser, spidar, dt, render);
		if(bMinForce) for(int i = 0; i < 2; ++i) spidar[i].SetMinForce();
		if(phHuman.IsLoaded()) predictor.ContactTest(phHuman, phUser, scene);
	}

	//THuman�p
	if (human.IsLoaded()){
		//VH�̃��f���̐���
		//VHControl();
		//VH��ZMP�A�ڐG�i����j�̈�̌v�Z�A�`��A�`�F�b�N
		//balance.CheckBalance(render,human,dt);
		balance.CalcZmpTest(human,dt);
		balance.DrawZmp(&phHuman,render);

		//VHControl���g����Ƃ������g�p
		if(appState == MAKE_FORCE && time > 0){
			human.SaveForce(time, scene->GetTimeStep());
		}
	}
	
	if(waitCount++ > 300){
		scene->GenerateForce();
	}

	scene->Integrate();

	UpdateMouse();
	stepCount ++;
	time += dt;

	// �t�@�C���ǂݍ���
	if(bLoaded){
		Load(loadFileName.c_str());
		bLoaded = false;
	}
}


//�_�C�A���O��\���{�^������������
void CVirtualHumanView::OnZmpDraw() 
{
	checkDialog.Create(IDD_VIEW_ZMP);
	checkDialog.ShowWindow(SW_SHOWNORMAL);
	
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
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	timer.Release();
}

////////////////////////////////////////////////////////////////////////////
/////////////         �}�E�X�̃C�x���g             /////////////////////////
////////////////////////////////////////////////////////////////////////////

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
// CVirtualHumanView �N���X�̐f�f

#ifdef _DEBUG
void CVirtualHumanView::AssertValid() const
{
	CView::AssertValid();
}

void CVirtualHumanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVirtualHumanDoc* CVirtualHumanView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVirtualHumanDoc)));
	return (CVirtualHumanDoc*)m_pDocument;
}

#endif //_DEBUG


////////////////////////////////////////////////////////////////////////////
/////////////			�V�K�쐬�u�g�֐��@�@�@�@�@//////////////////////////
////////////////////////////////////////////////////////////////////////////

void CVirtualHumanView::VHControl(){

//appState�ɂ���Đ�����@�A�Ώۂ�ς���B

	// Diff�t�@�C���Đ����e�֐ߊp�x��ۑ�
	if (appState == MAKE_ANGLE){
		AppStateMakeAngle();
	}
	// �ۑ����ꂽ�e�֐ߊp�x�̍Đ������̕␳���e�֐߂̃g���N�̕ۑ�
	else if (appState == MAKE_FORCE){
		AppStateMakeForce();
	}
	// �e�֐߃g���N�̍Đ�
	else if(appState == SET_TORQUE){
		AppStateSetTorque();
	}
	// ����Ȃ�
	else if(appState == NO_CONTROL){
	}

}
void CVirtualHumanView::AppStateMakeAngle(){
		if (waitCount++ < 300) time = 0;
		bool rv = diff.AddSpringForce(human, time, dt);
		if (time > 0){
			human.SaveJointAngle();
		}
		if (!rv){	//	�Ō�̃f�[�^�������ꍇ�C������x���[�h����appState��i�߂�D
			bLoaded = true;
			waitCount = 0;
		}
}
void CVirtualHumanView::AppStateMakeForce(){
	if (waitCount < 500){
		time = 0.0f;
		diff.AddSpringForce(human, time, dt);
		human.SetSpring(dt);
		human.LoadJointAngle(time, dt);
		waitCount++;
	}else if (waitCount < 2000){
		time = 0.0f;
		human.LoadJointAngle(time, dt);
		waitCount++;
	}else{
		bool rv = human.LoadJointAngle(time, dt);
		human.SaveTorque(time, dt);
		if (!rv){	//	�Ō�̃f�[�^�������ꍇ�C������x���[�h����appState��i�߂�D
			std::ofstream of("force.txt");
			for(float t=0.0005f; t<time; t+= 0.0005f){
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
			bLoaded = true;
			waitCount = 0;
		}
	}

}
void CVirtualHumanView::AppStateSetTorque(){
	if (waitCount < 500){
		time = 0.0f;
		diff.AddSpringForce(human, time, dt);
		human.SetSpring(dt);
		human.LoadJointAngle(time, dt);
		waitCount++;
	}else if (waitCount < 2000){
		time = 0.0f;
		human.LoadJointAngle(time, dt);
		waitCount++;
	}else{
		//�e�X�g:checkPart
		human.checkPart = true;
		if(human.checkPart)	human.LoadJointAngle(time, dt);
		bool rv = fFile.SetVHTorque(human,time,dt);
		human.NowAngle[0].push_back(human.joLElbow1->GetPosition()); //�e�X�g�p
		if (!rv){	//	�Ō�̃f�[�^�������ꍇ�C������x���[�h����appState��i�߂�D
			bLoaded = true;
			waitCount = 0;
			human.checkPart = false;
			//�e�X�g�p
			/*std::ofstream off2("NowAngle.txt");
			for(int c = 0; c < human.TargetAngle[0].size(); c++){
				off2 <<  human.NowAngle[0][c] << std::endl;
			}*/
		}
	}
}

void CVirtualHumanView::DrawDlg(){
	//�y�l�o�ƈ���̈���𑼂̃_�C�A���O�ɕ`��
	if(phHuman.IsLoaded()){
		checkDialog.DrawDlg(render,balance,&phHuman);
	}
}

void CVirtualHumanView::SetVH(){
	//PHHuman
	phHuman.Connect(scene);
	phHuman.SetVH(scene);
	//PHUser
	phUser.ConnectUser(scene);
	phUser.SetVH(scene);

	if(phHuman.IsLoaded()){
		//	�W���C���g�G���W���̍ď�����
		/*
		PHJointEngine* je;
		scene->GetBehaviors().Find(je);
		je->Loaded(scene);
		*/
		time = 0;

		//�ڐG�̈�v�Z�̂��߂�CollisionListener�̓o�^
		balance.collisionArea.ConnectBehaviorEngines(scene,render,phHuman);
		balance.Reload();
	}
	// ���[�U�̎p������pSpring �̐ݒ�
	if(phUser.IsLoaded() && user.bTUser) user.SetUserSprings(phUser);


//--------- THuman�i�e�X�g�j-------------//
	human.Connect(scene);

	if(human.IsLoaded()){
		//	diff�̃��[�h
		if (!diff.IsLoaded()){
			std::ifstream file("Test.DA");
			if (file.good()) diff.Load(file);
		}
		//	VH�̑傫�������킹��D
		diff.SetHumanScale(human);
		
		//	�W���C���g�G���W���̍ď�����
		PHJointEngine* je;
		scene->GetBehaviors().Find(je);
		je->Loaded(scene);
		
		// VH�̊֐߃o�l�_���p�ݒ�
		human.SetSpring(dt);
		time = 0;

		//�ڐG�̈�v�Z�̂��߂�CollisionListener�̓o�^
		balance.collisionArea.ConnectBehaviorEngines(scene,render,phHuman);
		
		//	�A�v���P�[�V�����̏��
		if (appState == INVALID){
			appState = MAKE_ANGLE;
			human.angles.clear();
		}else if (appState == MAKE_ANGLE){
			appState = MAKE_FORCE;
			fFile.jointTorque.clear();
		}else if (appState == MAKE_FORCE){
			appState = SET_TORQUE;
			//force.txt�̃��[�h
			if (!fFile.IsLoaded()){
				std::ifstream infile("force.txt");
				if (infile.good()) fFile.Load(infile);
			}
		}
	}else{
		appState = INVALID;
	}

}
void CVirtualHumanView::DataOut(){
	static std::ofstream off("AngleVelocity.xls");
	if(time == 0){
		for(int i = 0; i < phHuman.joints.size(); i++){
			off << phHuman.joints[i]->GetName() << "\t";
		}
		off << std::endl;
	}

	for(int i = 0; i < phHuman.joints.size(); i++){
		off <<  phHuman.joints[i]->GetVelocity() << "\t";
	}
	off << std::endl;
}

//���o�[�W�����iTHuman�j
//void CVirtualHumanView::SetVH(){
	//	Virtual Human ���V�[���Ɛڑ�
//	human.Connect(scene);

//	if(human.IsLoaded()){
/*		//	diff�̃��[�h
		if (!diff.IsLoaded()){
			std::ifstream file("Test.DA");
			if (file.good()) diff.Load(file);
		}
		//	VH�̑傫�������킹��D
		diff.SetHumanScale(human);
*/		
		//	�W���C���g�G���W���̍ď�����
	//	PHJointEngine* je;
	//	scene->GetBehaviors().Find(je);
	//	je->Loaded(scene);
		
		// VH�̊֐߃o�l�_���p�ݒ�
	//	human.SetSpring(dt);
	//	time = 0;

		//�ڐG�̈�v�Z�̂��߂�CollisionListener�̓o�^
	//	balance.collisionArea.ConnectBehaviorEngines(scene,render,&human);
	//}

	//�ȉ��͂��ꂩ��قƂ�ǂ���Ȃ��B
	/*if (human.IsLoaded()){
		//	diff�̃��[�h
		if (!diff.IsLoaded()){
			std::ifstream file("Test.DA");
			if (file.good()) diff.Load(file);
		}
		//	VH�̑傫�������킹��D
		diff.SetHumanScale(human);
		//	�W���C���g�G���W���̍ď�����
		PHJointEngine* je;
		scene->GetBehaviors().Find(je);
		je->Loaded(scene);
		//	�A�v���P�[�V�����̏��
		time = 0;
		if (appState == INVALID){
			appState = MAKE_ANGLE;
			human.angles.clear();
		}else if (appState == MAKE_ANGLE){
			appState = MAKE_FORCE;
			fFile.jointTorque.clear();
		}else if (appState == MAKE_FORCE){
			appState = SET_TORQUE;
			//force.txt�̃��[�h
			if (!fFile.IsLoaded()){
				std::ifstream infile("force.txt");
				if (infile.good()) fFile.Load(infile);
			}
		}
	}else{
		appState = INVALID;
	}
	*/
//}
