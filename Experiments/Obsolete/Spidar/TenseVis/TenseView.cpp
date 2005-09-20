#include <iostream>
#include "Render/glut/RDGlutView.h"
#include <GL/glut.h>
#include "qhull3.0/src/qhull_a.h"
#include <vector>
#include <iostream>

Affinef afCam, afProj, afScene;
using namespace VRRend;

class TFace: public std::vector<int>{};
class TFaces: public std::vector<TFace>{};
class TMotor{
public:
	Vec3f pos;
	float minForce, maxForce;
	TMotor(const Vec3f& p, float mi, float ma):pos(p), minForce(mi), maxForce(ma){}
};
class TMotors:public std::vector<TMotor>{};
class TestView: public RDGlutView{
public:
	Vec3f endEffector;
	TMotors motors;
	Vec3f frames[8];
	
	TestView(){
		float minf=0, maxf=50;
		motors.push_back(TMotor(Vec3f( 50, 50, 50), minf, maxf));
		motors.push_back(TMotor(Vec3f( 50,-50,-50), minf, maxf));
		motors.push_back(TMotor(Vec3f(-50, 50,-50), minf, maxf));
		motors.push_back(TMotor(Vec3f(-50,-50, 50), minf, maxf));
		for(int i=0; i<8; i++){
			frames[i] = Vec3f(
				(i/4)%2 ? 50 : -50,
				(i/2)%2 ? 50 : -50,
				i%2 ? 50 : -50);
		}
	}
	void MakeVtxs(std::vector<Vec3f>& vtxs, Vec3f pos, Vec3f* minF, Vec3f* maxF, int nVtx){
		if (nVtx > 0){
			MakeVtxs(vtxs, pos+minF[0], minF+1, maxF+1, nVtx-1);
			MakeVtxs(vtxs, pos+maxF[0], minF+1, maxF+1, nVtx-1);
		}else{
			vtxs.push_back(pos);
		}
	}
	void CalcConvex(TFaces& faces, const std::vector<Vec3f>& vtxs){
		char* argv[] = {"qhull","i"};
		qh_init_A(stdin, stdout, stderr, 2, argv);
		qh_initflags (qh qhull_command);
	
		double* points = new double[vtxs.size() * 3];
		for(int i=0; i<vtxs.size(); i++){
			points[i*3+0] = vtxs[i].X();
			points[i*3+1] = vtxs[i].Y();
			points[i*3+2] = vtxs[i].Z();
		}
		qh_init_B(points, vtxs.size(), 3, true);
		qh_qhull();
		qh_check_output();

		facetT *facet;
		vertexT *vertex;
		vertexT **vertexp;

		FORALLfacet_(qh facet_list){
			setT *vertices;
			vertices = qh_facet3vertex (facet);
			
			TFace face;
			FOREACHvertex_(vertices){
				face.push_back( qh_pointid(vertex->point) );
			}
			faces.push_back(face);
		}
		qh_freeqhull(!qh_ALL);
		int curlong, totlong;
		qh_memfreeshort(&curlong, &totlong);
	}
	
	virtual void OnOpen(){
		glEnable(GL_LIGHT0);
		afCam.PosZ() = 500.0f;
//		afCam.PosY() = -500.0f;
		afCam = afCam.LookAtGL(Vec3f());
	}
	virtual void OnIdle(){
		Invalidate();
	}
	void DrawFrame(int a, int b, int c, int d, int e, int f){
		int id1 = a + b*2 + c*4;
		int id2 = d + e*2 + f*4;
		glBegin(GL_LINES);
		glVertex3fv(frames[id1]);
		glVertex3fv(frames[id2]);
		glEnd();
	}
	virtual void OnDraw(){
		printf("d");
		glClearColor(1, 1, 1, 1);
		glColor3f(0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(afCam.Inv());

		float p[] = {0,0,1,0};
		glLightfv(GL_LIGHT0, GL_POSITION, p);

		glMultMatrixf(afScene);
		
		//	エンドエフェクタを表す赤い丸の表示
		float dif2[] = {1, 0, 0, 1};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
		glPushMatrix();
			glTranslatef(endEffector.X(), endEffector.Y(), endEffector.Z());
			glutSolidSphere(5, 32, 16);
		glPopMatrix();

		//	ワイヤーと最大，最小張力の表示
		float mWireA[] = {0.5, 0.5, 0.5, 1};
		glMaterialfv(GL_FRONT, GL_AMBIENT, mWireA);
		float mWireD[] = {0, 0, 0, 1};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mWireD);
		for(TMotors::iterator it = motors.begin(); it != motors.end(); ++it){
			glBegin(GL_LINES);
			glVertex3fv(endEffector);
			glVertex3fv(it->pos);
			glEnd();
			glPushMatrix();
				glTranslatef(it->pos.X(), it->pos.Y(), it->pos.Z());
				glutSolidSphere(5, 16, 8);
			glPopMatrix();
		}
		DrawFrame(0,0,0, 1,0,0);
		DrawFrame(0,0,0, 0,1,0);
		DrawFrame(0,0,0, 0,0,1);

		DrawFrame(1,1,0, 1,1,1);
		DrawFrame(1,1,0, 1,0,0);
		DrawFrame(1,1,0, 0,1,0);

		DrawFrame(1,0,1, 0,0,1);
		DrawFrame(1,0,1, 1,1,1);
		DrawFrame(1,0,1, 1,0,0);

		DrawFrame(0,1,1, 1,1,1);
		DrawFrame(0,1,1, 0,0,1);
		DrawFrame(0,1,1, 0,1,0);

		//	力が出せる範囲の表示
		std::vector<Vec3f> maxTense, minTense;
		for(TMotors::iterator it = motors.begin(); it != motors.end(); ++it){
			Vec3f dir = (it->pos - endEffector).Unit();
			maxTense.push_back(dir * it->maxForce);
			minTense.push_back(dir * it->minForce);
		}
		std::vector<Vec3f> vtxs;
		MakeVtxs(vtxs, endEffector, minTense.begin(), maxTense.begin(), minTense.size());
		TFaces faces;
		CalcConvex(faces, vtxs);

		float alpha = 0.5;
		float dif[] = {0.8, 0.8, 0.0, alpha};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		float amb[] = {0.4, 0.4, 0.0, alpha};
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		for(TFaces::iterator its = faces.begin(); its != faces.end(); ++its){
			glBegin(GL_POLYGON);
			TFace& f = *its;
			Vec3f a = vtxs[f[1]] - vtxs[f[0]];
			Vec3f b = vtxs[f[2]] - vtxs[f[0]];
			Vec3f n = -a ^ b;
			n = n.Unit();
			glNormal3fv(n);

			for(TFace::reverse_iterator it = f.rbegin(); it != f.rend(); ++it){
				glVertex3fv(vtxs[*it]);
			}
			glEnd();
		}
		Flip();
	}
	virtual void OnSize(int w, int h){
		printf("OnSize %d %d\n", w, h);
		glViewport(0, 0, w, h);
		glFlush();
#if 0
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(20.0, 1, 1, 10000);
		glGetFloatv(GL_PROJECTION_MATRIX, afProj);
		std::cout << afProj << "\n";
#else
		afProj = Affinef(Vec3f(0,0,500), Vec2f(w, h).Unit()*500, 1, 10000);
		std::cout << afProj << "\n";
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(afProj);
#endif
	}
	virtual void OnClose(){
		printf("OnClose\n");
	}
	virtual void OnMouse(Button b, bool s, int x, int y){
		printf("OnMouse %s %d %d\n", s ? "on" : "off", x, y);
	}
	virtual void OnKeyboard(int code, int x, int y){
		printf("OnKey %d\n", code);
		if (code == 0x1b){
			Exit();
		}
		if (-code == GLUT_KEY_UP) afScene = Affinef(Rad(1), 'x') * afScene;
		if (-code == GLUT_KEY_DOWN) afScene = Affinef(Rad(-1), 'x') * afScene;
		if (-code == GLUT_KEY_LEFT) afScene = Affinef(Rad(1), 'y') * afScene;
		if (-code == GLUT_KEY_RIGHT) afScene = Affinef(Rad(-1), 'y') * afScene;
		if (-code == GLUT_KEY_PAGE_UP) afScene = Affinef(Rad(1), 'z') * afScene;
		if (-code == GLUT_KEY_PAGE_DOWN) afScene = Affinef(Rad(-1), 'z') * afScene;
		if (code == 'x') endEffector.X() += 1;
		if (code == 'X') endEffector.X() -= 1;
		if (code == 'y') endEffector.Y() += 1;
		if (code == 'Y') endEffector.Y() -= 1;
		if (code == 'z') endEffector.Z() += 1;
		if (code == 'Z') endEffector.Z() -= 1;
		if (code == 'w') endEffector += (motors[0].pos - endEffector).Unit();
		if (code == 'W') endEffector -= (motors[0].pos - endEffector).Unit();
		Vec3f sum = motors[1].pos + motors[2].pos + motors[3].pos;
		if (code == 'e') endEffector += (sum - endEffector).Unit();
		if (code == 'E') endEffector -= (sum - endEffector).Unit();
	}
};


void main(){
	TestView view;
	view.WindowPosition(10,10, 300, 200);
	view.Init("OpenGL Window");
	view.Run();
}

char qh_version[] = "TenseView";
