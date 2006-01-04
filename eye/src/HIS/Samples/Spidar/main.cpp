#include <iostream>
#include "His/HISpidar4.h"


#ifdef _WIN32
 #include <windows.h>
 #include <conio.h>

 //	USB2.0�A���v�̃h���C�o
 #include <Device/DRUsb20Simple.h>
 #include <Device/DRUsb20Sh4.h>
 //	Contect��ISA�J�[�h�̃h���C�o�D�Â�����(1997-)�D
 //#include "Device/DRContecIsaDa.h"
 //#include "Device/DRContecIsaCounter.h"
 //#include <Device/DRUsbH8Simple.h>
#endif

#ifdef __sh__
 #include <cyg/kernel/kapi.h>
 #include <Device/DRIHCSh4.h>
 int kbhit(){return false;}
 int getch(){ return 0;}
 void Sleep(int msec){
	cyg_thread_delay(msec*10);
 }
 #define cout DSTR
#endif

using namespace Spr;	//	Spr��Springhead���C�u�����̖��O���
using namespace std;

int main(){
	//------------------------------------------------------------------------
	//	����������
	//
	DVDeviceManager devMan;		//	�f�o�C�X�h���C�o�����Ă������ꕨ

#ifdef _WIN32
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0�A���v(ID 10)��o�^
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	SH4�A���v(ID 0)��o�^
	//	ID�̓A���v���̃��[�^���X�C�b�`�̒l(0x0-0xF)�ł��D

	/*	�g���Ă��Ȃ��h���C�o�D�o�^���Ă����Ύ����F������D
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	*/
	bool bMinForce = true;							//	�����Ȃ�͂��o���Ɗ댯�Ȃ̂ŁC�X�y�[�X�������܂ł͍ŏ����͂��o��
	const float K = 2000.0f;						//	�΂˒萔
#endif
#ifdef __sh__
	devMan.RPool().Register(new DRIHCSh4);			//	SH4�p��RealDevice��o�^
	bool bMinForce = false;							//	SH�̏ꍇ�͒ʐM�ł��Ȃ��̂ŁC�ŏ�����ON
	const float K = 20000.0f;						//	�΂˒萔 10kHz������10�{
#endif

	/*	�h���C�o�̏������D���݂���f�o�C�X�ɑΉ�����o�[�`�����f�o�C�X��
		�����ŏ����������	*/
	devMan.Init();
	cout << devMan;	//	�f�o�C�X�̏�Ԃ̕\��

	HISpidar4 spidar;
	HISpidar4 spidar2;

	//	SPIDAR�̏������D�{���́C�n�[�h�E�G�A���� HISpidar4 �N���X��h��������ׂ�
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	0.265f	//	x�����̕ӂ̒���/2
	#define PY	0.265f	//	y�����̕ӂ̒���/2
	#define PZ	0.265f	//	z�����̕ӂ̒���/2
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};
	cout.flush();
	spidar.Init(devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
	cout.flush();
	printf("Waiting...");
	Sleep(1000);
	printf("Calibration\n");
/*
	spidar.Motor()[0].lengthPerPulse *= -1;
	spidar2.Motor()[0].lengthPerPulse *= -1;
	spidar2.Motor()[2].lengthPerPulse *= -1;
*/
	spidar.Calib();
	spidar2.Calib();
	
	//------------------------------------------------------------------------
	//	���C�����[�v
	//
	while(1){
		if (kbhit()){	//	�L�[����
			int ch = getch();
			switch(ch){
			case ' ':		//	�X�y�[�X�F�͊oON/OFF
				bMinForce = !bMinForce;
				break;
			case 0x1b:		//	ESC:�I��
				goto end;
				break;
			}
		}
		
		spidar.Update(0.001f);				//	�A���v�Ƃ̒ʐM(�͂�񎦁E�ʒu���擾)
		Vec3f force;					//	�񎦂����
		if (spidar.GetPos().Y() < 0){	//	���̒�
			force.Y() = - K * spidar.GetPos().Y();
		}
		if (force.norm() > 10){			//	�傫������͂͒񎦂ł��Ȃ��̂ŁC10N�ɐ���
			force = force.unit() * 10;
		}
		//------------------------------------------------------------------------
		//	�񎦗͂̐ݒ�
		if (bMinForce) spidar.SetMinForce();
		else spidar.SetForce(force);

		//	�ȉ�2�Ԗڂ�SPIDAR�ɂ��ē��l�ɁC
		Vec3f force2;
		spidar2.Update(0.001f);
		if (spidar2.GetPos().Y() < 0){
			force2.Y() = - K * spidar2.GetPos().Y();
		}
		if (bMinForce) spidar2.SetMinForce();
		else spidar2.SetForce(force2);
		//	�f�o�b�O�p�F�ʒu�Ɨ͂�\��
#if 0
		cout << "P:" << spidar.GetPos() << "\tF:" << spidar.GetForce() << std::endl;
#endif
		//	�f�o�b�O�p�F���̒�����\��
#if 1
		for(int i=0; i<4; i++){
			cout << spidar.Motor()[i].GetLength() << " ";
		}
		for(int i=0; i<4; i++){
			cout << spidar2.Motor()[i].GetLength() << " ";
		}
		cout << std::endl;
		cout.flush();
#endif
	} 
end:;
	return 0;
}
