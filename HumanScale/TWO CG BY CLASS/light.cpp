
#include <GL/glut.h>
#include <ATIExtensions.h>
#include <stdlib.h>
#include <stdio.h>
#include "Cgc.h"
#include <iostream.h>


bool flagShader = 0;
bool flagCg = 1;
Cgc *cgc;


//////////////////////////////////////////////////////////////////////////////////
//
//										OpenGL基本関数
//
/////////////////////////////////////////////////////////////////////////////////

void init(void) 
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

   cout << "Cgのon/offはｃを押してください" << endl;
   cout << "シェーダの切り替えはCgがonになっている状態で、ｓを押して下さい" << endl;
   glutSetWindowTitle("Cgのon/offはｃ、シェーダプログラムの切り替えは、Cgがonの状態でｓ");

}



void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glutSolidSphere (1.0, 20, 16);

 
   //基本的に、Ｃｇプログラムをロードするのは、gl～～などで頂点座標が決まる前になります。
   //決まってからじゃ、もう頂点シェーダのパイプラインを通過してるので、意味なし・・・
   //基本的に、loadprogramしたらまずdisable にして、使う直前でenableにするのが、管理が楽らしい。
   //なお、複数の描画対象によって使うＣｇプログラムを変えたい時は、
   //cgGLEnable　→　cgGLBindProgram　→　gl～～①　→　cgGLDisable
   //cgGLEnable　→　cgGLBindProgram　→　gl～～②　→　cgGLDisable
   //という流れにした方が良い。

   //cgGLDisableProfile(vertexProfile);
   //cgGLDisableProfile(fragmentProfile);
   //cgGLBindProgram(vertexProgram);
   //cgGLBindProgram(fragmentProgram);

   if(flagCg)cgc->DisableCgProfile();
   else		cgc->EnableCgProfile();

   if(flagShader == 0)cgc->BindVertexProgram(0);
   else					cgc->BindVertexProgram(1);

   cgc->BindFragmentProgram(0);

   glutSolidSphere(1.0, 20, 16);

   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// ARGSUSED1
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
	  case 's':
		  flagShader = !flagShader;
		  break;
	  case 'c':
		  flagCg = !flagCg;
		  break;
   }
   glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);

  
	init ();
	cgc = new Cgc;
	//識別番号、ファイル名、エントリ関数名の３つを引数にして、
	//使うべきCgプログラムをロードする。
	//識別番号は０から増やしていってください。
	cgc->LoadCgVertexPrograms(0, "C2E1v_green.cg", "main");
	cgc->LoadCgVertexPrograms(1, "C2E1v_red.cg", "main");
	cgc->LoadCgFragmentPrograms(0, "C2E2f_passthrough.cg", "main");

	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	for(int i = 0; i < VSMAX; i++)
		cgDestroyProgram(vertexProgram[i]);
	for(int j = 0; j < VSMAX; j++)
		cgDestroyProgram(fragmentProgram[i]);
	cgDestroyContext(context);

	return 0;
}
