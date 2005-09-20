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
const float K = 0.05;
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
static int cur_joint= 0;
static int chk_detect =0;
static int chk_reflex = 0; 
static int chk_end =0;
static int chk_contiune =0;
static int chk_quat =0;

static int chk_interp = 0;
static int Moving_ball = 0;
static int animating = 1;
static int chk_shoot = 0;
static int chk_hand = 0; //ball and hand collision
// camera 
static float fov = 45.0;//50
static float distZ = 180.0;
////////////////////////////////////////////////
rotInfo camera = { -5.0f, 0.0f, 360.0f, 0.0f, 0.0f, 360.0f, 0.0f, 0.0f, 360.0f };
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
static tVector reflex_v;
static tVector off_v;
static double t=0.0;
static double dt=0.0;
static double minY=0.0, minZ=0.0;

static tVector b_vector;
static tQuaternion b_quat;	
GLfloat ShadowMat[4][4];

GLfloat s_rad = 1;
static tVector grp_pos0;
static tVector grp_pos2;
static tVector user_hand0;
static tVector user_hand2;
static double thand_dis;

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
   
   paddle_v.x=-15;
   paddle_v.y=nRange; //nHeight-nRange; //0.88 m (2.8864) ~ 1m 1m=3.28feet ;30inch
   paddle_v.z=-115;//about 2.9 m 9.512 feet 115 inch
 
}
void init_hand()
{
   user_hand0.x = 0.0;
   user_hand0.y = nRange;
   user_hand0.z = paddle_v.z;
   
   user_hand2 =user_hand0;
   grp_pos0 =user_hand0;
   grp_pos2 =user_hand0;
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
   chk_shoot =0;
   chk_reflex= 0;
   Moving_ball = 0; 
   chk_contiune = 0;
   
   minZ=0;
   minY=0;
}

tVector parabola(double alpha, double t)
{
  	  tVector ball_v;
	  double beta0 = Ball.beta*PI/180; //rad conversion

	  double vh = Ball.v0 *cos(beta0);
	  double vy= Ball.v0*sin(beta0)- g*t;

      ball_v.x = Ball.p0.x+(vh*t*sin(PI*alpha/180));
	  ball_v.y = Ball.p0.y+(Ball.v0*t*sin(beta0)-0.5*g*t*t);
	  ball_v.z = Ball.p0.z+(vh*t*cos(PI*alpha/180));
  
	  minY =ball_v.y - Ball.radius; // distance from ball position to drop position
	  return (ball_v);  
}

void reflect(double dt)
{
      double ret_time;
	  double alpha=0.0;
	  double beta0 = (Ball.beta*PI)/180; //rad conversion
      double ref_vel = Ball.v0-35;
      

	  ret_time = abs(grp_pos0.z- off_v.z)/(ref_vel*cos(beta0));
      alpha =acos((off_v.x-(grp_pos0.x+grp_pos2.x)/2)/(ref_vel *cos(beta0)*ret_time));
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
  tVector l_pos, r_pos;

  l_pos.x=left_pos[cur_frm][0];
  l_pos.y=left_pos[cur_frm][1];
  l_pos.z=left_pos[cur_frm][2];

  r_pos.x = right_pos[cur_frm][0];
  r_pos.y = right_pos[cur_frm][1];
  r_pos.z = right_pos[cur_frm][2];

  avg_epos.x=(l_pos.x+r_pos.x)/2;
  avg_epos.y=(l_pos.y+r_pos.y)/2;
  avg_epos.z=(l_pos.z+r_pos.z)/2;

  float left_rad = VectorDistance(Ball.p1, l_pos);
  float right_rad = VectorDistance(Ball.p1, r_pos);
  float t_rad = VectorDistance(Ball.p1, avg_epos);
 
  if (t_rad < distance || left_rad < distance || right_rad < distance)
  {
	  if(off_frm > 1) 
	  {
		  chk_detect= 1; // if data is searched, chk_detect = true 
		  Moving_ball = 0;
		  //reflect start point
	      off_v.x= (left_pos[off_frm][0]+right_pos[off_frm][0])/2;
	      off_v.y= (left_pos[off_frm][1]+right_pos[off_frm][1])/2;
	      off_v.z= (left_pos[off_frm][2]+right_pos[off_frm][2])/2;
	  }
	  
  }
  else
  { 
	  chk_detect = 0;
	 //if (pre_frm != 0 && off_frm >1 )  frame_num = pre_frm;
  }
}

//hand collision detect
void handColliding()
{
	double t_rad = Ball.radius + s_rad;
	double l_dis = VectorDistance(grp_pos0, Ball.p0); 
    double r_dis = VectorDistance(grp_pos2, Ball.p0); 
 
	if (t_rad >= l_dis && t_rad >= r_dis)
	{
	  chk_hand= 1; 
	  Moving_ball = 0;

  	  //sound effect
	  //FSOUND_PlaySound(FSOUND_FREE, sound_paddle_bounce);
	}
	else
	{ 
	  chk_hand = 0;
	  //Moving_ball = 0 yet ball init pos;1 if chk_shoot;
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
   tVector temp_v;
  
   temp_v= parabola(Ball.angle, key_time);

   pos[0]=temp_v.x;
   pos[1]=temp_v.y;
   pos[2]=temp_v.z;

    //compare ball pos to hand pos
   double key_frame = key_time/unit_Time;
   int ret= select_motion(init_type, key_frame, (double*) pos);
  
   //m_ElapsedTime = timeGetTime();
   //printf( "秒:\t\t\t%u\n", m_ElapsedTime-m_StartTime );
 
   int i;
   if (ret !=0 ) // data searched
   {
		for(i=0;i <= frame_num ;i++)
		{
		  setup_endeffector(0, i);
		}
		for(i=0;i <= frame_num ;i++)
		{
		  setup_endeffector(1, i);
		}

   }
    chk_shoot = 0;
   	//init frame # =0
   cur_frm = 0;
	 
}

void idle()
{
  if (animating) 
  {
	  cur_frm++;

	  if (off_frm != 1 && cur_frm == off_frm && chk_reflex != 1 && chk_detect ) { 
		  chk_reflex = 1;
		  Moving_ball = 1; //start move ball
	  }
	 // check shoot
	 
	  if (chk_shoot !=1 &&  abs(Ball.b_force.x) <= 0.1  && thand_dis > 15 && Ball.p0.y <= 60) 
	  {
		  chk_shoot = 1;
		  Moving_ball=1;

		  //Ball.p0 = Ball.dev_pos;
		  printf( "force: %f, ball shooting pos: x%f  y%f \n",Ball.b_force.x, Ball.p0.x, Ball.p0.y);
	  }
	
 
	 if (!chk_detect && Moving_ball && !chk_reflex) 
	  {
		  Ball.p1=parabola(Ball.angle, t);
		  if (t>=1 && minY <= GnHeight || Ball.p1.z > 50) 
			 init_ball();
		 t +=unit_Time;
	  }
	 // if (chk_detect && !Moving_ball &&  minZ >= paddle_v.z) 
	 // {
		  if(( chk_reflex && cur_frm >= off_frm ) || chk_contiune)
		  {
			 reflect(dt);
			 if (minZ < grp_pos0.z) 
				 init_ball();
			 dt +=(unit_Time*2);
		  }
	 // }

	 if ( chk_shoot )  conn_motion(1); 
	  
	 if (chk_shoot !=1 && cur_frm >= frame_num ) 
	 {
		 if ( minZ < grp_pos0.z) 
			 init_ball();
		 else 
		 {
			 if(minZ != 0) chk_contiune =1;
			 // throwing motion -> animation	      
		 }
			End_Interp(cur_frm-1); // 0~ frame_num-1
			conn_motion(0); 
			Com_Interp(cur_frm);

	  }

	 if (chk_detect != 1 && Moving_ball ==1) ballColliding();
	 //if (chk_hand !=1 && Moving_ball != 1) handColliding();
  }
// animating 
  glutPostRedisplay();
  
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
		   if (chk_detect != 1) t+=unit_Time;
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
	   init_hand();
	   spidar.Calib();
       spidar2.Calib();
	  
	  // init_ball();
	   break;
  case 'f': 
	  full_mode =!full_mode;
	  if(full_mode) glutFullScreen();
	  else   glutReshapeWindow(400,300);
	   break;
  case '\t':
      chk_shoot = 1; 
	  Moving_ball=1;
	  chk_detect =0;
	  chk_hand =0;
	  break;
  case 'e': // end_effector selecting
	  chk_end =!chk_end;
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
 case 'i': // reset ball
	 init_ball();
     break;
 case 'h': // reset hand
	 init_hand();
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

		  drawSphere(Ball.radius/2);	
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
     tVector ss, ss2;
	 double dis, dis2;
     Vec3f force, force2;
     
	 force = Vec3f(0, 0, 0);
     force2 = Vec3f(0, 0, 0);

	 spidar.Update();
     ss.x = spidar.GetPos().X()*convert_inch;
     ss.y = spidar.GetPos().Y()*convert_inch;
     ss.z = spidar.GetPos().Z()*convert_inch;

	spidar2.Update(); 
	 ss2.x = spidar2.GetPos().X()*convert_inch;
     ss2.y = spidar2.GetPos().Y()*convert_inch;
     ss2.z = spidar2.GetPos().Z()*convert_inch;


    if (fabs(ss.x) <= 100 && fabs(ss2.x) <= 100 )
	 {
		 //printf( "spidar.x: %f, y: %f  z%f \n", ss.x, ss.y, ss.z);
		 grp_pos0.x = user_hand0.x - ss.x;
		 grp_pos0.y = user_hand0.y + ss.y;
		 grp_pos0.z = user_hand0.z - ss.z; //x,z sign , inverse
         
		 grp_pos2.x = user_hand2.x - ss2.x;
		 grp_pos2.y = user_hand2.y + ss2.y;
		 grp_pos2.z = user_hand2.z - ss2.z;
		
		 //printf( "hand pos.x: %f, y: %f  z%f \n", grp_pos0.x, grp_pos0.y, grp_pos0.z);
	
  		 dis = VectorDistance(grp_pos0, Ball.p0); 
		 dis2 =VectorDistance(grp_pos2, Ball.p0) ; 
	 	 thand_dis = VectorDistance(grp_pos0, grp_pos2);
	     printf( "dis: %f,  dis2: %f  hand_dis: %f\n",dis, dis2, thand_dis);
		if (dis-Ball.radius < s_rad)
		{
			//force = -K * ((Ball.radius+s_rad - dis)/convert_inch); 
			force = 0.5;
		}
		//spidar.SetForce(force, k_ratio);
		
		if (dis2-Ball.radius < s_rad)
		{
		//	force2 = -K * ((Ball.radius+s_rad - dis2)/convert_inch); 
			force2 = 0.5;
		
		}
	//spidar2.SetForce(force2, k_ratio);

		if (chk_hand !=1 && Moving_ball != 1) handColliding();  
	          	
//b_force,x != force.X()
		if (chk_shoot!=1 && abs(force.X()) >= 0.5 && abs(force2.X()) >= 0.5)
		 {
				 Ball.b_force.x = force.X();
		 		 Ball.dev_pos.x = (grp_pos0.x+grp_pos2.x)/2; 
				 Ball.dev_pos.y = (grp_pos0.y+grp_pos2.y)/2;
				 Ball.dev_pos.z = (grp_pos0.z+grp_pos2.z)/2; 
				
				 Ball.p0 = Ball.dev_pos;
		  	 //printf( "force: %f,  ball pos: x%f  y%f %i\n",force.X(), Ball.dev_pos.x, Ball.dev_pos.y, chk_hand);
		 }
	 }	 
  
}

void timerfunc(void* arg)
{

  update_physics(); 

}


void shadow_ball()
{
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
	      glScalef(100.0, 1.0, 100.0);
		  glTranslatef(-2, GnHeight, -0.5);
	      drawQuad(4, 1); 
		  glDisable(GL_TEXTURE_2D);
	 
	  glPopMatrix();
   
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
// ball display
		 glPushMatrix();

			if ( chk_reflex && Moving_ball || chk_contiune )
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
				else if (!chk_detect && chk_hand ) 
				{
				    Ball.p1 =Ball.p0;
				}
				
			}
			glTranslatef(Ball.p1.x, Ball.p1.y, Ball.p1.z);
			glBindTexture(GL_TEXTURE_2D, tex[2]);
			glEnable(GL_TEXTURE_2D);
			drawSphere(Ball.radius);
               
		glPopMatrix();

		//shadow
		shadow_ball();

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
	
	    glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[6]);
		  glEnable(GL_TEXTURE_2D);
		  glTranslatef(-paddle_v.x, paddle_v.y/2+GnHeight, paddle_v.z);
		  glRotatef(90, 1.0, 0.0, 0.0); 
	      drawQuad(20, 7); 
		  glDisable(GL_TEXTURE_2D);
	 
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

// using glaux.h texture load definition(^-^)
AUX_RGBImageRec *LoadBMPFile(char *filename)
{
	FILE *hFile = NULL;

	if(filename == "") return NULL;

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
	   (texRec[6]=LoadBMPFile("texture/basket.bmp")))	{   
		
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

  //generate the shadow matrix
	CalcShadowMat(ShadowMat, nGround, vLight);
   // Init connection
   connection();
   Ball.v0 = 85; //80 14 after 1.5 s, 25m
   Ball.beta=30;//15; //shot angle 60 after t=2 z->15m 
   Ball.angle=0.0;
   Ball.radius = 4.8; // 4.8inch => 0.12m =>0.3936feet
   
   init_paddle();
   init_hand();
   init_ball();	
   
   //InitFMOD();

 // compute database search condition
   key_time = what_time(-1.5); //distance => frame
   load_model(1);//1
   Throw_Num=random_throw();
   conn_motion(0);
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
	case 4:				// shot ball
	  chk_shoot = 1; // starting ball throw
	  Moving_ball=1;
	  chk_hand = 0;
	  chk_detect =0;
	  break;
	case 5:				
		free_resource();
	    exit(0);
        break;
	default:
		break;
	}
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

	spidar.Calib();
    spidar2.Calib();

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
// menu create
    glutCreateMenu(menu);
    glutAddMenuEntry("          -Virtual Human MENU-", 0);
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
