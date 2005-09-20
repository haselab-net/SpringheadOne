#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <SWIFT.h>
#include <SWIFT_Object.h>
#include <GL/glut.h>
#include <Base/Affine.h>

using namespace LVRM;

SWIFT_Scene* scene;
int id1,id2,id3;
float lookx = 5.0;
float looky = 5.0;
float lookz = 5.0;
GLdouble vo[3] = {0.0, 0.0, 0.0};
GLdouble vx[3] = {10.0, 0.0, 0.0};
GLdouble vy[3] = {0.0, 10.0, 0.0};
GLdouble vz[3] = {0.0, 0.0, 10.0};

static const char* object1_filename = "GoalFrame.poly";

SWIFT_Real vs[] =
{
    0,  0,  0,
    0.1,  0,  0,
	0,  0,  0.1
};

int fs[] = 
{
	0, 2, 1,
	0, 1, 2
};

static const int vn = 3;
static const int fn = 2;

GLdouble vertex1[][3] = 
{
	{0, 0, 0},
    {0.1, 0, 0},
	{0, 0, 0.1}
};

int edge1[][2] =
{
	{0,1},
	{1,2},
	{2,0}
};	
/*
static const SWIFT_Real vs2[] = 
{
	3.0, 3.0, 3.0,
	4.0, 3.0, 3.0,
	3.0, 4.0, 3.0,
	3.0, 3.0, 2.0
};

static const int fs2[] =
{
    0, 2, 1,
    0, 1, 3,
	0, 3, 2,
	1, 2, 3
};

static const int vn2 = 4;
static const int fn2= 4;

GLdouble vertex2[][3] = 
{
	{3.0,  3.0,  3.0},
    {4.0,  3.0,  3.0},
    {3.0,  4.0,  3.0},
    {3.0,  3.0,  2.0}
};

int face[][3] = {
	{0, 2, 1},
    {0, 1, 3},
	{0, 3, 2},
	{1, 2, 3}
};

static const SWIFT_Real vs3[] = 
{
	-1.0, 1.0, 1.0,
	0.0, 0.0, 3.0,
	-5.0, 3.0, 2.0,
	-2.0, 4.0, 2.0
};

static const int fs3[] =
{
    0, 1, 2,
    0, 3, 1,
	0, 2, 3,
	1, 3, 2
};

static const int vn3 = 4;
static const int fn3= 4;

GLdouble color[][3] = 
{
	{1.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 0.0, 1.0},
	{0.1, 0.4, 0.8}
};
*/
void Initialize_Scene()
{
	scene = new SWIFT_Scene(true,false);
	if( !scene->Add_Convex_Object( vs, fs, vn, fn, id1, false )) {
        cerr << "Adding object1 failed -- Exiting..." << endl;
        exit( -1 );
    } else {
        cerr << "Added object1 to scene" << endl;
    }
	
	if( !scene->Add_General_Object( object1_filename, id2, false ) ) {
        cerr << "Adding object2 failed -- Exiting..." << endl;
        exit( -1 );
    } else {
        cerr << "Added object2 to scene" << endl;
    }


	/*
	if( !scene->Add_Convex_Object( vs2, fs2, vn2, fn2, id2, false )) {
        cerr << "Adding object2 failed -- Exiting..." << endl;
        exit( -1 );
    } else {
        cerr << "Added object1 to scene" << endl;
    }
	
	if( !scene->Add_Convex_Object( vs3, fs3, vn3, fn3, id3, false )) {
        cerr << "Adding object3 failed -- Exiting..." << endl;
        exit( -1 );
    } else {
        cerr << "Added object1 to scene" << endl;
    }
	*/
	scene->Deactivate( );
    scene->Activate(id1);
}

void Run()
{
	int np,i,count;
    int* oids;
	int* nc;
    SWIFT_Real* dists;
	SWIFT_Real* npts;
	SWIFT_Real* norm;
	int* ftypes;
	int* fids;

	scene->Query_Contact_Determination(false,  SWIFT_INFINITY, np, &oids, &nc, &dists, &npts,
		&norm, &ftypes, &fids);
	count=0;
	while(abs(*(nc+count))<100) {      //仮の閾値
		count++;
	};
		
	cout << "オブジェクト数＝" << np << endl;
	for(i=0;i<count;i++){
		cout << "オブジェクト対＝(" << *(oids+2*i)<<","<<*(oids+2*i+1) << ")" << endl;
		cout << "最短距離＝" << *(dists+i) << endl;
		cout << "最近点(１)＝(" << *(npts+6*i)<<","<<*(npts +6*i+1)<<","<<*(npts+6*i+2)<<")"<<endl;
		cout << "最近点(２)＝(" << *(npts+6*i+3)<<","<<*(npts +6*i+4)<<","<<*(npts+6*i+5)<<")"<<endl;
		cout << "方向ベクトル＝("<< *(norm+3*i)<<","<<*(norm +3*i+1)<<","<<*(norm+3*i+2)<<")"<<endl;
	}
}

void init()
{
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SWIFT_Array<SWIFT_Object*>& objs = scene->Objects();

	glColor3f(1.0, 1.0, 1.0);
	for(int i=0; i<objs.Length(); ++i){
		Affinef af;
		af.Rot() = *(Matrix3d*)(objs[i]->Transformation().Rotation().Value());
		af.Pos() = *(Vec3d*)(objs[i]->Transformation().Translation().Value());
		glPushMatrix();
		glMultMatrixf(af);
		SWIFT_Array<SWIFT_Tri_Face>& faces = objs[i]->Mesh()->Faces();
		for(int f=0; f<faces.Length(); ++f){
			glBegin(GL_TRIANGLES);
			faces[f].Draw();
			glEnd();
		}
		glPopMatrix();
	}

/*	for(j=0;j<4;j++){
		glColor3dv(color[j]);
		for(i=0;i<3;i++){
			glVertex3dv(vertex2[face[j][i]]);
		}
	}*/	
	
	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3dv(vo);
		glVertex3dv(vx);
		glColor3d(0.0, 1.0, 0.0);
		glVertex3dv(vo);
		glVertex3dv(vy);
		glColor3d(0.0, 0.0, 1.0);
		glVertex3dv(vo);
		glVertex3dv(vz);
	glEnd();
	glFlush();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lookx, looky, lookz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}

main(int argc, char *argv[])
{
	Initialize_Scene();
	Run();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	init();
	glutMainLoop();
	return 0;	
}