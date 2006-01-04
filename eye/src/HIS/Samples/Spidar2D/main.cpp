//#include "His/HISpidar4.h"
#include "HISpidar2.h"

//	USB2.0�A���v�̃h���C�o
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
#include <WinBasis/WBMMTimer.h>
//	Contect��ISA�J�[�h�̃h���C�o�D�Â�����(1997-)�D
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>

#include <conio.h>
#include <iostream>

using namespace Spr;	//	Spr��Springhead���C�u�����̖��O���

#include <windows.h>
#include "Graph.h"

// �O���[�o���ϐ� ----------------------------
//extern int x_target_point;
//extern int y_target_point;
// (main�p)�}�E�X�Ŏw�肵���ʒu
double X_M;
double Y_M;
// -------------------------------------------

bool bMinForce = true;	//	�����Ȃ�͂��o���Ɗ댯�Ȃ̂ŁC�X�y�[�X�������܂ł͍ŏ����͂��o��
void TimerFunc(void*);

//----------------------------------------------------------------------------------		
/*	4�{����SPIDAR.
		���̒���������W�v�Z���s������A�o�͂���͊o����e���[�^��
		�o�͂��v�Z�����肷��B�o�͂̌v�Z��2���v��@��p���Ă��邽�߁C
		HIS/Fortran �� libF77.lib libI77.lib QuadPR.lib ���K�v�D
	@verbatim
	SPIDAR�̒萔�̐ݒ��
	Init(dev, motPos, 0.5776, 2.924062107079e-5f, 0.5f, 20.0f);

	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	0.265f	//	x�����̕ӂ̒���/2
	#define PY	0.265f	//	y�����̕ӂ̒���/2
	#define PZ	0.265f	//	z�����̕ӂ̒���/2
	Vec3f motorPos[][4] =	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���){
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	//	�P�j���[�g���o�����߂�DA�̏o�͓d��[V/N]
	//	maxon:		0.008[m] / 0.0438[Nm/A] / 0.5[A/V] =  0.365296803653
	//	��t���@:	0.008[m] / 0.0277[Nm/A] / 0.5[A/V] =  0.5776
	//	maxon��35:	0.008[m] / 0.0389[Nm/A] / 0.5[A/V] =  0.411311053984

	sp->VoltPerNewton(0.5776f);

	//	�P�J�E���g�̒��� [m/plus]	�����l: 2.924062107079e-5f
	sp->LenPerPlus(2.924062107079e-5f);

	//	�ŏ����� [N]
	sp->MinForce(0.5f);

	//	�ő咣�� [N]
	sp->MaxForce(20.0f);
		@endverbatim
*/
//----------------------------------------------------------------------------------

void main(){

// ���������� -----------------------------------------------------------------------

	DVDeviceManager devMan;		//	�f�o�C�X�h���C�o�����Ă������ꕨ
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0�A���v(ID 10)��o�^
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	SH4�A���v(ID 0)��o�^
	/*	�g���Ă��Ȃ��h���C�o�D�o�^���Ă����Ύ����F������D
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	*/
	// �h���C�o�̏������D���݂���f�o�C�X�ɑΉ�����o�[�`�����f�o�C�X�������ŏ����������
	devMan.Init();
	std::cout << devMan;	//	�f�o�C�X�̏�Ԃ̕\��

	HISpidar2 spidar;

	//	SPIDAR�̏������D�{���́C�n�[�h�E�G�A���� HISpidar4 �N���X��h��������ׂ�
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	0.417f	//	x�����̕ӂ̒���/2
	#define PY	0.0000001f	//	y�����̕ӂ̒���/2
	#define PZ	0.325f	//	z�����̕ӂ̒���/2

	Vec3f motorPos[1][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		Vec3f(PX, -PY, -PZ),Vec3f( PX, PY,PZ),Vec3f(-PX,-PY,PZ),Vec3f( -PX,PY,-PZ)
	};

	spidar.Init(devMan, motorPos[0], 0.41131105f, 2.924062107079e-5f, 1.0f, 20.0f);
// ----------------------------------------------------------------------------------
	
//	�}���`���f�B�A�^�C�}�[�� TimerFunc ���ďo ---------------------------------------	
	WBMMTimer mmtimer;
	mmtimer.Set(TimerFunc, &spidar);
	mmtimer.Resolution(5);
	mmtimer.Interval(5);
	mmtimer.Create();

//	���C�����[�v --------------------------------------------------------------------
	while(1){
		// �`��̂��߂� X_object_point Y_object_point ��Graph�N���X�ɑ���
		X_object_point=coordinate_X_max/2+(int)(coordinate_X_max/2*spidar.GetPos().X()/PX);
		Y_object_point=coordinate_Y_max/2-(int)(coordinate_Y_max/2*spidar.GetPos().Z()/PZ);
		// �}�E�X�ʒu���v�Z�p�ɕϊ�
		X_M = ((double)X_mouse_pos/coordinate_X_max - 0.5f) * PX * 2;
		Y_M = -1*((double)Y_mouse_pos/coordinate_Y_max - 0.5f) * PZ * 2;
		// �ʒu�w��̂��߂̃_�C�A���O�\��
		static Graph p_graph;


		// �ڕW���W
	//	std::cout << "P_X:" <<spidar.GetPos().X() << "\tP_Y:" <<spidar.GetPos().Z() << std::endl;
		// �}�E�X���W
	//	std::cout << "P_X:" <<X_M    << "\tP_Y:" <<Y_M    << std::endl;
		
		if (kbhit())
		{		//	�L�[����
			int ch = getch();
			switch(ch)
			{
			case ' ':		//	�X�y�[�X�F�͊oON/OFF
				bMinForce = !bMinForce;
				break;
			case 'c':		//	�L�����u���[�V����
				spidar.Calib();
				break;
			case 'g':		//	�L�����u���[�V����
					{
						p_graph.Window_Create_Plot();
					break;
					}

			case 0x1b:		//	ESC:�I��
				goto end;
				break;
			}
		}	
#if 0
#endif	
	}
// ----------------------------------------------------------------------------------
	// �}���`���f�B�A�^�C�}�[���X�g�b�v
	mmtimer.Release();
end:;
}

//----------------------------------------------------------------------------------
// �}���`���f�B�A�^�C�}�[�ŌĂяo���֐� 
//----------------------------------------------------------------------------------
void TimerFunc(void* arg){
	HISpidar2& spidar = *(HISpidar2*)arg;
		spidar.Update(0.001f);			//	�A���v�Ƃ̒ʐM(�͂�񎦁E�ʒu���擾)
		Vec3f force;					//	�񎦂����
		const float K = 2000.0f;		//	�΂˒萔
		/*	
		if (spidar.GetPos().Z() < 0){	//	���̒�
			force.Z() = - K * spidar.GetPos().Z();
			std::cout << "P_Z:" << spidar.GetPos().Z() << "\tF_Z:" << spidar.GetForce().Z() << std::endl;

		}
		*/

		//--------------------------------------------------------------------------
		//        ���x�E�����x�����߁C�΂˒萔�E�S���萔���Z�b�g����
		//--------------------------------------------------------------------------
		// ���݂̈ʒu�̍�
		static double X_difference = 0;
		static double Y_difference = 0;
		// ���݂̑��x�̍�
		static double V_X_difference = 0;
		static double V_Y_difference = 0;
		// ���x�̍������߂邽�߂̈ʒu�̍����c���Ă���
		double X_difference_buf = X_difference;
		double Y_difference_buf = Y_difference;

		X_difference = X_M - spidar.GetPos().X();
		Y_difference = Y_M - spidar.GetPos().Z();

		V_X_difference = X_difference - X_difference_buf;
		V_Y_difference = Y_difference - Y_difference_buf;

		// �΂˒萔�E�S���萔�ݒ�
			force.X()= 100 * spring_modulus * X_difference
				      +100 * damper_modulus * V_X_difference;

			force.Z()= 100 * spring_modulus * Y_difference
				      +100 * damper_modulus * V_Y_difference;
		//--------------------------------------------------------------------------
		if (force.norm() > 10){			//	�傫������͂͒񎦂ł��Ȃ��̂ŁC10N�ɐ���
			force = force.unit() * 10;
		}

		//	�񎦗͂̐ݒ�
		if (bMinForce) spidar.SetMinForce();
		//else spidar.SetForce(force,0.0000001f);
		else spidar.SetForce(force);

		/*
		static double L0_buf = spidar.motor[0].GetLength();
			   double L0     = spidar.motor[0].GetLength();
			   double L0_difference = L0 - L0_buf;	
			   L0_buf = L0;
		
		spidar.motor[0].SetForce(0.0f);
		spidar.motor[1].SetForce(0.0f);
		spidar.motor[2].SetForce(0.0f);
		spidar.motor[3].SetForce(0.0f);
			 
		//if(spidar.tension[0]<2.0f)	spidar.tension[0] += L0_difference;
		std::cout << "P_0:" << spidar.tension[0] << "\t";
		spidar.motor[0].SetForce(spidar.tension[0] - 500 * L0_difference);

		std::cout << "P_0:" << spidar.tension[0] - 500 * L0_difference<< "\t";
		std::cout << "P_L:" << L0_difference <<std::endl;
		*/
	
	// �S���␳ --------------------------------------------------------------------
	
		// ���݂̈ʒu �O���b�v�̈ʒu
		static double X_G = 0;
		static double Y_G = 0;
		// ���݂̑��x �O���b�v�̑��x
		static double V_X = 0;
		static double V_Y = 0;
		// ���x�����߂邽�߂̈ʒu���c���Ă���
		double X_G_buf = X_G;
		double Y_G_buf = Y_G;

		X_G = spidar.GetPos().X();
		Y_G = spidar.GetPos().Z();

		V_X = X_G - X_G_buf;
		V_Y = Y_G - Y_G_buf;

		// �O���b�v����e���[�^�ւ̈ʒu�����x�N�g��
		static double X_D[4] ={0}; 
		static double Y_D[4] ={0}; 
		// ������ ---------------------------
			//���[�^0 
			X_D[0] = PX - X_G;
			Y_D[0] = Y_G - PZ;
			//���[�^1
			X_D[1] = PX - X_G;
			Y_D[1] = PZ - Y_G;
			//���[�^2
			X_D[2] = X_G - PX;
			Y_D[2] = PZ - Y_G;
			//���[�^3
			X_D[3] = X_G - PX;
			Y_D[3] = Y_G - PZ;
		// ----------------------------------

		// �O���b�v����e���[�^�ւ̑��x�����x�N�g��
		static double V_X_D[4] ={0}; 
		static double V_Y_D[4] ={0};
		double P_convention[4];

		double aid_input[4];		//�S���␳������
		double aid_input_X[4];		//aid_input ��X����
		double aid_input_Y[4];		//aid_input ��Y����


		for(int i=0;i<4;i++)
		{
			P_convention[i] = ( X_D[i] * V_X + Y_D[i] * V_Y ) / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);
			V_X_D[i] =P_convention[i] * X_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);
			V_Y_D[i] =P_convention[i] * Y_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);

			// ���[�^��]���x����␳���͂ւ̕ϊ�
			//aid_input[i] = 50 * P_convention[i] + 0.5f;
			//aid_input[i] = 835.04 * P_convention[i] + 0.258553; //�v�Z�������
			//aid_input[i] = 150 * P_convention[i] + 1.0f;		  //���傤�ǂ������
			aid_input[i] = 0;									  //�␳���Ȃ����
		
			aid_input_X[i] = aid_input[i] * X_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);
			aid_input_Y[i] = aid_input[i] * Y_D[i] / sqrt(X_D[i] * X_D[i] + Y_D[i] * Y_D[i]);		
		}
		
		
		int can_not_offset_number[2];	//��]�����ɂ��␳�ł��Ȃ����[�^�ԍ�
		for(int i=0;i<4;i++)
		{
			static int j=0;
			if(P_convention[i]<0) //���[�^�̉�]���� �}�C�i�X
			{
				can_not_offset_number[j] = i;
				j=(j+1)%2; // 0 or 1�����ԂɎg��
			}
		}
		for(int i=0;i<4;i++)
		{
			if(P_convention[i]>0) //���[�^�̉�]���� �v���X
			{
				for(int j=0;j<2;j++)
				{
					int k = can_not_offset_number[j]; //�␳�ł��Ȃ����[�^�ԍ�
					aid_input[k] = ( aid_input_X[i] * aid_input_X[k] + aid_input_Y[i] + aid_input_Y[k]) 
									/ sqrt( aid_input_X[i] * aid_input_X[i] + aid_input_Y[i] + aid_input_Y[i]);					
				}
				for(int j=0;j<2;j++)
				{
					//aid_input[i] = aid_input[i] - aid_input[can_not_offset_number[j]];  
				}
				spidar.motor[i].SetForce(spidar.tension[i] + aid_input[i] );

			}
		}
		/*
		int i = 1;	
		static double L0_buf[4]={spidar.motor[0].GetLength(),
								 spidar.motor[1].GetLength(),
								 spidar.motor[2].GetLength(),
								 spidar.motor[3].GetLength()};
		double L0[4];
		double L0_difference[4];
		for(int i=0;i<4;i++)
		{
			L0[i]     = spidar.motor[i].GetLength();
			L0_difference[i] = L0[i] - L0_buf[i];	
			L0_buf[i] = L0[i];

			if(L0_difference<0)
			{
				double aid_input = 835.04 * (-1) * L0_difference[i] + 0.258553;
				spidar.motor[i].SetForce(spidar.tension[i] + aid_input);
			}
		}
		*/

	//	std::cout << "P :" <<  L0_difference[1] <<"\t"<<"\t";		
	//	std::cout << "P :" << V_X_D[i] <<"\t"<<"\t";
	//	std::cout << "P :" << L0_difference + V_X_D[i] <<std::endl;
		std::cout << "P :" <<  spidar.tension[1]  <<std::endl;

}
