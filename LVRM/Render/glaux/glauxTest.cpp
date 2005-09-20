#include <iostream>
#include "Render/RDGlauxView.h"
#include <GL/glaux.h>
using namespace LVRM;
class TestView: public RDGlauxView{
public:
	virtual void OnIdle(){
		Invalidate();
	}
	virtual void OnDraw(){
		printf("d");
		glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glNormal3d(0, 0, -1.0);
		glBegin(GL_POLYGON);
			glVertex3fv(Vec3f(0,0,0));
			glVertex3fv(Vec3f(100,0,0));
			glVertex3fv(Vec3f(0,100,0));
		glEnd();
		glEnable(GL_LIGHT0);
		glNormal3d(0, 0, -1.0);
		glBegin(GL_LINES);
		glVertex2d(-12, -12);
		glVertex2d(12, -12);
		glVertex2d(12, 12);
		glVertex2d(-12, 12);
		glEnd();
		Flip();
	}
	virtual void OnSize(int w, int h){
		printf("OnSize %d %d\n", w, h);
		glViewport(0, 0, w, h);

		glFlush();
		
		Affinef afCam;
		afCam.PosZ() = -500.0f;
		afCam.LookAtGL(Vec3f());
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(afCam.inv());
		Affinef afProj(Vec3f(0,0,300), Vec2f(300,200), 1, 1000);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(afProj);
		std::cout << afProj << "\n";
	}
	virtual void OnClose(){
		printf("OnClose\n");
	}
	virtual void OnMouse(Button b, bool s, int x, int y){
		printf("OnMouse %s %d %d\n", s ? "on" : "off", x, y);
	}
	virtual void OnKeyBoard(int code){
		printf("OnKey %d\n", code);
	}
};
void main(){
	TestView view;
	view.WindowPosition(10,10, 300, 200);
	view.Init("OpenGL Window");
	view.Run();
}
