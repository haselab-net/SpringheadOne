#include "ball.h"

//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
//////////////////////////////////////////////////////////////////////
CBall::CBall()
{
	//hand init
	init_vel.x= 0.0;
	init_vel.y = 85.0; //80 14 after 1.5 s, 25m
	init_vel.z = 0.0;
    beta=15;//15; //shot angle 60 after t=2 z->15m 
    angle=0.0; //X/Z plane angle
	radius = 4.8; // 4.8inch => 0.12m =>0.3936feet
    mass = 0.45; //14.5939/32.1740 

	ret_beta=45;

	CBall::Init();
}

CBall::~CBall()
{

}
void CBall::Init()
{

   init_pos.x= -5;
   init_pos.y= nRange;
   init_pos.z= -115;

   //ret_angle=0.0; //X/Z plane angle
   new_pos= init_pos;
   // 2003 10/26
   shot_pos = init_pos;
	
   //velocity
   bounce =0.87;
   new_vel= init_vel;
   minZ=0;
   minY=0;
//BALL INITIAL STATUS
   chk_state = PAUSE;
   chk_shoot = NOSHOOT;
   
}

double CBall::WhatTime(double dist, double left_x, double right_x)
{
	double deg = beta*M_PI/180;
    double tt = (dist- shot_pos.z)/(init_vel.y*cos(deg));
	double vh_pos_x = (left_x +right_x)/2;
    double h_pos_x = ball_spos.X();

	if (vh_pos_x < left_x)
	   angle = acos((left_x-h_pos_x)/(init_vel.y *cos(deg)*tt));
	else if (vh_pos_x > right_x)
		angle = acos((right_x-h_pos_x)/(init_vel.y *cos(deg)*tt));
		else
			angle = 0.0; 
	
		if (angle !=0.0) angle = 90-((180 * angle) / M_PI); //rad->degree

	return tt;
}

MyVector CBall::Parabola(double alpha, double t)
{
  	  MyVector ball_v;
	  double beta0 = beta*M_PI/180; //rad conversion

	  double vh = init_vel.y *cos(beta0);
	  double vy= init_vel.y *sin(beta0)- g*t;

      ball_v.x = shot_pos.x+(vh*t*sin(M_PI*alpha/180));
	  ball_v.y = shot_pos.y+(init_vel.y*t*sin(beta0)-0.5*g*t*t);
	  ball_v.z = shot_pos.z+(vh*t*cos(M_PI*alpha/180));
  
	  minY =new_pos.y - radius; // distance from ball position to drop position
	  return (ball_v);  
}

void CBall::RetAngle(double ref_vel, MyVector off)
{
      double beta0 = (beta*M_PI)/180; //rad conversion
    
	  double ret_time = abs(ReturnHand.Z()- off.z)/(ref_vel*cos(beta0));
      double alpha =acos((off.x-ReturnHand.X())/(ref_vel *cos(beta0)*ret_time));
	  
	  ret_angle = 90-((180 * alpha) / M_PI); //rad->degree
		
}

MyVector CBall::Reflect(double dt, MyVector off_v)
{
      double r_beta0 = (ret_beta*M_PI)/180; //rad conversion
	  double ref_vel = 68; //init_vel.y;  45 10 60 20 68
	  double vh = ref_vel*cos(r_beta0);
	  MyVector reflex_v;
    
	  //RetAngle(ref_vel, off_v);
      // vh inverse +=>-
	  reflex_v.x = off_v.x+(-vh*dt*sin(M_PI*ret_angle/180)); 
	  reflex_v.y = off_v.y+(ref_vel*dt*sin(r_beta0)-g*32*dt*dt); //0.5*
	  reflex_v.z = off_v.z+(-vh*dt*cos(M_PI*ret_angle/180)); 

	  minZ =reflex_v.z-radius; // distance 
      minY =reflex_v.y - radius;  
	  return reflex_v;
}
	
MyVector CBall::Fall_Roll(double speed, double t)
{
	double minY=0;
	MyVector fvec;

    //new_pos.y -= 0.5*g*t*t; 
	if (speed < 0.0) speed = 0.0; 
    fvec.x = Fall_pos.X();
	fvec.y = Fall_pos.Y()-(speed*t-0.5*g*t*t);
	if (fvec.y > Fall_pos.Y())
	   fvec.y = Fall_pos.Y();  //2003/12/25 jeong
	fvec.z = Fall_pos.Z(); 	
    
	minY=fvec.y;

	if(minY <= (GnHeight+radius))
	{
		new_vel.y = speed*bounce;
		bounce -= 0.3;
		if (new_vel.y < .00001 && new_vel.y > -.00001) 
		{
			//new_pos.x = init_pos.x;
			fvec.x = Fall_pos.X();	
			fvec.y = minY;
			fvec.z = Fall_pos.Z(); 	
		}
	}
   return(fvec);
}

void CBall::collision_repulse()
{
	GLfloat theta;
	angle = angle - 90 * (((int)angle + 45)/90);
	theta = angle * M_PI / 180.0;
	//new_vel.x = cos(2 * theta) * init_vel.x - sin(2 * theta) * init_vel.z;
	//new_vel.y = init_vel.y;
	//new_vel.z = - sin(2 * theta) * init_vel.x - cos(2 * theta) * init_vel.z; 
	MyVector normal(0.0, 0.0, -1.0);

    double omegaMag = new_vel.Magnitude();
	new_vel.Normalize();
	double multiplier = 2 * (normal * new_vel);	// calc normalized reflection vector
	new_vel = (multiplier * normal) - new_vel;
	new_vel *= omegaMag;	
	//new_vel.y = 0.8*new_vel.y;
    new_vel.z = - sin(2 * theta) * 15 - cos(2 * theta) * 15;
	new_vel.x = cos(2 * theta) * 15 - sin(2 * theta) * 15;
}

void CBall::DrawSphere(float rad)
{
  int numMajor = 24;
  int numMinor = 32;
  double majorStep = (M_PI / numMajor);
  double minorStep = (2.0 * M_PI / numMinor);
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
 //Texture
}

void CBall::Draw()
{
	 glPushMatrix();

		glTranslatef(new_pos.x, new_pos.y, new_pos.z);
		//glBindTexture(GL_TEXTURE_2D, tex[2]);
		glEnable(GL_TEXTURE_2D);
		DrawSphere(radius);
        glDisable(GL_TEXTURE_2D);      
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
	   glDisable(GL_TEXTURE_2D);      
	   DrawSphere(radius);
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

	double hh= 0.5f-(new_pos.y*0.01f);
	glPushMatrix();
		glTranslatef(new_pos.x, hh, new_pos.z);
		DrawShadow(hh, mat);
        
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
}
