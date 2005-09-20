#include "His/HISpidar4.h"
//	USB2.0�A���v�̃h���C�o
#include <Device/DRUsb20Simple.h>
#include <Device/DRUsb20Sh4.h>
//	Contect��ISA�J�[�h�̃h���C�o�D�Â�����(1997-)�D
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>

#include <conio.h>
#include <iostream>

using namespace Spr;	//	Spr��Springhead���C�u�����̖��O���

void main(){
	//------------------------------------------------------------------------
	//	����������
	//
	DVDeviceManager devMan;		//	�f�o�C�X�h���C�o�����Ă������ꕨ
	devMan.RPool().Register(new DRUsb20Simple(10));	//	USB2.0�A���v(ID 10)��o�^
	devMan.RPool().Register(new DRUsb20Sh4(0));		//	SH4�A���v(ID 0)��o�^
	/*	�g���Ă��Ȃ��h���C�o�D�o�^���Ă����Ύ����F������D
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	*/
	/*	�h���C�o�̏������D���݂���f�o�C�X�ɑΉ�����o�[�`�����f�o�C�X��
		�����ŏ����������	*/
	devMan.Init();
	std::cout << devMan;	//	�f�o�C�X�̏�Ԃ̕\��

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

		// 2003.10 NEW SETTING
	/*Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};*/

	spidar.Init(devMan, motorPos[0], 0.3776f, 2.924062107079e-5f, 0.5f, 20.0f);
	spidar2.Init(devMan, motorPos[1], 0.3776f, -2.924062107079e-5f, 0.5f, 20.0f);
	
	

	//------------------------------------------------------------------------
	//	���C�����[�v
	//
	bool bMinForce = true;	//	�����Ȃ�͂��o���Ɗ댯�Ȃ̂ŁC�X�y�[�X�������܂ł͍ŏ����͂��o��
	while(1){
		if (kbhit()){		//	�L�[����
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
		//------------------------------------------------------------------------
//			static float pos_x_buf1=0,pos_x_buf2=0,pos_x_buf3=0;

		//------------------------------------------------------------------------

		spidar.Update(0.001f);				//	�A���v�Ƃ̒ʐM(�͂�񎦁E�ʒu���擾)
		Vec3f force;					//	�񎦂����
		const float K = 20.0f;		//	�΂˒萔 2000->20
		if (spidar.GetPos().Y() < 0){	//	���̒�
			force.Y() = - K * spidar.GetPos().Y();
		}
		if (force.norm() > 3){			//	�傫������͂͒񎦂ł��Ȃ��̂ŁC3N(10N)�ɐ���
			force = force.unit() * 3;
		}
		//------------------------------------------------------------------------
/*			pos_x_buf3=pos_x_buf2;
			pos_x_buf2=pos_x_buf1;
			pos_x_buf1=spidar.GetPos().X();
			std::cout << "1:" << pos_x_buf1 << "\t2:" << pos_x_buf2 << "\t3:" << pos_x_buf3 << std::endl;

			force.X() = -5.0f*pos_x_buf2-pos_x_buf1-pos_x_buf3;
*/		//------------------------------------------------------------------------
		//	�񎦗͂̐ݒ�
		if (bMinForce) spidar.SetMinForce();
		else spidar.SetForce(force);

		//	�ȉ�2�Ԗڂ�SPIDAR�ɂ��ē��l�ɁC
		Vec3f force2;
		spidar2.Update(1); //1khz 0.001f

		if (spidar2.GetPos().Y() < 0){
			force2.Y() = - K * spidar2.GetPos().Y();
		}
		if (bMinForce) spidar2.SetMinForce();
		else spidar2.SetForce(force2);
		

	
		//	�f�o�b�O�p�F�ʒu�Ɨ͂�\��
		std::cout << "P:" << spidar.GetPos() << "\tF:" << spidar.GetForce() << std::endl;

		//	�f�o�b�O�p�F���̒�����\��
/*
		for(int i=0; i<4; i++){
			std::cout << spidar.Motor()[i].GetLength() << " ";
		}
		for(int i=0; i<4; i++){
			std::cout << spidar2.Motor()[i].GetLength() << " ";
		}
		std::cout << std::endl;
*/
} 
end:;
}
