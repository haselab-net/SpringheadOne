#include <stdio.h>
#include <stdlib.h>
#include <GL/glaux.h>
#include <GL/glut.h>
#include <GL/glu.h>								
#include <math.h>
#include "tex.h"       // primitive definition
#include "odbcexec.h"  // database open/close
#include "Ball.h" 
//////////////////////////////////////////////////////////////////////////
// check variables
static int full_mode =0;
static int chk_detect = NOCOLLISION;
static int chk_end =0;
static int chk_interp = 0;
static int animating = 1;
//static int Moving_ball = 0;
//static int BBall.chk_move =0;

CBall BBall;
//////////////////////////////
//lighting & color definition
static GLfloat light0pos[] = {0.0, 200.0, 200.0, 1.0};
static GLfloat light1pos[] = {200.0, 200.0, 0.0, 1.0};

//ball parabola /collision detect variable
static MyVector avg_epos;
static MyVector paddle_v;
static MyVector reflex_v;
static MyVector off_v;
static double t=0.0;
static double dt=0.0;

//////////////////////////////////////////////////////
static GLfloat ShadowMat[4][4];
static GLfloat s_rad = 1;
///////////////////////////////////////////////////////
// initial ball setting
void init_paddle()
{
   
   paddle_v.x=-3.5;
   paddle_v.y=nRange; //nHeight-nRange; //0.88 m (2.8864) ~ 1m 1m=3.28feet ;30inch
   paddle_v.z=-115;//about 2.9 m 9.512 feet 115 inch
 
}
void init_hand()
{
   t_hand.grp_pos0.x = 10.5;
   t_hand.grp_pos0.y = nRange;
   t_hand.grp_pos0.z = paddle_v.z;
   t_hand.grp_pos0.x = t_hand.grp_pos0.x-BBall.radius;
  
   t_hand.grp_pos2 = t_hand.grp_pos0;
   t_hand.grp_pos2.x = t_hand.grp_pos2.x-BBall.radius*2;
}
// initial ball setting
void init_ball()
{
   
   t=0.0;
   dt=0.0;
   
   BBall.Init();
   //Anim.init();		
   //ball pause

   reflex_v.x=0.0;
   reflex_v.y=0.0;
   reflex_v.z=0.0;
   
   chk_detect = NOCOLLISION;
}

//ball collision detect
void ballColliding(int cur_frm)
{
  float distance = JOINT_RAD+BBall.radius;
  MyVector l_pos, r_pos;

  l_pos.x=LA_bone[3].position_global[cur_frm].x;
  l_pos.y=LA_bone[3].position_global[cur_frm].y;
  l_pos.z=LA_bone[3].position_global[cur_frm].z;

  r_pos.x = RA_bone[3].position_global[cur_frm].x;
  r_pos.y = RA_bone[3].position_global[cur_frm].y;
  r_pos.z = RA_bone[3].position_global[cur_frm].z;

  avg_epos.x=(l_pos.x+r_pos.x)/2;
  avg_epos.y=(l_pos.y+r_pos.y)/2;
  avg_epos.z=(l_pos.z+r_pos.z)/2;

  float left_rad = MyVectorDistance(BBall.p1, l_pos);
  float right_rad = MyVectorDistance(BBall.p1, r_pos);
  float t_rad = MyVectorDistance(BBall.p1, avg_epos);
 
  if (t_rad <= distance || left_rad <= distance || right_rad <= distance)
  {
	  if(Anim.Off_frm > 1) 
	  {
		  chk_detect= V_COLLISION; // if data is searched, chk_detect = 1
		  BBall.chk_move = PAUSE;
		  int off_frm = Anim.Off_frm;
		  //reflect start point
	      off_v.x= (LA_bone[3].position_global[off_frm].x+RA_bone[3].position_global[off_frm].x)/2;
	      off_v.y= (LA_bone[3].position_global[off_frm].y+RA_bone[3].position_global[off_frm].y)/2;
	      off_v.z= (LA_bone[3].position_global[off_frm].z+RA_bone[3].position_global[off_frm].z)/2;
	  }
  }
  else
  { 
	  chk_detect = NOCOLLISION;
	 //if (pre_frm != 0 && off_frm >1 )  frame_num = pre_frm;
  }
}

//hand collision detect
void handColliding()
{
	double t_rad = BBall.radius + s_rad;
	double l_dis = MyVectorDistance(t_hand.grp_pos0, BBall.p0); 
    double r_dis = MyVectorDistance(t_hand.grp_pos2, BBall.p0); 
 
	if (t_rad >= l_dis && t_rad >= r_dis)
	{
		chk_detect = H_COLLISION; 
		BBall.chk_move = PAUSE;
	}
	else 
	{
		chk_detect = NOCOLLISION;
		BBall.chk_move = MOVING;
	}
}
// after databse connection, table open
void conn_motion(int init_type, double k_time)
{
   double pos[3];
   MyVector temp_v;
  
   temp_v= BBall.Parabola(BBall.angle, k_time);

   pos[0]=temp_v.x;
   pos[1]=temp_v.y;
   pos[2]=temp_v.z;

   m_StartTime = timeGetTime();
    //compare ball pos to hand pos
   Anim.Key_frm = k_time/Anim.unit_Time;
   int ret= select_motion(init_type, Anim.Key_frm, (double*) pos);
  
   m_ElapsedTime = timeGetTime();
   printf( "\t%u (ms)\n", m_ElapsedTime-m_StartTime );
 
  if (ret !=0 ) // data searched
  {
	  setup_endeffector(LEFT_ARM, Anim.Frame_num);
	  setup_endeffector(RIGHT_ARM, Anim.Frame_num);
  }
    BBall.chk_shoot = NOSHOOT;
   	//init frame # =0
   Anim.cur_frm = 0;
	 
}
void step()
{

   if (Passed(dwTime) > TIME_PER_FRAME)
   {
 	 glutPostRedisplay();
	 dwTime= GetTickCount();	
   }

}

void  idle()
{
 
  if (animating) 
  {
	  Anim.cur_frm++;

	  if (Anim.cur_frm == Anim.Off_frm && BBall.chk_shoot != REFLEX && chk_detect == V_COLLISION) 
	  { 
		  BBall.chk_shoot = REFLEX;
		  BBall.chk_move = MOVING; //start move ball
	  }
	 if (chk_detect == NOCOLLISION && BBall.chk_move == MOVING && BBall.chk_shoot != REFLEX) 
	  {
		  BBall.p1 = BBall.Parabola(BBall.angle, t);
		  if (t>=Anim.Key_time && BBall.minY <= GnHeight || BBall.p1.z > 80) 
			 init_ball();
			t +=Anim.unit_Time;
	  }
	 
	  if(( BBall.chk_shoot == REFLEX && Anim.cur_frm >= Anim.Off_frm ) || BBall.chk_move == CONTINUE)
	  {
		 reflex_v = BBall.Reflect(dt, off_v);
		 if (BBall.minZ < t_hand.grp_pos0.z) 
			 init_ball();
		 dt +=(Anim.unit_Time*2);
	  }
	 

	 if (BBall.chk_shoot == SHOOTED )  
		 conn_motion(1, Anim.Key_time); 
	  
	 if (BBall.chk_shoot != SHOOTED && Anim.cur_frm >= Anim.Frame_num ) 
	 {
		 if ( BBall.minZ < t_hand.grp_pos0.z) 
			 init_ball();
		 else 
		 {
			 if(BBall.minZ != 0) BBall.chk_move = CONTINUE;
			 // throwing motion -> animation	      
		 }
			//End_Interp(cur_frm-1); // 0~ frame_num-1
			conn_motion(0, Anim.Key_time); 
			Com_Interp(Anim.cur_frm);

	  }

	 if (chk_detect != V_COLLISION && BBall.chk_move == MOVING && Anim.cur_frm >= Anim.Key_frm-5) ballColliding(Anim.cur_frm);
	 if (chk_detect == NOCOLLISION && BBall.chk_move != MOVING) handColliding();
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
  if (key == GLUT_KEY_LEFT)    { Anim.camera.y -= deltaRot; }
  if (key == GLUT_KEY_RIGHT)   { Anim.camera.y += deltaRot; }
  if (key == GLUT_KEY_UP)      { Anim.camera.x -= deltaRot; }
  if (key == GLUT_KEY_DOWN)    { Anim.camera.x += deltaRot; }
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){

  switch(key){
  case '.':
	   Anim.camera.z -= deltaRot; 
	   break;
  case ',':
	   Anim.camera.z += deltaRot; 
	   break;
  case 'z':
	  Anim.distZ -= 5;
	  break;
  case 'Z':
	  Anim.distZ += 5;
	  break;
//next frame
  case 'n': Anim.cur_frm++; 
		   if (chk_detect != 1) t+=Anim.unit_Time;
	       printf( "current frame: %d time: %f ball_y:%f hand_y:%f\n", Anim.cur_frm, t, BBall.p1.y, RA_bone[3].position_global[Anim.cur_frm].y);
		   if (Anim.cur_frm >= Anim.Frame_num) Anim.cur_frm = Anim.Frame_num-1;
		  break;
//previous frame
  case 'p': Anim.cur_frm--; 
		    printf( "current frame: %d time: %f ball_y:%f hand_y:%f\n", Anim.cur_frm, t, BBall.p1.y, RA_bone[3].position_global[Anim.cur_frm].y);	
	       if (Anim.cur_frm <= 0) Anim.cur_frm = 0;
		   break;
  case ' ': animating = !animating; //space
	      printf( "current frame: %d time: %f ball.x:%f ball_y:%f ball_z:%f key_frame:%f hand_y:%f hand_z:%f \n", Anim.cur_frm, t, BBall.p1.x, BBall.p1.y, BBall.p1.z, Anim.Key_time/Anim.unit_Time,RA_bone[3].position_global[Anim.cur_frm].y, RA_bone[3].position_global[Anim.cur_frm].z);
		  break;
  case 'f': 
	  full_mode =!full_mode;
	  if(full_mode) glutFullScreen();
	  else   glutReshapeWindow(400,300);
	   break;
  case '\t':
      BBall.chk_shoot = SHOOTED; 
	  BBall.chk_move = MOVING;
	  chk_detect = NOCOLLISION;
	  break;
  case 'e': // end_effector selecting
	  chk_end =!chk_end;
	  break;
 //paddle move 'w':up 's':down 'a':left 'd':right
 case '+': //z
	  BBall.v0.y +=5;
	  printf( "ball speed: %f time: %f \n", BBall.v0.y, t);
	  break;
 case '-': 
	  BBall.v0.y -=5;
	  printf( "ball speed: %f time: %f \n", BBall.v0.y, t);
	  break;
 case 'w': //y
	  t_hand.grp_pos0.y +=delta_Pad;
	  t_hand.grp_pos2.y +=delta_Pad;
	  BBall.p0.y = (t_hand.grp_pos0.y + t_hand.grp_pos2.y)/2;
	  printf( "hand y pos: %f ball y: %f \n", t_hand.grp_pos0.y, BBall.p0.y);
	  break;
 case 's': 
	  t_hand.grp_pos0.y -=delta_Pad;
	  t_hand.grp_pos2.y -=delta_Pad;
	  BBall.p0.y = (t_hand.grp_pos0.y + t_hand.grp_pos2.y)/2;
	  printf( "hand y pos: %f ball y: %f \n", t_hand.grp_pos0.y, BBall.p0.y);
	  break;
 case 'a': //x
	  t_hand.grp_pos0.x +=delta_Pad;
	  t_hand.grp_pos2.x +=delta_Pad;
	  BBall.p0.x = (t_hand.grp_pos0.x + t_hand.grp_pos2.x)/2;
	  printf( "hand x pos: %f ball x: %f \n", t_hand.grp_pos0.x, BBall.p0.x);
	  break;
 case 'd': 
	  t_hand.grp_pos0.x -=delta_Pad;
	  t_hand.grp_pos2.x -=delta_Pad;
	  BBall.p0.x = (t_hand.grp_pos0.x + t_hand.grp_pos2.x)/2;
	  printf( "hand x pos: %f ball x: %f \n", t_hand.grp_pos0.x, BBall.p0.x);
	  break;
 case 'A': //z
	  t_hand.grp_pos0.z +=delta_Pad;
	  t_hand.grp_pos2.z +=delta_Pad;
	  BBall.p0.z = (t_hand.grp_pos0.z + t_hand.grp_pos2.z)/2;
	  printf( "hand z pos: %f ball z: %f \n", t_hand.grp_pos0.z, BBall.p0.z);
	  break;
 case 'D': 
	  t_hand.grp_pos0.z -=delta_Pad;
	  t_hand.grp_pos2.z -=delta_Pad;
	  BBall.p0.z = (t_hand.grp_pos0.z + t_hand.grp_pos2.z)/2;
	  break;
 case 'i': // reset ball
	 init_ball();
     break;
 case 'h': // reset hand
	 init_hand();
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
void drawjoint(int cur_frm, MyBone sk_bone)
{
  
	if (sk_bone.id == Hips){ //root translate
        glTranslatef(sk_bone.trans[cur_frm].x, sk_bone.trans[cur_frm].y, sk_bone.trans[cur_frm].z);
	}
	else{ // all joint except for root
	//
      Draw_Ellipse(sk_bone.b_trans.x, sk_bone.b_trans.y,sk_bone.b_trans.z);
      glTranslatef(sk_bone.b_trans.x, sk_bone.b_trans.y, sk_bone.b_trans.z);
    }

	glRotatef(sk_bone.rot[cur_frm].z, 0.0, 0.0, 1.0);  //z
	glRotatef(sk_bone.rot[cur_frm].x, 1.0, 0.0, 0.0);  //x
	glRotatef(sk_bone.rot[cur_frm].y, 0.0, 1.0, 0.0);  //y
	// if joint is selected, its color will be change
    if (sk_bone.id == Head) 
	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, headcolor);
	else 
	 glMaterialfv(GL_FRONT, GL_DIFFUSE, headcolor);
	
     glMaterialfv(GL_FRONT, GL_SPECULAR, white);
     glMaterialf(GL_FRONT, GL_SHININESS, 20);

	 if(sk_bone.id == Head){
		glPushMatrix();
		  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
		  
		  glBindTexture(GL_TEXTURE_2D, tex[0]);

		  glEnable(GL_TEXTURE_2D);
		  glRotatef(90, 0.0f, 1.0f, 0.0f);
		  glRotatef(85, 1.0f, 0.0f, 0.0f);

		  drawSphere(BBall.radius/2);	
		glPopMatrix();

	 }
	 else {
	     glDisable(GL_TEXTURE_2D);
		 glutSolidSphere(JOINT_RAD, 15, 12); // joint ball drawing
		
		if (sk_bone.id == RightAnkle || sk_bone.id == LeftAnkle ) // foot box drawing
		{
			   glPushMatrix();
				glTranslatef(0, 0, -1.5);
				glRotatef(-90, 0 ,1,0); 
				drawQuad(4.0, 1.0);
			   glPopMatrix();
		 }
		 else if (sk_bone.id== LeftWrist || sk_bone.id== RightWrist ) //hand end_site drawing
		 {
			  glPushMatrix();
				glScalef(1.0, 1.0, 2.0);
				Draw_Ellipse(0, -3.14, 0);
			  glPopMatrix();
		 }
	  }
}// all body drawing

void drawBody(int cur_frame)
{
glPushMatrix();
	
     drawjoint(cur_frame, A_bone[0]); //0
    
		glPushMatrix();
			drawjoint(cur_frame, LL_bone[0]); 
			drawjoint(cur_frame, LL_bone[1]); 
			drawjoint(cur_frame, LL_bone[2]); 
		glPopMatrix();

		glPushMatrix();
			drawjoint(cur_frame, RL_bone[0]);
			drawjoint(cur_frame, RL_bone[1]);
			drawjoint(cur_frame, RL_bone[2]);
		glPopMatrix();

	    glPushMatrix();
			drawjoint(cur_frame, A_bone[1]);
			drawjoint(cur_frame, A_bone[2]);  //8
	
			glPushMatrix();
				drawjoint(cur_frame, LA_bone[0]); //9
				drawjoint(cur_frame, LA_bone[1]);//10
				drawjoint(cur_frame, LA_bone[2]);  //11
				drawjoint(cur_frame, LA_bone[3]);  //12
			glPopMatrix();
		
			glPushMatrix();
				drawjoint(cur_frame, RA_bone[0]);  //13
				drawjoint(cur_frame, RA_bone[1]);  //14
				drawjoint(cur_frame, RA_bone[2]);   //15
				drawjoint(cur_frame, RA_bone[3]);   //16
		    glPopMatrix();

			glPushMatrix();
				drawjoint(cur_frame, H_bone[0]);  //17
				drawjoint(cur_frame, H_bone[1]);  //18
			glPopMatrix();

	glPopMatrix();

glPopMatrix();

}

void display(void)
{
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
  gluLookAt(0.0, 2.0, Anim.distZ, 0.0, 1.0,0.0, 0.0,1.0, 0.0);

  glPushMatrix();

	 glTranslatef(0, -nHeight, -30);
	 glRotatef(Anim.camera.x, 1.0f, 0.0f, 0.0f);
	 glRotatef(Anim.camera.y, 0.0f, 1.0f, 0.0f);
	 glRotatef(Anim.camera.z, 0.0f, 0.0f, 1.0f);

 	 glEnable(GL_LIGHTING);
	 
  //hip -5 
      drawBody(Anim.cur_frm);
// floor display
      glPushMatrix();
	      glBindTexture(GL_TEXTURE_2D, tex[1]);
		  glEnable(GL_TEXTURE_2D);
	      glScalef(100.0, 1.0, 100.0);
		  glTranslatef(-2, GnHeight, -0.5);
	      drawQuad(4, 1);
		  glDisable(GL_TEXTURE_2D);
	  glPopMatrix();
//end effector display   
		if (chk_end)
		 {
			// for end site tracking       
			int startFrame = (Anim.cur_frm < 50 ? 0: Anim.cur_frm - 50);
			for (int frame = startFrame; frame <= Anim.cur_frm; frame++)
			{
				glPushMatrix();	   
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
					glTranslatef(RA_bone[3].position_global[frame].x,RA_bone[3].position_global[frame].y,RA_bone[3].position_global[frame].z); 
					glutSolidCube(1.0);
				glPopMatrix();
			}
		 }
// ball display

			if ( BBall.chk_shoot == REFLEX && BBall.chk_move != PAUSE)
			{
				BBall.p1=reflex_v;
			}
			else
			{
				if (chk_detect == V_COLLISION) {
				   BBall.p1.x= (LA_bone[3].position_global[Anim.cur_frm].x+RA_bone[3].position_global[Anim.cur_frm].x)/2; 
				   BBall.p1.y= (LA_bone[3].position_global[Anim.cur_frm].y+RA_bone[3].position_global[Anim.cur_frm].y)/2;
				   BBall.p1.z= (LA_bone[3].position_global[Anim.cur_frm].z+RA_bone[3].position_global[Anim.cur_frm].z)/2;
				}
				else if (chk_detect == H_COLLISION ) 
				{
				    BBall.p1 =BBall.p0;
				}
				
			}
			BBall.Draw();
			//shadow
			BBall.Shadow_ball(ShadowMat);
//hand display
		glPushMatrix(); //left
	   	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(t_hand.grp_pos0.x,  t_hand.grp_pos0.y, t_hand.grp_pos0.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();

       glPushMatrix(); //right
		    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(t_hand.grp_pos2.x,  t_hand.grp_pos2.y, t_hand.grp_pos2.z);
	 		glutSolidSphere(s_rad, 15, 12);
	   glPopMatrix();
	
	glPopMatrix();

	glutSwapBuffers();
    //Flip();
}

void resize(GLsizei w, GLsizei h){

  GLfloat aspect;

 if(h == 0) { h=1;}

  glViewport(0, 0, w, h);

  aspect = w/h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(Anim.fov, aspect, 1, 1000);
 
}

void init(void)
{
 
 GLfloat	ambientProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	diffuseProperties[]  = {0.5f, 0.5f, 0.5f, 1.0f};
 GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
 tQuaternion nGround(0, 1, 0, 0);
 MyVector vLight(-1, 1, 0); 
	
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
 
  Load_Texture();

//generate the shadow matrix
	CalcShadowMat(ShadowMat, nGround, vLight);
   // Init connection
   connection();
   
   init_paddle();
   init_hand();
   init_ball();	
    
    // compute database search condition
   Anim.Key_time = BBall.WhatTime(-1.5); //distance => frame
   load_model(1);//1

   Anim.Throw_num=random_throw();
   conn_motion(0, Anim.Key_time);

   //SetTimer(TIME_PER_FRAME, step); //Step()ÇåƒÇ—èoÇ∑é¸ä˙
   dwTime = GetTickCount();

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
	  BBall.chk_shoot = SHOOTED; // starting ball throw
	  BBall.chk_move = MOVING;
	  chk_detect = NOCOLLISION;
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

  glutInitWindowSize(400, 300);/* window size 1152, 864*/
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Demonstration of Motion Database");
  
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeyHandler);
  glutIdleFunc(idle);
  //glutTimerFunc(33, step(),1);
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
    glutAddMenuEntry("BBall", 0);
    glutAddMenuEntry("   Init ball", 3);
	glutAddMenuEntry("   Start throw", 4);
	glutAddMenuEntry("------------------------------------------------", 0);
	glutAddMenuEntry("Program", 0);
	glutAddMenuEntry("   Exit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return(0);
}
