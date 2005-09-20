#include <stdio.h>
#include <stdlib.h>
#include <GL/glaux.h>
#include <GL/glut.h>
#include <GL/glu.h>								
#include <math.h>
#include "odbcexec.h"     // database open/close
#include "myPrimitive.h" // primitive definition
#include "Quatern.h" //for interpolate/ vector computation

#include "HIS/HISpidar4Quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <winBasis/WBMMTimer.h>
#include <conio.h>
#include <iostream>

using namespace LVRM;
#define PX	1.2f	//	x方向の辺の長さ/2
#define PY	1.2f	//	y方向の辺の長さ/2
#define PZ	1.2f	//	z方向の辺の長さ/2

HISpidar4quad spidar;
HISpidar4quad spidar2;
const float K = 50;
WBMMTimer timer;

Affinef ball_gl; 
GLfloat s_rad = 1;
Vec3f sp_pos, sp_pos1;

///////////////////////////////////////////
#define END_POS 5
#define JOINT_RAD 0.75

const GLfloat deltaRot = 5.0f;
const GLfloat nRange = 17.0f; 
const GLfloat nHeight = 45.0f; 
const GLfloat delta_Pad = 0.5f;
const GLfloat convert_inch = 39.37;
//////////////////////////////////////////

//static int force;
static int cur_frm =0;
// check variables
static int full_mode =0;
static int cur_joint= 0;
static int chk_detect =0;
static int chk_end =0;
static int chk_cont =0;
static int chk_quat =0;

static int chk_interp = 0;
static int chk_ball = 0;
static int animating = 1;
static int chk_shoot = 0;
// camera 
static float fov = 45.0;//50
static float distZ = 80.0;
////////////////////////////////////////////////
rotInfo camera = { 0.0f, 0.0f, 360.0f, 0.0f, 0.0f, 360.0f, 0.0f, 0.0f, 360.0f };
IKBone k_bone[7];

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
  tVector p0;
  tVector p1;
  tVector dev_pos;
  tVector b_force;
  GLfloat v0;
  GLfloat beta, angle;
  GLfloat radius;
} Ball;

//ball parabola /collision detect variable
static GLfloat g = 32;//9.80665(m/s^2) 32ft/s^2
static GLfloat unit_Time = 0.033; 
static tVector avg_epos;
static tVector paddle_v;
static tVector ball_v;
static tVector reflex_v;
static tVector off_v;
static double t=0.0;
static double dt=0.0;
static double minY=0.0, minZ=0.0;

static tVector b_vector;
static tQuaternion b_quat;	
GLfloat ShadowMat[4][4];

///////////////////////////////////////////////////////
// after that, this will load from model database 
enum jointID {
	Hips,
	LeftHip,
	LeftKnee,
	LeftAnkle,
	RightHip,
	RightKnee,
	RightAnkle,
	chest1,
	chest2,
	LeftCollar,
	LeftShoulder,
	LeftElbow,
	LeftWrist,
//	LeftEhand,
	RightCollar,
	RightShoulder,
	RightElbow,
	RightWrist,
//	RightEhand,
	Neck,
	Head
} joint;

// end site computation
static double left_pos[FRAME][3]; //transient array
static double right_pos[FRAME][3]; //transient array
//////////////////////////////////////////////////

//texture
AUX_RGBImageRec *texRec[7];
GLuint tex[7];
/////////////////////////////

double what_time(double dist)
{
	double deg = Ball.beta*PI/180;
    double tt = (dist- Ball.p0.z)/(Ball.v0*cos(deg));

	return tt;
}
// initial ball setting
void init_paddle()
{
   
   paddle_v.x=5;
   paddle_v.y=nRange; //nHeight-nRange; //0.88 m (2.8864) ~ 1m 1m=3.28feet ;30inch
   paddle_v.z=-115;//about 2.9 m 9.512 feet 115 inch
 
}

void init_spidar()
{

	float du_radius =(Ball.radius*1.2)/convert_inch;

	ball_gl.Pos().X()= Ball.p0.x/convert_inch-du_radius; //1m = 39.37 inch 
	ball_gl.Pos().Y()= Ball.p0.y/convert_inch+du_radius; //1m = 39.37 inch 
	ball_gl.Pos().Z()= Ball.p0.z/convert_inch-du_radius; //1m = 39.37 inch 

	spidar.Calib();
	sp_pos.X()= ball_gl.Pos().X()-spidar.GetPos().X(); 
    sp_pos.Y()= ball_gl.Pos().Y()+spidar.GetPos().Y();
    sp_pos.Z()= ball_gl.Pos().Z()-spidar.GetPos().Z();

	spidar2.Calib();
	sp_pos1.X()= ball_gl.Pos().X()-spidar2.GetPos().X(); 
    sp_pos1.Y()= ball_gl.Pos().Y()+spidar2.GetPos().Y();
    sp_pos1.Z()= ball_gl.Pos().Z()-spidar2.GetPos().Z();
  
	printf("spidar: %f Z: %f m Z: %f inch \n", spidar2.GetPos().Z(),sp_pos1.Z(), sp_pos1.Z()*convert_inch);
}

// initial ball setting
void init_ball()
{
   
   t=0.0;
   dt=0.0;
   
   Ball.p0= paddle_v;
   Ball.p1= Ball.p0;

   Ball.dev_pos.x=0.0;
   Ball.dev_pos.y=0.0;
   Ball.dev_pos.z=0.0;

   reflex_v.x=0.0;
   reflex_v.y=0.0;
   reflex_v.z=0.0;

   // ball_gl.Pos().X()= Ball.p1.x/convert_inch; //1m = 39.37 inch 
   // ball_gl.Pos().Y()= Ball.p1.y/convert_inch;
   // ball_gl.Pos().Z()= -Ball.p1.z/convert_inch;
   init_spidar();

   if (chk_detect) chk_detect = 0;
   chk_shoot =0;
   chk_ball = 0; 
   chk_cont = 0;
   minZ=0;
   minY=0;
}

tVector parabola(double alpha, double t)
{
  	  double beta0 = Ball.beta*PI/180; //rad conversion

	  double vh = Ball.v0 *cos(beta0);
	  double vy= Ball.v0*sin(beta0)- g*t;

      ball_v.x = Ball.p0.x+(vh*t*sin(PI*alpha/180));
	  ball_v.y = Ball.p0.y+(Ball.v0*t*sin(beta0)-0.5*g*t*t);
	  ball_v.z = Ball.p0.z+(vh*t*cos(PI*alpha/180));
  
	  minY =ball_v.y - Ball.radius; // distance from ball position to drop position
      return (ball_v);  
}

// check for collision with ground
void roll(double v0,double alpha)//initial velocity, 方位角
{
	double a = -100.0;//加速度（摩擦による減速）
	double v,dist;
	short nn, cnt;

	if(v0 < .0) { v0 = -v0; alpha = 180.0+alpha;}
	nn = (short)(fabs(v0/a)/unit_Time);//止まるまでに予想されるフレーム数

	double t = 0.0;

    	while (cnt < nn){
		cnt++;
		t += unit_Time;
		 //移動距離
		dist = v0*t+0.5*a*t*t;
              Ball.p1.z = Ball.p0.z + (dist * cos(PI * alpha / 180.0));
              Ball.p1.x = Ball.p0.x + (dist * sin(PI * alpha / 180.0));
	      Ball.p1.y = Ball.p0.y + (180.0*2.0*dist/(6*PI));
		v = v0 + a*t;//新しい速度
        if(v <= 0.0f ) break;//終了条件
	}

}	

void reflect(double dt)
{
      double ret_time;
	  double alpha=0.0;
	  double beta0 = (Ball.beta*PI)/180; //rad conversion
      double ref_vel = Ball.v0-35;

	  ret_time = abs(paddle_v.z- off_v.z)/(ref_vel*cos(beta0));
      alpha =acos((off_v.x-paddle_v.x)/(ref_vel *cos(beta0)*ret_time));
	  alpha = 90-((180 * alpha) / PI); //rad->degree
	  double  vh = ref_vel*cos(beta0);
   
	  // vh inverse +=>-
	  reflex_v.x = off_v.x+(-vh*dt*sin(PI*alpha/180)); 
	  reflex_v.y = off_v.y+(ref_vel*dt*sin(beta0)-0.5*g*dt*dt); 
	  reflex_v.z = off_v.z+(-vh*dt*cos(PI*alpha/180)); 

	  minZ =reflex_v.z-Ball.radius; // distance 

}

//ball collision detect
void ballColliding()
{
   
  float distance = JOINT_RAD+Ball.radius;

  avg_epos.x=(left_pos[cur_frm][0]+right_pos[cur_frm][0])/2;
  avg_epos.y=(left_pos[cur_frm][1]+right_pos[cur_frm][1])/2;
  avg_epos.z=(left_pos[cur_frm][2]+right_pos[cur_frm][2])/2;

  float left_rad = sqrt((Ball.p1.x-left_pos[cur_frm][0])*(Ball.p1.x-left_pos[cur_frm][0])+
		(Ball.p1.y-left_pos[cur_frm][1])*(Ball.p1.y-left_pos[cur_frm][1])+
		(Ball.p1.z-left_pos[cur_frm][2])*(Ball.p1.z-left_pos[cur_frm][2]));
  
  float right_rad = sqrt((Ball.p1.x-right_pos[cur_frm][0])*(Ball.p1.x-right_pos[cur_frm][0] )+
		(Ball.p1.y-right_pos[cur_frm][1])*(Ball.p1.y-right_pos[cur_frm][1])+
		(Ball.p1.z-right_pos[cur_frm][2])*(Ball.p1.z-right_pos[cur_frm][2]));
  
  float t_rad = sqrt((Ball.p1.x-avg_epos.x)*(Ball.p1.x-avg_epos.x)+
		(Ball.p1.y-avg_epos.y)*(Ball.p1.y-avg_epos.y)+
		(Ball.p1.z-avg_epos.z)*(Ball.p1.z-avg_epos.z));
 
  if (t_rad < distance || left_rad < distance || right_rad < distance){
	  if(off_frm > 1) chk_detect= 1; // if data is searched, chk_detect = true 
	  off_v.x= (left_pos[off_frm][0]+right_pos[off_frm][0])/2;;
		  off_v.y= (left_pos[off_frm][1]+right_pos[off_frm][1])/2;
		  off_v.z= (left_pos[off_frm][2]+right_pos[off_frm][2])/2;
  }
  else
  { 
	  chk_detect = 0;
	 //if (pre_frm != 0 && off_frm >1 )  frame_num = pre_frm;
  }
}

// local matrix -> global matrix
// I know this isn't good method, but,I may be use transiently for a while (-_-;)
void setup_endeffector(int which, int cur_frame)
{
	float tx, ty, tz, theta, cx, sx, cy, sy, cz, sz;
    int n,imin, imax, ix;
	tVector endVector;
	MRot rot ={1, 0, 0, 0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

	// root translate initial
			k_bone[0].ik_trans.x = mov[cur_frame][0][0]; 
			k_bone[0].ik_trans.y = mov[cur_frame][0][1];
			k_bone[0].ik_trans.z = mov[cur_frame][0][2];
			//rotation
			k_bone[0].ik_rot.z = mov[cur_frame][1][0];
			k_bone[0].ik_rot.x = mov[cur_frame][1][1];
			k_bone[0].ik_rot.y = mov[cur_frame][1][2];

		//chest1
			k_bone[1].ik_trans.x = Joint_Pos[6][0];
			k_bone[1].ik_trans.y = Joint_Pos[6][1];
			k_bone[1].ik_trans.z = Joint_Pos[6][2];
			k_bone[1].ik_rot.z = mov[cur_frame][8][0];
			k_bone[1].ik_rot.x = mov[cur_frame][8][1];
			k_bone[1].ik_rot.y = mov[cur_frame][8][2];
		//chest2	
			k_bone[2].ik_trans.x = Joint_Pos[7][0];
			k_bone[2].ik_trans.y = Joint_Pos[7][1];
			k_bone[2].ik_trans.z = Joint_Pos[7][2];
			k_bone[2].ik_rot.z = mov[cur_frame][9][0];
			k_bone[2].ik_rot.x = mov[cur_frame][9][1];
			k_bone[2].ik_rot.y = mov[cur_frame][9][2];


		  if (which ==0 ) 
		  {  
			 imin = 8;
			 imax = 12; //endsite
		  }
		  else 
		  {
			  imin = 12;
			  imax = 16;//endsite
		  }
		  ix= 3;
		  for( n=imin; n<imax; n++)
		  {
			 k_bone[ix].ik_trans.x = Joint_Pos[n][0];
			 k_bone[ix].ik_trans.y = Joint_Pos[n][1];
			 k_bone[ix].ik_trans.z = Joint_Pos[n][2];
			 
			 k_bone[ix].ik_rot.z = mov[cur_frame][n+2][0];
			 k_bone[ix].ik_rot.x = mov[cur_frame][n+2][1];
			 k_bone[ix].ik_rot.y = mov[cur_frame][n+2][2];
			 ix++;
		  }
		 k_bone[7].ik_trans.y=-3.0;  // end-effector ball_size 

		 endVector.x =0;
		 endVector.y =0;
		 endVector.z =0;

		 for(n=0; n<8;n++)
		 {
			 double local[4][4];
			 double result[4][4];
			 double v[4] ;
			 double v2[4];

			// ROTATE VECTOR COUNTERCLOCKWISE USING CUMULATIVE ROTATIONS
			tx = k_bone[n].ik_trans.x; 
			ty = k_bone[n].ik_trans.y;
			tz = k_bone[n].ik_trans.z;

			v[0] = tx;
			v[1] = ty;
			v[2] = tz;
			v[3] = 1;
			multMatrixVector(rot.Matrix, v, v2);

			// TRANSLATE SYSTEM BACK TO ORIGINAL LOCATION
			endVector.x += v2[0]; endVector.y += v2[1]; endVector.z += v2[2];
  
			// ACCUMULATE ROTATIONS FOR THE FUTURE
			// get local rotations first
			theta = k_bone[n].ik_rot.z*(M_PI/180.0);
			cz = cos(theta); sz = sin(theta);
			theta = k_bone[n].ik_rot.x*(M_PI/180.0);
			cx = cos(theta); sx = sin(theta);
			theta = k_bone[n].ik_rot.y*(M_PI/180.0);
			cy = cos(theta); sy = sin(theta);
			
			//euler->quat
			EulerToQuaternion(&k_bone[n].ik_rot, &k_bone[n].ik_quat);
			//rot order z->x->y

			local[0][0] = cz*cy-sz*sx*sy;
			local[0][1] = -sz*cx;
			local[0][2] = cz*sy+sz*sx*cy;
			local[0][3] = 0;
			
			local[1][0] = sz*cy+cz*sx*sy;
			local[1][1] = cz*cx;
			local[1][2] = sz*sy-cz*sx*cy;
			local[1][3] = 0;

			local[2][0] = -cx*sy;
			local[2][1] = sx;
			local[2][2] = cx*cy;
			local[2][3] = 0;

			local[3][0] = 0;
			local[3][1] = 0;
			local[3][2] = 0;
			local[3][3] = 1;
			
			// accumulate
			multMatrix(rot.Matrix, local, result);
			copyMatrix(result, rot.Matrix);

			k_bone[n].ik_pos.x =endVector.x;
			k_bone[n].ik_pos.y =endVector.y;
			k_bone[n].ik_pos.z =endVector.z;

		 }
if (which == 1)
{
  right_pos[cur_frame][0] = k_bone[7].ik_pos.x;
  right_pos[cur_frame][1] = k_bone[7].ik_pos.y;
  right_pos[cur_frame][2] = k_bone[7].ik_pos.z;

}
else
{
  left_pos[cur_frame][0] = k_bone[7].ik_pos.x;
  left_pos[cur_frame][1] = k_bone[7].ik_pos.y;
  left_pos[cur_frame][2] = k_bone[7].ik_pos.z;
}
 //return endVector;

}

// after databse connection, table open
void conn_motion(int init_type)
{
   double pos[3];
  
   parabola(Ball.angle, key_time);

   pos[0]=ball_v.x;
   pos[1]=ball_v.y;
   pos[2]=ball_v.z;

   //compare ball pos to hand pos
   double key_frame = key_time/unit_Time;
   int ret= select_motion(init_type, key_frame, (double*) pos);
  
   //m_ElapsedTime = timeGetTime();
   //printf( "秒:\t\t\t%u\n", m_ElapsedTime-m_StartTime );

   if (ret !=0 ) // data searched
   {
		for(int i=0;i <= frame_num ;i++)
		{
		  setup_endeffector(0, i);
		}
		for(int i=0;i <= frame_num ;i++)
		{
		  setup_endeffector(1, i);
		}

   }
	 chk_shoot=0;
	  //init frame # =0
     cur_frm = 0;
}

void idle()
{
  if (animating) cur_frm++;
 // check shoot

  if (chk_shoot !=1 &&  abs(Ball.b_force.x) <= 0.1  && Ball.dev_pos.z > 16 && Ball.p0.y <= 60) 
  {
	  chk_shoot = 1;
	  chk_ball=1;
	  if (chk_detect) chk_detect =0;
	  Ball.p0.x += Ball.dev_pos.x;
	  Ball.p0.y += Ball.dev_pos.y;
	  printf( "force: %f, ball shooting pos: x%f  y%f \n",Ball.b_force.x, Ball.p0.x, Ball.p0.y);
  }

 
 if (animating && !chk_detect && chk_ball) 
  {
	  Ball.p1=parabola(Ball.angle, t);
	  
     if (t>=1 && minY <=0 || Ball.p1.z > 50) 
		 init_ball();
	 t +=unit_Time;
  }
  if (animating && chk_detect && chk_ball &&  minZ >= paddle_v.z) 
  {
	  if( cur_frm >= off_frm || chk_cont)
	  {
         reflect(dt);
		 if (minZ < paddle_v.z) 
			 init_ball();
		 dt +=(unit_Time*2);
	  }
  }

 if (animating && chk_shoot ) conn_motion(1); 
 
 if (chk_shoot !=1 && cur_frm >= frame_num ) {
	 if ( minZ < paddle_v.z) 
		 init_ball();
	 else 
	 {
		 if(minZ != 0) chk_cont =1;
		 // throwing motion -> animation	      
	 }
	 if (animating ) { //&& !chk_cont
		End_Interp(cur_frm-1); // 0~ frame_num-1
        conn_motion(0); 
		Com_Interp(cur_frm);
	 } // waiting pose
  }

 if (!chk_detect  && animating) ballColliding();

  glutPostRedisplay();
  //glutTimerFunc(33, idle, 1);
}

void free_resource()
{
  //gluDeleteQuadric(qu);
  //database close
	disconnect();
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
  case 'n': cur_frm++; 
		   if (!chk_detect) t+=unit_Time;
	       printf( "current frame: %d time: %f ball_y:%f hand_y:%f\n", cur_frm, t, Ball.p1.y, right_pos[cur_frm][1]);
		   if (cur_frm >= frame_num) cur_frm = frame_num-1;
		  break;
//previous frame
  case 'p': cur_frm--; 
		    printf( "current frame: %d time: %f ball_y:%f hand_y:%f\n", cur_frm, t, Ball.p1.y, right_pos[cur_frm][1]);	
	       if (cur_frm <= 0) cur_frm = 0;
		   break;
  case ' ': animating = !animating; //space
	      printf( "current frame: %d time: %f ball.x:%f ball_y:%f ball_z:%f key_frame:%f hand_y:%f hand_z:%f \n", cur_frm, t, Ball.p1.x, Ball.p1.y, Ball.p1.z, key_time/unit_Time,right_pos[cur_frm][1], right_pos[cur_frm][2]);
		  break;
  case 'c': 
	   spidar.Calib();
       spidar2.Calib();
	   init_spidar();
	   break;
  case 'f': 
	  full_mode =!full_mode;
	  if(full_mode) glutFullScreen();
	   break;
  case 'r':
	  glutReshapeWindow(400,300);
	  break;
  case '\t':
      chk_shoot = !chk_shoot; // starting ball throw
	  if (chk_shoot) chk_ball=1;
	  if (chk_detect) chk_detect =0;
	  break;
  case 'e': // end_effector selecting
	  chk_end =!chk_end;
	  break;
  case 'b': // ball display selecting
	  chk_ball = !chk_ball;
	  break;
//paddle move 'w':up 's':down 'a':left 'd':right
 case '+': //z
	  Ball.v0 +=1;
	  printf( "ball speed: %f time: %f \n", Ball.v0, t);
	  break;
 case '-': 
	  Ball.v0 -=1;
	  printf( "ball speed: %f time: %f \n", Ball.v0, t);
	  break;
 case 'w': //y
	  paddle_v.y +=delta_Pad;
	  Ball.p0= paddle_v;
	  Ball.p1= Ball.p0;
	  printf( "paddle y pos: %f time: %f \n", paddle_v.y, t);
	  break;
 case 's': 
	  paddle_v.y -=delta_Pad;
	  Ball.p0= paddle_v;
	  Ball.p1= Ball.p0;
	  printf( "paddle y pos: %f time: %f \n", paddle_v.y, t);
	  break;
 case 'a': //x
	  paddle_v.x +=(delta_Pad*5);
	  Ball.p0= paddle_v;
	  Ball.p1= Ball.p0;
	  printf( "paddle x pos: %f time: %f \n", paddle_v.x, t);
	  break;
 case 'd': 
	  paddle_v.x -=(delta_Pad*5);
	  Ball.p0= paddle_v;
	  Ball.p1= Ball.p0;
	  printf( "paddle x pos: %f time: %f \n", paddle_v.x, t);
	  break;
 case 'i': // reset ball
	 init_paddle();
	 init_ball();
     break;
 case 'q':
	 chk_quat = !chk_quat;
	 break;
  case 0x1b: //exit
	free_resource();
	exit(0);
	 break;
  default:
    break;
  }
  glutPostRedisplay();
}

// Drawing joint ball/join
void drawjoint(int cur_frm, int n){
  
	tQuaternion axisAngle;

	if (n==0){ //root translate
        glTranslatef(mov[cur_frm][n][0], mov[cur_frm][n][1], mov[cur_frm][n][2]);
	}
	else{ // all joint except for root
	//

      draw_ellipse(Joint_Pos[n - 1][0],Joint_Pos[n - 1][1],Joint_Pos[n - 1][2]);
      
      glTranslatef(Joint_Pos[n-1][0],
                   Joint_Pos[n-1][1],
                   Joint_Pos[n-1][2]);

    }
    
	if (chk_quat == 1)
	{
		b_vector.z =mov[cur_frm][n + 1][0];
		b_vector.x =mov[cur_frm][n + 1][1];
		b_vector.y =mov[cur_frm][n + 1][2];
		
		EulerToQuaternion(&b_vector, &b_quat);
		QuatToAxisAngle(&b_quat, &axisAngle);
        glRotatef(axisAngle.w, axisAngle.x, axisAngle.y, axisAngle.z); 
	}
    else
	   {
		glRotatef(mov[cur_frm][n + 1][0], 0.0, 0.0, 1.0);  //z
		glRotatef(mov[cur_frm][n + 1][1], 1.0, 0.0, 0.0);  //x
		glRotatef(mov[cur_frm][n + 1][2], 0.0, 1.0, 0.0);  //y
	   }
	// if joint is selected, its color will be change
 	if(n== cur_joint){
	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red); 
	 }
	 else
	 {
		 if (n == Head) 
	        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, headcolor);
		 else 
			 glMaterialfv(GL_FRONT, GL_DIFFUSE, headcolor);
	 }

     glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     glMaterialf(GL_FRONT, GL_SHININESS, 20);

	 if(n == Head){
		glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		  
		  if ( chk_detect !=1 && Ball.p0.x >= 15)
		  	  glBindTexture(GL_TEXTURE_2D, tex[4]);
		  else if ( chk_detect !=1 && Ball.p0.x <= -15)
		  	  glBindTexture(GL_TEXTURE_2D, tex[3]);
		  else if ( chk_detect !=1 && Ball.p0.y <= 15)
		  	  glBindTexture(GL_TEXTURE_2D, tex[6]);
		  else if ( chk_detect !=1 && Ball.p0.y >= 40)
		  	  glBindTexture(GL_TEXTURE_2D, tex[5]);
          else
			 glBindTexture(GL_TEXTURE_2D, tex[0]);

		  glEnable(GL_TEXTURE_2D);
		  glRotatef(90, 0.0f, 1.0f, 0.0f);
		  glRotatef(85, 1.0f, 0.0f, 0.0f);

		  drawSphere(Ball.radius*0.6);	
		glPopMatrix();

	 }
	 else {
	     glDisable(GL_TEXTURE_2D);
		 glutSolidSphere(JOINT_RAD, 15, 12); // joint ball drawing
		
		if (n == RightAnkle || n == LeftAnkle ) // foot box drawing
		{
			   glPushMatrix();
				glTranslatef(0, 0, -1.5);
				glRotatef(-90, 0 ,1,0); 
				drawQuad(4.0, 1.0);
			   glPopMatrix();
		 }
		 else if (n== LeftWrist || n== RightWrist ) //hand end_site drawing
		 {
			  glPushMatrix();
				glScalef(1.0, 1.0, 2.0);
				draw_ellipse(0, -3.14, 0);
			  glPopMatrix();
		 }
  }
}

// all body drawing
void drawBody(int cur_frame)
{
glPushMatrix();
	
     drawjoint(cur_frame, Hips);
    
		glPushMatrix();
			drawjoint(cur_frame, LeftHip);
			drawjoint(cur_frame, LeftKnee);
			drawjoint(cur_frame, LeftAnkle);
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, RightHip);
			drawjoint(cur_frame, RightKnee);
			drawjoint(cur_frame, RightAnkle);
		glPopMatrix();
	    glPushMatrix();
			drawjoint(cur_frame, chest1);
			drawjoint(cur_frame, chest2);
	
			glPushMatrix();
			drawjoint(cur_frame, LeftCollar);
			drawjoint(cur_frame, LeftShoulder);
			drawjoint(cur_frame, LeftElbow);
			drawjoint(cur_frame, LeftWrist);
		
			glPopMatrix();
		
			glPushMatrix();
			drawjoint(cur_frame, RightCollar);
			drawjoint(cur_frame, RightShoulder);
			drawjoint(cur_frame, RightElbow);
			drawjoint(cur_frame, RightWrist);
		    glPopMatrix();

			glPushMatrix();
			drawjoint(cur_frame, Neck);
			drawjoint(cur_frame, Head);
			glPopMatrix();
	glPopMatrix();
glPopMatrix();

}

void DrawShadow(float hh)
{
	float fAmb[4] = { 0.5f,0.5f,0.5f,1 };
	float fDiff[4] = { 0.5f,0.5f,0.5f,1 };
    
	glMaterialfv(GL_FRONT, GL_AMBIENT, fAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, fDiff);
	// Make shadow intensity a function of distance from ground
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();
	   glMultMatrixf((GLfloat *) ShadowMat);
	   drawSphere(Ball.radius);
	glPopMatrix();
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}


void update_physics()
{
      Vec3f dis, dis2;
      Vec3f force, force2;
	  Vec3f pre_sp, pre_sp1;
      float rad = Ball.radius/convert_inch; //0.12m -> 4.8 inch
	  float srad = s_rad/convert_inch; //0.01m ->  inch
      
	  	 //previous point
	    pre_sp.X()= sp_pos.X()-ball_gl.Pos().X(); 
        pre_sp.Y()= sp_pos.Y()-ball_gl.Pos().Y();
        pre_sp.Z()= sp_pos.Z()-ball_gl.Pos().Z();

		pre_sp1.X()= sp_pos1.X()-ball_gl.Pos().X(); 
        pre_sp1.Y()= sp_pos1.Y()-ball_gl.Pos().Y();
        pre_sp1.Z()= sp_pos1.Z()-ball_gl.Pos().Z(); 

	   dis = pre_sp-spidar.GetPos();
       dis2 = pre_sp1-spidar2.GetPos();
				
	   spidar.Update();
	 
	   if (dis.norm() > 0.0f && dis.norm()-rad <= s_rad){
//			force = -K * (rad - dis.norm());
		    force = 0.05f;
			}
		//spidar.SetForce(force, 0.1f);
	
	 spidar2.Update(); 
	
//	 printf( "distance: %f \n", dis2.norm); 

	 if (dis2.norm() > 0.0f && abs(dis2.norm()-rad) <= s_rad){
			//force2 = -K * (rad - dis2.norm());
		  force2 = 0.05f;
			}
	   //spidar2.SetMinForce(force2, 0.1f);
      
     	sp_pos.X()= ball_gl.Pos().X()-spidar.GetPos().X(); 
        sp_pos.Y()= ball_gl.Pos().Y()+spidar.GetPos().Y();
        sp_pos.Z()= ball_gl.Pos().Z()-spidar.GetPos().Z();

		sp_pos1.X()= ball_gl.Pos().X()-spidar2.GetPos().X(); 
        sp_pos1.Y()= ball_gl.Pos().Y()+spidar2.GetPos().Y();
        sp_pos1.Z()= ball_gl.Pos().Z()-spidar2.GetPos().Z();


	 if (chk_shoot!=1 && Ball.b_force.x != force.X() && abs(force.X()) > 0 )
	 {
			 Ball.b_force.x = force.X();
	    	 Ball.dev_pos.x = (sp_pos.X()+sp_pos1.X())/2*convert_inch; //m->cm
			 Ball.dev_pos.y = (sp_pos.Y()+sp_pos1.Y())/2*convert_inch;
		     Ball.dev_pos.z = (sp_pos.Z()+sp_pos1.Z())/2*convert_inch; //z sign , inverse
		
			// printf( "force: %f,  ball pos: x%f  y%f \n",force.X(), Ball.dev_pos.x, Ball.dev_pos.y);
				
	 }
  
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
      drawBody(cur_frm);
      glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[1]);
		  glEnable(GL_TEXTURE_2D);
	      glScalef(50.0, 1.0, 100.0);
		  glTranslatef(-2, 2.5, -0.5);
	      drawQuad(4, 1); 
		  glDisable(GL_TEXTURE_2D);
	 
	  glPopMatrix();
   
	 /* glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[5]);
		  glEnable(GL_TEXTURE_2D);
		  glTranslatef(20, 11, -50);
		  glRotatef(90, 1.0, 0.0, 0.0); 
	      drawQuad(20, 7); 
		  glDisable(GL_TEXTURE_2D);
	 
	  glPopMatrix();*/


	 if (chk_end)
	 {
		// for end site tracking       
		int startFrame = (cur_frm < 50 ? 0: cur_frm - 50);
		for (int frame = startFrame; frame <= cur_frm; frame++)
		{
			glPushMatrix();	   
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
				glTranslatef(right_pos[frame][0],right_pos[frame][1],right_pos[frame][2]); 
				glutSolidCube(1.0);
			glPopMatrix();
		}

	 }
	   //if (chk_ball)
	   //{
		 glPushMatrix();
			//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
			if ((off_frm !=1 && cur_frm >= off_frm && chk_detect) || chk_cont)
			{
				Ball.p1=reflex_v;
			}
			else
			{
				if (chk_detect) {
				   Ball.p1.x= (left_pos[cur_frm][0]+right_pos[cur_frm][0])/2; 
				   Ball.p1.y= (left_pos[cur_frm][1]+right_pos[cur_frm][1])/2;
				   Ball.p1.z= (left_pos[cur_frm][2]+right_pos[cur_frm][2])/2;
				}
				
			}
			glTranslatef(Ball.p1.x, Ball.p1.y, Ball.p1.z);
			// ball_gl.Pos().X()= Ball.p1.x/convert_inch; //1m = 39.37 inch 
			// ball_gl.Pos().Y()= Ball.p1.y/convert_inch;
			// ball_gl.Pos().Z()= -Ball.p1.z/convert_inch;
			glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			drawSphere(Ball.radius);
               
		glPopMatrix();
			glPushMatrix();
	   		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(sp_pos.X()*convert_inch, sp_pos.Y()*convert_inch, sp_pos.Z()*convert_inch);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix();
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(sp_pos1.X()*convert_inch, sp_pos1.Y()*convert_inch, sp_pos1.Z()*convert_inch);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();
/*
		glPushMatrix(); //left
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(paddle_v.x+Ball.radius,  paddle_v.y, paddle_v.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix(); //right
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(paddle_v.x-Ball.radius,  paddle_v.y, paddle_v.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

 	 glPushMatrix();
			glTranslatef(-spidar.GetPos().X()*convert_inch,  Ball.p1.y+spidar.GetPos().Y()*convert_inch, paddle_v.z-spidar.GetPos().Z()*convert_inch);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix();
		
			glTranslatef(-spidar2.GetPos().X()*convert_inch,  Ball.p1.y+spidar2.GetPos().Y()*convert_inch, paddle_v.z-spidar2.GetPos().Z()*convert_inch);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();
*/


//shadow ball
		
        glDisable(GL_CULL_FACE);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
       // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		double hh= 0.5f-(Ball.p1.y*0.01f);
		glPushMatrix();
		    glTranslatef(Ball.p1.x, hh, Ball.p1.z);
			DrawShadow(hh);
            //drawSphere(Ball.radius);
		glPopMatrix();
		
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
        glEnable(GL_CULL_FACE);
		//}

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

// using glaux.h texture load definition(^-^)
AUX_RGBImageRec *LoadBMPFile(char *filename)
{
	FILE *hFile = NULL;

	if(!filename) return NULL;

	hFile = fopen(filename, "r");

	if(hFile) {
		fclose(hFile);
		return auxDIBImageLoad(filename);
	}

	return NULL;
}

void load_texture()
{
// Texture mapping array(^ ^), this files must be same size(ex;256*256)
	memset(texRec, 0, sizeof(void *)*7);

	if((texRec[0]=LoadBMPFile("texture/f_front.bmp")) && 
	   (texRec[1]=LoadBMPFile("texture/ground.bmp")) &&
	   (texRec[2]=LoadBMPFile("texture/basket_ball1.bmp")) &&
	      (texRec[3]=LoadBMPFile("texture/f_left.bmp")) &&
		     (texRec[4]=LoadBMPFile("texture/f_right.bmp")) &&
			    (texRec[5]=LoadBMPFile("texture/f_up.bmp")) &&
	   (texRec[6]=LoadBMPFile("texture/f_down.bmp")))	{   
		
		glGenTextures(7, &tex[0]);
		for(int i=0; i<7; i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  		 gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texRec[i]->sizeX, texRec[i]->sizeY,
                     GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data);
		}
	} //else return FALSE;

	for(int i=0; i<7; i++) {
		if(texRec[i])
		{
			if(texRec[i]->data) free(texRec[i]->data);
			free(texRec[i]);
		} 
	}

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
}

void init(void)
{
 
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 tQuaternion nGround = {0, 1, 0, 0};
 tVector vLight= {-1, 1, 0}; //-1, 1 0
	
 glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
 glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
 glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
 glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);
	
  glClearColor(0.75, 0.75, 1.0, 0.0); 
  glEnable(GL_DEPTH_TEST); //hidden surface removed
 
  glFrontFace(GL_CCW); //no counterclockwise
  glEnable(GL_CULL_FACE); 
  glCullFace(GL_BACK);
 
  load_texture();
	//QUADric
 // qu = gluNewQuadric();
 // gluQuadricNormals(qu, GLU_SMOOTH);
 // gluQuadricTexture(qu, GL_TRUE);

//generate the shadow matrix
	CalcShadowMat(ShadowMat, nGround, vLight);
   // Init connection
   connection();
   init_paddle();	
   Ball.v0 = 85; //80 14 after 1.5 s, 25m
   Ball.beta=30;//15; //shot angle 60 after t=2 z->15m 
   Ball.angle=0.0;
   Ball.radius = 4.8; // 4.8inch => 0.12m =>0.3936feet
   init_ball();	

 // compute database search condition
   key_time = what_time(-1.5); //distance => frame
   load_model(1);
   Throw_Num=random_throw();
   conn_motion(0);
}

int main(int argc, char *argv[])
{
	DVDeviceManager devMan;
     
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	
	devMan.Init();

//	std::cout << devMan;

	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	spidar.Init(devMan, motorPos[0], 0.5776f, 2.924062107079e-5f, 0.5f, 20.0f); 
	spidar2.Init(devMan, motorPos[1], 0.5776f, 2.924062107079e-5f, 0.5f, 20.0f); 

//timing set

  timer.Set(timerfunc, 0);
  timer.Resolution(1);
  timer.Interval(1);
  timer.Thread();

  glutInitWindowSize(400, 300);/* window size 1152, 864*/
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Demonstration of Motion Database");
  
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeyHandler);
  glutIdleFunc(idle);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return(0);
}
