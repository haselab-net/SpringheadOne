// Seesaw.cpp: CSeesaw クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Seesaw.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
namespace vroadrunner{

int CSeesaw::face[][4]
 = {
  { 0, 1, 2, 3 },
  { 1, 5, 6, 2 },
  { 5, 4, 7, 6 },
  { 4, 0, 3, 7 },
  { 4, 5, 1, 0 },
  { 3, 2, 6, 7 }
};

int CSeesaw::edge[][2] = {
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

GLdouble CSeesaw::normal[][3] = {
	{0.0, 0.0, -1.0},
	{1.0, 0.0, 0.0},
	{0.0, 0.0, 1.0},
	{-1.0, 0.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, 1.0, 0.0}
};

CSeesaw::CSeesaw()
{
	r = 15;
	x = y = z = 0.0;
}

CSeesaw::CSeesaw(GLdouble w,GLdouble h, GLdouble d)
{
	r = 0;

	x = w;
	y = h;
	z = d;
}

CSeesaw::~CSeesaw()
{

}

void CSeesaw::SetVertex()
{
	for(int i=0;i<V_SIZE;i++){
		
		vertex[i][0] = (double)-(w/2);
		vertex[i][1] = (double)-(h/2);
		vertex[i][2] = (double)-(d/2);
		
/*
		vertex[i][0] = 0;
		vertex[i][1] = 0;
		vertex[i][2] = 0;
*/
		if( (((i+1)%4)/2) == 1){
			vertex[i][0] = (double)(w/2);
		}
		if(((i%4)/2) == 1){
			vertex[i][1] = (double)(h/2);
		}
		if((i/4) == 1){
			vertex[i][2] = (double)(d/2);
		}
	}

	/*
	for(i=0;i<V_SIZE;i++){
		printf("%f,%f,%f\n",vertex[i][0],vertex[i][1],vertex[i][2]);
	}
	*/
	
}

void CSeesaw::OnDraw()
{
	glTranslated(x,y,z);
	glRotated((double)r,1.0,0.0,0.0);//回転移動

	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < 12; i++) {
		glVertex3dv(vertex[edge[i][0]]);
		glVertex3dv(vertex[edge[i][1]]);
	}
	glEnd();
}

void CSeesaw::OnDraw(GLdouble bx, GLdouble by, GLdouble bz)
{
	int dw=(w/2),dd=1;
	glTranslated(x,y,z);
	if( (this->x-dw < bx && bx < this->x + dw) && (this->z-dd < bz && bz < this->z + dd)){
		r = 0;
	}
	else if( (this->x-dw < bx && bx < this->x + dw) && (this->z+dd < bz && bz < this->z + (d/2))){
		r = 14;
	}
	else if( (this->x-dw < bx && bx < this->x + dw) && (this->z- (d/2) < bz && bz < this->z - dd)){
		r = -14;
	}
	glRotated((double)r,1.0,0.0,0.0);//回転移動

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shininess);
	glBegin(GL_QUADS);
	
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);

	glColor3d(64.0/255.0, 0.0, 0.0);
	for (int i = 0; i < 6; i++) {
		//glNormal3dv(normal[i]);
		for(int j=0;j<4;j++){
			glVertex3dv(vertex[face[i][j]]);
		}
		//glVertex3dv(vertex[edge[i][1]]);
	}
	//auxWireSphere(1);
	glEnd();
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);

}

void CSeesaw::InitInstance(GLdouble w, GLdouble h, GLdouble d)
{
	this->w = w;
	this->h = h;
	this->d = d;

	SetVertex();
	SetMaterial();

}

void CSeesaw::InitInstance(GLdouble x, GLdouble y, GLdouble z,GLdouble w, GLdouble h, GLdouble d)
{
	this->x = x;
	this->y = y;
	this->z = z;

	this->w = w;
	this->h = h;
	this->d = d;

	SetVertex();
	SetMaterial();

}


void CSeesaw::SetMaterial()
{
	ambient[0] = 0.1;
	ambient[1] = 0.1;
	ambient[2] = 0.1;
	ambient[3] = 1;

	diffuse[0] = 0.25;
	diffuse[1] = 0;
	diffuse[2] = 0;
	diffuse[3] = 1;

	specular[0] = 1;
	specular[1] = 1;
	specular[2] = 1;
	specular[3] = 1;

	shininess = 50;
}
} //namespace vroadrunner{
