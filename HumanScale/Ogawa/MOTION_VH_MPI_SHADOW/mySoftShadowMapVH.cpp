#include "mySoftShadowMapVH.h"



CMySoftShadowMap::CMySoftShadowMap(void){
	//�F��ȕϐ��̏������Ȃǂ̓R���X�g���N�^�ŁA
	//�e�N�X�`���ACg�Ȃǂ̏�������initShadow�ł���Ă��܂��B

	//�O���t�B�b�N�{�[�h���g����A�}���`�e�N�X�`���̍ő喇�����擾����B
	int maxTexNum = 0;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTexNum);
	cout << "maxTexNum = " << maxTexNum << endl;

	//�V���h�E�}�b�v�p�̃e�N�X�`���ݒ���������̂ŁA�X�e�[�W��ς��Ă���
	glActiveTextureARB(GL_TEXTURE1_ARB);
	


	//ShadowTexWidth = ShadowTexHeight = 512;
	ShadowTexWidth = ShadowTexHeight = 1024;
	
	/*
	red[0] = 1;
	red[1] = 0;
	red[2] = 0;
	red[3] = 1;
	*/

	red[0] = 0.8;
	red[1] = 0.8;
	red[2] = 0.8;
	red[3] = 1;
	

	green[0] = 0;
	green[1] = 1;
	green[2] = 0;
	green[3] = 1;

	

	for(int i = 0; i < TEXNUM; i++){
		copyToTextureDone[i] = false;
	}

	t = 1.13;
	blackRatio = 0.500;
	

	flagObject = 0;
	depthSave = NULL;		

	//�e�N�X�`���ACg�Ȃǂ̏�������initShadow�ł���Ă��܂��B
	initShadow();

	//�V���h�E�}�b�v�p�̃e�N�X�`���̐ݒ肪�I������̂ŁA
	//�e�N�X�`���X�e�[�W���O�ɖ߂�
	glActiveTextureARB(GL_TEXTURE0_ARB);
}

//�f�X�g���N�^�B�e�N�X�`���I�u�W�F�N�g�̏������s��
CMySoftShadowMap::~CMySoftShadowMap(void){
	glDeleteTextures(TEXNUM , smTexture);

}


void CMySoftShadowMap::initShadow(void)
{
	/*
	cout << glGetString(GL_EXTENSIONS) << endl;						
	cerr << "OpenGL version: " << glGetString(GL_VERSION) << endl;	
	if (!glutExtensionSupported("GL_ARB_multitexture"))
		printf("I require the GL_ARB_multitexture OpenGL extension to work.\n");
	if (!glutExtensionSupported("GL_ARB_depth_texture"))
		printf("I require the GL_ARB_depth_texture OpenGL extension to work.\n");
	if (!glutExtensionSupported("GL_ARB_shadow"))
		printf("I require the GL_ARB_shadow OpenGL extension to work.\n");
	if(!glutExtensionSupported("WGL_ARB_pixel_format"))
		printf("I require the WGL_ARB_pixel_format OpenGL extension to work.\n");
	*/


    glEnable(GL_NORMALIZE);

	 
	//�e�N�X�`���̏�����	///////////////////////////////////////
	
	glEnable(GL_TEXTURE_2D);								
	glGenTextures(TEXNUM,smTexture);						

	for(int i = 0; i<=TEXNUM - 1; i++){
		glBindTexture(GL_TEXTURE_2D, smTexture[i]);
		//����Ɏw�肵�Ȃ��ƁA�e�N�X�`�������Ă��A�F�����f����Ȃ�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_ARB);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_ARB);			
		//GL_DECAL�̑���GL_MODULATE��GL_BLEND������
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);					
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);					

	}

	glDisable(GL_TEXTURE_2D);

	

	//�P�����Ă�������@////////////////////////////////////////////////

	glEnable(GL_TEXTURE_1D);
	glGenTextures(1 , &tex1d);
	glBindTexture(GL_TEXTURE_1D , tex1d);
	//����Ɏw�肵�Ȃ��ƁA�e�N�X�`�������Ă��A�F�����f����Ȃ�
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);		
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);		

	//�P�����e�N�X�`�������
	make1dTexture();
	glTexImage1D(
		GL_TEXTURE_1D , 0 , GL_RGBA , TEXSIZE , 
		0 , GL_RGBA , GL_FLOAT , bits
		);



	glDisable(GL_TEXTURE_1D);
}



//�@�܂����C�g�̎��_����V�[����`��
//�A�[�x���e�N�X�`���ɃR�s�[�B���̎��AViewPort���V���h�E�e�N�X�`���̑傫���ɂ���΁A
//readpixel copyteximage2d �Ƃ���shadowtex��width height�̗̈��ǂ߂΂悭�Ȃ�
//�B�ˉe�e�N�X�`���s����쐬
//�C
//�Ȃ��A���ׂ����ʁAglcopy�ȂǂŃe�N�X�`��������Ă���Ƃ��낪�A��Ԏ��Ԃ������Ă���B
//�������Ȃ��������ŁA250fps�Ⴄ
void CMySoftShadowMap::makeSceneWithShadow(void)
{
	GLenum error;
	//��������̎�������Ē�`���Ă�����
	setLightView(eyeView);
	depthSave = (float *)realloc(depthSave, (*winWidth) * (*winHeight) * sizeof(float));

	modeChoice = 0;


	if(*modeChoice == COLOR){
		glutSetWindowTitle("�ʏ탂�[�h�i�e�Ȃ��j");
		renderScene2();

	}
	else if(*modeChoice == SHADOWMAPPING){
		glutSetWindowTitle("�n�[�h�V���h�E");
	
		


		//���C�g�̎��_����V�[����`�悵�A���̌��ʂ̐[�x�l���O�Ԃ̃e�N�X�`���Ɋi�[
		renderSceneFromLightView();
		copySceneDepthToTexture(0);

		glViewport(0, 0, *winWidth, *winHeight);

		//���_�Ǝ�����A�߂��Ă���
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(eyeView[0], eyeView[1], eyeView[2], eyeView[3]);		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyePos[0], eyePos[1], eyePos[2],  0, 0, 0,   0, 1, 0);		
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//�[�x�l�̔�r�֐��̐ݒ�
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D , smTexture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                        GL_COMPARE_R_TO_TEXTURE_ARB);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,GL_LEQUAL);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);			
		
		enableTexGen();
			makeProjectionMatrix();
			renderScene2();
		disableTexGen();
		glDisable(GL_TEXTURE_2D);
		
		//glPopMatrix();


	}


   error = glGetError();
   if (error) {
      printf("GL Error: %s\n", gluErrorString(error));
   
   }

}

//���C�g�̎��_����V�[����`�悷��O�̐ݒ�
void CMySoftShadowMap::makeSceneWithShadow1(void){
	//��������̎�������Ē�`���Ă�����
	setLightView(eyeView);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//�e�̂�������o��Ƃ��́A���̃I�t�Z�b�g�̒l��K���ɂׂ��B
	//�`���I�u�W�F�N�g�ɂ���ĉρB
	glPolygonOffset(1.5,7.0);
		
	glEnable(GL_POLYGON_OFFSET_FILL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(lightView[0], lightView[1], lightView[2], lightView[3]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lightPos[0], lightPos[1], -lightPos[2], // from
		     0, 0, 0, //target
			 0, 1, 0); // up
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glViewport(0, 0, ShadowTexWidth, ShadowTexHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
}

//�P�ƂQ�̊Ԃŕ`�揈������������֐����Ă�

//�Q�ł́A�I�t�Z�b�g�̐ݒ��߂��āA��������̕`�挋�ʂ��e�N�X�`���ɕۑ�
void CMySoftShadowMap::makeSceneWithShadow2(void){
	glDisable(GL_POLYGON_OFFSET_FILL);

	//���̌��ʂ̐[�x�l���O�Ԃ̃e�N�X�`���Ɋi�[
	//�V���h�E�}�b�v�Ńe�N�X�`���P���g���̂ŁA���̐ݒ�ɂ��Ă���
	glActiveTextureARB(GL_TEXTURE1_ARB);

		copySceneDepthToTexture(0);

	//�V���h�E�}�b�v�p�̃e�N�X�`�����g���I������̂ŁA
	//�e�N�X�`���X�e�[�W���O�ɖ߂�
	glActiveTextureARB(GL_TEXTURE0_ARB);


	glViewport(0, 0, *winWidth, *winHeight);
}

//�Q�ƂR�̊ԂŁA�v���W�F�N�V�����ϊ��A���f���r���[�ϊ��̐ݒ�����ɖ߂�

//�R�ł́A�V���h�E�}�b�s���O�̂��߂ɁA�����̊g�����߂̐ݒ�����A�ˉe�}�b�s���O�p��
//�e�N�X�`���s���p��
void CMySoftShadowMap::makeSceneWithShadow3(void){
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//�V���h�E�}�b�v�Ńe�N�X�`���P���g���̂ŁA���̐ݒ�ɂ��Ă���
	glActiveTextureARB(GL_TEXTURE1_ARB);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);			
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


	//�[�x�l�̔�r�֐��̐ݒ�
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D , smTexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                    GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,GL_GEQUAL);

	
		
	enableTexGen();
		makeProjectionMatrix();
		//�{���́A�����Ńe�N�X�`���O�̐ݒ�ɖ߂��Ă����āA
		//VH�v���O�������ł́A�e�N�X�`���O�ɐݒ肷��̂́A�������̂P�񂾂���
		//�������̂����A�Ȃ������܂������Ȃ��B
		//glActiveTextureARB(GL_TEXTURE0_ARB);
		
	
}

//�R�ƂS�̊ԂōĂѕ`�揈������������֐����Ă�

//�S�ł́A�㏈���B
void CMySoftShadowMap::makeSceneWithShadow4(void){
	disableTexGen();
	glDisable(GL_TEXTURE_2D);
	//�V���h�E�}�b�v�p�̃e�N�X�`�����g���I������̂ŁA
	//�e�N�X�`���X�e�[�W���O�ɖ߂�
	glActiveTextureARB(GL_TEXTURE0_ARB);



	GLenum error = glGetError();
	if(error)
		printf("GL Error: %s\n", gluErrorString(error));

}



//���_�̈ʒu����V�[����`��
//gluPerspective�́Aaspect������ς���΂���
//�������A���_�̈ʒu�������Ɉړ��B
//viewport�̓e�N�X�`���̃T�C�Y��������΂悢�B
void CMySoftShadowMap::renderSceneFromLightView(void){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//�e�̂�������o��Ƃ��́A���̃I�t�Z�b�g�̒l��K���ɂׂ��B
	//�`���I�u�W�F�N�g�ɂ���ĉρB
	
	if(flagObject == 0)	//��s�@�̎�
		glPolygonOffset(1.5,7.0);
	else if(flagObject == 1)		//�{�[���R�̎�
		glPolygonOffset(3.1,9.0);
	else 
		glPolygonOffset(3.1,9.0);
	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(lightView[0], lightView[1], lightView[2], lightView[3]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lightPos[0], lightPos[1], lightPos[2], // from 
		     0, 0, 0, //target
			 0, 1, 0); // up
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);


	
	glViewport(0, 0, ShadowTexWidth, ShadowTexHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	renderScene2();

	glDisable(GL_POLYGON_OFFSET_FILL);

}


//�ˉe�e�N�X�`�������O�̂��߂̃e�N�X�`�����W�������������镔��
//�@���_�̍��W���e�N�X�`�����W�ɃX�P�[���A�o�C�A�X����
//�A���C�g�r���[�̎�����`
//�B���C�g�̈ʒu�Ɏ��_���ړ�����
//�ȏ�łn�j�I�ȒP���`
void CMySoftShadowMap::makeProjectionMatrix(void){
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.5, 0.5, 0.5);
	//glTranslatef(0.5, 0.5, -0.5);
	glScalef(0.5, 0.5, 0.5);
	gluPerspective(lightView[0], lightView[1], lightView[2], lightView[3]);

	gluLookAt(lightPos[0], lightPos[1], -lightPos[2], //from 
		     0, 0, 0, //target
			 0, 1, 0); // up

	
	//�e�N�X�`���s��̕����ɂ́A��͂�V�[���S�̂ɂ����郂�f���r���[�s��͑S�Ă����Ȃ����
	//�����Ȃ��̂ŁAGLView.cpp�̕��ł��̌��rotate �� translate���s���Ă���
    glMatrixMode(GL_MODELVIEW);
	
}

//�t���[���o�b�t�@����A�[�x��i�Ԃ̃e�N�X�`���Ɋi�[����֐�
void CMySoftShadowMap::copySceneDepthToTexture(int i){
		glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D , smTexture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
		if(copyToTextureDone[i] == false){
			glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32_ARB,0,0,ShadowTexWidth,ShadowTexHeight,0);
			copyToTextureDone[i] = true;
		}else
			glCopyTexSubImage2D(GL_TEXTURE_2D,0, 0,0,0,0,ShadowTexWidth,ShadowTexHeight);
		
		glDisable(GL_TEXTURE_2D);
}

//�t���[���o�b�t�@����A�F��i�Ԃ̃e�N�X�`���Ɋi�[����֐�
void CMySoftShadowMap::copySceneColorToTexture(int i){
		glEnable(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D , smTexture[i]);
		//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
		if(copyToTextureDone[i] == false){
			glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,0,0,ShadowTexWidth,ShadowTexHeight,0);
			copyToTextureDone[i] = true;
		}else
			glCopyTexSubImage2D(GL_TEXTURE_2D,0, 0,0,0,0,ShadowTexWidth,ShadowTexHeight);
		
		glDisable(GL_TEXTURE_2D);
}

//�ˉe�e�N�X�`���s���������������ׂ̐ݒ�
void CMySoftShadowMap::enableTexGen(){
	static GLfloat sPlane[4] = { 1, 0, 0, 0 };
	static GLfloat tPlane[4] = { 0, 1, 0, 0 };
	static GLfloat rPlane[4] = { 0, 0, 1, 0 };
	static GLfloat qPlane[4] = { 0, 0, 0, 1 };

	glTexGenfv(GL_S, GL_EYE_PLANE, sPlane);
    glTexGenfv(GL_T, GL_EYE_PLANE, tPlane);
    glTexGenfv(GL_R, GL_EYE_PLANE, rPlane);
    glTexGenfv(GL_Q, GL_EYE_PLANE, qPlane);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);
}

void CMySoftShadowMap::disableTexGen(){
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);	
	glDisable(GL_TEXTURE_GEN_Q);
}


//�P�����e�N�X�`���p�̔z����`���Ă��镔��
void CMySoftShadowMap::make1dTexture(void){
	for (int i = 0 ; i < TEXSIZE ; i++) {
		if(i < TEXSIZE * blackRatio){
			bits[i][0] = 0;
			bits[i][1] = 0;
			bits[i][2] = 0;
			bits[i][3] = 1;
		}
		else{
			bits[i][0] = 1;
			bits[i][1] = 1;
			bits[i][2] = 1;
			bits[i][3] = 1;		
		}

	}
}

//view�s������֐�
void CMySoftShadowMap::makeViewMatrixAtLightView(float viewMatrix[16]){
	viewMatrix[0] = 1;
	viewMatrix[1] =	0;	
	viewMatrix[2] = 0;
	viewMatrix[3] = -lightPos[0];
	viewMatrix[4] = 0;
	viewMatrix[5] = 1;
	viewMatrix[6] = 0;
	viewMatrix[7] = -lightPos[1];
	viewMatrix[8] = 0;
	viewMatrix[9] = 0;
	viewMatrix[10] = 1;
	viewMatrix[11] = -lightPos[2];
	viewMatrix[12] = 0;
	viewMatrix[13] = 0;
	viewMatrix[14] = 0;
	viewMatrix[15] = 1;
}

//view�s��̋t�s������֐�
void CMySoftShadowMap::makeInverseEyeViewMatrix(void){
	invEyeViewMatrix[0] = 1;
	invEyeViewMatrix[1] = 0;
	invEyeViewMatrix[2] = 0;
	invEyeViewMatrix[3] = eyePos[0];

	invEyeViewMatrix[4] = 0;
	invEyeViewMatrix[5] = 1;
	invEyeViewMatrix[6] = 0;
	invEyeViewMatrix[7] = eyePos[1];
	
	invEyeViewMatrix[8] = 0;
	invEyeViewMatrix[9] = 0;
	invEyeViewMatrix[10] = 1;
	invEyeViewMatrix[11] = eyePos[2];
	
	invEyeViewMatrix[12] = 0;
	invEyeViewMatrix[13] = 0;
	invEyeViewMatrix[14] = 0;
	invEyeViewMatrix[15] = 1;
}

void CMySoftShadowMap::myReadPixels(void){
	int		x, y;
    GLfloat	max, min;

	glReadPixels(0, 0, ShadowTexWidth, ShadowTexHeight, GL_DEPTH_COMPONENT,
				GL_FLOAT,depthSave);

    //�O�`�P�ɃN�����v���Ă���
    max = 0;
    min = 1;
    for(y = 0; y < ShadowTexHeight; y++)
	for(x = 0; x < ShadowTexWidth; x++) {
	    if(depthSave[ShadowTexWidth * y + x] < min)
		min = depthSave[ShadowTexWidth * y + x];
	    if(depthSave[ShadowTexWidth * y + x] > max && depthSave[ShadowTexWidth * y + x] < .999)
		max = depthSave[ShadowTexWidth * y + x];
	}

    for(y = 0; y < ShadowTexHeight; y++)
	for(x = 0; x < ShadowTexWidth; x++) {
	    if(depthSave[ShadowTexWidth * y + x] <= max)
		depthSave[ShadowTexWidth * y + x] =/*1-*/(depthSave[ShadowTexWidth * y + x] - min) / (max - min);
	    else
		depthSave[ShadowTexWidth * y + x] = 0;
	}

}

//�{�[�����R�`�悷��֐�
void CMySoftShadowMap::drawBalls(void){
	GLfloat k = 6;
	GLfloat Red[]    = {1, 0, 0, 1};
	GLfloat Green[]  = {0, 1, 0, 1};
	GLfloat Yellow[] = {1, 1, 0, 1};


   // sphere
   glPushMatrix();
   glTranslatef(1.6, 2.2, 2.7);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Green);
   glColor4fv(Green);
   glutSolidSphere(1.5, 15, 15);
   glPopMatrix();
   // dodecahedron
   glPushMatrix();
   glTranslatef(-2.0, 1.2, 2.1);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Red);
   glColor4fv(Red);
   glutSolidDodecahedron();
   glPopMatrix();
   // icosahedron 
   glPushMatrix();
   glTranslatef(-0.6, 1.3, -0.5);
   glScalef(1.5, 1.5, 1.5);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Yellow);
   glColor4fv(Red);
   glutSolidIcosahedron();
   glPopMatrix();

}

//�n�ʂ������֐�
void CMySoftShadowMap::drawGround(void)
{
    GLfloat groundColor[4] = {.647, .165, .165, 1};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, groundColor);

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3f(10, 0, 10);
    glVertex3f(10, 0, -10);
    glVertex3f(-10, 0, -10);
    glVertex3f(-10, 0, 10);
    glEnd();
}

//������`�悷��֐��B�܂��������ǂ��ɂ��邩���m�F����ׂł��B
void CMySoftShadowMap::renderLight(void){
	//�܂��͌��_��`��
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
	glutSolidSphere(15,10,10);
	
	//���Ɍ�����`��
	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);	
		glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
		//glTranslatef(10,10,10);
		glutSolidSphere(15,10,10);
	glPopMatrix();


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                �e��̒l���R�s�[���Ă���֐��B
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void CMySoftShadowMap::setMode(ModeChoice *mode){
	modeChoice = mode;
}

void CMySoftShadowMap::setWindowSize(int *w, int *h){
	winWidth = w;
	winHeight = h;
	
	//ShadowTexWidth = *w;
	//ShadowTexHeight= *h;
}


void CMySoftShadowMap::copyEyePos(GLfloat *eyepos_array){
	eyePos = eyepos_array;
}

void CMySoftShadowMap::copyLightPos(GLfloat *lightpos_array){
	lightPos = lightpos_array;
}

void CMySoftShadowMap::copyEyeView(GLfloat *eyeview_array){
	eyeView = eyeview_array;	
}

void CMySoftShadowMap::setLightView(GLfloat *eyeView){
	

	//light��near �Ɓ@far�́A����Ă��Ȃ��ق����[�x�l�̐��x�����܂�
	lightView[0] = eyeView[0] + 45;
	//lightView[0] = eyeView[0] + 0;
	lightView[1] = ShadowTexWidth/ShadowTexHeight;
	lightView[2] = eyeView[2];
	lightView[3] = eyeView[3];

	//lightView���|�C���^�Ƃ��Đ錾���āA���̎����s���ƁAAccess Violation�ɂȂ�B
	//�Ȃ��Ȃ�A�|�C���^�̃A�h���X���������Őݒ肵�Ă����ĂȂ�����B
	//���̂������Ȃ��|�C���^�ɂ����Ȃ�l�����悤�Ƃ��Ă����������[
	/*
    *lightView = eyeView[0] + 10;
	*(lightView+1) = ShadowTexWidth/ShadowTexHeight;
	*(lightView+2) = eyeView[2];
	*(lightView+3) = eyeView[3] + 40;
	*/
  }
void CMySoftShadowMap::copyRot(float *x, float *y, float *z){
	Xrot = x;
	Yrot = y;
	Zrot = z;
	
}

void CMySoftShadowMap::copyMethodPointer(methodPointer mp){
	renderScene2 = mp;
}



//�A�t�B���s����R�s�[���Ă���֐�
void CMySoftShadowMap::copyAffineMatrix(Affinef afCamMatrix){
	afCam = afCamMatrix;
}


