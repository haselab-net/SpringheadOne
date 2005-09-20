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
	int accuracy; // スクリーン形状の正確さ
	float accQ1, accQ2, accQ3;
	float a;
	Vec3d rotCenter; 
	// プロジェクタの方向ベクトルとスクリーンとの交点座標

	void panel1(void); // 正面
	void panel2(void); // 床面
	void upper_cylinder_parts(float r, float length); // 上
	void lower_cylinder_parts(float r, float length); // 下
	void right_cylinder_parts(float r, float length); // 右
	void left_cylinder_parts(float r, float length); // 左
	void right_sphere_parts(float r); // 右上
	void left_sphere_parts(float r); // 左上
	void circle_parts1(int r1); // 右下
	void doughnut_parts1(int r1, int r2);
	void circle_parts2(int r1); // 左下
	void doughnut_parts2(int r1, int r2);

public:
	DrawScreen(); // コンストラクタ
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
