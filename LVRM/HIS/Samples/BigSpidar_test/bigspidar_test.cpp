#include <stdio.h>
#include <stdlib.h>
#include <GL/glaux.h>
#include <GL/glut.h>
#include <GL/glu.h>								
#include <math.h>
#include "HIS/HISpidar4Quad.h"
#include <Device/DRUsb20Simple.h>
#include <winBasis/WBMMTimer.h>
#include <conio.h>
#include <iostream>
#include "mymath.h"

using namespace LVRM;

HISpidar4quad h_spidar[2];
const float K = 10;  //10n/m
float k_ratio = 0.2;
WBMMTimer timer;

///////////////////////////////////////////
#define END_POS 5
#define JOINT_RAD 0.75

const GLfloat deltaRot = 5.0f;
const GLfloat nRange = 17.0f; 
const GLfloat nHeight = 45.0f; 
const GLfloat delta_Pad = 0.5f;
const GLfloat convert_inch = 39.37;
const GLfloat GnHeight = 2.5f; 
//////////////////////////////////////////
//static int force;
static int cur_frm =0;
// check variables
static int full_mode =0;
static int chk_detect =0;
static int animating = 1;
static int chk_hand = 0; //ball and hand collision
static int chk_wall = 0; //ball and wall collision
// camera 
static float fov = 60.0;//50
static float distZ = 120.0;
typedef struct rotInfo {
  GLfloat x, xMin, xMax;
  GLfloat y, yMin, yMax;
  GLfloat z, zMin, zMax;
} rotInfo;

////////////////////////////////////////////////
rotInfo camera = { -5.0f, 0.0f, 360.0f, 0.0f, 0.0f, 360.0f, 0.0f, 0.0f, 360.0f };

//////////////////////////////
//lighting & color definition
static GLfloat light0pos[] = {0.0, 200.0, 200.0, 1.0};
static GLfloat light1pos[] = {200.0, 200.0, 0.0, 1.0};

static GLfloat green[] = {0.25, 1.0, 0.25, 1.0};
static GLfloat red[] = {1.0, 0.25, 0.25, 1.0};
static GLfloat blue[] = {0.25, 0.25, 1.0, 1.0};
static GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
static GLfloat gray[] = {0.5, 0.5, 0.5, 1.0};
static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat yellow[] = {1.0, 1.0, 0.25, 1.0};
static GLfloat headcolor[] = {1.0, 0.8, 0.6, 1.0};
static GLfloat wallcolor[] = {0.64, 0.47, 0.36, 1.0};
static GLfloat shadow[] = { 0.0, 0.0, 0.0, 0.5   };
//////////////////////////////

struct BallInfo {
  MyVector p0;
  MyVector p1;
  MyVector dev_pos;
  MyVector b_force;
  GLfloat v0;
  GLfloat beta, angle;
  GLfloat radius;
} Ball;

//ball parabola /collision detect variable
static GLfloat unit_Time = 0.033; 
static MyVector avg_epos;
static MyVector paddle_v;
static MyVector reflex_v;
static MyVector off_v;
static double t=0.0;
static double dt=0.0;
static double minY=0.0, minZ=0.0;
static MyVector b_vector;
GLfloat s_rad = 1;
static MyVector grp_pos0;
static MyVector grp_pos2;
static MyVector user_hand0;
static MyVector user_hand2;
static double thand_dis;

double what_time(double dist)
{
	double deg = Ball.beta*M_PI/180;
    double tt = (dist- Ball.p0.z)/(Ball.v0*cos(deg));

	return tt;
}
// initial ball setting
void init_paddle()
{
   
   paddle_v.x=-15;
   paddle_v.y=nHeight; //nHeight-nRange; //0.88 m (2.8864) ~ 1m 1m=3.28feet ;30inch
   paddle_v.z=-10;//about 2.9 m 9.512 feet 115 inch
 
}
void init_hand()
{
   user_hand0.x = 0.0;
   user_hand0.y = nHeight;
   user_hand0.z = paddle_v.z;
   
   user_hand2 =user_hand0;
   grp_pos0 =user_hand0;
   grp_pos2 =user_hand0;
   grp_pos2.x += Ball.radius*2;
   chk_hand = 0;
   
  
}
// initial ball setting
void init_ball()
{
   
   t=0.0;
   dt=0.0;
   
   //init_hand();
  
   Ball.p0.x= 10;
   Ball.p0.y= (grp_pos0.y+grp_pos2.y)/2;
   Ball.p0.z= (grp_pos0.z+grp_pos2.z)/2;

   Ball.p1= Ball.p0;

   Ball.dev_pos.x=0.0;
   Ball.dev_pos.y=0.0;
   Ball.dev_pos.z=0.0;

   reflex_v.x=0.0;
   reflex_v.y=0.0;
   reflex_v.z=0.0;
   
   chk_detect = 0;
   chk_wall =0;  
   minZ=0;
   minY=0;
}

MyVector parabola(double alpha, double t)
{
  	  MyVector ball_v;
	  double beta0 = Ball.beta*M_PI/180; //rad conversion

	  double vh = Ball.v0 *cos(beta0);
	  double vy= Ball.v0*sin(beta0)- g*t;

      ball_v.x = Ball.p0.x+(vh*t*sin(M_PI*alpha/180));
	  ball_v.y = Ball.p0.y+(Ball.v0*t*sin(beta0)-0.5*g*t*t);
	  ball_v.z = Ball.p0.z+(vh*t*cos(M_PI*alpha/180));
  
	  minY =ball_v.y - Ball.radius; // distance from ball position to drop position
	  return (ball_v);  
}

void reflect(double dt)
{
      double ret_time;
	  double alpha=0.0;
	  double beta0 = (Ball.beta*M_PI)/180; //rad conversion
      double ref_vel = Ball.v0-35;
      

	  ret_time = abs(grp_pos0.z- off_v.z)/(ref_vel*cos(beta0));
      alpha =acos((off_v.x-(grp_pos0.x+grp_pos2.x)/2)/(ref_vel *cos(beta0)*ret_time));
	  alpha = 90-((180 * alpha) / M_PI); //rad->degree
	  double  vh = ref_vel*cos(beta0);
   
	  // vh inverse +=>-
	  reflex_v.x = off_v.x+(-vh*dt*sin(M_PI*alpha/180)); 
	  reflex_v.y = off_v.y+(ref_vel*dt*sin(beta0)-0.5*g*dt*dt); 
	  reflex_v.z = off_v.z+(-vh*dt*cos(M_PI*alpha/180)); 

	  minZ =reflex_v.z-Ball.radius; // distance 
       
}

//hand collision detect
void handColliding()
{
	double t_rad = Ball.radius + s_rad;
	double l_dis = MyVectorDistance(grp_pos0, Ball.p0); 
    double r_dis = MyVectorDistance(grp_pos2, Ball.p0); 
 
	if (t_rad >= l_dis && t_rad >= r_dis)  chk_hand= 1; 
	else  chk_hand = 0;

}

//Wall collision detect
void wallColliding()
{
	double t_rad = Ball.radius + (GnHeight*2);
	double dis = Ball.p1.y; 
 
	if (t_rad >= dis)  chk_wall= 1; 
	else  chk_wall = 0;

}

void idle()
{
//	 if (chk_detect != 1 && Moving_ball ==1) ballColliding();
	if (chk_hand !=1 ) handColliding();
	if (chk_wall !=1 ) {
		wallColliding();
  	    printf("check: %d ball y %f\n", chk_wall,Ball.p1.y); 
	}
// animating 
  glutPostRedisplay();
  
}

void specialKeyHandler(int key, int x, int y) {
  if (key == GLUT_KEY_LEFT)    { camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { camera.x -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { camera.x += deltaRot; }
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){

  switch(key){
  case '.':
	   camera.z -= deltaRot; 
	   break;
  case ',':
	  camera.z += deltaRot; 
	  break;
  case 'z':
	  distZ = distZ - 5;
	  break;
  case 'Z':
	  distZ = distZ + 5;
	  break;
//next frame
  case 'c': 
	   init_hand();
	   
	   h_spidar[0].Calib();
	   h_spidar[1].Calib();
  	  // init_ball();
	   break;
  case 'f': 
	  full_mode =!full_mode;
	  if(full_mode) glutFullScreen();
	  else   glutReshapeWindow(400,300);
	   break;
 case 'i': // reset ball
	 init_ball();
     break;
 case 'h': // reset hand
	 init_hand();
     break;
  case 'w': // reset hand
	 Ball.p1.y +=1;
	 printf("ball y %f\n", Ball.p1.y); 
     break;
  case 's': // reset hand
	 Ball.p1.y -=0.5;
	 printf("ball y %f\n", Ball.p1.y); 
     break;
  case '\x1b': //exit
	 exit(0);
	 break;
  default:
    break;
  }
  glutPostRedisplay();
}
void update_physics()
{
       Vec3f t_force[2];
     Vec3f t_pos[2];

    for(int i=0; i<2;i++)
	{
	  t_force[i] = Vec3f(0, 0, 0);
	
	  h_spidar[i].Update();
      t_pos[i] = h_spidar[i].GetPos();//*convert_inch
	}

    if (fabs(t_pos[0].X()) <= 100 && fabs(t_pos[1].X()) <= 100 )
	 {
		 //printf( "spidar.x: %f, y: %f  z%f \n", ss.x, ss.y, ss.z);
		 grp_pos0.x = user_hand0.x - t_pos[0].X();
		 grp_pos0.y = user_hand0.y + t_pos[0].Y();
		 grp_pos0.z = user_hand0.z - t_pos[0].Z(); //x,z sign , inverse
         
		 grp_pos2.x = user_hand2.x - t_pos[1].X();
		 grp_pos2.y = user_hand2.y + t_pos[1].Y();
		 grp_pos2.z = user_hand2.z - t_pos[1].Z();
		
		 //printf( "hand pos.x: %f, y: %f  z%f \n", grp_pos0.x, grp_pos0.y, grp_pos0.z);
	
  		 double dis = MyVectorDistance(grp_pos0, Ball.p0); 
		 double dis2 =MyVectorDistance(grp_pos2, Ball.p0) ; 
	 	 double t_dis = MyVectorDistance(grp_pos0, grp_pos2);
	    // printf( "dis: %f,  dis2: %f  hand_dis: %f\n",dis, dis2, t_dis);
		if (dis-Ball.radius < s_rad)
		{
			t_force[0].X() = -K * ((Ball.radius+s_rad - dis)); 
		}
		h_spidar[0].SetForce(t_force[0]);
		
		if (dis2-Ball.radius < s_rad)
		{
		   t_force[1].X() = -K * ((Ball.radius+s_rad - dis2)/convert_inch); 
	
		}
		//h_spidar[1].SetForce(t_force[1]);
		h_spidar[1].SetMinForce();
        //h_force =t_force[1].X();

		if (chk_detect == 0 ) handColliding();
	          	
//b_force,x != force.X()
		/*
		if (BBall.chk_shoot!=SHOOTED && abs(t_force[0].X()) >= 0.5 && abs(t_force[1].X()) >= 0.5)
		 {
			     BBall.b_force.x = t_force[0].X();
		 		 BBall.dev_pos.x = (t_hand.grp_pos0.x+t_hand.grp_pos2.x)/2; 
				 BBall.dev_pos.y = (t_hand.grp_pos0.y+t_hand.grp_pos2.y)/2;
				 BBall.dev_pos.z = (t_hand.grp_pos0.z+t_hand.grp_pos2.z)/2; 
				
				 BBall.p0 = BBall.dev_pos;
		  	 //printf( "force: %f,  ball pos: x%f  y%f %i\n",force.X(), Ball.dev_pos.x, Ball.dev_pos.y, chk_hand);
		 }
		 if (BBall.chk_move !=MOVING && BBall.chk_shoot !=SHOOTED && t_hand.h_force <= 0.1 && t_hand.hand_dis > 20 && BBall.p0.y <= 60) 
		 {
				BBall.chk_shoot = SHOOTED;
				BBall.chk_move= MOVING;

				BBall.p0 = BBall.dev_pos;
				//printf( "force: %f, ball shooting pos: x:%f  y:%f z:%f\n",t_hand.h_force, Ball.p0.x, Ball.p0.y, Ball.p0.z);
		 }*/
	 }	 //<=100

  
}

void timerfunc(void* arg)
{

  update_physics(); 

}


void display(void)
{
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  gluLookAt(0.0, 0.0, distZ, 0.0, 0.0,0.0, 0.0,1.0, 0.0);

  glPushMatrix();

	 glTranslatef(0, -nHeight, -30);
	 glRotatef(camera.x, 1.0f, 0.0f, 0.0f);
	 //glRotatef(camera.y, 0.0f, 1.0f, 0.0f);
	 glRotatef(camera.y+180, 0.0f, 1.0f, 0.0f);
	 glRotatef(camera.z, 0.0f, 0.0f, 1.0f);
	 
	  glEnable(GL_LIGHTING);
	 
  //hip -5 
      glPushMatrix();
          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);			
	      glScalef(50.0, 1.0, 50.0);
		  glTranslatef(-1, GnHeight, -0.5);
	      glutSolidCube(5);//drawQuad(4, 1); 
		  glDisable(GL_TEXTURE_2D);
	 
	  glPopMatrix();
   
// ball display
		 glPushMatrix();
			if (chk_hand ) Ball.p1 =Ball.p0;
            if (chk_wall ) Ball.p1.y =GnHeight*2+Ball.radius;
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);						
			glTranslatef(Ball.p1.x, Ball.p1.y, Ball.p1.z);
			glutSolidSphere(Ball.radius, 15, 12);
           
		glPopMatrix();
//shadow
		 glPushMatrix();
		   float yy = GnHeight*2;//-(Ball.p1.y-Ball.radius);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);						
			glTranslatef(Ball.p1.x, yy, Ball.p1.z);
			glScalef(1.0, 0.1, 1.0);
			glutSolidSphere(Ball.radius, 15, 12);
		 glPopMatrix();

		glPushMatrix(); //left
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(grp_pos0.x,  grp_pos0.y, grp_pos0.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix(); //right
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(grp_pos2.x,  grp_pos2.y, grp_pos2.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();
	
	glPopMatrix();
	glutSwapBuffers();
}

void resize(GLsizei w, GLsizei h){

  GLfloat aspect;

 if(h == 0) { h=1;}

  glViewport(0, 0, w, h);

  aspect = w/h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(fov, aspect, 1, 1000);
 
}

void init(void)
{
 
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 tQuaternion nGround(0, GnHeight, 0, 0);
 MyVector vLight(-1, 1, 0); 
	
 glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
 glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
 glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);
	
  glClearColor(0.0, 0.0, 0.0, 0.0); 
  glEnable(GL_DEPTH_TEST); //hidden surface removed
 
  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);

   // Init connection

   Ball.v0 = 85; //80 14 after 1.5 s, 25m
   Ball.beta=30;//15; //shot angle 60 after t=2 z->15m 
   Ball.angle=0.0;
   Ball.radius = 4.8; // 4.8inch => 0.12m =>0.3936feet

   init_paddle();
   init_hand();
   init_ball();	

}

void menu(int value)
{
    switch (value) {
    
	case 1:
		animating = 0;// stop animation	 
	  break;
	case 2:				// play animation
		animating = 1;
		break;
	case 3:				// init ball
	    init_ball();
		break;
	case 5:				
	    exit(0);
        break;
	default:
		break;
	}
}

int main(int argc, char *argv[])
{
	DVDeviceManager devMan;
    
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();

//	std::cout << devMan;

	//	VM室Big-SM_PIDAR用　モータの取り付け座標（ターンテーブルの中心から床上110cmが原点）	
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.75f, 2.00f, 1.12f),	Vec3f( 1.65f, 2.17f,-1.34f), Vec3f(-1.65f,-0.9f,-1.44f), Vec3f( 1.65f,-0.99f, 0.50f)},
		{Vec3f( 1.58f, 2.00f, 1.12f),	Vec3f(-1.65f, 2.18f,-1.34f), Vec3f( 1.65f,-0.98f,-1.44f), Vec3f(-1.65f,-0.98f, 0.50f)}
	};


	for(int i=0; i<2; i++)
	{
	  h_spidar[i].Init(devMan, motorPos[i], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f); 
	  h_spidar[i].Calib();
	}

//timing set

  timer.Set(timerfunc, 0);
  timer.Resolution(1);
  timer.Interval(1);
  timer.Thread();

  glutInitWindowSize(400, 300);/* window size 1152, 864*/
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Demonstration of BigSpidar Test");
  
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeyHandler);
  glutIdleFunc(idle);
  glutDisplayFunc(display);
  init();
// menu create
    glutCreateMenu(menu);
    glutAddMenuEntry("          -BigSpidar TEST MENU-", 0);
	glutAddMenuEntry("------------------------------------------------", 0);
	glutAddMenuEntry("Animations", 0);
	glutAddMenuEntry("   Stop Animation ", 1);
	glutAddMenuEntry("   Play Animation ", 2);
	glutAddMenuEntry("------------------------------------------------", 0);
    glutAddMenuEntry("Ball", 0);
    glutAddMenuEntry("   Init ball", 3);
	glutAddMenuEntry("   Start throw", 4);
	glutAddMenuEntry("------------------------------------------------", 0);
	glutAddMenuEntry("Program", 0);
	glutAddMenuEntry("   Exit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return(0);
}
