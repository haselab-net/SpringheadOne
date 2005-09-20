// Ball.h: CBall クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#ifndef _CBALL_H
#define _CBALL_H

#include <GL/glut.h>
#define PI        3.14159265358979323846f
//chk_detect
#define NOCOLLISION 0
#define V_COLLISION 1
#define H_COLLISION 2
//chk_shoot
#define NOSHOOT 0
#define SHOOTED 1
#define REFLEX -1
//chk_move
#define MOVING 1
#define PAUSE  0
#define CONTINUE 2

class CBall  
{
public:
	CBall();
	virtual ~CBall();

    MyVector p0;
    MyVector p1;
    MyVector dev_pos;
    MyVector b_force;
    MyVector v0;
    GLfloat beta, angle;
    GLfloat radius;
    GLfloat mass;
    GLfloat minY;
	GLfloat minZ;
    int chk_shoot;
	int chk_move;

	MyVector Parabola(double alpha, double t);
	MyVector Reflect(double dt, MyVector off_v);
	double WhatTime(double dist);
    void DrawSphere(float rad);
	void Draw();
	void DrawShadow(float hh, GLfloat mat[4][4]);
	void Shadow_ball(GLfloat mat[4][4]);
//PRIVATE
	void Init();

};

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
CBall::CBall()
{
	//hand init
	v0.x= 0.0;
	v0.y = 85.0; //80 14 after 1.5 s, 25m
	v0.z = 0.0;
    beta=30;//15; //shot angle 60 after t=2 z->15m 
    angle=0.0;
    radius = 4.8; // 4.8inch => 0.12m =>0.3936feet
   
	CBall::Init();
}

CBall::~CBall()
{

}
void CBall::Init()
{

   p0.x= (t_hand.grp_pos0.x+t_hand.grp_pos2.x)/2;
   p0.y= (t_hand.grp_pos0.y+t_hand.grp_pos2.y)/2;
   p0.z= (t_hand.grp_pos0.z+t_hand.grp_pos2.z)/2;

   p1= p0;
   minZ=0;
   minY=0;
   chk_shoot = NOCOLLISION;
   chk_move = PAUSE;
}

double CBall::WhatTime(double dist)
{
	double deg = beta*PI/180;
    double tt = (dist- p0.z)/(v0.y*cos(deg));

	return tt;
}

MyVector CBall::Parabola(double alpha, double t)
{
  	  MyVector ball_v;
	  double beta0 = beta*PI/180; //rad conversion

	  double vh = v0.y *cos(beta0);
	  double vy= v0.y*sin(beta0)- g*t;

      ball_v.x = p0.x+(vh*t*sin(PI*alpha/180));
	  ball_v.y = p0.y+(v0.y*t*sin(beta0)-0.5*g*t*t);
	  ball_v.z = p0.z+(vh*t*cos(PI*alpha/180));
  
	  minY =p1.y - radius; // distance from ball position to drop position
	  return (ball_v);  
}

MyVector CBall::Reflect(double dt, MyVector off_v)
{
      double ret_time;
	  double alpha=0.0;
	  double beta0 = (beta*PI)/180; //rad conversion
      double ref_vel = v0.y-35;
      MyVector reflex_v;

	  ret_time = abs(t_hand.grp_pos0.z- off_v.z)/(ref_vel*cos(beta0));
      alpha =acos((off_v.x-(t_hand.grp_pos0.x+t_hand.grp_pos2.x)/2)/(ref_vel *cos(beta0)*ret_time));
	  alpha = 90-((180 * alpha) / PI); //rad->degree
	  double  vh = ref_vel*cos(beta0);
   
	  // vh inverse +=>-
	  reflex_v.x = off_v.x+(-vh*dt*sin(PI*alpha/180)); 
	  reflex_v.y = off_v.y+(ref_vel*dt*sin(beta0)-0.5*g*dt*dt); 
	  reflex_v.z = off_v.z+(-vh*dt*cos(PI*alpha/180)); 

	  minZ =reflex_v.z-radius; // distance 
       
	  return reflex_v;
}

void CBall::DrawSphere(float rad)
{
  int numMajor = 24;
  int numMinor = 32;
  double majorStep = (PI / numMajor);
  double minorStep = (2.0 * PI / numMinor);
  int i, j;

  for (i = 0; i < numMajor; ++i) {
    double a = i * majorStep;
    double b = a + majorStep;
    double r0 = rad * sin(a);
    double r1 = rad * sin(b);
    GLfloat z0 = rad * cos(a);
    GLfloat z1 = rad * cos(b);

    glBegin(GL_TRIANGLE_STRIP);
    for (j = 0; j <= numMinor; ++j) {
      double c = j * minorStep;
      GLfloat x = cos(c);
      GLfloat y = sin(c);

      glNormal3f((x * r0) / rad, (y * r0) / rad, z0 / rad);
      glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
      glVertex3f(x * r0, y * r0, z0);

      glNormal3f((x * r1) / rad, (y * r1) / rad, z1 / rad);
      glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
      glVertex3f(x * r1, y * r1, z1);
    }
    glEnd();
	
  }
  glDisable(GL_TEXTURE_2D);
}

void CBall::Draw()
{
	 glPushMatrix();

		glTranslatef(p1.x, p1.y, p1.z);
		glBindTexture(GL_TEXTURE_2D, tex[2]);
		glEnable(GL_TEXTURE_2D);
		DrawSphere(radius);
               
	glPopMatrix();

}
void CBall::DrawShadow(float hh, GLfloat mat[4][4])
{
	float fAmb[4] = { 0.5f,0.5f,0.5f,1 };
	float fDiff[4] = { 0.5f,0.5f,0.5f,1 };
    
	glMaterialfv(GL_FRONT, GL_AMBIENT, fAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, fDiff);
	// Make shadow intensity a function of distance from ground
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.1, 0.1, 0.1, 1.0);
	glPushMatrix();
	   glMultMatrixf((GLfloat *) mat);
	   drawSphere(radius);
	glPopMatrix();
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

void CBall::Shadow_ball(GLfloat mat[4][4])
{
	glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
   // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double hh= 0.5f-(p1.y*0.01f);
	glPushMatrix();
		glTranslatef(p1.x, hh, p1.z);
		DrawShadow(hh, mat);
        
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
}


#endif 