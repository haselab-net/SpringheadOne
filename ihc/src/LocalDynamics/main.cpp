#include <IHCBase/IHUsbInterrupt.h>
#include <IHCBase/IHUsbPacket.h>

#include <His/HISpidar4.h>
#include <Device/DRIHCSh4.h>
#include "../../../Experiments/LocalDynamics/LDRenderer.h"

#include <stdio.h>
#include <cyg/kernel/kapi.h>

#define PACKET_SIZE 512

class IHThread{
protected:
	cyg_handle_t handle;
	cyg_thread   object;
	char stack[256*1024];
	
	volatile bool isActive;
	cyg_handle_t alarmHandle;
	cyg_alarm alarmObject;
	static cyg_handle_t systemCounter;
	static cyg_handle_t systemClock;
public:
	IHThread(){
		cyg_thread_create(1, ThreadFunc, (cyg_addrword_t)this, "alarmed_thread", stack, sizeof(stack), &handle, &object);
		
		//	本当は，アラームは派生クラスに移すべきだなあ．
		isActive = false;
		if (!systemClock){
			systemClock = cyg_real_time_clock();
			if(!systemClock){
				printf("No system clock...\n");
				return;
			}
			cyg_clock_to_counter(systemClock, &systemCounter);
		}
		//create alarms
		cyg_alarm_create(systemCounter, AlarmFunc, (cyg_addrword_t) this, &alarmHandle, &alarmObject);
		int interval = 1;	//	1秒	このへんは，コンストラクタか，Init関数の引数にすべき．
		cyg_alarm_initialize(alarmHandle, cyg_current_time()+interval, interval);
	}
	virtual ~IHThread(){}
	static void ThreadFunc(cyg_addrword_t arg){
		while(1){
			IHThread* th = (IHThread*) arg;
			th->Step();
			cyg_scheduler_lock();
			th->isActive = false;
			cyg_thread_suspend(th->handle);
			cyg_scheduler_unlock();
		}
	}
	virtual void Step(){
		DSTR << "Step" << std::endl;
	}
	static void AlarmFunc(cyg_handle_t alarm_hdl, cyg_addrword_t arg){
		IHThread* th = (IHThread*)arg;
		if(!th->isActive){
			cyg_scheduler_lock();
			th->isActive = true;
			cyg_thread_resume(th->handle);
			cyg_scheduler_unlock();
		}
	}
};
cyg_handle_t IHThread::systemCounter;
cyg_handle_t IHThread::systemClock;



using namespace Spr;
using namespace std;

class Thread:public IHThread{
public:
	DVDeviceManager devMan;		//	デバイスドライバを入れておく入れ物
	LDRenderer ldRenderer;
	HISpidar4 spidar;
	Thread(){
		devMan.RPool().Register(new DRIHCSh4);			//	SH4用のRealDeviceを登録
		devMan.Init();
		DSTR << devMan;	//	デバイスの状態の表示
		#define PX	0.265f	//	x方向の辺の長さ/2
		#define PY	0.265f	//	y方向の辺の長さ/2
		#define PZ	0.265f	//	z方向の辺の長さ/2
		Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
			{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
			{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
		};
		spidar.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
		ldRenderer.device = &spidar;
		spidar.SetMinForce();
		spidar.Update(0.001f);
		cyg_thread_delay(20000);	//	2秒待ち	
		spidar.Calib();
//		DSTR << "Calibrated." << std::endl;
	}
	virtual void Step(){
//		DSTR << "Step\n";
		ldRenderer.Step();
	}
};

class Usb: public IHUsbInterrupt{
protected:
	IHPacketLD packet;
public:
	Thread* ctrl;
	Usb():ctrl(NULL){
		std::cout << "Usb" << std::endl;
	}
	virtual void OnReceive(char* recv){
		static char send[PACKET_SIZE];
		IHPacketBase& recvPacket = (IHPacketBase&)recv[0];
		static IHPacketLDReturn ret;
		if (recvPacket.id == packet.id){
			ret.pointer = ctrl->ldRenderer.GetPointer();
			ret.pos = ctrl->ldRenderer.GetPos();
			if (recvPacket.size != packet.size){
				DSTR << "Invalid packet size:" << recvPacket.size << "should be " << packet.size << "." << std::endl;
			}
			memcpy(&packet, recv, sizeof(packet));
			if (ctrl){
				ctrl->ldRenderer.SetParams(packet.bChanged, packet.k, packet.b, packet.pos, packet.normal, packet.velocity, packet.f_external, packet.Minv);
			}
			memcpy(send, &ret, sizeof(ret));
		}else{
			memset(send, 0, sizeof(send));
		}
		Send(send);
	}
};

int main(){
	printf("\n%s: main started.\n", __FILE__);
	//	USBハンドラの生成
	Usb* usb = new Usb;
	//	制御スレッドの作成と開始
	cyg_scheduler_lock();
	Thread* thread = new Thread;
	usb->ctrl = thread;
	cyg_scheduler_unlock();
	while(1){
		cyg_thread_delay(1000);
	}
}
