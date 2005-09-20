#ifdef _WIN32				
#include <windows.h>		
#endif				
	
#include <iostream>	
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//この中で拡張命令を全て呼んでる
#include <ATIExtensions.h>
//ジョンさんのアフィン行列に対応するため
#include "../LVRM/Base/Affine.h"
using namespace LVRM;


//#include "Cgc.h"

using namespace std;



//GL_ARB_depth_texture			 DEPTH_COMPONENT16_ARB、DEPTH_TEXTURE_MODE_ARBなどの実装
//GL_ARB_shadow					深度値と距離の比較する処理を実装
//WGL_ARB_pixel_format　		ピクセルフォーマットを求めたり、選択したりする時
//GL_ARB_shadow_ambient         ピクセルの比較関数を変えられる。

//プログラムには必要ないが、参考に。
//WGL_ARB_render_texture　カラーバッファをレンダリングとテクスチャリングに使える。
//							pbufferを使うらしい
//

#define TEXNUM 7
#define TEXSIZE 256

typedef enum {COLOR, SHADOWMAPPING, SOFTSHADOW, PROJECTIONTEXTURE, HARDSHADOWBYCG
				, SHOWDEPTH} ModeChoice;
typedef void (*methodPointer) (void);

class CMySoftShadowMap{
public :
	//コンストラクタ、デストラクタ
	CMySoftShadowMap(void);
	~CMySoftShadowMap(void);
	//メンバ関数
	
	void makeSceneWithShadow(void);										
	void setMode(ModeChoice mode);
	void setWindowSize(int *w, int *h);
	void copyEyePos(GLfloat *eyepos_array);
	void copyLightPos(GLfloat *lightpos_array);
	void copyEyeView(GLfloat *eyeview_array);
	void copyRot(float *x, float *y, float *z);
	void copyMethodPointer(methodPointer mp);
	void setMode(ModeChoice *mode);
	void make1dTexture(void);
	void renderLight(void);


	//ジョンさん用に追加した変数＆関数
	Affinef afCam;
	void copyAffineMatrix(Affinef afCam);
	//描画処理だけの部分(例えばrenderScene()とか)の前後に、
	//以下の２つを置くことで、shadowをかける
	void makeSceneWithShadow1(void);
	void makeSceneWithShadow2(void);
	void makeSceneWithShadow3(void);
	void makeSceneWithShadow4(void);


	int flagObject;


protected:
	//メンバ変数
	GLint ShadowTexWidth, ShadowTexHeight;
	GLfloat *Xrot, *Yrot, *Zrot;
	ModeChoice *modeChoice;
	int *winWidth, *winHeight;
	float blackRatio;
	double t;						//シルエットの拡大幅ｔ
	GLuint tex1d;

	//配列系
	GLfloat bits[TEXSIZE][4];			//１次元テクスチャ用配列
	float red[4];
	float green[4];
	float lightView[4];
	GLuint smTexture[TEXNUM];
	GLfloat *lightPos;
	GLfloat *eyeView;
	GLfloat *eyePos;
	bool copyToTextureDone[TEXNUM];
	float invEyeViewMatrix[16];
	float 	*depthSave;


	//関数ポインタ。これにより、mainの方の関数をこちらから呼べるようになる。
	void (*renderScene2)(void);


	//クラスのメンバ関数たち														
	void initShadow(void);
	void renderSceneFromLightView(void);
	void makeProjectionMatrix(void);
	void copySceneDepthToTexture(int i);
	void copySceneColorToTexture(int i);
	void setLightView(GLfloat *eyeView);
	void makeViewMatrixAtLightView(float viewMatrix[16]);
	void enableTexGen(void);
	void disableTexGen(void);
	void makeInverseEyeViewMatrix(void);
	void myReadPixels(void);
	void drawGround(void);
	void drawBalls(void);




	
private:
	
};