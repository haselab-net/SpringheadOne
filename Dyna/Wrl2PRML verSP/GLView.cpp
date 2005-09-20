
#include "CalcPenalty.h"
#include "GeoNode.h"
#include "GLView.h"
#include "calcFriction.h"

//#include "His/HISpidar4Quad.h"
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>


#include <Base/Env.h>
#include <Solid/SOLID.h>
#include <VertexBase.h>
#include <Polyhedron.h>
#include <Complex.h>
#include <Sphere.h>
#include <Object.h>
#include <penalty.h>

#include <GL/glut.h>
#include <crtdbg.h>
#include <strstream>
#include <float.h>

#include "global.h"

using namespace std;

#define KEY_ESC -27
//#define KIM
//#define PAST
const double floorHeight = 0.0f;
const double dt = 1.0f / 100.0f;
const double gravity = 9.8f;
const double airResistance = 0;

//	SOLIDのグローバル変数
typedef map<DtObjectRef, Object *> ObjectList;
extern ObjectList objectList;
typedef vector<Complex *> ComplexList;
extern ComplexList complexList;

//	the instance of HISpidar4quad

TGLView::TGLView(){
	initialFullscreen = false;
	writeCounter = 10;
	radius = 0.5f;
	afBody = Affined(0, 0, 10);
	afBody.LookAtGL(Vec3d(0, 0, 0), Vec3d(0,1,0));
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
	Affined afCam = (afBody * afEye).inv();
	glLoadMatrixd(afCam);
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

void TGLView::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f , (double)width/(double)height , 0.1f , 1000.0f);
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
		afBody = Affined(Rad(90), 'y') * afBody;
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
			l0pos[] = {-10, 5, 20, 0};

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
	if (initialFullscreen) glutFullScreen();
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
		dtLoadMatrixd((*itn)->GetPosture());
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
	double accel = gravity * dt;
	if (keyboard[' ']) dvel.Y() += 4;
	if (specialKey[GLUT_KEY_UP]) dvel -= afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_DOWN]) dvel += afBody.Ez() * accel;
	if (specialKey[GLUT_KEY_HOME]) vel = Vec3d();
	if (specialKey[GLUT_KEY_LEFT]) afBody = afBody * Affined(Rad(5), 'Y');
	if (specialKey[GLUT_KEY_RIGHT]) afBody = afBody * Affined(Rad(-5), 'Y');
	
	//	位置の更新
	afBody.Pos() += vel * dt;
//	dtSelectObject(&afBody);
//	dtLoadIdentity();
//	dtTranslate(afBody.Pos().X(), afBody.Pos().Y(), afBody.Pos().Z());
	dtProceed();
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		(*it)->UpdatePosture(dt);
	}
	//	衝突判定
	penChekers.Detect();
	//	衝突結果を反映させる．
	for(TPenaltyDetectors::iterator it = penChekers.begin(); it != penChekers.end(); ++it){
		TPenaltyDetector& pc = *it;
		TCalcCollisionMotion cft;
		TGeoNode* g1 = (TGeoNode*)pc.obj[0]->ref;
		TGeoNode* g2 = (TGeoNode*)pc.obj[1]->ref;
		Vec3d penalty, penaltyMoment;
		Vec3d velocity, velocityMoment;
		Vec3d friction, normal, newinterior;
		double area=0;
		for(TPenaltyDetector::TIntersectionSet::iterator itIs = pc.iss.begin(); itIs != pc.iss.end(); ++itIs){
			TIntersection& is = *itIs;
			if (is.convex[0]->convexType()==Convex::POLYTOPE && is.convex[1]->convexType()==Convex::POLYTOPE){
				double a=0;
				Vec3d p1 = (Vec3d&)pc.obj[0]->curr(is.point[0]);
				Vec3d p2 = (Vec3d&)pc.obj[1]->curr(is.point[1]);
				Vec3d interior = (Vec3d&)pc.obj[0]->curr(is.interior[0]);
				Vec3d pen, penMom, vel, velMom;
#ifdef KIM
				pen = (p2 - p1).norm() * (Vec3d&)is.normal;
				penMom = Vec3d();
				a = 0.0;
				UsoPenalty(dt, pen, pc, g1, g2, p1, p2);
				pc.applicationPoint = (p1+p2)/2;
				pc.sprPoint[0] = g1->posture.inv() * pc.applicationPoint;
				pc.sprPoint[1] = g2->posture.inv() * pc.applicationPoint;
				pc.lastPenalty = pen;
#else
				CalcPenalty(pen, penMom, vel, velMom, a, (Vec3d&)is.normal, interior,
					g1, (TPolytope*)is.convex[0]->userData, is.support[0], p1,
					g2, (TPolytope*)is.convex[1]->userData, is.support[1], p2
				);
				penalty += pen;
				penaltyMoment += penMom;
				velocity += vel;
				velocityMoment += velMom;
				area += a;
				normal = (Vec3d&)is.normal;		//(5/10追加)
				newinterior = (p1 + p2) / 2;
#endif
			}
		}
#ifndef KIM
		if (area > 0){
			penalty = penalty / area;
			penaltyMoment = penaltyMoment / area;
			velocity = velocity / area;
			velocityMoment = velocityMoment / area;
			cft.CalcCollisionMotion(penalty, penaltyMoment, velocity, velocityMoment, 
				normal, newinterior, pc, g1, g2, dt);	//(5/6追加)
		}
#endif
	}
	//	速度の更新
	vel += dvel;
	dvel = Vec3d();
	vel = (1 - airResistance*dt) * vel;
	for(TGeoNodes::iterator it = allNodes.begin(); it != allNodes.end(); ++it){
		if ((*it)->Name() != "spidar"){
			if ((*it)->polytopes.size()){
				(*it)->AddGravity(Vec3d(0, -gravity, 0), dt);
			}
		}else{
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


			v3fAvatarReactionforce = (*it)->force / 15;
			
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
#ifndef PAST
		if ((*it)->Name() != "spidar"){
			if ((*it)->polytopes.size()){
				(*it)->UpdateMotion(dt);
			}
		}else{
			(*it)->force = Vec3d();
			(*it)->torque = Vec3d();
			(*it)->friction = Vec3d();
			(*it)->angularFriction = Vec3d();
		}
#endif
	}
	int drawRate = (1.0/30) / dt;
	if (drawRate <= 0) drawRate = 1;
	static int counter;
	if (counter < drawRate){
		counter ++;
	}else{
		Invalidate();
		counter = 1;
	}
	writeCounter++;
}

void TGLView::UsoPenalty(double dt, Vec3d pen, TPenaltyDetector& pc, 
							TGeoNode* g1, TGeoNode* g2, Vec3d near1, Vec3d near2){

	///< バネ係数の計算
	double k;
	double T = dt * TIM_MULT;
	if (g1->mass < g2->mass){
		k = g1->mass * (M_PI / T) * (M_PI / T);
	} else {
		k = g2->mass * (M_PI / T) * (M_PI / T);
	}
	Vec3d force = k * (pen + DAM_STF * (pen - pc.lastPenalty) / dt);
	Vec3d penMom1 = (near1 - g1->posture * g1->Center()) ^ pen;
	Vec3d penMom2 = (near2 - g2->posture * g2->Center()) ^ pen;
	Vec3d torque1 = k * (penMom1 + DAM_STF * (penMom1 - pc.lastPenaltyMoment[0])/dt);
	Vec3d torque2 = k * (penMom2 + DAM_STF * (penMom2 - pc.lastPenaltyMoment[1])/dt);
	if (g1->polytopes.size() && _finite(g1->mass) && g1->mass != HUGE_VAL){
		g1->momentum -= force*dt;
		g1->angularMomentum -= torque1 * dt;
	}
	if (g2->polytopes.size() && _finite(g2->mass) && g2->mass != HUGE_VAL){
		g2->momentum += force*dt;
		g2->angularMomentum += torque2 * dt;
	}
	pc.lastPenalty = pen;
	pc.lastPenaltyMoment[0] = penMom1;
	pc.lastPenaltyMoment[1] = penMom2;
}

bool TGLView::CalcPenaltySwitch(TPolytope* p1, TPolytope* p2, Vec3d interior){
	Affined posture1 = p1->vertices.posture;
	Affined posture2 = p2->vertices.posture;

	for (TPolytope::iterator it = p1->begin(); it != p1->end(); it++) {
		Vec3d point = p1->vertices.GCoord((*it)[0]);
		Vec3d normal = it->GNormal(posture1);

//		cout << normal * interior - normal * point << " " << normal.X() << endl;
		if ( normal * interior - normal * point >= 0){
			return false;
		}
	}
	for (TPolytope::iterator it = p2->begin(); it != p2->end(); it++) {
		Vec3d point = p2->vertices.GCoord((*it)[0]);
		Vec3d normal = it->GNormal(posture2);
		
//		cout << normal * interior - normal * point << " " << normal.X() << endl;		
		if ( normal * interior - normal * point >= 0) {
			return false;
		}
	}
	return true;
}
void TGLView::CalcPenalty(Vec3d& pen, Vec3d& penMom, Vec3d& vel, Vec3d& velMom, double& area, Vec3d& normal, Vec3d interior,
						  TGeoNode* g1, TPolytope* p1, TSupport* s1, Vec3d near1,
						  TGeoNode* g2, TPolytope* p2, TSupport* s2, Vec3d near2 ){
//	Vec3d newinterior = (near1 + near2) / 2;
//	Vec3d newinterior = interior;
//	Vec3d newinterior = ((near1 + near2) / 2 + interior) / 2; 
	Vec3d newinterior =	(near1 + near2 + interior) / 3;

	int k = 0;
	int j = 0;
	for (int i=0; i<4; i++) {
		if ((s1+i)->vtx != -1) j++;
		if ((s2+i)->vtx != -1) k++;
	}
#ifdef DEBUG_MESSAGE
	cout << "Number of support1:" << k << "   Number of support2:" << k << endl; 
#endif
	if (CalcPenaltySwitch(p1, p2, newinterior)) {
		TCalcPenalty cp(newinterior, normal);

		cp.CalcPenalty(g1, p1, g2, p2);
		pen = cp.penalty;
		penMom = cp.penaltyMoment;
		vel = cp.velocity;
		velMom = cp.velocityMoment;
		area = cp.area;
	}else {
		TCalcPenaltySub cps(g1->vertices, g2->vertices);

		cout << "Qhull has ERROR" << endl;		
		cps.normal = normal;
		FindInterface(cps.poly1, p1, s1, normal);
		cps.poly1.point = near1;
		FindInterface(cps.poly2, p2, s2, -normal);
		cps.poly2.point = near2;

		cps.CalcPenalty();
		pen = cps.penalty;
		penMom = cps.penaltyMoment;
		area = cps.area;
/*		penalty = Vec3d();
		penaltyMoment = Vec3d();
		area = 0.0f;*/
	}
}

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

void TGLView::FindInterface(TCollideInterface& rv, TPolytope* poly, TSupport* support, const Vec3d& normal){
	//	平行な面を列挙
	for(TPolytope::iterator it=poly->begin(); it != poly->end(); ++it){
		Vec3d a = poly->vertices.GCoord(it->begin()[1]) - poly->vertices.GCoord(it->begin()[0]);
		Vec3d b = poly->vertices.GCoord(it->begin()[2]) - poly->vertices.GCoord(it->begin()[0]);
		Vec3d n = (a ^ b).unit();
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

