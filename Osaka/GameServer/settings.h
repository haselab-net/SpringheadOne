#ifndef SETTINGS_H
#define SETTINGS_H
/*
 setting.h
 �Q�[���̏����l�Ȃǂ�ݒ肷��
 �ق��ɂ����ʂŗ��p�������l�Ȃǂ������痘�p���Ă�������
*/

#define NUM_PUCK	4	//Puck�̐�

//�J�n�ʒu
const float	lydown = 0.45f;		//���_���珰���ʂ܂ł̋���
const float ly = 0.75f;       //�V�V�䕽�ʂ܂ł̋���
const float lx = 0.65f;		//�V���[�܂ł̋���
const float lzfront = 0.55f;		//�V�O�ʂ܂ł̋���
const float lzback = 0.75f;		//�V��ʂ܂ł̋���

const float lyResetb = 0.65f;		//�y���M��(����)�������蔻����n�߂鋫�E��
const float lyReseth = 0.68f;		//�y���M��(����)�������蔻����n�߂鋫�E��

const Vec3f startPen1( 0.40f,-1.0*lydown+0.05, 0.15f);	//Penguin1�̏����ʒu
const Vec3f startPen2(-0.40f,-1.0*lydown+0.05,-1.00f);	//Penguin2�̏����ʒu
const Vec3f startPen3( 0.40f,-1.0*lydown+0.05,-1.50f);	//Penguin3�̏����ʒu

const Vec3f startPu1(-0.15f,-1.0f*lydown,-0.50f);	//Puck1�̏����ʒu
const Vec3f startPu2( 0.15f,-1.0f*lydown,-0.50f);	//Puck2�̏����ʒu
const Vec3f startPu3( 0.15f,-1.0f*lydown,-0.80f);	//Puck3�̏����ʒu
const Vec3f startPu4(-0.15f,-1.0f*lydown,-0.80f);	//Puck4�̏����ʒu

const float goalr = 0.30f;		//�S�[���̔��a

const float PenheadR = 0.10f;		//�y���M���̓����a
const float PenbodyR = 0.15f;		//�y���M���̓��̔��a
const float Penmass = 1.5f;			//�y���M���̏d��
const Vec3f PenheadPos = Vec3f(0,0.33,0);		//�y���M���̓����W  �ꂪ���_
const Vec3f PenbodyPos = Vec3f(0,0.15,0);		//�y���M���̓��̍��W

const float PuckheadR = 0.07f;		//Puck�������a
const float PuckbodyR = 0.10f;		//Puck���̕����a


// �T�E���h�֘A�̒萔�l
#define	MAXSNDPARAM			4.0	// �O���[�o���ϐ��̍ő�l���w��(���K���Ɏg�p)

/*
const Vec3f startPu1(-0.12f,floorY,-0.63f);	//Puck1�̏����ʒu
const Vec3f startPu2( 0.12f,floorY,-0.63f);	//Puck2�̏����ʒu
const Vec3f startPu3( 0.12f,floorY,-0.87f);	//Puck3�̏����ʒu
const Vec3f startPu4(-0.12f,floorY,-0.87f);	//Puck4�̏����ʒu
*/
//const Vec3f startPen1( 0.35f,-1.0*floorY+0.05, 0.35f);	//Penguin1�̏����ʒu
//const Vec3f startPen2(-0.35f,-floorY+0.05,-1.00f);	//Penguin2�̏����ʒu
//const Vec3f startPen3( 0.35f,-floorY+0.05,-1.10f);	//Penguin3�̏����ʒu

/* �͊o�ǂ����Ƃ��珑���Ă����l
	afPen1.Pos() = Vec3f(0.35,-0.4,0.35);
	afPen2.Pos() = Vec3f(-0.35,-0.4,-1.0);
	afPen3.Pos() = Vec3f(0.35,-0.4,-1.1);
	afPuck1.Pos() = Vec3f(-0.12,0,-0.63);
	afPuck2.Pos() = Vec3f(0.12,0,-0.63);
	afPuck3.Pos() = Vec3f(0.12,0,-0.87);
	afPuck4.Pos() = Vec3f(-0.12,0,-0.87);
*/
#endif