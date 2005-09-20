#include <Device/DRContecIsaDa.h>
#include <Device/DRContecIsaCounter.h>
#include <Device/DRUsbH8Simple.h>

#include "App.h"
#include <time.h>
#include <float.h>
#include <strstream>
#include <Render/RDGLUTView.h>
#include <gl/glut.h>


TMassDynamics::TMassDynamics(){
//	mass = 0.6f;
	mass = 3.0f;
}
void TMassDynamics::Update(){
	massPos += massVel * dt;
	massVel += force * dt / mass;
	force.clear();
}
THapticApp::~THapticApp(){
	delete file;
}
THapticApp::THapticApp(){
	bReviseDevPrx = false;

	file = NULL;	

	HRProxyBase::dt = 0.001f;
#ifdef SPIDAR8
	//	SPIDAR8�̏�����
	sp8.RegisterDevice(devMan);
	devMan.Init();
	sp8.Init(devMan);
	for(int i=0; i<8; i++){
		sp8.sp3[i].EasyCalib();
		sp8.sp3[i].Update();
	}
	//	DeviceProxy�̍쐬
	for(int i=0; i<8; ++i){
		devices.push_back(new HRDeviceProxy);
		devices.back()->SetDevice(&sp8.sp3[i]);
	}
#else
	//	SPIDAR�̏�����
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.Init();
	//		std::cout << devMan << std::endl;

	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
#define PX	0.31f	//	x�����̕ӂ̒���/2
#define PY	0.24f	//	y�����̕ӂ̒���/2
#define PZ	0.24f	//	z�����̕ӂ̒���/2
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
	bool spInitFlg[2];
//	spInitFlg[0] = sp[0].Init(devMan, motorPos[0], 0.3653f, 2.924062107079e-5f, 0.5f, 20.0f);
//	spInitFlg[1] = sp[1].Init(devMan, motorPos[1], 0.3653f, 2.924062107079e-5f, 0.5f, 20.0f);
	//140-220 180:200 = 0.38:;
	spInitFlg[0] = sp[0].Init(devMan, motorPos[0], 0.42f, 2.924062107079e-5f, 0.5f, 30.0f);
	spInitFlg[1] = sp[1].Init(devMan, motorPos[1], 0.42f, 2.924062107079e-5f, 0.5f, 30.0f);
//	spInitFlg[0] = sp[0].Init(devMan, motorPos[0], 0.5776f, 2.924062107079e-5f, 0.8f, 20.0f);
//	spInitFlg[1] = sp[1].Init(devMan, motorPos[1], 0.5776f, 2.924062107079e-5f, 0.8f, 20.0f);
	//	DeviceProxy�̍쐬
	for(int i=0; i<2; ++i){
		devices.push_back(new HRDeviceProxy3D);
		if (spInitFlg[0]) devices.back()->SetDevice(&sp[i]);
	}		
#endif
	//	SolidProxy�̏�����
	solid = new HRSolidProxy;
	shape = new TFrame;
	TFrame* f = new TFrame;
	shape->AddChild(f);
				
	TCube* cube = new TCube;
	f->Shape() = cube;
	//	cube->Extent(Vec3f(0.06f, 0.40f, 0.40f));
	cube->Extent(Vec3f(0.06f, 0.10f, 0.40f));
	f->AfParent().Pos() -= cube->Extent()/2;
	
	//	Timer�̏�����
	timer.Set(THapticApp::TimerFunc, this);
	timer.Resolution(HRProxyBase::dt*1000);
	timer.Interval(HRProxyBase::dt*1000);
	timer.Thread();
	windowsTickOffset = GetTickCount();
	hapticStepCount = 0;
	windowsTickCount = 0;
	NormalMode();
}

	
void THapticApp::StopSave(){
	if (file) delete file;
	file = NULL;
}
void THapticApp::StartSave(){
	StopSave();
	char fname[1024];
	struct tm *newtime;
	time_t aclock;
	time( &aclock );                 /* �b�P�ʂŎ������擾 */
	newtime = localtime( &aclock );  /* ������ tm �\���̌`���ɕϊ� */
	std::ostrstream(fname, sizeof(fname))
		<< "R" << devices[0]->reviseRatioNormal << devices[1]->reviseRatioNormal << "_" 
		<< newtime->tm_year+1900 << "." << newtime->tm_mon+1 << " " << newtime->tm_mday << "."
		<< newtime->tm_hour << " " << newtime->tm_min << ".txt" << '\0';
	file = new std::ofstream(fname);
}

void THapticApp::Calib(){
	sp[0].Calib();
	sp[1].Calib();
}

void THapticApp::Reset(){
	solid->ObjectPosture() = Affinef();
	solid->ProxyPosture() = Affinef();
	solid->Trans() = Vec3f();
	solid->Rotation() = Vec3f();
	massDynamics.massPos = Vec3f();
	massDynamics.massVel = Vec3f();
}
void THapticApp::Step(){
	//	�J�E���^
	hapticStepCount ++;
	windowsTickCount = GetTickCount() - windowsTickOffset;
	float s = sin(2 * M_PI* (float)hapticStepCount / 1000.0f);
	float c = cos(2 * M_PI* (float)hapticStepCount / 1000.0f);
	afModel.Pos().X() = 0.06 * sign(s) * pow(abs(s),1.0);
//	afModel.Pos().Y() = 0.03 * sign(c) * pow(abs(c),1.0);
//	float s = sin(1.0 * 2 * M_PI* (float)hapticStepCount / 1000.0f);
//	float s = sin(0.8 * 2 * M_PI* (float)hapticStepCount / 1000.0f);
//	float s = sin(2.0 * 2 * M_PI* (float)hapticStepCount / 1000.0f);
//	afModel.Pos().X() = 0.06 * sign(s) * pow(abs(s),1.0);

	//	���_�̃_�C�i�~�N�X�̍X�V
	massDynamics.Update();
	//	SolidProxy��ObjectPos�����_�̈ʒu�Ɉړ�
	solid->MoveObject(massDynamics.massPos - solid->ObjectPosture().Pos(), Vec3f());

	//	SPIDAR����ʒu��ǂݏo��
	for(int i=0; i<devices.size(); ++i){
		devices[i]->UpdateDevice();
	}
	//	DevPrx�̈ʒu���Փ˔�������čX�V
	shape->AfParent() = solid->ProxyPosture();
	for(TDeviceProxies::iterator it = devices.begin(); it != devices.end(); ++it){
		(*it)->UpdateProxy(shape);
	}
	
	if (bReviseDevPrx){
		//	�O�͂����߂邽�߂ɁC�c���͂������l�����Ă肠���ʒu�̌v�Z����
		//	�΂˂̒ǉ�
		solid->ClearSprings();
		shape->AfParent() = solid->ProxyPosture();
		for(TDeviceProxies::iterator it = devices.begin(); it != devices.end(); ++it){
			if ((*it)->CollideTo()){
				solid->AddSpring((*it)->Spring(), (*it)->GetProxyPos(), (*it)->GetDevicePos());
			}
		}
		//	�c���͂������l�����Ă肠���ʒu�̌v�Z
		Affinef old = solid->ProxyPosture();
		float kTransOrg = solid->KTrans();
		solid->KTrans() = kTransOrg / 1000;
		solid->UpdateProxy();
		solid->KTrans() = kTransOrg;
		Affinef delta = solid->ProxyPosture() * old.inv();

		//	SolidProxy�̈ړ��ɂ��킹�āCDeviceProxy�𓮂���
		shape->AfParent() = solid->ProxyPosture();
		for(TDeviceProxies::iterator it = devices.begin(); it != devices.end(); ++it){
			Vec3f dev = (*it)->GetDevicePos();
			Vec3f after = delta * (*it)->GetProxyPos();
			Vec3f newPrx, normal;
			if (shape->CalcProxyPos(newPrx, normal, after, dev)){
				(*it)->SetProxyPos(newPrx);
			}
		}
		//	�O�͂̌v�Z		
		externalForce =  solid->KTrans() * (massDynamics.massPos - solid->ProxyPosture().Pos());
		//	�O�͂̉e�����L�����Z�����邽�߂Ƀf�o�C�X�ʒu�𓮂���
		for(int i=0; i<devices.size(); i++){
			HRDeviceProxy3D& dev3D = *devices[i];
			TShape* collideTo = dev3D.collideTo;
			Vec3f normal = dev3D.colNormal;
			if (!collideTo){
				collideTo = dev3D.collideToLast;
				normal = dev3D.colNormalLast;
			}
			/*	normal:	�c���Ώە��̖̂ʂ̖@��
				�Ȃ̂ŁC externalForce * normal �͊O�͂̔c���͐���
				dN �͊O�͂̔c���͐�����񎦂��邽�߂ɕK�v�Ȃ΂˂̐L��	*/
			float dN = externalForce * normal / dev3D.Spring();
			float dP = sqrt(externalForce.square() - Square(externalForce * normal)) / dev3D.Spring();
			dN *= dev3D.reviseRatioNormal;
			dP *= dev3D.reviseRatioParallel;
			float d = dN+dP;
			if (d >0){
				dev3D.SetDevicePos(dev3D.GetDevicePos() - d*normal);
			}
		}
	}
	
	//	�΂˂̒ǉ�
	solid->ClearSprings();
	shape->AfParent() = solid->ProxyPosture();
	for(TDeviceProxies::iterator it = devices.begin(); it != devices.end(); ++it){
		if ((*it)->CollideTo()){
			solid->AddSpring((*it)->Spring(), (*it)->GetProxyPos(), (*it)->GetDevicePos());
		}
	}
	//	�肠���ʒu�̌v�Z
	Affinef old = solid->ProxyPosture();
	solid->UpdateProxy();
	Affinef delta = solid->ProxyPosture() * old.inv();
	
	//	SolidProxy�̈ړ��ɂ��킹�āCDeviceProxy�𓮂���
	shape->AfParent() = solid->ProxyPosture();
	for(TDeviceProxies::iterator it = devices.begin(); it != devices.end(); ++it){
		Vec3f dev = (*it)->GetDevicePos();
		Vec3f after = delta * (*it)->GetProxyPos();
		Vec3f revAfter = delta * (*it)->GetProxyPos();
		Vec3f newPrx, normal;
		if (shape->CalcProxyPos(newPrx, normal, after, dev)){
			(*it)->SetProxyPos(newPrx);
		}
	}
	//	DeviceProxy �ɂ��������͂��v�Z
	for(int i=0; i<devices.size(); ++i) devices[i]->CalcForce();

	//	SolidProxy�ɂ�����͂��v�Z
	Vec3f forceToMass;								//	���̂ɉ����O��
	forceToMass += solid->Force();					//	�w����̗�
	forceToMass += massDynamics.mass * gravity;		//	�d��
	float WALL = -0.08f;
	if (solid->ObjectPosture().Pos().Y() < WALL){
		forceToMass.Y() += 1000*(WALL - solid->ObjectPosture().Pos().Y());
		massDynamics.massVel *= 0.99;
	}
	massDynamics.AddForce(forceToMass);
}
//#define DRAW_DEBUG
void THapticApp::Draw(){

	if (file){
		File() << hapticStepCount;
		File() << "\t" << solid->ObjectPosture().Pos().X();
		File() << "\t" << massDynamics.massVel.X();
		File() << "\t" << afModel.Pos().X();
		File() << "\t" << devices[0]->devicePosOrg.X();
		File() << "\t" << devices[1]->devicePosOrg.X();
		File() << "\t" << devices[1]->devicePosOrg.X() - devices[0]->devicePosOrg.X();
		File() << "\t" << devices[0]->Force().X();
		File() << "\t" << devices[1]->Force().X();
		File() << "\t" << devices[1]->Force().X() + devices[0]->Force().X();
		File() << std::endl;
	}

#ifdef DRAW_DEBUG
	glDisable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(0, 0, 1, 0.5f));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(0, 0, 1, 0.5f));
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f());
	shape->AfParent(solid->ObjectPosture());
	shape->Draw();
		
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(1, 0.1f, 0, 0.5f));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(1, 0.1f, 0, 0.5f));
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f());
	shape->AfParent(solid->ProxyPosture());
	shape->Draw();

	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(0, 0, 1, 0.3f));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(0, 0, 1, 0.3f));
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f());
	shape->AfParent(solid->ObjectPosture());
	shape->Draw();
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(1, 0.1f, 0, 0.3f));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(1, 0.1f, 0, 0.3f));
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f());
	shape->AfParent(solid->ProxyPosture());
	shape->Draw();

	glDisable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(0,0,0,1));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(0,0,0,1));
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(0,1,1,1));
	glBegin(GL_LINES);
	for(int i=0; i<devices.size(); ++i){
		glVertex3fv(devices[i]->GetProxyPos());
		glVertex3fv(devices[i]->GetDevicePos());
	}
	glEnd();
	glPointSize(2.0);
	glBegin(GL_POINTS);
	for(int i=0; i<devices.size(); ++i){
		if (i == devices.current){
			glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(1,0,0,1));
		}else{
			glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(1,1,0,1));
		}
		glVertex3fv(devices[i]->GetDevicePos());
	}
	glEnd();
	
	glEnable(GL_DEPTH_TEST);
#else
	//	solid
	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(0.7f, 0.7f, 0.7f, 1));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(0.7f, 0.7f, 0.7f, 1));
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f());
	shape->AfParent(solid->ObjectPosture());
//	shape->AfParent(solid->ProxyPosture());
	shape->Draw();
/*	glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glMultMatrixf(shape->AfParent());
		glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(1,1,1,1));
		glutSolidSphere(0.01, 16, 8);
		glEnable(GL_DEPTH_TEST);
	glPopMatrix();
*/	//	finger
	Affinef prxToObj = solid->ObjectPosture() * solid->ProxyPosture().inv();
	for(int i=0; i<devices.size(); i++){
		glPushMatrix();
		Affinef afFinger;
		afFinger.Pos() = devices[i]->GetProxyPos();
		afFinger = prxToObj * afFinger;
		glMultMatrixf(afFinger);
		if (i==0){
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(0.9f, 0, 0, 1));
			glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(0.9f, 0, 0, 1));
		}else{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(0, 0, 0.9f, 1));
			glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(0, 0, 0.9f, 1));
		}
		glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f());
		glutSolidSphere(0.01, 16, 8);
		glPopMatrix();
	}
#endif
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 0);
	glBegin(GL_LINES);
	float zzz = 0.0f;
	float xxx = 0.06f;
	glVertex3f(xxx, 10, zzz);
	glVertex3f(xxx, -10, zzz);
	glVertex3f(-xxx, 10, zzz);
	glVertex3f(-xxx, -10, zzz);
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
//	/*
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
		glMultMatrixf(afModel);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Vec4f(0, 0, 0, 1.0));
		glMaterialfv(GL_FRONT, GL_AMBIENT, Vec4f(0, 0, 0, 1.0));
		glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(1,1,0, 1.0));
		glutSolidSphere(0.01, 16, 8);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
//*/
}
void THapticApp::TimerFunc(void* arg){
	THapticApp* app = (THapticApp*)arg;
	app->Step();
}

const float kTransProxy = 1000;
const float bTransProxy = 0;
const float kRotationSolid = 100;
const float bRotationSolid = 0;
void THapticApp::NormalMode(){
	solid->SetSpringDamper(kTransProxy * 1000, 0, kRotationSolid, bRotationSolid);
	for(int i=0; i<devices.size(); ++i) devices[i]->Spring() = kTransProxy;

	bReviseDevPrx = false;
	if (file) File() << "Normal Mode " << std::endl;
}
void THapticApp::KawaiYoshikawaMode(){
	solid->SetSpringDamper(kTransProxy, 0, kRotationSolid, bRotationSolid);
	for(int i=0; i<devices.size(); ++i) devices[i]->Spring() = kTransProxy;
	bReviseDevPrx = false;
	if(file) File() << "Kawai Yoshikawa Mode " << std::endl;
}
void THapticApp::HaseMode(float* nR, float* pR){
	solid->SetSpringDamper(kTransProxy, 0, kRotationSolid, bRotationSolid);
	bReviseDevPrx = true;
	if (file) File() << "Hase Mode  ratio:";
	for(int i=0; i<devices.size(); ++i){
		devices[i]->Spring() = kTransProxy;
		devices[i]->reviseRatioNormal = nR[i];
		devices[i]->reviseRatioParallel = pR[i];
		if (file){
			File() << " n" << i << ":" << devices[i]->reviseRatioNormal;
			File() << " p" << i << ":" << devices[i]->reviseRatioParallel;
		}
	}
	if (file) File() << std::endl;
}
