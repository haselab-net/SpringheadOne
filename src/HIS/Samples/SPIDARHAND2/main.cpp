#include <His/HISpidar4.h>
#include <Device/DRUsb20Simple.h>
#include <conio.h>
#include <iostream>
#include "HISpidarHand.h"
#include "HISpidarFinger.h"

const float K = 2000;
using namespace Spr;
using namespace std;

void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();
	HISpidar4 spidar;
	HISpidarHand aHand;

	std::cout << devMan;

	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	0.265f	//	x�����̕ӂ̒���/2
	#define PY	0.265f	//	y�����̕ӂ̒���/2
	#define PZ	0.265f	//	z�����̕ӂ̒���/2
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-PX, PY, PZ),Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
	Vec3f fingerPos[][4] = {	//	�w�̋Ȃ��n�߈ʒu
		{Vec3f(-0.04f, 0, -0.07f),Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};


	spidar.Init(devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);
	aHand.InitInstance(devMan);
	//�ŏ����͂̔����Ǝw�̏����ʒu�̒ǉ�
	aHand.Init(fingerPos[0],0.4237288f, 0.9203865e-5f, 0.1f, 1.0f);
	cout << aHand.m_pFinger[1]->InitFingerPos1.X()<<endl;

	for(int i=0;i<3;i++){
		aHand.m_pFinger[i]->FingerCalib();
	}

	for(int j=0;j<3;j++){
		aHand.m_pFinger[j]->FingerCalib1();
	}

	while(1){

		/*spidar.Update();
		cout << spidar.Motor()[0].counter->Count() << endl;
		cout << aHand.motor[0].counter->Count() << endl;
		//��̈ʒu���W��\��
		cout << aHand.GetPos().X() <<" "<<aHand.GetPos().Y() << " "<<aHand.GetPos().Z()<<endl;
		*/
		//cout <<"��̈ʒu���W"<< spidar.GetPos().X()<<" "<<spidar.GetPos().Y()<<" "<<spidar.GetPos().Z()<<endl;
		//cout <<"���[�^�̃G���R�[�_�l"<< aHand.motor[0].counter->Count()<<endl;
		//cout <<"�w��Y���W"<< aHand.m_pFinger[0]->FingerPos1.Y()<<endl;
		static int count = 0;
		count ++;
		if (count > 100){
			if (kbhit()){
				int ch = getch();
				switch(ch){
				case 0x1b://esc�L�[
					goto end;
					break;
				case 0x30://"1"�L�[�i�w��90���Ȃ��āj
					for(int i=0;i<3;i++){
						aHand.m_pFinger[i]->FingerCalib();
					}
					break;
				case 0x31://"2"�L�[�i�w���܂������L�΂��āj
					for(int j=0;j<3;j++){
						aHand.m_pFinger[j]->FingerCalib1();
					}
					break;
				}
			}
			cout <<"���̒���"<<aHand.m_pFinger[0]->FingerLength1<<endl;
			count = 0;
		}

		//float force = 10 * (aHand.m_pFinger[0]->FingerPos1.Y()-0.03);
		float diff = aHand.m_pFinger[0]->FingerLength1 - 0.01;
		float force = 0.1f;
		if (diff > 0) {
			force += diff * 1.0f;
		}
		aHand.m_pFinger[0]->m_pMotor1[0].SetForce(force);

		//�e�l���X�V
		aHand.Update();	
		for(int i=0;i<3;i++){
			aHand.m_pFinger[i]->Update();
		}
		
	}//end while
	end:;
}
