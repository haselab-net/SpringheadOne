//#include "Method.h"
#include "GLParameter.h"

GLint point[MAXPOINTS][2]; // ���W���L������z��  
int pointnum = 0;          // �L���������W�̐��@  
int rubberband = 0;        // ���o�[�o���h�̏���  
int GL_EyeAngle[2];
bool FlagR_Off = false;

int* ReturnAngle(){
	return GL_EyeAngle;
}


///////////////////////////////////
//    �ɂȂƂ�
//////////////////////////////////
void idle(void)
{
  glutPostRedisplay();
}

//////////////////////////////
//�@������
/////////////////////////////
void init(void)
{
	//glClear�Ŏg�p����F���w��
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//�f�v�X�o�b�t�@ON
	glEnable(GL_DEPTH_TEST);
	//�J�����OON
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	//�A�eON
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//���C�g�̃p�����[�^�̐ݒ�
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT1, GL_SPECULAR, green);

}

///////////////////////////////////
//    �`��
//////////////////////////////////
void display(void)
{
	static int rY = 0; // Y���̉�]�p 
	static int rX = 0; // X���̉�]�p
//	static int k = 0; // �`���

	rY = -GL_EyeAngle[0];
	rX = GL_EyeAngle[1];

	//�E�B���h�E��h��Ԃ�(�f�v�X�o�b�t�@���[�h���܂߁j
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�e�N�X�`���[�\��t��
//	glBindTexture(GL_TEXTURE_2D , texName);


	// �����̈ʒu�ݒ� 
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

	//���f���r���[�ϊ��s��̕ۑ�
	glPushMatrix();

	// �}�`�̉�]
	glRotated((double)rY, 0.0, 1.0, 0.0);
	glRotated((double)rX, -1.0, 0.0, 0.0);

	//�}�e���A���ݒ�i�F�j
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color1[2]);
	//�}�e���A���ݒ�i���ʔ��˗��j
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color1[6] );
	//�}�e���A���ݒ�i�j
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS , &intensity[0] );
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &intensity[1] );
	
	cube();

	//���f���r���[�ϊ��s��̕��A
	glPopMatrix();

	glutSwapBuffers();


	// ��]�̐���
	if (glutLayerGet(GLUT_NORMAL_DAMAGED) == 0) {
	    // glutPostRedisplay() �ɂ��ĕ`��
//		cout << "draw(" << k++ <<")"<<endl;
		if (FlagR_Off) {
		// ����������A�j���[�V�������~�߂�
			rX = 0;
			rY = 0;
//			glutIdleFunc(0);
		}

  }

}


void resize(int w, int h)
{
	// �E�B���h�E�S�̂��r���[�|�[�g�ɂ��� 
	glViewport(0, 0, w, h);
	// �����ϊ��s��̐ݒ�
	glMatrixMode(GL_PROJECTION);
	// �ϊ��s��̏����� 
	glLoadIdentity();
	//�J��������̓������e
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	// ���f���r���[�ϊ��s��̐ݒ�
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	// ���_�ʒu�Ǝ������� 
	gluLookAt(0.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//�I�u�W�F�̕��s�ړ�
	//glTranslated(0.0, 0.0, -5.0);

}

///////////////////////////////////
//    �L�[�{�[�h����
//////////////////////////////////
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	//�I��
	case 'e':
		exit(0);

	//��]�����X�^�[�g
	case 'r':
		glutIdleFunc(idle);
		break;
	//��]�����X�g�b�v
	case 's':
		glutIdleFunc(0);
		FlagR_Off = true;
		break;

	case '\033':
		exit(0);  /* '\033' �� ESC �� ASCII �R�[�h */

	default:
    break;
	}
}



///////////////////////////////////
//    �}�E�X����
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
			// �\���C�x���g�̖������[�v���~�߂�
			glutIdleFunc(0);
			glutPostRedisplay();
		}
		break;
  
	default:
		break;
	}
}


///////////////////////////////////////
///�@�@�@�L���[�u��`��
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
//    �`��
//////////////////////////////////
void display(void)
{
	int i = 0;
	//�E�B���h�E��h��Ԃ�
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
	// �E�B���h�E�S�̂��r���[�|�[�g�ɂ��� 
	glViewport(0, 0, w, h);

	// �ϊ��s��̏����� 
	glLoadIdentity();

	//�J��������̓������e
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);

	//���_�̈ʒu�ړ�
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//�I�u�W�F�̕��s�ړ�
	//glTranslated(0.0, 0.0, -5.0);



	//�X�N���[����̍��W�n���}�E�X�̍��W�n�Ɉ�v������
//	glOrtho(-0.5, (GLdouble)w -0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
	// �X�N���[����̕\���̈���r���[�|�[�g�̑傫���ɔ�Ⴓ����
//	glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);

}

*/


/*
///////////////////////////////////
//    ����
//////////////////////////////////
void motion(int x, int y)
{
	 static GLint savepoint[2]; // �ȑO�̃��o�[�o���h�̒[�_  

	 // �_�����Z�@�\ ON  
	 glEnable(GL_COLOR_LOGIC_OP);
	 glLogicOp(GL_INVERT);

	 glBegin(GL_LINES);
	 if (rubberband) {
		// �ȑO�̃��o�[�o���h������  
		glVertex2iv(point[pointnum - 1]);
		glVertex2iv(savepoint);
	 }
	  // �V�������o�[�o���h��`��  
	 glVertex2iv(point[pointnum - 1]);
	 glVertex2i(x, y);
	 glEnd();

	 glFlush();

	 // �_�����Z�@�\ OFF  
	 glLogicOp(GL_COPY);
	 glDisable(GL_COLOR_LOGIC_OP);

	 // ���`�������o�[�o���h�̒[�_��ۑ�  
	 savepoint[0] = x;
	 savepoint[1] = y;
	 rubberband = 1;
}
 */

/*
///////////////////////////////////
//    �}�E�X����
//////////////////////////////////
void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) {
		  // �{�^�����������ʒu���痣�����ʒu�܂Ő�������  

		}
		break;

	case GLUT_RIGHT_BUTTON:
    //�v���O�����I�� 
		if(state == GLUT_UP)
			exit(0);
		break;
  
	default:
		break;
	}
}
*/