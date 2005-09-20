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
		
		//	�{���́C�A���[���͔h���N���X�Ɉڂ��ׂ����Ȃ��D
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
		int interval = 1;	//	1�b	���̂ւ�́C�R���X�g���N�^���CInit�֐��̈����ɂ��ׂ��D
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
	DVDeviceManager devMan;		//	�f�o�C�X�h���C�o�����Ă������ꕨ
	LDRenderer ldRenderer;
	HISpidar4 spidar;
	Thread(){
		devMan.RPool().Register(new DRIHCSh4);			//	SH4�p��RealDevice��o�^
		devMan.Init();
		DSTR << devMan;	//	�f�o�C�X�̏�Ԃ̕\��
		#define PX	0.265f	//	x�����̕ӂ̒���/2
		#define PY	0.265f	//	y�����̕ӂ̒���/2
		#define PZ	0.265f	//	z�����̕ӂ̒���/2
		Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
			{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
			{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
		};
		spidar.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
		ldRenderer.device = &spidar;
		spidar.SetMinForce();
		spidar.Update(0.001f);
		cyg_thread_delay(20000);	//	2�b�҂�	
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
	//	USB�n���h���̐���
	Usb* usb = new Usb;
	//	����X���b�h�̍쐬�ƊJ�n
	cyg_scheduler_lock();
	Thread* thread = new Thread;
	usb->ctrl = thread;
	cyg_scheduler_unlock();
	while(1){
		cyg_thread_delay(1000);
	}
}
