// mkurahashi wrote... /////
#ifndef TwoPassRendering_H
#define TwoPassRendering_H

#include <Base/Affine.h>
#include <math.h>
#include <GL/glut.h>
#include "screen.h"

using namespace std;
using namespace LVRM;

class TPRender{
//	Vec2f local_eyedir; 

	SLCamera getSLCP;//ステレオラベリングカメラ
	Vec3f pos[2];

	Vec3d curr; // 視点の座標
	Vec3d rotCenter;
	// プロジェクタの方向ベクトルとスクリーンとの交点座標

	Vec3d vDef, vCur;
	Vec3d eDef, eCur;
	Vec3d vect;

	float znear, zfar, distance0;
	double angle;
	float xmin, xmax, ymin, ymax;
	Vec3d axis;

//	GLfloat firstEyePlaneS[4];
//	GLfloat firstEyePlaneT[4];
//	GLfloat firstEyePlaneR[4];
//	GLfloat firstEyePlaneQ[4];

	GLfloat textureXform[4][4];
	static GLuint texname[1];

	DrawScreen VMRoom;

	void loadTextureProjection(Affinef r0);
//	void loadTextureProjection(GLfloat m[16], Affinef r0);
	void projTexInit(double fov, int w, int h);
//	void projectiveTexture(void);
	void projectiveTexture(Affinef m);
	void drawTextureProjection(void);
	void matrixIdentity(GLfloat matrix[16]);
	void matrixTranspose(GLfloat matrix2[16], GLfloat matrix1[16]);
	void vertualInit(double fov, int w, int h);



public:

	// ytakeyama
	int isCenter;
	double fovCopy;
	int whichI;
	float IdirX, IdirY;
	//end
	
	float rate;
	TPRender(){isCenter = 0;}; // ytakeyama
	void initialize(double * cameraParam, int number);
	void TwoPassRend(int number, double fov, Affinef m, int w, int h);
//	void checkErrors(void);

	void testPattern(void);
	void makeDList(void);
	GLuint line;

	// 時間計測のため
	double past_time1;
	time_t   start1, finish1;
	int count1;

	// key入力のため
	Vec3f keyMove;
	Vec2f keyMove1;

	Vec3f SLCMove;//ytakeyama
	
	
};
#endif 

// Some <math.h> files do not define M_PI... //
#ifndef M_PI 
#define M_PI 3.1415926 
#endif 
