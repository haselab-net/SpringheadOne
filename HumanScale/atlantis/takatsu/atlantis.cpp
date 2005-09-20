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
#include "atlantis.h"
#include <time.h>
#include "kmenu.h"
#include <mpi.h>
//#include <HIS/HISpidar4.h>
//#include <Device/DRUsb20Simple.h>
//#include <winBasis/WBMMTimer.h>
#include <conio.h>
#include <iostream>

//#include "cluster.h"
//

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

GLboolean moving;
//GLboolean cam2;
//GLboolean cam1;
//GLboolean cam0;

static int keyflag=0;
static int key,x,y;
static int value;
static GLint sound_stat = 7;

//static GLint x_view = 0;
//static GLint y_view = 0;
//static GLint z_view = 0;
//static GLdouble view_theta = 90;
//static GLint look_at_x = 120000;
//static GLint look_at_y = 0;
//static GLint look_at_z = 0;
//static GLdouble look_at_r;
//static GLdouble look_at_theta = 90;
static int WIDTH,HEIGHT;
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

/*---------------------*/
int Sound(int);
//timer  
double mytime_old= 0;
double mytime = 0;
int frame_count = 0;

FILE* ad_result;

int shark_moving[NUM_SHARKS];
float P[3]={10000.0,0.0,0.0}; //目標位置
fishRec sharks_org[NUM_SHARKS];
float Ps[3][100000];
int iii;

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
		 shark_moving[i];
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

  // 音声認識結果ファイルを開く
  if((ad_result = fopen("ad_result.txt", "r")) == NULL){
	fprintf(stderr, "Cannot open ad_result file.\n");
	exit(1);
  }
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
		printf( "%5.1f fps\n", rate );

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

    /*2line--------------------------*/
    glDisable(GL_TEXTURE_2D);
    set_material_fish();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTranslatef(0, 0, Rot_Camera[2]);
	glRotatef(Rot_Camera[0], 1.0f, 0.0f, 0.0f);
	glRotatef(Rot_Camera[1], 0.0f, 1.0f, 0.0f);

    for (i = 0; i < NUM_SHARKS; i++) {
        glPushMatrix();
        FishTransform(&sharks[i]);
        DrawShark(&sharks[i]);
        glPopMatrix();
    }

    glPushMatrix();
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, red);
    FishTransform(&dolph);
    DrawDolphin(&dolph);
    glPopMatrix();

    glPushMatrix();
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, green);
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
		if(shark_moving[i]==0){
			sharks[i].stop=0;
			SharkPilot(&sharks[i]);
			SharkMiss(i);
		}
		else if(shark_moving[i]==1){
			MoveShark(i);
//			SharkMiss2(i);
			SharkMiss(i);
		}
		else if(shark_moving[i]==2){
			ReturnShark(i);
//			SharkMiss2(i);
			SharkMiss(i);
		}
		else if(shark_moving[i]==3){
			RotateShark(i);
		}
	}
	printf("%d\n",shark_moving[2]);


  WhalePilot(&dolph);
  dolph.phi++;
  WhalePilot(&momWhale);
  momWhale.phi++;
    WhalePilot(&babyWhale);
    babyWhale.phi++;
}

void
drawall(void)
{
  int result;

  frame_count++;
  //clusterBcast();

  //SOUNDSYSTEM_BEGIN
  /*if(frame_count%30 == 0){
    Sound(sound_stat = CheckSound(sound_stat));
    frame_count=0;
  }else{
    Sound(sound_stat);
    }*/

  //SOUNDSYSTEM_END
 
  // 音声認識結果の読み出し
 /* fscanf( ad_result, "%d\n", &result );
  fprintf(stderr, "%d\n", result);
  fseek(ad_result, 0, SEEK_SET);*/
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
void specialKeyHandler(int key, int x, int y) {
  if (key == GLUT_KEY_LEFT)    
  { 
      Rot_Camera[1] -= deltaRot;
      
  }
  if (key == GLUT_KEY_RIGHT)   
  { 
	  Rot_Camera[1] += deltaRot;
      
  }
  if (key == GLUT_KEY_UP)      {   
	  Rot_Camera[0] += deltaRot;
	  
  }
  if (key == GLUT_KEY_DOWN)    { 
	  Rot_Camera[0] -= deltaRot;
  }
  glutPostRedisplay();
}

void
Key(unsigned char key, int x, int y)
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
  case 'c':
	  FrustumMode = 0;
	  SetFrustum(FrustumMode);
	  break;
    case 'l':
	  FrustumMode = 1;
	  SetFrustum(FrustumMode);
	  break;
  case 'r':
	  FrustumMode = 2;
	  SetFrustum(FrustumMode);
	  break;
  case 'h':
	  Usage();
	  break;
  case '4':
	  shark_movingf();
	  break;
  case 'j':
	  P[1]-=100.0;
	  break;
	case 'k':
	  P[1]+=100.0;
	  break;
  case 27:           /* Esc will quit */
    //clusterexit(0);
    //exit(0);
	  exit_all = 1;
    break;
    
  }
 glutPostRedisplay();
}

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
/*
int
Sound(int sound_stat)
{
   
  switch (sound_stat) {
  case 0:
    exit(1);
    break;
  case 1:
    look_at_x += 300* (int) cos(look_at_theta/RAD);
    look_at_y += 300* (int) sin(look_at_theta/RAD);
    look_at_z -= 0;
    x_view += 300* (int) cos(look_at_theta/RAD);
    y_view += 300* (int) sin(look_at_theta/RAD);
    z_view -= 0;
    break;
  case 2:
    look_at_x -= 300* (int) cos(look_at_theta/RAD) ;
    look_at_y -= 300* (int) sin(look_at_theta/RAD);
    look_at_z -= 0;
    x_view -= 300* (int) cos(look_at_theta/RAD);
    y_view -= 300* (int) sin(look_at_theta/RAD);
    z_view -= 0;
    break;
  case 3:
    look_at_x += 300* (int) sin(look_at_theta/RAD);
    look_at_y -= 300* (int) cos(look_at_theta/RAD);
    look_at_z -= 0;
    x_view += 300* (int) sin(look_at_theta/RAD);
    y_view -= 300* (int) cos(look_at_theta/RAD);
    z_view -= 0;
    break;
  case 4:
    look_at_x -= 300* (int) sin(look_at_theta/RAD);
    look_at_y += 300* (int) cos(look_at_theta/RAD);
    look_at_z -= 0;
    x_view -= 300* (int) sin(look_at_theta/RAD);
    y_view += 300* (int) cos(look_at_theta/RAD);
    z_view -= 0;
   break;
  case 5:
    look_at_x -= 0;
    look_at_y -= 0;
    look_at_z += 300;
    x_view -= 0;
    y_view -= 0;
    z_view += 300;
    break;
  case 6:
    look_at_x -= 0;
    look_at_y -= 0;
    look_at_z -= 300;
    x_view -= 0;
    y_view -= 0;
    z_view -= 300;
    break;
  case 7:
    look_at_x -= 0;
    look_at_y -= 0;
    look_at_z -= 0;
    x_view -= 0;
    y_view -= 0;
    z_view -= 0;
    break;
  case 8:
    cllook_at_theta += 0.4;
    cllook_at_x += (int) look_at_r* (int) cos(look_at_theta/RAD);
    cllook_at_y += (int) look_at_r* (int) sin(look_at_theta/RAD);
    cllook_at_z -= 0;
    break;
   case 9:
    cllook_at_theta -= 0.4;
    cllook_at_x -= (int) look_at_r* (int) cos(look_at_theta/RAD);
    cllook_at_y -= (int) look_at_r* (int) sin(look_at_theta/RAD);
    cllook_at_z -= 0;
    break;
  }
}
*/

//////////////////////////////////////
/////////////MainFunction/////////////
//////////////////////////////////////
//////////////////// SPIDAR 動作確認 ////////////////////
/*void spidarCheck(){
	int step=0;
	if(step == 40){
		// SPIDAR の座標表示
		for(int j = 0; j < 2; j++){
			printf("%d: (x,y,z) = (%f, %f, %f)\n", j, h_spidar[j].GetPos().X(), h_spidar[j].GetPos().Y(), h_spidar[j].GetPos().Z());
		}
		printf("\n");

		// モーターの糸の長さ表示
		/*for(int j = 0; j < 2; j++){
			for(int i = 0; i < 4; i++){
				std::cout << i + 4 * j << ": " << h_spidar[j].Motor()[i].Length()  << " ";
			}
			printf("\n");
		}
		printf("\n");*/
/*		step = 0;
	}
	step++;
}

void update_physics()
{
	Vec3f t_pos[2];

    for(int i = 0; i < 2; i++){
		h_spidar[i].Update();
		t_pos[i]  = h_spidar[i].GetPos();
	}
}
void SpidarInit()
{
	DVDeviceManager devMan;
    
	devMan.RPool().Register(new DRUsb20Simple(0));
	devMan.Init();

	std::cout << devMan;
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-1.68f, 2.00f, 1.15f),	Vec3f( 1.66f, 2.16f,-1.33f), Vec3f(-1.65f,-1.00f,-1.44f), Vec3f( 1.97f,-1.045f, 1.21f)},
		{Vec3f( 1.68f, 2.00f, 1.16f),	Vec3f(-1.65f, 2.16f,-1.33f), Vec3f( 1.65f,-0.99f,-1.44f), Vec3f(-1.95f,-1.035f, 1.205f)}
	};

	for(int i = 0; i < 2; i++){
		h_spidar[i].Init(devMan, motorPos[i], 0.3776f, 2.924062107079e-5f, 1.0f, 20.0f);
		h_spidar[i].Calib();
	}
}
void GetPotision(void){
	int j;
	for(j=0;j<2;j++){
		Ps[0][iii]=h_spidar[j].GetPos().X();
		Ps[1][iii]=h_spidar[j].GetPos().Y();
		Ps[2][iii]=h_spidar[j].GetPos().Z();
	}
	iii++;
	if(iii==100000){
		iii=0;
	}
}



void timerfunc(void* arg){
	update_physics(); 
}
*/
///////////////////////////////////////////////////////////

void PrintCredit(void){
	fprintf(stderr, "Atlantis for Multi-Projection Display   [Ver 1.0]\n");
	fprintf(stderr, "  written by Naoki Hashimoto (naoki@hi.pi.titech.ac.jp)\n\n");
}

int
main(int argc, char **argv)
{
  int myrank;
  cllook_at_r = 2 * 120000 *  (int) sin(0.4/RAD);
  
  //clusterInit(argc,argv);
  //clusterInitWindowSize();
   MPI_Init(&argc, &argv);                  /* 初期化 */
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);  /* プロセッサ番号の取得 */
   FrustumMode = myrank;  
   PrintCredit();

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  //glutInitWindowSize(550, 250); 
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutCreateWindow("GLUT Atlantis Demo");
 
  moving = GL_TRUE;

//  SpidarInit();
  Init();
	
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  if(myrank == 0)
  {
	glutKeyboardFunc(Key);
	glutSpecialFunc(specialKeyHandler);
  }
  glutSetCursor(GLUT_CURSOR_NONE);
  glutCreateMenu(menuSelect);
  glutAddMenuEntry("Start motion", 1);
  glutAddMenuEntry("Stop motion", 2);
  glutAddMenuEntry("Quit", 3);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  //glutFullScreen();
  glutIdleFunc(drawall);
  glutMainLoop();
  //clusterFinalize();

  MPI_Finalize();
  return 0;             /* ANSI C requires main to return int. */
}
