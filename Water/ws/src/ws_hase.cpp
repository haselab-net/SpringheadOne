#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <GL/glut.h>
#include <io.h>

#define GLOBAL_MAIN

#include "ws.h"
#include "ogl.h"
#include "emlimg.h"
#include "sp/spAudio.h"
#include "sp/spWave.h"
#include "SpidarInterface.h"

#define WCC       4.

/*==== GLOBAL VARIABLES ====*/
GLOBAL Treald TIME_STEP = 0.1;
GLOBAL int    TIMER_INTERVAL = 1;
GLOBAL int    MX = 40, MY = 40;
GLOBAL time_t        comp_time;
GLOBAL Treald        *height, dt, dh, xo, yo, loss, visc, gravity, depth, rho_w;
GLOBAL Treald        *u, *v, *pw;
GLOBAL Tpoint3d      *normal;
GLOBAL Tpoint3d      *tvec;
GLOBAL Tcolor3f      *color;
GLOBAL Trealf        rnd_wave = 0., yflow = 0.0;

GLOBAL Trealf        *wmotion, max_motion;
GLOBAL int           nmotion;
GLOBAL int           isim_step = 0;

GLOBAL GLfloat       light_pos[4];
GLOBAL GLfloat       water_spec[4];
GLOBAL GLfloat       water_diffuse[4];
GLOBAL GLfloat       water_shin;

GLOBAL int          idebug = 0;
GLOBAL int          sim_type = 2;
GLOBAL FILE *sysout;

GLOBAL Tcamera cam;
GLOBAL Tear     ear;
GLOBAL EMLImg emlimg;
GLOBAL EMLImg *teximg;
GLOBAL GLuint    *texname;
GLOBAL int     ntex;
GLOBAL int     env_map, box_map, ground_map;

// FOR HAPTIC USING SPIDAR
GLOBAL Trealf   *vel_data, ve_max;
GLOBAL Tpoint3f *force_data, *torq_data;
GLOBAL Tpoint3f  pos_hap;
GLOBAL int    force_frq;
GLOBAL int    nforce_max;
GLOBAL Trealf pscale, force_max, torq_max;
GLOBAL int    use_spidar = FALSE;
GLOBAL ThapticObj pobj;

GLOBAL TfluidForceTex *force_tex;
GLOBAL int   nforce_tex = 0;
GLOBAL Trealf dv_force;
GLOBAL int force_calc_interval = 10;
GLOBAL int no_rotation = FALSE;

GLOBAL ThapticObj boat;
GLOBAL int        copy_oar = FALSE;

GLOBAL ThapticObj rod; // used for fishing

GLOBAL int       nfrc_set = 0;
GLOBAL TforceSet *frc_set;

GLOBAL Tzspan *hsmap = NULL;
GLOBAL Trealf  dx_hsmap, dy_hsmap;
GLOBAL int     n_zspan;
GLOBAL Trealf  unit_mass_water = 1.0;


GLOBAL Tpoint2i *mouse_pos[2];
GLOBAL int sim_mode = SIMULATION_REPEAT;

GLOBAL TspidarStatus *spidar_pos_rot;

GLOBAL Trealf *wav;
GLOBAL int    nwav;
GLOBAL Tsound sound;
GLOBAL Tsound *stex;
GLOBAL int   nstex;
GLOBAL Treald  *spos, dpos;
GLOBAL char *pcm_data = NULL;
GLOBAL char* pcm8 = NULL;
GLOBAL short* pcm16 =NULL ;
GLOBAL spAudio audio;

/*==== OBJECT ====*/
GLOBAL Tobject *obj;
GLOBAL int     nobj, nvtx;
GLOBAL Treald  objBox[2][3];

/* === LOCAL STATIC VARIABLES === */
int    npcm = 0;
PCMWAVEFORMAT pcmh;
DWORD dsize;

const char *ID_RIFF = "RIFF";
const char *ID_FMT = "fmt ";
const char *ID_DATA = "data";
const char *TYPE_WAVE = "WAVE";

Treald *wh1, *wh2; // water height at previous steps
Treald *u1, *v1;
Treald wcoeff;
Tpoint3d vlight;

/*== FUNCTIONS ==*/
void initWater();
void initSound();
void simulate();
void shadeWaterSurface();
void readBmpPixel(EMLImg *img, FILE *fp);
void readBmp(EMLImg *img, char *filename);
Tpoint3d calRefracVec(Tpoint3d n, Tpoint3d v, Treald ra);

void calPressure(ThapticObj *po);

void writeWaveMotion();

int initSpAudio(int frq, int bit, int nsmp, int istereo);
int readPcm(char *filename, PCMWAVEFORMAT *pcmh, DWORD *dsize, char **data);
void setupSoundSample(char *filename, Tsound *sound);
int writeWave();

int allocate_data(void);
void free_data(void);

Trealf lerpWaterHeight(Trealf x, Trealf y);
Tpoint3f lerpWaterVelocity(Trealf x, Trealf y);

extern void readCommand(char *filename);
extern void openWindow();
extern void display(void);

char wfile[256];
char *pcm_wav = NULL;
char* pcm8_wav = NULL;
short* pcm16_wav =NULL ;


static Treald *tmp = NULL;


//#define NX MX-2 //計算格子(x方向)
//#define NY MY-2 //計算格子(y方向)


//初期条件
//void initial(Treald *height,Treald *wh1,Treald *u,Treald *u1,Treald *v,Treald *v1);
//波高・速度計算
void solver(Treald *height,Treald *wh1,Treald *u,Treald *u1,Treald *v,Treald *v1, Treald *p);
//境界条件
void bound(Treald *f,int flag);



void main(int argc, char* argv[]) {
    int i, j, itex;
    char filename[256];
    Trealf rec_time;

    sysout = fopen("sysout.dat", "w");

    strcpy(wfile, "out.wav");
    sound.frq = 44100;
    sound.bit = 16;
    sound.stereo = TRUE;
    ear.dis = 0.1;
    ear.amb = 0.2;

    cam.winX = 640;
    cam.winY = 640;

    rec_time = 30;

    use_spidar = FALSE;

    nstex = -1;
    pscale = 1.0;

    itex = FALSE;
    MX = MY = 40;
    dt = 0.13;
    dh = 2.0;
    gravity=9.8; //重力加速度
    rho_w = 1000.; //水の密度
    depth=3.; //水底までの深さ
    xo = -(MX-1)/2.*dh;
    yo = -(MY-1)/2.*dh;
    loss = 0.9;
    visc = 1.0;

    i=1;
    while(i<argc) {
        if(!strcmp(argv[i], "-frq")) {
            sscanf(argv[i+1], "%d", &sound.frq);
            i++;
        } else if(!strcmp(argv[i], "-wav")) {
            sscanf(argv[i+1], "%d", &nstex);
            i++;
            stex = (Tsound *)malloc(sizeof(stex)*nstex);
            spos = (Treald *)malloc(sizeof(Treald)*nstex);
            dpos = 1.0;
            for(j=0; j<nstex; j++) {
                strcpy(filename, argv[i+1]);
                setupSoundSample(filename, &(stex[j]));
                i++;
            }
        } else if(!strcmp(argv[i], "-grid")) {
            sscanf(argv[i+1], "%d", &MX);
            i++;
            sscanf(argv[i+1], "%d", &MY);
            i++;
        } else if(!strcmp(argv[i], "-loss")) {
            sscanf(argv[i+1], "%lf", &loss);
            i++;
        } else if(!strcmp(argv[i], "-depth")) {
            sscanf(argv[i+1], "%lf", &depth);
            i++;
        } else if(!strcmp(argv[i], "-visc")) {
            sscanf(argv[i+1], "%lf", &visc);
            i++;
        } else if(!strcmp(argv[i], "-dt")) {
            sscanf(argv[i+1], "%lf", &dt);
            i++;
        } else if(!strcmp(argv[i], "-dh")) {
            sscanf(argv[i+1], "%lf", &dh);
            i++;
        } else if(!strcmp(argv[i], "-win")) {
            sscanf(argv[i+1], "%d", &cam.winX);
            i++;
            sscanf(argv[i+1], "%d", &cam.winY);
            i++;
        } else if(!strcmp(argv[i], "-bit")) {
            sscanf(argv[i+1], "%d", &sound.bit);
            i++;
        } else if(!strcmp(argv[i], "-no_stereo")) {
            printf("mono is not supported!\n");
//            istereo = 0;
        } else if(!strcmp(argv[i], "-rec_time")) {
            sscanf(argv[i+1], "%f", &rec_time);
            i++;
        } else if(!strcmp(argv[i], "-out")) {
            strcpy(wfile, argv[i+1]);
            i++;
        } else if(!strcmp(argv[i], "-spidar")) {
            use_spidar = TRUE;
            sscanf(argv[i+1], "%f", &pscale);
            i++;
        } else if(!strcmp(argv[i], "-sim1")) {
            sim_type = 1;
        } else if(!strcmp(argv[i], "-no_rotation")) {
            no_rotation = TRUE;
        } else if(!strcmp(argv[i], "-copy_oar")) {
            copy_oar = TRUE;
        } else if(!strcmp(argv[i], "-force_calc_interval")) {
            sscanf(argv[i+1], "%d", &force_calc_interval);
            i++;
        } else if(!strcmp(argv[i], "-repeat")) {
            sim_mode = SIMULATION_REPEAT;
        } else if(!strcmp(argv[i], "-once")) {
            sim_mode = SIMULATION_ONCE;
        } else if(!strcmp(argv[i], "-f")) {
            sscanf(argv[i+1], "%s", filename);
            readCommand(filename);
        }
        i++;
    }
    
    printf("time step: %f,   resolution: %f,   grids: %dx%d\n", dt, dh, MX, MY);

    nmotion = rec_time * 30;
    wmotion = (Trealf *)malloc(sizeof(Trealf)*(nmotion+1));
    allocate_data();
    initWater();
//    initial(height,wh1,u,u1,v,v1);
//    initSound();

    if(use_spidar == TRUE) {
        torq_max = force_max = 1.0;
        ve_max = 1.0;
        SPInit();
        force_frq = 500;
        nforce_max = force_frq*rec_time;
        force_data = (Tpoint3f *)malloc(sizeof(Tpoint3f)*nforce_max);
        torq_data = (Tpoint3f *)malloc(sizeof(Tpoint3f)*nforce_max);
        spidar_pos_rot = (TspidarStatus *)malloc(sizeof(TspidarStatus)*nforce_max);
    } else {
        torq_max = force_max = 1.0;
        ve_max = -1;
        force_frq = 30;
        nforce_max = nmotion;
        force_data = (Tpoint3f *)malloc(sizeof(Tpoint3f)*nforce_max);
        torq_data = (Tpoint3f *)malloc(sizeof(Tpoint3f)*nforce_max);
    }
    vel_data = (Trealf *)malloc(sizeof(Trealf)*nforce_max);
    mouse_pos[0] = (Tpoint2i *)malloc(sizeof(Tpoint2i)*nmotion);
    mouse_pos[1] = (Tpoint2i *)malloc(sizeof(Tpoint2i)*nmotion);
    
    printf("main finished\n"); fflush(stdout);

    openWindow();
}

void free_data ( void )
{
    if ( height ) free ( height );
    if ( u ) free ( u );
    if ( v ) free ( v );
    if ( u1 ) free ( u1 );
    if ( v1 ) free ( v1 );
    if ( wh1 ) free ( wh1 );
    if ( wh2 ) free ( wh2 );
    if ( tmp ) free ( tmp );
    if ( normal ) free ( normal );
    if ( tvec ) free ( tvec );
    if ( color ) free ( color );
}

int allocate_data ( void )
{
    int size = MX*MY;
    height    = (Treald *) malloc ( size*sizeof(Treald) );
    u         = (Treald *) malloc ( size*sizeof(Treald) );
    v         = (Treald *) malloc ( size*sizeof(Treald) );
    u1        = (Treald *) malloc ( size*sizeof(Treald) );
    v1        = (Treald *) malloc ( size*sizeof(Treald) );
    pw        = (Treald *) malloc ( size*sizeof(Treald) );
    wh1        = (Treald *) malloc ( size*sizeof(Treald) );
    wh2        = (Treald *) malloc ( size*sizeof(Treald) );
    tmp        = (Treald *) malloc ( size*sizeof(Treald) );
    normal     = (Tpoint3d *) malloc ( size*sizeof(Tpoint3d) );
    tvec       = (Tpoint3d *) malloc ( size*sizeof(Tpoint3d) );
    color      = (Tcolor3f *) malloc ( size*sizeof(Tcolor3f) );

    if ( !height || !u || !v || !u1 || !v1 || !wh1 || !wh2 || !tmp || !normal || !tvec || !tvec || !color || !pw) {
        fprintf ( stderr, "cannot allocate data\n" );
        return ( 0 );
    }

    return ( 1 );
}

void initWater() {
    int i, j;

    water_spec[0] = 1.0;
    water_spec[1] = 1.0;
    water_spec[2] = 1.0;
    water_spec[3] = 1.0;
    water_diffuse[0] = 0.8;
    water_diffuse[1] = 0.8;
    water_diffuse[2] = 0.8;
    water_diffuse[3] = 0.5;
    water_shin = 10.0;

    xo = -(MX-1)/2.*dh;
    yo = -(MY-1)/2.*dh;
    wcoeff = (dt*dt)/(dh*dh)*WCC*WCC;

    for (j = 0; j < MY; j++)
      for (i = 0; i < MX; i++) {
        height[IX(i,j)] = 0.0;
        normal[IX(i,j)].x = 0.0;
        normal[IX(i,j)].y = 0.0;
        normal[IX(i,j)].z = 1.0;
        wh1[IX(i,j)] = wh2[IX(i,j)] = height[IX(i,j)];
        tvec[IX(i,j)] = calRefracVec(normal[IX(i,j)], vlight, 1.3333);
        u[IX(i,j)] = 
        v[IX(i,j)] = 
        u1[IX(i,j)] = 
        v1[IX(i,j)] =
        pw[IX(i,j)] = 0.0;
      }

    vlight.x = light_pos[0] = 0./sqrt(1.);
    vlight.y = light_pos[1] = 0./sqrt(1.);
    vlight.z = light_pos[2] = 1./sqrt(1.);
    light_pos[3] = 0;

    for(i=0; i<nmotion; i++) wmotion[i] = 0.0;
//      shadeWaterSurface();
}

void initSound() {
    int i;
    DWORD dsize;

    sound.nsmp = (int)(TIME_STEP*sound.frq);
    sound.ndata = sound.nsmp*(sound.stereo == TRUE ? 2 : 1);
    sound.wave = (Trealf *)malloc(sizeof(Trealf)*sound.ndata);
    if(sound.bit == 8)
        dsize = sound.ndata;    // データサイズ
    else if(sound.bit == 16)
        dsize = sound.ndata*2;    // データサイズ
    pcm_data = (char *)realloc(pcm_data, sizeof(char)*dsize);
    pcm8 = pcm_data;
    pcm16 = (short *)pcm_data;
    initSpAudio(sound.frq, sound.bit, sound.nsmp, sound.stereo);

    nwav = nmotion/FRAME_RATE*sound.frq*(sound.stereo == TRUE ? 2 : 1);
    wav = (Trealf *)malloc(sizeof(Trealf)*nwav);
    for(i=0; i<nwav; i++) wav[i] = 0.0;
    if(sound.bit == 8)
        dsize = nwav;    // データサイズ
    else if(sound.bit == 16)
        dsize = nwav*2;    // データサイズ
    pcm_wav = (char *)malloc(sizeof(char)*dsize);
    pcm8_wav = pcm_wav;
    pcm16_wav = (short *)pcm_wav;
}

Trealf lerpWaterHeight(Trealf x, Trealf y) {

    if(x < -(MX-1)/2.*dh || x > (MX-1)/2.*dh ||
       y < -(MY-1)/2.*dh || y > (MY-1)/2.*dh) return 0.0;
    
    int ix0, ix1, iy0, iy1;
    Trealf xh, yh;
    
    xh = x/dh;
    yh = y/dh;

    ix0 = xh + MX/2.0;
    ix1 = ix0+1;
    iy0 = yh + MY/2.0;
    iy1 = iy0+1;
    
    if(ix0 < 0 || ix1 >= MX ||
       iy0 < 0 || iy1 >= MY) return 0.0;
    
    Trealf dx, dy, z0, z1, z;
    
    dx = xh - (int)(xh);
    dy = yh - (int)(yh);
    
    z0 = height[IX(ix0,iy0)]*(1.0-dy)+height[IX(ix0,iy1)]*dy;
    z1 = height[IX(ix1,iy0)]*(1.0-dy)+height[IX(ix1,iy1)]*dy;

    z = z0*(1.0-dx)+z1*dx;
    
    return(z);

}

Tpoint3f lerpWaterVelocity(Trealf x, Trealf y) {

    Tpoint3f vel;
    
    vel.x = vel.y = vel.z = 0;

    if(x < -(MX-1)/2. || x > (MX-1)/2. ||
       y < -(MY-1)/2. || y > (MY-1)/2.) return vel;
    
    int ix0, ix1, iy0, iy1;
    Trealf xh, yh;
    
    xh = x/dh;
    yh = y/dh;

    ix0 = xh + MX/2.0;
    ix1 = ix0+1;
    iy0 = yh + MY/2.0;
    iy1 = iy0+1;
    
    if(ix0 < 0 || ix1 >= MX ||
       iy0 < 0 || iy1 >= MY) return vel;
    
    Trealf vx0, vx1, vy0, vy1;
    Trealf dx, dy;
    
    dx = xh - (int)xh;
    dy = yh - (int)yh;

    vx0 = u[IX(ix0,iy0)]*(1.0-dy)+u[IX(ix0,iy1)]*dy;
    vx1 = u[IX(ix1,iy0)]*(1.0-dy)+u[IX(ix1,iy1)]*dy;
    vy0 = v[IX(ix0,iy0)]*(1.0-dy)+v[IX(ix0,iy1)]*dy;
    vy1 = v[IX(ix1,iy0)]*(1.0-dy)+v[IX(ix1,iy1)]*dy;
    

    vel.x = vx0*(1.0-dx)+vx1*dx;
    vel.y = vy0*(1.0-dx)+vy1*dx;
    vel.z = 0.0;
    
    return(vel);

}

#define RRR 1
void calPressure(ThapticObj *po) {
    int i, j, k, iwx, iwy;
    Trealf p, r, cx, cy, xh, yh, x, y, w;

    for(j=0; j<MY; j++) 
        for(i=0; i<MX; i++) {
            pw[IX(i,j)] = tmp[IX(i,j)] = 0.0;
         }
    for(k=0; k<po->nhsrc; k++) {
        cx = po->hsrc[k].x+po->hsrc[k].vx*20/2;
        cy = po->hsrc[k].y+po->hsrc[k].vy*20/2;
        p = 0.0;
        p = po->hsrc[k].prs.x*po->hsrc[k].nx
           +po->hsrc[k].prs.y*po->hsrc[k].ny
           +po->hsrc[k].prs.z*po->hsrc[k].nz;
        xh = cx/dh + MX/2.;
        yh = cy/dh + MY/2.;
		float limit = 0.7;
        for(j= -RRR; j<=RRR; j++) {
            iwy = yh + j;
            if(iwy>0 && iwy<MY-1) {
                for(i= -RRR; i<=RRR; i++) {
                    iwx = xh + i;
                    if(iwx>0 && iwx<MX-1) {
                        x = xo+dh*iwx;
                        y = yo+dh*iwy;
                        r = (cx-x)*(cx-x)+(cy-y)*(cy-y);
                        r = sqrt(r)/RRR;
                        w = 0.0;
                        if(r < 1.0){
							//	w = exp(-2*r) - exp(-2);
							w = -1/(1+exp( ((1-r)-0.5)*10 )) + 1;
							if (tmp[IX(iwx,iwy)] + w < limit){
								tmp[IX(iwx,iwy)] += w;
								if (tmp[IX(iwx,iwy)] > limit) w -= tmp[IX(iwx,iwy)]-limit;
//								pw[IX(iwx,iwy)] += 4*p*w;
								if (w>0){
									float sw = 1-w;
									sw = sw*sw*sw;
									if (sw<0) sw = 0;
									u[IX(iwx,iwy)] = (u[IX(iwx,iwy)] - po->vel.x*2) * sw + po->vel.x*2;
									v[IX(iwx,iwy)] = (v[IX(iwx,iwy)] - po->vel.y*2) * sw + po->vel.y*2;
								}
							}
						}
                    }
                }
            }
        }
    }
}

void simulate() {
    int i, j;
    Trealf h;
    Treald *p;

    // wh1, wh2の更新
/*
    for (j = 0; j < MY; j++) {
        for (i = 0; i < MX; i++) {
            wh2[IX(i,j)] = wh1[IX(i,j)];
            wh1[IX(i,j)] = height[IX(i,j)];
        }
    }
*/
    p = wh2;
    wh2 = wh1;
    wh1 = height;
    height = p;

    // 各メッシュ間の距離を求めて、１との差で力発生
    for (j = 1; j < MY - 1; j++) {
      for (i = 1; i < MX - 1; i++) {
        h = wh1[IX(i+1,j)]+wh1[IX(i-1,j)]+wh1[IX(i,j+1)]+wh1[IX(i,j-1)]-4*wh1[IX(i,j)];
        tmp[IX(i,j)] = wcoeff*h+2.0*wh1[IX(i,j)]-wh2[IX(i,j)]- 0.1*wh1[IX(i,j)];
//        tmp[IX(i,j)] = wcoeff*h+wh1[IX(i,j)]+visc*(wh1[IX(i,j)]-wh2[IX(i,j)]);
        height[IX(i,j)] = tmp[IX(i,j)]*loss;
      }
    }

    for(j=0; j<MY; j++) height[IX(0,j)] = height[IX(1,j)];
    for(j=0; j<MY; j++) height[IX(MX-1,j)] = height[IX(MX-2,j)];
    for(i=0; i<MX; i++) height[IX(i,0)] = height[IX(i,1)];
    for(i=0; i<MX; i++) height[IX(i,MY-1)] = height[IX(i,MY-2)];
/*
    for (j = 1; j < MY - 1; j++) {
      for (i = 1; i < MX - 1; i++) {
        height[IX(i,j)] = (tmp[IX(i,j)]+tmp[IX(i,j+1)]+tmp[IX(i,j-1)]+tmp[IX(i-1,j)]+tmp[IX(i+1,j)])/5.;
      }
    }
*/
    // 法線ベクトルを更新

    Tpoint3f vv1, vv2, vv;
    Trealf dis;

    for (j = 1; j < MY-1; j++) {
      for (i = 1; i < MX-1; i++) {
        vv1.x = -dh;
        vv1.y = 0.0;
        vv1.z = (height[IX(i,j)] - height[IX(i+1,j)]);
        vv2.x = 0.0;
        vv2.y = -dh;
        vv2.z = (height[IX(i,j)] - height[IX(i,j+1)]);
        vv.x = -vv1.z*vv2.y;
        vv.y = -vv1.x*vv2.z;
        vv.z =  vv1.x*vv2.y;
        dis = sqrt(vv.x*vv.x+vv.y*vv.y+vv.z*vv.z);
        normal[IX(i,j)].x = vv.x/dis;
        normal[IX(i,j)].y = vv.y/dis;
        normal[IX(i,j)].z = vv.z/dis;
        tvec[IX(i,j)] = calRefracVec(normal[IX(i,j)], vlight, 1.3333);
      }
    }

    Treald s;

    s = 0.0;
    for (j = 0; j < MY; j++) {
        for (i = 0; i < MX; i++) {
//          if(ABS(height[IX(i,j)]) > 0.5) s += ABS(height[IX(i,j)]);
          if(s < ABS(height[IX(i,j)])) s = ABS(height[IX(i,j)]);
        }
    }
//    s /= (MX*MY);

    wmotion[isim_step % nmotion + 1] = s;
    if(max_motion < s) max_motion = s;

  //速度ベクトルの算出
    for(j=1; j<MY-1; j++) {
        for(i=1; i<MX-1; i++) {
            u[IX(i,j)] =dt*(wh1[IX(i,j)]-wh1[IX(i-1,j)])/dh;
            v[IX(i,j)] =dt*(wh1[IX(i,j)]-wh1[IX(i,j-1)])/dh;
        }
    }

    if(yflow != 0.0) {
        for(j=0; j<MY; j++)
            for(i=0; i<MX; i++)
                tmp[IX(i,j)] = height[IX(i,j)];
        for(j=1; j<MY-1; j++) {
            for(i=1; i<MX-1; i++) {
                height[IX(i,j)] = (1.0-yflow)*tmp[IX(i,j)]+yflow*tmp[IX(i,j+1)];
            }
        }

        for(j=0; j<MY; j++)
            for(i=0; i<MX; i++)
                tmp[IX(i,j)] = wh1[IX(i,j)];
        for(j=1; j<MY-1; j++) {
            for(i=1; i<MX-1; i++) {
                wh1[IX(i,j)] = (1.0-yflow)*tmp[IX(i,j)]+yflow*tmp[IX(i,j+1)];
            }
        }

        for(j=0; j<MY; j++)
            for(i=0; i<MX; i++)
                tmp[IX(i,j)] = wh2[IX(i,j)];
        for(j=1; j<MY-1; j++) {
            for(i=1; i<MX-1; i++) {
                wh2[IX(i,j)] = (1.0-yflow)*tmp[IX(i,j)]+yflow*tmp[IX(i,j+1)];
            }
        }
    }
/*
    for(j=1; j<MY-1; j++) {
        for(i=1; i<MX-1; i++) {
            height[IX(i,j)] = height[IX(i,j+1)];
            wh1[IX(i,j)] = wh1[IX(i,j+1)];
            wh2[IX(i,j)] = wh2[IX(i,j+1)];
        }
    }
*/
}


/*================================================================*/

void shiftWater(Treald *x, Trealf shift) {
    int i,j;

    for(j=0; j<MY; j++)
        for(i=0; i<MX; i++)
            tmp[IX(i,j)] = x[IX(i,j)];
    for(j=1; j<MY-1; j++) {
        for(i=1; i<MX-1; i++) {
            x[IX(i,j)] = (1.0-shift)*tmp[IX(i,j)]+shift*tmp[IX(i,j+1)];
        }
    }
}

void simulate2(){
    int i,j;

    //boundary condition
    bound(u,1);
    bound(v,2);
    bound(height,3);
    //solve equation
    solver(height,wh1,u,u1,v,v1,pw);
    bound(u,1);
    bound(v,2);
    bound(height,3);
    
    if(yflow != 0.0) {
        shiftWater(height, yflow);
        shiftWater(wh1, yflow);
        shiftWater(u, yflow);
        shiftWater(v, yflow);
        shiftWater(u1, yflow);
        shiftWater(v1, yflow);
        shiftWater(pw, yflow);
    }


    Tpoint3f vv1, vv2, vv;
    Trealf dis;

    for (j = 1; j < MY-1; j++) {
      for (i = 1; i < MX-1; i++) {
        vv1.x = -dh;
        vv1.y = 0.0;
        vv1.z = height[IX(i,j)] - height[IX(i+1,j)];
        vv2.x = 0.0;
        vv2.y = -dh;
        vv2.z = height[IX(i,j)] - height[IX(i,j+1)];
        vv.x = -vv1.z*vv2.y;
        vv.y = -vv1.x*vv2.z;
        vv.z =  vv1.x*vv2.y;
        dis = sqrt(vv.x*vv.x+vv.y*vv.y+vv.z*vv.z);
        normal[IX(i,j)].x = vv.x/dis;
        normal[IX(i,j)].y = vv.y/dis;
        normal[IX(i,j)].z = vv.z/dis;
        tvec[IX(i,j)] = calRefracVec(normal[IX(i,j)], vlight, 1.3333);
      }
    }
}
/*
void initial(Treald *height,Treald *wh1,Treald *u,Treald *u1,Treald *v,Treald *v1){
    int i,j;

    for(i=0;i<MX;i++){
        for(j=0;j<MY;j++){
            height[IX(i,j)]=0.0;
            wh1[IX(i,j)]=0.0;
            u[IX(i,j)]=0.0;
            u1[IX(i,j)]=0.0;
            v[IX(i,j)]=0.0;
            v1[IX(i,j)]=0.0;
        }
    }

    dt=0.001; //時間刻み
    gravity=9.8; //重力加速度
    depth=0.1; //水底までの深さ
    dh=0.005; //格子幅

}
*/
void solver(Treald *height,Treald *wh1,Treald *u,Treald *u1,Treald *v,Treald *v1, Treald *p){
    int i,j;

	float hmul = 10.0;	//	高さを強調して描画
	float hinv = 1.0/hmul;

    for(i=1;i<=MX-2;i++){
        for(j=1;j<=MY-2;j++){
            u1[IX(i,j)]=u[IX(i,j)]-gravity*(dt/dh)*(height[IX(i+1,j)]-height[IX(i,j)])*hinv+(p[IX(i+1,j)]-p[IX(i,j)])/(rho_w*dh);
            v1[IX(i,j)]=v[IX(i,j)]-gravity*(dt/dh)*(height[IX(i,j+1)]-height[IX(i,j)])*hinv+(p[IX(i,j+1)]-p[IX(i,j)])/(rho_w*dh);
        }
    }
    for(i=1;i<=MX-2;i++){
        for(j=1;j<=MY-2;j++){
            wh1[IX(i,j)]=height[IX(i,j)]*hinv
                -depth*dt*((u1[IX(i,j)]-u1[IX(i-1,j)])*dh+(v1[IX(i,j)]-v1[IX(i,j-1)])*dh)/(dh*dh);
        }
    }

    for(i=1;i<MX-1;i++){
        for(j=1;j<MY-1;j++){
			//	ローパスフィルタ
			const float pass = 300;
			float h = wh1[IX(i,j)]*(4+pass) + 
			wh1[IX(i-1,j)]*2 + 
			wh1[IX(i+1,j)]*2 + 
			wh1[IX(i,j-1)]*2 + 
			wh1[IX(i,j+1)]*2 + 
			wh1[IX(i-1,j-1)] + 
			wh1[IX(i+1,j-1)] + 
			wh1[IX(i-1,j+1)] + 
			wh1[IX(i+1,j+1)];
			h /= (pass+16);
			height[IX(i,j)]=h*loss * hmul;

//    for(i=0;i<MX;i++){
//        for(j=0;j<MY;j++){
//            height[IX(i,j)]=wh1[IX(i,j)]*loss * hmul;

            u[IX(i,j)]=u1[IX(i,j)]*loss;
            v[IX(i,j)]=v1[IX(i,j)]*loss;
        }
    }

}
void bound(Treald *f,int flag){
    int i,j;
    if(flag==1){
        for(j=0;j<MY;j++){//u
            f[IX(0,j)]=0.0;
            f[IX(MX-2,j)]=0.0;
        }
        for(i=0;i<MX;i++){//自由端
            f[IX(i,0)]=f[IX(i,1)];
            f[IX(i,MY-1)]=f[IX(i,MY-2)];
        }
    }else if(flag==2){//v
        for(j=0;j<MY;j++){//自由端
            f[IX(0,j)]=f[IX(1,j)];
            f[IX(MX-1,j)]=f[IX(MX-2,j)];
        }
        for(i=0;i<MX;i++){
            f[IX(i,0)]=0.0;
            f[IX(i,MY-2)]=0.0;
        }
    }else if(flag==3){//height
        //固定端
        for(j=0;j<MY;j++){
            f[IX(0,j)]=f[IX(1,j)];
            f[IX(MX-1,j)]=f[IX(MX-2,j)];
        }
        for(i=0;i<MX;i++){
            f[IX(i,0)]=f[IX(i,1)];
            f[IX(i,MY-1)]=f[IX(i,MY-2)];
        }
    }
}

/*================================================================*/


Tpoint3d calRefracVec(Tpoint3d n, Tpoint3d v, Treald ra) {

    Tpoint3d vd, vd_n, tvec;
    Treald kf, vn, d;

    vn = v.x*n.x+v.y*n.y+v.z*n.z;
    vn = ABS(vn);
    vd.x = -v.x/vn;
    vd.y = -v.y/vn;
    vd.z = -v.z/vn;
    vd_n.x = vd.x + n.x;
    vd_n.y = vd.y + n.y;
    vd_n.z = vd.z + n.z;
    kf = ra*ra
        *(vd.x*vd.x+vd.y*vd.y+vd.z*vd.z)
        -(vd_n.x*vd_n.x+vd_n.y*vd_n.y+vd_n.z*vd_n.z);
    if(kf < 0.0) {
        tvec.x = tvec.y = tvec.z = 0.0;
        return(tvec);
    }
    kf = 1.0/sqrt(kf);

    tvec.x = kf*vd_n.x - n.x;
    tvec.y = kf*vd_n.y - n.y;
    tvec.z = kf*vd_n.z - n.z;
    d = sqrt(tvec.x*tvec.x+tvec.y*tvec.y+tvec.z*tvec.z);
    tvec.x /= d; tvec.y /=d; tvec.z /= d;

    return(tvec);
}

/*
void shadeWaterSurface() {
    int i, j;
    Treald co;

    for (j = 0; j < MY; j++) {
        for (i = 0; i < MX; i++) {
            co = sun.xdir*normal[IX(i,j)].x
                +sun.ydir*normal[IX(i,j)].y
                +sun.zdir*normal[IX(i,j)].z;
            color[IX(i,j)].r = co*sun.r;
            color[IX(i,j)].g = co*sun.g;
            color[IX(i,j)].b = co*sun.b;
        }
    }
}
*/

void writeWaveMotion() {

    FILE *fp;

    if((fp = fopen("test.wmt", "wb")) == NULL) {
        printf("Can't open a file: test.wmt\n");
        return;
    }

    fwrite(&nmotion, sizeof(int), 1, fp);
    fwrite(wmotion, sizeof(Trealf), nmotion, fp);

    fclose(fp);
}

int writeWave() {

    Trealf val, max_val;
    int iw, i, k;
    DWORD tsize, fsize;
    DWORD dsize;

    // サンプリング周波数
    pcmh.wf.nSamplesPerSec = sound.frq;
    
    // 量子化ビット数
    pcmh.wBitsPerSample = sound.bit;
    
    pcmh.wf.wFormatTag = WAVE_FORMAT_PCM;    // PCMフォーマット
    pcmh.wf.nChannels = 1;    // チャンネル数
    if(sound.stereo)     pcmh.wf.nChannels = 2;    // ステレオ
    pcmh.wf.nBlockAlign = pcmh.wBitsPerSample * pcmh.wf.nChannels / 8;    // ブロックサイズ
    pcmh.wf.nAvgBytesPerSec = pcmh.wf.nBlockAlign * pcmh.wf.nSamplesPerSec;    // データ速度

    if(sound.bit == 8)
        dsize = nwav;    // データサイズ
    else if(sound.bit == 16)
        dsize = nwav*2;    // データサイズ

    // 8,16ビットそれぞれに対応したポインタを作る

    max_val = 0.0;
    for(i=0; i<nwav; i++) {
        if(max_val < ABS(wav[i])) max_val = ABS(wav[i]);
    }

    printf("max_val : %f\n", max_val);

    k = 0;
    for(i=0; i<nwav; i++) {
        val = wav[i]/max_val;
        if(sound.bit == 8) {
            iw = (int)(val * 128 + 128);
            if(iw < 0) iw = 0;
            if(iw > 255) iw = 255;
            pcm8_wav[k] = (char)iw; k++;
        } else if(sound.bit == 16) {
            iw = (int)(val * 32767);
            if(iw < -32767) iw = 32767;
            if(iw > 32767) iw = 32767;
            pcm16_wav[k] = (short)iw; k++;
        }
    }

    tsize = sizeof(pcmh);    // タグサイズ
    fsize = tsize + dsize + 28;    // ファイルサイズ（邪道に）

    FILE *fp;

    if((fp = fopen(wfile, "wb")) == NULL) {
        printf("Can't open file: %s\n", wfile);
        return 0;
    }

    fwrite(ID_RIFF, sizeof(char), 4, fp);
    fwrite(&fsize, sizeof(DWORD), 1, fp);
    fwrite(TYPE_WAVE, sizeof(char), 4, fp);
    fwrite(ID_FMT, sizeof(char), 4, fp);
    fwrite(&tsize, sizeof(DWORD), 1, fp);
    fwrite(&pcmh, sizeof(PCMWAVEFORMAT), 1, fp);
    fwrite(ID_DATA, sizeof(char), 4, fp);
    fwrite(&dsize, sizeof(DWORD), 1, fp);
    fwrite(pcm_wav, sizeof(char), dsize, fp);

    fclose(fp);
/*
    ofstream fout(wfile, ios::binary);
    
    if (!fout)
    {
        cout << "出力ファイル " << wfile << " が開けませんでした." << endl;
        return 1;
    }
    
    fout.write(ID_RIFF, sizeof(ID_RIFF));    // RIFF
    fout.write((char*)&fsize, sizeof(fsize));    // ファイルサイズ
    
    fout.write(TYPE_WAVE, sizeof(TYPE_WAVE));    // WAVE
    fout.write(ID_FMT, sizeof(ID_FMT));    // fmt 
    fout.write((char*)&tsize, sizeof(tsize));    // タグ長
    fout.write((char*)&pcmh, sizeof(pcmh));    // PCMヘッダ
    
    fout.write(ID_DATA, sizeof(ID_DATA));    // DATA
    fout.write((char*)&dsize, sizeof(dsize));    // データサイズ
    fout.write(pcm_data, dsize);    // PCMデータ
    
    fout.close();
*/
//    delete [] pcm_data;
    
    return 0;
}


int initSpAudio(int frq, int bit, int nsmp, int istereo) {
    double tmp;
    int itmp, nch;
    int nbuf;

    audio = spInitAudio();
    spSetAudioSampleRate(audio, (double)frq);
    spGetAudioSampleRate(audio, &tmp);
    if(tmp != (double)frq) {
        printf("sp error! sample rate, %f != %f\n", (double)frq, tmp);
        return(FALSE);
    }
    nch = 1;
    if(istereo == 1) nch = 2;
    spSetAudioChannel(audio, nch);
    spGetAudioChannel(audio, &itmp);
    if(itmp != nch) {
        printf("sp error! channel, %d != %d\n", nch, itmp);
        return(FALSE);
    }

    nbuf = nsmp;
    if(bit == 16) nbuf *= 2;
    spSetAudioBufferSize(audio, nbuf);
    spGetAudioBufferSize(audio, &itmp);
    if(itmp != nbuf) {
        printf("sp error! buf size, %d != %d\n", nbuf, itmp);
        return(FALSE);
    }

    spSetAudioSampleBit(audio, bit);
    spGetAudioSampleBit(audio, &itmp);
    if(itmp != bit) {
        printf("sp error! bit, %d != %d\n", bit, itmp);
        return(FALSE);
    }

    spOpenAudioDevice(audio, "w");
//    spSyncAudio(audio);
    return(TRUE);
}

void setupSoundSample(char *filename, Tsound *sound) {
    int i;
    int iw;
    Trealf val;

    readPcm(filename, &pcmh, &dsize, &pcm_data);
    pcm8 = pcm_data;
    pcm16 = (short *)pcm_data;
    npcm = dsize;
    if(pcmh.wBitsPerSample == 16) npcm /= 2;

    sound->frq = pcmh.wf.nSamplesPerSec;
    sound->bit = pcmh.wBitsPerSample;
    sound->stereo = TRUE;
    if(pcmh.wf.nChannels == 1) sound->stereo = FALSE;
    sound->nsmp = npcm;
    sound->ndata = sound->nsmp * (sound->stereo == TRUE ? 2 : 1);
    sound->wave = (Trealf *)malloc(sizeof(Trealf)*sound->ndata);

    for(i=0; i<npcm; i++) {
        if(pcmh.wBitsPerSample == 8) {
            iw = (int)pcm8[i];
            val = (Trealf)(iw - 128.0)/(Trealf)128.0;
        } else if(pcmh.wBitsPerSample == 16) {
            iw = (int)pcm16[i];
            val = (Trealf)iw/(Trealf)32767.0;
        }
        sound->wave[i] = val;
    }
}

int readPcm(char *filename, PCMWAVEFORMAT *pcmh, DWORD *dsize, char **data) {
    FILE *fp;
    int tsize, fsize;
    char ID_RIFF[4];
    char ID_FMT[4];
    char ID_DATA[4];
    char TYPE_WAVE[4];

    if((fp = fopen(filename, "rb")) == NULL) {
        printf("Can't open the file:%s\n", filename);
        return 1;
    }

    fread(ID_RIFF, sizeof(char), 4, fp);
    fread(&fsize, sizeof(DWORD), 1, fp);
    fread(TYPE_WAVE, sizeof(char), 4, fp);
    fread(ID_FMT, sizeof(char), 4, fp);
    fread(&tsize, sizeof(DWORD), 1, fp);
    fread(pcmh, sizeof(PCMWAVEFORMAT), 1, fp);
    fread(ID_DATA, sizeof(char), 4, fp);
    fread(dsize, sizeof(DWORD), 1, fp);

    if(*data == NULL)
        *data = (char *)malloc(sizeof(char)*(*dsize));
    else
        *data = (char *)realloc(*data, sizeof(char)*(*dsize));

    fread(*data, sizeof(char), *dsize, fp);
/*
    printf("ID_RIFF: %c%c%c%c\n", ID_RIFF[0], ID_RIFF[1], ID_RIFF[2], ID_RIFF[3]);
    printf("fsize: %d\n", fsize);
    printf("TYPE_WAVE: %c%c%c%c\n", TYPE_WAVE[0], TYPE_WAVE[1], TYPE_WAVE[2], TYPE_WAVE[3]);
    printf("ID_FMT: %c%c%c%c\n", ID_FMT[0], ID_FMT[1], ID_FMT[2], ID_FMT[3]);
    printf("tsize: %d\n", tsize);
    printf("frq: %d\n", pcmh->wf.nSamplesPerSec);
    printf("bit: %d\n", pcmh->wBitsPerSample);
    printf("ch:  %d\n", pcmh->wf.nChannels);
    printf("ID_DATA: %c%c%c%c\n", ID_DATA[0], ID_DATA[1], ID_DATA[2], ID_DATA[3]);
    printf("dsize: %d\n", *dsize);
*/
    if(pcmh->wf.nChannels > 2) {
        printf("*** Too much channles.. %d\n", pcmh->wf.nChannels);
    }

    fclose(fp);

    return 0;
}
