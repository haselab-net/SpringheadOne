#include <Base/Affine.h> 
#include <Haptic/HDSpidar4quad.h>
#include <Device/DRContecIsaDa.h>
#include <Device/DRContecIsaCounter.h>
#include <WinBasis/WBMMTimer.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <SWIFT.h>
#include <GL/glut.h>
#include <SWIFT_Object.h>

using namespace LVRM;
HDSpidar4quad spidar;

int id1,id2;
SWIFT_Scene* scene;
DVDeviceManager devMan;
static const char* object1_filename = "Cube12Polygons.chr";
Vec3f pos;

float lookx = 1.5;
float looky = 0.0;
float lookz = 0.0;
GLdouble glSnear[3];
GLdouble glOnear[3];
GLfloat light0pos[] = {0.0, 2.0 ,0.0, 1.0};
GLfloat light1pos[] = {1.0, 0.0 ,0.0, 1.0};
GLdouble vo[3] = {0.0, 0.0, 0.0};
GLdouble vx[3] = {10.0, 0.0, 0.0};
GLdouble vy[3] = {0.0, 10.0, 0.0};
GLdouble vz[3] = {0.0, 0.0, 10.0};

void SpidarInit()
{	
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.Init();
	
	std::cerr << devMan;
	
    //スパイダー初期化
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	spidar.Init(devMan, motorPos[0], 0.5776f, 2.924062107079e-5f, 0.5f, 20.0f);

	spidar.Calib();
}

void Initialize_Scene()
{
	scene = new SWIFT_Scene(true,false);

	static const int vn = 4;
    static const int fn = 4;
	static const SWIFT_Real vs[] =
	{
		0, 0, 0,
		0.05, 0, 0,
		0, 0.05, 0,
		0, 0, 0.05
	};
	static const int fs[] = 
	{
		0, 2, 1,
		0, 1, 3,
		0, 3, 2,
		1, 2, 3
	};

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
	scene->Deactivate();
	scene->Activate(id1);
	
};
	
WBMMTimer timer;
void timerFunc(void* arg)
{
	int np,i;
    int* oids;
	int* nc;
    SWIFT_Real* dists;
	SWIFT_Real* npts;
	SWIFT_Real* norm;
//	int* ftypes;
//	int* fids;

	const float dlimit = 0.05;
	const float k = 100;
	const float re = 0.2;
 
	SWIFT_Real R[9];
	SWIFT_Real T[3];
	spidar.Update();
	pos = spidar.GetPos();

	R[0]=1.0; R[1]=0.0; R[2]=0.0;
	R[3]=0.0; R[4]=1.0; R[5]=0.0;
	R[6]=0.0; R[7]=0.0; R[8]=1.0;

	T[0]=pos.X(); T[1]=pos.Y(); T[2]=pos.Z();

	scene->Set_Object_Transformation(id1,R,T);

	scene->Query_Contact_Determination(false, SWIFT_INFINITY, np, &oids, &nc ,&dists, 
		&npts, &norm, NULL, NULL);
	
	float d = *(dists);
	int num = 0;
	
	for(i=1;i<np;i++){
		if (d > *(dists+i)){
			 d = *(dists+i);
			 num = i;
		}
	}
	Vec3f force;
	if(d < dlimit){
		if(d==-1) d = 0.0;
		Vec3f vd,vunit;
		vd.X() = *(norm+3*num);
		vd.Y() = *(norm+3*num+1);
		vd.Z() = *(norm+3*num+2);
		vunit = vd.unit();
		force = k*(dlimit-d)*vunit;
	}
	spidar.SetForce(force,re);
	glSnear[0] = *(npts+6*num) + T[0];
	glSnear[1] = *(npts+6*num+1) + T[1];
	glSnear[2] = *(npts+6*num+2) + T[2];
	glOnear[0] = *(npts+6*num+3);
	glOnear[1] = *(npts+6*num+4);
	glOnear[2] = *(npts+6*num+5);

	cerr << "オブジェクト対＝(" << *(oids+2*num)<<","<<*(oids+2*num+1) << ")" << endl;
	cerr << "最短距離＝" << *(dists+num) << endl;
	cerr << "最近点(１)＝(" << glSnear[0] <<","<< glSnear[1] <<","<< glSnear[2] <<")"<<endl;
	cerr << "最近点(２)＝(" << glOnear[0] <<","<< glOnear[1] <<","<< glOnear[2] <<")"<<endl;
	cerr << "方向ベクトル＝("<< *(norm+3*num)<<","<<*(norm +3*num+1)<<","<<*(norm+3*num+2)<<")"<<endl;
/*
	cout << "最短距離＝" << *(dists+num) << endl;
	cout << "force=("  << force.X() 
		<< ", " << force.Y()
		<< ", " << force.Z()
		<<") "   << endl;
*/
}

void init()
{
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

void display()
{
	glLoadIdentity();
	gluLookAt(lookx, looky, lookz, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light0pos);
	SWIFT_Array<SWIFT_Object*>& objs = scene->Objects();
	
	for(int i=0; i<objs.Length(); ++i){
		Affinef af;
		af.Rot() = *(Matrix3d*)(objs[i]->Transformation().Rotation().Value());
		af.Pos() = *(Vec3d*)(objs[i]->Transformation().Translation().Value());
	glPushMatrix();
		glMultMatrixf(af);
		SWIFT_Array<SWIFT_Tri_Face>& faces = objs[i]->Mesh()->Faces();
	//	objs[i]->Mesh()->Verify();
//		float emit[] = {1.0f, 0.1f, 0.1f, 1.0f};
//		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emit);
		for(int f=0; f<faces.Length(); ++f){
			glBegin(GL_TRIANGLES);
			faces[f].Draw();
			glEnd();
		}
		glPopMatrix();
	}

	glBegin(GL_LINES);
		glColor3d(1.0, 0.0, 0.0);
		glVertex3dv(glSnear);
		glVertex3dv(glOnear);
	glEnd();

	glBegin(GL_LINES);
		glColor3d(0.0, 0.0, 0.0);
		glVertex3dv(vo);
		glVertex3dv(vx);
		glVertex3dv(vo);
		glVertex3dv(vy);
		glVertex3dv(vo);
		glVertex3dv(vz);
	glEnd();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key){
	case'x':
		lookx += 0.1;
		break;
	case'X':
		lookx -= 0.1;
		break;
	case'y':
		looky += 0.1;
		break;
	case'Y':
		looky -= 0.1;
		break;
	case'z':
		lookz += 0.1;
		break;
	case'Z':
		lookz -= 0.1;
		break;
	case'r':
		lookx=2,looky=0;lookz=0;
		break;
	}
}

void main(int argc, char *argv[])
{
	SpidarInit();
	Initialize_Scene();
	timer.Set(timerFunc, NULL);
	timer.Resolution(50);
	timer.Interval(50);
	timer.Create();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	init();	
	glutMainLoop();
}