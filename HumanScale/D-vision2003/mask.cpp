
#include "GLView.h"
#include "GeoNode.h"

#include <vector>
#include <Base/Env.h>

#include "GLView.h"
#include <GL/glut.h>


#include "mask.h"


 
void drawMask(Affinef* afBody, Affinef* afCam){

   GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat mat_diffuse[] = {0.0, 0.0, 0.0, 1.0}; 
   GLfloat no_shininess[] = { 0.0 };

   glDisable(GL_DEPTH_TEST);
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

	// ���_���S�Ɉړ�
	glTranslatef(
		afBody->Pos().X(),
		afBody->Pos().Y(), 
		afBody->Pos().Z()
		);

	// ���������ɍ��킹�ĉ�]
	Affinef R180(Rad(180),'y');  // ���W�n��Y���ɑ΂���180�x��]
	Matrix3f Rot = R180.Rot() * afBody->Rot();  // �����3x3�s��
	Affinef Move_to_Avator;  // 4x4�s��Ƃ��Ď擾
	Move_to_Avator.Rot() = Rot;
	glMultMatrixf(Move_to_Avator);

	// ���_�̍������A�o�^�[�ɍ��킹��
	glTranslatef(0, afCam->Pos().Y() - afBody->Pos().Y(), 40.f);
	

	// �F�����̒���
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	// �I�u�W�F�N�g�̕`��
	glutSolidTorus(11.0f, 20.0f, 40.f, 40.f);  // �����ƊO�a��傫�����Ȃ��Ă�
	glTranslatef(0, 0, 100);                   // D-vision�ł�Mask�ł��Ȃ������c

	// OpenGL�̏�Ԃ����ɖ߂�
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}