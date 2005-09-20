
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

	// 視点中心に移動
	glTranslatef(
		afBody->Pos().X(),
		afBody->Pos().Y(), 
		afBody->Pos().Z()
		);

	// 視線方向に合わせて回転
	Affinef R180(Rad(180),'y');  // 座標系をY軸に対して180度回転
	Matrix3f Rot = R180.Rot() * afBody->Rot();  // これは3x3行列
	Affinef Move_to_Avator;  // 4x4行列として取得
	Move_to_Avator.Rot() = Rot;
	glMultMatrixf(Move_to_Avator);

	// 視点の高さをアバターに合わせる
	glTranslatef(0, afCam->Pos().Y() - afBody->Pos().Y(), 40.f);
	

	// 色合いの調整
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	// オブジェクトの描画
	glutSolidTorus(11.0f, 20.0f, 40.f, 40.f);  // もっと外径を大きくしなくては
	glTranslatef(0, 0, 100);                   // D-visionではMaskできないかも…

	// OpenGLの状態を元に戻す
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}