// mkurahashi wrote... /////
#ifndef screen_H
#define screen_H

#include <math.h>
#include <GL/glut.h>
#include "GLView.h"
#include "TwoPassRender.h"

using namespace LVRM;
using namespace std;

class DrawScreen{


	GLuint d_vision;
	int accuracy; // �X�N���[���`��̐��m��
	float accQ1, accQ2, accQ3;
	float a;
	Vec3d rotCenter; 
	// �v���W�F�N�^�̕����x�N�g���ƃX�N���[���Ƃ̌�_���W

	void panel1(void); // ����
	void panel2(void); // ����
	void upper_cylinder_parts(float r, float length); // ��
	void lower_cylinder_parts(float r, float length); // ��
	void right_cylinder_parts(float r, float length); // �E
	void left_cylinder_parts(float r, float length); // ��
	void right_sphere_parts(float r); // �E��
	void left_sphere_parts(float r); // ����
	void circle_parts1(int r1); // �E��
	void doughnut_parts1(int r1, int r2);
	void circle_parts2(int r1); // ����
	void doughnut_parts2(int r1, int r2);

public:
	DrawScreen(); // �R���X�g���N�^
	void makeDList(void);
	void create_screen(void);
	Vec3d GetRotCenter(double * cameraParam, int number);
	int count;
};
#endif 

// Some <math.h> files do not define M_PI... //
#ifndef M_PI 
#define M_PI 3.1415926 
#endif 
