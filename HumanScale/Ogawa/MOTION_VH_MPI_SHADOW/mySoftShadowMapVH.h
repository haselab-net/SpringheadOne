#ifdef _WIN32				
#include <windows.h>		
#endif				
	
#include <iostream>	
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//���̒��Ŋg�����߂�S�ČĂ�ł�
#include <ATIExtensions.h>
//�W��������̃A�t�B���s��ɑΉ����邽��
#include "../LVRM/Base/Affine.h"
using namespace LVRM;


//#include "Cgc.h"

using namespace std;



//GL_ARB_depth_texture			 DEPTH_COMPONENT16_ARB�ADEPTH_TEXTURE_MODE_ARB�Ȃǂ̎���
//GL_ARB_shadow					�[�x�l�Ƌ����̔�r���鏈��������
//WGL_ARB_pixel_format�@		�s�N�Z���t�H�[�}�b�g�����߂���A�I�������肷�鎞
//GL_ARB_shadow_ambient         �s�N�Z���̔�r�֐���ς�����B

//�v���O�����ɂ͕K�v�Ȃ����A�Q�l�ɁB
//WGL_ARB_render_texture�@�J���[�o�b�t�@�������_�����O�ƃe�N�X�`�������O�Ɏg����B
//							pbuffer���g���炵��
//

#define TEXNUM 7
#define TEXSIZE 256

typedef enum {COLOR, SHADOWMAPPING, SOFTSHADOW, PROJECTIONTEXTURE, HARDSHADOWBYCG
				, SHOWDEPTH} ModeChoice;
typedef void (*methodPointer) (void);

class CMySoftShadowMap{
public :
	//�R���X�g���N�^�A�f�X�g���N�^
	CMySoftShadowMap(void);
	~CMySoftShadowMap(void);
	//�����o�֐�
	
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


	//�W��������p�ɒǉ������ϐ����֐�
	Affinef afCam;
	void copyAffineMatrix(Affinef afCam);
	//�`�揈�������̕���(�Ⴆ��renderScene()�Ƃ�)�̑O��ɁA
	//�ȉ��̂Q��u�����ƂŁAshadow��������
	void makeSceneWithShadow1(void);
	void makeSceneWithShadow2(void);
	void makeSceneWithShadow3(void);
	void makeSceneWithShadow4(void);


	int flagObject;


protected:
	//�����o�ϐ�
	GLint ShadowTexWidth, ShadowTexHeight;
	GLfloat *Xrot, *Yrot, *Zrot;
	ModeChoice *modeChoice;
	int *winWidth, *winHeight;
	float blackRatio;
	double t;						//�V���G�b�g�̊g�啝��
	GLuint tex1d;

	//�z��n
	GLfloat bits[TEXSIZE][4];			//�P�����e�N�X�`���p�z��
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


	//�֐��|�C���^�B����ɂ��Amain�̕��̊֐��������炩��Ăׂ�悤�ɂȂ�B
	void (*renderScene2)(void);


	//�N���X�̃����o�֐�����														
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