// JumpZone.cpp: CJumpZone クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "JumpZone.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

namespace vroadrunner{

CJumpZone::CJumpZone()
{
	x = y = z = 0;
	isJump = false;
}

CJumpZone::~CJumpZone()
{
}

void CJumpZone::InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d, GLint jumpDist)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->w = w;
	this->h = h;
	this->d = d;

	this->jumpDist = jumpDist;
}

//void CJumpZone::OnDraw(Affinef *p_afBody, Vec3f *p_vel, float accel)
//void CJumpZone::OnDraw(Affinef *p_afBody, Vec3f *p_vel, float gravity)
void CJumpZone::OnDraw(Affinef *p_afBody, Vec3f *p_vel, float dt)
{
//	float dt = float(dwTime - dwLastTime) / 1000 * 4;
	float gravity = 9.8f;
	float accel = gravity * dt / 4;
		

	if( (p_afBody->Pos().X() > this->x && p_afBody->Pos().X() < this->x + w) && (p_afBody->Pos().Z() > this->z && p_afBody->Pos().Z() < this->z + d) ) {
		//p_dvel->Y() += 0.5f * jumpDist;
//		p_dvel->Y() += accel / 2 * jumpDist;
//		*p_dvel -= p_afBody->Ez() * accel / 4.0f * jumpDist;
		isJump = true;
		// ターンテーブルを使わないとき
//		p_vel->Y() += accel * gravity / 3.3 * jumpDist;
//		*p_vel -= p_afBody->Ez() * accel * jumpDist;   

		// ターンテーブルを使うとき
		p_vel->Y() += accel * gravity / 2 * jumpDist;
		*p_vel -= p_afBody->Ez() * accel * 5 * jumpDist;   // ターンテーブルを使うとき
	}
	else{ isJump = false; }

	//std::cout << "accel = " << accel << std::endl;

/*
	// ジャンプ床のGLでの描画
	//	glDisable(GL_LIGHTING);
	glColor3d(0.0, 0.5, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(x, y+0.01, z);
	glVertex3f(x, y+0.01, z + d);
	glVertex3f(x + w, y+0.01, z + d);
	glVertex3f(x + w, y+0.01, z);
	glEnd();
*/
 }
/*
// 
void CJumpZone::OnDraw(Affinef *p_afBody, Vec3f *p_dvel, float accel)
{
	if( (p_afBody->Pos().X() > this->x && p_afBody->Pos().X() < this->x + w) && (p_afBody->Pos().Z() > this->z && p_afBody->Pos().Z() < this->z + d) ) {
		//p_dvel->Y() += 0.5f * jumpDist;
//		p_dvel->Y() += accel / 2 * jumpDist;
//		*p_dvel -= p_afBody->Ez() * accel / 4.0f * jumpDist;
		p_dvel->Y() += accel  * jumpDist;
		*p_dvel -= p_afBody->Ez() * accel  * jumpDist;
	}	

	std::cout << "accel = " << accel << std::endl;

	//	glDisable(GL_LIGHTING);
	glColor3d(0.0, 0.5, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(x, y+0.01, z);
	glVertex3f(x, y+0.01, z + d);
	glVertex3f(x + w, y+0.01, z + d);
	glVertex3f(x + w, y+0.01, z);
	glEnd();
}
*/
}  //namespace vroadrunner