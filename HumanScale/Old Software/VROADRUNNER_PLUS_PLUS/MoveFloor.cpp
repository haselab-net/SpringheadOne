// MoveFloor.cpp: CMoveFloor クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "MoveFloor.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

namespace vroadrunner{

int CMoveFloor::face[][4]
 = {
  { 0, 1, 2, 3 },
  { 1, 5, 6, 2 },
  { 5, 4, 7, 6 },
  { 4, 0, 3, 7 },
  { 4, 5, 1, 0 },
  { 3, 2, 6, 7 }
};

int CMoveFloor::edge[][2] = {
  { 0, 1 },
  { 1, 2 },
  { 2, 3 },
  { 3, 0 },
  { 4, 5 },
  { 5, 6 },
  { 6, 7 },
  { 7, 4 },
  { 0, 4 },
  { 1, 5 },
  { 2, 6 },
  { 3, 7 }
};

GLdouble CMoveFloor::normal[][3] = {
	{ 0.0, 0.0,-1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{-1.0, 0.0, 0.0 },
	{ 0.0,-1.0, 0.0 },
	{ 0.0, 1.0, 0.0 }
};

CMoveFloor::CMoveFloor()
{
	x = y = z = 0.0;
}

CMoveFloor::~CMoveFloor()
{
}

void CMoveFloor::SetVertex()
{
	for(int i=0;i<V_SIZE;i++){
		
		vertex[i][0] = (float) x;
		vertex[i][1] = (float) y;
		vertex[i][2] = (float) z;

		if( (((i+1)%4)/2) == 1){
			vertex[i][0] = (float) x+w;
		}
		if(((i%4)/2) == 1){
			vertex[i][1] = (float) y+h;
		}
		if((i/4) == 1){
			vertex[i][2] = (float) z+d;
		}
	}
}


void CMoveFloor::OnDraw()
{
}


void CMoveFloor::OnDraw(GLfloat *bx, GLfloat *by, GLfloat *bz, bool *bGravity)
{
/*
#ifdef _OKADA_TEST_APP
	std::cout << "T:" << T << " , reverseT:" << reverseT << std::endl;
	std::cout << "floorTime:" << floorTime << std::endl;
#endif	// _OKADA_TEST_APP
*/
	
	// 一つ目の移動床を真ん中の島で止める
	if(++floorTime > T/2 && floorTime <= T/2 + stopT) {
		bMove = false;
	} else { 
		bMove = true;
	}

	//	floorTimeが沿岸に着いたら一旦止めて、その後床の動く向きを反転
	if(floorTime >= T + stopT){
		if( floorTime >= reverseT ){

#ifdef _OKADA_TEST_APP
			std::cout << "T_CMove:" << T << " , reverseT_CMove:" << reverseT << std::endl;
			std::cout << "floorTime_CMove:" << floorTime << std::endl;
#endif	// _OKADA_TEST_APP			

			floorDirection *= -1;
			floorTime = 0;
			bMove = true;
		}else{
			bMove = false;
		}
	}
	
	//	床の位置の更新（前の位置＋速度）
	if( bMove ){
		this->x += floorDirection * vx;
		this->y += floorDirection * vy;
		this->z += floorDirection * vz;
		SetVertex();
	}

	//	動く床に乗っているかどうかの判定および乗っているときの処理
	this->Judge(bx, by, bz, bGravity);

#ifdef _OKADA_TEST_APP //** 動く床描画 **//
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 1.0);
	for (int i = 0; i < 6; i++) {
		glNormal3dv(normal[i]);
		for(int j=0;j<4;j++){
			glVertex3fv(vertex[face[i][j]]);
		}
	}
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
#endif //_OKADA_TEST_APP
  }


void CMoveFloor::Judge(GLfloat *bx, GLfloat *by, GLfloat *bz, bool *bGravity)
{
//	if( (*bx > this->x && *bx < this->x + w) && (*bz > this->z && *bz < this->z + d) && (*by > this->y && *by <= this->y + h + 0.1)){
	if( (*bx > this->x && *bx < this->x + w) && (*bz > this->z && *bz < this->z + d) && (*by > this->y && *by <= this->y + h + 6.0)){
		if( bMove ){
			*bx += floorDirection * vx;
			*by = y + h + 5 ;  
			*bz += floorDirection * vz;
		}
		*bGravity = false;	//	重力スイッチOFF	
	} else {
		*bGravity &= true;	//	重力スイッチON
	}
}

void CMoveFloor::InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->w = w;
	this->h = h;
	this->d = d;

	SetVertex();

}

void CMoveFloor::InitInstance(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat d, GLfloat mx, GLfloat my, GLfloat mz, GLfloat T)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->w = w;
	this->h = h;
	this->d = d;

	this->mx = mx;
	this->my = my;
	this->mz = mz;
	
	this->T = T;

	if ( T == 200 ) {
		stopT = 100;
	} else { stopT = 0; }

	this->reverseT = T + stopT + 100 ;
	this->bMove = true;

	this->vx = mx / T;
	this->vy = my / T;
	this->vz = mz / T;
	
	this->floorDirection = 1;
	this->floorTime = 0;

	SetVertex();
}

} //namespace vroadrunner

