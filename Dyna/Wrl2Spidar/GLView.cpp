#include <Base/Env.h>
#include <Solid/SOLID.h>
#include <VertexBase.h>
#include <Polyhedron.h>
#include <Complex.h>
#include <Sphere.h>
#include <Object.h>
#include <penalty.h>

#include "CalcPenalty.h"
#include "GeoNode.h"
#include "GLView.h"
#include "calcFriction.h"

#include <GL/glut.h>
#include <crtdbg.h>
#include <strstream>
#include <float.h>

#include "His/HISpidar4Quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <Device/DRUsbH8Simple.h>

using namespace std;

#define KEY_ESC -27
const float floorHeight = 0.0f;
const float dt = 1.0f / 30.0f/3;
const float gravity = 9.8f;
const float airResistance = 0.01f;

//	SOLIDのグローバル変数
typedef map<DtObjectRef, Object *> ObjectList;
extern ObjectList objectList;
typedef vector<Complex *> ComplexList;
extern ComplexList complexList;

//	the instance of HISpidar4quad


TGLView::TGLView(){
	radius = 0.1f;
	afBody = Affinef(0, 0, 10);
	afBody.LookAtGL(Vec3f(0, 0, 0), Vec3f(0,1,0));
	bDrawCollision = true;
	sphere = NULL;
}
TGLView::~TGLView(){
	delete sphere;
	for(ComplexList::iterator it=complexList.begin(); it!=complexList.end(); ++it){
		delete it;
	}
	for(ObjectList::iterator it=objectList.begin(); it!=objectList.end(); ++it){
		delete it->second;
	}
}
bool TGLView::OnTimer(){
	Step();
	return true;
}
void TGLView::OnDraw(){
	glClearColor(clearColor.X(), clearColor.Y(), clearColor.Z(), 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	Affinef afCam = (afBody * afEye).inv();
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

	Flip();
}

void TGLView::OnSize
(
 int width,
 int height
 )
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f , (float)width/(float)height , 0.1f , 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

void TGLView::OnKeyboard(int key, int x, int y)
{
	switch(key){
	case '\x1b':
		Exit();
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
	case 'r':
		afBody = Affinef(Rad(90), 'y') * afBody;
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

	SetTimer(dt*1000, &TGLView::OnTimer);
}

void TGLView::InitCollisionDetector(){
	//	SOLIDの初期化
	//	ノードの登録
	for(TGeoNodes::iterator itn = allNodes.begin(); itn != allNodes.end(); ++itn){
		if (!(*itn)->polytopes.size()) continue;
		TCoords& vtxs = (*itn)->vertices;
		Complex* shape = new Complex;
		complexList.push_back(shape);
		shape->setBase((*itn)->vertices.begin());
		vector<Polyhedron*> phs;
		for(TPolytopes::iterator itp=(*itn)->polytopes.begin(); itp != (*itn)->polytopes.end(); ++itp){ 
			vector<unsigned int> idxs;
			for(TPolytope::iterator itf = itp->begin(); itf != itp->end(); ++itf){
				for(TFace::iterator itv = itf->begin(); itv != itf->end(); ++itv){
					idxs.push_back(*itv);
				}
			}
			unsigned int* idxsAlloc = new unsigned int [idxs.size()];
			memcpy(idxsAlloc, idxs.begin(), sizeof(unsigned int) * idxs.size());
			Polyhedron* ph = new Polyhedron(shape->getBase(), idxs.size(), idxsAlloc);
			ph->userData = itp;
			itp->convex = ph;
			phs.push_back(ph);
		}
		const Polytope** phsAlloc = new const Polytope* [phs.size()];
		memcpy(phsAlloc, phs.begin(), sizeof(Polytope*)*phs.size());
		shape->finish(phs.size(), phsAlloc);
		dtCreateObject(*itn, shape);
		(*itn)->solid = objectList[*itn];
		dtLoadMatrixf((*itn)->GetPosture());
	}

//	sphere = new Sphere(radius);
//	dtCreateObject(&afBody, sphere);

	//	チェックすべきオブジェクトの追加
	for (ObjectList::const_iterator i = objectList.begin(); i != objectList.end(); ++i) {
		for (ObjectList::const_iterator j = objectList.begin(); j != i; ++j) {
			TPenaltyDetector pc(i->second, j->second);
			penChekers.push_back(pc);
		}
	}
}

void TGLView::Step(){
	//	キー状態チェック
	float accel = gravity * dt;
	if (keyboard[' ']) dvel.Y() += 4;
	if (specialKey[GLUT_KEY_UP]) dvel -= afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_DOWN]) dvel += afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_HOME]) vel = Vec3f();
	if (specialKey[GLUT_KEY_LEFT]) afBody = afBody * Affinef(Rad(5), 'Y');
	if (specialKey[GLUT_KEY_RIGHT]) afBody = afBody * Affinef(Rad(-5), 'Y');
	
	//	位置の更新
	afBody.Pos() += vel * dt;
//	dtSelectObject(&afBody);
//	dtLoadIdentity();
//	dtTranslate(afBody.Pos().X(), afBody.Pos().Y(), afBody.Pos().Z());
	dtProceed(); 
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->UpdatePosture(dt);
/*		if((*it)->Name() != "spidar")	(*it)->UpdatePosture(dt);
		else
		{
			// load the position from spidar
			Affinef pos;
			pos.Pos() = spidar->GetPos();
			(*it)->SetPosture(pos);
		}*/
	}
	//	衝突判定
	penChekers.Detect();
	//	衝突結果を反映させる．
	for(TPenaltyDetectors::iterator it = penChekers.begin(); it != penChekers.end(); ++it){
		TPenaltyDetector& pc = *it;
		TCalcFriction cf;
		TGeoNode* g1 = (TGeoNode*)pc.obj[0]->ref;
		TGeoNode* g2 = (TGeoNode*)pc.obj[1]->ref;
		Vec3f force, torque, friction, normal;
		float area=0;
		for(TPenaltyDetector::TIntersectionSet::iterator itIs = pc.iss.begin(); itIs != pc.iss.end(); ++itIs){
			TIntersection& is = *itIs;
			if (is.convex[0]->convexType()==Convex::POLYTOPE && is.convex[1]->convexType()==Convex::POLYTOPE){
				TCalcPenalty::PenaltyType tp;
				Vec3f f, t;
				float a=0;
				Vec3d p1 = (Vec3d&)pc.obj[0]->curr(is.point[0]);
				Vec3d p2 = (Vec3d&)pc.obj[1]->curr(is.point[1]);
//				std::cout << "Nearest:" << p1 << "  " << p2 << std::endl;
				CalcPenalty(tp,f,t,a, (Vec3d&)is.normal, 
					g1, (TPolytope*)is.convex[0]->userData, is.support[0], p1,
					g2, (TPolytope*)is.convex[1]->userData, is.support[1], p2
				);				
				force += f;
				torque += t;
				area += a;
				normal = (Vec3f&)is.normal;		//(5/10追加)
			}
/*			cout << itIs->state << ' ';
			if(is.state == TIntersection::CREATED & pc.state != TPenaltyDetector::KEEP)	
				pc.state = TPenaltyDetector::CREATED;
			if(is.state == TIntersection::KEEP)	
				pc.state = TPenaltyDetector::KEEP;*/
		}
//		cout << pc.state <<endl;
//		if (pc.state == TPenaltyDetector::CREATED) cout <<"------------------------------------------------";
//		if(pc.iss.size() == 0)	pc.state = TPenaltyDetector::NOCHECK;

//		g1->Add(it, friction);			//(5/9追加)
//		g2->AddFriction(-friction, dt);			//(5/9追加)
		if (area > 0){
			force = force / area;
			torque = torque / area;
			std::cout.precision(2);
//			std::cout << "Force:" << force << "  Torque:" << torque << std::endl;
			cf.CalcFriction(force, torque, normal, pc, g1, g2, dt);	//(5/6追加)
			float mass = g1->mass;
			if (g2->mass < mass || mass < 0 || !_finite(mass)) mass = g2->mass;
			float k = mass * (M_PI / dt) * (M_PI / dt) / 100;
			g1->AddPenalty(-force * k, -torque * k); //(5/6変更)
			g2->AddPenalty(force * k, torque * k); //(5/6変更)
			if(g1->Name() == "spidar") cout << "k = " << k <<" "<< "force = " << force << endl;
			if(g2->Name() == "spidar") cout << "k = " << k <<" "<< "force = " << force << endl;
		}
	}
	//	速度の更新
	vel += dvel;
	dvel = Vec3f();
//	vel.Y() -= gravity * dt;
	vel = (1 - airResistance*dt) * vel;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->Name() != "spidar"){
			if ((*it)->polytopes.size()){
				(*it)->ConvertToForce(dt);
				(*it)->AddGravity(Vec3f(0, -gravity, 0), dt);
				(*it)->AirResistance(airResistance, dt);
			} 
		}
		else
		{
			// transmit force to spidar
			float k = 0.05;
			spidar->Update();
			
			// ******************************************************
			// ********************  Test zone   ********************
			// ********************   Beginning   *******************
			// ******************************************************
			//
			// ------------------------------------------------------
			// -------------------- INFORMATION　--------------------
			//
			// Request from:	Fujii-san.
			// Test date:		13/06/2002.
			// Test author:		Duval Sebastien.
			//
			// ------------------------------------------------------
			// --------------------- OBJECTIVE ----------------------
			//
			// The goal of this test is to check the force that is
			// calculated when the avatar and the VRML "floor"
			// collide.
			//
			// Normally the force should be colinear to Y-axis like:
			// (0 fYaxisValue 0).
			//
			// ------------------------------------------------------
			// ---------------------- RESULTS　----------------------
			//
			// This test showed that the calculation of the reaction
			// force is not colinear to Y-axis. The values displayed
			// in the console is sometimes like:
			// (0.0011 -12 79)
			//
			// The value in Z-axis is so strong that you only feel
			// this one.The problem comes from penalty calculation.
			//
			// ------------------------------------------------------
			// --------------------- NEXT STEP　---------------------
			//
			// Check penalty calculation.
			//
			// ------------------------------------------------------
			// ---------------- ORIGINAL SOURCE CODE　---------------
			//
			// spidar->SetForce((*it)->penalty * k);
			// cout << "Spidarforce =" << (*it)->penalty * k << endl;
			//
			// ------------------------------------------------------

			// Create an object to store the data about the force
			// exerced on the avatar.

			Vec3f v3fAvatarReactionforce;

			// Calculate and store the value of the force exerced
			// on the avatar.

			v3fAvatarReactionforce = (*it)->penalty * k;
			
			// Display in console  the value of the force exerced
			// on the avatar.

			cout << "Spidarf force: " << v3fAvatarReactionforce << endl;
			
			// Send reaction force to the Spidar device.
			spidar->SetForce(v3fAvatarReactionforce);
			
			// ******************************************************
			// ********************  Test zone   ********************
			// ********************      End     ********************
			// ******************************************************
		}
		(*it)->UpdatePenalty();
	}

/*	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->polytopes.size()){
//			assert(-50 < (*it)->momentum.norm() && (*it)->momentum.norm() < 50);
			(*it)->AddFriction(dt);
		}
	}*/
/*	for(TPenaltyDetectors::iterator it = penChekers.begin(); it != penChekers.end(); ++it){
		TPenaltyDetector& pc = *it;
		TCalcFriction cf;
		TGeoNode* g1 = (TGeoNode*)pc.obj[0]->ref;
		TGeoNode* g2 = (TGeoNode*)pc.obj[1]->ref;*/
	Invalidate();
}
void TGLView::CalcPenalty(TCalcPenalty::PenaltyType& type, Vec3f& force, Vec3f& torque, float& area, Vec3d& n,
						  TGeoNode* g1, TPolytope* p1, TSupport* s1, Vec3d near1,
						  TGeoNode* g2, TPolytope* p2, TSupport* s2, Vec3d near2){
	TCalcPenalty cp(g1->vertices, g2->vertices);
	cp.normal = n;
	FindInterface(cp.poly1, p1, s1, n);
//	std::cout << "------------------------------------------------------\n";
//	std::cout << cp.poly1 << std::endl;
	cp.poly1.point = near1;
	FindInterface(cp.poly2, p2, s2, -n);
	cp.poly2.point = near2;
//	std::cout << cp.poly2 << std::endl;
	
	cp.CalcPenalty();
	force = cp.force;
	torque = cp.torque;
	area = cp.area;
	type = cp.type;
}
/********** (5/6追加) ************
Vec3f TGLView::CalcFriction(Vec3f& force, Vec3f& torque, TPenaltyDetector& pc, 
						   TGeoNode* g1, TGeoNode* g2){
	TCalcFriction cf;
	
	if(pc.state == TPenaltyDetector::CREATED){
		pc.sprPoint[0] = (Point)(g1->posture.inv() * cf.SprPoint(force, torque));
		pc.sprPoint[1] = (Point)(g2->posture.inv() * cf.SprPoint(force, torque));
		return Vec3f(0.0,0.0,0.0);
	}
	if(pc.state == TPenaltyDetector::KEEP){
		return cf.CalcFriction(force, torque, pc, g1, g2);
	}
}
*********************************/
class TEdge{
public:
	int vtx[2];
	TEdge(int v1, int v2){
		if (v1<v2){
			vtx[0] = v1;
			vtx[1] = v2;
		}else{
			vtx[0] = v2;
			vtx[1] = v1;
		}
	}
};
bool operator < (const TEdge& e1, const TEdge& e2){
	if (e1.vtx[0] < e2.vtx[0]) return true;
	if (e1.vtx[0] > e2.vtx[0]) return false;
	return e1.vtx[1] < e2.vtx[1];
}

void TGLView::FindInterface(TCollideInterface& rv, TPolytope* poly, TSupport* support, const Vec3f& normal){
	//	平行な面を列挙
	for(TPolytope::iterator it=poly->begin(); it != poly->end(); ++it){
		Vec3d a = poly->vertices.GCoord(it->begin()[1]) - poly->vertices.GCoord(it->begin()[0]);
		Vec3d b = poly->vertices.GCoord(it->begin()[2]) - poly->vertices.GCoord(it->begin()[0]);
		Vec3f n = (a ^ b).unit();
		if (n*normal > 0.98f){
			rv.push_back(*it);
		}
	}
	if (rv.size()){
		rv.state = TCollideInterface::FACE;
		return;
	}
	//	面がない場合は平行な辺を列挙
	std::set<TEdge> edges;
	for(TPolytope::iterator itf=poly->begin(); itf != poly->end(); ++itf){
		TFace& face = *itf;
		for(int i=0; i<face.size()-1; ++i){
			edges.insert(TEdge(face[i], face[i+1]));
		}
	}	
	for(std::set<TEdge>::iterator it=edges.begin(); it != edges.end(); ++it){
		Vec3d p1 = poly->vertices.GCoord(it->vtx[0]);
		Vec3d p2 = poly->vertices.GCoord(it->vtx[1]);
		Vec3d l = p2 - p1;
		if (-0.01f < l*normal && l*normal < 0.01f){
			TFace face;
			face.push_back(it->vtx[0]);
			face.push_back(it->vtx[1]);
			rv.push_back(face);
		}
	}
	if (rv.size()){
		rv.state = TCollideInterface::EDGE;
		return;
	}
	//	辺もない場合は頂点を返す．
	rv.state = TCollideInterface::VERTEX;
}

