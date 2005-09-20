#include <Base/Env.h>
#include <Solid/SOLID.h>
#include <Object.h>

#include "GeoNode.h"
#include "GLView.h"
#include <GL/glut.h>
#include <crtdbg.h>
#include <strstream>
#include <penalty.h>

using namespace std;

#define KEY_ESC -27

TGLView::TGLView(){
	radius = 0.1f;
}
void TGLView::OnIdle(){
}

void TGLView::OnDraw(){
//	HapticStep();

	glClearColor(clearColor.X(), clearColor.Y(), clearColor.Z(), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glLoadMatrixf(afCam.inv());
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->Draw();
//		(*it)->DrawGeometry();
	}
	glPushMatrix();
	glTranslatef(probe.X(), probe.Y(), probe.Z());
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Vec4f(0.3f,0.3f,0.3f,1));
	glutSolidSphere(radius,8,6);
	glPopMatrix();

	pcs.Draw();
	Flip();
}

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f , (float)width/(float)height , 0.1f , 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	switch(key){
	case 'x':
		afCam = Affinef(Rad(5.0), 'X') * afCam;
		Invalidate();
		break;
	case 'X':
		afCam = Affinef(Rad(-5.0), 'X') * afCam;
		Invalidate();
		break;
	case 'y':
		afCam = Affinef(Rad(5.0), 'Y') * afCam;
		Invalidate();
		break;
	case 'Y':
		afCam = Affinef(Rad(-5.0), 'Y') * afCam;
		Invalidate();
		break;
	case 'z':
		afCam = Affinef(Rad(5.0), 'Z') * afCam;
		Invalidate();
		break;
	case 'Z':
		afCam = Affinef(Rad(-5.0), 'Z') * afCam;
		Invalidate();
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
	case ' ':
		HapticStep();
		glutPostRedisplay();
		break;
	case 'i':
		afCam.Pos() = Vec3f(0, 0, 5);
		afCam.LookAtGL(Vec3f(), Vec3f(0,1,0));
		Invalidate();
		break;
	default:
	break;
	}
}

void TGLView::OnSpecialKey(int key, int x, int y)
{

	glMatrixMode(GL_MODELVIEW);
	float delta = 0.1f;
	switch (key){
		case GLUT_KEY_UP:
			probe -= afCam.Ez() * delta;
			Invalidate();
			break;
		case GLUT_KEY_DOWN:
			probe += afCam.Ez() * delta;
			Invalidate();
			break;
		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;
		case GLUT_KEY_LEFT:
			probe -= afCam.Ex() * delta;
			Invalidate();
			break;
		case GLUT_KEY_RIGHT:
			probe += afCam.Ex() * delta;
			Invalidate();
			break;
		case GLUT_KEY_PAGE_UP:
			probe += afCam.Ey() * delta;
			Invalidate();
			break;
		case GLUT_KEY_PAGE_DOWN:
			probe -= afCam.Ey() * delta;
			Invalidate();
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
/*	case LEFT:
		if(x == mouseX && y == mouseY)
			return;
		spinY = (x - mouseX);	spinX = (y - mouseY);
		afCam = Affinef(Rad(spinY*0.6f), 'Y') * Affinef(Rad(spinX*0.6f), 'X') * afCam;
		Invalidate();
		break;			*/
	case LEFT:
		afCam.Pos() += afCam.Ez() * (y - mouseY) * 0.1f;
		Invalidate();
		break;		
	case MIDDLE:
		afCam.Pos() -= afCam.Ey() * (y - mouseY) * 0.1f;
		Invalidate();
		break;
	case RIGHT:
		afCam.Pos() += afCam.Ex() * (x - mouseX) * 0.1f;
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
	
	//	視点の初期化
	afCam.Pos() = Vec3f(0, 0, 5);
	afCam.LookAtGL(Vec3f(), Vec3f(0,1,0));
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
	dtCreateObject(&probe, shape);
	dtDisableCaching();

	//	チェックすべきオブジェクトの追加
    for (ObjectList::const_iterator j = objectList.begin(); j != objectList.end(); ++j){
		for (ObjectList::const_iterator i = objectList.begin(); i != j; ++i) {
			TPenaltyChecker pc((*i).second, (*j).second);
			pcs.push_back(pc);
		}
	}
}
void TGLView::HapticStep(){
	dtSelectObject(&probe);
	dtLoadIdentity();
	dtTranslate(probe.X(), probe.Y(), probe.Z());
	static int count;
	count ++;
	cout << "Count " << count << " ============================================" << endl;
	pcs.Check();
	dtProceed();
}
