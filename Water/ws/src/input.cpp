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
#include <io.h>
#include <GL/glut.h>

#include "ws.h"
#include "emlimg.h"

#define NUM_OBJ_FILE 10

/*==== GLOBAL VARIABLES ====*/
GLOBAL Tcamera cam;
GLOBAL Tear     ear;
GLOBAL EMLImg emlimg;
GLOBAL EMLImg *teximg;
GLOBAL GLuint    *texname;
GLOBAL int     ntex;
GLOBAL int     env_map, box_map, ground_map;
GLOBAL FILE *sysout;
GLOBAL int    MX, MY;
GLOBAL Treald        *height, dt, dh, xo, yo, loss, visc, gravity, depth;
GLOBAL Trealf rnd_wave, xflow, yflow, flow_scl;
GLOBAL GLfloat       light_pos[4];
GLOBAL GLfloat       water_spec[4];
GLOBAL GLfloat       water_diffuse[4];
GLOBAL GLfloat       water_shin;

GLOBAL Tobject *obj;
GLOBAL int     nobj, nvtx;
GLOBAL Treald  objBox[2][3];

GLOBAL Trealf *wav;
GLOBAL int    nwav;
GLOBAL Tsound sound;
GLOBAL Tsound *stex;
GLOBAL int   nstex;
GLOBAL Treald  *spos, dpos;
GLOBAL char *pcm_data;
GLOBAL char* pcm8;
GLOBAL short* pcm16;

// FOR HAPTIC USING SPIDAR
GLOBAL Tpoint3f *force_data, *torq_data;
GLOBAL Tpoint3f  pos_hap;
GLOBAL int    force_frq;
GLOBAL int    nforce, nforce_max;
GLOBAL Trealf pscale, force_max, torq_max;
GLOBAL int    use_spidar;
GLOBAL ThapticObj pobj;

GLOBAL TfluidForceTex *force_tex;
GLOBAL int   nforce_tex;
GLOBAL Trealf dv_force;

GLOBAL ThapticObj boat;

GLOBAL Trod rod;

GLOBAL int       nfrc_set;
GLOBAL TforceSet *frc_set;

/* === LOCAL STATIC VARIABLES === */
BMPFILEHEADER bmFileHeader;
BMPINFOHEADER bmInfoHeader;

/*== FUNCTIONS ==*/
void readObject(FILE *fp);
void readCommand(char *fname);
void cal_minmax_obj();
void cal_normal();
void transObj(char *comd, int io1, int io2
             ,Treald u, Treald v, Treald w
             ,Treald tu, Treald tv, Treald tw);
void readPatch(FILE *fp, int *nvtx, int no1, int no2, int type);
void cal_inertia_tensor(ThapticObj *po);

void setCameraParam(FILE *fp);
void setEnvMap(FILE *fp);
void setBoxMap(FILE *fp);
void setGroundMap(FILE *fp);
void setTexture(FILE *fp);
void readBmp(EMLImg *img, char *filename);
void setImage(EMLImg *img, FILE *fp);
void readBmpPixel(EMLImg *img, FILE *fp);
void setSound(FILE *fp);
void setFluidForceTex(FILE *fp);
void setFluidForceSet(FILE *fp);
void setPointerObj(FILE *fp);
void setFluidForceMap(FILE *fp);

void genPoints();
void genMesh(ThapticObj *ho);
void genMeshForQuad(Tmesh *bm, Tface *fac);
void genMeshForTri(Tmesh *bm, Tface *fac);
void setHsrcID(ThapticObj *ho);

void setBoat(FILE *fp);
void setFishing(FILE *fp);

void setWater(FILE *fp);
void setRandomWave(FILE *fp);

extern void setupSoundSample(char *filename, Tsound *sound);
extern Treald getAreaofTriangle(Tpoint3d p[3]);

/*=====================================
   READ COMMAND FROM FILE
=======================================*/
void readCommand(char *filename) {
    FILE *fp;
    char cmd[256];
    int iend;

    if((fp = fopen(filename, "r")) == NULL) {
        printf("can't open the file: %s\n", filename);
        printf("program stop.\n");
        exit(1);
    }

    cam.init_view.vx = -2.5;
    cam.init_view.vy = 0.0;
    cam.init_view.vz = 0.0;
    cam.init_view.rx = 0.0;
    cam.init_view.ry = 0.0;
    cam.init_view.rz = 0.0;
    cam.init_view.vr = 2.5;
    cam.init_view.the = M_PI*0.5;
    cam.init_view.phi = 0.0;
    cam.aspect = (GLdouble)cam.winX/(GLdouble)cam.winY;
    cam.init_view.fovy = 60.0;
    pobj.id_obj = 0;
    pobj.vmax = 1.;
    pobj.disp_scl = 1.0;

    boat.id_obj = -1;
    boat.disp_scl = 1.0;
    rod.id_obj = -1;
    rod.lure_view_display = -100.0;
/*
    back_color.r = 0.0;
    back_color.g = 0.2;
    back_color.b = 0.5;
    back_color.a = 1.0;
    fog.col[0] = 0.0;
    fog.col[1] = 0.0;
    fog.col[2] = 0.0;
    fog.dens = -1.0;
*/
//    nstex = 0;
    nobj = 0;
    ntex = 0;
    box_map = ground_map = env_map = -1;
    rnd_wave = -1.0;
    iend = 0;
    while(!iend) {

        fscanf(fp, "%s", cmd);
/*
        if(!strcmp("SOUNDTEX", cmd) || !strcmp("soundtex",cmd)) 
            readSoundTex(fp);
        else if(!strcmp("BACK", cmd) || !strcmp("back",cmd)) 
            setBackColor(fp);
        else if(!strcmp("PROJ-TEXTURE", cmd) || !strcmp("proj-texture",cmd)) 
            setProjTex(fp);
        else if(!strcmp("EAR", cmd) || !strcmp("ear",cmd)) 
            setEar(fp);
*/
        if(!strcmp("OBJECT", cmd) || !strcmp("object",cmd)) 
            readObject(fp);
        else if(!strcmp("CAMERA", cmd) || !strcmp("camera",cmd)) 
            setCameraParam(fp);
        else if(!strcmp("WATER", cmd) || !strcmp("water",cmd)) 
            setWater(fp);
        else if(!strcmp("RAND-WAVE", cmd) || !strcmp("rand-wave",cmd)) 
            setRandomWave(fp);
        else if(!strcmp("FLUID-FORCE", cmd) || !strcmp("fluid-force",cmd)) 
            setFluidForceTex(fp);
        else if(!strcmp("FORCE-SET", cmd) || !strcmp("force-set",cmd)) 
            setFluidForceSet(fp);
        else if(!strcmp("POINTER-OBJ", cmd) || !strcmp("pointer-obj",cmd)) 
            setPointerObj(fp);
        else if(!strcmp("BOAT", cmd) || !strcmp("boat",cmd)) {
            setBoat(fp);
        } else if(!strcmp("FISHING", cmd) || !strcmp("fishing",cmd)) 
            setFishing(fp);
        else if(!strcmp("ENV-MAP", cmd) || !strcmp("env-map",cmd)) 
            setEnvMap(fp);
        else if(!strcmp("BOX", cmd) || !strcmp("box",cmd)) 
            setBoxMap(fp);
        else if(!strcmp("GROUND", cmd) || !strcmp("GROUND",cmd)) 
            setGroundMap(fp);
        else if(!strcmp("TEXTURE", cmd) || !strcmp("texture",cmd)) 
            setTexture(fp);
        else if(!strcmp("SOUND", cmd) || !strcmp("sound",cmd)) 
            setSound(fp);
        else if(!strcmp("END",cmd) || !strcmp("end",cmd))
            iend = 1;
    }
    fclose(fp);
    
    if(nobj > 0) {
        cal_minmax_obj();
    }
    
    setHsrcID(&pobj);
//    if(boat.id_obj >= 0) setHsrcID(&boat);
//  set intertia tesor of haptic obj
    if(pobj.im.x > 0) cal_inertia_tensor(&pobj);
    if(boat.id_obj >= 0) {
        cal_inertia_tensor(&boat);
    }
}

void setCameraParam(FILE *fp) {

    fscanf(fp, "%lf%lf%lf", &cam.init_view.vx, &cam.init_view.vy, &cam.init_view.vz);
    fscanf(fp, "%lf%lf%lf", &cam.init_view.rx, &cam.init_view.ry, &cam.init_view.rz);
    fscanf(fp, "%lf%lf%lf", &cam.init_view.vr, &cam.init_view.the, &cam.init_view.phi);
    fscanf(fp, "%d%d%lf", &cam.winX, &cam.winY, &cam.init_view.fovy);
}

void setWater(FILE *fp) {
    Trealf spec, diff, shin;

    fscanf(fp, "%d%d%lf%lf", &MX, &MY, &loss, &depth);
    fscanf(fp, "%lf%lf", &dt, &dh);
    fscanf(fp, "%f%f", &yflow, &flow_scl);
    fscanf(fp, "%f%f%f%f", &spec, &diff, &shin);
    water_spec[0] = 
    water_spec[1] = 
    water_spec[2] = spec;
    water_spec[3] = spec;
    water_diffuse[0] = 
    water_diffuse[1] = 
    water_diffuse[2] = diff;
    water_diffuse[3] = diff;
    water_shin = shin;
}

void setRandomWave(FILE *fp) {

    fscanf(fp, "%f", &rnd_wave);

}

void setPointerObj(FILE *fp) {

    int i, j, k;
    Trealf dlen, v0, dx, dy, dz, sx, sy, sz;
    Tpoint3f pos0, we0;
    char cx[5], cy[5], cz[5], com[80];
    char filename[80];
    FILE *fm;

    if(nobj <= 0) {
       printf("POINTER-OBJ command error !: no objects specified. !!\n");
       return;
    }

    fscanf(fp, "%d%f%f%f%f%f%f", &pobj.id_obj, &pobj.buo_scl, &pobj.pres_scl, &pobj.fric_scl
                               , &pobj.vel_scl, &pobj.unit_mass, &pobj.disp_scl);
    fscanf(fp, "%f%f", &pobj.wz, &pobj.wa);
    fscanf(fp, "%f%f%f%f%f%f", &pos0.x, &pos0.y, &pos0.z, &we0.x, &we0.y, &we0.z);
    fscanf(fp, "%f", &dlen);
    if(dlen > 0.0) { 
        pobj.mesh = (Tmesh *)malloc(sizeof(Tmesh)*obj[pobj.id_obj].nf);
        for(i=0; i<obj[pobj.id_obj].nf; i++) pobj.mesh[i].dlen = dlen;
        genMesh(&pobj);
    } else {
        // USE OBJECT VERTEX AS SAMPLE POINTS
        pobj.mesh = NULL;
        genPoints();
    }

    Trealf stx, sty, stz, ctx, cty, ctz;
    
    we0.x = we0.x/180.0*M_PI;
    we0.y = we0.y/180.0*M_PI;
    we0.z = we0.z/180.0*M_PI;

    stx = sin(we0.x); ctx = cos(we0.x);
    sty = sin(we0.y); cty = cos(we0.y);
    stz = sin(we0.z); ctz = cos(we0.z);

    pobj.m[0] = cty*ctz; pobj.m[4] = -stx*sty*ctz-ctx*stz; pobj.m[8] = -ctx*sty*ctz+stx*stz;
    pobj.m[1] = cty*stz; pobj.m[5] = -stx*sty*stz+ctx*ctz; pobj.m[9] = -ctx*sty*stz-stx*ctz;
    pobj.m[2] = sty; pobj.m[6] = stx*cty; pobj.m[10] =  ctx*cty;
    pobj.m[3] = 0.0; pobj.m[7] = 0.0; pobj.m[11] = 0; pobj.m[15] = 1.0;

    pobj.m[12] = pos0.x;
    pobj.m[13] = pos0.y;
    pobj.m[14] = pos0.z;

    printf("%f %f %f %f\n", pobj.m[0], pobj.m[4], pobj.m[8], pobj.m[12]);
    printf("%f %f %f %f\n", pobj.m[1], pobj.m[5], pobj.m[9], pobj.m[13]);
    printf("%f %f %f %f\n", pobj.m[2], pobj.m[6], pobj.m[10], pobj.m[14]);
    printf("%f %f %f %f\n", pobj.m[3], pobj.m[7], pobj.m[11], pobj.m[15]);

    ThapticSource *hp;
    fscanf(fp, "%f%s", &v0, filename);
    fscanf(fp, "%f%f%f%f%f%f", &dx, &dy, &dz, &sx, &sy, &sz);
    fm = fopen(filename, "rb");
    if(fm != NULL) {
        Trealf dthe, dphi, fmax, frc_max = 0.0, d;
        int ndata, nthe, nphi, rate, sym[3], n, m, nd, *id, nhsrc;
        Tpoint3f p, normal;
        Trealf xmean, ymean, zmean;
        
        printf("Reading %s ...\n", filename);
        fread(&dthe, sizeof(Trealf), 1, fm);
        fread(&dphi, sizeof(Trealf), 1, fm);
        fread(&nhsrc, sizeof(int), 1, fm);
        fread(&ndata, sizeof(int), 1, fm);
        fread(&rate, sizeof(int), 1, fm);
        fread(&nthe, sizeof(int), 1, fm);
        fread(&nphi, sizeof(int), 1, fm);
        fread(sym, sizeof(int), 3, fm);
        fread(&n, sizeof(int), 1, fm);
        id = (int *)malloc(sizeof(int)*n);
        fread(id, sizeof(int), n, fm);
        
        printf("  dthe, dphi   : %f,  %f\n", dthe, dphi);
        printf("  nthe, nphi   : %d,  %d\n", nthe, nphi);
        printf("  no. of hsrc  : %d\n", nhsrc);
        printf("  no. of data  : %d\n", ndata);
        printf("  rate of data : %d\n", rate);
        printf("  symmetry flag: %d %d %d\n", sym[0], sym[1], sym[2]);
        printf("  no. of frc   : %d\n", n);
        printf("  id of frc    : ");
        for(i=0; i<n; i++) printf("%d  ", id[i]);
        printf("\n");

        nd = (nthe-2)*nphi+2;

        frc_set = (TforceSet *)malloc(sizeof(TforceSet)*nhsrc);
        pobj.nhsrc = nhsrc;
        pobj.hsrc = (ThapticSource *)malloc(sizeof(ThapticSource)*pobj.nhsrc);
        hp = pobj.hsrc;
        for(i=0; i<nhsrc; i++) {
            frc_set[i].dthe = dthe;
            frc_set[i].dphi = dphi;
            frc_set[i].nthe = nthe;
            frc_set[i].nphi = nphi;
            frc_set[i].ntex = nd;
            frc_set[i].v0 = v0;
            frc_set[i].frc = (TfluidForceTex *)malloc(sizeof(TfluidForceTex)*frc_set[i].ntex);
            for(j=0; j<frc_set[i].ntex; j++) {
                frc_set[i].frc[j].xprs =  frc_set[i].frc[j].yprs = frc_set[i].frc[j].zprs =
                frc_set[i].frc[j].xfri =  frc_set[i].frc[j].yfri = frc_set[i].frc[j].zfri = NULL;
                frc_set[i].frc[j].n = 0;
                frc_set[i].frc[j].rate = 0;
            }
        }
        for(i=0; i<nhsrc; i++) {
            fread(&p, sizeof(Tpoint3f), 1, fm);
            fread(&normal, sizeof(Tpoint3f), 1, fm);
            p.x += dx; p.y += dy; p.z += dz;
            p.x *= sx; p.y *=sy; p.z *=sz;
            printf("hsrc %4d\n", i);
            printf("    pos: %f %f %f\n", p.x, p.y, p.z);
            printf("    normal: %f %f %f\n", normal.x, normal.y, normal.z);
            if(nhsrc == 1) {normal.x = 1.0; normal.y = 0.0; normal.z = 0.0;}
            hp->ntex = -1;
            hp->itex = NULL;
            hp->fset = &(frc_set[i]);
            hp->sym.x = sym[0];
            hp->sym.y = sym[1];
            hp->sym.z = sym[2];
            hp->vx = hp->vy = hp->vz =
            hp->v = hp->tcoord = 0.0;
            hp->x0 = p.x;
            hp->y0 = p.y;
            hp->z0 = p.z;
            hp->x = hp->x0;
            hp->y = hp->y0;
            hp->z = hp->z0;
            hp->x_ori = hp->x0;
            hp->y_ori = hp->y0;
            hp->z_ori = hp->z0;
            hp->nx = normal.x;
            hp->ny = normal.y;
            hp->nz = normal.z;
            hp->prs.x = 
            hp->prs.y = 
            hp->prs.z = 0.0;
            for(j=0; j<n; j++) {
                k = id[j];
                frc_set[i].frc[k].n = ndata;
                frc_set[i].frc[k].rate = rate;
                frc_set[i].frc[k].v = v0;
                frc_set[i].frc[k].xprs = (Trealf *)malloc(sizeof(Trealf)*ndata);
                frc_set[i].frc[k].yprs = (Trealf *)malloc(sizeof(Trealf)*ndata);
                frc_set[i].frc[k].zprs = (Trealf *)malloc(sizeof(Trealf)*ndata);
                frc_set[i].frc[k].xfri = (Trealf *)malloc(sizeof(Trealf)*ndata);
                frc_set[i].frc[k].yfri = (Trealf *)malloc(sizeof(Trealf)*ndata);
                frc_set[i].frc[k].zfri = (Trealf *)malloc(sizeof(Trealf)*ndata);
                fread(frc_set[i].frc[k].xprs, sizeof(Trealf), ndata, fm);
                fread(frc_set[i].frc[k].yprs, sizeof(Trealf), ndata, fm);
                fread(frc_set[i].frc[k].zprs, sizeof(Trealf), ndata, fm);
                fread(frc_set[i].frc[k].xfri, sizeof(Trealf), ndata, fm);
                fread(frc_set[i].frc[k].yfri, sizeof(Trealf), ndata, fm);
                fread(frc_set[i].frc[k].zfri, sizeof(Trealf), ndata, fm);
                frc_set[i].frc[k].peri_prs.x =
                frc_set[i].frc[k].peri_prs.y =
                frc_set[i].frc[k].peri_prs.z = (Trealf)frc_set[i].frc[k].n/(Trealf)frc_set[i].frc[k].rate;
                frc_set[i].frc[k].peri_fri.x =
                frc_set[i].frc[k].peri_fri.y =
                frc_set[i].frc[k].peri_fri.z = (Trealf)frc_set[i].frc[k].n/(Trealf)frc_set[i].frc[k].rate;
                frc_set[i].frc[k].phas_prs.x =
                frc_set[i].frc[k].phas_prs.y =
                frc_set[i].frc[k].phas_prs.z = 0.0;
                frc_set[i].frc[k].phas_fri.x =
                frc_set[i].frc[k].phas_fri.y =
                frc_set[i].frc[k].phas_fri.z = 0.0;

                fmax = 0.0;
                xmean = ymean = zmean = 0.0;
                for(m=0; m<ndata; m++) {
                    d = frc_set[i].frc[k].xprs[m]*frc_set[i].frc[k].xprs[m]
                       +frc_set[i].frc[k].yprs[m]*frc_set[i].frc[k].yprs[m]
                       +frc_set[i].frc[k].zprs[m]*frc_set[i].frc[k].zprs[m];
                    if(d > 0.0) d = sqrt(d);
                    if(fmax < d) fmax = d;
                    d = frc_set[i].frc[k].xfri[m]*frc_set[i].frc[k].xfri[m]
                       +frc_set[i].frc[k].yfri[m]*frc_set[i].frc[k].yfri[m]
                       +frc_set[i].frc[k].zfri[m]*frc_set[i].frc[k].zfri[m];
                    if(d > 0.0) d = sqrt(d);
                    if(fmax < d) fmax = d;
                    xmean += frc_set[i].frc[k].xprs[m];
                    ymean += frc_set[i].frc[k].yprs[m];
                    zmean += frc_set[i].frc[k].zprs[m];
                }
                
//                fprintf(sysout, "%d, %d: %f, %f %f %f\n", id[j], nd, fmax*100000., xmean*100000., ymean*100000., zmean*100000.);
                if(frc_max < fmax) frc_max = fmax;
            }
            hp++;
        }
        printf("frc_max: %f\n", frc_max);
        for(i=0; i<nhsrc; i++) {
            for(j=0; j<frc_set[i].ntex; j++) {
                if(frc_set[i].frc[j].rate > 0) {
                    for(k=0; k<frc_set[i].frc[j].n; k++) {
                        frc_set[i].frc[j].xprs[k] /= frc_max;
                        frc_set[i].frc[j].yprs[k] /= frc_max;
                        frc_set[i].frc[j].zprs[k] /= frc_max;
                        frc_set[i].frc[j].xfri[k] /= frc_max;
                        frc_set[i].frc[j].yfri[k] /= frc_max;
                        frc_set[i].frc[j].zfri[k] /= frc_max;
                    }
                }
            }
        }
        free(id);
    } else {
        // SET POINT HAPTIC SOURCE
        int j, n, id_fset;

        fscanf(fp, "%d", &n);
        pobj.nhsrc = n;
        pobj.hsrc = (ThapticSource *)malloc(sizeof(ThapticSource)*n);
        hp = pobj.hsrc;
        for(i=0; i<n; i++) {
            fscanf(fp,"%f%f%f%d", &hp->x0, &hp->y0, &hp->z0, &hp->ntex);
            if(hp->ntex > 0) {
                hp->itex = (int *)malloc(sizeof(int)*hp->ntex);
                for(j=0; j<hp->ntex; j++) fscanf(fp, "%d", &hp->itex[j]);
                hp->fset = NULL;
            } else {
                hp->itex = NULL;
                fscanf(fp, "%d", &id_fset);
                hp->fset = &(frc_set[id_fset]);
            }
            fscanf(fp,"%s%s%s", cx, cy, cz);
            hp->x = hp->x0;
            hp->y = hp->y0;
            hp->z = hp->z0;
            hp->x_ori = hp->x0;
            hp->y_ori = hp->y0;
            hp->z_ori = hp->z0;
            hp->vx = hp->vy = hp->vz =
            hp->v = hp->tcoord = 0.0;
            hp->sym.x = hp->sym.y = hp->sym.z = FALSE;
            if(cx[0] == 'x') hp->sym.x = TRUE;
            if(cy[0] == 'y') hp->sym.y = TRUE;
            if(cz[0] == 'z') hp->sym.z = TRUE;
            hp++;
        }
    }
    
    pobj.im.x = pobj.im.y = pobj.im.z = -1.0;
    fscanf(fp, "%s", com);
    if(!strcmp("param", com)) {
        fscanf(fp, "%f%f%f%f%f", &pobj.mass, &pobj.im.x, &pobj.im.y, &pobj.im.z, &pobj.loss);
        pobj.ori[0] = 1.0; pobj.ori[1] = 0.0; pobj.ori[2] = 0.0; pobj.ori[3] = 0.0;
        pobj.a.x = pobj.a.y = pobj.a.z = 
        pobj.vel.x = pobj.vel.y = pobj.vel.z = 0.0;
    }
}

void setBoat(FILE *fp) {

    Trealf dlen;

    fscanf(fp, "%d", &boat.id_obj);
    fscanf(fp, "%f%f%f%f", &boat.mass, &boat.im.x, &boat.im.y, &boat.im.z);
    fscanf(fp, "%f%f", &boat.loss, &boat.buo_scl);
    fscanf(fp, "%f%f", &boat.wz, &boat.wa);
    fscanf(fp, "%f", &dlen);
    if(dlen <= 0.0) dlen = 1.0;

    int i;
    
    boat.mesh = (Tmesh *)malloc(sizeof(Tmesh)*obj[boat.id_obj].nf);
    for(i=0; i<obj[boat.id_obj].nf; i++) boat.mesh[i].dlen = dlen;
    genMesh(&boat);

    boat.ori[0] = 1.0;
    boat.ori[1] = 0.0;
    boat.ori[2] = 0.0;
    boat.ori[3] = 0.0;

    TsamplePoint *sp;

    sp = boat.sp;
    for(i=0; i<boat.n_sp; i++)
        if(sp[i].n[2] > 0.0) sp[i].s = 0.0;
    boat.nhsrc = -1;
    boat.a.x = 
    boat.a.y = 
    boat.a.z = 
    boat.vel.x = 
    boat.vel.y = 
    boat.vel.z = 
    boat.pos.x= 
    boat.pos.y = 
    boat.pos.z = 0.0;
}

void setFishing(FILE *fp) {

    fscanf(fp, "%d", &rod.id_obj);
    fscanf(fp, "%f%f%f", &rod.s, &rod.r0, &rod.length);
    fscanf(fp, "%f%f", &rod.heso, &rod.ude);
    fscanf(fp, "%f%f%f", &rod.lure.x, &rod.lure.y, &rod.lure.z);
    fscanf(fp, "%f\n", &rod.lure_view_display);
    rod.rr0 = rod.r0*rod.r0;
}

Trealf readForceTex(char *filename, TfluidForceTex *fftex) {
    FILE *ffp;
    int j;
    Trealf d;
    Tpoint3f pmax, fmax;
    Trealf pma, pmi, fma, fmi, pave, fave, frc_max;

    if((ffp = fopen(filename, "rb")) == NULL) {
        fftex->v = -1.0;
        printf("Can't open frc file: %s\n", filename);
    } else {
        fread(&fftex->n, sizeof(int), 1, ffp);
        fread(&fftex->rate, sizeof(int), 1, ffp);
        fftex->xprs = (Trealf *)malloc(sizeof(Trealf)*fftex->n);
        fftex->yprs = (Trealf *)malloc(sizeof(Trealf)*fftex->n);
        fftex->zprs = (Trealf *)malloc(sizeof(Trealf)*fftex->n);
        fftex->xfri = (Trealf *)malloc(sizeof(Trealf)*fftex->n);
        fftex->yfri = (Trealf *)malloc(sizeof(Trealf)*fftex->n);
        fftex->zfri = (Trealf *)malloc(sizeof(Trealf)*fftex->n);
        fread(fftex->xprs, sizeof(Trealf), fftex->n, ffp);
        fread(fftex->yprs, sizeof(Trealf), fftex->n, ffp);
        fread(fftex->zprs, sizeof(Trealf), fftex->n, ffp);
        fread(fftex->xfri, sizeof(Trealf), fftex->n, ffp);
        fread(fftex->yfri, sizeof(Trealf), fftex->n, ffp);
        fread(fftex->zfri, sizeof(Trealf), fftex->n, ffp);
        fclose(ffp);
        
        pmax.x = pmax.y = pmax.z = -10000.0;
        fmax.x = fmax.y = fmax.z = -10000.0;
        pave = fave = 0.0;
        pmi = fmi =  100000.0;
        pma = fma = -100000.0;
        frc_max = 0.;
        for(j=0; j<fftex->n; j++) {
            if(pmax.x < ABS(fftex->xprs[j])) pmax.x = ABS(fftex->xprs[j]);
            if(pmax.y < ABS(fftex->yprs[j])) pmax.y = ABS(fftex->yprs[j]);
            if(pmax.z < ABS(fftex->zprs[j])) pmax.z = ABS(fftex->zprs[j]);
            if(fmax.x < ABS(fftex->xfri[j])) fmax.x = ABS(fftex->xfri[j]);
            if(fmax.y < ABS(fftex->yfri[j])) fmax.y = ABS(fftex->yfri[j]);
            if(fmax.z < ABS(fftex->zfri[j])) fmax.z = ABS(fftex->zfri[j]);

            d = fftex->xprs[j]*fftex->xprs[j]
               +fftex->yprs[j]*fftex->yprs[j]
               +fftex->zprs[j]*fftex->zprs[j];
            if(d > 0.0) d = sqrt(d);
            if(pma < d) pma = d;
            if(pmi > d) pmi = d;
            if(frc_max < d) frc_max = d;
            pave += d;

            d = fftex->xfri[j]*fftex->xfri[j]
               +fftex->yfri[j]*fftex->yfri[j]
               +fftex->zfri[j]*fftex->zfri[j];
            if(d > 0.0) d = sqrt(d);
            if(fma < d) fma = d;
            if(fmi > d) fmi = d;
            if(frc_max < d) frc_max = d;
            fave += d;
        }
            
        pave /= fftex->n;
        fave /= fftex->n;

        printf("frc tex %s: size %d, rate %d, speed %f\n"
            , filename, fftex->n, fftex->rate, fftex->v);
        printf("       dir    : (%8.4f  %8.4f)\n", fftex->the/M_PI*180.0, fftex->phi/M_PI*180.0);
        printf("       frc max:  %8.4f x 10^-5\n", frc_max*100000.);
        printf("       prs max: (%8.4f  %8.4f  %8.4f)\n", pmax.x/frc_max, pmax.y/frc_max, pmax.z/frc_max);
        printf("       fri max: (%8.4f  %8.4f  %8.4f)\n", fmax.x/frc_max, fmax.y/frc_max, fmax.z/frc_max);
        printf("       prs abs: (%8.4f <-> %8.4f,  %8.4f)\n", pmi/frc_max, pma/frc_max, pave/frc_max);
        printf("       fri abs: (%8.4f <-> %8.4f,  %8.4f)\n", fmi/frc_max, fma/frc_max, fave/frc_max);
    }
    
    fclose(ffp);
    return(frc_max);
}

void setFluidForceTex(FILE *fp) {

    int i;
    char filename[256];
    Trealf x, y, z, the, phi, d;
    Trealf frc_max;
//    Trealf vscl;
    
    fscanf(fp, "%d", &nforce_tex);
//    fscanf(fp, "%d%f%f", &nforce_tex, &vscl, &dv_force);
    force_tex = (TfluidForceTex *)malloc(sizeof(TfluidForceTex)*nforce_tex);
    frc_max = 0.0;
    for(i=0; i<nforce_tex; i++) {
        fscanf(fp,"%f%f%f%f%s", &force_tex[i].v
            , &x, &y, &z, filename);
        d = sqrt(x*x + y*y + z*z);
        x /= d; y /= d; z /= d;
        force_tex[i].dir.x = x;
        force_tex[i].dir.y = y;
        force_tex[i].dir.z = z;
        the = acos(z);
        phi = acos(x);
        if(y < 0.0) phi = 2.0*M_PI-phi;
        force_tex[i].the = the;
        force_tex[i].phi = phi;
//        force_tex[i].v *= vscl;
        d = readForceTex(filename, &(force_tex[i]));
        if(frc_max < d) frc_max = d;
    }
    pobj.vmax = force_tex[nforce_tex-1].v;
}

void setFluidForceSet(FILE *fp) {
    int i, j, n;
    char filename[256];
    TfluidForceTex *pfrc;
    Trealf d, frc_max;

    printf("FORCE-SET COMMAND!\n");
    fscanf(fp, "%d", &nfrc_set);
    frc_set = (TforceSet *)malloc(sizeof(TforceSet)*nfrc_set);
    frc_max = 0;
    for(i=0; i<nfrc_set; i++) {
        fscanf(fp, "%f%d%d", &frc_set[i].v0, &frc_set[i].nthe, &frc_set[i].nphi);
        frc_set[i].dthe = M_PI/(frc_set[i].nthe-1);
        frc_set[i].dphi = 2.0*M_PI/frc_set[i].nphi;
        n = (frc_set[i].nthe-2)*frc_set[i].nphi+2;
        frc_set[i].ntex = n;
        frc_set[i].frc = (TfluidForceTex *)malloc(sizeof(TfluidForceTex)*n);
        pfrc = frc_set[i].frc;
        for(j=0; j<n; j++) {
            fscanf(fp, "%f%f%s", &pfrc->the, &pfrc->phi, filename);
            pfrc->the = pfrc->the/180.0*M_PI;
            pfrc->phi = pfrc->phi/180.0*M_PI;
            pfrc->dir.x = cos(pfrc->phi)*sin(pfrc->the);
            pfrc->dir.y = sin(pfrc->phi)*sin(pfrc->the);
            pfrc->dir.z = cos(pfrc->the);
            pfrc->v = frc_set[i].v0;
            if(!strcmp(filename, "-")) {
                pfrc->xprs = 
                pfrc->yprs = 
                pfrc->zprs = 
                pfrc->xfri = 
                pfrc->yfri = 
                pfrc->zfri = NULL;
                pfrc->n = 0;
                pfrc->rate = 0;
            } else {
                d = readForceTex(filename, pfrc);
                if(frc_max < d) frc_max = d;
                pfrc->peri_prs.x =
                pfrc->peri_prs.y =
                pfrc->peri_prs.z = (Trealf)pfrc->n/(Trealf)pfrc->rate;
                pfrc->peri_fri.x =
                pfrc->peri_fri.y =
                pfrc->peri_fri.z = (Trealf)pfrc->n/(Trealf)pfrc->rate;
                pfrc->phas_prs.x =
                pfrc->phas_prs.y =
                pfrc->phas_prs.z = 0.0;
                pfrc->phas_fri.x =
                pfrc->phas_fri.y =
                pfrc->phas_fri.z = 0.0;
//                printf("    period: %f\n", pfrc->T);
            }
            pfrc++;
        }
    }

    int k;
    for(i=0; i<nfrc_set; i++) {
        pfrc = frc_set[i].frc;
        for(j=0; j<frc_set[i].ntex; j++) {
            if(pfrc->rate > 0) {
                for(k=0; k<pfrc->n; k++) {
                    pfrc->xprs[k] /= frc_max;
                    pfrc->yprs[k] /= frc_max;
                    pfrc->zprs[k] /= frc_max;
                    pfrc->xfri[k] /= frc_max;
                    pfrc->yfri[k] /= frc_max;
                    pfrc->zfri[k] /= frc_max;
                }
            }
            pfrc++;
        }
    }
}

void setEnvMap(FILE *fp) {
    
    fscanf(fp, "%d", &env_map);
}

void setBoxMap(FILE *fp) {
    
    fscanf(fp, "%d", &box_map);
}

void setGroundMap(FILE *fp) {
    
    fscanf(fp, "%d", &ground_map);
}

void setSound(FILE *fp) {
    int j;
    char filename[80];
    
//    printf("sound command\n");
    
    fscanf(fp, "%d%d", &sound.frq, &sound.bit);
    fscanf(fp, "%d", &nstex);
    stex = (Tsound *)malloc(sizeof(stex)*nstex);
    spos = (Treald *)malloc(sizeof(Treald)*nstex);
    dpos = 1.0;
    for(j=0; j<nstex; j++) {
        fscanf(fp, "%s", filename);
        setupSoundSample(filename, &(stex[j]));
    }
}

void setTexture(FILE *fp) {

    fscanf(fp, "%d", &ntex);
    teximg = (EMLImg *)malloc(sizeof(EMLImg)*(ntex+1));

    int i;
    
    for(i=0; i<ntex; i++) {
        fscanf(fp, "%s", teximg[i].filename);
        teximg[i].pix = NULL;
        readBmp(&teximg[i], teximg[i].filename);
    }
}

void readBmp(EMLImg *img, char *filename) {

    FILE *fp;

    /* Open the bmp file in binay mode */
    if((fp = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "can't open the file %s.\n",filename);
        exit(1);
    }
    
    /* Read bmp file header */
    fread(bmFileHeader.bfType, sizeof(char), 2, fp);
    fread(&bmFileHeader.bfSize, sizeof(unsigned long int), 1, fp);
    fread(&bmFileHeader.bfReserved1, sizeof(unsigned short int), 1, fp);
    fread(&bmFileHeader.bfReserved2, sizeof(unsigned short int), 1, fp);
    fread(&bmFileHeader.bfOffBits, sizeof(unsigned long int), 1, fp);
/*
    fprintf(stdout,"\n-----ビットマップ・ファイルヘッダ内の情報-----\n");
    fprintf(stdout,"ファイル識別  : %s\n",bmFileHeader.bfType);
    fprintf(stdout,"ファイルサイズ: %ld\n",bmFileHeader.bfSize);
    fprintf(stdout,"予約領域1     : %hd\n",bmFileHeader.bfReserved1);
    fprintf(stdout,"予約領域2     : %hd\n",bmFileHeader.bfReserved2);
    fprintf(stdout,"オフセット    : %ld\n\n",bmFileHeader.bfOffBits);
*/
    /* Read bmp information header */
    fread(&bmInfoHeader.biSize, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biWidth, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biHeight, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biPlanes, sizeof(unsigned short int), 1, fp);
    fread(&bmInfoHeader.biBitCount, sizeof(unsigned short int), 1, fp);
    fread(&bmInfoHeader.biCompression, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biSizeImage, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biXPelsPerMeter, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biYPelsPerMeter, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biClrUsed, sizeof(unsigned long int), 1, fp);
    fread(&bmInfoHeader.biClrImportant, sizeof(unsigned long int), 1, fp);
/*
    fprintf(stdout,"-----ビットマップ・インフォメーションヘッダ内の情報-----\n");
    fprintf(stdout,"ヘッダサイズ  : %ld\n",bmInfoHeader.biSize);
    fprintf(stdout,"画像の幅      : %ld\n",bmInfoHeader.biWidth);
    fprintf(stdout,"画像の高さ    : %ld\n",bmInfoHeader.biHeight);
    fprintf(stdout,"プレーン数    : %hd\n",bmInfoHeader.biPlanes);
    fprintf(stdout,"カラービット数: %hd\n",bmInfoHeader.biBitCount);
    fprintf(stdout,"圧縮方式      : %ld\n",bmInfoHeader.biCompression);
    fprintf(stdout,"画像サイズ    : %ld\n",bmInfoHeader.biSizeImage);
    fprintf(stdout,"水平解像度    : %ld\n",bmInfoHeader.biXPelsPerMeter);
    fprintf(stdout,"垂直解像度    : %ld\n",bmInfoHeader.biYPelsPerMeter);
    fprintf(stdout,"色数    : %ld\n",bmInfoHeader.biClrUsed);
    fprintf(stdout,"重要な色数    : %ld\n\n",bmInfoHeader.biClrImportant);
*/
    /* Set Eml Parameters */
    setImage(img, fp);
        
    /* Read pixel values */
    readBmpPixel(img, fp);

    fclose(fp);
}

/*-------------------------------------------------------------
                       Reading BMP file
--------------------------------------------------------------*/
void setImage(EMLImg *img, FILE *fp) {

    int    bytes;
    char   buf[256];
    time_t tim, tloc;
    char   *tm;

    /* == set parameters for emlimg ==*/
    img->width = (int)bmInfoHeader.biWidth;
    img->height = (int)bmInfoHeader.biHeight;
    img->right = (int)bmInfoHeader.biWidth-1;
    img->top = (int)bmInfoHeader.biHeight-1;
    img->left = 0;
    img->bottom = 0;
    
    bytes = img->width * sizeof(Pixel);
    bytes *= img->height;
    
    if((img->pix = (Pixel *) malloc (bytes)) == NULL) {
        fprintf(stderr, "can't malloc pixel.\n");
        exit(1);
    }
    
    sprintf(buf, "BMP -> EML");
    strncpy(img->comment, buf, COMMENTBYTE);
    tim = time(&tloc);
    tm = ctime(&tim);
    strncpy(img->date, tm, DATEBYTE);

}

void readBmpPixel(EMLImg *img, FILE *fp) {

    int    i, j;
    int    n;
    int    bmp_id, eml_id;
    int    bytes;
    Pixel  *tmp_pix;
    
    //fseek(fp, bmp.offsetL, SEEK_SET);
    
    n = img->width*img->height;
    bytes = img->width * sizeof(Pixel);
    bytes *= img->height;
    
    if((tmp_pix = (Pixel *) malloc (bytes)) == NULL) {
        fprintf(stderr, "can't malloc bmp pixel.\n");
        exit(1);
    }
 
    /* ファイルからピクセルデータを一気に(bytesバイト分)読みこみます */
    fread(tmp_pix, sizeof(Pixel), n, fp);
    
    /* BMP形式からEML形式へのラスタデータ変換を行います */
    bmp_id = eml_id = 0;
    for(j=0; j<img->height; j++) {
//        bmp_id = n-(img->width*(j+1));
        for(i=0; i<img->width; i++) {
            img->pix[eml_id].b = tmp_pix[bmp_id].r;
            img->pix[eml_id].r = tmp_pix[bmp_id].b;
            img->pix[eml_id].g = tmp_pix[bmp_id].g;
            eml_id++;
            bmp_id++;
        }
    }
}


void setHsrcID(ThapticObj *ho) {
    int i, j, id;
    Trealf x, y, z, dx, dy, dz, d, dmin;
    TsamplePoint *sp;

    for(j=0; j<ho->nhsrc; j++) ho->hsrc[j].n_sp = 0;

    sp = ho->sp;
    for(i=0; i<ho->n_sp; i++) {
        x = sp[i].p[0];
        y = sp[i].p[1];
        z = sp[i].p[2];
        sp[i].i_hsrc = -1;
        dmin = 10000000.;
        id = -1;
        for(j=0; j<ho->nhsrc; j++) {
            dx = x - ho->hsrc[j].x;
            dy = y - ho->hsrc[j].y;
            dz = z - ho->hsrc[j].z;
            d = dx*dx + dy*dy + dz*dz;
            if(dmin > d) {
                dmin = d;
                id = j;

            }
        }
        sp[i].i_hsrc = id;
        ho->hsrc[id].n_sp++;
    }
/*********おかしーーーーーーーー
stick.datでdlen = 0.5、Release時に間違った
近傍点を選択する場合がある。

*/
    for(j=0; j<ho->nhsrc; j++) {
        ho->hsrc[j].col.r = (Trealf)rand()/(Trealf)RAND_MAX*0.9 + 0.1;
        ho->hsrc[j].col.g = (Trealf)rand()/(Trealf)RAND_MAX*0.9 + 0.1;
        ho->hsrc[j].col.b = (Trealf)rand()/(Trealf)RAND_MAX*0.9 + 0.1;
    }
}

void genPoints() {
    int i, j, k, n;
    Treald s;
    Tpoint3d p[3];
    Tface *fac;

    i = pobj.id_obj;
    pobj.n_sp = obj[i].nf;
    pobj.sp = (TsamplePoint *)malloc(sizeof(TsamplePoint)*pobj.n_sp);
    pobj.area = 0.0;
    fac = obj[i].face;
    for(j=0; j<obj[i].nf; j++) {
        pobj.sp[j].p[0] = pobj.sp[j].p[1] = pobj.sp[j].p[2] = 0.0;
        n = 0;
        for(k=0; k<4; k++) {
            if(fac->pv[k] != NULL) {
                pobj.sp[j].p[0] += fac->pv[k]->p[0];
                pobj.sp[j].p[1] += fac->pv[k]->p[1];
                pobj.sp[j].p[2] += fac->pv[k]->p[2];
                n++;
            }
        }
        pobj.sp[j].p[0] /= (Trealf)n;
        pobj.sp[j].p[1] /= (Trealf)n;
        pobj.sp[j].p[2] /= (Trealf)n;
        pobj.sp[j].n[0] = fac->normal[0];
        pobj.sp[j].n[1] = fac->normal[1];
        pobj.sp[j].n[2] = fac->normal[2];
        
        pobj.sp[j].p0[0] = pobj.sp[j].p[0];
        pobj.sp[j].p0[1] = pobj.sp[j].p[1];
        pobj.sp[j].p0[2] = pobj.sp[j].p[2];
        pobj.sp[j].n0[0] = pobj.sp[j].n[0];
        pobj.sp[j].n0[1] = pobj.sp[j].n[1];
        pobj.sp[j].n0[2] = pobj.sp[j].n[2];

        p[0].x = fac->pv[0]->p[0];
        p[0].y = fac->pv[0]->p[1];
        p[0].z = fac->pv[0]->p[2];
        p[1].x = fac->pv[1]->p[0];
        p[1].y = fac->pv[1]->p[1];
        p[1].z = fac->pv[1]->p[2];
        p[2].x = fac->pv[2]->p[0];
        p[2].y = fac->pv[2]->p[1];
        p[2].z = fac->pv[2]->p[2];
        s = getAreaofTriangle(p);
        if(fac->pv[3] != NULL) {
            p[0].x = fac->pv[0]->p[0];
            p[0].y = fac->pv[0]->p[1];
            p[0].z = fac->pv[0]->p[2];
            p[1].x = fac->pv[2]->p[0];
            p[1].y = fac->pv[2]->p[1];
            p[1].z = fac->pv[2]->p[2];
            p[2].x = fac->pv[3]->p[0];
            p[2].y = fac->pv[3]->p[1];
            p[2].z = fac->pv[3]->p[2];
            s += getAreaofTriangle(p);
        }
        pobj.sp[j].s = s;
        pobj.area += s;
        fac++;
    }
}

void genMesh(ThapticObj *ho) {

    int i, j;
    int    nnp;

    nnp = 0;

    i = ho->id_obj;
    for(j=0; j<obj[i].nf; j++) {
        if(obj[i].face[j].pv[3] != NULL) genMeshForQuad(&(ho->mesh[j]), &(obj[i].face[j]));
        else genMeshForTri(&(ho->mesh[j]), &(obj[i].face[j]));
        nnp += ho->mesh[j].np;
    }

    printf("No. of patches: %d\n", nnp);

    ho->n_sp = nnp;
    ho->sp = (TsamplePoint *)malloc(sizeof(TsamplePoint)*ho->n_sp);
    ho->area = 0.0;

    int k = 0;

    for(i=0; i<obj[ho->id_obj].nf; i++) {
        for(j=0; j<ho->mesh[i].np; j++) {
            ho->sp[k].p[0] = (ho->mesh[i].patch[j].pv[0]->x
                                  +ho->mesh[i].patch[j].pv[1]->x
                                  +ho->mesh[i].patch[j].pv[2]->x
                                  +ho->mesh[i].patch[j].pv[3]->x)/4.0;
            ho->sp[k].p[1] = (ho->mesh[i].patch[j].pv[0]->y
                                  +ho->mesh[i].patch[j].pv[1]->y
                                  +ho->mesh[i].patch[j].pv[2]->y
                                  +ho->mesh[i].patch[j].pv[3]->y)/4.0;
            ho->sp[k].p[2] = (ho->mesh[i].patch[j].pv[0]->z
                                  +ho->mesh[i].patch[j].pv[1]->z
                                  +ho->mesh[i].patch[j].pv[2]->z
                                  +ho->mesh[i].patch[j].pv[3]->z)/4.0;
            ho->sp[k].n[0] = obj[ho->id_obj].face[i].normal[0];
            ho->sp[k].n[1] = obj[ho->id_obj].face[i].normal[1];
            ho->sp[k].n[2] = obj[ho->id_obj].face[i].normal[2];
            ho->sp[k].s = ho->mesh[i].patch[j].area;
            ho->sp[k].p0[0] = ho->sp[k].p[0];
            ho->sp[k].p0[1] = ho->sp[k].p[1];
            ho->sp[k].p0[2] = ho->sp[k].p[2];
            ho->sp[k].n0[0] = ho->sp[k].n[0];
            ho->sp[k].n0[1] = ho->sp[k].n[1];
            ho->sp[k].n0[2] = ho->sp[k].n[2];
            ho->area += ho->sp[k].s;
            k++;
         }
    }
    
}

void genMeshForQuad(Tmesh *bm, Tface *fac) {

    int k,iu,iv, n, m;
    Treald du1, du2, dv1, dv2;
    Treald xu1, yu1, zu1, xv1, yv1, zv1;
    Treald xu2, yu2, zu2, xv2, yv2, zv2;
    Tpoint3d pa, pb, p[3];
//    Tmesh *bm;

//    bm = &(fac->mesh);

    xu1=fac->pv[1]->p[0]-fac->pv[0]->p[0];
    yu1=fac->pv[1]->p[1]-fac->pv[0]->p[1];
    zu1=fac->pv[1]->p[2]-fac->pv[0]->p[2];
    du1 = sqrt(xu1*xu1+yu1*yu1+zu1*zu1);
    n = du1/bm->dlen+1;

    xu2=fac->pv[2]->p[0]-fac->pv[3]->p[0];
    yu2=fac->pv[2]->p[1]-fac->pv[3]->p[1];
    zu2=fac->pv[2]->p[2]-fac->pv[3]->p[2];
    du2 = sqrt(xu2*xu2+yu2*yu2+zu2*zu2);
    if(n < du2/bm->dlen+1) n = du2/bm->dlen+1;

    xv1=fac->pv[3]->p[0]-fac->pv[0]->p[0];
    yv1=fac->pv[3]->p[1]-fac->pv[0]->p[1];
    zv1=fac->pv[3]->p[2]-fac->pv[0]->p[2];
    dv1 = sqrt(xv1*xv1+yv1*yv1+zv1*zv1);
    m = dv1/bm->dlen+1;

    xv2=fac->pv[2]->p[0]-fac->pv[1]->p[0];
    yv2=fac->pv[2]->p[1]-fac->pv[1]->p[1];
    zv2=fac->pv[2]->p[2]-fac->pv[1]->p[2];
    dv2 = sqrt(xv2*xv2+yv2*yv2+zv2*zv2);
    if(m < dv2/bm->dlen+1) m = dv2/bm->dlen+1;

    if(n == 1) n++;
    if(m == 1) m++;

    bm->nu = n;
    bm->nv = m;

    bm->vp = (Tpoint3f *)malloc(sizeof(Tpoint3f)*bm->nu*bm->nv);
    bm->share_cnt = (unsigned char *)malloc(sizeof(unsigned char)*bm->nu*bm->nv);
    k = 0;
    for(iv=0; iv<bm->nv; iv++) {
        pa.x = fac->pv[0]->p[0]+xv1/(m-1)*iv;
        pa.y = fac->pv[0]->p[1]+yv1/(m-1)*iv;
        pa.z = fac->pv[0]->p[2]+zv1/(m-1)*iv;
        pb.x = fac->pv[1]->p[0]+xv2/(m-1)*iv;
        pb.y = fac->pv[1]->p[1]+yv2/(m-1)*iv;
        pb.z = fac->pv[1]->p[2]+zv2/(m-1)*iv;
        for(iu=0; iu<bm->nu; iu++) {
            bm->vp[k].x = pa.x+(pb.x-pa.x)/(n-1)*iu;
            bm->vp[k].y = pa.y+(pb.y-pa.y)/(n-1)*iu;
            bm->vp[k].z = pa.z+(pb.z-pa.z)/(n-1)*iu;
            bm->share_cnt[k] = 0;
            k++;
        }
    }

    bm->np = (bm->nu-1)*(bm->nv-1);
    bm->patch = (Tpatch *)malloc(sizeof(Tpatch)*bm->np);
//    nnp += bm->np;
    k = 0;
    for(iv=0; iv<bm->nv-1; iv++) {
        for(iu=0; iu<bm->nu-1; iu++) {
            bm->patch[k].pv[0] = &(bm->vp[iv*bm->nu+iu]);
            bm->patch[k].pv[1] = &(bm->vp[iv*bm->nu+iu+1]);
            bm->patch[k].pv[2] = &(bm->vp[(iv+1)*bm->nu+iu+1]);
            bm->patch[k].pv[3] = &(bm->vp[(iv+1)*bm->nu+iu]);
            bm->share_cnt[iv*bm->nu+iu]++;
            bm->share_cnt[iv*bm->nu+iu+1]++;
            bm->share_cnt[(iv+1)*bm->nu+iu+1]++;
            bm->share_cnt[(iv+1)*bm->nu+iu]++;

            p[0].x = bm->patch[k].pv[0]->x;
            p[0].y = bm->patch[k].pv[0]->y;
            p[0].z = bm->patch[k].pv[0]->z;
            p[1].x = bm->patch[k].pv[1]->x;
            p[1].y = bm->patch[k].pv[1]->y;
            p[1].z = bm->patch[k].pv[1]->z;
            p[2].x = bm->patch[k].pv[2]->x;
            p[2].y = bm->patch[k].pv[2]->y;
            p[2].z = bm->patch[k].pv[2]->z;
            bm->patch[k].area = getAreaofTriangle(p);

            p[0].x = bm->patch[k].pv[0]->x;
            p[0].y = bm->patch[k].pv[0]->y;
            p[0].z = bm->patch[k].pv[0]->z;
            p[1].x = bm->patch[k].pv[2]->x;
            p[1].y = bm->patch[k].pv[2]->y;
            p[1].z = bm->patch[k].pv[2]->z;
            p[2].x = bm->patch[k].pv[3]->x;
            p[2].y = bm->patch[k].pv[3]->y;
            p[2].z = bm->patch[k].pv[3]->z;
            bm->patch[k].area += getAreaofTriangle(p);
            k++;
        }
    }
}

void genMeshForTri(Tmesh *bm, Tface *fac) {
    int k,iu,iv, n, m;
    Treald lenu, lenv, lenuv;
    Treald xu, yu, zu, xv, yv, zv, xx, yy, zz;
    Tpoint3d ori, uvec, p[3];
//    Tmesh *bm;


//    bm = &(fac->mesh);
    ori.x = fac->pv[0]->p[0];
    ori.y = fac->pv[0]->p[1];
    ori.z = fac->pv[0]->p[2];

    xu=fac->pv[1]->p[0]-fac->pv[0]->p[0];
    yu=fac->pv[1]->p[1]-fac->pv[0]->p[1];
    zu=fac->pv[1]->p[2]-fac->pv[0]->p[2];

    xv=fac->pv[2]->p[0]-fac->pv[0]->p[0];
    yv=fac->pv[2]->p[1]-fac->pv[0]->p[1];
    zv=fac->pv[2]->p[2]-fac->pv[0]->p[2];

    xx=fac->pv[2]->p[0]-fac->pv[1]->p[0];
    yy=fac->pv[2]->p[1]-fac->pv[1]->p[1];
    zz=fac->pv[2]->p[2]-fac->pv[1]->p[2];

    lenu = xu*xu+yu*yu+zu*zu;
    lenu = (Treald)sqrt(lenu);
    n = lenu/bm->dlen+1;

    lenv = xv*xv+yv*yv+zv*zv;
    lenv = (Treald)sqrt(lenv);
    if(n < lenv/bm->dlen+1) n = lenv/bm->dlen+1;

    lenuv = xx*xx+yy*yy+zz*zz;
    lenuv = (Treald)sqrt(lenuv);
    m = lenuv/bm->dlen+1;

    if(n == 1) n++;
    if(m == 1) m++;

    bm->nu = n;
    bm->nv = m;

    bm->vp = (Tpoint3f *)malloc(sizeof(Tpoint3f)*bm->nu*bm->nv);
    bm->share_cnt = (unsigned char *)malloc(sizeof(unsigned char)*bm->nu*bm->nv);
    k = 0;
    for(iv=0; iv<bm->nv; iv++) {
        for(iu=0; iu<bm->nu; iu++) {
            uvec.x = fac->pv[1]->p[0]+iv*xx/(m-1)-ori.x;
            uvec.y = fac->pv[1]->p[1]+iv*yy/(m-1)-ori.y;
            uvec.z = fac->pv[1]->p[2]+iv*zz/(m-1)-ori.z;
            bm->vp[k].x = iu*uvec.x/(n-1)+ori.x;
            bm->vp[k].y = iu*uvec.y/(n-1)+ori.y;
            bm->vp[k].z = iu*uvec.z/(n-1)+ori.z;
            bm->share_cnt[k] = 0;
            k++;
        }
    }

    bm->np = (bm->nu-1)*(bm->nv-1);
    bm->patch = (Tpatch *)malloc(sizeof(Tpatch)*bm->np);
//    nnp += bm->np;
    k = 0;
    for(iv=0; iv<bm->nv-1; iv++) {
        for(iu=0; iu<bm->nu-1; iu++) {
            bm->patch[k].pv[0] = &(bm->vp[iv*bm->nu+iu]);
            bm->patch[k].pv[1] = &(bm->vp[iv*bm->nu+iu+1]);
            bm->patch[k].pv[2] = &(bm->vp[(iv+1)*bm->nu+iu+1]);
            bm->patch[k].pv[3] = &(bm->vp[(iv+1)*bm->nu+iu]);
            bm->share_cnt[iv*bm->nu+iu]++;
            bm->share_cnt[iv*bm->nu+iu+1]++;
            bm->share_cnt[(iv+1)*bm->nu+iu+1]++;
            bm->share_cnt[(iv+1)*bm->nu+iu]++;
            p[0].x = bm->patch[k].pv[0]->x;
            p[0].y = bm->patch[k].pv[0]->y;
            p[0].z = bm->patch[k].pv[0]->z;
            p[1].x = bm->patch[k].pv[1]->x;
            p[1].y = bm->patch[k].pv[1]->y;
            p[1].z = bm->patch[k].pv[1]->z;
            p[2].x = bm->patch[k].pv[2]->x;
            p[2].y = bm->patch[k].pv[2]->y;
            p[2].z = bm->patch[k].pv[2]->z;
            bm->patch[k].area = getAreaofTriangle(p);

            p[0].x = bm->patch[k].pv[0]->x;
            p[0].y = bm->patch[k].pv[0]->y;
            p[0].z = bm->patch[k].pv[0]->z;
            p[1].x = bm->patch[k].pv[2]->x;
            p[1].y = bm->patch[k].pv[2]->y;
            p[1].z = bm->patch[k].pv[2]->z;
            p[2].x = bm->patch[k].pv[3]->x;
            p[2].y = bm->patch[k].pv[3]->y;
            p[2].z = bm->patch[k].pv[3]->z;
            bm->patch[k].area += getAreaofTriangle(p);
            k++;
        }
    }
}


/*====================================
    subroutines for reading objects
======================================*/
void readObject(FILE *ffp) {

    FILE *fp;
    int    i,j,k,iv[4],iend, nfile;
    int no, no1, no2, n, io1,io2, len;
    char comd[5];
    char comment[80];
    char buf[256];
    Treald u,v,w,tu,tv,tw;
    char filename[NUM_OBJ_FILE][80], ctype[80], stype[80];
    int type[NUM_OBJ_FILE], use_stencil[NUM_OBJ_FILE];
    int dtype[NUM_OBJ_FILE], itex[NUM_OBJ_FILE];
    Trealf tex_scl[NUM_OBJ_FILE];
    Trealf spec[NUM_OBJ_FILE], diffuse[NUM_OBJ_FILE], shin[NUM_OBJ_FILE];

    fscanf(ffp, "%d", &nfile);
    if(nfile > NUM_OBJ_FILE) {
        printf("set NUM_OBJ_FILE > %d in render.c\n", nfile);
        exit(1);
    }

    nobj = 0;
    for(i=0; i<nfile; i++) {
        fscanf(ffp,"%s%s%s%d%f",filename[i], ctype, stype,&(itex[i]), &(tex_scl[i]));
        fscanf(ffp,"%f%f%f",&spec[i], &diffuse[i], &shin[i]);
        printf("tex: %d, scl: %f\n", itex[i],tex_scl[i]);
        fp=fopen(filename[i],"r");
        if(fp == NULL) {
            fprintf(stderr,"Can't open the file: %s\n",filename[i]);
            exit(1);
        }
        type[i] = STATIC_OBJ;
        use_stencil[i] = FALSE;
        if(!strcmp(ctype, "dynamic")) type[i] = DYNAMIC_OBJ;
        if(!strcmp(stype, "stencil")) use_stencil[i] = TRUE;
        fgets(buf,256,fp);
        sscanf(buf,"%d",&no);
        fclose(fp);
        len = strlen(filename[i]);
        dtype[i] = 0;
        if(!strcmp("pat", filename[i]+len-3)) dtype[i] = 1;
        nobj += no;
    }
    obj = (Tobject *)malloc(sizeof(Tobject)*nobj);

    no1 = 0;
    nvtx=0;
    for(n=0; n<nfile; n++) {

//    printf("reading object file:%s\n",filename[n]);

    fp=fopen(filename[n],"r");

    fgets(buf,256,fp);
    sscanf(buf,"%d",&no);
    no2 = no1+no;

    if(dtype[n] == 1) {
        readPatch(fp, &nvtx, no1, no2, type[n]);
        no1 = no2;
        fclose(fp);
        continue;
    }
    for(i=no1; i<no2; i++) {
        obj[i].type = type[n];
        obj[i].stencil = use_stencil[n];
        obj[i].id_tex = itex[n];
        obj[i].tex_scl = tex_scl[n];
        obj[i].fog = FALSE;
        obj[i].shaded = FALSE;
        obj[i].transparent = FALSE;
        Trealf *m;
        m = obj[i].m;
        m[0] = 1.0; m[4] = 0.0; m[8] =  0.0; m[12] = 0.0;
        m[1] = 0.0; m[5] = 1.0; m[9] =  0.0; m[13] = 0.0;
        m[2] = 0.0; m[6] = 0.0; m[10] = 1.0; m[14] = 0.0;
        m[3] = 0.0; m[7] = 0.0; m[11] = 0.0; m[15] = 1.0;
        fgets(buf,256,fp);
        sscanf(buf,"%d%d%lf%lf%lf%s",&obj[i].nv
                                ,&obj[i].nf
                                ,&obj[i].ref[0]
                                ,&obj[i].ref[1]
                                ,&obj[i].ref[2],comment);
        if(obj[i].ref[0] < 0.0 ||
           obj[i].ref[1] < 0.0 ||
           obj[i].ref[2] < 0.0) {
            obj[i].ref[0] *= -1.0;
            obj[i].ref[1] *= -1.0;
            obj[i].ref[2] *= -1.0;
            obj[i].transparent = TRUE;
        }
        if(!strcmp("textured", comment)) obj[i].textured = TRUE;
        if(shin[i] >= 0.0) {
            obj[i].shin = shin[i];
            obj[i].spec[0] = 
            obj[i].spec[1] = 
            obj[i].spec[2] = 
            obj[i].spec[3] = spec[i];
            obj[i].diffuse[0] = diffuse[i]*obj[i].ref[0];
            obj[i].diffuse[1] = diffuse[i]*obj[i].ref[1];
            obj[i].diffuse[2] = diffuse[i]*obj[i].ref[2];
            obj[i].diffuse[3] = 1.0;
        }
/*
        if(obj[i].ref[0] > 1.0) obj[i].ref[0]/=100.;
        if(obj[i].ref[1] > 1.0) obj[i].ref[1]/=100.;
        if(obj[i].ref[2] > 1.0) obj[i].ref[2]/=100.;
*/
        obj[i].face=(Tface *)malloc(sizeof(Tface)*obj[i].nf);
        obj[i].vtx=(Tvertex *)malloc(sizeof(Tvertex)*obj[i].nv);
        nvtx+=obj[i].nv;

        for(j=0; j<obj[i].nv; j++) {
             fgets(buf,256,fp);
             sscanf(buf,"%lf%lf%lf",&obj[i].vtx[j].p[0]
                                ,&obj[i].vtx[j].p[1]
                                ,&obj[i].vtx[j].p[2]);
        }

        for(j=0; j<obj[i].nf; j++) {
             fgets(buf,256,fp);
             sscanf(buf,"%d%d%d%d",&iv[0]
                                 ,&iv[1]
                                 ,&iv[2]
                                 ,&iv[3]);
             if(iv[2] == iv[3]) iv[3] = 0;
             iv[0]--;iv[1]--;iv[2]--;iv[3]--;
             for(k=0; k<4; k++) {
                 if(iv[k] >= 0)
                     obj[i].face[j].pv[k] = &(obj[i].vtx[iv[k]]);
                 else
                     obj[i].face[j].pv[k] = NULL;

             }
        }

        /*=== READING TEXTURE COORDINATES IF TEXTURED ===*/
        if(obj[i].textured == TRUE) {
            int ntex;
            fgets(buf,256,fp);
            sscanf(buf, "%d%d", &ntex, &obj[i].id_tex);
            obj[i].tex_coords = (Tpoint2f *)malloc(sizeof(Tpoint2f)*ntex);
            obj[i].ntex_coords = ntex;

            for(j=0; j<obj[i].ntex_coords; j++) {
                fgets(buf, 256,fp);
                sscanf(buf, "%f%f", &obj[i].tex_coords[j].x
                                  , &obj[i].tex_coords[j].y);
            }

            for(j=0; j<obj[i].nf; j++) {
                 fgets(buf,256,fp);
                 sscanf(buf,"%d%d%d%d",&iv[0]
                                     ,&iv[1]
                                     ,&iv[2]
                                     ,&iv[3]);
                 iv[0]--;iv[1]--;iv[2]--;iv[3]--;
                 for(k=0; k<4; k++) {
                     if(iv[k] >= 0)
                         obj[i].face[j].tex[k] = &(obj[i].tex_coords[iv[k]]);
                     else
                         obj[i].face[j].tex[k] = NULL;
                 }
            }
        }
    }

/*** CHANGE GEOMETRY ***/
    iend=1;
    while(iend) {
        fgets(buf,256,fp);
        sscanf(buf,"%s%d%d%lf%lf%lf%lf%lf%lf",comd,&io1,&io2
                                      ,&u,&v,&w,&tu,&tv,&tw);
        io1--; io2--; io1 += no1; io2 += no1;
        if(!strcmp(comd,"END")) {
            iend=0;
        } else {
            transObj(comd, io1, io2, u, v, w, tu, tv, tw);
        }
    }
    no1 = no2;
    fclose(fp);
    }

    cal_normal();
}

void transObj(char *comd, int io1, int io2
             ,Treald u, Treald v, Treald w
             ,Treald tu, Treald tv, Treald tw) {
    int i, j;

    if(!strcmp(comd,"MOVE")) {
        for(i=io1; i<=io2; i++) {
            for(j=0; j<obj[i].nv; j++) {
                obj[i].vtx[j].p[0]+=u;
                obj[i].vtx[j].p[1]+=v;
                obj[i].vtx[j].p[2]+=w;
            }
        }
    } else if(!strcmp(comd,"SCALE") || !strcmp(comd,"SCAL")) {
        for(i=io1; i<=io2; i++) {
            for(j=0; j<obj[i].nv; j++) {
                obj[i].vtx[j].p[0]=(obj[i].vtx[j].p[0]-u)*tu+u;
                obj[i].vtx[j].p[1]=(obj[i].vtx[j].p[1]-v)*tv+v;
                obj[i].vtx[j].p[2]=(obj[i].vtx[j].p[2]-w)*tw+w;
            }
        }
    } else if(!strcmp(comd,"ZREV")) {
        Treald th,ct,st,x,y;
        th=(Treald)(tu*M_PI/180.0);
        ct=(Treald)cos(th); st=(Treald)sin(th);
        for(i=io1; i<=io2; i++) {
            for(j=0; j<obj[i].nv; j++) {
                x=(obj[i].vtx[j].p[0]-u)*ct-(obj[i].vtx[j].p[1]-v)*st;
                y=(obj[i].vtx[j].p[0]-u)*st+(obj[i].vtx[j].p[1]-v)*ct;
                obj[i].vtx[j].p[0]=x+u;
                obj[i].vtx[j].p[1]=y+v;
            }
        }
    } else if(!strcmp(comd,"XREV")) {
        Treald th,ct,st,x,y;
        th=(Treald)(tu*M_PI/180.0);
        ct=(Treald)cos(th); st=(Treald)sin(th);
        for(i=io1; i<=io2; i++) {
            for(j=0; j<obj[i].nv; j++) {
                x=(obj[i].vtx[j].p[1]-v)*ct-(obj[i].vtx[j].p[2]-w)*st;
                y=(obj[i].vtx[j].p[1]-v)*st+(obj[i].vtx[j].p[2]-w)*ct;
                obj[i].vtx[j].p[1]=x+v;
                obj[i].vtx[j].p[2]=y+w;
            }
        }
    } else if(!strcmp(comd,"YREV")) {
        Treald th,ct,st,x,y;
        th=(Treald)(tu*M_PI/180.0);
        ct=(Treald)cos(th); st=(Treald)sin(th);
        for(i=io1; i<=io2; i++) {
            for(j=0; j<obj[i].nv; j++) {
                x=(obj[i].vtx[j].p[0]-u)*ct-(obj[i].vtx[j].p[2]-w)*st;
                y=(obj[i].vtx[j].p[0]-u)*st+(obj[i].vtx[j].p[2]-w)*ct;
                obj[i].vtx[j].p[0]=x+u;
                obj[i].vtx[j].p[2]=y+w;
            }
        }
    }
}

/*
void readObject(FILE *ffp) {

    FILE *fp;
    int    i,j,k,iv[4],iend;
    int io1,io2, nnf;
    char comd[24];
    char comment[80];
    char buf[256];
    Treald u,v,w,tu,tv,tw;
    int duplicate;
    char filename[80];

    fscanf(ffp,"%s",filename);
    printf("object file:%s\n",filename);

    fp=fopen(filename,"r");
    if(fp == NULL) {
        fprintf(stderr,"Can't open the file: %s\n",filename);
        exit(1);
    }

    nnf = 0;
    duplicate = FALSE;

    fgets(buf,256,fp);
    sscanf(buf,"%d%s",&nobj, comment);
    if(!strcmp(comment, "BACKFACE") || !strcmp(comment, "backface")) duplicate = TRUE;
    obj = (Tobject *)malloc(sizeof(Tobject)*nobj);
    nvtx=0;

    for(i=0; i<nobj; i++) {
        obj[i].tx = obj[i].ty = obj[i].tz = 0.0;
        fgets(buf,256,fp);
        sscanf(buf,"%d%d%lf%lf%lf%s",&obj[i].nv
                                ,&obj[i].nf
                                ,&obj[i].ref[0]
                                ,&obj[i].ref[1]
                                ,&obj[i].ref[2],comment);
        if(obj[i].ref[0] < 0.0 ||
           obj[i].ref[1] < 0.0 ||
           obj[i].ref[2] < 0.0) {
            obj[i].ref[0] *= -1.0;
            obj[i].ref[1] *= -1.0;
            obj[i].ref[2] *= -1.0;
        }
        if(obj[i].ref[0] > 1.0) obj[i].ref[0]/=100.;
        if(obj[i].ref[1] > 1.0) obj[i].ref[1]/=100.;
        if(obj[i].ref[2] > 1.0) obj[i].ref[2]/=100.;
        if(duplicate == TRUE)
            obj[i].face=(Tface *)malloc(sizeof(Tface)*obj[i].nf*2);
        else
            obj[i].face=(Tface *)malloc(sizeof(Tface)*obj[i].nf);

        obj[i].vtx=(Tvertex *)malloc(sizeof(Tvertex)*obj[i].nv);
        nvtx+=obj[i].nv;
        nnf += obj[i].nf;
        for(j=0; j<obj[i].nv; j++) {
             fgets(buf,256,fp);
             sscanf(buf,"%lf%lf%lf",&obj[i].vtx[j].p[0]
                                ,&obj[i].vtx[j].p[1]
                                ,&obj[i].vtx[j].p[2]);
        }

        for(j=0; j<obj[i].nf; j++) {
             fgets(buf,256,fp);
             sscanf(buf,"%d%d%d%d",&iv[0]
                                 ,&iv[1]
                                 ,&iv[2]
                                 ,&iv[3]);
             if(iv[2] == iv[3]) iv[3] = 0;
             iv[0]--;iv[1]--;iv[2]--;iv[3]--;
             for(k=0; k<4; k++) {
                 if(iv[k] >= 0)
                     obj[i].face[j].pv[k] = &(obj[i].vtx[iv[k]]);
                 else
                     obj[i].face[j].pv[k] = NULL;

             }
             if(duplicate == TRUE) {
                 if(iv[3] < 0) {
                     obj[i].face[j+obj[i].nf].pv[0] = &(obj[i].vtx[iv[2]]);
                     obj[i].face[j+obj[i].nf].pv[1] = &(obj[i].vtx[iv[1]]);
                     obj[i].face[j+obj[i].nf].pv[2] = &(obj[i].vtx[iv[0]]);
                     obj[i].face[j+obj[i].nf].pv[3] = NULL;
                 } else {
                     obj[i].face[j+obj[i].nf].pv[0] = &(obj[i].vtx[iv[3]]);
                     obj[i].face[j+obj[i].nf].pv[1] = &(obj[i].vtx[iv[2]]);
                     obj[i].face[j+obj[i].nf].pv[2] = &(obj[i].vtx[iv[1]]);
                     obj[i].face[j+obj[i].nf].pv[3] = &(obj[i].vtx[iv[0]]);
                 }
             }
        }
        if(duplicate == TRUE) {
            nnf += obj[i].nf;
            obj[i].nf *= 2;
        }
    }


//  *** CHANGE GEOMETRY **
    iend=1;
    while(iend) {
        fgets(buf,256,fp);
        sscanf(buf,"%s%d%d%lf%lf%lf%lf%lf%lf",comd,&io1,&io2
                                      ,&u,&v,&w,&tu,&tv,&tw);
        io1--; io2--;
        if(!strcmp(comd,"MOVE")) {
            for(i=io1; i<=io2; i++) {
                for(j=0; j<obj[i].nv; j++) {
                    obj[i].vtx[j].p[0]+=u;
                    obj[i].vtx[j].p[1]+=v;
                    obj[i].vtx[j].p[2]+=w;
                }
            }
        } else if(!strcmp(comd,"SCALE") || !strcmp(comd,"SCAL")) {
            for(i=io1; i<=io2; i++) {
                for(j=0; j<obj[i].nv; j++) {
                    obj[i].vtx[j].p[0]=(obj[i].vtx[j].p[0]-u)*tu+u;
                    obj[i].vtx[j].p[1]=(obj[i].vtx[j].p[1]-v)*tv+v;
                    obj[i].vtx[j].p[2]=(obj[i].vtx[j].p[2]-w)*tw+w;
                }
            }
        } else if(!strcmp(comd,"ZREV")) {
            Treald th,ct,st,x,y;
            th=(Treald)(tu*M_PI/180.0);
            ct=(Treald)cos(th); st=(Treald)sin(th);
            for(i=io1; i<=io2; i++) {
                for(j=0; j<obj[i].nv; j++) {
                    x=(obj[i].vtx[j].p[0]-u)*ct-(obj[i].vtx[j].p[1]-v)*st;
                    y=(obj[i].vtx[j].p[0]-u)*st+(obj[i].vtx[j].p[1]-v)*ct;
                    obj[i].vtx[j].p[0]=x+u;
                    obj[i].vtx[j].p[1]=y+v;
                }
            }
        } else if(!strcmp(comd,"XREV")) {
            Treald th,ct,st,x,y;
            th=(Treald)(tu*M_PI/180.0);
            ct=(Treald)cos(th); st=(Treald)sin(th);
            for(i=io1; i<=io2; i++) {
                for(j=0; j<obj[i].nv; j++) {
                    x=(obj[i].vtx[j].p[1]-v)*ct-(obj[i].vtx[j].p[2]-w)*st;
                    y=(obj[i].vtx[j].p[1]-v)*st+(obj[i].vtx[j].p[2]-w)*ct;
                    obj[i].vtx[j].p[1]=x+v;
                    obj[i].vtx[j].p[2]=y+w;
                }
            }
        } else if(!strcmp(comd,"YREV")) {
            Treald th,ct,st,x,y;
            th=(Treald)(tu*M_PI/180.0);
            ct=(Treald)cos(th); st=(Treald)sin(th);
            for(i=io1; i<=io2; i++) {
                for(j=0; j<obj[i].nv; j++) {
                    x=(obj[i].vtx[j].p[0]-u)*ct-(obj[i].vtx[j].p[2]-w)*st;
                    y=(obj[i].vtx[j].p[0]-u)*st+(obj[i].vtx[j].p[2]-w)*ct;
                    obj[i].vtx[j].p[0]=x+u;
                    obj[i].vtx[j].p[2]=y+w;
                }
            }
        } else if(!strcmp(comd,"END")) {
            iend=0;
        }
    }

    printf("No. of obj, No. of face: %d, %d\n", nobj, nnf);

    cal_normal();
}
*/

void cal_normal() {

    int i,j,k;
    Treald x1,y1,z1, x2,y2,z2;
    Treald da,db,dc,dd;


/* FACE NORMAL */
    for(i=0; i<nobj; i++) {
        for(j=0; j<obj[i].nf; j++) {
            x1=obj[i].face[j].pv[1]->p[0]-obj[i].face[j].pv[0]->p[0];
            y1=obj[i].face[j].pv[1]->p[1]-obj[i].face[j].pv[0]->p[1];
            z1=obj[i].face[j].pv[1]->p[2]-obj[i].face[j].pv[0]->p[2];

            x2=obj[i].face[j].pv[2]->p[0]-obj[i].face[j].pv[0]->p[0];
            y2=obj[i].face[j].pv[2]->p[1]-obj[i].face[j].pv[0]->p[1];
            z2=obj[i].face[j].pv[2]->p[2]-obj[i].face[j].pv[0]->p[2];

            da = -y1*z2+y2*z1;
            db = -z1*x2+z2*x1;
            dc = -x1*y2+x2*y1;
            dd=(Treald)sqrt(da*da+db*db+dc*dc);
            da /= dd;
            db /= dd;
            dc /= dd;
            if(dd < 0.0000001) {
                if(obj[i].face[j].pv[3] != NULL) {
                    x2=obj[i].face[j].pv[3]->p[0]-obj[i].face[j].pv[0]->p[0];
                    y2=obj[i].face[j].pv[3]->p[1]-obj[i].face[j].pv[0]->p[1];
                    z2=obj[i].face[j].pv[3]->p[2]-obj[i].face[j].pv[0]->p[2];

                    da = -y1*z2+y2*z1;
                    db = -z1*x2+z2*x1;
                    dc = -x1*y2+x2*y1;
                    dd=(Treald)sqrt(da*da+db*db+dc*dc);
                    da /= dd;
                    db /= dd;
                    dc /= dd;
                }
            }
/*
            if(dd < 0.0000001) {
                printf("%d %d: %f, %f, %f, %f\n", i, j, da, db, dc, dd);
                printf("%f, %f, %f\n", obj[i].face[j].pv[0]->p[0]
                                     , obj[i].face[j].pv[0]->p[1]
                                     , obj[i].face[j].pv[0]->p[2]);
                printf("%f, %f, %f\n", obj[i].face[j].pv[1]->p[0]
                                     , obj[i].face[j].pv[1]->p[1]
                                     , obj[i].face[j].pv[1]->p[2]);
                printf("%f, %f, %f\n", obj[i].face[j].pv[2]->p[0]
                                     , obj[i].face[j].pv[2]->p[1]
                                     , obj[i].face[j].pv[2]->p[2]);
                if(obj[i].face[j].pv[3] != NULL)
                                printf("%f, %f, %f\n", obj[i].face[j].pv[3]->p[0]
                                     , obj[i].face[j].pv[3]->p[1]
                                     , obj[i].face[j].pv[3]->p[2]);
                obj[i].face[j].pv[0] = NULL;
            }
*/
            if(obj[i].face[j].pv[0] != NULL) {
            dd = -( da*obj[i].face[j].pv[0]->p[0]
                   +db*obj[i].face[j].pv[0]->p[1]
                   +dc*obj[i].face[j].pv[0]->p[2]
                  );
            obj[i].face[j].normal[0] = da;
            obj[i].face[j].normal[1] = db;
            obj[i].face[j].normal[2] = dc;
            obj[i].face[j].normal[3] = dd;
            }
        }
    }

/* VERTEX NORMAL */
    int *count, *num_vtx, iv;
    Tvertex *vp;
    
    count = (int *)malloc(sizeof(int)*nvtx*10);
    num_vtx = (int *)malloc(sizeof(int)*nobj);
    for(i=0; i<nvtx; i++) count[i] = 0;
    num_vtx[0] = 0;
    for(i=1; i<nobj; i++) num_vtx[i] = num_vtx[i-1]+obj[i-1].nv;
    for(i=0; i<nobj; i++)
        for(j=0; j<obj[i].nv; j++)
            obj[i].vtx[j].normal[0] =
            obj[i].vtx[j].normal[1] =
            obj[i].vtx[j].normal[2] = 0.0;
    
    for(i=0; i<nobj; i++) {
        for(j=0; j<obj[i].nf; j++) {
            for(k=0; k<4; k++) {
                vp = obj[i].face[j].pv[k];
                if(vp != NULL) {
                    iv = vp - obj[i].vtx;
                    vp->normal[0] += obj[i].face[j].normal[0];
                    vp->normal[1] += obj[i].face[j].normal[1];
                    vp->normal[2] += obj[i].face[j].normal[2];
                    count[num_vtx[i]+iv] += 1;
                }
            }
        }
    }
    

    for(i=0; i<nobj; i++) {
        for(j=0; j<obj[i].nv; j++) {
            k = count[num_vtx[i]+j];
            if(k != 0) {
                obj[i].vtx[j].normal[0] /= (Treald)k;
                obj[i].vtx[j].normal[1] /= (Treald)k;
                obj[i].vtx[j].normal[2] /= (Treald)k;
            } else {
                printf("*** error. no faces are associated. obj %d, vtx %d\n", i, j);
            }
        }
    }

    free(count);
    free(num_vtx);
}

//慣性テンソルはバウンディングボックスの楕円体を用いる
void cal_inertia_tensor(ThapticObj *po) {

    int io;
    Trealf x, y, z;
    
    po->I[0][0] = po->I[0][1] = po->I[0][2] =
    po->I[1][0] = po->I[1][1] = po->I[1][2] =
    po->I[2][0] = po->I[2][1] = po->I[2][2] = 0.0;
    io = po->id_obj;
    if(io >= 0) {
        x = obj[io].box[1][0] - obj[io].box[0][0];
        y = obj[io].box[1][1] - obj[io].box[0][1];
        z = obj[io].box[1][2] - obj[io].box[0][2];
        po->I[0][0] = 2.0/5.0*y*z*po->im.x;
        po->I[1][1] = 2.0/5.0*z*x*po->im.y;
        po->I[2][2] = 2.0/5.0*x*y*po->im.z;
        po->pos.x = (obj[io].box[1][0] - obj[io].box[0][0])/2.;
        po->pos.y = (obj[io].box[1][1] - obj[io].box[0][1])/2.;
        po->pos.z = (obj[io].box[1][2] - obj[io].box[0][2])/2.;
    }
}

void cal_minmax_obj() {

    int i,j;

    objBox[0][0] = objBox[0][1] = objBox[0][2] = 10000000000000.0f;
    objBox[1][0] = objBox[1][1] = objBox[1][2] =-10000000000000.0f;

    for(i=0; i<nobj; i++) {
        obj[i].box[0][0] = obj[i].box[0][1] = obj[i].box[0][2] = 100000000000.0;
        obj[i].box[1][0] = obj[i].box[1][1] = obj[i].box[1][2] = -100000000000.0;
        for(j=0; j<obj[i].nv; j++) {
            if(objBox[0][0] > obj[i].vtx[j].p[0]) objBox[0][0] = obj[i].vtx[j].p[0];
            if(objBox[0][1] > obj[i].vtx[j].p[1]) objBox[0][1] = obj[i].vtx[j].p[1];
            if(objBox[0][2] > obj[i].vtx[j].p[2]) objBox[0][2] = obj[i].vtx[j].p[2];

            if(objBox[1][0] < obj[i].vtx[j].p[0]) objBox[1][0] = obj[i].vtx[j].p[0];
            if(objBox[1][1] < obj[i].vtx[j].p[1]) objBox[1][1] = obj[i].vtx[j].p[1];
            if(objBox[1][2] < obj[i].vtx[j].p[2]) objBox[1][2] = obj[i].vtx[j].p[2];

            if(obj[i].box[0][0] > obj[i].vtx[j].p[0]) obj[i].box[0][0] = obj[i].vtx[j].p[0];
            if(obj[i].box[0][1] > obj[i].vtx[j].p[1]) obj[i].box[0][1] = obj[i].vtx[j].p[1];
            if(obj[i].box[0][2] > obj[i].vtx[j].p[2]) obj[i].box[0][2] = obj[i].vtx[j].p[2];

            if(obj[i].box[1][0] < obj[i].vtx[j].p[0]) obj[i].box[1][0] = obj[i].vtx[j].p[0];
            if(obj[i].box[1][1] < obj[i].vtx[j].p[1]) obj[i].box[1][1] = obj[i].vtx[j].p[1];
            if(obj[i].box[1][2] < obj[i].vtx[j].p[2]) obj[i].box[1][2] = obj[i].vtx[j].p[2];
        }
        printf("obj %d bbox size:", i);
        printf("(%8.4f %8.4f %8.4f)\n"
            ,obj[i].box[1][0]-obj[i].box[0][0], obj[i].box[1][1]-obj[i].box[0][1], obj[i].box[1][2]-obj[i].box[0][2]);
//            ,obj[i].box[1][0], obj[i].box[1][1], obj[i].box[1][2]);
    }
}


void readPatch(FILE *fp, int *nvtx, int no1, int no2, int type) {

    int i, j, i1, i2, i3, i4;
    int iend;
    char buf[256];
    Treald u,v,w,tu,tv,tw;
    int io1,io2;
    char comd[5];

    for(i = no1; i<no2; i++ ){
        obj[i].type = type;
        obj[i].id_tex = -1;
        obj[i].fog = FALSE;
        obj[i].shaded = TRUE;
        fscanf(fp, "%d%d", &obj[i].nv, &obj[i].nf);
        obj[i].ref[0] = obj[i].ref[1] = obj[i].ref[2] = 1.0;
        obj[i].face=(Tface *)malloc(sizeof(Tface)*obj[i].nf);
        obj[i].vtx=(Tvertex *)malloc(sizeof(Tvertex)*obj[i].nv);
        nvtx+=obj[i].nv;
        for(j=0; j<obj[i].nv; j++) {
            fscanf(fp, "%lf%lf%lf%lf%lf%lf",
                 &obj[i].vtx[j].p[0], &obj[i].vtx[j].p[1], &obj[i].vtx[j].p[2], 
                 &obj[i].vtx[j].lumi[0], &obj[i].vtx[j].lumi[1], &obj[i].vtx[j].lumi[2]);
obj[i].vtx[j].lumi[0] *= 1.0;
obj[i].vtx[j].lumi[1] *= 1.0;
obj[i].vtx[j].lumi[2] *= 1.0;
obj[i].vtx[j].lumi[0] += 0.2;
obj[i].vtx[j].lumi[1] += 0.2;
obj[i].vtx[j].lumi[2] += 0.2;
        }
        for(j=0; j<obj[i].nf; j++) {
            fscanf(fp, "%d%d%d%d", &i1, &i2, &i3, &i4);
            obj[i].face[j].pv[0] = &(obj[i].vtx[i1]);
            obj[i].face[j].pv[1] = &(obj[i].vtx[i2]);
            obj[i].face[j].pv[2] = &(obj[i].vtx[i3]);
            obj[i].face[j].pv[3] = &(obj[i].vtx[i4]);
        }
    }
/*** CHANGE GEOMETRY ***/
    iend=1;
    while(iend) {
        fgets(buf,256,fp);
        sscanf(buf,"%s%d%d%lf%lf%lf%lf%lf%lf",comd,&io1,&io2
                                      ,&u,&v,&w,&tu,&tv,&tw);
        io1--; io2--; io1 += no1; io2 += no1;
        if(!strcmp(comd,"END")) {
            iend=0;
        } else {
            transObj(comd, io1, io2, u, v, w, tu, tv, tw);
        }
    }
}
