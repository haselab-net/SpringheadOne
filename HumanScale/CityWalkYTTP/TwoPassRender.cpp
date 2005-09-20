// mkurahashi wrote... /////

#include "GLView.h"
#include "screen.h"
#include "TwoPassRender.h"

using namespace std;
using namespace LVRM;

GLuint  TPRender::texname[1];

void TPRender::initialize(double * cameraParam, int number)
{

	switch(number){
	case 0:   case 1:   case 2:   case 3:
	case 15:  case 14:  case 17:  case 16:
	case 20:  case 30:  case 21:  case 31:
		whichI = 1;
		break;
	default:
		whichI = 2;
	}

//	pos[2] = Vec3f(0.0, 0.0, 0.0);
	SLCMove = Vec3f(0.0, 0.0, 0.0);
	keyMove = Vec3f(0.0, 0.0, 0.0);

//	float p = 0.1*tan(Rad(cameraParam[3]));
	// 始めの描画で使った画角をここでも用いる
	rate = 1.0; //HITACHIのカメラパラメータの設定の2倍の場合
	float p = 0.1*tan(Rad(cameraParam[3] * rate));

	double ww;
	switch (number){
	case 0: case 1: case 2: case 3:
	case 4: case 5: case 6: case 7:
		cout << "SXGA !" << endl;
		// 1280×1024画素（SXGA）用
		ww = p / 4;
		xmin = -5*ww, xmax = 11*ww;
		ymin = -4*ww, ymax = 4*ww;
		isCenter = 1;//ytakeyama
		break;
	default:
		cout << "XGA !" << endl;
		// 1024×768画素（XGA）用
		ww = p / 3;   // aspect比3:4
		xmin = -4*ww; xmax = 4*ww;
		ymin = -3*ww; ymax = 5*ww;
		break;
	}

	znear = 0.10;
	zfar = 50.0;
	distance0 = 0.0;

/////////////////////////////////////////////////////////


	// firstEyePlaneを初期化
	//glGetTexGenfv(GL_S,GL_EYE_PLANE, firstEyePlaneS); 
	//glGetTexGenfv(GL_T,GL_EYE_PLANE, firstEyePlaneT); 
	//glGetTexGenfv(GL_R,GL_EYE_PLANE, firstEyePlaneR); 
	//glGetTexGenfv(GL_Q,GL_EYE_PLANE, firstEyePlaneQ); 

	GLfloat eyePlaneS[] = { 1.0, 0.0, 0.0, 0.0 };
    GLfloat eyePlaneT[] = { 0.0, 1.0, 0.0, 0.0 };
    GLfloat eyePlaneR[] = { 0.0, 0.0, 1.0, 0.0 };
    GLfloat eyePlaneQ[] = { 0.0, 0.0, 0.0, 1.0 };
    //
    // Setup Misc.
    //

    glLineWidth(2.0);

    matrixIdentity((GLfloat *) textureXform);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i(0, 0);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

	

    //
    // Setup Texture
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_S, GL_EYE_PLANE, eyePlaneS);

    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_T, GL_EYE_PLANE, eyePlaneT);

    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_R, GL_EYE_PLANE, eyePlaneR);

    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    glTexGenfv(GL_Q, GL_EYE_PLANE, eyePlaneQ);

	
	VMRoom.makeDList();

	
}


void TPRender::vertualInit(double fov, int w, int h)
{

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (float)w/(float)h , 0.1f , 1000000.0f);

	glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
//	glEnable(GL_LIGHTING);
}
void TPRender::projTexInit(double fov, int w, int h)
{

//	glDisable(GL_LIGHTING);

	// Setup Lighting
	//GLfloat matAmb[4] = { 0.01, 0.01, 0.01, 1.00 };
    //GLfloat matDiff[4] = { 0.65, 0.65, 0.65, 1.00 };
    //GLfloat matSpec[4] = { 0.30, 0.30, 0.30, 1.00 };
    //GLfloat matShine = 10.0;

    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	//glEnable(GL_COLOR_MATERIAL);

	// 透視変換行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 日立製作所の設定　想定視点からのレンダリング
//	gluPerspective(fov, (float)w/(float)h , 0.1f , 100.0f);
	gluPerspective(fov /rate, (float)w/(float)h , 0.1f , 100.0f);

	//　モデルビュー変換行列の設定
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.2, 0.2, 0.2, 0.0);

	
}
//projectionするfrustumを決定
 void TPRender::loadTextureProjection(Affinef r0)
{
//    GLfloat mInverse[4][4];

    //
    // Should use true inverse, but since m consists only
    // of rotations, we can just use the transpose.
    //
//    matrixTranspose((GLfloat *) mInverse, m);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glTranslatef(0.5, 0.5, 0.0);
	// テクスチャの位置を変更している
    glScalef(0.5, 0.5, 1.0);
	// テクスチャの大きさを変更している

	//glFrustum(xmin, xmax, ymin, ymax, znear, zfar); // ytakeyama comment out
	//ytakeyama
	float aspect;
	if(isCenter){
		aspect = (float)1280/(float)1024;
	}else{
		aspect  = (float)1024/(float)768;
	}
	gluPerspective(fovCopy*1.5, aspect, 0.1 ,50.0);
	//end

	// 仮想プロジェクタの位置を移動する
	//keyMove = Vec3f(keyMove1[0], 0.0, keyMove1[1]);//ytakeyama comment out
		GLfloat r0Inverse[4][4];
		matrixTranspose((GLfloat *) r0Inverse, r0);

		// HITACHIのカメラパラメータの設定と座標軸を一致させる
		glMultMatrixf(r0);

		//ytakeyama
		if(whichI == 1){
			glTranslatef(-SLCMove[0]/* - 0.0325 * IdirY*/, -SLCMove[1], -SLCMove[2]/* - 0.0325 * IdirX*/);
		}else if(whichI == 2){
			glTranslatef(-SLCMove[0]/* + 0.0325 * IdirY*/, -SLCMove[1], -SLCMove[2]/* + 0.0325 * IdirX*/);
		}else{
			glTranslatef(-SLCMove[0], -SLCMove[1], -SLCMove[2]);
		}
		//cout << IdirY << endl;
		//glTranslatef(-SLCMove[0], -SLCMove[1], -SLCMove[2] + 0.9);
		//glTranslatef(-1.0, 0.0, 0.0);//1m=1unit
		//end

		glMultMatrixf((GLfloat *) r0Inverse);
//		cout << "SLCMove = " << SLCMove[0] << SLCMove[1] << SLCMove[2] << endl;
//    glMultMatrixf((GLfloat *) mInverse);
    glMatrixMode(GL_MODELVIEW);
}
void TPRender::projectiveTexture(Affinef r0)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	getSLCP.GetLabelPos(pos);

	loadTextureProjection( r0);

	// 光源の設定
	GLfloat light2Pos[4] = { 0.3, 0.3, 0.0, 1.0 };
    // 光源の位置設定
	glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);
    glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_GEN_Q);



	// rendering the objects for "2-pass rendering"
	VMRoom.create_screen();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_LIGHT2);
	glDisable(GL_LIGHTING);
	//  checkErrors();
	//	glDisable(GL_COLOR_MATERIAL);
}

void TPRender::TwoPassRend(int number, double fov, Affinef m, int w, int h){
	// 1次レンダリング終了後

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	// ytakeyama
	// テクスチャオブジェクトをbind
	glBindTexture(GL_TEXTURE_2D, buf->texture_id);
	// pbufferをテクスチャにbind
	buf->BindAsTexture(WGL_FRONT_LEFT_ARB);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//end

	/// glCopyTexImage2D //// //ytakeyama comment out
	//glGenTextures(1, texname);
	//glBindTexture(GL_TEXTURE_2D, texname[0]); //end

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

//    if (linearFilter) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    } else {
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    }
	GLfloat borderColor[4] = { 0.1, 0.1, 0.1, 0.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	int winWidth, winHeight;
	switch (number){
	case 0: case 1: case 2: case 3:
	case 4: case 5: case 6: case 7:
		// 1280×1024画素（SXGA）用
		winWidth  = 2048;  winHeight = 1024;
		break;
	default:
		// 1024×768画素（XGA）用
		winWidth  = 1024;  winHeight = 1024;
		break;
	}

	/*glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 
		winWidth, winHeight, 0);*/ //ytakeyama comment out
	// レンダリング結果をテクスチャとして取り出す


	projTexInit(fov, w, h);

	glLoadIdentity();
	static Affinef R180(Rad(180),'y');
	Affinef R0 = (R180 * m * R180).inv();
	glLoadMatrixf(R0);

	projectiveTexture(R0);

	//ytakeyama
	buf->ReleaseTexture(WGL_FRONT_LEFT_ARB);
	//end
	
	glFlush();
	glutSwapBuffers();
	//	Flip(); の代わり

	//glDeleteTextures(1, texname); // ytakeyama comment out

	vertualInit(fov, w, h);
}

// matrix = identity 
void TPRender::matrixIdentity(GLfloat matrix[16])
{
    matrix[ 0] = 1.0;
    matrix[ 1] = 0.0;
    matrix[ 2] = 0.0;
    matrix[ 3] = 0.0;
    matrix[ 4] = 0.0;
    matrix[ 5] = 1.0;
    matrix[ 6] = 0.0;
    matrix[ 7] = 0.0;
    matrix[ 8] = 0.0;
    matrix[ 9] = 0.0;
    matrix[10] = 1.0;
    matrix[11] = 0.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = 0.0;
    matrix[15] = 1.0;
}
// matrix2 = transpose(matrix1) 
void TPRender::matrixTranspose(GLfloat matrix2[16], GLfloat matrix1[16])
{
    matrix2[ 0] = matrix1[ 0];
    matrix2[ 1] = matrix1[ 4];
    matrix2[ 2] = matrix1[ 8];
    matrix2[ 3] = matrix1[12];

    matrix2[ 4] = matrix1[ 1];
    matrix2[ 5] = matrix1[ 5];
    matrix2[ 6] = matrix1[ 9];
    matrix2[ 7] = matrix1[13];

    matrix2[ 8] = matrix1[ 2];
    matrix2[ 9] = matrix1[ 6];
    matrix2[10] = matrix1[10];
    matrix2[11] = matrix1[14];

    matrix2[12] = matrix1[ 3];
    matrix2[13] = matrix1[ 7];
    matrix2[14] = matrix1[14];
    matrix2[15] = matrix1[15];
}
void TPRender::makeDList(void){
	line = glGenLists(1);
	glNewList(line, GL_COMPILE);

	int x_val =  4000;
	int y_val =  3000;
	int z_val = 10000;
	int lineRow = 6;
	int lineColumn = 8;

	glBegin(GL_LINES);
	for (int i = -lineRow; i <= lineRow; i++) {
		glVertex3i(-x_val, i*500, z_val);
		glVertex3i( x_val, i*500, z_val);
	}
	for (int i = -lineColumn; i <= lineColumn; i++) {
		glVertex3i( i*500,-y_val, z_val);
		glVertex3i( i*500, y_val, z_val);
	}
	for (int i = -lineRow; i <= lineRow; i++) {
		glVertex3i( x_val, i*500,-z_val);
		glVertex3i( x_val, i*500, z_val);
	}
	for (int i = -lineRow; i <= lineRow; i++) {
		glVertex3i(-x_val, i*500,-z_val);
		glVertex3i(-x_val, i*500, z_val);
	}
	for (int i = -lineColumn; i <= lineColumn; i++) {
		glVertex3i( i*500, y_val, -z_val);
		glVertex3i( i*500, y_val,  z_val);
	}
	for (int i = -lineColumn; i <= lineColumn; i++) {
		glVertex3i( i*500, -y_val, -z_val);
		glVertex3i( i*500, -y_val,  z_val);
	}
	for (int i = 0; i <= 40; i++) {
		glVertex3i( x_val,  y_val,  z_val - i*300);
		glVertex3i( x_val, -y_val,  z_val - i*300);
	}
	for (int i = 0; i <= 40; i++) {
		glVertex3i(-x_val,  y_val,  z_val - i*300);
		glVertex3i(-x_val, -y_val,  z_val - i*300);
	}
	for (int i = 0; i <= 40; i++) {
		glVertex3i( x_val, -y_val,  z_val - i*300);
		glVertex3i(-x_val, -y_val,  z_val - i*300);
	}
	for (int i = 0; i <= 40; i++) {
		glVertex3i( x_val,  y_val,  z_val - i*300);
		glVertex3i(-x_val,  y_val,  z_val - i*300);
	}
	glEnd();

	glEndList();
}
void TPRender::testPattern(void){

	cout << "count = " << VMRoom.count << endl;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth (4.0);
	glColor3d(1.0, 1.0, 1.0);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	glCallList(line);

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
}
///////////////////////////////////////////////////////////////////
/*
void TGLView::checkErrors(void)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
	fprintf(stderr, "Error: %s\n", (char *) gluErrorString(error));
    }
}
*/
