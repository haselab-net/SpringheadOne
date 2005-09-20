
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
//										OpenGL��{�֐�
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

   cout << "Cg��on/off�͂��������Ă�������" << endl;
   cout << "�V�F�[�_�̐؂�ւ���Cg��on�ɂȂ��Ă����ԂŁA���������ĉ�����" << endl;
   glutSetWindowTitle("Cg��on/off�͂��A�V�F�[�_�v���O�����̐؂�ւ��́ACg��on�̏�Ԃł�");

}



void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glutSolidSphere (1.0, 20, 16);

 
   //��{�I�ɁA�b���v���O���������[�h����̂́Agl�`�`�ȂǂŒ��_���W�����܂�O�ɂȂ�܂��B
   //���܂��Ă��炶��A�������_�V�F�[�_�̃p�C�v���C����ʉ߂��Ă�̂ŁA�Ӗ��Ȃ��E�E�E
   //��{�I�ɁAloadprogram������܂�disable �ɂ��āA�g�����O��enable�ɂ���̂��A�Ǘ����y�炵���B
   //�Ȃ��A�����̕`��Ώۂɂ���Ďg���b���v���O������ς��������́A
   //cgGLEnable�@���@cgGLBindProgram�@���@gl�`�`�@�@���@cgGLDisable
   //cgGLEnable�@���@cgGLBindProgram�@���@gl�`�`�A�@���@cgGLDisable
   //�Ƃ�������ɂ��������ǂ��B

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
	//���ʔԍ��A�t�@�C�����A�G���g���֐����̂R�������ɂ��āA
	//�g���ׂ�Cg�v���O���������[�h����B
	//���ʔԍ��͂O���瑝�₵�Ă����Ă��������B
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
