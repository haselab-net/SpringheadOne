// Ball.h: CBall �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#ifndef _CBALL_H
#define _CBALL_H

#include <GL/glut.h>
#include "../LVRM/Base/Affine.h"
#include "mymath.h"

using namespace LVRM;
//chk_collision
#define NOCOLLISION 0
#define COLLISION 1
//chk_shoot
#define NOSHOOT 0
#define SHOOTED 1
#define REFLEXED -1
//chk_state
#define PAUSE 0		
#define HALFHOLD 1
#define HOLDING 2
#define FALLING 3
#define THROWING 4
#define RETURNING 5
#define CATCHING 6
#define REPULSE 7

const GLfloat deltaRot = 5.0f;
const GLfloat nRange = 25.0f; 
const GLfloat nHeight = 45.0f; //45.0
const GLfloat delta_Pad = 0.5f;
const GLfloat convert_inch = 39.37;
//const GLfloat convert_inch = 39.37 * 3;	// �H�쎺SPIDAR�p�X�P�[������
const GLfloat GnHeight = 2.5f; //

//SPIDAR-H 
#define LogNum 5

struct SpidarInfo {

	Vec3f user_hand[2];
    Vec3f grp_pos[2];
    //JEONG
	float  hand_rad; //s_rad ����
	double hand_dis;
	double speed;
	//�ΐ�	
	Vec3f ball_pos;			// �{�[���ʒu(��΍��W)
	/////// Info for HalfHoliding //////
	Vec3f initBallPosition;		// �{�[���ʒu(�G��Ă���肩��̑��΍��W)
	/////// Info for Holding //////
	float VBradius;				// ���z�{�[���̔��a
	Vec3f initInclination;		// �X���̏����l
	Vec3f initBallPole;			// �{�[���ʒu(�G��Ă���肩��̑��΋ɍ��W)
	Vec3f BallPosLog[LogNum];	// �{�[���ʒu(��΍��W)�̗���

};

class CBall  
{
public:
	CBall();
	virtual ~CBall();

    MyVector init_pos;
	MyVector shot_pos; 
    MyVector new_pos;
    MyVector b_force;
    MyVector init_vel;
	MyVector new_vel;

    GLfloat beta, angle;
	float ret_angle;
	float ret_beta;
    GLfloat radius;
    GLfloat mass;
	GLfloat bounce;
    GLfloat minY;
	GLfloat minZ;
    int chk_shoot;
	int chk_state;

	//SPIDAR-H �p
	Vec3f ball_spos;
    Vec3f BallPosLog[LogNum];
	// Human hand position when virtual human returns a ball:JEONG
    Vec3f ReturnHand;	// �\��
	Vec3f Fall_pos;		// Falling Ball positon

	MyVector Parabola(double alpha, double t);
	void RetAngle(double ref_vel, MyVector off);
	MyVector Reflect(double dt, MyVector off_v);
	MyVector Fall_Roll(double speed, double t);
	void collision_repulse();
	double WhatTime(double dist, double left_x, double right_x);
    void DrawSphere(float rad);
	void Draw();

	void DrawShadow(float hh, GLfloat mat[4][4]);
	void Shadow_ball(GLfloat mat[4][4]);
//PRIVATE
	void Init();

};


#endif 