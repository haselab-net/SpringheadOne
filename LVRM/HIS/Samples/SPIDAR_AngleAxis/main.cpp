#include "HIS/HISpidar4AngleAxisQuad.h"
#include "glut_test.h"
/// USB20���g��Ȃ��Ƃ��ɂ͉��́u#define _USB20_�v���R�����g�A�E�g
//#define _USB20_

#ifdef _USB20_
#include <Device/DRUsb20Simple.h>
#else
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>
#endif //_USB20_

#include <conio.h>
#include <iostream>
//�ǉ�
#include<GL/glut.h>
#include <windows.h>
#include <mmsystem.h>

const float K = 2000;
using namespace LVRM;

//�^�C�}�[�p�̕ϐ�
	DWORD uTimerRes;
	UINT TimerId;

//Multimediatimer
//�����ł̓^�C�}�[�ōs������������
void CALLBACK TimerProc(
    UINT uID,     // �^�C�}�[ID
    UINT uMsg,    // �g��Ȃ�
    DWORD dwUser, // ���[�U�[��`
    DWORD dw1,    // �g��Ȃ�
    DWORD dw2     // �g��Ȃ�
   )
{
	//�^�C�}�[����
	((HISpidar4angleaxisquad*)dwUser)->Update();

}


HISpidar4angleaxisquad spidar;
Glut_test gl;
void main(){

	DVDeviceManager devMan;
	
	//Glut_test�ō����HISpidar4quad�̃I�u�W�F�N�g�ɃA�h���X�������
	gl.sp = &spidar;

#ifdef _USB20_
	devMan.RPool().Register(new DRUsb20Simple(0));
#else
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
#endif //_USB20_
	devMan.Init();

	std::cout << devMan;

	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
//	#define PX	0.265f//0.53f	//	x�����̕ӂ̒���/2
//	#define PY	0.265f//0.53f	//	y�����̕ӂ̒���/2
//	#define PZ	0.265f//0.53f	//	z�����̕ӂ̒���/2
	//���̒l�͎����l��0,3�Ԗڂ̃��[�^��-z�������A1,2�Ԗڂ̃��[�^��-y�������Ɏ��t���Ă���ꍇ�ł���
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		Vec3f(-0.315f, 0.280f, 0.172f),	Vec3f( 0.315f, 0.200f,-0.230f), Vec3f(-0.315f,-0.200f,-0.230f), Vec3f( 0.315f,-0.280f, 0.172f)
	};

	

	//�@�茳�ʒu�̍ŏ��̈ʒu�A�p��
	//  (x+x_1,y+y_1,z+z_1),(x-x_1,y-y_1,z-z_1),(x+x_2,y+y_2,z+z_2),(x-x_2,y-y_2,z-+z_2),(x,y,z)
	//  �̏��Ԃŏ����Ă���
	Vec3f initPos[5] = {
		Vec3f(-0.045f,0.0f,0.04f), Vec3f(0.045f,0.0f,-0.04f), Vec3f(-0.045f,0.0f,-0.04f), Vec3f(0.045f,0.0f,0.04f), Vec3f(0.0f,0.0f,0.0f)
	};
	//CD�o�[�W����
/*	Vec3f initPos[5] = {
		Vec3f(-0.0389f,0.0f,0.0389f), Vec3f(0.0389f,0.0f,-0.0389f), Vec3f(-0.0389f,0.0f,-0.0389f), Vec3f(0.0389f,0.0f,0.0389f), Vec3f(0.0f,0.0f,0.0f)
	};
*/	
	spidar.Init(devMan, motorPos[0], initPos, /*0.2f*/0.3776f, 2.924062107079e-5f, 0.001570796326794896618f/*1.570796326795e-3f*/, 2.0f, 20.0f);
	//�p�x�p�G���R�[�_�̉�]�̌�����␳���邽�߂̌v�Z
	spidar.Motor()[0].anglePerPulse *= -1;
	spidar.Motor()[2].anglePerPulse *= -1;
	spidar.Motor()[3].anglePerPulse *= -1;
	//���[�^�̉�]���t�ɂ���ꍇ
//	spidar.Motor()[0].lengthPerPulse *= -1;

	spidar.Calib();

	//�^�C�}�[�̃X�^�[�g
//	TimerId = timeSetEvent(1, 1, TimerProc, (DWORD)& spidar, TIME_PERIODIC);

	
	//�`��̊J�n
//	gl.Init("SPIDAR ANGLE TEST");
//	gl.Run();

	while(1){

		if (kbhit()){
			int ch = getch();
			switch(ch){
			case 0x1b:
				goto end;
				break;
			case  0x36:
				spidar.Calib();
				break;
			}
		}




//		std::cout << "/****************�����Ɗp�x*****************/" << std::endl;
/*		std::cout << "l1   =   " <<  spidar.l1 << std::endl;
		std::cout << "l2   =   " <<  spidar.l2 << std::endl;
		std::cout << "l3   =   " <<  spidar.l3 << std::endl;
		std::cout << "l4   =   " <<  spidar.l4 << std::endl;
		std::cout << "a1   =   " <<  spidar.a1*(180.0/3.141592) << std::endl;
		std::cout << "a2   =   " <<  spidar.a2*(180.0/3.141592) << std::endl;
		std::cout << "a3   =   " <<  spidar.a3*(180.0/3.141592) << std::endl;
		std::cout << "a4   =   " <<  spidar.a4*(180.0/3.141592) << std::endl;
		std::cout << initPos[0].Z()-motorPos[0][0].Z() <<std::endl;
		std::cout << initPos[0].X()-motorPos[0][0].X() <<std::endl;
		std::cout << atan(-0.225/0.22)*(180.0/3.141592) <<std::endl;
		std::cout << std::endl;
*/
//		std::cout << "/****************�ʒu*****************/" << std::endl;
//		std::cout << "X= " <<  spidar.Xnew <<" , " << "Y=   " <<  spidar.Ynew << " , "<< "Z =   " <<  spidar.Znew << std::endl;
//		std::cout << "X1= " <<  spidar.X1new <<" , " << "Y1=   " <<  spidar.Y1new << " , "<< "Z1 =   " <<  spidar.Z1new << std::endl;
//		std::cout << "X2= " <<  spidar.X2new <<" , " << "Y2=   " <<  spidar.Y2new << " , "<< "Z2 =   " <<  spidar.Z2new << std::endl;
//		std::cout << "P1(x)= " <<  spidar.Xnew+spidar.X1new <<" , " << "P1(y)=   " <<  spidar.Ynew+spidar.Y1new << " , "<< "P1(z) =   " <<  spidar.Znew.spidar.Z1new << std::endl;
//		std::cout << "X2= " <<  spidar.X2new <<" , " << "Y2=   " <<  spidar.Y2new << " , "<< "Z2 =   " <<  spidar.Z2new << std::endl;
		
		spidar.Update();
/*
		float rad = 0.25;
		Vec3f colde;
		Vec3f force; 

		Affinef graphic = Affinef(0, 0, -0.5);

		colde = graphic.Pos() - spidar.GetPos(4); 
						
		spidar.Update();
		if (rad - colde.norm() > 0){
			force = -K * (rad - colde.norm()) * (graphic.Pos() - spidar.GetPos(4)).unit();
			}
			spidar.SetForce(force, 0.1f);
*/
	}


end:;
}


