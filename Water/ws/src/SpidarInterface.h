#ifndef SPIDARINTERFACE_H
#define SPIDARINTERFACE_H

#ifdef __cplusplus
extern "C"{
#endif

///	SPIDAR�̏������C���̊֐����ĂԑO��1�x�Ăяo���Ă��������D
void SPInit();
void SPCalib();

///	SPIDAR�̍X�V�D�ʒu���v�Z���ė͂�񎦂��܂��D
void SPUpdate();

///	�񎦗͂̐ݒ�DUpdate()���ĂԂ܂ł͔��f����܂���D
void SPSetForce(float x, float y, float z);
///	�񎦃g���N�̐ݒ�DUpdate()���ĂԂ܂ł͔��f����܂���D
void SPSetTorque(float x, float y, float z);

///	�O���b�v�ʒu�̎擾�D�Ō��Update()���Ă񂾂Ƃ��̃f�[�^�ł��D
float* SPGetPosition();
///	�O���b�v�ʒu�̎擾�D�Ō��Update()���Ă񂾂Ƃ��̃f�[�^�ł��D
float* SPGetOrientation();	//	Quaternion�`���ifloat [4]�j�ŕԂ��܂��D
float* SPGetRotation();		//	Matrix�`�� (float[9])�ŕԂ��܂��D

#ifdef __cplusplus
}
#endif


#endif
