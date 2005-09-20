#include "mySoftShadowMapVH.h"



CMySoftShadowMap::CMySoftShadowMap(void){
	//色んな変数の初期化などはコンストラクタで、
	//テクスチャ、Cgなどの初期化はinitShadowでやっています。

	//グラフィックボードが使える、マルチテクスチャの最大枚数を取得する。
	int maxTexNum = 0;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxTexNum);
	cout << "maxTexNum = " << maxTexNum << endl;

	//シャドウマップ用のテクスチャ設定をしたいので、ステージを変えておく
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

	//テクスチャ、Cgなどの初期化はinitShadowでやっています。
	initShadow();

	//シャドウマップ用のテクスチャの設定が終わったので、
	//テクスチャステージを０に戻す
	glActiveTextureARB(GL_TEXTURE0_ARB);
}

//デストラクタ。テクスチャオブジェクトの消去を行う
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

	 
	//テクスチャの初期化	///////////////////////////////////////
	
	glEnable(GL_TEXTURE_2D);								
	glGenTextures(TEXNUM,smTexture);						

	for(int i = 0; i<=TEXNUM - 1; i++){
		glBindTexture(GL_TEXTURE_2D, smTexture[i]);
		//これに指定しないと、テクスチャ張っても、色が反映されない
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_ARB);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_ARB);			
		//GL_DECALの他にGL_MODULATEとGL_BLENDがある
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);					
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);					

	}

	glDisable(GL_TEXTURE_2D);

	

	//１次元てくすちゃ　////////////////////////////////////////////////

	glEnable(GL_TEXTURE_1D);
	glGenTextures(1 , &tex1d);
	glBindTexture(GL_TEXTURE_1D , tex1d);
	//これに指定しないと、テクスチャ張っても、色が反映されない
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);		
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);		

	//１次元テクスチャを作る
	make1dTexture();
	glTexImage1D(
		GL_TEXTURE_1D , 0 , GL_RGBA , TEXSIZE , 
		0 , GL_RGBA , GL_FLOAT , bits
		);



	glDisable(GL_TEXTURE_1D);
}



//①まずライトの視点からシーンを描画
//②深度をテクスチャにコピー。その時、ViewPortをシャドウテクスチャの大きさにすれば、
//readpixel copyteximage2d ともにshadowtexのwidth heightの領域を読めばよくなる
//③射影テクスチャ行列を作成
//④
//なお、調べた結果、glcopyなどでテクスチャを作っているところが、一番時間かかっている。
//ここをなくすだけで、250fps違う
void CMySoftShadowMap::makeSceneWithShadow(void)
{
	GLenum error;
	//光源からの視錘台を再定義してあげる
	setLightView(eyeView);
	depthSave = (float *)realloc(depthSave, (*winWidth) * (*winHeight) * sizeof(float));

	modeChoice = 0;


	if(*modeChoice == COLOR){
		glutSetWindowTitle("通常モード（影なし）");
		renderScene2();

	}
	else if(*modeChoice == SHADOWMAPPING){
		glutSetWindowTitle("ハードシャドウ");
	
		


		//ライトの視点からシーンを描画し、その結果の深度値を０番のテクスチャに格納
		renderSceneFromLightView();
		copySceneDepthToTexture(0);

		glViewport(0, 0, *winWidth, *winHeight);

		//視点と視野を、戻しておく
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(eyeView[0], eyeView[1], eyeView[2], eyeView[3]);		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eyePos[0], eyePos[1], eyePos[2],  0, 0, 0,   0, 1, 0);		
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//深度値の比較関数の設定
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

//ライトの視点からシーンを描画する前の設定
void CMySoftShadowMap::makeSceneWithShadow1(void){
	//光源からの視錘台を再定義してあげる
	setLightView(eyeView);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//影のちらつきが出るときは、このオフセットの値を適当にべし。
	//描くオブジェクトによって可変。
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

//１と２の間で描画処理だけをする関数を呼ぶ

//２では、オフセットの設定を戻して、光源からの描画結果をテクスチャに保存
void CMySoftShadowMap::makeSceneWithShadow2(void){
	glDisable(GL_POLYGON_OFFSET_FILL);

	//その結果の深度値を０番のテクスチャに格納
	//シャドウマップでテクスチャ１を使うので、その設定にしておく
	glActiveTextureARB(GL_TEXTURE1_ARB);

		copySceneDepthToTexture(0);

	//シャドウマップ用のテクスチャを使い終わったので、
	//テクスチャステージを０に戻す
	glActiveTextureARB(GL_TEXTURE0_ARB);


	glViewport(0, 0, *winWidth, *winHeight);
}

//２と３の間で、プロジェクション変換、モデルビュー変換の設定を元に戻す

//３では、シャドウマッピングのために、ｇｌの拡張命令の設定をし、射影マッピング用の
//テクスチャ行列を用意
void CMySoftShadowMap::makeSceneWithShadow3(void){
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//シャドウマップでテクスチャ１を使うので、その設定にしておく
	glActiveTextureARB(GL_TEXTURE1_ARB);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);			
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);


	//深度値の比較関数の設定
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D , smTexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                    GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,GL_GEQUAL);

	
		
	enableTexGen();
		makeProjectionMatrix();
		//本来は、ここでテクスチャ０の設定に戻してあげて、
		//VHプログラム側では、テクスチャ０に設定するのは、初期化の１回だけに
		//したいのだが、なぜかうまくいかない。
		//glActiveTextureARB(GL_TEXTURE0_ARB);
		
	
}

//３と４の間で再び描画処理だけをする関数を呼ぶ

//４では、後処理。
void CMySoftShadowMap::makeSceneWithShadow4(void){
	disableTexGen();
	glDisable(GL_TEXTURE_2D);
	//シャドウマップ用のテクスチャを使い終わったので、
	//テクスチャステージを０に戻す
	glActiveTextureARB(GL_TEXTURE0_ARB);



	GLenum error = glGetError();
	if(error)
		printf("GL Error: %s\n", gluErrorString(error));

}



//視点の位置からシーンを描画
//gluPerspectiveは、aspectだけを変えればいい
//もちろん、視点の位置を光源に移動。
//viewportはテクスチャのサイズだけあればよい。
void CMySoftShadowMap::renderSceneFromLightView(void){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//影のちらつきが出るときは、このオフセットの値を適当にべし。
	//描くオブジェクトによって可変。
	
	if(flagObject == 0)	//飛行機の時
		glPolygonOffset(1.5,7.0);
	else if(flagObject == 1)		//ボール３つの時
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


//射影テクスチャリングのためのテクスチャ座標を自動生成する部分
//①頂点の座標をテクスチャ座標にスケール、バイアスする
//②ライトビューの視野を定義
//③ライトの位置に視点を移動する
//以上でＯＫ！簡単だ～
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

	
	//テクスチャ行列の部分には、やはりシーン全体にかかるモデルビュー行列は全てかけなければ
	//いけないので、GLView.cppの方でこの後にrotate と translateを行っている
    glMatrixMode(GL_MODELVIEW);
	
}

//フレームバッファから、深度をi番のテクスチャに格納する関数
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

//フレームバッファから、色をi番のテクスチャに格納する関数
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

//射影テクスチャ行列を自動生成する為の設定
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


//１次元テクスチャ用の配列を定義している部分
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

//view行列を作る関数
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

//view行列の逆行列を作る関数
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

    //０～１にクランプしている
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

//ボールを３つ描画する関数
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

//地面を書く関数
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

//光源を描画する関数。まぁ光源がどこにあるかを確認する為です。
void CMySoftShadowMap::renderLight(void){
	//まずは原点を描画
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
	glutSolidSphere(15,10,10);
	
	//次に光源を描画
	glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);	
		glTranslatef(lightPos[0],lightPos[1],lightPos[2]);
		//glTranslatef(10,10,10);
		glutSolidSphere(15,10,10);
	glPopMatrix();


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                各種の値をコピーしてくる関数達
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
	

	//lightのnear と　farは、離れていないほうが深度値の精度が高まる
	lightView[0] = eyeView[0] + 45;
	//lightView[0] = eyeView[0] + 0;
	lightView[1] = ShadowTexWidth/ShadowTexHeight;
	lightView[2] = eyeView[2];
	lightView[3] = eyeView[3];

	//lightViewをポインタとして宣言して、この式を行うと、Access Violationになる。
	//なぜなら、ポインタのアドレスを初期化で設定してあげてないから。
	//実体を持たないポインタにいきなり値を入れようとしても無理っすー
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



//アフィン行列をコピーしてくる関数
void CMySoftShadowMap::copyAffineMatrix(Affinef afCamMatrix){
	afCam = afCamMatrix;
}


