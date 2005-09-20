#include <Base/Env.h>
#include <Solid/SOLID.h>
#include <VertexBase.h>
#include <Polyhedron.h>
#include <Object.h>

#include "GeoNode.h"
#include "GLView.h"
#include <GL/glut.h>
#include <crtdbg.h>
#include <strstream>
#include <penalty.h>
#include "communication.h"
using namespace std;

#define KEY_ESC -27
const float floorHeight = 0.0f;
const float dt = 1.0f / 30.0f;
const float gravity = 9.8f;
const float elasticity = 0.8f;
//const float elasticity = 0.0f;

class CmdEye:public Command{
public:
	TGLView* view;
	CmdEye(TGLView* v):view(v),Command(&(v->cmdSocket)){}
	int Id()const{ return 1; }
	int Receive(char* data){
		Affinef& af = *(Affinef*)data;
		view->afBody = af;
//		cout << af << endl;
		view->Invalidate();
		return sizeof(af);
	}
	void Send(){
		SendId();
//		cout << view->afBody << endl;
		SendData(&view->afBody, sizeof(view->afBody));
		cmdSocket->Flush();
	}
};
bool TGLView::LoadSettings(){
	char* cname = getenv("COMPUTERNAME");
	char* p = cname;
	while(*p && !isdigit(*p)) p++;
	int cpNum = -1;
	if (strlen(p)) cpNum = atoi(p);
	if (cpNum == -1) return false;
	std::map<int,int> cpProj;
	cpProj[0]=0;
	cpProj[1]=1;
	cpProj[2]=2;
	cpProj[3]=3;
	cpProj[4]=0;
	cpProj[5]=1;
	cpProj[6]=2;
	cpProj[7]=3;

	cpProj[11]=8;
	cpProj[20]=9;
	cpProj[21]=10;
	cpProj[15]=11;
	cpProj[10]=12;
	cpProj[14]=13;
	cpProj[13]=14;
	cpProj[17]=15;
	
	cpProj[12]=16;
	cpProj[30]=17;
	cpProj[31]=18;
	cpProj[16]=19;
	cpProj[22]=9;
	cpProj[23]=10;
	cpProj[32]=17;
	cpProj[33]=18;

	int projNum = cpProj[cpNum];
	if(projNum <= 3){
		aspect_x = 5.0f;
		aspect_y = 4.0f;
	}else{
		aspect_x = 4.0f;
		aspect_y = 3.0f;
	}
	char filename[1024];
	ostrstream ostrfn(filename, sizeof(filename));
	ostrfn << "sf" << projNum << '\0';

	FILE *fp;
	int number;
	int set = 0;
	char ch[128];
    
    if((fp = fopen(filename, "r")) == NULL){
        cout << "Can not read or find file sf1" << endl;
 		return false;
   }
    fgets(ch,sizeof(ch),fp);

	std::istrstream cStrStrm(ch, sizeof(ch));

//	cout << ch <<endl;
	cStrStrm >> number >> setting[0] >> setting[1] >> setting[2] >> setting[3]
		>> setting[4] >> setting[5] >> setting[6];
//	cout << number << endl;
//        	for(int j=0;j<6;j++)
//	cout << setting[j] << endl;
    fclose(fp);
    return true;
}
TGLView::TGLView(){
	radius = 0.5f;
	afBody = Affinef(0, 0, 10);
	afBody.LookAtGL(Vec3f(0, 0, 0), Vec3f(0,1,0));
	if (LoadSettings()) SetParam();
	cmdSocket.Register(new CmdEye(this));
}
void TGLView::SetParam(){
	double angleA,angleB,angleC;
	angleB = asin(setting[1]);
	angleA = asin(setting[0]/cos(angleB));
	angleC = setting[5]/180 * M_PI;
	Affinef Rzx,Rzy,Rxy;
	Rzx.Ex() = Vec3f(cos(angleA),0,-sin(angleA));
	Rzx.Ey() = Vec3f(0,1,0);
	Rzx.Ez() = Vec3f(sin(angleA),0,cos(angleA));
	Rzy.Ex() = Vec3f(1,0,0);
	Rzy.Ey() = Vec3f(0,cos(angleB),-sin(angleB));
	Rzy.Ez() = Vec3f(0,sin(angleB),cos(angleB));
	Rxy.Ex() = Vec3f(cos(angleC),sin(angleC),0);
	Rxy.Ey() = Vec3f(-sin(angleC),cos(angleC),0);
	Rxy.Ez() = Vec3f(0,0,1);
//	cout << angleB ;
	R = Rzx*Rzy*Rxy;
	fov_v = setting[3] * 2;
}
bool TGLView::OnTimer(){
	Step();
	return true;
}
void TGLView::OnDraw(){
	glClearColor(clearColor.X(), clearColor.Y(), clearColor.Z(), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	static Affinef R180(Rad(180),'y');
	
	Affinef afProjector = afBody * R180 * R * R180;
	
	Affinef afCam = (afProjector * afEye).inv();
	glLoadMatrixf(afCam);
//	glLoadMatrixf((afBody * afEye).inv());
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->Draw();
//		(*it)->DrawGeometry();
	}
	if (bDrawCollision){
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		penChekers.Draw();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	if (bDrawNormal){
		glEnd();
		glLoadIdentity();
		Vec3f pos(1,1,-8);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);
		for(int i=0; i<normals.size(); ++i){
			glBegin(GL_LINES);
			glVertex3fv(pos);
			glVertex3fv(pos + normals[i].unit());
			glEnd();
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}

	Flip();
}

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov_v , aspect_x / aspect_y , 0.1f , 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	switch(key){
	case '\x1b':
		Exit();
		break;
	case 'n':
		bDrawNormal = !bDrawNormal;
		break;
	case 'c':
		bDrawCollision = !bDrawCollision;
		break;
	case 'b':
		glutPostRedisplay();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	default:
	break;
	}
}

void TGLView::OnMouse(Button button, bool on, int x, int y)
{
	mouseButton = button;
	mouseX = x;	mouseY = y;
	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}
}

void TGLView::OnMouseMove(bool on, int x, int y)
{
	switch(mouseButton){
	case LEFT:
		Invalidate();
		break;		
	case MIDDLE:
		Invalidate();
		break;
	case RIGHT:
		Invalidate();
		break;
	}
	mouseX = x;	mouseY = y;
}
void TGLView::OnOpen(void){
	//	GLの初期化
	float	lmamb[] = {.7, .7, .7, 1},
			l0amb[] = {0, 0, 0, 1},
			l0dif[] = {1, 1, 1, 1},
			l0spc[] = {1, 1, 1, 1},
			l0pos[] = {0, 0, 10, 0};

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);

	SetTimer(33, &TGLView::OnTimer);
	bDrawNormal = false;
}

typedef map<DtObjectRef, Object *> ObjectList;
extern ObjectList objectList;
void TGLView::InitCollisionDetector(){
	//	SOLIDの初期化
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		TGeoNode::TVertices& vtxs = (*it)->vertices;
		if (!(*it)->faces.size()) continue;
		
		DtShapeRef shape = dtNewComplexShape();
		for(int i=0; i<(*it)->faces.size(); ++i){
			TGeoNode::TFace& face = (*it)->faces[i];
			dtBegin(DT_POLYGON);
			for(int v=0; v<face.size(); ++v){
				dtVertex(vtxs[face[v]].X(), vtxs[face[v]].Y(), vtxs[face[v]].Z());
			}
			dtEnd();
		}
		dtEndComplexShape();
		dtCreateObject(*it, shape); 
		dtLoadMatrixf((*it)->GetPosture());
	}
	DtShapeRef shape = dtSphere(radius);	//	小さな球
	dtCreateObject(&afBody, shape);

	//	チェックすべきオブジェクトの追加
	for (ObjectList::const_iterator i = objectList.begin(); i != objectList.end(); ++i) {
		if (i->first == &afBody) continue;
		TPenaltyChecker pc(i->second, objectList[&afBody]);
		penChekers.push_back(pc);
	}
}
void TGLView::Step(){
	float accel = gravity * dt;
	if (keyboard[' ']) dvel.Y() += 4;
	if (specialKey[GLUT_KEY_UP]) dvel -= afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_DOWN]) dvel += afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_HOME]) vel = Vec3f();
	if (specialKey[GLUT_KEY_LEFT]) afBody = afBody * Affinef(Rad(5), 'Y');
	if (specialKey[GLUT_KEY_RIGHT]) afBody = afBody * Affinef(Rad(-5), 'Y');
	
	afBody.Pos() += vel * dt;
	dtSelectObject(&afBody);
	dtLoadIdentity();
	dtTranslate(afBody.Pos().X(), afBody.Pos().Y(), afBody.Pos().Z());
	static int count;
	count ++;
//	cout << "Count " << count << " ============================================" << endl;
	penChekers.Check();

	vel += dvel;
	dvel = Vec3f();
	vel.Y() -= gravity * dt;
	vel = 0.98f * vel;

	//	頂点インデックスを作る．

	unsigned int idxs[1024];
	for(int i=0; i<sizeof(idxs)/sizeof(idxs[0]); ++i) idxs[i] = i;
	normals.clear();
	normals.push_back(Vec3d());			//	原点を加えておく
	
	//	衝突面の法線を列挙する．
	count=0;
	if (floorHeight + radius > afBody.Pos().Y()){
		normals.push_back(Vec3d(0,1,0));
	}
	for(TPenaltyCheckers::iterator i=penChekers.begin(); i != penChekers.end(); ++i){
		for(TIntersections::iterator j=i->iss.begin(); j != i->iss.end(); ++j){
			Vec3d n = 1000*(Vec3d&)j->normal;
			int i;
			for(i=0; i<normals.size(); ++i){
				if ((normals[i] - n).square() < 1E-4) break;
			}
			if (normals.size() == i){
				normals.push_back(n);
//				std::cout << "Col" << count << ":" << n << std::endl;
				count ++;
			}
		}
	}
	if (normals.size()>2){
		reflect = -vel.unit();
//		std::cout << "Reflect" << reflect << std::endl;
	}

	Vec3f normal;	//	反射法線
	if(normals.size() > 2){
		VertexBase vbase((Point*)normals.begin());
		Polyhedron pointPoly((Point*)&reflect, 1, idxs);
		Polyhedron normalPoly(vbase, normals.size(), idxs);
		Transform tr;
		tr.setIdentity();
		Point pa, pb;
		closest_points(normalPoly, pointPoly, tr, tr, pa, pb);
		if ((pa-pb).length2() > 0){
			normal = (Vec3d&)pa;
			if (normal.square() > 0) normal = normal.unit();
			else normal = Vec3f();
		}else{
			normal = reflect;
		}
	}else if(normals.size() == 2){
		normal = (normals[1]).unit();
	}
	if (vel*normal < -6*gravity*dt){
		vel -= (1+elasticity) * (vel * normal) * normal;
	}else if (vel*normal < 0){
		vel -= (vel * normal) * normal;
	}
	dtProceed();
	if (!bSender){
		cmdSocket.Receive();
	}else{
		CmdEye cmdEye(this);
		cmdEye.Send();
	}
	Invalidate();
}
