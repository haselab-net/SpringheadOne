//#include "Method.h"
#include "GLParameter.h"

GLint point[MAXPOINTS][2]; // 座標を記憶する配列  
int pointnum = 0;          // 記憶した座標の数　  
int rubberband = 0;        // ラバーバンドの消去  
int GL_EyeAngle[2];
bool FlagR_Off = false;

int* ReturnAngle(){
	return GL_EyeAngle;
}


///////////////////////////////////
//    暇なとき
//////////////////////////////////
void idle(void)
{
  glutPostRedisplay();
}

//////////////////////////////
//　初期化
/////////////////////////////
void init(void)
{
	//glClearで使用する色を指定
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//デプスバッファON
	glEnable(GL_DEPTH_TEST);
	//カリングON
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	//陰影ON
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//ライトのパラメータの設定
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT1, GL_SPECULAR, green);

}

///////////////////////////////////
//    描画
//////////////////////////////////
void display(void)
{
	static int rY = 0; // Y軸の回転角 
	static int rX = 0; // X軸の回転角
//	static int k = 0; // 描画回数

	rY = -GL_EyeAngle[0];
	rX = GL_EyeAngle[1];

	//ウィンドウを塗りつぶす(デプスバッファモードも含め）
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//テクスチャー貼り付け
//	glBindTexture(GL_TEXTURE_2D , texName);


	// 光源の位置設定 
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

	//モデルビュー変換行列の保存
	glPushMatrix();

	// 図形の回転
	glRotated((double)rY, 0.0, 1.0, 0.0);
	glRotated((double)rX, -1.0, 0.0, 0.0);

	//マテリアル設定（色）
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1[2]);
	//マテリアル設定（鏡面反射率）
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1[6] );
	//マテリアル設定（）
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS , &intensity[0] );
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &intensity[1] );
	
	cube();

	//モデルビュー変換行列の復帰
	glPopMatrix();

	glutSwapBuffers();


	// 回転の制御
	if (glutLayerGet(GLUT_NORMAL_DAMAGED) == 0) {
	    // glutPostRedisplay() による再描画
//		cout << "draw(" << k++ <<")"<<endl;
		if (FlagR_Off) {
		// 一周回ったらアニメーションを止める
			rX = 0;
			rY = 0;
//			glutIdleFunc(0);
		}

  }

}


void resize(int w, int h)
{
	// ウィンドウ全体をビューポートにする 
	glViewport(0, 0, w, h);
	// 透視変換行列の設定
	glMatrixMode(GL_PROJECTION);
	// 変換行列の初期化 
	glLoadIdentity();
	//カメラからの透視投影
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	// モデルビュー変換行列の設定
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	// 視点位置と視線方向 
	gluLookAt(0.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//オブジェの平行移動
	//glTranslated(0.0, 0.0, -5.0);

}

///////////////////////////////////
//    キーボード入力
//////////////////////////////////
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	//終了
	case 'e':
		exit(0);

	//回転同期スタート
	case 'r':
		glutIdleFunc(idle);
		break;
	//回転同期ストップ
	case 's':
		glutIdleFunc(0);
		FlagR_Off = true;
		break;

	case '\033':
		exit(0);  /* '\033' は ESC の ASCII コード */

	default:
    break;
	}
}



///////////////////////////////////
//    マウス入力
//////////////////////////////////
void mouse(int button, int state, int x, int y)
{
	switch (button) {
/*	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) {
			glutIdleFunc(idle);
		}
		break;
*/
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_UP){
			// 表示イベントの無限ループを止める
			glutIdleFunc(0);
			glutPostRedisplay();
		}
		break;
  
	default:
		break;
	}
}


///////////////////////////////////////
///　　　キューブを描画
//////////////////////////////////////
void cube(void)
{
  int i;
  int j;
  glBegin(GL_QUADS);
  for (j = 0; j < 6; j++) {
    glNormal3dv(normal[j]);
    for (i = 0; i < 4; i++) {
		glColor3dv(color[j]);
/*
		switch(i){
		case 0:
			glTexCoord2f(0 , 1); 
			break;
		case 1:
			glTexCoord2f(1 , 1); 
			break
		case 2:
			glTexCoord2f(1 , 0); 
			break
		case 3:
			glTexCoord2f(0 , 0); 
			break;
		}
*/
		glVertex3dv(vertex[face[j][i]]);
    }
  }
  glEnd();
}



/*
///////////////////////////////////
//    描画
//////////////////////////////////
void display(void)
{
	int i = 0;
	//ウィンドウを塗りつぶす
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINES);

	for(i = 0; i < EDGE_MAX; i++){
		glVertex3dv(vertex[edge[i][0]]);
		glVertex3dv(vertex[edge[i][1]]);
	}

	glEnd();
 	
	glFlush();

	cout << "test\n";

}

void resize(int w, int h)
{
	// ウィンドウ全体をビューポートにする 
	glViewport(0, 0, w, h);

	// 変換行列の初期化 
	glLoadIdentity();

	//カメラからの透視投影
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

	//視点の位置移動
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//オブジェの平行移動
	//glTranslated(0.0, 0.0, -5.0);



	//スクリーン上の座標系をマウスの座標系に一致させる
//	glOrtho(-0.5, (GLdouble)w -0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
	// スクリーン上の表示領域をビューポートの大きさに比例させる
//	glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);

}

*/


/*
///////////////////////////////////
//    動き
//////////////////////////////////
void motion(int x, int y)
{
	 static GLint savepoint[2]; // 以前のラバーバンドの端点  

	 // 論理演算機能 ON  
	 glEnable(GL_COLOR_LOGIC_OP);
	 glLogicOp(GL_INVERT);

	 glBegin(GL_LINES);
	 if (rubberband) {
		// 以前のラバーバンドを消す  
		glVertex2iv(point[pointnum - 1]);
		glVertex2iv(savepoint);
	 }
	  // 新しいラバーバンドを描く  
	 glVertex2iv(point[pointnum - 1]);
	 glVertex2i(x, y);
	 glEnd();

	 glFlush();

	 // 論理演算機能 OFF  
	 glLogicOp(GL_COPY);
	 glDisable(GL_COLOR_LOGIC_OP);

	 // 今描いたラバーバンドの端点を保存  
	 savepoint[0] = x;
	 savepoint[1] = y;
	 rubberband = 1;
}
 */

/*
///////////////////////////////////
//    マウス入力
//////////////////////////////////
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) {
		  // ボタンを押した位置から離した位置まで線を引く  

		}
		break;

	case GLUT_RIGHT_BUTTON:
    //プログラム終了 
		if(state == GLUT_UP)
			exit(0);
		break;
  
	default:
		break;
	}
}
*/