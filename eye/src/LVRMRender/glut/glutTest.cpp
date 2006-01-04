#include <LVRMRender/RDGlutView.h>
#include <gl/glut.h>
#include <iostream>

using namespace Spr;

Affinef afCam;
class TestView: public RDGlutView{
public:
	virtual void OnOpen(){
		glEnable(GL_LIGHT0);
		Vec4f lightPos = Vec4f(0.0f, 1.0f, 0.0f, 0.0f);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		afCam.PosZ() = 400.0f;
		afCam.PosY() = 300.0f;
		afCam.LookAtGL(Vec3f());
	}
	virtual void OnIdle(){
		Invalidate();
	}
	virtual void OnDraw(){
		printf("d");
		glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
		glColor3f(0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		afCam = Affinef::Rot(Rad(1), 'x') * afCam;

		glLoadMatrixf(afCam.inv());
		glBegin(GL_TRIANGLES);
			glVertex3f(0,0,0);
			glNormal3f(0,0,1);
			glVertex3f(100,0,0);
			glNormal3f(0,0,1);
			glVertex3f(0,100,0);
			glNormal3f(0,0,1);
		glEnd();
		glPushMatrix();
			glMultMatrixf(Affinef::Trn(0,-20,0));
			glutSolidSphere(20,10,10);
		glPopMatrix();

		Affinef af;
		af.Rot() *= -1;
		glMultMatrixf(af);
		glutSolidTeapot(30);
		Flip();
	}
	virtual void OnSize(int w, int h){
		printf("OnSize %d %d\n", w, h);
		glViewport(0, 0, w, h);

		glFlush();
		
#if 0
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(10.0, 3, 1, 10000);
		Affinef afProjGL;
		glGetFloatv(GL_PROJECTION_MATRIX, afProjGL);
		std::cout << afProjGL << "\n";
#else
		glMatrixMode(GL_PROJECTION);
		Affinef afProj=Affinef::ProjectionGL(Vec3f(0,0,500), Vec2f(w, h), 1, 10000);
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
