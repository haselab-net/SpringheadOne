#include <iostream>
#include "Render/glut/RDGlutView.h"
#include <GL/glut.h>
Affinef afCam;

using namespace VRRend;
class TestView: public RDGlutView{
public:
	virtual void OnOpen(){
		glEnable(GL_LIGHT0);
		afCam.PosZ() = 400.0f;
		afCam.PosY() = 300.0f;
		afCam = afCam.LookAtGL(Vec3f());
	}
	virtual void OnIdle(){
		Invalidate();
	}
	virtual void OnDraw(){
//		printf("d");
		glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
		glColor3f(0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(afCam.Inv());
		static Affinef afObj;
		static Vec3f vel(0,0,0);
		static Vec3f omega(Rad(0),0,0);
		const float dt = 0.05;
		Affinef dangle(dt*omega.Size(), omega.Unit());
		Affinef afObjNext;
		Matrix3f rot = afObj;
		afObjNext = dangle * rot;
		afObjNext.Pos(afObj.Pos() + dt * vel);
		afObj = afObjNext;
		glMultMatrixf(afObj);
		glMultMatrixf(Affinef(0,10,0));
		glutSolidSphere(20, 32, 16);
		glMultMatrixf(Affinef(0,-25,0));
		glutSolidSphere(30, 32, 16);
		Flip();

		if (afObj.Pos().Y() < 0){
			vel += Vec3f(0,200,0);
		}
		vel += Vec3f(0,-10,0);
	}
	virtual void OnSize(int w, int h){
		printf("OnSize %d %d\n", w, h);
		glViewport(0, 0, w, h);

		glFlush();
		
#if 0
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.0, 3, 10, 100);
		Affinef afProjGL;
		glGetFloatv(GL_PROJECTION_MATRIX, afProjGL);
		std::cout << afProjGL << "\n";
#else
		glMatrixMode(GL_PROJECTION);
		Affinef afProj(Vec3f(0,0,500), Vec2f(w, h), 1, 10000);
		glLoadMatrixf(afProj);
		std::cout << afProj << "\n";
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
	}
};


void main(){
	TestView view;
	view.WindowPosition(10,10, 300, 200);
	view.Init("OpenGL Window");
	view.Run();
}
