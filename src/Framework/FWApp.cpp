#include "Framework.h"
#include "GraphicsGL/GLRender.h"
#include "ImpD3D/D3Render.h"
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include <Device/DRTokin3D.h>
#include <HIS/HISpidar4.h>
#include <HIS/HISpidar4D.h>
#include <HIS/HISpidarG6C.h>
#include <HIS/HISpidarG6O.h>
#include <HIS/HISpidarG6X.h>
#include <HIS/HISpidarG6X2.h>
#include <HIS/HISpidarG6X3.h>
#include <HIS/HISpidarCanoe.h>
#include <HIS/HISpidarG6WB.h>
#include <HIS/HISpidarG6Jun.h>
#include <HIS/HIRuiKuma.h>
#include <HIS/HIOrientation.h>
#include <Physics/PHContactEngine.h>
#include <strstream>
#include <fstream>
#include <WinBasis/WBPath.h>
#pragma hdrstop


namespace Spr{;
FWApp::FWApp(){
	hWnd = NULL;
	bOutForce = false;
	bBusy = false;
	bPause = false;
	defaultTimeStep = 0.005f;
	InitDevice();
	simuTimerPeriod = 0.033f;
}
FWApp::~FWApp(){
	timer.Release();
	pointers.clear();
}
void FWApp::InitDevice(){
	HIHapticDevice::ClearRealDeviceDependency();
	mouse = new HIMouse;
	devMan.Clear();
	devMan.RPool().Register(new DRUsb20Sh4(0));
	devMan.RPool().Register(new DRUsb20Sh4(1));
	devMan.RPool().Register(new DRUsb20Sh4(2));
	devMan.RPool().Register(new DRUsb20Sh4(3));
	devMan.RPool().Register(new DRUsb20Sh4(4));
	devMan.RPool().Register(new DRUsb20Sh4(5));
	devMan.RPool().Register(new DRUsb20Simple(10));
	devMan.RPool().Register(new DRUsb20Simple(11));
	devMan.RPool().Register(new DRTokin3D);
	devMan.Init();
	DSTR << devMan;
}

///	�}���`���f�B�A�^�C�}�[�ɌĂяo�����֐�
void FWApp::TimerFunc(void* arg){
	FWApp* app = (FWApp*) arg;
	app->Step();
}

FWApp* app;
void CALLBACK FWApp::WMTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if (idEvent == FWApp::TIMER_SIMU){
		int times = 1;
		if (app->scene){
			times = int(app->GetSimuTimerPeriod() / app->scene->GetTimeStep());
		}
		for(int i=0; i<times; ++i) app->Step();
	}
}
void FWApp::Init(HWND hw){
	if (hWnd == hw) return;
	hWnd = hw;
	if (!scene){
		scene = new SGScene;
		CreateD3DRender();
	}
	//	�V�~�����[�V�����p�̕��ʂ̃^�C�}�[�̐ݒ�
	simuTimerId = SetTimer(hWnd, TIMER_SIMU, (UINT)(simuTimerPeriod*1000), (TIMERPROC)WMTimerProc);
	app = this;
	//	�V�~�����[�V�����p�̃}���`���f�B�A�^�C�}�[�̐ݒ�
	timer.Set(TimerFunc, this);
}


#define IF_SPIDAR(t)										\
		if (t::GetTypeInfoStatic()->Inherit(type)){			\
			UTRef<t> dev = new t;							\
			if (dev->Init(devMan)){							\
				pointers.push_back(new FWPointer6D(dev));	\
				rv = true;									\
			}												\
		}

bool FWApp::AddHis(const char* str){
	bBusy = true;
	cs.Enter();
	bool rv = true;
	std::istrstream is(str, strlen(str));
	char type[100]="HI";
	is >> type+2;
	if (HIRuiKuma::GetTypeInfoStatic()->Inherit(type)){
		UTRef<HIRuiKuma> dev = new HIRuiKuma;
		if (dev->Init()){
			pointers.push_back(new FWPointerRui(&*dev));
			rv = true;
		}
	}else if (HIOrientation::GetTypeInfoStatic()->Inherit(type)){
		UTRef<HIOrientation> ori = new HIOrientation;
		if (ori->Init(devMan)){
			pointers.push_back(new FWPointer6D(ori));
			rv = true;
		}
	}else if (HIMouse::GetTypeInfoStatic()->Inherit(type)){
		if (!mouse->IsGood()){
			mouse->Init();
			pointers.push_back(new FWPointer6D(mouse));
			rv = true;
		}
	}else if (HISpidar4::GetTypeInfoStatic()->Inherit(type)){
		//	Spidar4�̏����l
		#define PX	0.265f	//JA	x�����̕ӂ̒���/2	EN length of the x side of the frame /2
		#define PY	0.265f	//JA	y�����̕ӂ̒���/2	EN length of the y side of the frame /2
		#define PZ	0.265f	//JA	z�����̕ӂ̒���/2	EN length of the z side of the frame /2
		Vec3f motorPos[4] = {	//JA	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
								//EN	Motor's position (origin is calibration position)
			Vec3f(PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f(PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)
		};
		Vec3f frame(PX, PY, PZ);
		float minForce = 0.5f;
		float maxForce = 20.0f;
		float vpn = 0.3776f;
		float lpp = 2.924062107079e-5f;
		float sign[4]={1.0f, 1.0f, 1.0f, 1.0f};
		while(is.good()){
			char argType[100];
			is >> argType;
			if (stricmp(argType, "frame")==0){
				is >> frame.X() >> frame.Y() >> frame.Z();
			}else if (stricmp(argType, "motor")==0){
				for(int i=0; i<4; ++i){
					for(int j=0; j<3; ++j){
						char ch;
						is >> ch;
						if (ch == '-'){
							motorPos[i][j] = -frame[j]/2;
						}else{
							motorPos[i][j] = frame[j]/2;
						}
					}
				}
			}else if (stricmp(argType, "pos")==0){
				for(int i=0; i<4; ++i){
					for(int j=0; j<3; ++j){
						is >> motorPos[i][j];
					}
				}
			}else if (stricmp(argType, "sign")==0){
				for(int i=0; i<4; ++i){
					char ch;
					is >> ch;
					if (ch == '-') sign[i] *= -1;
				}
			}else if (stricmp(argType, "range")==0){
				is >> minForce;
				is >> maxForce;
			}else if (stricmp(argType, "vpn")==0){
				is >> vpn;
			}else if (stricmp(argType, "lpp")==0){
				is >> lpp;
			}else if (stricmp(argType, "reverse")==0){		//	�Ђ�����Ԃ�
				for(int i=0; i<4; ++i) motorPos[i].X()*=-1;
				lpp *= -1;
			}
		}
		UTRef<HISpidar4> spidar = new HISpidar4;
		if (spidar->Init(devMan, motorPos, vpn, lpp, minForce, maxForce)){
			for(int i=0; i<4; ++i) spidar->Motor()[i].lengthPerPulse *= sign[i];
			spidar->Calib();
			pointers.push_back(new FWPointer6D(spidar));
			rv = true;
		}
	}else if (HISpidar4D::GetTypeInfoStatic()->Inherit(type)){
		//	Spidar4�̏����l
		/*
		#define PX	0.265f	//JA	x�����̕ӂ̒���/2	EN length of the x side of the frame /2
		#define PY	0.265f	//JA	y�����̕ӂ̒���/2	EN length of the y side of the frame /2
		#define PZ	0.265f	//JA	z�����̕ӂ̒���/2	EN length of the z side of the frame /2
		*/
		#define PXD	0.382f	//JA	x�����̕ӂ̒���/2	EN length of the x side of the frame /2
		#define PYD	0.456f	//JA	y�����̕ӂ̒���/2	EN length of the y side of the frame /2
		#define PZD	0.500f	//JA	z�����̕ӂ̒���/2	EN length of the z side of the frame /2

		/*
		Vec3f motorPos[4] = {	//JA	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
								//EN	Motor's position (origin is calibration position)
			Vec3f(PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f(PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)
		};
		*/
		Vec3f motorPos[4] = {	//JA	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
								//EN	Motor's position (origin is calibration position)
			Vec3f( PXD, PYD, -PZD), Vec3f( PXD,-PYD, PZD), Vec3f(-PXD, -PYD, -PZD), Vec3f(-PXD, PYD, PZD)
//			Vec3f( -PXD, -PYD, PZD), Vec3f( -PXD, PYD, -PZD), Vec3f(PXD, PYD, PZD), Vec3f(PXD, -PYD, -PZD)
		};

		Vec3f frame(PXD, PYD, PZD);
		float minForce = 0.02f;
		float maxForce = 20.0f;
		float vpn = 0.577617328520f;	// ��t����
//		float vpn = 1.0f;
		float lpp = -2.924062107079e-5f;
//		float lpp = -2.513274122872e-5f; // 0.008mm���a�v�[���A500�p���X�G���R�[�_
		while(is.good()){
			char argType[100];
			is >> argType;
			if (stricmp(argType, "frame")==0){
				is >> frame.X() >> frame.Y() >> frame.Z();
			}else if (stricmp(argType, "motor")==0){
				for(int i=0; i<4; ++i){
					for(int j=0; j<3; ++j){
						char ch;
						is >> ch;
						if (ch == '-'){
							motorPos[i][j] = -frame[j]/2;
						}else{
							motorPos[i][j] = frame[j]/2;
						}
					}
				}
			}else if (stricmp(argType, "range")==0){
				is >> minForce;
				is >> maxForce;
			}else if (stricmp(argType, "vpn")==0){
				is >> vpn;
			}else if (stricmp(argType, "lpp")==0){
				is >> lpp;
			}else if (stricmp(argType, "reverse")==0){		//	�Ђ�����Ԃ�
				for(int i=0; i<4; ++i) motorPos[i].X()*=-1;
				for(int i=0; i<4; ++i) motorPos[i].Y()*=-1;
				for(int i=0; i<4; ++i) motorPos[i].Z()*=-1;
				lpp *= -1;
			}
		}
		UTRef<HISpidar4D> spidar = new HISpidar4D;
		if (spidar->Init(devMan, motorPos, vpn, lpp, minForce, maxForce)){
			pointers.push_back(new FWPointer6D(spidar));
			rv = true;
		}
	}
	else IF_SPIDAR(HISpidarG6C)
	else IF_SPIDAR(HISpidarG6O)
	else IF_SPIDAR(HISpidarG6X)
	else IF_SPIDAR(HISpidarG6X2)
	else if (strcmp("HISpidarG6X3R", type) == 0){
		UTRef<HISpidarG6X3> dev = new HISpidarG6X3;
		if (dev->Init(devMan, false)){
			pointers.push_back(new FWPointer6D(dev));
			rv = true;
		}
	}else if (strcmp("HISpidarG6X3L", type) == 0){
		UTRef<HISpidarG6X3> dev = new HISpidarG6X3;
		if (dev->Init(devMan, true)){
			pointers.push_back(new FWPointer6D(dev));
			rv = true;
		}
	}
	else IF_SPIDAR(HISpidarCanoe)
	else IF_SPIDAR(HISpidarG6WB)
	else IF_SPIDAR(HISpidarG6Jun)
	bBusy = false;
	cs.Leave();
	return rv;
}
void FWApp::ClearHis(){
	bBusy = true;
	cs.Enter();
	pointers.clear();
	InitDevice();
	cs.Leave();
	bBusy = false;
}
void FWApp::ConnectHis(){
	pointers.Connect(scene);

	FWPointer6D* pMouse = (FWPointer6D*) pointers.Find(mouse);
	if (pMouse && pMouse->GetSolid()){
		// �}�E�X�f�o�C�X�̐ݒ�
		mouse->SetMass((float)pMouse->GetSolid()->GetMass());
	}
	mouse->SetAxis(render->camera->data.view);
	mouse->SetAxisInit(render->camera->data.view);

}
bool FWApp::PreviewMessage(MSG* pMsg){
	bool rv = false;
	rv |= mouse->PreviewMessage(pMsg);
	if (mouse->IsGood()){
		for(FWPointers::iterator it = pointers.begin(); it != pointers.end(); ++it){
			if ((*it)->device == mouse){
				(*it)->Step((float) scene->GetTimeStep());
				break;
			}
		}
		// �J�������}�E�X�̊���W���ɍX�V
		if (render && render->camera && !render->camera->frPosture) render->camera->data.view = mouse->GetAxis();
	}
	if (pMsg->message == WM_KEYDOWN){
		int nVirtKey = (int) pMsg->wParam;
		long lKeyData = pMsg->lParam;
		rv |= OnKeyDown(nVirtKey);
	}
	return rv;
}

void FWApp::Step(){
	if (bBusy) return;
	cs.Enter();
	scene->Step();
	pointers.Step(scene->GetTimeStep());
	cs.Leave();
}

void FWApp::Reset(){
    Load(lastLoadFile);
    return;
}

bool FWApp::OnKeyDown(unsigned int nChar){
	//	�L�[����
	//	���_�ړ��� HIMouse ���s���D
	if (nChar == 'P'){		//	pause
		timer.Release();
		KillTimer(hWnd, simuTimerId);
		Step();
	}else if (nChar == 'T'){		//	message timer
		timer.Release();
		SetTimer(hWnd, TIMER_SIMU, UINT(simuTimerPeriod*1000), (TIMERPROC)WMTimerProc);
	}else if (nChar == 'M'){		//	multi media timer		
		if (scene){
			timer.Resolution(UINT(scene->GetTimeStep()*TIMERTICK + 0.5f));
			timer.Interval(UINT(scene->GetTimeStep()*TIMERTICK + 0.5f));
		}
		timer.Create();				//	�}���`���f�B�A�^�C�}�[
		KillTimer(hWnd, simuTimerId);
	}else if (nChar == 'H'){		//	�X���b�h
		timer.Thread();
		KillTimer(hWnd, simuTimerId);
	}else if (nChar == VK_RETURN){	//	���X�g���玟�̃t�@�C�������[�h
		if (fileList.size()){
			LoadList();
		}
	}else if ( nChar == 'R' ){		//	�����[�h
		Reset();
	}else if ( nChar == VK_SPACE ){
		bOutForce = !bOutForce;
		for(unsigned i=0; i<pointers.size(); ++i){
			pointers[i]->SetOutForce(bOutForce);
		}
	}else if ( nChar == 'C'){
		for(unsigned i=0; i<pointers.size(); ++i){
			if (pointers[i]->device){
				pointers[i]->device->Calib();
			}
		}
	}else if ( nChar == 'F' ){
		render->Fullscreen();
	}else if ( nChar == 'G' ){
		render->Window();
	}else if ( nChar == ',' ){
		if (scene->GetTimeStep() > 0.01) scene->SetTimeStep(scene->GetTimeStep() - 0.01);
		else scene->SetTimeStep(scene->GetTimeStep() - 0.001);
		DSTR << "dt:" << scene->GetTimeStep() << std::endl;
	}else if ( nChar == '.' ){
		if (scene->GetTimeStep() > 0.01) scene->SetTimeStep(scene->GetTimeStep() + 0.01);
		else scene->SetTimeStep(scene->GetTimeStep() + 0.001);
		DSTR << "dt:" << scene->GetTimeStep() << std::endl;
	}else if ( nChar == 'D' ){
		render->bDrawDebug = !render->bDrawDebug;
	}else{
		return false;
	}
	return true;
}
void FWApp::Draw(){
	if (!render) return;
	render->Render(scene);			//	�V�[���̕`��
	render->EndScene();				//	�`��I��
	render->Present();				//	�\��
	render->SetViewport(hWnd);	//	Viewport�̐ݒ�
	render->ClearBuffer();			//	�o�b�t�@�N���A
	render->BeginScene();			//	�`��J�n
}
void FWApp::CreateD3DRender(){
	if (render) render->EndScene();
	render = new D3Render;
	render->Create(hWnd);
	if (mouse && render && render->camera) mouse->SetAxis(render->camera->data.view);
	if (lastLoadFile.length()) Load(lastLoadFile.c_str());
}
void FWApp::CreateGLRender(){
	if (render) render->EndScene();
	render = new GLRender;
	render->Create(hWnd);	
	if (lastLoadFile.length()) Load(lastLoadFile.c_str());
}
void FWApp::LoadList(){
	if (fileListCur<0) fileListCur = 0;
	if (fileListCur>=(int)fileList.size()) fileListCur = 0;
	if (fileList.size()){
		DSTR << fileListCur << ": " << fileList[fileListCur].c_str() << std::endl;
		Load(fileList[fileListCur]);
	}
	fileListCur ++;
	if (fileListCur>=(int)fileList.size()) fileListCur = 0;
}


//	�t�@�C���̃��[�h
void FWApp::Load(UTString fn){
	if (!render){
		initialLoadFile = fn;
		return;
	}
	WBPath path;
	path.Path(fn);
	//	�t�@�C�����X�g�t�@�C���̏ꍇ
	if (strcmpi(path.Ext().c_str(), ".lst") == 0){
		std::ifstream fs(fn.c_str());
		while(fs.good()){
			char line[1024];
			fs.getline(line, sizeof(line));
			if (strlen(line)){
				if (line[0] == ':'){		//	�R�}���h
					for(int i=1; line[i]; ++i){
						OnKeyDown(line[i]);
					}
				}else if (line[0] == '#'){	//	�R�����g
					//	�������Ȃ��D
				}else{
					fileList.push_back(line);
				}
			}
		}
		path.SetCwd(path.FullDir());
		fileListCur = 0;
		LoadList();
	//	���ʂ̃V�[���t�@�C���̏ꍇ
	}else{
		if (scene) scene->SetTimeStep(defaultTimeStep);
		bool bTimer = timer.IsCreated();
		timer.Release();
		LoadImp(fn);
		if (scene){
			timer.Resolution(UINT(scene->GetTimeStep()*TIMERTICK + 0.5f));
			timer.Interval(UINT(scene->GetTimeStep()*TIMERTICK + 0.5f));
		}
		if (bTimer) timer.Create();
	}
}
void FWApp::Save(UTString fn){
	bool bTimerCreated = timer.IsCreated();
	bool bTimerThread = timer.IsThread();
	timer.Release();
	SaveImp(fn);
	if (bTimerCreated) timer.Create();
	if (bTimerThread) timer.Thread();
}

}

