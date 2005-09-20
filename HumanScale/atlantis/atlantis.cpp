
//
//                        z
//                        |        x   
//                        |       /
//                        |      /
//                        |     /
//                        |    /
//                        |   /
//                        |  /
//                        | /
//                        |/
//---------------------------------------------y
//                      / |
//                     /  |
//                    /   |
//                   /    |
//                  /     |
//                 /      |
//                /       |
//               /        |
//              /         |
//                        |
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include "atlantis.h"
#include "kmenu.h"

//spidar
#include <HIS/HISpidar4Quad.h>  //LVRM
//#include <HIS/HISpidar4.h> //SpringHead
#include <Device/DRUsb20Simple.h>
#include <winBasis/WBMMTimer.h>
#include <conio.h>
#include <iostream>

// lib path 登録
#include "mpi/mpi.h"
///////////////////////////////////
///SOUND
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

using namespace LVRM;
//using namespace Spr; 
using namespace std;

//SPIDAR-H
////////////////////////////
#define ScaleFactor 1000*2 //m-> mm unit
const float K = 10.0f;	// 10n/m
static int IsTouched =0;
static int IsReleased =0;
static int ClapDone =0;
static int SharkTouchDone=0;
static int TouchCount = 0; 
static float s_rad = 100;
bool bMinForce =1;

WBMMTimer timer;
	DVDeviceManager devMan;
    HISpidar4quad h_spidar[2];
UINT SpidarUpdateTimer;
Vec3f grip_pos[2];
void reset_spidar();
void Clap_Event();
void Collision_Fish();
void update_spidar();
void init_spidar();
//for mpi data sending
int FindIndex = -1; //found sharks index
Vec3f FindSharks[NUM_SHARKS];
//////////////////////////
/*----------------*/
GLubyte *texture_image;
GLuint texture_id;

char *texfilename = "floor.ppm";
static GLfloat fog_color[4] = {0.2, 0.2, 0.5, 0.5};
/*---------*/

fishRec sharks[NUM_SHARKS];
fishRec momWhale;
fishRec babyWhale;
fishRec dolph;
fishRec camera0;
fishRec camera1;
fishRec camera2;

bubble bub[NUM_BUBBLES];

GLboolean moving = 1;

static int keyflag=0;
static int key,x,y;
static int value;

static float WIDTH,HEIGHT;
#define  WindowWidth   300
#define  WindowHeight  240
//camera working delta jeong
#define deltaRot 10
GLfloat Rot_Camera[3];

GLboolean toFull = false;

static GLint clx_view = 0;
static GLint cly_view = 0;
static GLint clz_view = 0;
static GLdouble clview_theta = 90.0;
static GLint cllook_at_x = 120000;
static GLint cllook_at_y = 0;
static GLint cllook_at_z = 0;
static GLdouble cllook_at_r;
static GLdouble cllook_at_theta = 0.0;


GLint FrustumMode = 0;
GLint exit_all = 0;
GLint CameraMode = 0;
GLint ScreenMode = 0;
GLboolean ShowPerformance = false;
/*---------------------*/
void InitBubbles();
void InitTexture();
void loadTexture();
void InitFog();
void set_material_bubble();
void set_material_fish();

void Play_wave(char *StrFile);
//timer  
double mytime_old= 0;
double mytime = 0;
int frame_count = 0;

FILE* ad_result;


//2004.10 tagatsu and jeong
//int shark_moving[NUM_SHARKS]; -> sharks[NUM_SHARKS].status
//foat TargetPos[3]={5000.0, 0.0, -500.0};
//float TargetPos[3]={0.0, -500.0, -5000.0};
float TargetPos[3]={5000.0,0.0,0.0};
//{0.0, -1000.0, 5000.0}; //目標位置
fishRec sharks_org[NUM_SHARKS];
/////////////////////////////

//////////////////////////
//JEONG SPIDAR-H
void reset_spidar()
{
	for(int i = 0; i < 2; i++)
	{
		h_spidar[i].Calib();
		grip_pos[i].X() = TargetPos[0]; 
		grip_pos[i].Y() = TargetPos[1];
		grip_pos[i].Z() = TargetPos[2];
	}
	
	IsTouched =0;
	IsReleased =0;
	ClapDone =0;
	SharkTouchDone=0;
	TouchCount = 0; 
}

void init_spidar()
{

	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();

			// 動作確認SPIDAR用 モータの取り付け座標
	#define PX	0.3125f			//	x方向の辺の長さ/2 [m]
	#define PY	0.23f			//	y方向の辺の長さ/2 [m]
	#define PZ	0.265f			//	z方向の辺の長さ/2 [m]
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)},
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}};	

	std::cout << devMan;

	/*Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};*/

	for(int i = 0; i < 2; i++){
		h_spidar[i].Init(devMan, motorPos[i], 0.3776f, 2.924062107079e-5f, 0.5f, 10.0f);
	}
	reset_spidar();

}
void timerfunc(void* arg){
	if (myrank == 0) update_spidar(); 
}
///////////////////////////// *_* ////////// 2004.10.30 jeong
void update_spidar()
{
	Vec3f t_pos[2];
	Vec3f t_force[2];
	float dis[2];

		for(int i = 0; i < 2; i++)
		{
			h_spidar[i].Update();
			t_pos[i]  = h_spidar[i].GetPos()* ScaleFactor;// test すべからず

			grip_pos[i].X() = TargetPos[0] + t_pos[i].X(); //-1
				grip_pos[i].Y() = TargetPos[1] + t_pos[i].Y();
					grip_pos[i].Z() = TargetPos[2] + t_pos[i].Z();
		}
		
	//printf("%f %f \n", h_spidar[0].GetPos().Z(), h_spidar[1].GetPos().Z());
	//	printf("%f %f %f\n", grip_pos[0].Z(), grip_pos[0].Y(), grip_pos[0].X());
//checked collision detection
	   Collision_Fish(); 

		if (ClapDone)
		{
			dis[0] = (grip_pos[0] - grip_pos[1]).norm();
			if (dis[0] < s_rad) t_force[0] = -K * (s_rad - dis[0]);
				
				if(bMinForce == 1) h_spidar[0].SetMinForce();
				else {
					if (t_force[0].norm() > 10) t_force[0] = t_force[0].unit() * 10;
					h_spidar[0].SetForce(t_force[0]);
				}
			
			Play_wave("clap.wav"); //sound/
			shark_movingf();
			ClapDone = 0; //reset
  		
		} else 	Clap_Event(); 

}

void Clap_Event()
{

	float HandDis = ((grip_pos[0] - grip_pos[1]).norm());// * ScaleFactor;
	if (HandDis <= s_rad*2 )
	{
		IsTouched = 1;
	}
	else
	{
		IsReleased = 1;
		if (IsTouched) 
		{
			TouchCount++;
			IsTouched = 0;
			IsReleased = 0;
		}
// if touch and release movements is repeated, Clap!!!
		if (TouchCount >=3) 
		{
			ClapDone = 1;
			TouchCount = 0;
		}
	}
				

}
void Collision_Fish()
{
		
	Vec3f hand_pos = (grip_pos[0] + grip_pos[1])/2;
    float FishDis = 0; 
	Vec3f handForce;

	for(int i = 0; i < NUM_SHARKS; i++)
	{
		FindSharks[i].X() = sharks[i].x;
		FindSharks[i].Y() = sharks[i].y;
		FindSharks[i].Z() = sharks[i].z;

		FishDis = (hand_pos - FindSharks[i]).norm();

		//printf("%f \n", FishDis);
		if (FishDis <= (s_rad*2 + SHARKSIZE/2)) 
		{
			sharks[i].collide =1;
			FindIndex = i; //only one time, meanless
			if ( FindIndex > -1)
			{
				
				  float dd = (hand_pos - FindSharks[FindIndex]).norm();
				  if(dd < (SHARKSIZE/2 + s_rad*2)) 
					handForce = -K * ((SHARKSIZE/2 + s_rad*2) - dd) * (hand_pos - FindSharks[FindIndex]).unit();

				  if(bMinForce == 1) 
				  {
					  h_spidar[0].SetMinForce();
					  h_spidar[1].SetMinForce();
				  }
				  else 
				  {
					if (handForce.norm() > 10) handForce = handForce.unit() * 10;
					h_spidar[0].SetForce(handForce);
					h_spidar[1].SetForce(handForce);
				}
			}
			
			Play_wave("HeartBeat.wav"); //sound/
		}
	}

}
//////////////////////
//InitializeFunction//
//////////////////////

void
InitFishs(void)
{
    int i;

    for (i = 0; i < NUM_SHARKS; i++) {
        sharks[i].x = 100000.0 + rand() % 6000;
//		sharks[i].x=26000.0;
        if(i%2 == 0)sharks[i].y = 60000 + rand() % 6000;
        else sharks[i].y = -60000 - rand() % 6000;
	sharks[i].z = rand() % 6000; 
        sharks[i].psi = rand() % 360 - 180.0;
        sharks[i].v = 1.0;
		sharks[i].status = 0;
		sharks_org[i]=sharks[i];
	}

    dolph.x = 26000.0;
    dolph.y = 0.0;
    dolph.z = 6000.0;
    dolph.psi = 90.0;
    dolph.theta = 0.0;
    dolph.v = 3.0;

    momWhale.x = 50000.0;
    momWhale.y = 0.0;
    momWhale.z = 0.0;
    momWhale.psi = 90.0;
    momWhale.theta = 0.0;
    momWhale.v = 3.0;

    babyWhale.x = 40000.0;
    babyWhale.y = -2000.0;
    babyWhale.z = -2000.0;
    babyWhale.psi = 90.0;
    babyWhale.theta = 0.0;
    babyWhale.v = 3.0;

    camera0.x = 0.0;
    camera0.y = 0.0;
    camera0.z = 0.0;
    camera0.psi = 00.0;
    camera0.theta = 0.0;
    camera0.v = 0.0;

    camera1.x = 20000.0;
    camera1.y = 0.0;
    camera1.z = 0.0;
    camera1.psi = 90.0;
    camera1.theta = 0.0;
    camera1.v = 4.0;

    camera2.x = 60000.0;
    camera2.y = 6000.0;
    camera2.z = 0.0;
    camera2.psi = 90.0;
    camera2.theta = 0.0;
    camera2.v = 1.2;
}

void InitBubbles(void){
  int i;
  
  for (i = 0; i<NUM_BUBBLES ; i++){
    bub[i].x = (rand() % 30000) * 1.5f + 40000;
    bub[i].y = (rand() % 180000) * 2.0f - 30000;
    bub[i].z = rand() % 80000 - 40000;
    bub[i].size = rand() % 50 + 100;/*SHARK_SIZE == 6000*/
    bub[i].r = rand() % 100;
    bub[i].theta = rand() % 30;
    bub[i].v = rand() % 300 + 100;
    bub[i].color = (rand() % 101 )/100;
  }

}

void InitFog(void){
  int fogmode = GL_EXP2;
  GLfloat fog_color[4] = {0.2, 0.2, 0.5, 0.5};
  GLfloat fog_density = 0.000004;
  GLfloat fog_l_start = 1.0;
  GLfloat fog_l_end = 7.0;
  
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE,fogmode);
  glFogfv(GL_FOG_COLOR,fog_color);
  glFogf(GL_FOG_DENSITY, fog_density);
  glFogf(GL_FOG_START, fog_l_start);
  glFogf(GL_FOG_END, fog_l_end);
  glHint(GL_FOG_HINT, GL_FASTEST);
}

void InitTexture(void){
  loadTexture();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bi[0].Height, bi[0].Width, 
	  0, GL_RGB, GL_UNSIGNED_BYTE, images[0]);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CW);
}

void
Init(void)
{
  static float ambient[] =
  {0.1, 0.1, 0.1, 1.0};
  static float diffuse[] =
  {1.0, 1.0, 1.0, 1.0};
  static float position[] =
  {0.0, 1.0, 0.0, 0.0};
  static float lmodel_ambient[] =
  {0.4, 0.4, 0.4, 1.0};
  static float lmodel_localviewer[] =
  {0.0};
  
  glFrontFace(GL_CW);
  
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, lmodel_localviewer);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

   /////////////////////

  set_material_fish();
  InitFishs();
  set_material_bubble();
  InitBubbles();
  InitTexture();
  InitFog();
  
//Init Camera
  Rot_Camera[0]=0.0f;
  Rot_Camera[1]=0.0f;
  Rot_Camera[2]=0.0f;

  glClearColor(fog_color[0],fog_color[1],fog_color[2],fog_color[3]);
//jeong
   PlaySound("dolphin.wav", NULL, SND_ASYNC|SND_LOOP);

}

void set_material_bubble(void){
  static float mat_shininess[] =
    {90.0};
  static float mat_specular[] =
    {1.0, 1.0, 1.0, 1.0};
  static float mat_diffuse[] =
    {0.2, 0.3, 0.7, 0.8};
  static float mat_ambient[] =
    {0.6, 0.6, 0.8, 1.0};
 
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
}

void set_material_fish(void){
  static float mat_shininess[] =
    {90.0};
  static float mat_specular[] =
    {0.8, 0.8, 0.8, 1.0};
  static float mat_diffuse[] =
    {0.46, 0.66, 0.795, 1.0};
  static float mat_ambient[] =
    {0.0, 0.1, 0.2, 1.0};

  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
}

/////////////
///texture///
/////////////
void 
loadTexture(){
  
//  FILE *fp;
// char buf[100];
//  int i, j;
//  int header_lines = 3;

  imgread(0, "floor.bmp");
 // texture_image = (GLubyte *)malloc (sizeof(GLubyte)*TEXTURE_IMAGE_WIDTH * TEXTURE_IMAGE_HEIGHT * 3);
  /*
  if ((fp = fopen(texfilename, "r")) == NULL){
    fprintf(stderr, "Cannot open texture file %s\n", texfilename);
    exit(0);
  }
  
  for(j = 0; j<header_lines; j++){
    fgets(buf, 100 , fp);
    if (*buf == '#') j--;
  }
  
  fread(texture_image, TEXTURE_IMAGE_WIDTH * TEXTURE_IMAGE_HEIGHT * 3, 1 , fp);
  fclose(fp);
  */
  }

void SetFrustum(int mode){
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	switch(mode){
		case 0:
			glFrustum(-1000.0,  1000.0, -1000, 1000, 1000, 1000000);  // Center
			break;
		case 1:
			glFrustum(-3000.0, -1000.0, -1000, 1000, 1000, 1000000);  // Left
			break;
		case 2:
			glFrustum(1000.0, 3000.0, -1000, 1000, 1000, 1000000);  // Right
			break;
	}
	glMatrixMode(GL_MODELVIEW);
}

///////////////////
//ReshapeFunction//
///////////////////
void
Reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    WIDTH = width;
    HEIGHT = height;
    

    //clusterFrustum(-3000.0, 3000.0, -1000, 1000, 1000, 1000000);
    //glFrustum(-3000.0, 3000.0, -1000, 1000, 1000, 1000000);
	SetFrustum(FrustumMode);

}

/////////////////////
//Print_performance//
/////////////////////

void
print_performance( void )
{
	static int first = 1;
	static int start, num_frames;
	int current;

	if ( first )
	{
		start = glutGet( GLUT_ELAPSED_TIME );
		num_frames = 0;
		first = 0;
	}

	num_frames++;
	current = glutGet( GLUT_ELAPSED_TIME );

	if ( current - start > 1000 )
	{
		double elapsed = 1e-3 * (double) ( current - start );
		double rate = (double) num_frames / elapsed;
	//	printf( "%5.1f fps\n", rate );

		num_frames = 0;
		start = current;
	}
}

///////////////////
//DisplayFunction//
///////////////////

void
Display(void)
{
    int i;

   GLfloat green[] = {0.25, 1.0, 0.25, 1.0};
   GLfloat red[] = {1.0, 0.25, 0.25, 1.0};
   GLfloat blue[] = {0.25, 0.25, 1.0, 1.0};
   GLfloat pink[] = {0.85, 0.25, 0.85, 1.0};

    /*2line--------------------------*/
    glDisable(GL_TEXTURE_2D);
    set_material_fish();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotatef(Rot_Camera[0], 1.0f, 0.0f, 0.0f);
	glRotatef(Rot_Camera[1], 0.0f, 1.0f, 0.0f);
    glRotatef(Rot_Camera[2], 0.0f, 0.0f, 1.0f);

    for (i = 0; i < NUM_SHARKS; i++) {
        glPushMatrix();
        FishTransform(&sharks[i]);
        DrawShark(&sharks[i]);

		if (sharks[i].status == 1 )
		{
			glPushMatrix();
		  //glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, blue);
		   glutWireSphere(SHARKSIZE/2,15,12);
	      glPopMatrix();
		}
        glPopMatrix();
    }

	//spidar grip position
	glPushMatrix();
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, red);
		//glTranslatef(TargetPos[1], TargetPos[2], -TargetPos[0]);
		glTranslatef(grip_pos[0].X(), grip_pos[0].Y(), grip_pos[0].Z());
		glutWireSphere(s_rad,15,12);
	glPopMatrix();
	
	glPushMatrix();
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, blue);
		glTranslatef(grip_pos[1].X(), grip_pos[1].Y(), grip_pos[1].Z());
		glutWireSphere(s_rad,15,12);
	glPopMatrix();

    glPushMatrix();
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, green);
    FishTransform(&dolph);
    DrawDolphin(&dolph);
    glPopMatrix();

    glPushMatrix();
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, blue);
    FishTransform(&momWhale);
    DrawWhale(&momWhale);
    glPopMatrix();

    glPushMatrix();
    FishTransform(&babyWhale);
    glScalef(0.45, 0.45, 0.3);
    DrawWhale(&babyWhale);
    glPopMatrix();

    /*---------------------------------*/
    set_material_bubble();
    drawbubbles();

    glEnable(GL_TEXTURE_2D);
    drawplean();
    /*--------------------------------*/

    //clusterSwapBuffers();
	int pastScreenMode = ScreenMode;
    MPI_Bcast(&CameraMode, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ScreenMode, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&exit_all, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    MPI_Bcast(&grip_pos[0].X(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&grip_pos[0].Y(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&grip_pos[0].Z(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&grip_pos[1].X(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&grip_pos[1].Y(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&grip_pos[1].Z(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&IsTouched, 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 
	MPI_Bcast(&IsReleased, 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 
	MPI_Bcast(&ClapDone, 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 
	MPI_Bcast(&SharkTouchDone, 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 
	MPI_Bcast(&TouchCount, 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 

	// grp_pos, spidar_pos broadcast
	//fprintf(stderr, "%d, %d\n", ScreenMode, pastScreenMode);
	if((ScreenMode != pastScreenMode) || toFull){
		toFull = false;
		if(ScreenMode)
			glutFullScreen();
		else
			glutReshapeWindow(WindowWidth, WindowHeight);
	}

    if(exit_all){
    	MPI_Finalize();
	    exit(0);
	} 

	MPI_Barrier(MPI_COMM_WORLD);
    glutSwapBuffers();
	if(ShowPerformance) print_performance( );
}

///////////////////
//AnimateFunction//
///////////////////

void
change_view(void)
{
  glLoadIdentity();

  WhalePilot(&camera1);
  SharkPilot(&camera2);
  switch(CameraMode){
  case 1:
    gluLookAt(camera1.y, camera1.z, -camera1.x, 
	      0.0, 0.0, -120000.0, 
	      0.0, 1.0, 0.0);
	break;
  case 2:
    gluLookAt(camera2.y, camera2.z, -camera2.x, 
	      0.0, 0.0, -120000.0, 
	      0.0, 1.0, 0.0);
	break;
  default:
    //for camera0
    camera0.x = clx_view;
    camera0.y = cly_view;
    camera0.z = clz_view;
    glRotatef(cllook_at_theta,0.0,1.0,0.0);
    //clusterLookAt(camera0.y, camera0.z, -camera0.x,
    gluLookAt(camera0.y, camera0.z, -camera0.x, 
	      cllook_at_y, cllook_at_z, -cllook_at_x, 
	      0.0, 1.0, 0.0);
	break;
  } 
  glutPostRedisplay();
}

void
Animate(void)
{
  int i;
  change_view();
	for (i = 0; i < NUM_SHARKS; i++) {
		if(sharks[i].status== SHARK_MOVE){
			sharks[i].stop=0;
			SharkPilot(&sharks[i]);
			SharkMiss(i);
		}
		else if(sharks[i].status== SHARK_TARGET){
			MoveShark(i);
//			SharkMiss2(i);
			SharkMiss(i);

		}
		else if(sharks[i].status== SHARK_RETURN){
			ReturnShark(i);
//			SharkMiss2(i);
			SharkMiss(i);

		}
		else if(sharks[i].status== SHARK_ROTATE){
			RotateShark(i);
		}
	}


  WhalePilot(&dolph);
  dolph.phi++;
  WhalePilot(&momWhale);
  momWhale.phi++;
    WhalePilot(&babyWhale);
    babyWhale.phi++;
}

void drawall(void)
{
 
  frame_count++;
  //clusterBcast();

  //SOUNDSYSTEM_BEGIN
  if(frame_count%500 == 0){
     Play_wave("bubble.wav"); //sound/
     frame_count=0;
  }
  
  if(moving)Animate();
}

void Usage(void){
	fprintf(stderr, "[Usage]\n");
	fprintf(stderr, "  ESC：終了\n");
	fprintf(stderr, "　０：基準視点\n");
	fprintf(stderr, "　１：視点切替（鯨Watch）\n");
	fprintf(stderr, "　２：視点切替（水中散歩）\n");
	fprintf(stderr, "　------------------------------\n");
	fprintf(stderr, "　ｆ：フルスクリーン・Window表示切替\n");
	fprintf(stderr, "　ｈ：キー操作一覧\n");
	fprintf(stderr, "　------------------------------\n");
	fprintf(stderr, "　ｃ：中央用View\n");
	fprintf(stderr, "　ｒ：右用View\n");
	fprintf(stderr, "　ｌ：左用VIew\n\n");
	fprintf(stderr, "※ 全ての操作は中央のPCにて行ってください\n");

}

/////////////////////
//Interfacefunction//
/////////////////////
//SOUND
//2004.10.13
void Play_wave(char *StrFile)
{
	char openfile[80];
	
	strcpy(openfile, "open ");
	strcat(openfile, StrFile);
	strcat(openfile, " alias WAVEFILE");
// At frist all device have to close
   mciSendString("close all", NULL, 0,0);
  if (mciSendString(openfile, NULL,0,NULL)!=0)
  {
	  //::MessageBox(NULL,"Open wave file Fail!","SOUND CHECK",MB_OK);
  }
  else
  {
		mciSendString("play WAVEFILE", NULL, 0,0);
		mciSendString("set WAVEFILE time format tmsf notisfy", NULL, 0,0); //wait
   // mciSendString("stop WAVEFILE", NULL, 0,0);
   }
}
void Key(unsigned char key, int x, int y)
{
  switch (key) {     
  case '1':        
    CameraMode = 1;
    break;
  case '2':        
   CameraMode = 2;
    break;
  case '0':
   CameraMode = 0;
    break;
  case 'd':
    cllook_at_y = cllook_at_y + 1000;
    cly_view =  cly_view +1000;
    break;
  case 'a':
    cllook_at_y = cllook_at_y - 1000;
    cly_view = cly_view - 1000;
    break;
  case 'w':
    cllook_at_x = cllook_at_x + 1000;
    clx_view = clx_view + 1000;
    break;
  case 's':
    cllook_at_x = cllook_at_x - 1000;
    clx_view = clx_view - 1000;
   break;    
  case 'z':
    cllook_at_z = cllook_at_z + 1000;
    clz_view = clz_view + 1000;
    break;
  case 'x':
    cllook_at_z = cllook_at_z - 1000;
    clz_view = clz_view - 1000;
    break;
  case 'q':
    cllook_at_theta = cllook_at_theta + 0.4;
    break;
   case 'p':
    cllook_at_theta = cllook_at_theta - 0.4;
    break;
  case ' ':          /* space will advance frame */
	 moving = !moving;
    break;
  case 'i':
      InitFishs();
	  break;
  case 'f':
	  ScreenMode = !ScreenMode;
	  toFull = !toFull;
	  break;
  case 'h':
	  Usage();
	  break;
	 /////////////////////SPIDAR 
  case 'm':
   	 bMinForce = !bMinForce;
	 break;
  case 'c':
 	reset_spidar();
	break;
  
	/////////////////////SPIDAR 
  case 't': 
	  shark_movingf();
	  break;
  case 'j':
	  //TargetPos[1]-=500.0;
	  grip_pos[1].X()-=500.0;
	  break;
	case 'k':
	  //TargetPos[1]+=500.0;
	  grip_pos[1].X()+=500.0;
	  break;
	  ////////////////////////
  case 27:           /* Esc will quit */
    //clusterexit(0);
    //exit(0);
	  timer.Release();
	  exit_all = 1;
    break;
    
  }
 glutPostRedisplay();
}

/*
void specialKeyHandler(int skey) 
{
switch (skey)
{
  case GLUT_KEY_LEFT:    
      Rot_Camera[1] -= deltaRot;
	break;
  case GLUT_KEY_RIGHT:   
  	  Rot_Camera[1] += deltaRot;
	break;    
  case GLUT_KEY_UP:
	  Rot_Camera[0] += deltaRot;
	  break;
  case GLUT_KEY_DOWN:    { 
	  Rot_Camera[0] -= deltaRot;
	break;
  ////////// Camera ///////////////////
	case GLUT_KEY_F1: //c
	  FrustumMode = 0;
	  SetFrustum(FrustumMode);
	  break;
    case GLUT_KEY_F2: //l
	  FrustumMode = 1;
	  SetFrustum(FrustumMode);
	  break;
	case GLUT_KEY_F3: //'r':
	  FrustumMode = 2;
	  SetFrustum(FrustumMode);
	 break;
//////////////////////////////////////
}
  glutPostRedisplay();

}*/


void
menuSelect(int value)
{
  switch (value) {
  case 1:
    moving = GL_TRUE;
    glutIdleFunc(drawall);
    break;
  case 2:
    moving = GL_FALSE;;
    glutIdleFunc(drawall);
    break;
  case 3:
    exit(0);
    break;
  }
}
/////////////////////////////////////
/////////////MainFunction/////////////
//////////////////////////////////////
void PrintCredit(void){
	fprintf(stderr, "Atlantis for Multi-Projection Display   [Ver 1.0]\n");
	fprintf(stderr, "  written by Naoki Hashimoto (naoki@hi.pi.titech.ac.jp)\n\n");
}

int
main(int argc, char **argv)
{
  int myrank;
  cllook_at_r = 2 * 120000 *  (int) sin(0.4/RAD);
  
  
   MPI_Init(&argc, &argv);                  /* 初期化 */
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);  /* プロセッサ番号の取得 */
   FrustumMode = myrank;  
   PrintCredit();

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  //glutInitWindowSize(550, 250); 
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutCreateWindow("GLUT Atlantis Demo");
 
 if (myrank == 0) init_spidar();
  Init();
	
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  if(myrank == 0)
  {
	glutKeyboardFunc(Key);
	//glutSpecialFunc(specialKeyHandler);
  }
  glutSetCursor(GLUT_CURSOR_NONE);
  glutCreateMenu(menuSelect);
  glutAddMenuEntry("Start motion", 1);
  glutAddMenuEntry("Stop motion", 2);
  glutAddMenuEntry("Quit", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  //glutFullScreen();
  glutIdleFunc(drawall);

  ////////////////////////////
   //*timing set
	timer.Set(timerfunc, 0);
	timer.Resolution(2);
	timer.Interval(2);
	timer.Thread();
   //*/
  ////////////////////////  

  glutMainLoop();
  //clusterFinalize();

  MPI_Finalize();
  return 0;             /* ANSI C requires main to return int. */
}
