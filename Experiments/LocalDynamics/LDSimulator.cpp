#include "LDSimulator.h"
#include "Base/TQuaternion.h"
#include "Base/Affine.h"
#include <Graphics/GRRender.h>
#include <Graphics/GRMaterial.h>

#include "../../ihc/src/IHCBase/IHUsbPacket.h"

#define FR_DUMMYSPHERE	"frDummySphere"
#define SO_DUMMYSPHERE	"soDummySphere"
#define FR_POINTER		"frPointer1"
#define SO_POINTER		"Pointer1"
#define FR_POINTER2		"frPointer2"
#define SO_POINTER2		"Pointer2"
	

#define PACKETSIZE 512

//#define EMULATE_IHC

namespace Spr{;

LDSimulator::LDSimulator(){
	int buffer = 8;
	char str_k[8], str_b[8];
	GetPrivateProfileString("PARAMETERS", "K", "500.0", str_k, buffer, "./setting.ini");
	GetPrivateProfileString("PARAMETERS", "B", "10.0" , str_b, buffer, "./setting.ini");
	k = atof(str_k);
	b = atof(str_b);

	ResetParameters();
	prevPos.clear();
	pointer.clear();
	pDSFrameRecord = NULL;
	pPointerFrameRecord = NULL;
	pPointer2FrameRecord = NULL;
	pPrevFrameRecord = NULL;
	bChanged = false;
	calc_ld  = true;
	bExperiment = false;
	impulse.clear();
}

LDSimulator::~LDSimulator(){
}

void LDSimulator::ResetParameters(){
	pFrameRecord = NULL;
	mass_inv = 1.0f;
	pos.clear();
	normal.clear();
	velocities.clear();
	f_external.clear();
	torque_ex.clear();
	Iinv.clear();
}

void LDSimulator::Step(){
	prevPos = pos;
	pPrevFrameRecord = pFrameRecord;

#ifdef USE_IHC
	IHCReceive();
#else
#ifndef EMULATE_IHC
	pLDRenderer->CSEnter();
	Receive();   //LocalDynamicsから情報取得
	pLDRenderer->CSLeave();
#endif //EMULATE_IHC
#endif
	if(calc_ld && !bExperiment){ // 局所動力学を計算する場合
		if(pFrameRecord){
			IntegrateLocalDynamics();
			pFrameRecord->solid->SetIntegrationMode(PHINT_NONE);
			scene->Integrate();
			pFrameRecord->solid->SetIntegrationMode(PHINT_SIMPLETIC);
		}
		else{
			scene->Integrate();
		}
	}
	else{ // 局所動力学を計算しない場合
		if(pFrameRecord)
			pFrameRecord->solid->AddForce(impulse/simuDT, pos);
		scene->Integrate();
	}

	//球の位置の更新
	if(pDSFrameRecord){
		pDSFrameRecord->solid->SetCenterPosition(pointer);
		pDSFrameRecord->solid->UpdateFrame();
	}

	if(pPointerFrameRecord){
		pPointerFrameRecord->solid->SetCenterPosition(pointer);
		pPointerFrameRecord->solid->UpdateFrame();
	}

	//衝突エンジン
	scene->ClearForce();
	scene->GenerateForce();
//	scene->Integrate();

	DefineLocalArea();

	if(pPrevFrameRecord != pFrameRecord)
		bChanged = true;
	else
		bChanged = false;

#ifdef USE_IHC
	IHCSend();
#else
	pLDRenderer->CSEnter();
	Send();	//LocalDynamicsへ情報送信
#ifdef EMULATE_IHC
	Receive();
#endif //EMULATE_IHC
	pLDRenderer->CSLeave();
#endif

}

void LDSimulator::IntegrateLocalDynamics(){
	//物体の移動
	if(pFrameRecord->solid->GetMass() < 10000){ // 暫定処置(床が動かないように)
		//速度・角速度更新
		pFrameRecord->solid->SetVelocity(velocities.sub_vector(0, Vec3d()));
		pFrameRecord->solid->SetAngularVelocity(velocities.sub_vector(3, Vec3d()));
		
		//位置・方向の更新
		pFrameRecord->solid->SetFramePosition(pFrameRecord->solid->GetFramePosition() + displacement.sub_vector(0, Vec3d()));
		Vec3d dTheta = displacement.sub_vector(3, Vec3d());
		if(dTheta.norm() > 1e-6f){
			Quaterniond quat = Quaterniond::Rot(dTheta.norm(), dTheta.unit());
			pFrameRecord->solid->SetOrientation(quat * pFrameRecord->solid->GetOrientation());
		}
		
		pFrameRecord->solid->UpdateFrame();
	}
}

void LDSimulator::DefineLocalArea(){
	if(!vClosestPoints.empty()){
		int iClosestPoint;
		iClosestPoint = FindClosestPoint();
		pFrameRecord  = vFrameRecords[iClosestPoint];
		
		Matrix3d inertia_inv;
		mass_inv	= pFrameRecord->solid->GetMassInv();
		inertia_inv	= pFrameRecord->solid->GetInertiaInv(); // ローカル座標系
		rotation	= pFrameRecord->solid->GetFrame()->GetRotation();

		inertia_inv = rotation * inertia_inv * rotation.inv(); // ワールド座標系へ
//		inertia_inv = rotation.inv() * inertia_inv * rotation;

		// 位置・速度・力（外力）の取得
		pos        = vClosestPoints[iClosestPoint] + pFrameRecord->solid->GetFramePosition();
		center     = pFrameRecord->solid->GetCenterPosition();
		torque_ex  = pFrameRecord->solid->GetTorque();
		f_external = pFrameRecord->solid->GetForce();
		p          = pos - center;
		velocities.sub_vector(0, Vec3d()) = pFrameRecord->solid->GetVelocity();
		velocities.sub_vector(3, Vec3d()) = pFrameRecord->solid->GetAngularVelocity();

		//normal
		normal = vNormals[iClosestPoint];

		//慣性行列の設定
		for(int i = 0; i < 3; i++)
			Iinv[i][i] = mass_inv;
		Iinv.sub_matrix(3,3,inertia_inv) = inertia_inv;

		vClosestPoints.clear();
		vFrameRecords.clear();
		vNormals.clear();
	}
	else { 
		ResetParameters();
	}

}

int LDSimulator::FindClosestPoint(){
	
	double closest = (pointer - (vClosestPoints[0] + vFrameRecords[0]->solid->GetFramePosition())) * vNormals[0];
	int index = 0;
	for(int i = 1; i < vClosestPoints.size(); i++)
		if(closest > (pointer - (vClosestPoints[i] + vFrameRecords[i]->solid->GetFramePosition())) * vNormals[i]){ index = i; }
	
	return index;
}

void LDSimulator::Send(){
	if(bChanged)
		DSTR << "bChanged" << std::endl;
	IHPacketLD packet;
	packet.bChanged   = bChanged;
	packet.calc_ld    = calc_ld;
	packet.k          = k;
	packet.b          = b;
	packet.pos        = pos;
	packet.normal     = normal;
	packet.velocities = velocities;
	packet.f_external = f_external;
	packet.torque_ex  = torque_ex;
	packet.Iinv       = Iinv;
	packet.center     = center;

	pLDRenderer->SetParameters(packet);
}

void LDSimulator::Receive(){
	pos          = pLDRenderer->GetPos();
	velocities   = pLDRenderer->GetVelocities();
	pointer      = pLDRenderer->GetPointer();
	impulse      = pLDRenderer->GetImpulse();
	displacement = pLDRenderer->GetDisplacement();
	pLDRenderer->ResetParameters();
}

void LDSimulator::IHCSend(){
	char buf[PACKETSIZE];
	IHPacketLD packet;

	packet.bChanged   = bChanged;
	packet.calc_ld    = calc_ld;
	packet.k          = k;
	packet.b          = b;
	packet.pos        = pos;
	packet.normal     = normal;
	packet.velocities = velocities;
	packet.f_external = f_external;
	packet.torque_ex  = torque_ex;
	packet.Iinv       = Iinv;
	packet.center     = center;

	packet.posScale    = device->posScale;
	packet.forceScale  = device->forceScale;
	packet.qtOffset    = device->GetQTOffset();
	packet.qtOffsetInv = device->GetQTOffsetInv();
	packet.v3Offset    = device->GetV3Offset();

	packet.EndianAll();
	memset(buf, 0, sizeof(buf));
	memcpy(buf, &packet, sizeof(packet));
	ihc->UsbSend((unsigned char*)buf);
}

void LDSimulator::IHCReceive(){
	char buf[PACKETSIZE];
	memset(buf, 0, sizeof(buf));
	ihc->UsbRecv((unsigned char*)buf);

	IHPacketLDReturn ret;
	memcpy(&ret, buf, sizeof(ret));
	ret.EndianAll();

	pointer      = ret.pointer;
	pos          = ret.pos;
	velocities   = ret.velocities;
	displacement = ret.displacement;
	impulse		 = ret.impulse;

	DSTR << "velocities = " << velocities << "\n"
		<< "displace   = " << displacement << "\n" << std::endl;
}

void LDSimulator::Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer){
	int iConvex;
	SGFrame* sg;
	PHContactEngine* pce;
	scene->GetBehaviors().Find(pce);

	if(!pce){ return; }

	sg = fr->frame[0]->frame;

	if(!strcmp(FR_DUMMYSPHERE, sg->GetName())){
		iConvex = 1;
		sg = fr->frame[1]->frame;
	}
	else { iConvex = 0; }

	if(!strcmp("frFloor", fr->frame[iConvex]->frame->GetName())){
		return;
	}

	if(!strcmp(FR_POINTER, fr->frame[iConvex]->frame->GetName())){
		return;
	}

	if(!strcmp(FR_POINTER2, fr->frame[iConvex]->frame->GetName())){
		return;
	}

	vClosestPoints.push_back(conv->closestPoint[iConvex]);
	vFrameRecords.push_back(pce->GetFrameRecord(sg));

	if(iConvex == 1)
		vNormals.push_back(-conv->normal.unit());
	else
		vNormals.push_back(conv->normal.unit());
}

void LDSimulator::AddListener(){
	UTRef<CDCollisionEngine> ce;
	scene->GetBehaviors().Find(ce);

	if (!ce){ return; }

	// リスナに登録
	listenerPos = ce->AddListener(this);

	SGFrame* fr = DCAST(SGFrame, scene->FindObject(FR_DUMMYSPHERE));

	if(fr){
		for(int i = 0; i < ce->NFrame(); i++){
			SGFrame* sg = ce->GetFrame(i);
			if(sg != fr){
				ce->AddActive(fr, sg, listenerPos);
			}
		}
	}
	//	初期化(frames から pairsを作る)
	ce->Init();
}

void LDSimulator::FindDSFrameRecord(){
	PHContactEngine* pce;
	scene->GetBehaviors().Find(pce);

	if(!pce){ pDSFrameRecord = NULL; return; }

	SGFrame* fr = DCAST(SGFrame, scene->FindObject(FR_DUMMYSPHERE));
	if(fr){
		pDSFrameRecord = pce->GetFrameRecord(fr);
		DSTR << "FindDSRecord" << std::endl;
	}
	else
		pDSFrameRecord = NULL;

	fr = NULL;
	fr = DCAST(SGFrame, scene->FindObject(FR_POINTER));
	if(fr){
		pPointerFrameRecord = pce->GetFrameRecord(fr);
		DSTR << "PointerRecordFind" << std::endl;
	}
	else
		pPointerFrameRecord = NULL;

	fr = NULL;
	fr = DCAST(SGFrame, scene->FindObject(FR_POINTER2));
	if(fr){
		pPointer2FrameRecord = pce->GetFrameRecord(fr);
	}
	else
		pPointer2FrameRecord = NULL;
}

void LDSimulator::Load(std::string fn){
	bBusy = true;
	cs.Enter();
	if (!render){
		initialLoadFile = fn;
		return;
	}
	lastLoadFile = fn;

	scene->Clear();
	scene->GetRenderers().Add(render);

	fileIO.Load(lastLoadFile.c_str(), scene);
	ConnectHis();
	cs.Leave();
	bBusy = false;

	AddListener();
	PHContactEngine* pc;
	scene->GetBehaviors().Find(pc);
	if (pc){
		pc->Init(scene);
	}

	SetSimuDT(scene->GetTimeStep());
#ifndef USE_IHC
	pLDRenderer->SetSimuDT(scene->GetTimeStep());
#endif
	FindDSFrameRecord();
}

bool LDSimulator::OnKeyDown(unsigned int nChar){
	//	キー操作
	//	視点移動は HIMouse が行う．
	if ( nChar == 'C'){
		for(unsigned i=0; i<pointers.size(); ++i){
			if (pointers[i]->device){
				pointers[i]->device->Calib();
			}
		}
	}else if ( nChar == 'D' ){
		PHContactEngine* pce;
		scene->GetBehaviors().Find(pce);
		if (pce) pce->bDraw = !pce->bDraw;
	}else if ( nChar == 'F' ){
		render->Fullscreen();
	}else if ( nChar == 'G' ){
		render->Window();
	}else if (nChar == 'H'){		//	スレッド
		timer.Thread();
		KillTimer(hWnd, simuTimerId);
	}else if (nChar == 'M'){		//	multi media timer		
/*		if (scene){
			timer.Resolution(UINT(scene->GetTimeStep()*TIMERTICK));
			timer.Interval(UINT(scene->GetTimeStep()*TIMERTICK));
		}
		timer.Create();				//	マルチメディアタイマー
		KillTimer(hWnd, simuTimerId);*/
	}else if (nChar == 'P'){		//	pause
		timer.Release();
		KillTimer(hWnd, simuTimerId);
		Step();
	}else if ( nChar == 'R' ){		//	リロード
		Reset();
	}else if (nChar == 'T'){		//	message timer
		timer.Release();
		SetTimer(hWnd, TIMER_SIMU, UINT(simuTimerPeriod*1000), (TIMERPROC)WMTimerProc);
	}else if ( nChar == VK_SPACE ){
		bOutForce = !bOutForce;
		for(unsigned i=0; i<pointers.size(); ++i){
			pointers[i]->SetOutForce(bOutForce);
		}
	}else if (nChar == VK_RETURN){	//	リストから次のファイルをロード
		if (fileList.size()){
			LoadList();
		}
	}else if ( nChar == ',' ){
		if (scene->GetTimeStep() > 0.01) scene->SetTimeStep(scene->GetTimeStep() - 0.01);
		else scene->SetTimeStep(scene->GetTimeStep() - 0.001);
		DSTR << "dt:" << scene->GetTimeStep() << std::endl;
	}else if ( nChar == '.' ){
		if (scene->GetTimeStep() > 0.01) scene->SetTimeStep(scene->GetTimeStep() + 0.01);
		else scene->SetTimeStep(scene->GetTimeStep() + 0.001);
		DSTR << "dt:" << scene->GetTimeStep() << std::endl;
	}else{
		return false;
	}
	return true;
}

} // Spr

