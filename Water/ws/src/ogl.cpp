/* OpenGL 

     by Yoshinori Dobashi, Jul. 1998 */

#include <WinBasis/WBSocket.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include <math.h>

#include "ogl.h"
#include "emlimg.h"
#include "ws.h"
#include "SpidarInterface.h"
//#include <sp/spAudio.h>
//#include <sp/spWave.h>

#include <Base/Affine.h>
using namespace Spr;

#define mmTimerPeriod 10

//#define DEBUG

#define VISCOSITY 0.5

#define INFINITY_FORCE 100000
#define PUSH_FORCE     2.1
#define POBJ_SIZE      5.
//#define WATER_DEPTH   50.
#define WATER_DEPTH   25.   // for fishing
#define NUM_SOUND_SOURCE 1000

//#define LURE_VIEW_DISPLAY 0.4
#define WRITE_DATA_MOVEMENT_DIRECTION

#define KEY_ESC 27
#define FORCE_MAX 7
#define TORQUE_MAX 7

/* GLOBAL VARIABLES */
GLOBAL Treald TIME_STEP;
GLOBAL int    TIMER_INTERVAL;
GLOBAL int    MX, MY;
GLOBAL time_t        comp_time;
GLOBAL Treald        *height, dt, dh, xo, yo;
GLOBAL Treald        *u, *v, *pw;
GLOBAL Tpoint3d      *normal;
GLOBAL Tpoint3d      *tvec;
GLOBAL Tcolor3f      *color;
GLOBAL time_t        comp_time;
GLOBAL Trealf        rnd_wave, yflow, flow_scl;

GLOBAL Trealf        *wmotion, max_motion;
GLOBAL int           nmotion;
GLOBAL int           isim_step;

GLOBAL GLfloat       light_pos[4];
GLOBAL GLfloat       water_spec[4];
GLOBAL GLfloat       water_diffuse[4];
GLOBAL GLfloat       water_shin;

GLOBAL int          idebug;
GLOBAL int          sim_type;
GLOBAL FILE *sysout;

GLOBAL Tcamera cam;
GLOBAL Tear    ear;
GLOBAL EMLImg emlimg;
GLOBAL int     ntex;
GLOBAL EMLImg *teximg;
GLOBAL GLuint    *texname;
GLOBAL int    env_map, box_map, ground_map;

// FOR HAPTIC USING SPIDAR
GLOBAL Trealf   *vel_data, ve_max;
GLOBAL Tpoint3f *force_data, *torq_data;
GLOBAL Trealf   *force_abs, *torq_abs;
GLOBAL Tpoint3f  	pos_hap;
GLOBAL int    force_frq;
GLOBAL int    nforce_max;
GLOBAL Trealf pscale, force_max, torq_max;
GLOBAL int    use_spidar;
GLOBAL ThapticObj pobj;

GLOBAL TfluidForceTex *force_tex;
GLOBAL int   nforce_tex;
GLOBAL Trealf dv_force;
GLOBAL int force_calc_interval;
GLOBAL int no_rotation;

GLOBAL ThapticObj boat;
GLOBAL int        copy_oar;
GLOBAL Trod rod; // used for fishing

GLOBAL int       nfrc_set;
GLOBAL TforceSet *frc_set;

GLOBAL TspidarStatus *spidar_pos_rot;
GLOBAL char spidar_file[80];

GLOBAL Tzspan *hsmap;
GLOBAL Trealf  dx_hsmap, dy_hsmap;
GLOBAL int     n_zspan;
GLOBAL Trealf  unit_mass_water;

GLOBAL Tpoint2i *mouse_pos[2];
GLOBAL int sim_mode;

GLOBAL Trealf *wav;
GLOBAL int    nwav;
GLOBAL Tsound sound;
GLOBAL Tsound *stex;
GLOBAL int   nstex;
GLOBAL Treald   *spos, dpos;
//GLOBAL spAudio audio;
GLOBAL char *pcm_data;
GLOBAL char* pcm8;
GLOBAL short* pcm16;

GLOBAL Trealf frc_wav_scl, frc_arrw_scl;

extern int screen;

/*==== OBJECT ====*/
GLOBAL Tobject *obj;
GLOBAL int     nobj, nvtx;
GLOBAL Treald  objBox[2][3];

/* EXTERNAL FUNCTIONS */

/* INTERNAL FUNCTIONS */
void openWindow();
void display(void);
void menu_operation(int val);
void key_operation(unsigned char key, int x, int y);
void buttonAction(int button, int state, int x, int y);
void initSimulation(void);
void initViewpoint(void);
void setCamera(void);
void updateEar();
void mouse_operation(int x, int y);
void writeFrameBmp(int i);
void writeBitmap(char* filename, void* bits, int width, int height, int bpp);
void makeMatrix(Treald vx, Treald vy, Treald vz
               ,Treald rx, Treald ry, Treald rz
               ,Treald roll_ang, Treald rot[3][3]);
void renderCharacter(char *text, int x_pos, int y_pos);
Tpoint3f invtrans(Treald sx, Treald sy, Tcamera *cam);
Tpoint3f getSurfacePos(int x, int y);
void updateWaterSurface(int val);
void updateWaterSurfaceOri(int val);
void drawBottom();
void drawCaustics();
Treald getAreaofTriangle(Tpoint3d p[3]);
void drawWaterSurface();
void drawDropShadow(void);
void drawWave();
void drawObject(int i);
void drawObjBox(int i);

void timerDisplay(int val);

void objWaterIntersection();

void genHSMap();

void timer_drop_sound(int val);
void timer_push_sound(int val);
void timer_sound(int val);
void displayHaptic();
void updateHapticObj();
void updateHapticSource(ThapticObj *hp);
void writeData();
void drawAxis();

int isTouchWall(ThapticObj *ho, Tpoint3f *wall);
void getBuoyancy(ThapticObj *ho, Tpoint3f *buo, Tpoint3f *tbuo);
void getForce(Tpoint3f *force, Tpoint3f *torq);

void moveBoat(Tpoint3f force, Tpoint3f pos);
void moveBoatHase(Tpoint3f force, Tpoint3f pos);
void moveHapticObj(ThapticObj *hp, Tpoint3f force, Tpoint3f pos);
void multMatrix(Trealf *m, Trealf *a, Trealf *b);
void writeMovie();
void experiment();

void writeSpidar();
void readSpidar();

/* EXTERNAL FUNCTION */
extern void simulate();
extern void simulate2();
extern void calPressure(ThapticObj *po, int use_vel);
extern void genRandWave(Trealf wx, Trealf wy, Trealf amp);
extern void shadeWaterSurface();
extern void initWater();
extern int initSpAudio(int frq, int bit, int nsmp, int istereo);
extern void writeWaveMotion();
extern int writeWave();
extern Trealf lerpWaterHeight(Trealf x, Trealf y);
extern Tpoint3f lerpWaterVelocity(Trealf x, Trealf y);

/* INTERNAL VARIABLES */
int winID;
int img_no = 0, iwrt_bmp = FALSE;
int idrag, prev_x, prev_y,iview = MOVE_ROTATE, imode=PUSH_WATER;
int isound = SOUND_EFFECT_OFF;
int count, isim = FALSE;
Tpatch *spatch = NULL;
int  px, py, iwx, iwy;
int kick_sound = FALSE;
Treald volume = 1.0;
int iwav;
int kforce = 0;
unsigned int hap_step = 0;

int disp_obj = TRUE, disp_hsmap = FALSE, disp_point = FALSE, disp_param = FALSE;
int disp_hap_src = FALSE, disp_wave_vel = FALSE;
int ifog = FALSE;

Treald sspos[NUM_SOUND_SOURCE], svpos[NUM_SOUND_SOURCE], ssvol[NUM_SOUND_SOURCE];
int    nss = NUM_SOUND_SOURCE, itex[NUM_SOUND_SOURCE];
int ittt = 0;

int left_oar = FALSE, right_oar = FALSE;
Trealf cpy[16];

Trealf zow[10000];
int now, xow[10000], yow[10000];

int boat_view = FALSE;
int lure_view = FALSE;
int is_ready = FALSE;

Tpoint3f vel_cur;
//Trealf safety_coef = 1.;

Trealf   pobj_mat_hist[500][16];
int      n_pobj_mat_hist = 500, disp_hist = FALSE;

clock_t gpre;

//Tpoint3f uuu, vvv, www;


extern bool bSender;

#define COMPORT 11111
void SendState(Trealf* p, Trealf* rot){
	static bool bFirst = true;
	static WBSocket sock(AF_INET, SOCK_DGRAM, 0);
	static WBSockAddr cl_sin;
	if (bFirst){
		bFirst = false;
		WBNetInterfaces nis;
		nis.Init();
		cl_sin = nis[0].Broadcast();
		cl_sin.AdrIn().sin_port = htons(COMPORT);	//	ポート番号指定
	}
	/*送信バッファ作成*/
	char buf[1024];
	int cur=0;
	memcpy(buf+cur, p, sizeof(Trealf)*3); cur += sizeof(Trealf)*3;
	memcpy(buf+cur, rot, sizeof(Trealf)*9); cur += sizeof(Trealf)*9;
//	memcpy(buf+cur, &boat.pos, sizeof(Vec3f)); cur += sizeof(Vec3f);
	Vec3f dmmy = (Vec3f&)boat.pos;
	dmmy.X() = dmmy.Z() = 0;
	memcpy(buf+cur, &dmmy, sizeof(Vec3f)); cur += sizeof(Vec3f);
	memcpy(buf+cur, &boat.ori, sizeof(Quaternionf)); cur += sizeof(Quaternionf);
	memcpy(buf+cur, &boat.vel, sizeof(Vec3f)); cur += sizeof(Vec3f);
	memcpy(buf+cur, &boat.we, sizeof(Vec3f)); cur += sizeof(Vec3f);
	sock.SendTo(buf, cur, cl_sin);
}
Quaternionf recvOri;
void RecvState(Trealf* p, Trealf* rot){	
	static bool bFirst = true;
	static WBSocket sock(AF_INET, SOCK_DGRAM, 0);
	if (bFirst){
		bFirst = false;
		WBSockAddr adr;
		adr.AdrIn().sin_family = AF_INET;
		adr.AdrIn().sin_port = htons(COMPORT);
		adr.AdrIn().sin_addr.s_addr = htonl(INADDR_ANY);		//	アドレス
		if(bind(sock, (LPSOCKADDR)&adr, sizeof(adr))==SOCKET_ERROR){
			int error = WSAGetLastError();
			closesocket(sock);
			sock = INVALID_SOCKET;
			return;
		}
	}
	DWORD avail=0;
	do{
		ioctlsocket(sock, FIONREAD, &avail);
		if (avail){
			char buf[1024];
			WBSockAddr a;
			int rlen = sock.RecvFrom(buf, sizeof(buf), a);
			int cur=0;
			memcpy(p, buf+cur, sizeof(Trealf)*3); cur += sizeof(Trealf)*3;
			memcpy(rot, buf+cur, sizeof(Trealf)*9); cur += sizeof(Trealf)*9;
			memcpy(&boat.pos, buf+cur, sizeof(Vec3f)); cur += sizeof(Vec3f);
			memcpy(&recvOri, buf+cur, sizeof(Quaternionf)); cur += sizeof(Quaternionf);
			memcpy(&boat.vel, buf+cur, sizeof(Vec3f)); cur += sizeof(Vec3f);
			memcpy(&boat.we, buf+cur, sizeof(Vec3f)); cur += sizeof(Vec3f);
			assert(cur <= rlen);
		}
	} while(avail);
}


void CALLBACK TimeProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2){
//	printf(".");
    displayHaptic();
}

MMRESULT timerID;

void buttonAction(int button, int state, int x, int y) {

    switch(button) {
        case GLUT_LEFT_BUTTON:
           if(state == GLUT_DOWN) {
               idrag = TRUE;
ittt = 0;
               prev_x = x;
               prev_y = y;
               if(imode == PUSH_WATER || imode == DROP_TEAPOT)
                   {px = x; py = y;} //pushWaterSurface(x, y);
               if(imode == DROP_TEAPOT) pobj.pos.z = 0.01;
//               display();
           } else if(state == GLUT_UP) {
               idrag = FALSE;
//               display();
           }
           break;
    }
}

void menu_operation(int val) {
    time_t ttt;

    switch(val) {
      case SIMULATION_START:
        initSimulation();
        if(use_spidar == TRUE)
            timerID = timeSetEvent(mmTimerPeriod, mmTimerPeriod, TimeProc, 0, TIME_PERIODIC);
        comp_time = time(&ttt);
        glutTimerFunc(TIMER_INTERVAL, timerDisplay, TRUE);
/*
        isim_step = 0;
        img_no = 0;
        isim = TRUE;
        iwav = 0;
        initWater();
        if(use_spidar == TRUE) {
            float *p;
            // timer API
            timerID = timeSetEvent(2, 2, TimeProc, 0, TIME_PERIODIC);
            p = SPGetPosition();
            pos_hap.x = p[0];
            pos_hap.y = p[1];
            pos_hap.z = p[2];
            prev_x = p[0];
            prev_y = p[1];
            kforce = 0;
            hap_step = 0;
        }
        pobj.prev_pos.x = pobj.prev_pos.y = pobj.prev_pos.z =
        pobj.pos.x = pobj.pos.y = pobj.pos.z = 0.0;
        prev_x = prev_y = 0.;
        boat.a.x = 
        boat.a.y = 
        boat.a.z = 
        boat.aa.x = 
        boat.aa.y = 
        boat.aa.z = 
        boat.vel.x = 
        boat.vel.y = 
        boat.vel.z = 
        boat.we.x = 
        boat.we.y = 
        boat.we.z = 0.0;
        boat.prev_pos.x = boat.prev_pos.y = boat.prev_pos.z =
        boat.pos.x = boat.pos.y = boat.pos.z = 0.0;
        is_ready = FALSE;
        comp_time = time(&ttt);
        glutTimerFunc(TIMER_INTERVAL, timerDisplay, TRUE);
*/
//        glutTimerFunc(TIMER_INTERVAL, updateWaterSurface, TRUE);
//        printf("used time:%d\n\n", time(&ttt)-comp_time);
//        glutIdleFunc(updateWaterSurface);
        break;
      case SIMULATION_STOP:
        if(use_spidar == TRUE) timeKillEvent(timerID);
        isim = FALSE;
        printf("used time:%d\n\n", time(&ttt)-comp_time);
        display();
        glutTimerFunc(TIMER_INTERVAL, timerDisplay, FALSE);
//        glutTimerFunc(TIMER_INTERVAL, updateWaterSurface, FALSE);
//        glutIdleFunc(NULL);
        break;
      case SIMULATION_REPEAT:
        if(sim_mode == SIMULATION_REPEAT) sim_mode = SIMULATION_ONCE;
        else if(sim_mode == SIMULATION_ONCE) sim_mode = SIMULATION_REPEAT;
        break;
      case PUSH_WATER:
        imode = val;
        break;
      case DROP_TEAPOT:
        imode = val;
        break;
      case DISP_OBJECT:
        if(disp_obj == TRUE) disp_obj = FALSE;
        else if(disp_obj == FALSE) disp_obj = TRUE;
        display();
        break;
      case DISP_HSMAP:
        if(disp_hsmap == TRUE) disp_hsmap = FALSE;
        else if(disp_hsmap == FALSE) disp_hsmap = TRUE;
        display();
        break;
      case DISP_POINTS:
        if(disp_point == TRUE) disp_point = FALSE;
        else if(disp_point == FALSE) disp_point = TRUE;
        display();
        break;
      case DISP_HAPTIC_SRC:
        if(disp_hap_src == TRUE) disp_hap_src = FALSE;
        else if(disp_hap_src == FALSE) disp_hap_src = TRUE;
        display();
        break;
      case DISP_WAVE_VELOCITY:
        if(disp_wave_vel == TRUE) disp_wave_vel = FALSE;
        else if(disp_wave_vel == FALSE) disp_wave_vel = TRUE;
        display();
        break;
      case DISP_PARAM:
        if(disp_param == TRUE) disp_param = FALSE;
        else if(disp_param == FALSE) disp_param = TRUE;
        display();
        break;
      case DISP_ADD_FOG:
        if(ifog == TRUE) {
            ifog = FALSE;
            glDisable(GL_FOG);
        } else if(ifog == FALSE) {
            ifog = TRUE;
            glEnable(GL_FOG);
        }
        display();
        break;
      case SOUND_EFFECT_ON:
        isound = val;
        break;
      case SOUND_EFFECT_OFF:
        isound = val;
        break;
      case MOVE_CAMERA:
        imode = val;
        break;
      case MOVE_ROTATE:
        iview = val;
        break;
      case MOVE_WALK:
        iview = val;
        break;
      case MOVE_APPROACH:
        iview = val;
        break;
      case MOVE_PAN:
        iview = val;
        break;
      case RESET_VIEW:
        initViewpoint();
//        initWater();
        break;
      case PRINT_CAMERA:
        printf("view: %f  %f  %f\n", cam.view[0], cam.view[1], cam.view[2]);
        printf("refe: %f  %f  %f\n", cam.refe[0], cam.refe[1], cam.refe[2]);
        printf("pol : %f  %f  %f\n", cam.vpol[0], cam.vpol[1], cam.vpol[2]);
        break;
      case WRT_BMP:
        if(iwrt_bmp == FALSE) { iwrt_bmp = TRUE; printf("bmp on\n"); }
        else if(iwrt_bmp == TRUE) { iwrt_bmp = FALSE; printf("bmp off\n"); }
        break;
      case WRT_WAV:
        writeWave();
        break;
      case WRT_WAVE_MOTION:
        writeWaveMotion();
        break;
      case WRT_DATA:
        writeData();
        break;
      case WRITE_SPIDAR:
        writeSpidar();
        break;
      case READ_SPIDAR:
        readSpidar();
        break;
      case MOVIE_REPLAY:
        imode = MOVIE_REPLAY;
        initSimulation();
        if(use_spidar == TRUE)
            timerID = timeSetEvent(mmTimerPeriod, mmTimerPeriod, TimeProc, 0, TIME_PERIODIC);
        comp_time = time(&ttt);
        glutTimerFunc(TIMER_INTERVAL, timerDisplay, TRUE);
        break;
      case MOVIE_WRITE:
        imode  =MOVIE_WRITE;
        writeMovie();
        break;
      case QUIT:
        if(use_spidar == TRUE) timeKillEvent(timerID);
        if(sysout) fclose(sysout);
        exit(1);
        break;
      default:
        break;
    }
}

void key_operation(unsigned char key, int x, int y) {
//    int i, j;

    imode = MOVE_CAMERA;
    switch(key) {
        case 'd':
//            imode = DROP_TEAPOT;
            break;
        case 's':
            if(use_spidar == FALSE) imode = PUSH_WATER;
            break;
        case 'w':
            iview = MOVE_WALK;
            break;
        case 'a':
            iview = MOVE_APPROACH;
            break;
        case 'r':
            iview = MOVE_ROTATE;
            break;
        case 't':
            iview = MOVE_TURN;
            break;
        case 'p':
            iview = MOVE_PAN;
            break;
/*
        case ' ':
            Treald v;
            v = (Treald)rand()/(Treald)RAND_MAX*1.5+ 0.5;
            imode = PUSH_WATER;
            j = (int)((Treald)rand()/(Treald)RAND_MAX*(MY-2))+1;
            i = (int)((Treald)rand()/(Treald)RAND_MAX*(MX-2))+1;
            height[IX(i,j)] -= PUSH_FORCE/v;
            kick_sound = TRUE;
            dpos = v;
            volume = 1.;
            for(i=0; i<nstex; i++) spos[i] = 0.0;

            j = FALSE;
            while(i<nss && j == FALSE) {
                if(sspos[i] < 0.0) {
                    sspos[i] = 0.0;
                    svpos[i] = v;
                    j = TRUE;
                }
                i++;
            }

            break;
*/
        case 'b':
            if(boat.id_obj >= 0) {
                if(boat_view == FALSE) boat_view = TRUE;
                else if(boat_view == TRUE) {
                    boat_view = FALSE;
                    initViewpoint();
                }
            }
            display();
        case 'f':
            if(rod.id_obj >= 0) {
                if(lure_view == FALSE) lure_view = TRUE;
                else if(lure_view == TRUE) {
                    lure_view = FALSE;
                    initViewpoint();
                }
            }
            display();
            break;
/*
        case 'g':
            TIME_STEP += 0.001;
            printf("TIME STEP: %f\n", TIME_STEP);
            sound.nsmp = (int)(TIME_STEP*sound.frq);
            sound.ndata = sound.nsmp*(sound.stereo == TRUE ? 2 : 1);
            spFreeAudio(audio)
            spCloseAudioDevice(audio);
            initSpAudio(sound.frq, sound.bit, sound.nsmp, sound.stereo);
            break;
        case 'b':
            TIME_STEP -= 0.001;
            if(TIME_STEP < 0.0000001) TIME_STEP = 0.000001;
            printf("TIME_STEP: %f\n", TIME_STEP);
            sound.nsmp = (int)(TIME_STEP*sound.frq);
            sound.ndata = sound.nsmp*(sound.stereo == TRUE ? 2 : 1);
            spFreeAudio(audio)
            spCloseAudioDevice(audio);
            initSpAudio(sound.frq, sound.bit, sound.nsmp, sound.stereo);
            break;
        case 'f':
            TIMER_INTERVAL += 1;
            printf("TIMER_INTERVAL: %d\n", TIMER_INTERVAL);
            break;
        case 'v':
            TIMER_INTERVAL -= 1;
            if(TIMER_INTERVAL < 1) TIMER_INTERVAL = 1;
            printf("TIMER_INTERVAL: %d\n", TIMER_INTERVAL);
            break;
*/
        case '.':
            if(isim == FALSE) menu_operation(SIMULATION_START);
            else if(isim == TRUE) menu_operation(SIMULATION_STOP);
            break;
        case 'e':
            experiment();
            break;
        case 'x':
            right_oar = left_oar = FALSE;
            printf("both oar\n");
            break;
        case 'z':
            left_oar = TRUE;
            right_oar = FALSE;
            printf("left oar\n");
            break;
        case 'h':
            if(disp_hist == TRUE) disp_hist = FALSE;
            else if(disp_hist == FALSE) disp_hist = TRUE;
            break;
        case 'c':
            left_oar = FALSE;
            right_oar = TRUE;
            printf("right oar\n");
            break;
        case '0':
            initViewpoint();
//            display();
            break;
		case 'F':
			glutFullScreen();
			break;
		case 'C':
			SPCalib();
			break;
        case KEY_ESC:
            menu_operation(QUIT);
            break;
    }
}

void multMatrix(Trealf *m, Trealf *a, Trealf *b) {

    m[0] =  a[0]*b[0]+ a[4]*b[1]+ a[8]*b[2]+ a[12]*b[3];
    m[4] =  a[0]*b[4]+ a[4]*b[5]+ a[8]*b[6]+ a[12]*b[7];
    m[8] =  a[0]*b[8]+ a[4]*b[9]+ a[8]*b[10]+a[12]*b[11];
    m[12] = a[0]*b[12]+a[4]*b[13]+a[8]*b[14]+a[12]*b[15];

    m[1] =  a[1]*b[0]+ a[5]*b[1]+ a[9]*b[2]+ a[13]*b[3];
    m[5] =  a[1]*b[4]+ a[5]*b[5]+ a[9]*b[6]+ a[13]*b[7];
    m[9] =  a[1]*b[8]+ a[5]*b[9]+ a[9]*b[10]+a[13]*b[11];
    m[13] = a[1]*b[12]+a[5]*b[13]+a[9]*b[14]+a[13]*b[15];

    m[2] =   a[2]*b[0]+ a[6]*b[1]+ a[10]*b[2]+ a[14]*b[3];
    m[6] =   a[2]*b[4]+ a[6]*b[5]+ a[10]*b[6]+ a[14]*b[7];
    m[10] =  a[2]*b[8]+ a[6]*b[9]+ a[10]*b[10]+a[14]*b[11];
    m[14] =  a[2]*b[12]+a[6]*b[13]+a[10]*b[14]+a[14]*b[15];

    m[3] =   a[3]*b[0]+ a[7]*b[1]+ a[11]*b[2]+ a[15]*b[3];
    m[7] =   a[3]*b[4]+ a[7]*b[5]+ a[11]*b[6]+ a[15]*b[7];
    m[11] =  a[3]*b[8]+ a[7]*b[9]+ a[11]*b[10]+a[15]*b[11];
    m[15] =  a[3]*b[12]+a[7]*b[13]+a[11]*b[14]+a[15]*b[15];
}

static Tpoint3f w_vel;

//Treald rod_rot[3][3],rod_view[3];
void updateRod(Trod *rod) {
    Trealf *m;
    Trealf d, dxy;
    Tpoint3f pos, uu, vv, ww, spos, heso, root;
    Trealf *p;
 
    if(imode == MOVIE_REPLAY || imode == MOVIE_WRITE) {
        pos_hap = spidar_pos_rot[kforce].pos;
    } else {
        SPUpdate();
        p = SPGetPosition();
        pos_hap.x = p[0];
        pos_hap.y = p[1];
        pos_hap.z = p[2];
    }
    if(sim_mode == SIMULATION_ONCE && imode != MOVIE_REPLAY &&  imode != MOVIE_WRITE) {
        spidar_pos_rot[kforce].pos = pos_hap;
    }

    pos.x = pos_hap.x*pscale;
    pos.y = -pos_hap.z*pscale;
    pos.z = pos_hap.y*pscale;
    spos.x =  pos.x*cam.rot[0][0]-pos.y*cam.rot[2][0]+pos.z*cam.rot[1][0];
    spos.y =  pos.x*cam.rot[0][1]-pos.y*cam.rot[2][1]+pos.z*cam.rot[1][1];
    spos.z =  pos.x*cam.rot[0][2]-pos.y*cam.rot[2][2]+pos.z*cam.rot[1][2];
/*
    spos.x =  pos.x*rod_rot[0][0]-pos.y*rod_rot[2][0]+pos.z*rod_rot[1][0];
    spos.y =  pos.x*rod_rot[0][1]-pos.y*rod_rot[2][1]+pos.z*rod_rot[1][1];
    spos.z =  pos.x*rod_rot[0][2]-pos.y*rod_rot[2][2]+pos.z*rod_rot[1][2];

    heso.x = rod_view[0];
    heso.y = rod_view[1];
    heso.z = rod_view[2]+rod->heso;
*/
    heso.x = cam.view[0];
    heso.y = cam.view[1];
    heso.z = cam.view[2]+rod->heso;
    ww.x = spos.x - heso.x;
    ww.y = spos.y - heso.y;
    ww.z = spos.z - heso.z;
    dxy = ww.x*ww.x+ww.y*ww.y;
    if(dxy < 0.000000001) {
        uu.x = 1.0; uu.y = 0.0; uu.z = 0.0;
        vv.x = 0.0; vv.y = 1.0; vv.z = 0.0;
        ww.x = 0.0; ww.y = 0.0; ww.z = 1.0;
    } else {
        uu.x = ww.y; uu.y = -ww.x; uu.z = 0.0;
        vv.x = ww.x*ww.z; vv.y = ww.y*ww.z; vv.z = -dxy;
        d = dxy+ww.z*ww.z;
        d = sqrt(d);
        dxy = sqrt(dxy);
        ww.x /= d; ww.y /= d; ww.z /= d;
        uu.x /= dxy; uu.y /= dxy;
        d = sqrt(vv.x*vv.x+vv.y*vv.y+vv.z*vv.z);
        vv.x /= d; vv.y /= d; vv.z /= d;
    }
    root.x = heso.x+ww.x*rod->ude;
    root.y = heso.y+ww.y*rod->ude;
    root.z = heso.z+ww.z*rod->ude;
    rod->pos.x = root.x+ww.x*rod->length;
    rod->pos.y = root.y+ww.y*rod->length;
    rod->pos.z = root.z+ww.z*rod->length;
    
    m = obj[rod->id_obj].m;
    m[0] =   uu.x; m[4] = vv.x; m[8] =  ww.x; m[12] = root.x;
    m[1] =   uu.y; m[5] = vv.y; m[9] =  ww.y; m[13] = root.y;
    m[2] =   uu.z; m[6] = vv.z; m[10] = ww.z; m[14] = root.z;
}
void moveLure(ThapticObj *lure, Tpoint3f force, Tpoint3f torq) {
    Trealf  *m, mw[16];
    Tpoint3f fc;

    fc = force;
//    fc.z -= 1;
    moveHapticObj(&pobj, fc, torq);

    m = obj[pobj.id_obj].m;
    multMatrix(mw, obj[pobj.id_obj].m, pobj.m);

    m[0] =  mw[0];  m[1] =  mw[1];  m[2] =  mw[2];  m[3] =  mw[3];
    m[4] =  mw[4];  m[5] =  mw[5];  m[6] =  mw[6];  m[7] =  mw[7];
    m[8] =  mw[8];  m[9] =  mw[9];  m[10] = mw[10]; m[11] = mw[11];
    m[12] = mw[12]; m[13] = mw[13]; m[14] = mw[14]; m[15] = mw[15];

    updateHapticSource(lure);
    m = obj[pobj.id_obj].m;
    rod.lure_cur.x = m[0]*rod.lure.x+m[4]*rod.lure.y+m[8]*rod.lure.z+pobj.pos.x;
    rod.lure_cur.y = m[1]*rod.lure.x+m[5]*rod.lure.y+m[9]*rod.lure.z+pobj.pos.y;
    rod.lure_cur.z = m[2]*rod.lure.x+m[6]*rod.lure.y+m[10]*rod.lure.z+pobj.pos.z;
}

void updateHapticObj() {
    Trealf *p;
    Trealf *rot, *m;

    Trealf mw[16];

    pobj.prev_pos = pobj.pos;
    m = obj[pobj.id_obj].m;
    if(use_spidar == TRUE) {
        //	SPIDARの読み出し
		if(imode == MOVIE_REPLAY || imode == MOVIE_WRITE) {
            pos_hap = spidar_pos_rot[kforce].pos;
            rot = spidar_pos_rot[kforce].rot;
            left_oar = spidar_pos_rot[kforce].left_oar;
            right_oar = spidar_pos_rot[kforce].right_oar;
        } else {
            SPUpdate();
            p = SPGetPosition();
            pos_hap.x = p[0];
            pos_hap.y = p[1];
            pos_hap.z = p[2];
            rot = SPGetRotation();
			if(no_rotation == TRUE) {
                rot[0] = 1.0; rot[1] = 0.0; rot[2] = 0.0;
                rot[3] = 0.0; rot[4] = 1.0; rot[5] = 0.0;
                rot[6] = 0.0; rot[7] = 0.0; rot[8] = 1.0;
            }
        }
        //	通信
		if (bSender){
			SendState(p, rot);	//hase	ネットワークにSPIDARとボートの位置を送信
		}else{
			RecvState(p, rot);
		}

        if(sim_mode == SIMULATION_ONCE && imode != MOVIE_REPLAY &&  imode != MOVIE_WRITE) {
            spidar_pos_rot[kforce].pos = pos_hap;
            spidar_pos_rot[kforce].rot[0] = rot[0];
            spidar_pos_rot[kforce].rot[1] = rot[1];
            spidar_pos_rot[kforce].rot[2] = rot[2];
            spidar_pos_rot[kforce].rot[3] = rot[3];
            spidar_pos_rot[kforce].rot[4] = rot[4];
            spidar_pos_rot[kforce].rot[5] = rot[5];
            spidar_pos_rot[kforce].rot[6] = rot[6];
            spidar_pos_rot[kforce].rot[7] = rot[7];
            spidar_pos_rot[kforce].rot[8] = rot[8];
            spidar_pos_rot[kforce].left_oar = left_oar;
            spidar_pos_rot[kforce].right_oar = right_oar;
        }
        pobj.pos.x = pos_hap.x*pscale;
        pobj.pos.y = -pos_hap.z*pscale;
        pobj.pos.z = pos_hap.y*pscale;
        m[0] = rot[0];
        m[1] = -rot[2];
        m[2] = rot[1];
        m[3] = 0.0;
        m[4] = -rot[6];
        m[5] = rot[8];
        m[6] = -rot[7];
        m[7] = 0.0;
        m[8] = rot[3];
        m[9] = -rot[5];
        m[10] = rot[4];
        m[11] = 0.0;
        m[12] = pobj.pos.x;
        m[13] = pobj.pos.y;
        m[14] = pobj.pos.z;
        m[15] = 1.0;
    } else {
        pobj.pos = getSurfacePos(px, py);
        m[0] = 1.0;
        m[1] = 0.0;
        m[2] = 0.0;
        m[3] = 0.0;
        m[4] = 0.0;
        m[5] = 1.0;
        m[6] = 0.0;
        m[7] = 0.0;
        m[8] = 0.0;
        m[9] = 0.0;
        m[10] = 1.0;
        m[11] = 0.0;
        m[12] = pobj.pos.x;
        m[13] = pobj.pos.y;
        m[14] = pobj.pos.z;
        m[15] = 1.0;
    }

    multMatrix(mw, obj[pobj.id_obj].m, pobj.m);
    
    m[0] =  mw[0];  m[1] =  mw[1];  m[2] =  mw[2];  m[3] =  mw[3];
    m[4] =  mw[4];  m[5] =  mw[5];  m[6] =  mw[6];  m[7] =  mw[7];
    m[8] =  mw[8];  m[9] =  mw[9];  m[10] = mw[10]; m[11] = mw[11];
    m[12] = mw[12]; m[13] = mw[13]; m[14] = mw[14]; m[15] = mw[15];


    if(boat.id_obj >= 0) {
        if(copy_oar == TRUE) {
            cpy[0] = -m[0]; cpy[4] = -m[4]; cpy[8] = -m[8]; cpy[12] = -m[12];
            cpy[1] = m[1]; cpy[5] = m[5]; cpy[9] = m[9]; cpy[13] = m[13];
            cpy[2] = m[2]; cpy[6] = m[6]; cpy[10] = m[10]; cpy[14] = m[14];
            cpy[3] = m[3]; cpy[7] = m[7]; cpy[11] = m[11]; cpy[15] = m[15];
    
            multMatrix(mw, obj[boat.id_obj].m, cpy);
            cpy[0] = mw[0]; cpy[4] = mw[4]; cpy[8] = mw[8]; cpy[12] = mw[12];
            cpy[1] = mw[1]; cpy[5] = mw[5]; cpy[9] = mw[9]; cpy[13] = mw[13];
            cpy[2] = mw[2]; cpy[6] = mw[6]; cpy[10] = mw[10]; cpy[14] = mw[14];
            cpy[3] = mw[3]; cpy[7] = mw[7]; cpy[11] = mw[11]; cpy[15] = mw[15];
        }
        // copyのマトリックスおかしくなってます。
        multMatrix(mw, obj[boat.id_obj].m, m);
        
        m[0] =  mw[0];  m[1] =  mw[1];  m[2] =  mw[2];  m[3] =  mw[3];
        m[4] =  mw[4];  m[5] =  mw[5];  m[6] =  mw[6];  m[7] =  mw[7];
        m[8] =  mw[8];  m[9] =  mw[9];  m[10] = mw[10]; m[11] = mw[11];
        m[12] = mw[12]; m[13] = mw[13]; m[14] = mw[14]; m[15] = mw[15];
        pobj.pos.x = m[12];
        pobj.pos.y = m[13];
        pobj.pos.z = m[14];
    }

    pobj.vel.x = (pobj.pos.x - pobj.prev_pos.x) + w_vel.x;
    pobj.vel.y = (pobj.pos.y - pobj.prev_pos.y) + w_vel.y;
    pobj.vel.z = (pobj.pos.z - pobj.prev_pos.z) + w_vel.z;
    pobj.v = sqrt(pobj.vel.x*pobj.vel.x+pobj.vel.y*pobj.vel.y+pobj.vel.z*pobj.vel.z);
    pobj.vel.x /= force_calc_interval;
    pobj.vel.y /= force_calc_interval;
    pobj.vel.z /= force_calc_interval;
    pobj.v /= force_calc_interval;

    vel_cur = pobj.vel;
    updateHapticSource(&pobj);

}

void updateHapticSource(ThapticObj *hp) {
    int i;
    Trealf dx, dy, dz, xpre, ypre, zpre, *m, dt;
    TsamplePoint *sp;

    dt = pscale/force_frq;
    m = obj[hp->id_obj].m;
    for(i=0; i<hp->nhsrc; i++) {
        xpre = hp->hsrc[i].x;
        ypre = hp->hsrc[i].y;
        zpre = hp->hsrc[i].z;
        hp->hsrc[i].x = m[0]*hp->hsrc[i].x_ori+m[4]*hp->hsrc[i].y_ori+m[8]*hp->hsrc[i].z_ori+m[12];
        hp->hsrc[i].y = m[1]*hp->hsrc[i].x_ori+m[5]*hp->hsrc[i].y_ori+m[9]*hp->hsrc[i].z_ori+m[13];
        hp->hsrc[i].z = m[2]*hp->hsrc[i].x_ori+m[6]*hp->hsrc[i].y_ori+m[10]*hp->hsrc[i].z_ori+m[14];
        dx = (hp->hsrc[i].x - xpre)/dt + w_vel.x;
        dy = (hp->hsrc[i].y - ypre)/dt + w_vel.y;
        dz = (hp->hsrc[i].z - zpre)/dt + w_vel.z;

        hp->hsrc[i].v = dx*dx + dy*dy + dz*dz;
        hp->hsrc[i].vx = dx;
        hp->hsrc[i].vy = dy;
        hp->hsrc[i].vz = dz;
        
        if(hp->hsrc[i].v > 0.0) hp->hsrc[i].v = sqrt(hp->hsrc[i].v);

        hp->hsrc[i].vx /= force_calc_interval;
        hp->hsrc[i].vy /= force_calc_interval;
        hp->hsrc[i].vz /= force_calc_interval;
        hp->hsrc[i].v /= force_calc_interval;

    }

   sp = hp->sp;
   for(i=0; i<hp->n_sp; i++) {
        sp[i].p[0] = m[0]*sp[i].p0[0]+m[4]*sp[i].p0[1]+m[8]*sp[i].p0[2]+m[12];
        sp[i].p[1] = m[1]*sp[i].p0[0]+m[5]*sp[i].p0[1]+m[9]*sp[i].p0[2]+m[13];
        sp[i].p[2] = m[2]*sp[i].p0[0]+m[6]*sp[i].p0[1]+m[10]*sp[i].p0[2]+m[14];
        
        sp[i].n[0] = m[0]*sp[i].n0[0]+m[4]*sp[i].n0[1]+m[8]*sp[i].n0[2];
        sp[i].n[1] = m[1]*sp[i].n0[0]+m[5]*sp[i].n0[1]+m[9]*sp[i].n0[2];
        sp[i].n[2] = m[2]*sp[i].n0[0]+m[6]*sp[i].n0[1]+m[10]*sp[i].n0[2];
   }
}


Tpoint3f lerpForce2(Trealf the, Trealf phi, Trealf t, Tpoint3f *vp, TforceSet *fset, Tpoint3f *pres, Tpoint3f *fric) {

    int ithe, iphi, k00, k01, k10, k11;
    Tpoint3f prs0, prs1, prs, fri0, fri1, fri, uu, vv, ww;
    Trealf ft, fp, fthe, fphi;
//    Tpoint3f peri_prs, peri_fri, phas_prs, phas_fri;
    Trealf   a; //, b, c, d;
    TfluidForceTex *frc;

    vv.x = -vp->x; vv.y = -vp->y; vv.z = -vp->z;
//    vv.x = vp->x; vv.y = vp->y; vv.z = vp->z;
    uu.x = vv.y; uu.y = -vv.x; uu.z = 0.0;
    a = uu.x*uu.x + uu.y*uu.y;
    if(a < 0.000000001) {
        if(vv.z > 0.) {
            uu.x = 1.0; uu.y = 0.0; uu.z = 0.0;
            ww.x = 0.0; ww.y = -1.0; ww.z = 0.0;
        } else {
            uu.x = 1.0; uu.y = 0.0; uu.z = 0.0;
            ww.x = 0.0; ww.y = 1.0; ww.z = 0.0;
        }
    } else {
        a = sqrt(a);
        uu.x /= a; uu.y /=a;
        ww.x = uu.y*vv.z;
        ww.y = -uu.x*vv.z;
        ww.z = uu.x*vv.y-uu.y*vv.x;
        a = ww.x*ww.x + ww.y*ww.y + ww.z*ww.z;
        a = sqrt(a);
        ww.x /= a; ww.y /= a; ww.z /= a;
    }
/*
    uu.x *= 4;
    uu.y *= 4;
    uu.z *= 4;
    ww.x *= 4;
    ww.y *= 4;
    ww.z *= 4;
*/
//vv.x = vv.y = vv.z = 0.;

    fthe = the/fset->dthe;
    fphi = phi/fset->dphi;

    ithe = (int)fthe;
    iphi = (int)fphi;
    ft = fthe - ithe;
    fp = fphi - iphi;

    if(ithe > 0) {
        k00 = (ithe-1)*fset->nphi+1+iphi;
        if(iphi < fset->nphi-1)
            k01 = k00+1;
        else
            k01 = k00 - iphi;
        k10 = k00+fset->nphi;
        k11 = k10+1;
        if(k10 >= fset->ntex) k10 = fset->ntex-1;
        if(k11 >= fset->ntex) k11 = fset->ntex-1;
    } else {
        k00 = 0;
        k01 = 0;
        k10 = iphi+1;
        if(iphi < fset->nphi-1)
            k11 = k10+1;
        else
            k11 = 1;
    }
    Tpoint3f prs00, prs01, prs10, prs11, fri00, fri01, fri10, fri11;
    
    prs00.x = prs00.y = prs00.z = 0.0;
    prs01.x = prs01.y = prs01.z = 0.0;
    prs10.x = prs10.y = prs10.z = 0.0;
    prs11.x = prs11.y = prs11.z = 0.0;

    fri00.x = fri00.y = fri00.z = 0.0;
    fri01.x = fri01.y = fri01.z = 0.0;
    fri10.x = fri10.y = fri10.z = 0.0;
    fri11.x = fri11.y = fri11.z = 0.0;

    frc = fset->frc;

    if(frc[k00].n > 0) {
        prs00.x = frc[k00].xprs[(int)(t*frc[k00].rate) % frc[k00].n];
        prs00.y = frc[k00].yprs[(int)(t*frc[k00].rate) % frc[k00].n];
        prs00.z = frc[k00].zprs[(int)(t*frc[k00].rate) % frc[k00].n];
        fri00.x = frc[k00].xfri[(int)(t*frc[k00].rate) % frc[k00].n];
        fri00.y = frc[k00].yfri[(int)(t*frc[k00].rate) % frc[k00].n];
        fri00.z = frc[k00].zfri[(int)(t*frc[k00].rate) % frc[k00].n];
    }
//printf("%f %f %f\n", prs00.x, prs00.y, prs00.z);
    if(frc[k01].n > 0) {
        prs01.x = frc[k01].xprs[(int)(t*frc[k01].rate) % frc[k01].n];
        prs01.y = frc[k01].yprs[(int)(t*frc[k01].rate) % frc[k01].n];
        prs01.z = frc[k01].zprs[(int)(t*frc[k01].rate) % frc[k01].n];
        fri01.x = frc[k01].xfri[(int)(t*frc[k01].rate) % frc[k01].n];
        fri01.y = frc[k01].yfri[(int)(t*frc[k01].rate) % frc[k01].n];
        fri01.z = frc[k01].zfri[(int)(t*frc[k01].rate) % frc[k01].n];
    }
    if(frc[k10].n > 0) {
        prs10.x = frc[k10].xprs[(int)(t*frc[k10].rate) % frc[k10].n];
        prs10.y = frc[k10].yprs[(int)(t*frc[k10].rate) % frc[k10].n];
        prs10.z = frc[k10].zprs[(int)(t*frc[k10].rate) % frc[k10].n];
        fri10.x = frc[k10].xfri[(int)(t*frc[k10].rate) % frc[k10].n];
        fri10.y = frc[k10].yfri[(int)(t*frc[k10].rate) % frc[k10].n];
        fri10.z = frc[k10].zfri[(int)(t*frc[k10].rate) % frc[k10].n];
    }
    if(frc[k11].n > 0) {
        prs11.x = frc[k11].xprs[(int)(t*frc[k11].rate) % frc[k11].n];
        prs11.y = frc[k11].yprs[(int)(t*frc[k11].rate) % frc[k11].n];
        prs11.z = frc[k11].zprs[(int)(t*frc[k11].rate) % frc[k11].n];
        fri11.x = frc[k11].xfri[(int)(t*frc[k11].rate) % frc[k11].n];
        fri11.y = frc[k11].yfri[(int)(t*frc[k11].rate) % frc[k11].n];
        fri11.z = frc[k11].zfri[(int)(t*frc[k11].rate) % frc[k11].n];
    }

    prs0.x = prs00.x*(1.0-ft)+prs10.x*ft;
    prs0.y = prs00.y*(1.0-ft)+prs10.y*ft;
    prs0.z = prs00.z*(1.0-ft)+prs10.z*ft;
    prs1.x = prs01.x*(1.0-ft)+prs11.x*ft;
    prs1.y = prs01.y*(1.0-ft)+prs11.y*ft;
    prs1.z = prs01.z*(1.0-ft)+prs11.z*ft;
    prs.x = prs0.x*(1.0-fp)+prs1.x*fp;
    prs.y = prs0.y*(1.0-fp)+prs1.y*fp;
    prs.z = prs0.z*(1.0-fp)+prs1.z*fp;

    pres->x = -prs.x*uu.x - prs.y*vv.x - prs.z*ww.x;
    pres->y = -prs.x*uu.y - prs.y*vv.y - prs.z*ww.y;
    pres->z = -prs.x*uu.z - prs.y*vv.z - prs.z*ww.z;

    fri0.x = fri00.x*(1.0-ft)+fri10.x*ft;
    fri0.y = fri00.y*(1.0-ft)+fri10.y*ft;
    fri0.z = fri00.z*(1.0-ft)+fri10.z*ft;
    fri1.x = fri01.x*(1.0-ft)+fri11.x*ft;
    fri1.y = fri01.y*(1.0-ft)+fri11.y*ft;
    fri1.z = fri01.z*(1.0-ft)+fri11.z*ft;
    fri.x = fri0.x*(1.0-fp)+fri1.x*fp;
    fri.y = fri0.y*(1.0-fp)+fri1.y*fp;
    fri.z = fri0.z*(1.0-fp)+fri1.z*fp;

    fric->x = fri.x*uu.x + fri.y*vv.x + fri.z*ww.x;
    fric->y = fri.x*uu.y + fri.y*vv.y + fri.z*ww.y;
    fric->z = fri.x*uu.z + fri.y*vv.z + fri.z*ww.z;
    
    return(prs);
}


void getForce2(Tpoint3f *force, Tpoint3f *torq) {

    Tpoint3f buo, fric, pres, r, wall;
    Tpoint3f tbuo, tfric, tpres, tmp;
    Trealf wz, s;
    int i, touch_walls;
    Trealf *mo;
   
    touch_walls = isTouchWall(&pobj, &wall);
wall.x = wall.y = wall.z = 0.;

    buo.x = buo.y = buo.z = 0.0;
    tbuo.x = tbuo.y = tbuo.z = 0.0;
//    if(touch_walls == FALSE) getBuoyancy(&pobj, &buo, &tbuo);
    getBuoyancy(&pobj, &buo, &tbuo);

    fric.x = fric.y = fric.z = 0.0;
    tfric.x = tfric.y = tfric.z = 0.0;

    Tpoint3f vo, vd, prs, fri, wve;
    Trealf c, c2, vs;
    ThapticSource *hp;
    Trealf the, phi, d, e;
    
    pobj.fpos.x = pobj.fpos.y = pobj.fpos.z = 0.0;
    pobj.n_fpos = 0;
    pres.x = pres.y = pres.z = 0.0;
    fric.x = fric.y = fric.z = 0.0;
    tpres.x = tpres.y = tpres.z = 0.0;
    tfric.x = tfric.y = tfric.z = 0.0;
    s = 1.0/(Trealf)pobj.nhsrc;
    hp = pobj.hsrc;
    mo = obj[pobj.id_obj].m;
    for(i=0; i<pobj.nhsrc; i++) {
        if(hp->a > 0.0) {

/*
            wve = lerpWaterVelocity(hp->x, hp->y);
            vd.x = -hp->vx+wve.x;
            vd.y = -hp->vy+wve.y;
            vd.z = -hp->vz+wve.z;
            vs = vd.x*vd.x + vd.y*vd.y + vd.z*vd.z;
            if(vs > 0.001 && vs < 1.) {
                vs = sqrt(vs);
//                printf("vs: %f, (%f %f %f)\n", vs, wve.x, wve.y, wve.z);
*/
            vd.x = -hp->vx;
            vd.y = -hp->vy;
            vd.z = -hp->vz;
            vs  = vd.x*vd.x+ vd.y*vd.y+ vd.z*vd.z;
            if(vs > 0.00001) {
                pobj.fpos.x += hp->x;
                pobj.fpos.y += hp->y;
                pobj.fpos.z += hp->z;
                pobj.n_fpos++;
                vs = sqrt(vs);
                vd.x /= vs;
                vd.y /= vs;
                vd.z /= vs;
 //               printf("vo, vs: %f %f %f, %f\n", hp->vx, hp->vy, hp->vz, hp->v);
                r.x = hp->x - pobj.pos.x;
                r.y = hp->y - pobj.pos.y;
                r.z = hp->z - pobj.pos.z;
//                座標変換して局所座標での速度ベクトルに変換
                tmp = vd;
                vd.x = tmp.x*mo[0]+tmp.y*mo[1]+tmp.z*mo[2];
                vd.y = tmp.x*mo[4]+tmp.y*mo[5]+tmp.z*mo[6];
                vd.z = tmp.x*mo[8]+tmp.y*mo[9]+tmp.z*mo[10];
                vo = vd;
                if(hp->sym.x == TRUE) vd.x = ABS(vd.x);
                if(hp->sym.y == TRUE) vd.y = ABS(vd.y);
                if(hp->sym.z == TRUE) vd.z = ABS(vd.z);
                the = 0.0;
                phi = 0.0;
                d = vd.x*vd.x+vd.y*vd.y;
                if(d > 0.0) {
                    d = sqrt(d);
                    if(vd.z >= 1.0) the = 0.0;
                    else            the = acos(vd.z);
                    e = vd.x/d;
                    if(e >= 1.0) phi = 0.0;
                    else          phi = acos(e);
                    if(vd.y < 0.0) phi = 2.0*M_PI - phi;
                    if(no_rotation == TRUE) {
                        the = M_PI/2.; 
                        vo.z = vd.z = 0.0;
                    }
                }
//                    the = M_PI/2.; 
//                    vo.z = vd.z = 0.0;
                hp->prs = lerpForce2(the, phi, hp->tcoord, &vd, hp->fset, &prs, &fri);
                if(vo.x*vd.x < 0.0) {prs.x = -prs.x; fri.x = -fri.x;}
                if(vo.y*vd.y < 0.0) {prs.y = -prs.y; fri.y = -fri.y;}
                if(vo.z*vd.z < 0.0) {prs.z = -prs.z; fri.z = -fri.z;}
                //逆変換してグローバル座標での力に変換
                tmp.x = prs.x*mo[0]+prs.y*mo[4]+prs.z*mo[8];
                tmp.y = prs.x*mo[1]+prs.y*mo[5]+prs.z*mo[9];
                tmp.z = prs.x*mo[2]+prs.y*mo[6]+prs.z*mo[10];
                prs = tmp;
                tmp.x = fri.x*mo[0]+fri.y*mo[4]+fri.z*mo[8];
                tmp.y = fri.x*mo[1]+fri.y*mo[5]+fri.z*mo[9];
                tmp.z = fri.x*mo[2]+fri.y*mo[6]+fri.z*mo[10];
                fri = tmp;
                c = vs/hp->fset->v0;
                c2 = c*c;

//printf("%f %f\n", vs, hp->fset->v0);
                hp->tcoord += c*pobj.vel_scl/(Trealf)force_frq;
                wz = pobj.wa*exp(-pobj.wz*(ABS(hp->z)))*hp->a;
                prs.x *= c2*hp->a;
                prs.y *= c2*hp->a;
                prs.z *= c2*hp->a;
                hp->prs.x *= c2*wz;
                hp->prs.y *= c2*wz;
                hp->prs.z *= c2*wz;
//                printf("%f %f\n", hp->prs.x, hp->prs.y);
                fri.x *= c*hp->a;
                fri.y *= c*hp->a;
                fri.z *= c*hp->a;
                pres.x += prs.x*s;
                pres.y += prs.y*s;
                pres.z += prs.z*s;
                fric.x += fri.x*s;
                fric.y += fri.y*s;
                fric.z += fri.z*s;
                tpres.x += (r.y*prs.z - r.z*prs.y)*s;
                tpres.y += (r.z*prs.x - r.x*prs.z)*s;
                tpres.z += (r.x*prs.y - r.y*prs.x)*s;
                tfric.x += (r.y*fri.z - r.z*fri.y)*s;
                tfric.y += (r.z*fri.x - r.x*fri.z)*s;
                tfric.z += (r.x*fri.y - r.y*fri.x)*s;
            }  else {
                hp->tcoord = 0.0;
                hp->prs.x = hp->prs.y = hp->prs.z = 0.0;
            }
        } else {
            hp->tcoord = 0.0;
            hp->prs.x = hp->prs.y = hp->prs.z = 0.0;
        }
//        printf("%f %f %f\n", hp->prs.x, hp->prs.y, hp->prs.z);
        hp++;
    }
    
    if(pobj.n_fpos > 0) {
        pobj.fpos.x /= pobj.n_fpos;
        pobj.fpos.y /= pobj.n_fpos;
        pobj.fpos.z /= pobj.n_fpos;
    }
    buo.z -= pobj.mass;
    
    pres.x *= pobj.pres_scl;
    pres.y *= pobj.pres_scl;
    pres.z *= pobj.pres_scl;

    fric.x *= pobj.fric_scl;
    fric.y *= pobj.fric_scl;
    fric.z *= pobj.fric_scl;

    tpres.x *= pobj.pres_scl;
    tpres.y *= pobj.pres_scl;
    tpres.z *= pobj.pres_scl;

    tfric.x *= pobj.fric_scl;
    tfric.y *= pobj.fric_scl;
    tfric.z *= pobj.fric_scl;

    force->x = buo.x + fric.x + pres.x + wall.x;
    force->y = buo.y + fric.y + pres.y + wall.y;
    force->z = buo.z + fric.z + pres.z + wall.z;
/*
    force->x -= safety_coef*pobj.vel.x;
    force->y -= safety_coef*pobj.vel.y;
    force->z -= safety_coef*pobj.vel.z;
*/
    torq->x = tbuo.x + tfric.x + tpres.x;
    torq->y = tbuo.y + tfric.y + tpres.y;
    torq->z = tbuo.z + tfric.z + tpres.z;


}


void getBuoyancy(ThapticObj *ho, Tpoint3f *buo, Tpoint3f *tbuo) {
    Tpoint3f b, n, r;
    Tpoint3f wve;
    Trealf wz, dz;
    TsamplePoint *sp;
    int i;
    
    for(i=0; i<ho->nhsrc; i++) ho->hsrc[i].a = 0.0;

    sp = ho->sp;
    for(i=0; i<ho->n_sp; i++) {
        wz = lerpWaterHeight(sp->p[0], sp->p[1]);
        r.x = sp->p[0] - ho->pos.x;
        r.y = sp->p[1] - ho->pos.y;
        r.z = sp->p[2] - ho->pos.z;
        n.x = sp->n[0];
        n.y = sp->n[1];
        n.z = sp->n[2];

    //   wz = 0.0;
        if(wz > sp->p[2]) {
            b.x = -n.x*(wz-sp->p[2])*sp->s;
            b.y = -n.y*(wz-sp->p[2])*sp->s;
            b.z = -n.z*(wz-sp->p[2])*sp->s;

            buo->x += b.x;
            buo->y += b.y;
            buo->z += b.z;
/*
            wve = lerpWaterVelocity(sp->p[0], sp->p[1]);
            if(sim_type == 1)
                dz = 30.*exp(-0.02*(wz-sp->p[2]))*dh;
            else if(sim_type == 2)
//                dz = 1.*exp(-0.0*(wz-sp->p[2]))*dh;
//                dz = 200.*exp(-0.0*(wz-sp->p[2]))*dh;
           dz = 0.0;

            buo->x -= wve.x*dz;
            buo->y -= wve.y*dz;
            buo->z -= wve.z*dz;
*/
            tbuo->x += r.y*b.z - r.z*b.y;
            tbuo->y += r.z*b.x - r.x*b.z;
            tbuo->z += r.x*b.y - r.y*b.x;

            if(ho->nhsrc > 0 && sp->i_hsrc >= 0) ho->hsrc[sp->i_hsrc].a = 1.0;
        }
        sp++;
    }
    
    buo->x *= ho->buo_scl;
    buo->y *= ho->buo_scl;
    buo->z *= ho->buo_scl;
    tbuo->x *= ho->buo_scl;
    tbuo->y *= ho->buo_scl;
    tbuo->z *= ho->buo_scl;

    for(i=0; i<ho->nhsrc; i++) {
        if(ho->hsrc[i].n_sp > 0) ho->hsrc[i].a /= (Trealf)ho->hsrc[i].n_sp;
    }
}


int isTouchWall(ThapticObj *ho, Tpoint3f *wall) {

    int i, touch_walls;
    TsamplePoint *sp;
   
    touch_walls = FALSE;
    wall->x = wall->y = wall->z = 0.0;
    i = 0;
    sp = ho->sp;
    while(i<ho->n_sp) {
        if(sp->p[2] <= -WATER_DEPTH) {
            wall->z = INFINITY_FORCE*2;
            touch_walls = TRUE;
        }
        if(sp->p[0] < -MX/2.*dh) {
            wall->x = INFINITY_FORCE*2;
            touch_walls = TRUE;
        }
        if(sp->p[0] > MX/2.*dh) {
            wall->x = -INFINITY_FORCE*2;
            touch_walls = TRUE;
        }
        if(sp->p[1] < -MY/2.*dh) {
            wall->y = INFINITY_FORCE*2;
            touch_walls = TRUE;
        }
        if(sp->p[1] > MY/2.*dh) {
            wall->y = -INFINITY_FORCE*2;
            touch_walls = TRUE;
        }
        i++;
        sp++;
    }
    
    return(touch_walls);
}

void displayHaptic() {
static    Tpoint3f frc_pre, trq_pre, frc_cur, trq_cur, wve_cur, wve_pre;
    Tpoint3f frc, trq;
    
    
    if(sim_mode == SIMULATION_ONCE && kforce == nforce_max-1) return;


    if(hap_step == 0) {
        frc_pre.x = frc_pre.y = frc_pre.z = 0.0;
        trq_pre.x = trq_pre.y = trq_pre.z = 0.0;
    }
//    if(use_spidar == TRUE) updateHapticObj();
    if(hap_step % force_calc_interval == 0){
        w_vel.x = w_vel.y = w_vel.z = 0.0;
        w_vel = lerpWaterVelocity(pobj.pos.x, pobj.pos.y);
        w_vel.y += yflow;
        w_vel.x *= flow_scl;
        w_vel.y *= flow_scl;
        w_vel.z *= flow_scl;
        if(rod.id_obj >= 0)
            updateRod(&rod);
        else if(use_spidar == TRUE)
            updateHapticObj();
        updateWaterSurface(TRUE);
        frc = frc_pre = frc_cur;
        trq = trq_pre = trq_cur;
        getForce2(&frc_cur, &trq_cur);
    } else {
        Trealf a;
        int i;
        a = (Trealf)(hap_step % force_calc_interval)/(Trealf)force_calc_interval;
        frc.x = (1.-a)*frc_pre.x+a*frc_cur.x;
        frc.y = (1.-a)*frc_pre.y+a*frc_cur.y;
        frc.z = (1.-a)*frc_pre.z+a*frc_cur.z;
        trq.x = (1.-a)*trq_pre.x+a*trq_cur.x;
        trq.y = (1.-a)*trq_pre.y+a*trq_cur.y;
        trq.z = (1.-a)*trq_pre.z+a*trq_cur.z;
        ThapticSource *hp;
        hp = pobj.hsrc;
        for(i=0; i<pobj.nhsrc; i++) {
            if(hp->v > 0.0) {
               hp->tcoord += hp->v/hp->fset->v0*pobj.vel_scl/(Trealf)force_frq;
            } else {
               hp->tcoord = 0.0;
            }
            hp++;
        }
    }
    if(hap_step < force_frq/2 && is_ready == FALSE) {hap_step++; kforce++; return;}
    else is_ready = TRUE;
    force_data[kforce] = pobj.force = frc;
    torq_data[kforce] = pobj.torq = trq;

#ifdef WRITE_DATA_MOVEMENT_DIRECTION
{
    Tpoint3f uu, vv, ww;
    Trealf a;
    vv.x = -vel_cur.x; vv.y = -vel_cur.y; vv.z = -vel_cur.z;
    a = sqrt(vv.x*vv.x + vv.y*vv.y + vv.z*vv.z);
    vv.x /= a; vv.y /= a; vv.z /= a;
    uu.x = vv.y; uu.y = -vv.x; uu.z = 0.0;
    a = uu.x*uu.x + uu.y*uu.y;
    if(a < 0.000000001) {
        if(vv.z > 0.) {
            uu.x = 1.0; uu.y = 0.0; uu.z = 0.0;
            ww.x = 0.0; ww.y = -1.0; ww.z = 0.0;
        } else {
            uu.x = 1.0; uu.y = 0.0; uu.z = 0.0;
            ww.x = 0.0; ww.y = 1.0; ww.z = 0.0;
        }
    } else {
        a = sqrt(a);
        uu.x /= a; uu.y /=a;
        ww.x = uu.y*vv.z;
        ww.y = -uu.x*vv.z;
        ww.z = uu.x*vv.y-uu.y*vv.x;
        a = ww.x*ww.x + ww.y*ww.y + ww.z*ww.z;
        a = sqrt(a);
        ww.x /= a; ww.y /= a; ww.z /= a;
    }
    force_data[kforce].x = frc.x*uu.x+frc.y*uu.y+frc.z*uu.z;
    force_data[kforce].y = frc.x*vv.x+frc.y*vv.y+frc.z*vv.z;
    force_data[kforce].z = frc.x*ww.x+frc.y*ww.y+frc.z*ww.z;
}
#endif    
    vel_data[kforce] = pobj.v;
    if(ve_max < vel_data[kforce]) ve_max = vel_data[kforce];
    Tpoint3f fff, ttt;
    if(use_spidar == TRUE) {
        if(rod.id_obj < 0) {
            fff = frc;;
            ttt = trq;
            if(boat.id_obj >= 0) {
                Trealf *m;
                m = obj[boat.id_obj].m;
                fff.x = frc.x*m[0]+frc.y*m[1]+frc.z*m[2];
                fff.y = frc.x*m[4]+frc.y*m[5]+frc.z*m[6];
                fff.z = frc.x*m[8]+frc.y*m[9]+frc.z*m[10];
                ttt.x = trq.x*m[0]+trq.y*m[1]+trq.z*m[2];
                ttt.y = trq.x*m[4]+trq.y*m[5]+trq.z*m[6];
                ttt.z = trq.x*m[8]+trq.y*m[9]+trq.z*m[10];
            }
        } else {
            fff.x =  rod.tension.x*cam.rot[0][0]+rod.tension.y*cam.rot[0][1]+rod.tension.z*cam.rot[0][2];
            fff.y = -rod.tension.x*cam.rot[2][0]-rod.tension.y*cam.rot[2][1]-rod.tension.z*cam.rot[2][2];
            fff.z =  rod.tension.x*cam.rot[1][0]+rod.tension.y*cam.rot[1][1]+rod.tension.z*cam.rot[1][2];
/*
            fff.x =  rod.tension.x*rod_rot[0][0]+rod.tension.y*rod_rot[0][1]+rod.tension.z*rod_rot[0][2];
            fff.y = -rod.tension.x*rod_rot[2][0]-rod.tension.y*rod_rot[2][1]-rod.tension.z*rod_rot[2][2];
            fff.z =  rod.tension.x*rod_rot[1][0]+rod.tension.y*rod_rot[1][1]+rod.tension.z*rod_rot[1][2];
*/
            ttt.x = ttt.y = ttt.z = 0.0;
        }
        if(imode != MOVIE_REPLAY && imode != MOVIE_WRITE) {
        if(is_ready == TRUE) {
            if(rod.id_obj < 0) {
                SPSetForce(fff.x*pobj.disp_scl, fff.z*pobj.disp_scl, -fff.y*pobj.disp_scl);
                SPSetTorque(ttt.x*pobj.disp_scl, ttt.z*pobj.disp_scl, -ttt.y*pobj.disp_scl);
            } else {
                SPSetForce(fff.x*pobj.disp_scl, fff.z*pobj.disp_scl, -fff.y*pobj.disp_scl);
            }

        }
        }
    }

    Trealf d;
    d = force_data[kforce].x*force_data[kforce].x
       +force_data[kforce].y*force_data[kforce].y
       +force_data[kforce].z*force_data[kforce].z;

d = fff.x*fff.x + fff.y*fff.y + fff.z*fff.z;
    if(d > 0.0) d = sqrt(d);
    force_abs[kforce] = d;

//    if(d > force_max && d < FORCE_MAX) force_max = d;
    if(d > force_max && kforce > 1.5*force_frq) force_max = d;

    d = torq_data[kforce].x*torq_data[kforce].x
       +torq_data[kforce].y*torq_data[kforce].y
       +torq_data[kforce].z*torq_data[kforce].z;
    if(d > 0.0) d = sqrt(d);
    torq_abs[kforce] = d;
    if(d > torq_max && d < TORQUE_MAX) torq_max = d;

    if(boat.id_obj >= 0 && d < TORQUE_MAX) {
        Tpoint3f r, torq, force, buo, tbuo;
        buo.x = buo.y = buo.z = 0.0;
        tbuo.x = tbuo.y = tbuo.z = 0.0;

        getBuoyancy(&boat, &buo, &tbuo);

        force.x = force_data[kforce].x;
        force.y = force_data[kforce].y;
        force.z = 0.0; //ボートへのオールによる上下方向の力は無視

        if(copy_oar == TRUE) {
            torq.x = torq.y = torq.z = 0.0;
            if(right_oar == TRUE) {
                r.x = pobj.pos.x - boat.pos.x;
                r.y = pobj.pos.y - boat.pos.y;
                r.z = pobj.pos.z - boat.pos.z;
                torq.x = r.y*force.z - r.z*force.y;
                torq.y = r.z*force.x - r.x*force.z;
                torq.z = r.x*force.y - r.y*force.x;
            }
            if(left_oar == TRUE) {
                Tpoint3f pos, f;
                pos.x = pobj.pos.x*cpy[0]+pobj.pos.y*cpy[4]+pobj.pos.z*cpy[8]+cpy[12];
                pos.y = pobj.pos.x*cpy[1]+pobj.pos.y*cpy[5]+pobj.pos.z*cpy[9]+cpy[13];
                pos.z = pobj.pos.x*cpy[2]+pobj.pos.y*cpy[6]+pobj.pos.z*cpy[10]+cpy[14];
                f.x = force.x*cpy[0]+force.y*cpy[4]+force.z*cpy[8];
                f.y = force.x*cpy[1]+force.y*cpy[5]+force.z*cpy[9];
                f.z = force.x*cpy[2]+force.y*cpy[6]+force.z*cpy[10];
                r.x = pos.x - boat.pos.x;
                r.y = pos.y - boat.pos.y;
                r.z = pos.z - boat.pos.z;
                torq.x += r.y*f.z - r.z*f.y;
                torq.y += r.z*f.x - r.x*f.z;
                torq.z += r.x*f.y - r.y*f.x;
            }
        } else {
            r.x = pobj.pos.x - boat.pos.x;
            r.y = pobj.pos.y - boat.pos.y;
            r.z = pobj.pos.z - boat.pos.z;
            torq.x = r.y*force.z - r.z*force.y;
            torq.y = r.z*force.x - r.x*force.z;
            torq.z = r.x*force.y - r.y*force.x;
        }

        force.x += buo.x;
        force.y += buo.y;
        force.z += buo.z-boat.mass;
        torq.x += tbuo.x;
        torq.y += tbuo.y;
        torq.z += tbuo.z;
      //  torq.x = 0.0; torq.y = 0.0; //なぜかうまくいかないのでゼロ。。。
/*
        force.x = buo.x;
        force.y = buo.y;
        force.z = buo.z-boat.mass;
        torq.x = tbuo.x;
        torq.y = tbuo.y;
        torq.z = tbuo.z;
*/
        moveBoatHase(force, torq);
    }

    if(rod.id_obj >= 0) {
        Trealf r, dx, dy, dz;
        Tpoint3f tmp, tq, rr;

        tmp = frc_cur;
        dx = pobj.pos.x - rod.pos.x;
        dy = pobj.pos.y - rod.pos.y;
        dz = pobj.pos.z - rod.pos.z;
        rod.tension.x = rod.tension.y = rod.tension.z = 0.;
        r = dx*dx+dy*dy+dz*dz;
        if(r > rod.rr0) {
            r = sqrt(r)-rod.r0;
            rod.tension.x = rod.s*r*dx;
            rod.tension.y = rod.s*r*dy;
            rod.tension.z = rod.s*r*dz;
            tmp.x -= rod.tension.x;
            tmp.y -= rod.tension.y;
            tmp.z -= rod.tension.z;
            rr.x = rod.lure_cur.x - pobj.pos.x;
            rr.y = rod.lure_cur.y - pobj.pos.y;
            rr.z = rod.lure_cur.z - pobj.pos.z;
            tq.x = rr.y*rod.tension.z - rr.z*rod.tension.y;
            tq.y = rr.z*rod.tension.x - rr.x*rod.tension.z;
            tq.z = rr.x*rod.tension.y - rr.y*rod.tension.x;
            trq_cur.x -= tq.x;
            trq_cur.y -= tq.y;
            trq_cur.z -= tq.z;
        }
        moveLure(&pobj, tmp, trq_cur);
    }
    int i;
    for(i=0; i<16; i++)
        pobj_mat_hist[kforce % n_pobj_mat_hist][i] = obj[pobj.id_obj].m[i];

    kforce++;
    hap_step++;
    if(kforce >= nforce_max) kforce = 0;
}


void moveHapticObj(ThapticObj *hp, Tpoint3f force, Tpoint3f torq) {
    Trealf dt;
    dt = (Trealf)force_calc_interval/(Trealf)force_frq;

    // 行列・ベクトル・クォータニオンライブラリを使うため，キャストして参照型に．
    Vec3f& pos = (Vec3f&)hp->pos;
    Quaternionf& ori = (Quaternionf&)hp->ori;
    Vec3f& vel = (Vec3f&)hp->vel;
    Vec3f& we = (Vec3f&)hp->we;
    Vec3f& a = (Vec3f&)hp->a;
    Vec3f& amom = (Vec3f&)hp->amom;
    Vec3f& tq = (Vec3f&)torq;
    Matrix3f rot;
    ori.to_matrix(rot);

// 船の傾きを元に戻すトルクを加える
//    Vec3f ang = Vec3f(0,0,1) ^ (Vec3f(0,0,1) - rot.Ez());
  //  tq += ang * 5;

    // 加速度を求める
    a = (Vec3f&)force / hp->mass;
    // 速度を求める
    vel += a*dt;

    // 角運動量を求める．
    amom += tq * dt;
    // 慣性テンソルを求める．
    Matrix3f& inertia = (Matrix3f&)hp->I; // 列が密に並んだ行列だと思っています．
    Matrix3f inertia_w = rot * inertia * rot.trans();
// 角速度を求める．
    we = inertia_w.inv() * amom;
    
// 位置と角度を求める
    pos += vel*dt + 0.5f*a*dt*dt;
    Vec3f d_ori =  we*dt;
    ori = Quaternionf::Rot(d_ori)*ori;
    ori.to_matrix(rot);

    vel*= hp->loss;
    amom *= hp->loss;

    Trealf *m=obj[hp->id_obj].m;

    m[0] = rot[0][0]; m[1] = rot[1][0]; m[2]  = rot[2][0]; m[3] = 0.0;
    m[4] = rot[0][1]; m[5] = rot[1][1]; m[6]  = rot[2][1]; m[7] = 0.0;
    m[8] = rot[0][2]; m[9] = rot[1][2]; m[10] = rot[2][2]; m[11] = 0.0;
    m[12] = pos.x;
    m[13] = pos.y;
    m[14] = pos.z;
    m[15] = 1.0;
/*
   TsamplePoint *sp;
   int i;
   sp = hp->sp;
   for(i=0; i<hp->n_sp; i++) {
        sp[i].p[0] = m[0]*sp[i].p0[0]+m[4]*sp[i].p0[1]+m[8]*sp[i].p0[2]+m[12];
        sp[i].p[1] = m[1]*sp[i].p0[0]+m[5]*sp[i].p0[1]+m[9]*sp[i].p0[2]+m[13];
        sp[i].p[2] = m[2]*sp[i].p0[0]+m[6]*sp[i].p0[1]+m[10]*sp[i].p0[2]+m[14];
        
        sp[i].n[0] = m[0]*sp[i].n0[0]+m[4]*sp[i].n0[1]+m[8]*sp[i].n0[2];
        sp[i].n[1] = m[1]*sp[i].n0[0]+m[5]*sp[i].n0[1]+m[9]*sp[i].n0[2];
        sp[i].n[2] = m[2]*sp[i].n0[0]+m[6]*sp[i].n0[1]+m[10]*sp[i].n0[2];
   }
*/
    hp->v = hp->vel.x*hp->vel.x+hp->vel.y*hp->vel.y+hp->vel.z*hp->vel.z;
    if(hp->v > 0.0) hp->v = sqrt(hp->v);
}

void moveBoatHase(Tpoint3f force, Tpoint3f torq) {
	Trealf dt;
    dt = 1.0/force_frq;

    // 行列・ベクトル・クォータニオンライブラリを使うため，キャストして参照型に．
    Vec3f& pos = (Vec3f&)boat.pos;
    Quaternionf& ori = (Quaternionf&)boat.ori;
    Vec3f& vel = (Vec3f&)boat.vel;
    Vec3f& we = (Vec3f&)boat.we;
    Vec3f& a = (Vec3f&)boat.a;
    Vec3f& amom = (Vec3f&)boat.amom;
    Vec3f& tq = (Vec3f&)torq;
    Matrix3f rot;
    ori.to_matrix(rot);

	// 船の傾きを元に戻すトルクを加える
    Vec3f ang;
	if (screen == CENTER_SCREEN){	//	床はボートを固定
		ang = Vec3f(0,0,1) ^ (Vec3f(0,0,1) - rot.Ez());
	}else{
		ang = -ori.rotation();
	}
	tq += ang * 5;

    // 加速度を求める
    a = (Vec3f&)force / boat.mass;
    // 速度を求める
    vel += a*dt;

    // 角運動量を求める．
    amom += tq * dt;
    // 慣性テンソルを求める．
    Matrix3f& inertia = (Matrix3f&)boat.I; // 列が密に並んだ行列だと思っています．
    Matrix3f inertia_w = rot * inertia * rot.trans();
// 角速度を求める．
    we = inertia_w.inv() * amom;
    
	// 位置と角度を求める
	if (screen == CENTER_SCREEN){	//	床は位置を止めてみた hase
		pos += vel*dt + 0.5f*a*dt*dt;
	}else{
		pos = Vec3f();		
	}


    Vec3f d_ori =  we*dt;
    ori = Quaternionf::Rot(d_ori)*ori;
    ori.to_matrix(rot);

    vel*= boat.loss;
    amom *= boat.loss;

    Trealf *m=obj[boat.id_obj].m;

    m[0] = rot[0][0]; m[1] = rot[1][0]; m[2]  = rot[2][0]; m[3] = 0.0;
    m[4] = rot[0][1]; m[5] = rot[1][1]; m[6]  = rot[2][1]; m[7] = 0.0;
    m[8] = rot[0][2]; m[9] = rot[1][2]; m[10] = rot[2][2]; m[11] = 0.0;
    m[12] = pos.x;
    m[13] = pos.y;
    m[14] = pos.z;
    m[15] = 1.0;

   TsamplePoint *sp;
   int i;
   sp = boat.sp;
   for(i=0; i<boat.n_sp; i++) {
        sp[i].p[0] = m[0]*sp[i].p0[0]+m[4]*sp[i].p0[1]+m[8]*sp[i].p0[2]+m[12];
        sp[i].p[1] = m[1]*sp[i].p0[0]+m[5]*sp[i].p0[1]+m[9]*sp[i].p0[2]+m[13];
        sp[i].p[2] = m[2]*sp[i].p0[0]+m[6]*sp[i].p0[1]+m[10]*sp[i].p0[2]+m[14];
        
        sp[i].n[0] = m[0]*sp[i].n0[0]+m[4]*sp[i].n0[1]+m[8]*sp[i].n0[2];
        sp[i].n[1] = m[1]*sp[i].n0[0]+m[5]*sp[i].n0[1]+m[9]*sp[i].n0[2];
        sp[i].n[2] = m[2]*sp[i].n0[0]+m[6]*sp[i].n0[1]+m[10]*sp[i].n0[2];
   }
    boat.v = boat.vel.x*boat.vel.x+boat.vel.y*boat.vel.y+boat.vel.z*boat.vel.z;
    if(boat.v > 0.0) boat.v = sqrt(boat.v);

	if (screen != CENTER_SCREEN){	//	床は水を動かす hase
		yflow = -boat.vel.z;
		if (yflow > 0.3) yflow = 0.3;
		if (yflow < -0.3) yflow = -0.3;
	}
}

/*
#define MIN_PITCH 0.25
#define MAX_PITCH 3.
#define MAX_BUBBLE_MOTION 25.0
#define BUBBLE_LEVEL 8
*/
#define MIN_PITCH 0.25
#define MAX_PITCH 1.5
#define MAX_BUBBLE_MOTION 25.0
#define BUBBLE_LEVEL 8
Trealf bubbleR[16];
Trealf pitch[16];
Trealf bubbleW[16];

void timer_sound(int val) {

    int j, k;
    int i, ii, i1, i2;
    Treald c, vol, a, da, vv;
    Trealf s;

    pitch[0] = MIN_PITCH;
    s = bubbleR[0] = 1.0/pitch[0];
    bubbleW[0] = MAX_BUBBLE_MOTION;
    for(k=1; k<BUBBLE_LEVEL; k++) {
        pitch[k] = pitch[k-1]+(MAX_PITCH-MIN_PITCH)/BUBBLE_LEVEL;
        bubbleR[k] = 1.0/pitch[k];
        bubbleW[k] = bubbleW[k-1]-MAX_BUBBLE_MOTION/(BUBBLE_LEVEL-1);
        s += bubbleR[k];
    }

    for(k=0; k<sound.ndata; k++) sound.wave[k] = 0.0;

    da = 1.0/sound.nsmp;
    vv = 1.0;
/*
    for(ii=0; ii<BUBBLE_LEVEL; ii++) {
    if(wmotion[isim_step] > MAX_BUBBLE_MOTION*(1.0 - (ii+1)/BUBBLE_LEVEL)) {
    dpos = (MAX_PITCH-MIN_PITCH)*ii/(BUBBLE_LEVEL-1) + MIN_PITCH;
    vv = 1.0 - ii/(BUBBLE_LEVEL-1);
*/
    for(ii=0; ii<BUBBLE_LEVEL; ii++) {
    if(wmotion[isim_step] > bubbleW[ii]) {
    dpos = pitch[ii];
    vv = bubbleR[ii]/s;

    a = 0.0;
    k = 0;
    while(k < sound.ndata) {
        vol = wmotion[isim_step-1]*(1.0-a) + wmotion[isim_step]*a;
        vol *= volume;
//        vol -= 0.2;
        vol -= 0.1;
        if(vol < 0.0) vol = 0.0;
        vol *= vv;
        for(j=0; j<nstex; j++) {
            i = (int)(spos[j]);
            c = spos[j]-i;
            if(sound.stereo == TRUE) {
                if(stex[j].stereo == TRUE) {
                    i1 = i*2; i2 = i1+2;
                    sound.wave[k] += vol*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                    i1++; i2++;
                    sound.wave[k+1] += vol*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                } else {
                    i1 = i; i2 = i1+1;
                    sound.wave[k] += vol*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                    sound.wave[k+1] += vol*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                }
            } else {
                if(stex[j].stereo == TRUE) {
                    i1 = i*2; i2 = i1+2;
                    sound.wave[k] += vol*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                } else {
                    i1 = i; i2 = i1+1;
                    sound.wave[k] += vol*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                }
            }
            spos[j] += dpos; if(spos[j] >= stex[j].nsmp-1) spos[j] -= stex[j].nsmp;
        }

        a += da;
        k++;
        if(sound.stereo == TRUE) k++;
    }
    }
    }

    Trealf w;
    int iw;

    for(k=0; k<sound.ndata; k++) {
        w = sound.wave[k];
        wav[iwav] = w;
        iwav++;
        if(iwav >= nwav) iwav = 0;
        if(sound.bit == 8) {
            iw = (int)(w*128+128);
            if(iw < 0) iw = 0;
            if(iw > 255) iw = 255;
            pcm8[k] = (char)iw;
        } else if(sound.bit == 16) {
            iw = (int)(w*32767);
            if(iw < -32767) iw = -32767;
            if(iw > 32767) iw = 32767;
            pcm16[k] = (short)iw;
        }
    }
    if(sound.bit == 16) {
//        spWriteAudio(audio, pcm16, sound.ndata);
    } else if(sound.bit == 8) {
//        spWriteAudio(audio, pcm8, sound.ndata);
    }
}

void timer_drop_sound(int val) {

    int j, k, flag;
    int i, i1, i2;
    Treald c;

    for(k=0; k<sound.ndata; k++) sound.wave[k] = 0.0;

    flag = FALSE;
    k = 0;
    while(k < sound.ndata) {
        for(j=0; j<nstex; j++) {
            if(spos[j] > -1 && spos[j] < stex[j].nsmp) {
                flag = TRUE;
                i = (int)(spos[j]);
                c = spos[j]-i;
                if(sound.stereo == TRUE) {
                    if(stex[j].stereo == TRUE) {
                        i1 = i*2; i2 = i1+2;
                        sound.wave[k] += volume*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                        i1++; i2++;
                        sound.wave[k+1] += volume*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                    } else {
                        i1 = i; i2 = i1+1;
                        sound.wave[k] += volume*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                        sound.wave[k+1] += volume*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                    }
                } else {
                    if(stex[j].stereo == TRUE) {
                        i1 = i*2; i2 = i1+2;
                        sound.wave[k] += volume*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                    } else {
                        i1 = i; i2 = i1+1;
                        sound.wave[k] += volume*((1.0-c)*stex[j].wave[i1]+c*stex[j].wave[i2]);
                    }
                }
                spos[j] += dpos;
/*
                sound.wave[k] += stex[j].wave[spos[j]];
                if(sound.stereo == TRUE) {
                    if(stex[j].stereo == FALSE) {
                        sound.wave[k+1] += stex[j].wave[spos[j]];
                    } else if(stex[j].stereo == TRUE) {
                        spos[j]+=step_sound;
                        sound.wave[k+1] += stex[j].wave[spos[j]];
                    }
                } else if(sound.stereo == FALSE) {
                    if(stex[j].stereo == TRUE) spos[j]+=step_sound;
                }
                spos[j]+=step_sound;
*/
            }
        }
        k++;
        if(sound.stereo == TRUE) k++;
    }

    if(flag == TRUE) {

        Trealf w;
        int iw;

        for(k=0; k<sound.ndata; k++) {
            w = sound.wave[k];
            if(sound.bit == 8) {
                iw = (int)(w*128+128);
                if(iw < 0) iw = 0;
                if(iw > 255) iw = 255;
                pcm8[k] = (char)iw;
            } else if(sound.bit == 16) {
                iw = (int)(w*32767);
                if(iw < -32767) iw = -32767;
                if(iw > 32767) iw = 32767;
                pcm16[k] = (short)iw;
            }
        }
        if(sound.bit == 16) {
//            spWriteAudio(audio, pcm16, sound.ndata);
        } else if(sound.bit == 8) {
//            spWriteAudio(audio, pcm8, sound.ndata);
        }
    } else {
        kick_sound = FALSE;
    }
/*
    if(flag == TRUE) {
        glutTimerFunc(TIMER_INTERVAL, timer_drop_sound, TRUE);
    } else {
        glutTimerFunc(TIMER_INTERVAL, updateWaterSurface, TRUE);
    }
*/
}

void timer_push_sound(int val) {

    int j, k, flag;
    int i, i1, i2;
    Treald c;

    for(k=0; k<sound.ndata; k++) sound.wave[k] = 0.0;

    flag = FALSE;
    k = 0;
    while(k < sound.ndata) {
        for(j=0; j<nss; j++) {
            if(sspos[j] > -1 && sspos[j] < stex[itex[j]].nsmp) {
                flag = TRUE;
                i = (int)(sspos[j]);
                c = sspos[j]-i;
                volume = ssvol[j];
                if(sound.stereo == TRUE) {
                    if(stex[itex[j]].stereo == TRUE) {
                        i1 = i*2; i2 = i1+2;
                        sound.wave[k] += volume*((1.0-c)*stex[itex[j]].wave[i1]+c*stex[itex[j]].wave[i2]);
                        i1++; i2++;
                        sound.wave[k+1] += volume*((1.0-c)*stex[itex[j]].wave[i1]+c*stex[itex[j]].wave[i2]);
                    } else {
                        i1 = i; i2 = i1+1;
                        sound.wave[k] += volume*((1.0-c)*stex[itex[j]].wave[i1]+c*stex[itex[j]].wave[i2]);
                        sound.wave[k+1] += volume*((1.0-c)*stex[itex[j]].wave[i1]+c*stex[itex[j]].wave[i2]);
                    }
                } else {
                    if(stex[itex[j]].stereo == TRUE) {
                        i1 = i*2; i2 = i1+2;
                        sound.wave[k] += volume*((1.0-c)*stex[itex[j]].wave[i1]+c*stex[itex[j]].wave[i2]);
                    } else {
                        i1 = i; i2 = i1+1;
                        sound.wave[k] += volume*((1.0-c)*stex[itex[j]].wave[i1]+c*stex[itex[j]].wave[i2]);
                    }
                }
                sspos[j] += svpos[j];
            } else if(sspos[j] >= stex[itex[j]].nsmp) {
                sspos[j] = -1;
            }
        }
        k++;
        if(sound.stereo == TRUE) k++;
    }

    if(flag == TRUE) {

        Trealf w;
        int iw;

        for(k=0; k<sound.ndata; k++) {
            w = sound.wave[k];
            if(sound.bit == 8) {
                iw = (int)(w*128+128);
                if(iw < 0) iw = 0;
                if(iw > 255) iw = 255;
                pcm8[k] = (char)iw;
            } else if(sound.bit == 16) {
                iw = (int)(w*32767);
                if(iw < -32767) iw = -32767;
                if(iw > 32767) iw = 32767;
                pcm16[k] = (short)iw;
            }
        }
        if(sound.bit == 16) {
//            spWriteAudio(audio, pcm16, sound.ndata);
        } else if(sound.bit == 8) {
//            spWriteAudio(audio, pcm8, sound.ndata);
        }
    } else {
        kick_sound = FALSE;
    }
/*
    if(flag == TRUE) {
        glutTimerFunc(TIMER_INTERVAL, timer_drop_sound, TRUE);
    } else {
        glutTimerFunc(TIMER_INTERVAL, updateWaterSurface, TRUE);
    }
*/
}

void experiment() {

    int i;
    Trealf t;
    
    initSimulation();
//    pobj.pos.y = 20.;
    
    force_calc_interval = 1;
    force_frq = 50;
    t = 0.;
    for(i=0; i<1500; i++) {
    if(i % 100 == 0) printf("%d / %d\n", i, 500);
        displayHaptic();
        if(isim_step/30.00 < t) {
            timerDisplay(TRUE);
        }
        t += 1.0/force_frq;
    }
}

void writeMovie() {

    int i;
    Trealf t;
    
    initSimulation();
    
    t = 0.;
    for(i=0; i<nforce_max; i++) {
        displayHaptic();
        if(isim_step/30.00 < t) {
            if(isim_step % 100 == 0) printf("%d %d %d\n", i, kforce, isim_step);
//            if(isim_step == 190) disp_hist = TRUE;
            if(isim_step <= 480) timerDisplay(TRUE);
        }
        t += 1.0/force_frq;
    }
}

void initSimulation(void) {
        isim_step = 0;
        img_no = 0;
        isim = TRUE;
        iwav = 0;
        initWater();

        if(use_spidar == TRUE) {
            float *p;
            // timer API
//            timerID = timeSetEvent(2, 2, TimeProc, 0, TIME_PERIODIC);
            p = SPGetPosition();
            pos_hap.x = p[0];
            pos_hap.y = p[1];
            pos_hap.z = p[2];
            prev_x = p[0];
            prev_y = p[1];
            kforce = 0;
            hap_step = 0;
        }
        pobj.prev_pos.x = pobj.prev_pos.y = pobj.prev_pos.z =
        pobj.pos.x = pobj.pos.y = pobj.pos.z = 0.0;
        prev_x = prev_y = 0.;
        is_ready = FALSE;
        int i;
        Trealf *m;
        i = pobj.id_obj;
        m = obj[i].m;
        m[0] = 1.0; m[4] = 0.0; m[8] =  0.0; m[12] = 0.0;
        m[1] = 0.0; m[5] = 1.0; m[9] =  0.0; m[13] = 0.0;
        m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = 0.0;
        m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
        updateHapticSource(&pobj);
        pobj.a.x = pobj.a.y = pobj.a.z = 
        pobj.aa.x = pobj.aa.y = pobj.aa.z = 
        pobj.vel.x = pobj.vel.y = pobj.vel.z = 
        pobj.we.x = pobj.we.y = pobj.we.z = 
        pobj.amom.x = pobj.amom.y = pobj.amom.z = 0.0;
        pobj.ori[0] = 1.0; pobj.ori[1] = 0.0; pobj.ori[2] = 0.0; pobj.ori[3] = 0.0;
        pobj.prev_pos.x = pobj.prev_pos.y = pobj.prev_pos.z =
        pobj.pos.x = pobj.pos.y = pobj.pos.z = 0.0;
        if(boat.id_obj >= 0) {
            boat.a.x = boat.a.y = boat.a.z = 
            boat.aa.x = boat.aa.y = boat.aa.z = 
            boat.vel.x = boat.vel.y = boat.vel.z = 
            boat.we.x = boat.we.y = boat.we.z = 
            boat.amom.x = boat.amom.y = boat.amom.z = 0.0;
            boat.ori[0] = 1.0;
            boat.ori[1] = 0.0;
            boat.ori[2] = 0.0;
            boat.ori[3] = 0.0;
            boat.prev_pos.x = boat.prev_pos.y = boat.prev_pos.z =
            boat.pos.x = boat.pos.y = boat.pos.z = 0.0;
            i = boat.id_obj;
            m = obj[i].m;
            m[0] = 1.0; m[4] = 0.0; m[8] =  0.0; m[12] = 0.0;
            m[1] = 0.0; m[5] = 1.0; m[9] =  0.0; m[13] = 0.0;
            m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = 0.0;
            m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
//            updateHapticSource();
        }
}

void initViewpoint(void) {
/*
    cam.vpol[0]=MX*1.5;
    cam.vpol[1]=0.0;
    cam.vpol[2]=0.0;

    cam.view[0] = cam.vpol[0];
    cam.view[1] = 0.;
    cam.view[2] = 0.;

    cam.refe[0] = 0.;
    cam.refe[1] = 0.;
    cam.refe[2] = 0.;

    cam.view[0] = cam.vpol[0]*cos(cam.vpol[2])*cos(cam.vpol[1])+cam.refe[0];
    cam.view[1] = cam.vpol[0]*cos(cam.vpol[2])*sin(cam.vpol[1])+cam.refe[1];
    cam.view[2] = cam.vpol[0]*sin(cam.vpol[2])+cam.refe[2];

    cam.vr[0] = cam.refe[0] - cam.view[0];
    cam.vr[1] = cam.refe[1] - cam.view[1];
    cam.vr[2] = cam.refe[2] - cam.view[2];
    cam.vr[0] /= cam.vpol[0];
    cam.vr[1] /= cam.vpol[0];
    cam.vr[2] /= cam.vpol[0];
    
    cam.view[0] = 0.385148;
    cam.view[1] = -139.605533;
    cam.view[2] = 171.321764;
    cam.refe[0] = 0.0;
    cam.refe[1] = 0.0;
    cam.refe[2] = 0.0;
    cam.vr[0] = -0.001743;
    cam.vr[1] = 0.631699;
    cam.vr[2] = -0.775212;
    cam.vpol[0] = 221.0;
    cam.vpol[1] = -1.568037;
    cam.vpol[2] = 0.887050;
*/
    cam.view[0] = cam.init_view.vx;
    cam.view[1] = cam.init_view.vy;
    cam.view[2] = cam.init_view.vz;
    cam.refe[0] = cam.init_view.rx;
    cam.refe[1] = cam.init_view.ry;
    cam.refe[2] = cam.init_view.rz;
    cam.vpol[0] = cam.init_view.vr;
    cam.vpol[1] = cam.init_view.the;
    cam.vpol[2] = cam.init_view.phi;
	cam.fovy = cam.init_view.fovy;
    cam.vr[0] = cam.refe[0] - cam.view[0];
    cam.vr[1] = cam.refe[1] - cam.view[1];
    cam.vr[2] = cam.refe[2] - cam.view[2];
    cam.vr[0] /= cam.vpol[0];
    cam.vr[1] /= cam.vpol[0];
    cam.vr[2] /= cam.vpol[0];

    setCamera();

    makeMatrix(cam.view[0], cam.view[1], cam.view[2]
             , cam.refe[0], cam.refe[1], cam.refe[2]
             , 0.0, cam.rot);
/*
    rod_view[0] = cam.view[0];
    rod_view[1] = cam.view[1];
    rod_view[2] = cam.view[2];
    makeMatrix(cam.view[0], cam.view[1], cam.view[2]
             , cam.refe[0], cam.refe[1], cam.refe[2]
             , 0.0, rod_rot);
*/
    updateEar();
}

void updateEar() {

    ear.lp.x = cam.view[0] - cam.rot[0][0]*ear.dis;
    ear.lp.y = cam.view[1] - cam.rot[0][1]*ear.dis;
    ear.lp.z = cam.view[2] - cam.rot[0][2]*ear.dis;
    ear.rp.x = cam.view[0] + cam.rot[0][0]*ear.dis;
    ear.rp.y = cam.view[1] + cam.rot[0][1]*ear.dis;
    ear.rp.z = cam.view[2] + cam.rot[0][2]*ear.dis;

    ear.dir_l.x = -cam.rot[0][0];//*0.70710678 + cam.rot[2][0]*0.70710678;
    ear.dir_l.y = -cam.rot[0][1];//*0.70710678 + cam.rot[2][1]*0.70710678;
    ear.dir_l.z = -cam.rot[0][2];//*0.70710678 + cam.rot[2][2]*0.70710678;

    ear.dir_r.x = cam.rot[0][0];//*0.70710678 + cam.rot[2][0]*0.70710678;
    ear.dir_r.y = cam.rot[0][1];//*0.70710678 + cam.rot[2][1]*0.70710678;
    ear.dir_r.z = cam.rot[0][2];//*0.70710678 + cam.rot[2][2]*0.70710678;

}


void setCamera(void) {
    if(rod.lure_view_display > 0.0) {
        if(lure_view == TRUE)
            glViewport(0, cam.winY*(1.0-rod.lure_view_display)
                   , cam.winX*rod.lure_view_display, cam.winY*rod.lure_view_display);
        else
            glViewport(0, 0, cam.winX, cam.winY);
    } else {
        if (screen == CENTER_SCREEN){
			int LEFT=100;
			int RIGHT=0;
			glViewport(LEFT, 0, cam.winX-LEFT-RIGHT, cam.winY);
			glScissor(LEFT, 0, cam.winX-LEFT-RIGHT, cam.winY);
		}else{
			glViewport(0, 0, cam.winX, cam.winY);
		}
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	float w = 0.5;
	float h = 0.5 / cam.aspect;
	if (screen==LEFT_SCREEN){
		glFrustum(-w, w, -2*h, 0, 0.5, Z_FAR);
	}else if (screen==RIGHT_SCREEN){
		glFrustum(-w, w, -2*h, 0, 0.5, Z_FAR);
	}else{
		float w = 0.5 * cam.aspect;
		float h = 0.5;
		glFrustum(-w, w, -h, h, 0.5, Z_FAR);
	}

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	if (screen==LEFT_SCREEN){
		glRotatef(90, 1,0,0);
		glRotatef(-90, 0,1,0);
		glTranslatef(0.2, 0, 0);
	}else if (screen==RIGHT_SCREEN){
		glRotatef(90, 1,0,0);		
		glRotatef(90, 0,1,0);
		glTranslatef(-0.2, 0, 0);
//	水を見るためのカメラ位置
//		glTranslatef(0, -10, 0);
//		glRotatef(-60, 0,0,1);
	
	}
    

    if(boat_view == TRUE) {
        Trealf view[3], refe[3], *m;
        m = obj[boat.id_obj].m;
        
        view[0] = cam.view[0]*m[0]+cam.view[1]*m[4]+cam.view[2]*m[8]+m[12];
        view[1] = cam.view[0]*m[1]+cam.view[1]*m[5]+cam.view[2]*m[9]+m[13];
        view[2] = cam.view[0]*m[2]+cam.view[1]*m[6]+cam.view[2]*m[10]+m[14];
        refe[0] = cam.refe[0]*m[0]+cam.refe[1]*m[4]+cam.refe[2]*m[8]+m[12];
        refe[1] = cam.refe[0]*m[1]+cam.refe[1]*m[5]+cam.refe[2]*m[9]+m[13];
        refe[2] = cam.refe[0]*m[2]+cam.refe[1]*m[6]+cam.refe[2]*m[10]+m[14];
        gluLookAt((float)view[0], (float)view[1], (float)view[2]
                 ,(float)refe[0], (float)refe[1], (float)refe[2]
                 ,m[8], m[9], m[10]);
    } else if(lure_view == TRUE) {
        Trealf  *m, d = 5.;
        m = obj[pobj.id_obj].m;
        
        gluLookAt((float)(m[12]+d), (float)(m[13]+d), (float)(m[14]+d/5)
                 ,(float)m[12], (float)m[13], (float)m[14]
                 ,0, 0, 1);
    } else {
        gluLookAt((float)cam.view[0], (float)cam.view[1], (float)cam.view[2]
                 ,(float)cam.refe[0], (float)cam.refe[1], (float)cam.refe[2]
                 ,0      , 0      , 1);
    }
}

Tpoint3f invtrans(Treald sx, Treald sy, Tcamera *cam) {
    Tpoint3f res;
    Treald xs, ys, ww, hh;

    xs = sx;
    ys = sy;
    xs -= cam->winX/2; xs /= (cam->winX*0.5);
    ys -= cam->winY/2; ys /= (cam->winY*0.5);
    hh  = cam->vpol[0]*tan(cam->fovy/360.0*M_PI);
    ww  = hh*cam->aspect;
    xs *= ww;
    ys *= hh;
    res.x = cam->view[0]+cam->vpol[0]*cam->vr[0]+cam->rot[0][0]*xs+cam->rot[1][0]*ys;
    res.y = cam->view[1]+cam->vpol[0]*cam->vr[1]+cam->rot[0][1]*xs+cam->rot[1][1]*ys;
    res.z = cam->view[2]+cam->vpol[0]*cam->vr[2]+cam->rot[0][2]*xs+cam->rot[1][2]*ys;
    return(res);
}

void mouse_operation(int x, int y) {

    float dx, dy;

    if(idrag == TRUE) {
        if(imode == MOVE_CAMERA) {
          if(iview == MOVE_ROTATE) {
            dx = prev_x - x;
            dy = prev_y - y;
            cam.vpol[1] += 0.5*dx/(Treald)cam.winX*3.14;
            cam.vpol[2] += 0.5*dy/(Treald)cam.winY*3.14;
            cam.view[0] = cam.vpol[0]*cos(cam.vpol[2])*cos(cam.vpol[1])+cam.refe[0];
            cam.view[1] = cam.vpol[0]*cos(cam.vpol[2])*sin(cam.vpol[1])+cam.refe[1];
            cam.view[2] = cam.vpol[0]*sin(cam.vpol[2])+cam.refe[2];
          } else if(iview == MOVE_WALK) {
            dy = prev_y - y;
            dy *= 0.05;
            cam.view[0] += dy*cam.rot[2][0];
            cam.view[1] += dy*cam.rot[2][1];
            cam.view[2] += dy*cam.rot[2][2];
            cam.refe[0] += dy*cam.rot[2][0];
            cam.refe[1] += dy*cam.rot[2][1];
            cam.refe[2] += dy*cam.rot[2][2];
          } else if(iview == MOVE_APPROACH) {
            dy = prev_y - y;
            dy *= 1.;
            cam.vpol[0] += dy;
          } else if(iview == MOVE_PAN) {
            dx = prev_x - x;
            dy = prev_y - y;
            dx *= 0.1; dy *= 0.1;
            if(ABS(dx) > ABS(dy)) {
                cam.view[0] += cam.rot[0][0]*dx;
                cam.view[1] += cam.rot[0][1]*dx;
                cam.view[2] += cam.rot[0][2]*dx;
                cam.refe[0] += cam.rot[0][0]*dx;
                cam.refe[1] += cam.rot[0][1]*dx;
                cam.refe[2] += cam.rot[0][2]*dx;
            } else {
                cam.view[0] += cam.rot[1][0]*dy;
                cam.view[1] += cam.rot[1][1]*dy;
                cam.view[2] += cam.rot[1][2]*dy;
                cam.refe[0] += cam.rot[1][0]*dy;
                cam.refe[1] += cam.rot[1][1]*dy;
                cam.refe[2] += cam.rot[1][2]*dy;
            }
          } else if(iview == MOVE_TURN ) {
            dx = prev_x - x;
            dy = prev_y - y;
            cam.vpol[1] -= 0.5*dx/(Treald)cam.winX*3.14;
            cam.vpol[2] += 0.5*dy/(Treald)cam.winY*3.14;
            cam.refe[0] = -cam.vpol[0]*cos(cam.vpol[2])*cos(cam.vpol[1]) + cam.view[0];
            cam.refe[1] = -cam.vpol[0]*cos(cam.vpol[2])*sin(cam.vpol[1]) + cam.view[1];
            cam.refe[2] = -cam.vpol[0]*sin(cam.vpol[2]) + cam.view[2];
          }
          prev_x = x;
          prev_y = y;
          cam.view[0] = cam.vpol[0]*cos(cam.vpol[2])*cos(cam.vpol[1])+cam.refe[0];
          cam.view[1] = cam.vpol[0]*cos(cam.vpol[2])*sin(cam.vpol[1])+cam.refe[1];
          cam.view[2] = cam.vpol[0]*sin(cam.vpol[2])+cam.refe[2];

          cam.vr[0] = cam.refe[0] - cam.view[0];
          cam.vr[1] = cam.refe[1] - cam.view[1];
          cam.vr[2] = cam.refe[2] - cam.view[2];
          cam.vr[0] /= cam.vpol[0];
          cam.vr[1] /= cam.vpol[0];
          cam.vr[2] /= cam.vpol[0];

          makeMatrix(cam.view[0], cam.view[1], cam.view[2]
                   , cam.refe[0], cam.refe[1], cam.refe[2]
                   , 0.0, cam.rot);
          updateEar();
        } else if(imode == PUSH_WATER || imode == DROP_TEAPOT) {
          px = x; py = y; // pushWaterSurface(x, y);
        }
/*
printf("view: %f %f %f\n", cam.view[0], cam.view[1], cam.view[2]);
printf("refe: %f %f %f\n", cam.refe[0], cam.refe[1], cam.refe[2]);
printf("vr  : %f %f %f\n", cam.vr[0], cam.vr[1], cam.vr[2]);
printf("vpol: %f %f %f\n", cam.vpol[0], cam.vpol[1], cam.vpol[2]);
*/
        if(isim == FALSE) display();
    }
}

Tpoint3f getSurfacePos(int x, int y) {
    Tpoint3f p, ray, pw;
    Treald a;

    p = invtrans((Treald)x, (Treald)(cam.winY - y), &cam);
    ray.x = p.x - cam.view[0];
    ray.y = p.y - cam.view[1];
    ray.z = p.z - cam.view[2];
    if(ABS(ray.z) > 0.00001)
        a = cam.view[2]/ray.z;
    else
        a = 0.0;
    pw.x = cam.view[0]-a*ray.x;
    pw.y = cam.view[1]-a*ray.y;
    pw.z = 0.0;

    return(pw);
}

void genWave(ThapticObj *ho) {

    Trealf h, ve;
    int i;
    Trealf x, y, z, c, d;
    TsamplePoint *sp;

    sp = ho->sp;
    ve = ho->v;
    for(i=0; i<ho->n_sp; i++) {
        x = sp[i].p[0];
        y = sp[i].p[1];
        z = sp[i].p[2];
        h = lerpWaterHeight(x, y);
        c = sp[i].n[0]*ho->vel.x
           +sp[i].n[1]*ho->vel.y
           +sp[i].n[2]*ho->vel.z;
        h = 0.0;
        if(h > z && c > 0.) {
            d = sqrt(ho->vel.x*ho->vel.x+ho->vel.y*ho->vel.y)/2.;
            x += ho->vel.x/d;
            y += ho->vel.y/d;
            iwx = (int)(x/dh+MX/2.0);
            iwy = (int)(y/dh+MY/2.0);
            if(iwx > 1 && iwx < MX-2 && iwy > 1 && iwy < MY-2) {
                if(sim_type == 1) {
                    height[IX(iwx,iwy)] += PUSH_FORCE*ve*exp(-0.2*(h-z))*sp[i].s;
                } else if(sim_type == 2) {
                    height[IX(iwx,iwy)] += boat.wa*ve*exp(-boat.wz*(h-z))*sp[i].s;
                }

            }
       }
   }
}

void updateWaterSurface(int val) {
    static Treald ve;
/*
    if(imode != MOVIE_REPLAY && imode != MOVIE_WRITE) {
        mouse_pos[0][isim_step].x = px;
        mouse_pos[0][isim_step].y = py;
        mouse_pos[1][isim_step].x = prev_x;
        mouse_pos[1][isim_step].y = prev_y;
    } else if(imode == MOVIE_REPLAY || imode == MOVIE_WRITE) {
        px = mouse_pos[0][isim_step].x;
        py = mouse_pos[0][isim_step].y;
        prev_x = mouse_pos[1][isim_step].x;
        prev_y = mouse_pos[1][isim_step].y;
    }
*/
    dpos = 2.0;
    volume = 0.05;
    

    if(imode == PUSH_WATER || imode == MOVIE_REPLAY || imode == MOVIE_WRITE) {
        if(use_spidar == TRUE) {
//            genWave(&pobj);
//            if(boat.id_obj >= 0) genWave(&boat);
        } else if(idrag == TRUE || imode == MOVIE_REPLAY || imode == MOVIE_WRITE || yflow > 0) {
//            updateHapticObj();
//            genWave(&pobj);
//            if(boat.id_obj >= 0) genWave(&boat);
            pobj.prev_pos = pobj.pos;
        }else if(iwx > 0 && iwx < MX-1 && iwy > 0 && iwy < MY-1) {
            pobj.pos.z = height[IX(iwx,iwy)]+POBJ_SIZE/4.;
        }
//            genWave(&pobj);
            if(boat.id_obj >= 0) genWave(&boat);
    } else if(imode == DROP_TEAPOT) {
        if(idrag == TRUE || imode == MOVIE_REPLAY || imode == MOVIE_WRITE) {
            Tpoint3f p;
            p = getSurfacePos(px, py);
            pobj.pos.x = p.x;
            pobj.pos.y = p.y;
            pobj.pos.z += 2.;
            iwx = (int)(pobj.pos.x+MX/2.0);
            iwy = (int)(pobj.pos.y+MY/2.0);
            ve = 0.0;
        } else {
            if(pobj.pos.z > -WATER_DEPTH+POBJ_SIZE/2.) {
                ve += 1.;
                if(pobj.pos.z > 0 && pobj.pos.z-ve < 0
                   && iwx > 0 && iwx < MX-1 && iwy > 0 && iwy < MY-1) {
                    height[IX(iwx,iwy)] -= ve*PUSH_FORCE;
                }
                pobj.pos.z -= ve;
            } else {
                pobj.pos.z = -WATER_DEPTH+POBJ_SIZE/2.;
                ve = 0.0;
            }
        }
    }
    if(isim_step % nmotion == 0) max_motion = 0.0;

    if(isim == TRUE) {

        calPressure(&pobj, (rod.id_obj >= 0 ? FALSE : TRUE));
        if(sim_type == 1) simulate();
        else if(sim_type == 2) simulate2();

        if(rnd_wave > 0.0) {
            int k;
            Trealf wx, wy;
            for(k=0; k<MX*MY*0.01; k++) {
                wx = (Trealf)rand()/(Trealf)RAND_MAX*MX - MX/2.0;
                wx *= dh;
                wy = (Trealf)rand()/(Trealf)RAND_MAX*MY - MY/2.0;
                wy *= dh;
                genRandWave(wx, wy, dh*rnd_wave);
            }
/*
            int i, j, k;
            for(k=0; k<MX*MY*0.005; k++) {
            i = (Trealf)rand()/(Trealf)RAND_MAX*(MX-1)+1;
            j = (Trealf)rand()/(Trealf)RAND_MAX*(MY-1)+1;
            if(rod.id_obj >= 0) {
                j = MY-2;
            }
            height[IX(i,j)] += dh*rnd_wave*(2.0*(Trealf)rand()/(Trealf)RAND_MAX-1.0);
            }
*/
        }
    }
}

#define HX 30
#define HY 30
static Pixel *buf1 = NULL, *buf2;
static GLfloat *zbuf1, *zbuf2;
void genHSMap() {

    int i, j, k, kk;
    int nx, ny;
    Treald a, b, c;
    Trealf vx, vy, vz, rx, ry, rz;
    Trealf vol;


    nx = HX;
    ny = HY;
    
    dx_hsmap = (Trealf)POBJ_SIZE*2.0/(Trealf)HX;
    dy_hsmap = (Trealf)POBJ_SIZE*2.0/(Trealf)HY;
    
    kk = 1;
    if(nx < 100) {nx = (100/nx+1)*nx; kk = nx/HX;}
    if(ny < 100) {ny = (100/ny+1)*ny; kk = ny/HY;}

    if(buf1 == NULL) {
        buf1 = (Pixel *)malloc(sizeof(Pixel)*nx*ny);
        zbuf1 = (GLfloat *)malloc(sizeof(GLfloat)*nx*ny);
        buf2 = (Pixel *)malloc(sizeof(Pixel)*nx*ny);
        zbuf2 = (GLfloat *)malloc(sizeof(GLfloat)*nx*ny);
    }
    glutSetWindow(winID);

//    glDisable(GL_LIGHTING);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    a = POBJ_SIZE;
    b = POBJ_SIZE;
    c = POBJ_SIZE;
    
    glViewport(0, 0, nx, ny);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho((GLdouble)(-a)
           ,(GLdouble)a
           ,(GLdouble)(-b)
           ,(GLdouble)b
           ,(GLdouble)(-c)
           ,(GLdouble)Z_FAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vx = 0.;
    vy = 0.;
    vz = Z_FAR/2.;
    rx = 0.;
    ry = 0.;
    rz = 0.;
    
    vx = pobj.pos.x;
    vy = pobj.pos.y;
    vz = pobj.pos.z+Z_FAR/2.;
    rx = pobj.pos.x;
    ry = pobj.pos.y;
    rz = pobj.pos.z;
    
    gluLookAt( vx, vy, vz, rx, ry, rz, 0, 1, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glEnable(GL_CULL_FACE);

//    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    drawObject(pobj.id_obj);
    glReadPixels(0, 0, nx, ny, GL_RGB, GL_UNSIGNED_BYTE, buf1);
    glReadPixels(0, 0, nx, ny, GL_DEPTH_COMPONENT, GL_FLOAT, zbuf1);

//    glCullFace(GL_BACK);
    glDepthFunc(GL_GREATER);
    drawObject(pobj.id_obj);
    glReadPixels(0, 0, nx, ny, GL_RGB, GL_UNSIGNED_BYTE, buf2);
    glReadPixels(0, 0, nx, ny, GL_DEPTH_COMPONENT, GL_FLOAT, zbuf2);
    glDepthFunc(GL_LEQUAL);
    
    n_zspan = 0;
    for(j=0; j<ny; j+=kk)
        for(i=0; i<nx; i+=kk) {
            k = j*nx + i;
            if( zbuf1[k] < 1.0 && buf1[k].r != 0) n_zspan++;
        }

    if(hsmap == NULL) hsmap = (Tzspan *)malloc(sizeof(Tzspan)*n_zspan);

    int n;
    Trealf x, y, z, f, d;
    f = Z_FAR - c;
    n = 0;
    k = 0;
    vol = 0.0;
    for(j=0; j<ny; j+=kk){
        for(i=0; i<nx; i+=kk) {
            k = j*nx + i;
            if( zbuf1[k] < 1.0 && buf1[k].r != 0) {
                x = a*(i-nx/2.)/(nx/2.);
                y = a*(j-ny/2.)/(ny/2.);
                d = 2.0*zbuf1[k]-1;
                d = f+0.5*(d*f-f);
                z = Z_FAR/2.-d;
                hsmap[n].x = x;
                hsmap[n].y = y;
                hsmap[n].zs = z;
                d = sqrt(x*x+y*y+z*z);
                hsmap[n].ns.x = x/d;
                hsmap[n].ns.y = y/d;
                hsmap[n].ns.z = z/d;
                d = 2.0*zbuf2[k]-1;
                d = f+0.5*(d*f-f);
                z = Z_FAR/2.-d;
                hsmap[n].ze = z;
                d = sqrt(x*x+y*y+z*z);
                hsmap[n].ne.x = x/d;
                hsmap[n].ne.y = y/d;
                hsmap[n].ne.z = z/d;

            // ??? TEMPORARY SETTING ???
                hsmap[n].as = dx_hsmap*dy_hsmap/ABS(hsmap[n].ns.z);
                hsmap[n].ae = dx_hsmap*dy_hsmap/ABS(hsmap[n].ne.z);
                
                vol += hsmap[n].zs - hsmap[n].ze;

                n++;
            }
        }
    }
    
//    pobj.unit_mass = 0.4;
    vol *= dx_hsmap*dy_hsmap;
    pobj.volume = vol;
    pobj.mass = vol*pobj.unit_mass;
    
    printf("n_zspan: %d\n", n_zspan);
    printf("mass : %f\n", pobj.mass);

    glDisable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);
/*
    free(buf1);
    free(buf2);
    free(zbuf1);
    free(zbuf2);
*/
//    glutSwapBuffers();
}

void projectArea() {

    int i, j, k, kk;
    int nx, ny;
    Treald a, b, c;
    Trealf vx, vy, vz, rx, ry, rz;
    Trealf s;
    
    if(pobj.v < 0.01) return;

    nx = HX;
    ny = HY;
        
    kk = 1;
    if(nx < 100) {nx = (100/nx+1)*nx; kk = nx/HX;}
    if(ny < 100) {ny = (100/ny+1)*ny; kk = ny/HY;}

    if(buf1 == NULL) {
        buf1 = (Pixel *)malloc(sizeof(Pixel)*nx*ny);
        zbuf1 = (GLfloat *)malloc(sizeof(GLfloat)*nx*ny);
        buf2 = (Pixel *)malloc(sizeof(Pixel)*nx*ny);
        zbuf2 = (GLfloat *)malloc(sizeof(GLfloat)*nx*ny);
    }
    glutSetWindow(winID);

//    glDisable(GL_LIGHTING);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    a = POBJ_SIZE;
    b = POBJ_SIZE;
    c = POBJ_SIZE;
    
    glViewport(0, 0, nx, ny);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho((GLdouble)(-a)
           ,(GLdouble)a
           ,(GLdouble)(-b)
           ,(GLdouble)b
           ,(GLdouble)(-c)
           ,(GLdouble)Z_FAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vx = pobj.pos.x+pobj.vel.x*100.;
    vy = pobj.pos.y+pobj.vel.y*100.;
    vz = pobj.pos.z+pobj.vel.z*100.;
    rx = pobj.pos.x;
    ry = pobj.pos.y;
    rz = pobj.pos.z;
    
    gluLookAt( vx, vy, vz, rx, ry, rz, 0, 1, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawObject(pobj.id_obj);
    glReadPixels(0, 0, nx, ny, GL_RGB, GL_UNSIGNED_BYTE, buf1);

    s = 0.0;
    for(j=0; j<ny; j+=kk){
        for(i=0; i<nx; i+=kk) {
            k = j*nx + i;
            if( buf1[k].r > 10) s += 1.0;
        }
    }
    

    s *= dx_hsmap*dy_hsmap;
    
    //printf("area: %f\n", s);
    
    pobj.p_area = s;

    glDisable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);

//    glutSwapBuffers();
}


static Pixel *buf = NULL;
static GLfloat *zbuf;
void objWaterIntersection() {

    int i, j;
    int nx, ny;
    Treald a, b, c;
    Trealf vx, vy, vz, rx, ry, rz;
    Trealf x, y;

    nx = MX;
    ny = MY;
/*
    nx = cam.winX;
    ny = cam.winY;

    if(nx < 100) nx = 100;
    if(ny < 100) ny = 100;
*/
    if(buf == NULL) {
        buf = (Pixel *)malloc(sizeof(Pixel)*nx*ny);
        zbuf = (GLfloat *)malloc(sizeof(GLfloat)*nx*ny);
    }
    glutSetWindow(winID);

//    glDisable(GL_LIGHTING);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    a = MX/2.;
    b = MY/2.;
    c = 10.0;
    
    glViewport(0, 0, nx, ny);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho((GLdouble)(-a)
           ,(GLdouble)a
           ,(GLdouble)(-b)
           ,(GLdouble)b
           ,(GLdouble)(-c)
           ,(GLdouble)Z_FAR);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    vx = 0.;
    vy = 0.;
    vz = Z_FAR/2.;
    rx = 0.;
    ry = 0.;
    rz = 0.;
    gluLookAt( vx, vy, vz, rx, ry, rz, 0, 1, 0);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    y = yo;
    for (j = 0; j < MY-1; j++) {
      glBegin(GL_QUAD_STRIP);
        x = xo;
        for (i = 0; i < MX; i++) {
          glVertex3f(x, y, height[IX(i,j)]);
          glVertex3f(x, y+dh, height[IX(i,j+1)]);
          x += dh;
        }
      glEnd();
      y+=dh;
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_GEQUAL);

    glPushMatrix();
    glTranslatef(pobj.pos.x, pobj.pos.y, pobj.pos.z);
    i = pobj.id_obj;
    Tface *fac = obj[i].face;
    glColor3f(1.0, 1.0, 1.0);
    for(j=0; j<obj[i].nf; j++) {
        if(fac->pv[0] != NULL) {
        glBegin(GL_POLYGON);
          glVertex3f(fac->pv[0]->p[0], fac->pv[0]->p[1], fac->pv[0]->p[2]);
          glVertex3f(fac->pv[1]->p[0], fac->pv[1]->p[1], fac->pv[1]->p[2]);
          glVertex3f(fac->pv[2]->p[0], fac->pv[2]->p[1], fac->pv[2]->p[2]);
          if(fac->pv[3] != NULL) {
              glVertex3f(fac->pv[3]->p[0], fac->pv[3]->p[1], fac->pv[3]->p[2]);
          }
        glEnd();
        }
        fac++;
    }
    glPopMatrix();

    glDepthFunc(GL_LEQUAL);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    y = yo;
    for (j = 0; j < MY-1; j++) {
      glBegin(GL_QUAD_STRIP);
        x = xo;
        for (i = 0; i < MX; i++) {
//          glColor3f(i/255., j/255., 0);
          glColor3ub((GLubyte)i, (GLubyte)j, 0);
          glVertex3f(x, y, height[IX(i,j)]);
//          glColor3f(i/255., (j+1)/255., 0);
          glColor3ub((GLubyte)i, (GLubyte)(j+1), 0);
          glVertex3f(x, y+dh, height[IX(i,j+1)]);
          x += dh;
        }
      glEnd();
      y += dh;
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);


    glReadPixels(0, 0, nx, ny, GL_RGB, GL_UNSIGNED_BYTE, buf);

    int k;
    Trealf z, f, d;
    f = Z_FAR - c;
    k = 0;
    now = 0;
    for(j=0; j<ny; j++){
        for(i=0; i<nx; i++) {
            if( zbuf[k] < 1.0 && buf[k].r != 0) {
                y = a*(i-nx/2.)/(nx/2.);
                z = a*(j-ny/2.)/(ny/2.);
                d = 2.0*zbuf[k]-1;
                d = f+0.5*(d*f-f);
                x = vz-d;
                zow[now]=x;
                xow[now]=buf[k].g;
                yow[now]=buf[k].r;
                now++;
            }
            k++;
        }
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
//    glEnable(GL_LIGHTING);

//    glutSwapBuffers();
}

void updateWaterSurfaceOri(int val) {
    static Treald v;
    int i;

    if(imode == PUSH_WATER) {
        if(idrag == TRUE) {
            pobj.pos = getSurfacePos(px, py);
            v = (prev_x-px)*(prev_x-px)+(prev_y-py)*(prev_y-py);
            v = sqrt(v);
            iwx = (int)(pobj.pos.x+MX/2.0);
            iwy = (int)(pobj.pos.y+MY/2.0);
            if(iwx > 0 && iwx < MX-1 && iwy > 0 && iwy < MY-1) {
                height[IX(iwx,iwy)] -= PUSH_FORCE*v/10.;
                if(kick_sound == FALSE) {
                    kick_sound = TRUE;
                    for(i=0; i<nstex; i++) spos[i] = 0.0;
                }
                int j;
                i=0;
                j = FALSE;
                volume = v*0.1;
                while(i<nss && j == FALSE) {
                    if(sspos[i] < 0.0) {
                        itex[i] = ittt;
                        if(itex[i] >= nstex) itex[i] = nstex-1;
                        ittt++; if(ittt >= nstex) ittt = 0;
                        sspos[i] = stex[itex[i]].nsmp*(Treald)rand()/(Treald)RAND_MAX*0.8;
                        svpos[i] = 20.0/v+1.0+(Treald)rand()/(Treald)RAND_MAX*5.0;
                        ssvol[i] = volume*0.5;
                        j = TRUE;
                    }
                    i++;
                }
            }
            prev_x = px; prev_y = py;
        }else if(iwx > 0 && iwx < MX-1 && iwy > 0 && iwy < MY-1) {
            pobj.pos.z = height[IX(iwx,iwy)]+POBJ_SIZE/4.;
        }
    } else if(imode == DROP_TEAPOT) {
        if(idrag == TRUE) {
            Tpoint3f p;
            p = getSurfacePos(px, py);
            pobj.pos.x = p.x;
            pobj.pos.y = p.y;
            pobj.pos.z += 2.;
            iwx = (int)(pobj.pos.x+MX/2.0);
            iwy = (int)(pobj.pos.y+MY/2.0);
            v = 0.0;
        } else {
            if(pobj.pos.z > -WATER_DEPTH+POBJ_SIZE/2.) {
                v += 1.;
                if(pobj.pos.z > 0 && pobj.pos.z-v < 0
                   && iwx > 0 && iwx < MX-1 && iwy > 0 && iwy < MY-1) {
                    height[IX(iwx,iwy)] -= v*PUSH_FORCE;
                    for(i=0; i<nstex; i++) spos[i] = 0.0;
                    kick_sound = TRUE;
                    dpos = 10./v;
                    volume = v*0.1;
                    volume *= volume;
                    if(dpos < 0.5) dpos = 0.5;
                    if(dpos > 3.) dpos = 3.;
                }
                pobj.pos.z -= v;
            } else {
                pobj.pos.z = -WATER_DEPTH+POBJ_SIZE/2.;
                v = 0.0;
            }
        }
    }
    if(isim_step % nmotion == 0) max_motion = 0.0;

    simulate();
    if(kick_sound == TRUE && isound == SOUND_EFFECT_ON) {
        if(imode == DROP_TEAPOT) timer_drop_sound(TRUE);
        else if(imode == PUSH_WATER) timer_push_sound(TRUE);
    }
    display();
    if(iwrt_bmp == TRUE) writeFrameBmp(isim_step);
    if(isim_step < nmotion-1) {
        if(isim == TRUE) glutTimerFunc(TIMER_INTERVAL, updateWaterSurface, TRUE);
        isim_step++;
    } else {
        isim = FALSE; printf("isim %d\n", isim_step);
    }
}

void drawUnitCube() {

    glBegin(GL_QUADS);
      glNormal3f(0., 0., -1.);
      glTexCoord2f(0., 1.);
      glVertex3f(-0.5, -0.5, -0.5);
      glTexCoord2f(1., 1.);
      glVertex3f( 0.5, -0.5, -0.5);
      glTexCoord2f(1., 0.);
      glVertex3f( 0.5,  0.5, -0.5);
      glTexCoord2f(0., 0.);
      glVertex3f(-0.5,  0.5, -0.5);

      glNormal3f(0., 0., 1.);
      glTexCoord2f(0., 1.);
      glVertex3f(-0.5,  0.5,  0.5);
      glTexCoord2f(1., 1.);
      glVertex3f( 0.5,  0.5,  0.5);
      glTexCoord2f(1., 0.);
      glVertex3f( 0.5, -0.5,  0.5);
      glTexCoord2f(0., 0.);
      glVertex3f(-0.5, -0.5,  0.5);

      glNormal3f(0., -1., 0.);
      glTexCoord2f(0., 1.);
      glVertex3f(-0.5, -0.5, -0.5);
      glTexCoord2f(1., 1.);
      glVertex3f(-0.5, -0.5,  0.5);
      glTexCoord2f(1., 0.);
      glVertex3f( 0.5, -0.5,  0.5);
      glTexCoord2f(0., 0.);
      glVertex3f( 0.5, -0.5, -0.5);

      glNormal3f(0., 1., 0.);
      glTexCoord2f(0., 1.);
      glVertex3f( 0.5,  0.5, -0.5);
      glTexCoord2f(1., 1.);
      glVertex3f( 0.5,  0.5,  0.5);
      glTexCoord2f(1., 0.);
      glVertex3f(-0.5,  0.5,  0.5);
      glTexCoord2f(0., 0.);
      glVertex3f(-0.5,  0.5, -0.5);

      glNormal3f(-1., 0., 0.);
      glTexCoord2f(0., 1.);
      glVertex3f(-0.5, -0.5, -0.5);
      glTexCoord2f(1., 1.);
      glVertex3f(-0.5,  0.5, -0.5);
      glTexCoord2f(1., 0.);
      glVertex3f(-0.5,  0.5,  0.5);
      glTexCoord2f(0., 0.);
      glVertex3f(-0.5, -0.5,  0.5);

      glNormal3f( 1., 0., 0.);
      glTexCoord2f(0., 1.);
      glVertex3f( 0.5, -0.5,  0.5);
      glTexCoord2f(1., 1.);
      glVertex3f( 0.5,  0.5,  0.5);
      glTexCoord2f(1., 0.);
      glVertex3f( 0.5,  0.5, -0.5);
      glTexCoord2f(0., 0.);
      glVertex3f( 0.5, -0.5, -0.5);
    glEnd();
}

void drawSideWall() {
   Trealf thick = 3., height = WATER_DEPTH*1.4;

    thick *= dh;
    glPushMatrix();
      glTranslatef(-MX/2.*dh, 0.0, -WATER_DEPTH/2.);
      glScalef(thick/2., MY*dh, height);
      drawUnitCube();
    glPopMatrix();
    glPushMatrix();
      glTranslatef( MX/2.*dh, 0.0, -WATER_DEPTH/2.);
      glScalef(thick/2., MY*dh, height);
      drawUnitCube();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.0, -MY/2.*dh, -WATER_DEPTH/2.);
      glScalef(MX*dh, thick/2., height);
      drawUnitCube();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.0,  MY/2.*dh, -WATER_DEPTH/2.);
      glScalef(MX*dh, thick/2., height);
      drawUnitCube();
    glPopMatrix();
}

void drawBottom() {
   Trealf mat_diffuse[4], spec[4];
   
   glEnable(GL_LIGHTING);
   mat_diffuse[0] = 0.8;
   mat_diffuse[1] = 0.8;
   mat_diffuse[2] = 0.8;
   mat_diffuse[3] = 0.8;
   spec[0] = spec[1] = spec[2] = spec[3] = 0.0;
   glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, &water_shin);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texname[ground_map]);
    glColor3f(1., 1., 1.);
    glBegin(GL_QUADS);
      glTexCoord2f(0., 1.);
      glNormal3f(0., 0., 1.);
      glVertex3f(-MX*dh,  MY*dh, -WATER_DEPTH);

      glTexCoord2f(1., 1.);
      glNormal3f(0., 0., 1.);
      glVertex3f( MX*dh,  MY*dh, -WATER_DEPTH);

      glTexCoord2f(1., 0.);
      glNormal3f(0., 0., 1.);
      glVertex3f( MX*dh, -MY*dh, -WATER_DEPTH);

      glTexCoord2f(0., 0.);
      glNormal3f(0., 0., 1.);
      glVertex3f(-MX*dh, -MY*dh, -WATER_DEPTH);

    glEnd();
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

Treald getAreaofTriangle(Tpoint3d p[3]) {

    Treald a, b, c, dx, dy, dz, s;

    dx = p[0].x - p[1].x;
    dy = p[0].y - p[1].y;
    dz = p[0].z - p[1].z;
    a = sqrt(dx*dx+dy*dy+dz*dz);

    dx = p[1].x - p[2].x;
    dy = p[1].y - p[2].y;
    dz = p[1].z - p[2].z;
    b = sqrt(dx*dx+dy*dy+dz*dz);

    dx = p[0].x - p[2].x;
    dy = p[0].y - p[2].y;
    dz = p[0].z - p[2].z;
    c = sqrt(dx*dx+dy*dy+dz*dz);

    s = 0.5*(a+b+c);

    return((Treald)(sqrt(s*(s-a)*(s-b)*(s-c))));
}

void drawCaustics() {
    Tpoint3d ps[3], pb[3], tv[3];
    int i,j,k;
    Treald t, ss, sb, f, x, y;

    glDepthMask(GL_FALSE);
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_BLEND);
    glBegin(GL_TRIANGLES);
    y = yo;
    for (j = 0; j < MY-1; j++) {
        x = xo;
        for (i = 0; i < MX-1; i++) {
            tv[0] = tvec[IX(i,j)];
            tv[1] = tvec[IX(i+1,j+1)];
            tv[2] = tvec[IX(i+1,j)];
            ps[0].x = x;
            ps[0].y = y;
            ps[0].z = height[IX(i,j)];
            ps[1].x = x+dh;
            ps[1].y = y+dh;
            ps[1].z = height[IX(i+1,j+1)];
            ps[2].x = x+dh;
            ps[2].y = y;
            ps[2].z = height[IX(i+1,j)];
            ss = getAreaofTriangle(ps);
            for(k=0; k<3; k++) {
                if(ABS(tv[k].z) > 0.00001) {
                    t = (-WATER_DEPTH - ps[k].z)/tv[k].z;
                    pb[k].x = ps[k].x+tv[k].x*t;
                    pb[k].y = ps[k].y+tv[k].y*t;
                    pb[k].z = -WATER_DEPTH;
                } else {
                    pb[k].x = ps[k].x;
                    pb[k].y = ps[k].y;
                    pb[k].z = -WATER_DEPTH;
                }
            }
            sb = getAreaofTriangle(pb);
            f = ss/sb;
            f *= f;
            f *= f;
            glColor3f(0.1*f, 0.1*f, 0.1*f);
            glVertex3f(pb[0].x, pb[0].y, pb[0].z);
            glVertex3f(pb[1].x, pb[1].y, pb[1].z);
            glVertex3f(pb[2].x, pb[2].y, pb[2].z);

            tv[0] = tvec[IX(i,j)];
            tv[1] = tvec[IX(i,j+1)];
            tv[2] = tvec[IX(i+1,j+1)];
            ps[0].x = x;
            ps[0].y = y;
            ps[0].z = height[IX(i,j)];
            ps[1].x = x;
            ps[1].y = y+dh;
            ps[1].z = height[IX(i,j+1)];
            ps[2].x = x+dh;
            ps[2].y = y+dh;
            ps[2].z = height[IX(i+1,j+1)];
            ss = getAreaofTriangle(ps);
            for(k=0; k<3; k++) {
                if(ABS(tv[k].z) > 0.00001) {
                    t = (-WATER_DEPTH - ps[k].z)/tv[k].z;
                    pb[k].x = ps[k].x+tv[k].x*t;
                    pb[k].y = ps[k].y+tv[k].y*t;
                    pb[k].z = -WATER_DEPTH;
                } else {
                    pb[k].x = ps[k].x;
                    pb[k].y = ps[k].y;
                    pb[k].z = -WATER_DEPTH;
                }
            }
            sb = getAreaofTriangle(pb);
            f = ss/sb;
            f *= f;
            f *= f;
            glColor3f(0.1*f, 0.1*f, 0.1*f);
            glVertex3f(pb[0].x, pb[0].y, pb[0].z);
            glVertex3f(pb[1].x, pb[1].y, pb[1].z);
            glVertex3f(pb[2].x, pb[2].y, pb[2].z);
            x += dh;
        }
        y += dh;
    }
    glEnd();

//    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
//    glEnable(GL_DEPTH_TEST);
}

void drawWaterSurface() {
    int i, j;
    Trealf x, y;

    if(ground_map >= 0) drawBottom();

    if(box_map >= 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texname[box_map]);
        glColor3f(0.6, 0.6, 0.6);
//        glDisable(GL_LIGHTING);
        drawSideWall();
//        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    }

    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT, GL_SPECULAR, water_spec);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, water_diffuse);
    glMaterialfv(GL_FRONT, GL_SHININESS, &water_shin);
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);
    glEnable(GL_BLEND);
    if(env_map >= 0) {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texname[env_map]);
    }
    

    y = yo;
	
	if (screen != CENTER_SCREEN){
		Affinef mat;
		mat.Pos() = Vec3f(-10,0,0);
		recvOri.to_matrix(mat.Rot());

//		static float texOffset=-50;
//		texOffset += yflow * 0.03f;
//		if (texOffset > 40) texOffset = -50;
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
//		glTranslatef(texOffset, 0, 0);
		glMultMatrixf(mat);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		Affinef model;
		glGetFloatv(GL_MODELVIEW_MATRIX, model);
		glLoadIdentity();
		glMultMatrixf(mat.inv());
		glMultMatrixf(model);
	}

    for (j = 0; j < MY-1; j++) {
      glBegin(GL_QUAD_STRIP);
        x = xo;
        for (i = 0; i < MX; i++) {
            glNormal3f(normal[IX(i,j)].x ,normal[IX(i,j)].y ,normal[IX(i,j)].z);
	        glVertex3f(x, y, height[IX(i,j)]);

            glNormal3f(normal[IX(i,j+1)].x ,normal[IX(i,j+1)].y ,normal[IX(i,j+1)].z);
          glVertex3f(x, y+dh, height[IX(i,j+1)]);
          x += dh;
        }
      glEnd();
      y += dh;
    }
	if (screen != CENTER_SCREEN){
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
    
    glDisable(GL_LIGHTING);
    if(env_map >= 0) {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
    
//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(disp_wave_vel == TRUE) {
//        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        glColor3f(1., 0., 0.);
        y = yo;
        for (j = 0; j < MY; j+=1) {
            x = xo;
            for (i = 0; i < MX; i+=1) {
                glVertex3f(x, y, height[IX(i,j)]);
                glVertex3f(x+u[IX(i,j)]*50, y+v[IX(i,j)]*50, height[IX(i,j)]);
                x += dh;
            }
            y += dh;
        }
        glEnd();
//        glEnable(GL_LIGHTING);
    }
}


void drawPressure() {
    int i, j;
    Trealf x, y;

    if(ground_map >= 0) drawBottom();
    if(box_map >= 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texname[box_map]);
        glColor3f(1., 1., 1.);
        drawSideWall();
        glDisable(GL_TEXTURE_2D);
    }

//    glDisable(GL_LIGHTING);

    y = yo;
    for (j = 0; j < MY-1; j++) {
      glBegin(GL_QUAD_STRIP);
        x = xo;
        for (i = 0; i < MX; i++) {
          if(pw[IX(i,j)] > 0.0)
              glColor3f(pw[IX(i,j)]*1., 0.0, 0.0);
          else
              glColor3f(0.0, 0.0, -pw[IX(i,j)]*1.);
          glVertex3f(x, y, height[IX(i,j)]);

          if(pw[IX(i,j+1)] > 0.0)
              glColor3f(pw[IX(i,j+1)]*1., 0.0, 0.0);
          else
              glColor3f(0.0, 0.0, -pw[IX(i,j+1)]*1.);
          glVertex3f(x, y+dh, height[IX(i,j+1)]);
          x += dh;
        }
      glEnd();
      y += dh;
    }

//    glEnable(GL_LIGHTING);

}

void renderCharacter(char *text, int x_pos, int y_pos) {

    int i, s, len;

    len = strlen(text);

    glDisable(GL_DEPTH_TEST);
    glPushMatrix();

    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLfloat)cam.winX, 0.0, (GLfloat)cam.winY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPixelZoom(1.0, 1.0);
    glRasterPos2i(x_pos,y_pos);
    for(i = 0; i<len; i++) {
       s = text[i];
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s);
    }
    glPixelZoom(1.0,1.0);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glRasterPos2i(0, 0);
}


void drawWave() {

    int i, i0, k, kk;
    int ws = 0, wc = 50;  //, sstep, istep;
    Trealf dk, fk, d;
//    Treald max;

    wc = 0.1*cam.winY;
    if(wc > 50) wc = 50;

    glPushMatrix();

    glViewport(0.0, 0.0, (GLfloat)cam.winX, (GLfloat)wc*2);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLfloat)cam.winX, 0.0, (GLfloat)wc*2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    glEnable(GL_BLEND);
    glColor3f(0.25, 0.25, 0.25);
    glBegin(GL_QUADS);
      glVertex2i(0, wc*2);
      glVertex2i(cam.winX, wc*2);
      glVertex2i(cam.winX, 0);
      glVertex2i(0, 0);
    glEnd();
    glDisable(GL_BLEND);
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
      glVertex2i(ws, wc);
      glVertex2i(cam.winX, wc);
    glEnd();

// DRAW FORCE
/*
    i0 = kforce - (kforce % nforce_max);
    dk = (Trealf)cam.winX/(Trealf)nforce_max;
*/
    i0 = kforce - (kforce % 2500);
    dk = (Trealf)cam.winX/(Trealf)2500;

    glLineWidth(2.0);
    glBegin(GL_LINES);
    fk = 0.;
    for(i=i0; i<kforce-1; i++) {
        k = (int)fk;
        kk = (int)(fk+dk);
/*
        glColor3f(0.75, 0.75, 0.75);
        glVertex2i(ws+k, (int)(wc+vel_data[i]*wc/ve_max));
        glVertex2i(ws+kk, (int)(wc+vel_data[i+1]*wc/ve_max));
*/
        glColor3f(1.0, 0., 0.);
        d = force_abs[i];
        if(force_max > 0.0) d /= force_max/frc_wav_scl;
        glVertex2i(ws+k, (int)(wc+d*wc));
        d = force_abs[i+1];
        if(force_max > 0.0) d /= force_max/frc_wav_scl;
        glVertex2i(ws+kk, (int)(wc+d*wc));

/*
        glColor3f(1., 0., 0.);
        glVertex2i(ws+k, (int)(wc+force_data[i].x*1./force_max*wc));
        glVertex2i(ws+kk, (int)(wc+force_data[i+1].x*1./force_max*wc));

        glColor3f(0., 1., 0.);
        glVertex2i(ws+k, (int)(wc+force_data[i].y*1./force_max*wc));
        glVertex2i(ws+kk, (int)(wc+force_data[i+1].y*1./force_max*wc));

        glColor3f(0., 0., 1.);
        glVertex2i(ws+k, (int)(wc+force_data[i].z*1./force_max*wc));
        glVertex2i(ws+kk, (int)(wc+force_data[i+1].z*1./force_max*wc));
*/
        fk += dk;
    }
    glEnd();

// DRAW TORQUE
/*
    glBegin(GL_LINES);
    fk = 0;
    for(i=i0; i<kforce; i++) {
        k = (int)fk;
        kk = (int)(fk+dk);

        glColor3f(0., 0., 1.);
        d = torq_data[i].x*torq_data[i].x+torq_data[i].y*torq_data[i].y+torq_data[i].z*torq_data[i].z;
        if(d > 0.0) d = sqrt(d);
        if(torq_max > 0.0) d /= torq_max*5;
        glVertex2i(ws+k, (int)(wc+d*wc));
        d = torq_data[i+1].x*torq_data[i+1].x+torq_data[i+1].y*torq_data[i+1].y+torq_data[i+1].z*torq_data[i+1].z;
        if(d > 0.0) d = sqrt(d);
        if(torq_max > 0.0) d /= torq_max*5;
        glVertex2i(ws+kk, (int)(wc+d*wc));
        fk += dk;
    }
    glEnd();
*/
    glLineWidth(1.0);
    glPopMatrix();
    glViewport(0.0, 0.0, (GLfloat)cam.winX, (GLfloat)cam.winY);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,cam.winX, 0,cam.winY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawAxis() {
    glPushMatrix();
    glTranslatef(cam.refe[0], cam.refe[1], cam.refe[2]);
    glLineWidth(2.0);
//    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
      glColor3f(0.25, 0., 0.);
      glVertex3f(-5.0, 0.0, 0.0);
      glColor3f(1., 0., 0.);
      glVertex3f( 5.0, 0.0, 0.0);

      glColor3f(0., 0.25, 0.);
      glVertex3f(0.0,-5.0, 0.0);
      glColor3f(0., 1., 0.);
      glVertex3f(0.0, 5.0, 0.0);

      glColor3f(0., 0., 0.25);
      glVertex3f(0.0, 0.0,-5.0);
      glColor3f(0., 0., 1.);
      glVertex3f(0.0, 0.0, 5.0);
    glEnd();
//    glEnable(GL_LIGHTING);
    glLineWidth(1.0);
    glPopMatrix();
}

void drawHapticPoints() {
    if(disp_hap_src == TRUE) {
        int i;
        glPointSize(5.);
//        glDisable(GL_LIGHTING);
        glColor3f(1., 0., 0.);
        glBegin(GL_POINTS);
          for(i=0; i<pobj.nhsrc; i++)
            glVertex3f(pobj.hsrc[i].x, pobj.hsrc[i].y, pobj.hsrc[i].z);
          glColor3f(1., 1., 0.);
          glVertex3f(pobj.pos.x, pobj.pos.y, pobj.pos.z);
        glEnd();
        glPointSize(1.0);
//        glEnable(GL_LIGHTING);
    }
    if(disp_hsmap == TRUE) {
        int i;
        glPointSize(3.);
//        glDisable(GL_LIGHTING);
        glPushMatrix();
        glTranslatef(pobj.pos.x, pobj.pos.y, pobj.pos.z);
        glBegin(GL_POINTS);
          for(i=0; i<n_zspan; i++) {
            glColor3f(1., 0., 0.);
            glPointSize(hsmap[i].as*100);
            glVertex3f(hsmap[i].x, hsmap[i].y, hsmap[i].zs);
            glColor3f(0., 0., 1.);
            glPointSize(hsmap[i].ae*100);
            glVertex3f(hsmap[i].x, hsmap[i].y, hsmap[i].ze);
          }
        glEnd();
        glBegin(GL_LINES);
          for(i=0; i<n_zspan; i++) {
            glColor3f(1., 0., 0.);
            glVertex3f(hsmap[i].x, hsmap[i].y, hsmap[i].zs);
            glVertex3f(hsmap[i].x+hsmap[i].ns.x, hsmap[i].y+hsmap[i].ns.y, hsmap[i].zs+hsmap[i].ns.z);
            glColor3f(0., 0., 1.);
            glVertex3f(hsmap[i].x, hsmap[i].y, hsmap[i].ze);
            glVertex3f(hsmap[i].x+hsmap[i].ne.x, hsmap[i].y+hsmap[i].ne.y, hsmap[i].ze+hsmap[i].ne.z);
            glColor3f(0.7, 0.7, 0.7);
            glVertex3f(hsmap[i].x, hsmap[i].y, hsmap[i].zs);
            glVertex3f(hsmap[i].x, hsmap[i].y, hsmap[i].ze);
          }
        glEnd();
        glPopMatrix();
//        glEnable(GL_LIGHTING);
        glPointSize(1.0);
    }
    if(disp_point == TRUE) {
        int i;
        TsamplePoint *sp;
        
        glPointSize(5.);
//        glDisable(GL_LIGHTING);
        glColor3f(1., 1., 0.);
        glBegin(GL_POINTS);
          sp = pobj.sp;
          for(i=0; i<pobj.n_sp; i++) {
              glColor3f(pobj.hsrc[sp[i].i_hsrc].col.r, pobj.hsrc[sp[i].i_hsrc].col.g, pobj.hsrc[sp[i].i_hsrc].col.b);
              glVertex3f(sp[i].p[0], sp[i].p[1], sp[i].p[2]);
          }
        glEnd();
/*
        glColor3f(1., 0., 0.);
        glBegin(GL_LINES);
          sp = pobj.sp;
          for(i=0; i<pobj.n_sp; i++) {
              glVertex3f(sp[i].p[0], sp[i].p[1], sp[i].p[2]);
              glVertex3f(sp[i].p[0]+sp[i].n[0], sp[i].p[1]+sp[i].n[1], sp[i].p[2]+sp[i].n[2]);
          }
        glEnd();
*/
        if(boat.id_obj >= 0) {
            glColor3f(1., 0., 0.);
            glBegin(GL_POINTS);
              sp = boat.sp;
              for(i=0; i<boat.n_sp; i++)
                  glVertex3f(sp[i].p[0], sp[i].p[1], sp[i].p[2]);
            glEnd();

            glColor3f(1., 0., 0.);
            glBegin(GL_LINES);
              sp = boat.sp;
              for(i=0; i<boat.n_sp; i++) {
                  glVertex3f(sp[i].p[0], sp[i].p[1], sp[i].p[2]);
                  glVertex3f(sp[i].p[0]+sp[i].n[0]*0.1, sp[i].p[1]+sp[i].n[1]*0.1, sp[i].p[2]+sp[i].n[2]*0.1);
              }
            glEnd();

        }

//        glEnable(GL_LIGHTING);
        glPointSize(1.0);
    }
}

// 矢印の表示
void drawArrow(int n) {
    Trealf dz, d, the, phi, a1 = 1.5, a2 = 1.75, ah = 2.5;
    Tpoint3f dir, pp;

    the = 0.0;
    phi = 0.0;
    if(rod.id_obj >= 0 && lure_view == FALSE) {
        pp = rod.pos;
        dir = rod.tension;
        dz = rod.tension.x*rod.tension.x+rod.tension.y*rod.tension.y+rod.tension.z*rod.tension.z;
        if(dz > 0) {
            dz = sqrt(dz);
            dir.x /= dz;
            dir.y /= dz;
            dir.z /= dz;
        }
    } else if(rod.id_obj >= 0) {
        pp = pobj.fpos;
        dz = 0.0;
        if(pobj.n_fpos > 0) {
            dir = force_data[n];
            dz = force_abs[n];
            dz = force_data[n].x*force_data[n].x
                +force_data[n].y*force_data[n].y
                +force_data[n].z*force_data[n].z;
            if(dz > 0.0) {
                dz = sqrt(dz);
                dir.x /= dz;
                dir.y /= dz;
                dir.z /= dz;
            }
        }
    } else {
        pp = pobj.fpos;
        dz = 0.0;
        if(pobj.n_fpos > 0) {
            dir = force_data[n];
            dz = force_abs[n];
            if(dz > 0.0) {
                dir.x /= dz;
                dir.y /= dz;
                dir.z /= dz;
            }
        }
    }
    if(rod.id_obj >= 0 && lure_view == TRUE) {
        a1 = 0.25; a2 = 0.2; ah = 0.75;
    }
    d = dir.x*dir.x+dir.y*dir.y;
    d = sqrt(d);
    if(dir.z >= 1.0) the = 0.0;
    else            the = acos(dir.z);
    if(dir.x >= d) phi = 0.0;
    else          phi = acos(dir.x/d);
    if(dir.y < 0.0) phi = 2.0*M_PI - phi;
    the = the/M_PI*180.;
    phi = phi/M_PI*180.;
    if(dz > 0.0) {
        dz *= frc_arrw_scl;
        glPushMatrix();
          glColor3f(1.0, 0., 0.);
            glTranslatef(pp.x, pp.y, pp.z);
            glRotatef(phi, 0.0, 0.0, 1.0);
            glRotatef(the, 0.0, 1.0, 0.0);
          glPushMatrix();
            glTranslatef(0., 0., dz);
            glutSolidCone(a1, ah, 20, 2);
          glPopMatrix();
          glScalef(a2, a2, dz);
          glTranslatef(0., 0., 0.5);
          glutSolidCube(1.0);
        glPopMatrix();
    }
}

void drawParam() {

    char str[256];
static Trealf fmax = 0, tmax = 0;
    int n;
    
    n = kforce-1;
//    glDisable(GL_LIGHTING);
    if(n >= 0) {
/*
        Trealf *m;
        m = obj[pobj.id_obj].m;
        glDisable(GL_LIGHTING);
        glLineWidth(3.0);
        glPushMatrix();
        glTranslatef(pobj.pos.x, pobj.pos.y, pobj.pos.z);
        glScalef(20., 20., 20.);
        glBegin(GL_LINES);
          glColor3f(1.0, 0.0, 0.0);
          glVertex3f(0.0, 0.0, 0.0);
          glVertex3f(m[0], m[1], m[2]);
          glColor3f(0.0, 1.0, 0.0);
          glVertex3f(0.0, 0.0, 0.0);
          glVertex3f(m[4], m[5], m[6]);
          glColor3f(0.0, 0.0, 1.0);
          glVertex3f(0.0, 0.0, 0.0);
          glVertex3f(m[8], m[9], m[10]);
        glEnd();
        glPopMatrix();

        if(boat.id_obj >= 0) {
            m = obj[boat.id_obj].m;
            glPushMatrix();
            glTranslatef(boat.pos.x, boat.pos.y, boat.pos.z);
            glScalef(20., 20., 20.);
            glBegin(GL_LINES);
              glColor3f(1.0, 0.0, 0.0);
              glVertex3f(0.0, 0.0, 0.0);
              glVertex3f(m[0], m[1], m[2]);
              glColor3f(0.0, 1.0, 0.0);
              glVertex3f(0.0, 0.0, 0.0);
              glVertex3f(m[4], m[5], m[6]);
              glColor3f(0.0, 0.0, 1.0);
              glVertex3f(0.0, 0.0, 0.0);
              glVertex3f(m[8], m[9], m[10]);
            glEnd();
            glPopMatrix();
        }
*/
    drawArrow(n);
/*
        glLineWidth(3.0);
        glPushMatrix();
        glTranslatef(pobj.pos.x, pobj.pos.y, pobj.pos.z);
        glScalef(30.0, 30.0, 30.0);
        glBegin(GL_LINES);
            glColor3f(0.5, 0., 0.);
            glVertex3f(0., 0., 0.);
            glVertex3f(force_data[n].x/force_max
                     , force_data[n].y/force_max
                     , force_data[n].z/force_max);
*/
/*
            Trealf d;
            d = force_abs[n];
            if(d == 0.0) d = 1.0;
            glColor3f(0.5, 0., 0.);
            glVertex3f(0., 0., 0.);
            glVertex3f(force_data[n].x/d //force_max*5.
                     , force_data[n].y/d //force_max*5.
                     , force_data[n].z/d); //force_max*5.);
*/
/*
            glColor3f(0., 0., 0.5);
            glVertex3f(0., 0., 0.);
            glVertex3f(torq_data[n].x/torq_max*5.
                     , torq_data[n].y/torq_max*5.
                     , torq_data[n].z/torq_max*5.);
*/
/*
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(0., 0., 0.);
            glVertex3f(obj[pobj.id_obj].m[0]
                      ,obj[pobj.id_obj].m[1]
                      ,obj[pobj.id_obj].m[2]);

            glColor3f(0.0, 1.0, 0.0);
            glVertex3f(0., 0., 0.);
            glVertex3f(obj[pobj.id_obj].m[4]
                      ,obj[pobj.id_obj].m[5]
                      ,obj[pobj.id_obj].m[6]);
            glColor3f(0.0, 0.0, 1.0);
            glVertex3f(0., 0., 0.);
            glVertex3f(obj[pobj.id_obj].m[8]
                      ,obj[pobj.id_obj].m[9]
                      ,obj[pobj.id_obj].m[10]);
*/
/*
            glColor3f(1., 0., 0.);
            glVertex3f(pobj.pos.x, pobj.pos.y, pobj.pos.z);
            glVertex3f(pobj.pos.x+uuu.x*50.
                     , pobj.pos.y+uuu.y*50.
                     , pobj.pos.z+uuu.z*50.);
            glColor3f(0., 1., 0.);
            glVertex3f(pobj.pos.x, pobj.pos.y, pobj.pos.z);
            glVertex3f(pobj.pos.x+vvv.x*50.
                     , pobj.pos.y+vvv.y*50.
                     , pobj.pos.z+vvv.z*50.);
            glColor3f(0., 0., 1.);
            glVertex3f(pobj.pos.x, pobj.pos.y, pobj.pos.z);
            glVertex3f(pobj.pos.x+www.x*50.
                     , pobj.pos.y+www.y*50.
                     , pobj.pos.z+www.z*50.);
*/
        glEnd();
        glPopMatrix();
        glLineWidth(1.0);

    }
/*
    glDisable(GL_DEPTH_TEST);

    glColor3f(1., 1., 1.);
    str[0] ='\0';
    if(imode == MOVE_CAMERA)
        sprintf(str, "mode: move camera\n");
    else if(imode == PUSH_WATER)
        sprintf(str, "mode: push water\n");
    else if(imode == DROP_TEAPOT)
        sprintf(str, "mode: drop teapot\n");
    else if(imode == MOVIE_REPLAY)
        sprintf(str, "mode: replay\n");
    else if(imode == MOVIE_WRITE)
        sprintf(str, "mode: writing movie\n");

    int k;
    k = 12;
    renderCharacter(str, 5, cam.winY-k); k += 15;
    sprintf(str, "pointer obj pos.: %6.2f %6.2f %6.2f   vel.: %6.2f\n", pobj.pos.x, pobj.pos.y, pobj.pos.z, pobj.v);
    renderCharacter(str, 5, cam.winY-k); k += 15;
    sprintf(str, "spidar      pos.: %6.2f %6.2f %6.2f\n", pos_hap.x*pscale, pos_hap.y*pscale, pos_hap.z*pscale);
    renderCharacter(str, 5, cam.winY-k); k+= 15;
    sprintf(str, "volume: %6.2f    pitch: %6.2f\n", volume, dpos);
    renderCharacter(str, 5, cam.winY-k); k += 15;
    sprintf(str, "step: %6d    wave: %6.2f (max: %6.2f)  mode:%s\n", isim_step, wmotion[isim_step], max_motion
          , sim_mode == SIMULATION_REPEAT ? "repeat" : "once");
    renderCharacter(str, 5, cam.winY-k); k += 15;

    Trealf f, t;

    if(isim_step % 30 == 0) fmax = tmax = 0;
    f = force_abs[n];
    t = torq_abs[n];
    if(fmax < f) fmax = f;
    if(tmax < t) tmax = t;
    sprintf(str, "force: %6.2f %6.2f %6.2f %6.2f   torque: %6.2f %6.2f %6.2f %6.2f\n"
        , force_data[n].x, force_data[n].y, force_data[n].z
        , fmax
        , torq_data[n].x, torq_data[n].y, torq_data[n].z
        , tmax);
    renderCharacter(str, 5, cam.winY-k); k += 15;
    drawWave();
*/
    drawWave();
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_LIGHTING);
}

void timerDisplay(int val) {

    if(isound == SOUND_EFFECT_ON) {
        timer_sound(TRUE);
    }
//    if(boat.id_obj >= 0) genWave(&boat);
    if(use_spidar == FALSE) {
        updateWaterSurface(val);
        if(imode == PUSH_WATER && (idrag == TRUE || yflow > 0)) 
            updateHapticObj();
        Tpoint3f trq, frc;
        frc.x = frc.y = frc.z = 0.0;
        trq.x = trq.y = trq.z = 0.0;
        getForce2(&frc, &trq);
        force_data[kforce] = pobj.force = frc;
        torq_data[kforce] = trq;
        vel_data[kforce] = pobj.v;
        if(ve_max < vel_data[kforce]) ve_max = vel_data[kforce];
        Trealf d;
        d = force_data[kforce].x*force_data[kforce].x
           +force_data[kforce].y*force_data[kforce].y
           +force_data[kforce].z*force_data[kforce].z;
        if(d > 0.0) {
            d = sqrt(d);
            if(d > force_max && d < INFINITY_FORCE*0.99) force_max = d;
        }
        force_abs[kforce] = d;
        d = torq_data[kforce].x*torq_data[kforce].x
           +torq_data[kforce].y*torq_data[kforce].y
           +torq_data[kforce].z*torq_data[kforce].z;
        if(d > 0.0) {
            d = sqrt(d);
            if(d > torq_max && d < INFINITY_FORCE*0.99) torq_max = d;
        }
        torq_abs[kforce] = d;
        if(boat.id_obj >= 0) {
            Tpoint3f r, torq, force, buo, tbuo;
            buo.x = buo.y = buo.z = 0.0;
            tbuo.x = tbuo.y = tbuo.z = 0.0;
            getBuoyancy(&boat, &buo, &tbuo);

            force.x = buo.x+force_data[kforce].x;
            force.y = buo.y+force_data[kforce].y;
            force.z = buo.z+force_data[kforce].z-boat.mass;

            if(copy_oar == TRUE) {
                torq.x = torq.y = torq.z = 0.0;
                if(right_oar == TRUE) {
                    r.x = pobj.pos.x - boat.pos.x;
                    r.y = pobj.pos.y - boat.pos.y;
                    r.z = pobj.pos.z - boat.pos.z;
                    torq.x = r.y*force.z - r.z*force.y;
                    torq.y = r.z*force.x - r.x*force.z;
                    torq.z = r.x*force.y - r.y*force.x;
                }
                if(left_oar == TRUE) {
                    Trealf pos[3];
                    pos[0] = pobj.pos.x*cpy[0]+pobj.pos.y*cpy[4]+pobj.pos.z*cpy[8]+cpy[12];
                    pos[1] = pobj.pos.x*cpy[1]+pobj.pos.y*cpy[5]+pobj.pos.z*cpy[9]+cpy[13];
                    pos[2] = pobj.pos.x*cpy[2]+pobj.pos.y*cpy[6]+pobj.pos.z*cpy[10]+cpy[14];
                    r.x = pos[0] - boat.pos.x;
                    r.y = pos[1] - boat.pos.y;
                    r.z = pos[2] - boat.pos.z;
                    torq.x += r.y*force.z - r.z*force.y;
                    torq.y += r.z*force.x - r.x*force.z;
                    torq.z += r.x*force.y - r.y*force.x;
                }
            } else {
                r.x = pobj.pos.x - boat.pos.x;
                r.y = pobj.pos.y - boat.pos.y;
                r.z = pobj.pos.z - boat.pos.z;
                torq.x = r.y*force.z - r.z*force.y;
                torq.y = r.z*force.x - r.x*force.z;
                torq.z = r.x*force.y - r.y*force.x;
            }
            torq.x += tbuo.x;
            torq.y += tbuo.y;
            torq.z += tbuo.z;

            moveBoatHase(force, torq);
        }
        kforce++;
        hap_step++;
        if(kforce >= nforce_max) kforce = 0;
    }

    display();
    if(iwrt_bmp == TRUE || imode == MOVIE_WRITE) writeFrameBmp(isim_step);
    if(sim_mode == SIMULATION_REPEAT && imode != MOVIE_WRITE) {
        if(isim == TRUE) glutTimerFunc(TIMER_INTERVAL, timerDisplay, TRUE);
        isim_step++;
        if(isim_step == nmotion-1) isim_step = 0;
    } else if(sim_mode == SIMULATION_ONCE || imode == MOVIE_WRITE) {
//        if(isim_step < nmotion-1) {
        if(kforce < nforce_max - 1) {
            if(imode != MOVIE_WRITE) if(isim == TRUE) glutTimerFunc(TIMER_INTERVAL, timerDisplay, TRUE);
            isim_step++;
        } else {
            isim = FALSE; printf("isim %d  kforce %d %d %d\n", isim_step, kforce, hap_step, nforce_max);
            initSimulation();
            if(use_spidar == TRUE) timeKillEvent(timerID);
//            if(imode == MOVIE_WRITE) writeWave();
        }
    }
}

void dispObjects() {
    if(disp_obj == TRUE) {

        int i;
        
        for(i=0; i<nobj; i++) {
            if(obj[i].stencil == TRUE) {
                glEnable(GL_STENCIL_TEST);
                glStencilFunc(GL_ALWAYS, 1, ~0);
                glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
            }
            if(obj[i].type == DYNAMIC_OBJ) {
                glPushMatrix();
                glMultMatrixf(obj[i].m);
            }
            drawObject(i);
            if(obj[i].type == DYNAMIC_OBJ)
                glPopMatrix();
            if(obj[i].stencil == TRUE)
                glDisable(GL_STENCIL_TEST);
        }
        if(copy_oar == TRUE) {
            glPushMatrix();
            glMultMatrixf(cpy);
            drawObject(pobj.id_obj);
            glPopMatrix();
        }
    } else {
        int i;
        
        for(i=0; i<nobj; i++) {
            if(obj[i].type == DYNAMIC_OBJ) {
                glPushMatrix();
                glMultMatrixf(obj[i].m);
            }
            drawObjBox(i);
            if(obj[i].type == DYNAMIC_OBJ)
                glPopMatrix();
        }
    }
}

void display(void) {

    glutSetWindow(winID);
    
    if(hsmap == NULL) {
        genHSMap();
//        genPoints();
//        menu_operation(SIMULATION_START);
    }

//    if(nforce_tex <= 0) projectArea();
    if(rod.lure_view_display > 0.0) {
         lure_view = FALSE;
    }
    setCamera();
	glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_SCISSOR_TEST);
    glClearColor(0.7, 0.8, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	


    if(imode == MOVE_CAMERA && idrag == TRUE) drawAxis();

    glClearStencil(0x0);
    glClear(GL_STENCIL_BUFFER_BIT);

    if(rod.id_obj >= 0) {
        glColor3f(1., 1., 1.);
        glBegin(GL_LINES);
           glVertex3f(rod.pos.x, rod.pos.y, rod.pos.z);
           glVertex3f(rod.lure_cur.x, rod.lure_cur.y, rod.lure_cur.z);
//           glVertex3f(pobj.pos.x, pobj.pos.y, pobj.pos.z);
        glEnd();
    }
    
    dispObjects();
    drawHapticPoints();

    // DROP SHADOW 
/*
    Treald sm[16];
    sm[0] = 1.0; sm[1] = 0.0; sm[2] = 0.0; sm[3] = 0.0;
    sm[4] = 0.0; sm[5] = 1.0; sm[6] = 0.0; sm[7] = 0.0;
    sm[8] = -light_pos[0]/light_pos[2]; sm[9] = -light_pos[1]/light_pos[2]; sm[10] = 0.0; sm[11] = 0.0;
    sm[12] = -light_pos[0]/light_pos[2]*WATER_DEPTH; sm[13] = -light_pos[1]/light_pos[2]*WATER_DEPTH; sm[14] =  -WATER_DEPTH; sm[15] = 1.0;
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixd(sm);
    drawObject(pobj.id_obj);
    glPopMatrix();
    glEnable(GL_LIGHTING);
*/

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_EQUAL, 0, ~0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    drawWaterSurface();
    glDisable(GL_STENCIL_TEST);

        if(disp_hist == TRUE) {
            Tpoint3f tmp;
            int i;
            tmp = pobj.fpos;
            //glBlendFunc(GL_ONE, GL_ONE);
            glEnable(GL_BLEND);
            for(i=0; i<n_pobj_mat_hist; i+=60) {
                pobj.fpos.x = pobj_mat_hist[i][12];
                pobj.fpos.y = pobj_mat_hist[i][13];
                pobj.fpos.z = pobj_mat_hist[i][14];
                glPushMatrix();
                glMultMatrixf(pobj_mat_hist[i]);
                drawObject(pobj.id_obj);
                glPopMatrix();
                glDisable(GL_DEPTH_TEST);
                drawArrow((kforce - i) % nforce_max);
                glEnable(GL_DEPTH_TEST);
            }

            pobj.fpos = tmp;
            glDisable(GL_BLEND);
        }
//    drawPressure();
    if(disp_param == TRUE) drawParam();

    if(rod.lure_view_display > 0.0) {
        lure_view = TRUE;
        glViewport(0.0, 0.0, (GLfloat)cam.winX, (GLfloat)cam.winY);
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0, (GLfloat)cam.winX, 0.0, (GLfloat)cam.winY);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
//        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glColor3f(0.56, 0.64, 0.72);
        glColor3f(0.2, 0.3, 0.6);
        glBegin(GL_QUADS);
          glVertex2i(0, cam.winY*(1.0-rod.lure_view_display));
          glVertex2i(cam.winX*rod.lure_view_display, cam.winY*(1.0-rod.lure_view_display));
          glVertex2i(cam.winX*rod.lure_view_display, cam.winY);
          glVertex2i(0, cam.winY);
        glEnd();
        glLineWidth(3.0);
        glColor3f(0.0, 0.0, 0.0);
    	glLineWidth(5.0);
	    glBegin(GL_LINE_LOOP);
    //    glBegin(GL_QUADS);
          glVertex2i(0, cam.winY*(1.0-rod.lure_view_display));
          glVertex2i(cam.winX*rod.lure_view_display, cam.winY*(1.0-rod.lure_view_display));
          glVertex2i(cam.winX*rod.lure_view_display, cam.winY);
          glVertex2i(0, cam.winY);
        glEnd();
        glLineWidth(1.0);
        glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_LIGHTING);

        setCamera();

        if(imode == MOVE_CAMERA && idrag == TRUE) drawAxis();

        glClearStencil(0x0);
        glClear(GL_STENCIL_BUFFER_BIT);

        if(rod.id_obj >= 0) {
            glColor3f(1., 1., 1.);
            glBegin(GL_LINES);
               glVertex3f(rod.pos.x, rod.pos.y, rod.pos.z);
               glVertex3f(rod.lure_cur.x, rod.lure_cur.y, rod.lure_cur.z);
            glEnd();
        }
/*
        if(disp_obj == TRUE) {
            int i;
            i = pobj.id_obj;
            if(obj[i].stencil == TRUE) {
                glEnable(GL_STENCIL_TEST);
                glStencilFunc(GL_ALWAYS, 1, ~0);
                glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
            }
            if(obj[i].type == DYNAMIC_OBJ) {
                glPushMatrix();
                glMultMatrixf(obj[i].m);
            }
            drawObject(i);
            if(obj[i].type == DYNAMIC_OBJ)
                glPopMatrix();
            if(obj[i].stencil == TRUE)
                glDisable(GL_STENCIL_TEST);
        } else {
            int i;
            
            i = pobj.id_obj;
            if(obj[i].type == DYNAMIC_OBJ) {
                glPushMatrix();
                glMultMatrixf(obj[i].m);
            }
            drawObjBox(i);
            if(obj[i].type == DYNAMIC_OBJ)
                glPopMatrix();
        }
*/
    Trealf col[4];
    col[0] = 0.25; col[1] = 0.35; col[2] = 0.65; col[3] = 1.0;
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, col);
    glFogf(GL_FOG_DENSITY, (Trealf)0.02);
        glEnable(GL_FOG);
        dispObjects();
        glDisable(GL_FOG);
        drawHapticPoints();

        if(disp_param == TRUE) drawParam();
        glDisable(GL_STENCIL_TEST);
    }
    glutSwapBuffers();
}


void GLUTCALLBACK reshape(int width, int height){
	cam.winX= width;
	cam.winY = height;
}
void openWindow() {

    int sim_menu, op_menu, i;
    int wrt_menu, move_sub_menu, disp_menu, sound_menu;
    int main_menu, movie_menu;
    char buf[256];
    time_t tim, tloc;
    char *tm;

    for(i=0; i<nss; i++) sspos[i] = -1.0;

    emlimg.width = cam.winX;
    emlimg.height = cam.winY;
    emlimg.top = cam.winY-1;
    emlimg.bottom = 0;
    emlimg.left = 0;
    emlimg.right = cam.winX-1;
    emlimg.pix = (Pixel *)malloc(sizeof(Pixel)
                        *cam.winX*cam.winY);
    if(emlimg.pix == NULL){
        fprintf(stderr,"Can't malloc Pix.\n");
        exit(1);
    }
    sprintf(buf, "OpenGL Cloud Renderer by Y. Dobashi");
    strncpy(emlimg.comment, buf, COMMENTBYTE);
    tim = time(&tloc);
    tm = ctime(&tim);
    strncpy(emlimg.date, tm, DATEBYTE);

    initViewpoint();

//    glutInit(NULL, NULL);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(cam.winX, cam.winY);
    glutInitWindowPosition(0, 0);
    winID = glutCreateWindow("water surface");
    glLoadIdentity();
    cam.aspect = (GLdouble)cam.winX/(GLdouble)cam.winY;
    cam.vang[0] = cam.vang[1] = cam.fovy;
    gluPerspective(cam.fovy, cam.aspect, 0.1, 500);
    glutMouseFunc(buttonAction);
    glutMotionFunc(mouse_operation);
    glutDisplayFunc(display);
    glutKeyboardFunc(key_operation);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    sim_menu = glutCreateMenu(menu_operation);
    glutAddMenuEntry("start", SIMULATION_START);
    glutAddMenuEntry("stop", SIMULATION_STOP);
    glutAddMenuEntry("once/repeat", SIMULATION_REPEAT);

    wrt_menu = glutCreateMenu(menu_operation);
    glutAddMenuEntry("motion", WRT_WAVE_MOTION);
    glutAddMenuEntry("bmp", WRT_BMP);
    glutAddMenuEntry("wav", WRT_WAV);
    glutAddMenuEntry("data", WRT_DATA);

    op_menu = glutCreateMenu(menu_operation);
    glutAddMenuEntry("move camera", MOVE_CAMERA);
    glutAddMenuEntry("push water", PUSH_WATER);
    glutAddMenuEntry("drop teapot", DROP_TEAPOT);

    disp_menu = glutCreateMenu(menu_operation);
    glutAddMenuEntry("toggle obj", DISP_OBJECT);
    glutAddMenuEntry("toggle hsmap", DISP_HSMAP);
    glutAddMenuEntry("toggle point", DISP_POINTS);
    glutAddMenuEntry("toggle hap src", DISP_HAPTIC_SRC);
    glutAddMenuEntry("toggle wave vel", DISP_WAVE_VELOCITY);
    glutAddMenuEntry("toggle param", DISP_PARAM);
    glutAddMenuEntry("toggle fog", DISP_ADD_FOG);

    sound_menu = glutCreateMenu(menu_operation);
    glutAddMenuEntry("on", SOUND_EFFECT_ON);
    glutAddMenuEntry("off", SOUND_EFFECT_OFF);

    move_sub_menu = glutCreateMenu(menu_operation);
    glutAddMenuEntry("rotate", MOVE_ROTATE);
    glutAddMenuEntry("walk", MOVE_WALK);
    glutAddMenuEntry("pan", MOVE_PAN);
    glutAddMenuEntry("approach", MOVE_APPROACH);
    glutAddMenuEntry("reset", RESET_VIEW);
    glutAddMenuEntry("print", PRINT_CAMERA);

    movie_menu = glutCreateMenu(menu_operation);
    glutAddMenuEntry("replay", MOVIE_REPLAY);
    glutAddMenuEntry("write", MOVIE_WRITE);
    glutAddMenuEntry("write spidar", WRITE_SPIDAR);
    glutAddMenuEntry("read spidar", READ_SPIDAR);

    main_menu = glutCreateMenu(menu_operation);
    glutAddSubMenu("view", move_sub_menu);
    glutAddSubMenu("simulation", sim_menu);
    glutAddSubMenu("operation", op_menu);
    glutAddSubMenu("display", disp_menu);
    glutAddSubMenu("sound", sound_menu);
    glutAddSubMenu("write", wrt_menu);
    glutAddSubMenu("movie", movie_menu);
    glutAddMenuEntry("quit", QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
/*
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
*/
    for(i=0; i<nstex; i++) spos[i] = 0.0;

    texname = (GLuint *)malloc(sizeof(GLuint)*ntex);
    glGenTextures(ntex, texname);

    for(i=0; i<ntex+1; i++) {
        glBindTexture(GL_TEXTURE_2D, texname[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
        glTexImage2D(GL_TEXTURE_2D, 0, 4
                        ,teximg[i].width, teximg[i].height, 0
                        ,GL_RGB, GL_UNSIGNED_BYTE, teximg[i].pix);
    }
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_LIGHT0);

    Trealf col[4];
    col[0] = 0.55; col[1] = 0.55; col[2] = 0.65; col[3] = 1.0;
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, col);
    glFogf(GL_FOG_DENSITY, (Trealf)0.005);
    
    if(boat.id_obj >= 0) boat_view = TRUE;

	glutReshapeFunc(reshape);

    glutMainLoop();
}

void writeData() {
    int i;
    FILE *fp;

    if((fp = fopen("force.dat", "w")) == NULL) {
        printf("Can't open force.dat\n");
        return;
    }
    
    for(i=0; i<nforce_max; i++)
        fprintf(fp, "%f %f %f %f\n", vel_data[i], force_data[i].x, force_data[i].y, force_data[i].z);

    fclose(fp);
}

void writeSpidar() {
    int i, oar[2];
    FILE *fp;

    if((fp = fopen(spidar_file, "wb")) == NULL) {
        printf("Can't open spidar.dat\n");
        return;
    }
    fwrite(&nforce_max, sizeof(int), 1, fp);
    for(i=0; i<nforce_max; i++) {
        fwrite(&(spidar_pos_rot[i].pos), sizeof(Tpoint3f), 1, fp);
        fwrite(spidar_pos_rot[i].rot, sizeof(Trealf), 9, fp);
        oar[0] = spidar_pos_rot[i].left_oar;
        oar[1] = spidar_pos_rot[i].right_oar;
        fwrite(oar, sizeof(int), 2, fp);
    }
    fclose(fp);
}
void readSpidar() {
    int i, oar[2], n;
    char filename[80];
    FILE *fp;

//    printf("input filename: ");
//    scanf("%s", filename);
    strcpy(filename, spidar_file);
    if((fp = fopen(filename, "rb")) == NULL) {
        printf("Can't open spidar.dat\n");
        return;
    }
    
    fread(&n, sizeof(int), 1, fp);
    if(n != nforce_max) {
        printf("different spidar data. %d != %d\n", n, nforce_max);
    }
    for(i=0; i<nforce_max; i++) {
        fread(&(spidar_pos_rot[i].pos), sizeof(Tpoint3f), 1, fp);
        fread(spidar_pos_rot[i].rot, sizeof(Trealf), 9, fp);
        fread(oar, sizeof(int), 2, fp);
        spidar_pos_rot[i].left_oar = oar[0];
        spidar_pos_rot[i].right_oar = oar[1];
    }
    fclose(fp);
}

char numberStr[10] = {'0','1','2','3','4','5','6','7','8','9'};

void writeFrameBmp(int i) {

    int n, m, len;
    int j;
    Pixel *pix;
    char bmp_filename[80];
    unsigned char tmp;

    pix = emlimg.pix+emlimg.width*(emlimg.height-1);
    pix = emlimg.pix;
    for(j=0; j<emlimg.height; j++) {
        glReadPixels(0
                    ,j
                    ,emlimg.width
                    ,1
                    ,GL_RGB,GL_UNSIGNED_BYTE
                    ,pix);
         pix += emlimg.width;
    }
    for(j=0; j<emlimg.width*emlimg.height; j++) {
        tmp = emlimg.pix[j].r;
        emlimg.pix[j].r = emlimg.pix[j].b;
        emlimg.pix[j].b = tmp;
    }

    strcpy(emlimg.filename,"image/test");
    len = strlen(emlimg.filename);
    emlimg.filename[len] = '.'; len++;
    m = i;
    n = m / 1000; m  = m % 1000;
    emlimg.filename[len] = numberStr[n]; len++;
    n = m / 100; m  = m % 100;
    emlimg.filename[len] = numberStr[n]; len++;
    n = m / 10; m  = m % 10;
    emlimg.filename[len] = numberStr[n]; len++;
    n = m ;
    emlimg.filename[len] = numberStr[n]; len++;
//    emlimg.filename[len] = '0'; len++;
    emlimg.filename[len] = '.'; len++;
    emlimg.filename[len] = 'i'; len++;
    emlimg.filename[len] = 'm'; len++;
    emlimg.filename[len] = 'g'; len++;
    emlimg.filename[len] = '\0';

    strcpy(bmp_filename, emlimg.filename);
    bmp_filename[len-3] = 'b';
    bmp_filename[len-2] = 'm';
    bmp_filename[len-1] = 'p';
    bmp_filename[len] = '\0';
    writeBitmap(bmp_filename, emlimg.pix, emlimg.width, emlimg. height ,24);
}

void writeBitmap(char* filename, void* bits, int width, int height, int bpp)
{
	// 出力ファイルを開く
	FILE *fout;
	int unit, widthbytes, sizeimage, h;
	char* outbits;
	char* srcbits;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bmih;

	fout = fopen(filename, "wb");

	// サイズを計算
	unit = bpp >> 3; // ピクセル当りのバイト数
	widthbytes = ((unit * width) + 3) & ~3; // 4バイト境界に合わせる
	sizeimage = widthbytes * height;

	// ファイルヘッダを出力
	bf.bfType    = 0x4d42;
	bf.bfOffBits = sizeof(bf) + sizeof(BITMAPINFOHEADER);
	bf.bfSize    = bf.bfOffBits + sizeimage;
	bf.bfReserved1 = 0;
	bf.bfReserved2 = 0;
	fwrite((char*)&bf, sizeof(bf), 1, fout);

	// ビットマップヘッダを出力
	bmih.biSize          = sizeof(bmih); 
	bmih.biWidth         = width;
	bmih.biHeight        = height;
	bmih.biPlanes        = 1;
	bmih.biBitCount      = bpp;
	bmih.biCompression   = BI_RGB; 
	bmih.biSizeImage     = sizeimage;
	bmih.biXPelsPerMeter = 0; 
	bmih.biYPelsPerMeter = 0; 
	bmih.biClrUsed       = 0;  
	bmih.biClrImportant  = 0; 
	fwrite((char*)&bmih, sizeof(bmih), 1, fout);

	// データを出力
	outbits = (char *)malloc(sizeof(char)*widthbytes); // 1行分のメモリを確保
	srcbits = (char*)bits;
	for (h = 0; h < height; h++) {
		// 1行分のデータが4バイトで割り切れるようにしなければいけないので
		// こうする。
		memcpy(outbits, srcbits, unit * width);
		fwrite(outbits, sizeof(char), widthbytes, fout);
		srcbits += unit * width;
	}
	free(outbits);

	// ファイルを閉じる
	fclose(fout);
}

void drawObject(int i)
{
    Tface *fac;
    register int j;
//    GLfloat mat_diffuse[4], spec[4];
    
    if(obj[i].id_tex >= 0) {
        glBindTexture(GL_TEXTURE_2D, texname[obj[i].id_tex]);
        glEnable(GL_TEXTURE_2D);
/*
        Trealf p[4];
        p[0] = 0.001;
        p[1] = p[2] = p[3] = 0.0;
        glTexGenfv(GL_S, GL_OBJECT_PLANE, p);
        p[2] = 0.001;
        p[0] = p[1] = p[3] = 0.0;
        glTexGenfv(GL_T, GL_OBJECT_PLANE, p);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
*/
    }

//    for(i=0; i<nobj; i++) {
//        glPushMatrix();
//        if(obj[i].type == DYNAMIC_OBJ) glMultMatrixd(obj[i].m);
//        if(obj[i].type == DYNAMIC_OBJ) glTranslatef(pobj.pos.x, pobj.pos.y, pobj.pos.z);
        fac = obj[i].face;
/*
        glColor3f(obj[i].ref[0], obj[i].ref[1], obj[i].ref[2]);
        mat_diffuse[0] = obj[i].ref[0];
        mat_diffuse[1] = obj[i].ref[1];
        mat_diffuse[2] = obj[i].ref[2];
        mat_diffuse[3] = 1.;
        spec[0] = spec[1] = spec[2] = spec[3] = 0.0;
        if(rod.id_obj >= 0 && i == pobj.id_obj) spec[0] = spec[1] = spec[2] = spec[3] = 1.0;
        glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
*/
        if(obj[i].shin >= 0.0) {
            glEnable(GL_LIGHTING);
            glMaterialfv(GL_FRONT, GL_SPECULAR, obj[i].spec);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, obj[i].diffuse);
            glMaterialfv(GL_FRONT, GL_SHININESS, &obj[i].shin);
        } else {
            glDisable(GL_LIGHTING);
            glColor4f(obj[i].ref[0], obj[i].ref[1], obj[i].ref[2], 0.2);
        }
        for(j=0; j<obj[i].nf; j++) {
            if(fac->pv[0] != NULL) {
            glBegin(GL_POLYGON);
              glNormal3f(fac->pv[0]->normal[0], fac->pv[0]->normal[1], fac->pv[0]->normal[2]);
              if(obj[i].id_tex >= 0) glTexCoord2f(fac->pv[0]->p[0]*obj[i].tex_scl, fac->pv[0]->p[1]*obj[i].tex_scl);
              glVertex3f(fac->pv[0]->p[0], fac->pv[0]->p[1], fac->pv[0]->p[2]);

              glNormal3f(fac->pv[1]->normal[0], fac->pv[1]->normal[1], fac->pv[1]->normal[2]);
              if(obj[i].id_tex >= 0) glTexCoord2f(fac->pv[1]->p[0]*obj[i].tex_scl, fac->pv[1]->p[1]*obj[i].tex_scl);
              glVertex3f(fac->pv[1]->p[0], fac->pv[1]->p[1], fac->pv[1]->p[2]);

              glNormal3f(fac->pv[2]->normal[0], fac->pv[2]->normal[1], fac->pv[2]->normal[2]);
              if(obj[i].id_tex >= 0) glTexCoord2f(fac->pv[2]->p[0]*obj[i].tex_scl, fac->pv[2]->p[1]*obj[i].tex_scl);
              glVertex3f(fac->pv[2]->p[0], fac->pv[2]->p[1], fac->pv[2]->p[2]);
              if(fac->pv[3] != NULL) {
                  glNormal3f(fac->pv[3]->normal[0], fac->pv[3]->normal[1], fac->pv[3]->normal[2]);
                  if(obj[i].id_tex >= 0) glTexCoord2f(fac->pv[3]->p[0]*obj[i].tex_scl, fac->pv[3]->p[1]*obj[i].tex_scl);
                  glVertex3f(fac->pv[3]->p[0], fac->pv[3]->p[1], fac->pv[3]->p[2]);
              }
            glEnd();
            }
            fac++;
        }
//        glPopMatrix();
//    }

    glDisable(GL_LIGHTING);
    if(obj[i].id_tex >= 0) {
        glDisable(GL_TEXTURE_2D);
/*
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
*/
    }
}

void drawObjBox(int i)
{

//    glDisable(GL_LIGHTING);
    glColor3f(obj[i].ref[0], obj[i].ref[1], obj[i].ref[2]);
    glLineWidth(2.0);
    
    glPushMatrix();
    glTranslatef((obj[i].box[1][0] + obj[i].box[0][0])/2.
                ,(obj[i].box[1][1] + obj[i].box[0][1])/2.
                ,(obj[i].box[1][2] + obj[i].box[0][2])/2.);
    glScalef(obj[i].box[1][0] - obj[i].box[0][0]
            ,obj[i].box[1][1] - obj[i].box[0][1]
            ,obj[i].box[1][2] - obj[i].box[0][2]);
    glutWireCube(1.); 

    glPopMatrix();
    glLineWidth(1.0);
//    glEnable(GL_LIGHTING);
}


void makeMatrix(Treald vx, Treald vy, Treald vz
               ,Treald rx, Treald ry, Treald rz
               ,Treald roll_ang, Treald rot[3][3]) {
    Treald dd,dxy;
    Treald xx,yy,zz;
    Treald dx2, dy2, dz2;
    Treald xdir, ydir, zdir;
    Treald ct, st;
    Treald xo[3], yo[3];

    xdir = vx-rx;
    ydir = vy-ry;
    zdir = vz-rz;

    dx2 = xdir*xdir;
    dy2 = ydir*ydir;
    dz2 = zdir*zdir;
    dxy = dx2+dy2;
    if(dxy < 0.000001) {
         rot[0][0] = 1.0;
         rot[0][1] = 0.0;
         rot[0][2] = 0.0;
         rot[1][0] = 0.0;
         rot[1][1] = 1.0;
         rot[1][2] = 0.0;
         rot[2][0] = 0.0;
         rot[2][1] = 0.0;
         rot[2][2] = 1.0;
    } else {
        dd = dxy+dz2;
        dd = (Treald)sqrt(dd);
        rot[2][0] = xdir/dd;
        rot[2][1] = ydir/dd;
        rot[2][2] = zdir/dd;
        dxy = (Treald)sqrt(dxy);
        rot[0][0] = -ydir/dxy;
        rot[0][1] = xdir/dxy;
        rot[0][2] = 0.0;
        xx = -xdir*zdir;
        yy = -ydir*zdir;;
        zz = dx2+dy2;
        dd = (Treald)sqrt(xx*xx+yy*yy+zz*zz);
        rot[1][0] = xx/dd;
        rot[1][1] = yy/dd;
        rot[1][2] = zz/dd;
    }

   /*** ROLL ***/
    ct = cos(roll_ang);
    st = sin(roll_ang);
    xo[0] = rot[0][0]; xo[1] = rot[0][1]; xo[2]=rot[0][2];
    yo[0] = rot[1][0]; yo[1] = rot[1][1]; yo[2]=rot[1][2];
    rot[0][0] = xo[0]*ct+yo[0]*st;
    rot[0][1] = xo[1]*ct+yo[1]*st;
    rot[0][2] = xo[2]*ct+yo[2]*st;
    rot[1][0] = -xo[0]*st+yo[0]*ct;
    rot[1][1] = -xo[1]*st+yo[1]*ct;
    rot[1][2] = -xo[2]*st+yo[2]*ct;
}


void lerpForceOld(Tpoint3f *p, Trealf t, Tpoint3f *vp, int itex, Tpoint3f *pres, Tpoint3f *fric) {

    int n;
    Tpoint3f ff, uu, vv, ww;
    Trealf a;

    vv.x = -vp->x; vv.y = -vp->y; vv.z = -vp->z;
    uu.x = vv.y; uu.y = -vv.x; uu.z = 0.0;
    a = sqrt(uu.x*uu.x + uu.y*uu.y);
    uu.x /= a; uu.y /=a;
    ww.x = uu.y*vv.z;
    ww.y = -uu.x*vv.z;
    ww.z = uu.x*vv.y-uu.y*vv.x;
    a = sqrt(ww.x*ww.x + ww.y*ww.y + ww.z*ww.z);
    ww.x /= a; ww.y /= a; ww.z /= a;

    n = t*force_tex[itex].rate;
    n = n % force_tex[itex].n;
/*
    uu.x *= 6;
    uu.y *= 6;
    uu.z *= 6;
    ww.x *= 6;
    ww.y *= 6;
    ww.z *= 6;
*/
    ff.x = force_tex[itex].xprs[n];// + force_tex[itex].xfri[n];
    ff.y = force_tex[itex].yprs[n];// + force_tex[itex].yfri[n];
    ff.z = force_tex[itex].zprs[n];// + force_tex[itex].zfri[n];

    pres->x = ff.x*uu.x + ff.y*vv.x + ff.z*ww.x;
    pres->y = ff.x*uu.y + ff.y*vv.y + ff.z*ww.y;
    pres->z = ff.x*uu.z + ff.y*vv.z + ff.z*ww.z;

    ff.x = force_tex[itex].xfri[n];
    ff.y = force_tex[itex].yfri[n];
    ff.z = force_tex[itex].zfri[n];

    fric->x = -ff.x*uu.x - ff.y*vv.x - ff.z*ww.x;
    fric->y = -ff.x*uu.y - ff.y*vv.y - ff.z*ww.y;
    fric->z = -ff.x*uu.z - ff.y*vv.z - ff.z*ww.z;
}

Tpoint3f* getFriction(Tpoint3f *n, Tpoint3f *vp) {

    static Tpoint3f u, v, f;
    Trealf co, d;

    f.x = f.y = f.z = 0.0;
    // v = n X pobj.vel;
    v.x = n->y*vp->z-n->z*vp->y;
    v.y = n->z*vp->x-n->x*vp->z;
    v.z = n->x*vp->y-n->y*vp->x;
    // u = v X n;
    u.x = v.y*n->z-v.z*n->y;
    u.y = v.z*n->x-v.x*n->z;
    u.z = v.x*n->y-v.y*n->x;
    d = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);

    if(d > 0.000001) {
        u.x /= d; u.y /= d; u.z /= d;
        co = u.x*vp->x+u.y*vp->y+u.z*vp->z;
        f.x += co*u.x;
        f.y += co*u.y;
        f.z += co*u.z;
   }
   
   return(&f);
}

void getForceOld(Tpoint3f *force, Tpoint3f *torq) {

    Tpoint3f buo, fric, pres, r, wall;
    Tpoint3f tbuo, tfric, tpres;
    Trealf wz, s;
    int i, touch_walls;
//    Tpoint3f *f, n, b, wve;
//    Trealf x, y ,z, dz;
//    TsamplePoint *sp;
    
    touch_walls = isTouchWall(&pobj, &wall);
//wall.x = wall.y = wall.z = 0.;

    buo.x = buo.y = buo.z = 0.0;
    tbuo.x = tbuo.y = tbuo.z = 0.0;
    if(touch_walls == FALSE) getBuoyancy(&pobj, &buo, &tbuo);

    fric.x = fric.y = fric.z = 0.0;
    tfric.x = tfric.y = tfric.z = 0.0;
    s = 0.0;
/*
    if(touch_walls == FALSE) {
        sp = pobj.sp;
        for(i=0; i<pobj.n_sp; i++) {
            x = sp[i].p[0];
            y = sp[i].p[1];
            z = sp[i].p[2];
            wz = lerpWaterHeight(x, y);
            r.x = sp[i].p[0] - pobj.pos.x;
            r.y = sp[i].p[1] - pobj.pos.y;
            r.z = sp[i].p[2] - pobj.pos.z;
            n.x = sp[i].n[0];
            n.y = sp[i].n[1];
            n.z = sp[i].n[2];
//            wz = 0.0;
            if(wz > z) {
                wve = lerpWaterVelocity(x, y);
                s += sp[i].s;
                b.x = -n.x*(wz-z)*sp[i].s;
                b.y = -n.y*(wz-z)*sp[i].s;
                b.z = -n.z*(wz-z)*sp[i].s;
                if(sim_type == 1)
                    dz = 1.*exp(-0.02*(wz-z))*dh;
                else if(sim_type == 2)
                    dz = 0.2*exp(-0.02*(wz-z))*dh;
//                dz = 0.0;
                buo.x += b.x + wve.x*dz;
                buo.y += b.y + wve.y*dz;
                buo.z += b.z + wve.z*dz;
                tbuo.x += r.y*b.z - r.z*b.y;
                tbuo.y += r.z*b.x - r.x*b.z;
                tbuo.z += r.x*b.y - r.y*b.x;
                if(nforce_tex <= 0) {
                    f = getFriction(&n, &pobj.vel);
                    f->x *= sp[i].s;
                    f->y *= sp[i].s;
                    f->z *= sp[i].s;
                    fric.x -= f->x;
                    fric.y -= f->y;
                    fric.z -= f->z;
                    tfric.x += r.y*f->z - r.z*f->y;
                    tfric.y += r.z*f->x - r.x*f->z;
                    tfric.z += r.x*f->y - r.y*f->x;
                }
            }
        }
    }
*/
    if(nforce_tex <= 0) {
        // APPROXIMATED PRESSURE RESISTANCE
        pres.x = pres.y = pres.z = 0.0;
        tpres.x = tpres.y = tpres.z = 0.0;
        if(pobj.v > 0.1) {
            s = pobj.p_area*s/pobj.area;
            pres.x += s*pobj.vel.x/pobj.v;
            pres.y += s*pobj.vel.y/pobj.v;
            pres.z += s*pobj.vel.z/pobj.v;
        }
        fric.x *= VISCOSITY;
        fric.y *= VISCOSITY;
        fric.z *= VISCOSITY;
        tfric.x *= VISCOSITY;
        tfric.y *= VISCOSITY;
        tfric.z *= VISCOSITY;
    } else {
        Tpoint3f p, vel, vd, prs, fri, pr, fr;
        Tpoint3f tprs, tfri;
        Tpoint3f vo;
        Trealf c, c2, w, ws, co, vs;
        int j, itex;
        ThapticSource *hp;

        pres.x = pres.y = pres.z = 0.0;
        fric.x = fric.y = fric.z = 0.0;
        tpres.x = tpres.y = tpres.z = 0.0;
        tfric.x = tfric.y = tfric.z = 0.0;
        s = 1;
        s /= pobj.nhsrc;
        hp = pobj.hsrc;
        for(i=0; i<pobj.nhsrc; i++) {
            if(hp->v > 0.1) {
//                wz = lerpWaterHeight(hp->x, hp->y);
                wz = 0.0;
                if(wz > hp->z) {
                    p.x = hp->x;
                    p.y = hp->y;
                    p.z = hp->z;
                    vo.x = hp->vx;
                    vo.y = hp->vy;
                    vo.z = hp->vz;
                    vs  = hp->v;
/*
                wve = lerpWaterVelocity(x, y);
                wve.x *= 50.;
                wve.y *= 50.;
                wve.z *= 50.;
                dz = wve.x*wve.x+wve.y*wve.y;
                if(dz > 0.0) dz = sqrt(dz);
                vo.x = -wve.x;
                vo.y = -wve.y;
                vo.z = -wve.z;
                vs = sqrt(vo.x*vo.x+vo.y*vo.y+vo.z*vo.z);
if(vs > 0.0) {
*/
                    vo.x /= vs;
                    vo.y /= vs;
                    vo.z /= vs;
                    vd.x = vel.x = vo.x;
                    vd.y = vel.y = vo.y;
                    vd.z = vel.z = vo.z;
                    r.x = hp->x - pobj.pos.x;
                    r.y = hp->y - pobj.pos.y;
                    r.z = hp->z - pobj.pos.z;
                    prs.x = prs.y = prs.z = 0.0;
                    fri.x = fri.y = fri.z = 0.0;
                    tprs.x = tprs.y = tprs.z = 0.0;
                    tfri.x = tfri.y = tfri.z = 0.0;
                    ws = 0.0;
                    if(hp->sym.x == TRUE) vd.x = ABS(vel.x);
                    if(hp->sym.y == TRUE) vd.y = ABS(vel.y);
                    if(hp->sym.z == TRUE) vd.z = ABS(vel.z);
                    for(j=0; j<hp->ntex; j++) {
                        itex = hp->itex[j];
                        lerpForceOld(&p, hp->tcoord, &vel, itex, &pr, &fr);
                        co = vd.x*force_tex[itex].dir.x
                            +vd.y*force_tex[itex].dir.y
                            +vd.z*force_tex[itex].dir.z;
                        co -= 0.8;
                        if(co < 0.0) 
                            w = 0.0;
                        else
                            w = co*co*co;
                        ws += w;
                        prs.x += w*pr.x;
                        prs.y += w*pr.y;
                        prs.z += w*pr.z;
                        fri.x += w*fr.x;
                        fri.y += w*fr.y;
                        fri.z += w*fr.z;
                        tprs.x += w*(r.y*pr.z - r.z*pr.y);
                        tprs.y += w*(r.z*pr.x - r.x*pr.z);
                        tprs.z += w*(r.x*pr.y - r.y*pr.x);
                        tfri.x += w*(r.y*fr.z - r.z*fr.y);
                        tfri.y += w*(r.z*fr.x - r.x*fr.z);
                        tfri.z += w*(r.x*fr.y - r.y*fr.x);
                    }
                    if(ws > 0.0) {
                        prs.x /= ws;
                        prs.y /= ws;
                        prs.z /= ws;
                        fri.x /= ws;
                        fri.y /= ws;
                        fri.z /= ws;
                        tprs.x /= ws;
                        tprs.y /= ws;
                        tprs.z /= ws;
                        tfri.x /= ws;
                        tfri.y /= ws;
                        tfri.z /= ws;
                       
                        c = vs/force_tex[hp->itex[0]].v; // ←基準速度は全て同じ？
                        c2 = c*c;
                        hp->tcoord += c*pobj.vel_scl/(Trealf)force_frq;
                        prs.x *= c2;
                        prs.y *= c2;
                        prs.z *= c2;
                        fri.x *= c;
                        fri.y *= c;
                        fri.z *= c;
                        pres.x += prs.x*s;
                        pres.y += prs.y*s;
                        pres.z += prs.z*s;
                        fric.x += fri.x*s;
                        fric.y += fri.y*s;
                        fric.z += fri.z*s;

                        tprs.x *= c2;
                        tprs.y *= c2;
                        tprs.z *= c2;
                        tfri.x *= c;
                        tfri.y *= c;
                        tfri.z *= c;
                        tpres.x += tprs.x*s;
                        tpres.y += tprs.y*s;
                        tpres.z += tprs.z*s;
                        tfric.x += tfri.x*s;
                        tfric.y += tfri.y*s;
                        tfric.z += tfri.z*s;
                    }
                }  else {
                    hp->tcoord = 0.0;
                }
            } else {
                hp->tcoord = 0.0;
            }
            hp++;
        }
    }

    buo.x *= pobj.buo_scl;
    buo.y *= pobj.buo_scl;
    buo.z *= pobj.buo_scl;

    buo.z -= pobj.mass;
    
    pres.x *= pobj.pres_scl;
    pres.y *= pobj.pres_scl;
    pres.z *= pobj.pres_scl;

    fric.x *= pobj.fric_scl;
    fric.y *= pobj.fric_scl;
    fric.z *= pobj.fric_scl;

    tbuo.x *= pobj.buo_scl;
    tbuo.y *= pobj.buo_scl;
    tbuo.z *= pobj.buo_scl;
    
    tpres.x *= pobj.pres_scl;
    tpres.y *= pobj.pres_scl;
    tpres.z *= pobj.pres_scl;

    tfric.x *= pobj.fric_scl;
    tfric.y *= pobj.fric_scl;
    tfric.z *= pobj.fric_scl;

//buo.x = buo.y = buo.z = 0.0;
//fric.x = fric.y = fric.z = 0.0;
//pres.x = pres.y = pres.z = 0.0;
    force->x = buo.x + fric.x + pres.x + wall.x;
    force->y = buo.y + fric.y + pres.y + wall.y;
    force->z = buo.z + fric.z + pres.z + wall.z;
   
//    force->x = force->y = force->z = 0.;

    torq->x = tbuo.x + tfric.x + tpres.x;
    torq->y = tbuo.y + tfric.y + tpres.y;
    torq->z = tbuo.z + tfric.z + tpres.z;

//    torq->x = torq->y = torq->z = 0.;
}

void getForceTex(Tpoint3f peri_prs, Tpoint3f peri_fri, Tpoint3f phas_prs, Tpoint3f phas_fri, Trealf to, TfluidForceTex *fftex, Tpoint3f *prs, Tpoint3f *fri) {
    
    prs->x = prs->y = prs->z = 0.0;
    fri->x = fri->y = fri->z = 0.0;
    
    if(fftex->n >0) {
        Trealf s, t;

        t = to-phas_prs.x;
        s = t - (int)(t/peri_prs.x)*peri_prs.x;
        s = s/peri_prs.x*fftex->peri_prs.x;
        if(s < 0.0) s = fftex->peri_prs.x;
        prs->x = fftex->xprs[(int)(s*fftex->rate) % fftex->n];

        t = to-phas_prs.y;
        s = t - (int)(t/peri_prs.y)*peri_prs.y;
        s = s/peri_prs.y*fftex->peri_prs.y;
        if(s < 0.0) s = fftex->peri_prs.y;
        prs->y = fftex->yprs[(int)(s*fftex->rate) % fftex->n];

        t = to-phas_prs.z;
        s = t - (int)(t/peri_prs.z)*peri_prs.z;
        s = s/peri_prs.z*fftex->peri_prs.z;
        if(s < 0.0) s = fftex->peri_prs.z;
        prs->z = fftex->zprs[(int)(s*fftex->rate) % fftex->n];

        t = to-phas_fri.x;
        s = t - (int)(t/peri_fri.x)*peri_fri.x;
        s = s/peri_fri.x*fftex->peri_fri.x;
        if(s < 0.0) s = fftex->peri_fri.x;
        fri->x = fftex->xfri[(int)(s*fftex->rate) % fftex->n];

        t = to-phas_fri.y;
        s = t - (int)(t/peri_fri.y)*peri_fri.y;
        s = s/peri_fri.y*fftex->peri_fri.y;
        if(s < 0.0) s = fftex->peri_fri.y;
        fri->y = fftex->yfri[(int)(s*fftex->rate) % fftex->n];

        t = to-phas_fri.z;
        s = t - (int)(t/peri_fri.z)*peri_fri.z;
        s = s/peri_fri.z*fftex->peri_fri.z;
        if(s < 0.0) s = fftex->peri_fri.z;
        fri->z = fftex->zfri[(int)(s*fftex->rate) % fftex->n];
    }
}

void lerpForce2Old(Trealf the, Trealf phi, Trealf t, Tpoint3f *vp, TforceSet *fset, Tpoint3f *pres, Tpoint3f *fric) {

    int ithe, iphi, k00, k01, k10, k11;
    Tpoint3f prs0, prs1, prs, fri0, fri1, fri, uu, vv, ww;
    Trealf ft, fp, fthe, fphi;
    Tpoint3f peri_prs, peri_fri, phas_prs, phas_fri;
    Trealf   a, b, c, d;
    TfluidForceTex *frc;

    vv.x = -vp->x; vv.y = -vp->y; vv.z = -vp->z;
    uu.x = vv.y; uu.y = -vv.x; uu.z = 0.0;
    a = sqrt(uu.x*uu.x + uu.y*uu.y);
    uu.x /= a; uu.y /=a;
    ww.x = uu.y*vv.z;
    ww.y = -uu.x*vv.z;
    ww.z = uu.x*vv.y-uu.y*vv.x;
    a = sqrt(ww.x*ww.x + ww.y*ww.y + ww.z*ww.z);
    ww.x /= a; ww.y /= a; ww.z /= a;
/*
    uuu = uu;
    vvv = vv;
    www = ww;
*/
/*
    uu.x *= 2;
    uu.y *= 2;
    uu.z *= 2;
    ww.x *= 2;
    ww.y *= 2;
    ww.z *= 2;
*/
//vv.x = vv.y = vv.z = 0.;
vv.x *= -1;
vv.y *= -1;
vv.z *= -1;
    fthe = the/fset->dthe;
    fphi = phi/fset->dphi;

    ithe = (int)fthe;
    iphi = (int)fphi;
    ft = fthe - ithe;
    fp = fphi - iphi;

    if(ithe > 0) {
        k00 = (ithe-1)*fset->nphi+1+iphi;
        if(iphi < fset->nphi-1)
            k01 = k00+1;
        else
            k01 = k00 - iphi;
        k10 = k00+fset->nphi;
        k11 = k10+1;
        if(k10 >= fset->ntex) k10 = fset->ntex-1;
        if(k11 >= fset->ntex) k11 = fset->ntex-1;
    } else {
        k00 = 0;
        k01 = 0;
        k10 = iphi+1;
        if(iphi < fset->nphi-1)
            k11 = k10+1;
        else
            k11 = 1;
    }
//k01 = k11 = k10 = k00;

    Tpoint3f prs00, prs01, prs10, prs11, fri00, fri01, fri10, fri11;

    frc = fset->frc;
/*
    printf("%f %f, %f %f\n", the/M_PI*180., phi/M_PI*180., ft, fp);
    printf("k00 %d: %f %f\n", k00, frc[k00].the, frc[k00].phi);
    printf("k01 %d: %f %f\n", k01, frc[k01].the, frc[k01].phi);
    printf("k10 %d: %f %f\n", k10, frc[k10].the, frc[k10].phi);
    printf("k11 %d: %f %f\n\n", k11, frc[k11].the, frc[k11].phi);
*/

    a = (1.0-ft)*(1.0-fp);
    b = ft*(1.0-fp);
    c = (1.0-ft)*fp;
    d = ft*fp;
    peri_prs.x = a*frc[k00].peri_prs.x+b*frc[k10].peri_prs.x+c*frc[k01].peri_prs.x+d*frc[k11].peri_prs.x;
    peri_prs.y = a*frc[k00].peri_prs.y+b*frc[k10].peri_prs.y+c*frc[k01].peri_prs.y+d*frc[k11].peri_prs.y;
    peri_prs.z = a*frc[k00].peri_prs.z+b*frc[k10].peri_prs.z+c*frc[k01].peri_prs.z+d*frc[k11].peri_prs.z;
    peri_fri.x = a*frc[k00].peri_fri.x+b*frc[k10].peri_fri.x+c*frc[k01].peri_fri.x+d*frc[k11].peri_fri.x;
    peri_fri.y = a*frc[k00].peri_fri.y+b*frc[k10].peri_fri.y+c*frc[k01].peri_fri.y+d*frc[k11].peri_fri.y;
    peri_fri.z = a*frc[k00].peri_fri.z+b*frc[k10].peri_fri.z+c*frc[k01].peri_fri.z+d*frc[k11].peri_fri.z;

    phas_prs.x = a*frc[k00].phas_prs.x+b*frc[k10].phas_prs.x+c*frc[k01].phas_prs.x+d*frc[k11].phas_prs.x;
    phas_prs.y = a*frc[k00].phas_prs.y+b*frc[k10].phas_prs.y+c*frc[k01].phas_prs.y+d*frc[k11].phas_prs.y;
    phas_prs.z = a*frc[k00].phas_prs.z+b*frc[k10].phas_prs.z+c*frc[k01].phas_prs.z+d*frc[k11].phas_prs.z;
    phas_fri.x = a*frc[k00].phas_fri.x+b*frc[k10].phas_fri.x+c*frc[k01].phas_fri.x+d*frc[k11].phas_fri.x;
    phas_fri.y = a*frc[k00].phas_fri.y+b*frc[k10].phas_fri.y+c*frc[k01].phas_fri.y+d*frc[k11].phas_fri.y;
    phas_fri.z = a*frc[k00].phas_fri.z+b*frc[k10].phas_fri.z+c*frc[k01].phas_fri.z+d*frc[k11].phas_fri.z;

    getForceTex(peri_prs, peri_fri, phas_prs, phas_fri, t, &(frc[k00]),&prs00, &fri00);
    getForceTex(peri_prs, peri_fri, phas_prs, phas_fri, t, &(frc[k01]),&prs01, &fri01);
    getForceTex(peri_prs, peri_fri, phas_prs, phas_fri, t, &(frc[k10]),&prs10, &fri10);
    getForceTex(peri_prs, peri_fri, phas_prs, phas_fri, t, &(frc[k11]),&prs11, &fri11);

    prs0.x = prs00.x*(1.0-ft)+prs10.x*ft;
    prs0.y = prs00.y*(1.0-ft)+prs10.y*ft;
    prs0.z = prs00.z*(1.0-ft)+prs10.z*ft;
    prs1.x = prs01.x*(1.0-ft)+prs11.x*ft;
    prs1.y = prs01.y*(1.0-ft)+prs11.y*ft;
    prs1.z = prs01.z*(1.0-ft)+prs11.z*ft;
    prs.x = prs0.x*(1.0-fp)+prs1.x*fp;
    prs.y = prs0.y*(1.0-fp)+prs1.y*fp;
    prs.z = prs0.z*(1.0-fp)+prs1.z*fp;
    
    pres->x = prs.x*uu.x + prs.y*vv.x + prs.z*ww.x;
    pres->y = prs.x*uu.y + prs.y*vv.y + prs.z*ww.y;
    pres->z = prs.x*uu.z + prs.y*vv.z + prs.z*ww.z;
    
//fprintf(sysout, "%f %f %f %f %f %f", the, phi, ft, fp, prs.x);

    fri0.x = fri00.x*(1.0-ft)+fri10.x*ft;
    fri0.y = fri00.y*(1.0-ft)+fri10.y*ft;
    fri0.z = fri00.z*(1.0-ft)+fri10.z*ft;
    fri1.x = fri01.x*(1.0-ft)+fri11.x*ft;
    fri1.y = fri01.y*(1.0-ft)+fri11.y*ft;
    fri1.z = fri01.z*(1.0-ft)+fri11.z*ft;
    fri.x = fri0.x*(1.0-fp)+fri1.x*fp;
    fri.y = fri0.y*(1.0-fp)+fri1.y*fp;
    fri.z = fri0.z*(1.0-fp)+fri1.z*fp;

    fric->x = -fri.x*uu.x - fri.y*vv.x - fri.z*ww.x;
    fric->y = -fri.x*uu.y - fri.y*vv.y - fri.z*ww.y;
    fric->z = -fri.x*uu.z - fri.y*vv.z - fri.z*ww.z;
    
}


void moveBoatOld(Tpoint3f force, Tpoint3f torq) {

    Trealf dthe, the, ct, st, rot[3][3];
//    Tpoint3f r, torq, buo, tbuo, force;
/*
    buo.x = buo.y = buo.z = 0.0;
    tbuo.x = tbuo.y = tbuo.z = 0.0;
    getBuoyancy(&boat, &buo, &tbuo);
buo.x = buo.y = 0.0;    
tbuo.x = tbuo.y = tbuo.z = 0.0;

    buo.x *= boat.buo_scl;
    buo.y *= boat.buo_scl;
    buo.z *= boat.buo_scl;
    tbuo.x *= boat.buo_scl;
    tbuo.y *= boat.buo_scl;
    tbuo.z *= boat.buo_scl;

    force.x = eforce.x+buo.x;
    force.y = eforce.y+buo.y;
    force.z = buo.z-boat.mass;
force.z = 0.0;
*/
    boat.a.x = force.x/boat.mass;
    boat.a.y = force.y/boat.mass;
    boat.a.z = force.z/boat.mass;
//    boat.a.z = 0.0;

    boat.vel.x += boat.a.x/(Trealf)force_frq;
    boat.vel.y += boat.a.y/(Trealf)force_frq;
    boat.vel.z += boat.a.z/(Trealf)force_frq;

    Tpoint3f wall;
    if(isTouchWall(&boat, &wall) == TRUE) {
        if(wall.x > 0.0) { 
            boat.vel.x = -boat.vel.x;//*boat.loss;
        }
        if(wall.x < 0.0) {
            boat.vel.x = -boat.vel.x;//*boat.loss;
        }
        if(wall.y > 0.0) {
            boat.vel.y = -boat.vel.y;//*boat.loss;
        }
        if(wall.y < 0.0) {
            boat.vel.y = -boat.vel.y;//*boat.loss;
        }
    }

    boat.pos.x += boat.vel.x/(Trealf)force_frq;
    boat.pos.y += boat.vel.y/(Trealf)force_frq;
    boat.pos.z += boat.vel.z/(Trealf)force_frq;

boat.pos.z = -0.5;

    boat.vel.x *= boat.loss;
    boat.vel.y *= boat.loss;
    boat.vel.z *= boat.loss;
    
    boat.v = sqrt(boat.vel.x*boat.vel.x+boat.vel.y*boat.vel.y+boat.vel.z*boat.vel.z)*0.004;

    Trealf *m, u[3], v[3], w[3];

    m=obj[boat.id_obj].m;
/*
    r.x = pos.x - boat.pos.x;
    r.y = pos.y - boat.pos.y;
    r.z = pos.z - boat.pos.z;

    torq.x = r.y*force.z - r.z*force.y;
    torq.y = r.z*force.x - r.x*force.z;
    torq.z = r.x*force.y - r.y*force.x;

    torq.x += tbuo.x;
    torq.y += tbuo.y;
    torq.z += tbuo.z;

    torq.x = torq.y = 0.0;
*/

    dthe = torq.z/boat.im.x/(Trealf)force_frq;
    boat.we.z -= dthe/(Trealf)force_frq;
    the = boat.we.z;
    ct = cos(the);
    st = sin(the);
    boat.we.z *= boat.loss;
/*
    rot[0][0] = torq.x*torq.x*(1.0-ct)+ct;
    rot[0][1] = torq.x*torq.y*(1.0-ct)-torq.z*st;
    rot[0][2] = torq.z*torq.x*(1.0-ct)+torq.y*st;
    rot[1][0] = torq.x*torq.y*(1.0-ct)+torq.z*st;
    rot[1][1] = torq.y*torq.y*(1.0-ct)+ct;
    rot[1][2] = torq.y*torq.z*(1.0-ct)+torq.x*st;
    rot[2][0] = torq.z*torq.x*(1.0-ct)-torq.y*st;
    rot[2][1] = torq.y*torq.z*(1.0-ct)+torq.x*st;
    rot[2][2] = torq.z*torq.z*(1.0-ct)+ct;

    u[0] = rot[0][0]*m[0]+rot[0][1]*m[4]+rot[0][2]*m[8];
    u[1] = rot[0][0]*m[1]+rot[0][1]*m[5]+rot[0][2]*m[9];
    u[2] = rot[0][0]*m[2]+rot[0][1]*m[6]+rot[0][2]*m[10];
    v[0] = rot[1][0]*m[0]+rot[1][1]*m[4]+rot[1][2]*m[8];
    v[1] = rot[1][0]*m[1]+rot[1][1]*m[5]+rot[1][2]*m[9];
    v[2] = rot[1][0]*m[2]+rot[1][1]*m[6]+rot[1][2]*m[10];
    w[0] = rot[2][0]*m[0]+rot[2][1]*m[4]+rot[2][2]*m[8];
    w[1] = rot[2][0]*m[1]+rot[2][1]*m[5]+rot[2][2]*m[9];
    w[2] = rot[2][0]*m[2]+rot[2][1]*m[6]+rot[2][2]*m[10];
*/
    rot[0][0] = ct;
    rot[0][1] = -st;
    rot[0][2] = 0.0;
    rot[1][0] = st;
    rot[1][1] = ct;
    rot[1][2] = 0.0;
    rot[2][0] = 0.0;
    rot[2][1] = 0.0;
    rot[2][2] = 1.0;

    u[0] = rot[0][0]*m[0]+rot[0][1]*m[4]+rot[0][2]*m[8];
    u[1] = rot[0][0]*m[1]+rot[0][1]*m[5]+rot[0][2]*m[9];
    u[2] = rot[0][0]*m[2]+rot[0][1]*m[6]+rot[0][2]*m[10];
    v[0] = rot[1][0]*m[0]+rot[1][1]*m[4]+rot[1][2]*m[8];
    v[1] = rot[1][0]*m[1]+rot[1][1]*m[5]+rot[1][2]*m[9];
    v[2] = rot[1][0]*m[2]+rot[1][1]*m[6]+rot[1][2]*m[10];
    w[0] = rot[2][0]*m[0]+rot[2][1]*m[4]+rot[2][2]*m[8];
    w[1] = rot[2][0]*m[1]+rot[2][1]*m[5]+rot[2][2]*m[9];
    w[2] = rot[2][0]*m[2]+rot[2][1]*m[6]+rot[2][2]*m[10];

    m[0] = u[0];
    m[1] = u[1];
    m[2] = u[2];
    m[3] = 0.0;

    m[4] = v[0];
    m[5] = v[1];
    m[6] = v[2];
    m[7] = 0.0;

    m[8] = w[0];
    m[9] = w[1];
    m[10] = w[2];
    m[11] = 0.0;

    m[12] = boat.pos.x;
    m[13] = boat.pos.y;
    m[14] = boat.pos.z;
    m[15] = 1.0;
   TsamplePoint *sp;
   int i;
   sp = boat.sp;
   for(i=0; i<boat.n_sp; i++) {
        sp[i].p[0] = m[0]*sp[i].p0[0]+m[4]*sp[i].p0[1]+m[8]*sp[i].p0[2]+m[12];
        sp[i].p[1] = m[1]*sp[i].p0[0]+m[5]*sp[i].p0[1]+m[9]*sp[i].p0[2]+m[13];
        sp[i].p[2] = m[2]*sp[i].p0[0]+m[6]*sp[i].p0[1]+m[10]*sp[i].p0[2]+m[14];
        
        sp[i].n[0] = m[0]*sp[i].n0[0]+m[4]*sp[i].n0[1]+m[8]*sp[i].n0[2];
        sp[i].n[1] = m[1]*sp[i].n0[0]+m[5]*sp[i].n0[1]+m[9]*sp[i].n0[2];
        sp[i].n[2] = m[2]*sp[i].n0[0]+m[6]*sp[i].n0[1]+m[10]*sp[i].n0[2];
   }
}
void moveBoatHaseOld(Tpoint3f force, Tpoint3f torq) {

    Trealf dt;
    
    dt = 1.0/force_frq;

    // 行列・ベクトル・クォータニオンライブラリを使うため，キャストして参照型に．
    Vec3f& pos = (Vec3f&)boat.pos;
    Quaternionf& ori = (Quaternionf&)boat.ori;
    Vec3f& vel = (Vec3f&)boat.vel;
    Vec3f& we = (Vec3f&)boat.we;
    Vec3f& a = (Vec3f&)boat.a;
    Vec3f& aa = (Vec3f&)boat.aa;

    // 加速度を求める
    a = (Vec3f&)force / boat.mass;

    // 角加速度を求める．
    // まず慣性テンソルを求める．
    Matrix3f& inertia = (Matrix3f&)boat.I; // 列が蜜に並んだ行列だと思っています．
    Matrix3f rot;

    Trealf *m, brot[3][3];
    m=obj[boat.id_obj].m;
    brot[0][0] = m[0]; brot[0][1] = m[1]; brot[0][2] = m[2];
    brot[1][0] = m[4]; brot[1][1] = m[5]; brot[1][2] = m[6];
    brot[2][0] = m[8]; brot[2][1] = m[9]; brot[2][2] = m[10];
    rot = (Matrix3f&)brot;

    ori.to_matrix(rot); // 3x3行列に変換

    // (_w:ワールド系, _l:ローカル系) として，
    //   物体姿勢の回転行列を R その逆行列を Ri とすると 
     //  N_w = I_w * omega_w; // 運動方程式を
     //  R*N_l = I_w * R*omega_l; // と書き換えられるので，
     //  N_l= Ri*I_w*R * omega_l; // とかける．
     //  つまり，
     //  I_l = Ri*I_w*R; なので，
    //   R*I_l*Ri = Iw
    //
    Matrix3f inertia_w = rot * inertia * rot.trans();
    // 重心周りのトルクに変換
    // tq_w = pos % force. 重心周りに変換するには， pos を (pos-cog) にすればよい．(%は 外積です)
    //tq_g = (pos-cog) % force
     //    = tq_w - cog % force
    //
//    Vec3f tq_g = (Vec3f&)torq - (pos%(Vec3f&)force); 
    Vec3f tq_g = (Vec3f&)torq; 
    // 慣性テンソルから加速度を求める．
    (Vec3f&)boat.aa = inertia_w.inv() * tq_g;
    
    pos += vel*dt + 0.5f*a*dt*dt;
    vel += a*dt;
    Vec3f d_ori =  we*dt + 0.5f*aa*dt*dt;
    ori = Quaternionf::Rot(d_ori)*ori;
    we += aa*dt;
    
    vel.x *= boat.loss;
    vel.y *= boat.loss;
//    vel.z *= boat.loss;
//
//    | cosz   -sinz     0.0| | cosy     0.0   -siny| |  1.0     0.0     0.0|
//    | sinz    cosz     0.0| |  0.0     1.0     0.0| |  0.0    cosx   -sinx}
//    |  0.0     0.0     1.0| | siny     0.0    cosy| |  0.0    sinx    cosx|
//
//    | cosz   -sinz     0.0| |cosy -sinx*siny -cosx*siny|
//    | sinz    cosz     0.0| |0.0     cosx       -sinx  |
//    |  0.0     0.0     1.0| |siny  sinx*cosy  cosx*cosy|
//    
//    |cosy*cosz   -sinx*siny*cosz-cosx*sinz -cosx*siny*cosz+sinx*sinz|
//    |cosy*sinz   -sinx*siny*sinz+cosx*cosz -cosx*siny*sinz-sinx*cosz|
//    |siny         sinx*cosy                 cosx*cosy               |


    Trealf sx, sy, sz, cx, cy, cz, u[3], v[3], w[3];

    sx = sin(-boat.we.x); cx = cos(-boat.we.x);
    sy = sin(-boat.we.y); cy = cos(-boat.we.y);
    sz = sin(-boat.we.z); cz = cos(-boat.we.z);
    boat.we.x *= boat.loss;
    boat.we.y *= boat.loss;
    boat.we.z *= boat.loss;
    brot[0][0] = cy*cz; brot[0][1] = -sx*sy*cz-cx*sz; brot[0][2] = -cx*sy*cz+sx*sz;
    brot[1][0] = cy*sz; brot[1][1] = -sx*sy*sz+cx*cz; brot[1][2] = -cx*sy*sz-sx*cz;
    brot[2][0] = sy; brot[2][1] = sx*cy; brot[2][2] = cx*cy;

    u[0] = brot[0][0]*m[0]+brot[0][1]*m[4]+brot[0][2]*m[8];
    u[1] = brot[0][0]*m[1]+brot[0][1]*m[5]+brot[0][2]*m[9];
    u[2] = brot[0][0]*m[2]+brot[0][1]*m[6]+brot[0][2]*m[10];
    v[0] = brot[1][0]*m[0]+brot[1][1]*m[4]+brot[1][2]*m[8];
    v[1] = brot[1][0]*m[1]+brot[1][1]*m[5]+brot[1][2]*m[9];
    v[2] = brot[1][0]*m[2]+brot[1][1]*m[6]+brot[1][2]*m[10];
    w[0] = brot[2][0]*m[0]+brot[2][1]*m[4]+brot[2][2]*m[8];
    w[1] = brot[2][0]*m[1]+brot[2][1]*m[5]+brot[2][2]*m[9];
    w[2] = brot[2][0]*m[2]+brot[2][1]*m[6]+brot[2][2]*m[10];

    m[0] = u[0]; m[1] = u[1]; m[2] = u[2]; m[3] = 0.0;
    m[4] = v[0]; m[5] = v[1]; m[6] = v[2]; m[7] = 0.0;
    m[8] = w[0]; m[9] = w[1]; m[10] = w[2]; m[11] = 0.0;

    m[12] = boat.pos.x;
    m[13] = boat.pos.y;
    m[14] = boat.pos.z;
    m[15] = 1.0;
    
   TsamplePoint *sp;
   int i;
   sp = boat.sp;
   for(i=0; i<boat.n_sp; i++) {
        sp[i].p[0] = m[0]*sp[i].p0[0]+m[4]*sp[i].p0[1]+m[8]*sp[i].p0[2]+m[12];
        sp[i].p[1] = m[1]*sp[i].p0[0]+m[5]*sp[i].p0[1]+m[9]*sp[i].p0[2]+m[13];
        sp[i].p[2] = m[2]*sp[i].p0[0]+m[6]*sp[i].p0[1]+m[10]*sp[i].p0[2]+m[14];
        
        sp[i].n[0] = m[0]*sp[i].n0[0]+m[4]*sp[i].n0[1]+m[8]*sp[i].n0[2];
        sp[i].n[1] = m[1]*sp[i].n0[0]+m[5]*sp[i].n0[1]+m[9]*sp[i].n0[2];
        sp[i].n[2] = m[2]*sp[i].n0[0]+m[6]*sp[i].n0[1]+m[10]*sp[i].n0[2];
   }
    boat.v = boat.vel.x*boat.vel.x+boat.vel.y*boat.vel.y+boat.vel.z*boat.vel.z;
    if(boat.v > 0.0) boat.v = sqrt(boat.v);
}


void moveBoat(Tpoint3f force, Tpoint3f torq) {

    Trealf dthe, the, ct, st, rot[3][3];

    boat.a.x = force.x/boat.mass;
    boat.a.y = force.y/boat.mass;
    boat.a.z = force.z/boat.mass;

    boat.vel.x += boat.a.x/(Trealf)force_frq;
    boat.vel.y += boat.a.y/(Trealf)force_frq;
    boat.vel.z += boat.a.z/(Trealf)force_frq;

    Tpoint3f wall;
    if(isTouchWall(&boat, &wall) == TRUE) {
        if(wall.x > 0.0) { 
            boat.vel.x = -boat.vel.x;
        }
        if(wall.x < 0.0) {
            boat.vel.x = -boat.vel.x;
        }
        if(wall.y > 0.0) {
            boat.vel.y = -boat.vel.y;
        }
        if(wall.y < 0.0) {
            boat.vel.y = -boat.vel.y;
        }
    }

    boat.pos.x += boat.vel.x/(Trealf)force_frq;
    boat.pos.y += boat.vel.y/(Trealf)force_frq;
    boat.pos.z += boat.vel.z/(Trealf)force_frq;

boat.pos.z = -0.5;

    boat.vel.x *= boat.loss;
    boat.vel.y *= boat.loss;
    boat.vel.z *= boat.loss;
    
    boat.v = sqrt(boat.vel.x*boat.vel.x+boat.vel.y*boat.vel.y+boat.vel.z*boat.vel.z)*0.004;

    Trealf *m, u[3], v[3], w[3];

    m=obj[boat.id_obj].m;

    dthe = torq.z/boat.im.x/(Trealf)force_frq;
    boat.we.z -= dthe/(Trealf)force_frq;
    the = boat.we.z;
    ct = cos(the);
    st = sin(the);
    boat.we.z *= boat.loss;

    rot[0][0] = ct;
    rot[0][1] = -st;
    rot[0][2] = 0.0;
    rot[1][0] = st;
    rot[1][1] = ct;
    rot[1][2] = 0.0;
    rot[2][0] = 0.0;
    rot[2][1] = 0.0;
    rot[2][2] = 1.0;

    u[0] = rot[0][0]*m[0]+rot[0][1]*m[4]+rot[0][2]*m[8];
    u[1] = rot[0][0]*m[1]+rot[0][1]*m[5]+rot[0][2]*m[9];
    u[2] = rot[0][0]*m[2]+rot[0][1]*m[6]+rot[0][2]*m[10];
    v[0] = rot[1][0]*m[0]+rot[1][1]*m[4]+rot[1][2]*m[8];
    v[1] = rot[1][0]*m[1]+rot[1][1]*m[5]+rot[1][2]*m[9];
    v[2] = rot[1][0]*m[2]+rot[1][1]*m[6]+rot[1][2]*m[10];
    w[0] = rot[2][0]*m[0]+rot[2][1]*m[4]+rot[2][2]*m[8];
    w[1] = rot[2][0]*m[1]+rot[2][1]*m[5]+rot[2][2]*m[9];
    w[2] = rot[2][0]*m[2]+rot[2][1]*m[6]+rot[2][2]*m[10];

    m[0] = u[0];
    m[1] = u[1];
    m[2] = u[2];
    m[3] = 0.0;

    m[4] = v[0];
    m[5] = v[1];
    m[6] = v[2];
    m[7] = 0.0;

    m[8] = w[0];
    m[9] = w[1];
    m[10] = w[2];
    m[11] = 0.0;

    m[12] = boat.pos.x;
    m[13] = boat.pos.y;
    m[14] = boat.pos.z;
    m[15] = 1.0;

   TsamplePoint *sp;
   int i;
   sp = boat.sp;
   for(i=0; i<boat.n_sp; i++) {
        sp[i].p[0] = m[0]*sp[i].p0[0]+m[4]*sp[i].p0[1]+m[8]*sp[i].p0[2]+m[12];
        sp[i].p[1] = m[1]*sp[i].p0[0]+m[5]*sp[i].p0[1]+m[9]*sp[i].p0[2]+m[13];
        sp[i].p[2] = m[2]*sp[i].p0[0]+m[6]*sp[i].p0[1]+m[10]*sp[i].p0[2]+m[14];
        
        sp[i].n[0] = m[0]*sp[i].n0[0]+m[4]*sp[i].n0[1]+m[8]*sp[i].n0[2];
        sp[i].n[1] = m[1]*sp[i].n0[0]+m[5]*sp[i].n0[1]+m[9]*sp[i].n0[2];
        sp[i].n[2] = m[2]*sp[i].n0[0]+m[6]*sp[i].n0[1]+m[10]*sp[i].n0[2];
   }
}
