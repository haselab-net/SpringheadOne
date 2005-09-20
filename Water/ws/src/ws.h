#define MAXWIDTH 8192
// #define ERROR    (-1)
#define ODD      1
#define EVEN     0
#define FALSE    0
#define TRUE     1
#define ABS(a) ((((a) >= 0)) ? (a) : -(a))
#define MAX(a,b) (((a) >= (b)) ? (a) : (b))
#define MIN(a,b) (((a) >= (b)) ? (b) : (a))
#define M_PI     3.1415926
#define MAX_NODES 5
#define ROW       3
//#define MX       40
//#define MY       40
//#define DT       0.13
//#define DH       1.0

#define IX(i,j) ((i)+(MX)*(j))

#define FRAME_RATE 30.0

#define DYNAMIC_OBJ 1
#define STATIC_OBJ 2

#ifdef GLOBAL_MAIN
#define GLOBAL 
#else
#define GLOBAL extern
#endif

typedef float Trealf;
typedef double Treald;

typedef struct{
    Trealf r, g, b;
}Tfimg;

typedef struct {
    int    x, y;
} Tpoint2i;

typedef struct {
    int    x, y, z;
} Tpoint3i;

typedef struct {
    Trealf    x, y;
} Tpoint2f;

typedef struct{
    Trealf r, g, b;
}Tcolor3f;

typedef struct {
    Trealf r,g,b,a;
} Tpixelf;

typedef struct {
    Treald r,g,b;
} Tcolor3d;

typedef struct {
    Treald r,g,b,a;
} Tpixeld;

typedef struct {
    unsigned char r, g, b, a;
} Tpixelub;


typedef struct {
    Treald x,y,z;
} Tpoint3d;

typedef struct {
    Trealf x,y,z;
} Tpoint3f;

typedef struct {
    Trealf x, y;
    Trealf zs, ze;
    Trealf as, ae;
    Tpoint3f ns, ne;
} Tzspan;

typedef struct {
    Treald p[3];       /* point */
    Treald lumi[3];   /* luminance */
    Treald normal[3];
} Tvertex;

typedef struct {

    Treald p[3];
    Treald dir[3];
} Tray;

typedef struct {
    Treald r, g, b;
    Treald xdir, ydir, zdir;
    Treald theta, phi;
    int active;
} Tsun;

typedef struct {
    Tvertex *pv[4];    /* pointer to f_vtx */
    Treald   normal[4];  /* normal vector */
    Tpoint2f *tex[4]; /* pointer to tex. coored */
} Tface;

typedef struct {
    Tpixelub *buf;
    int xs, ys;
    int name;
} TtextureRGBA;

typedef struct {
    Treald vx, vy, vz;
    Treald rx, ry, rz;
    Treald vr, the, phi, roll_ang;
    Treald fovy;
    int    key_frame;
} Tview;

typedef struct {
    Treald refe[3];
    Treald view[3];
    Treald vpol[3];
    Treald vr[3];
    Treald vang[2];
    Treald rvr,ct,cf,st,sf;
    Treald ax,ay;
    Treald dxp,dyp;
    Treald fovy;
    Treald aspect;
    int    winX, winY;
    Treald rot[3][3];
    Tview init_view;
} Tcamera;

typedef struct {
    Tpoint3d lp, rp;
    Treald   dis, amb;
    Tpoint3d dir_l, dir_r;
} Tear;

typedef struct {
    int  frq;
    int  bit;
    int  stereo;
    int  nsmp;
    int  ndata;
    Trealf *wave;
} Tsound;

typedef struct {
    Trealf p0[3];
    Trealf n0[3];
    Trealf p[3];  // point;
    Trealf n[3];  // normal
    Trealf s;   // area
    int    i_hsrc; // nearest haptic source
} TsamplePoint;

typedef struct {
    Tpoint3f *pv[4];
    Treald area;
} Tpatch;

typedef struct {
    Treald   dlen;
    int      nu, nv;
    int      np;
    Tpoint3f uvec, vvec, ori;
    Treald   lenu, lenv;
    Tpoint3f *vp;
    unsigned char      *share_cnt;
    Tpatch *patch;
} Tmesh;

typedef struct {
    Trealf *xprs, *yprs, *zprs;
    Trealf *xfri, *yfri, *zfri;
    Trealf v;
    Tpoint3f dir;
    Trealf   the, phi;
    int     n;
    int     rate;
    Tpoint3f  peri_prs, peri_fri; //周期
    Tpoint3f  phas_prs, phas_fri; //位相
} TfluidForceTex;

typedef struct {
    Trealf dthe, dphi;
//    Trealf the0, the1, phi0, phi1;
    int    nthe, nphi, ntex;
    Trealf v0;
    TfluidForceTex *frc;
} TforceSet;

typedef struct {
    Trealf x0, y0, z0;
    Trealf x, y, z;
    Trealf nx, ny, nz;
    Trealf x_ori, y_ori, z_ori;
    Trealf vx, vy, vz;
    Trealf v;
    Tpoint3f prs;
    Trealf tcoord;
    int    ntex;
    int    *itex;
    TforceSet *fset;
    Tpoint3i sym;   // symmetry flag;
    int    n_sp; // number of sample points attached to this haptic source
    Trealf a; // ratio of sample points under water
    Tcolor3f col; // color ID, used for display only;
} ThapticSource;

typedef struct {
    Tpoint3f prev_pos;
    Tpoint3f pos;
    Trealf m[16]; //initial position and orientation
    Trealf   ori[4]; // Quaternion
    Tpoint3f a, aa, amom, vel, we; // acceleration and velocity, angular velocity
    Tpoint3f force;
    Tpoint3f torq;
    Tpoint3f fpos; //力の作用点
    int      n_fpos;
    Trealf   unit_mass, mass;
    Tpoint3f   im; //慣性モーメント(inertial moment)
    Trealf   I[3][3];
    Trealf   v, p_area, vmax;
    Trealf   volume, area;
    int      id_obj;
    TsamplePoint *sp;
    int           n_sp;
    Tmesh *mesh; // mesh for each face of pointer object;
    
    int     nhsrc;       // no. of point haptic sources
    ThapticSource *hsrc; // point haptic source
    
    Trealf disp_scl; // scale for force display
    
    Trealf buo_scl;
    Trealf pres_scl;
    Trealf fric_scl;
    Trealf vel_scl;
    Trealf wz, wa; //parameters for generating waves
    Trealf loss; //temporary for boat
} ThapticObj;

typedef struct {
    Tpoint3f pos;
    Trealf   rot[9];
    int      left_oar, right_oar;
} TspidarStatus;

typedef struct {
    int id_obj;
    Trealf s, r0, rr0;  // ばね係数と静止状態の長さ
    Trealf length;   // ロッドの長さ
    Tpoint3f lure; //ルアーに糸をつなぐ位置(ルアー中心からの相対位置）
    Tpoint3f lure_cur; //ルアーの現在位置
    Tpoint3f pos;    //先端の位置
    Tpoint3f tension;
    Trealf heso, ude;   //へその位置（ロッドの回転中心）と腕の長さ
    Trealf lure_view_display;
} Trod;             //釣竿用の構造体

/*==================================
      Data Structure for Objects 
==================================*/

typedef struct {
    Tpoint3d min, max;
} Tbbox;

/*----------------------- OBJECT ------------------------*/
typedef struct {
    int nf;         /* num of face */
    int nv;         /* num of vertex */
    Treald ref[3];      /* reflectance */
    Trealf spec[4], diffuse[4], shin; // parameters for opengl lighting
    Tface *face;     /* pointer to face */
    Tvertex *vtx;
    Treald box[2][3];
    int shaded;
    int fog;
    int type; // object type ( DYNAMIC OR STATIC )
    int stencil;  // stencil usage (TRUE: use, FALSE: normal)
    int id_tex;
    Trealf tex_scl;
    int       ntex_coords, textured;
    int transparent;
    Tpoint2f *tex_coords;
    int nssrc;
    Trealf m[16];
    Trealf rigidity; // used for haptic
} Tobject;

    
/*----------------- Quad Stripped Object-----------------*/
typedef struct {
    int nu, nv, npoints;
    Tpoint3d *pnt;
    Tcolor3d *col;
    Tpoint3d *points;
    Tcolor3d point_col;
} Tquads;

typedef struct {
    unsigned char      bfType[2];
    unsigned long int  bfSize;
    unsigned short int bfReserved1;
    unsigned short int bfReserved2;
    unsigned long int  bfOffBits;
} BMPFILEHEADER;

typedef struct {
    unsigned long int  biSize;
    unsigned long int  biWidth;
    unsigned long int  biHeight;
    unsigned short int biPlanes;
    unsigned short int biBitCount;
    unsigned long int  biCompression;
    unsigned long int  biSizeImage;
    unsigned long int  biXPelsPerMeter;
    unsigned long int  biYPelsPerMeter;
    unsigned long int  biClrUsed;
    unsigned long int  biClrImportant;
} BMPINFOHEADER;

enum Screen{
	CENTER_SCREEN,
	LEFT_SCREEN,
	RIGHT_SCREEN,
};
