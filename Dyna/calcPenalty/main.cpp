#include "calcPenalty.h"
#include <Render/RDGlutView.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace LVRM;

Affinef afCam;


class TestView: public RDGlutView{
public:
	TCalcPenalty calcPen;
	virtual void OnOpen(){
		glEnable(GL_LIGHT0);
		Vec4f lightPos = Vec4f(0.0f, 1.0f, 0.0f, 0.0f);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		afCam.PosZ() = 4.0f;
		afCam.PosY() = 3.0f;
		afCam.LookAtGL(Vec3f());
		Build();
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
		afCam = Affinef(Rad(1), 'x') * afCam;

		glLoadMatrixf(afCam.inv());
		calcPen.Draw();
		Flip();
	}
	virtual void OnSize(int w, int h){
		printf("OnSize %d %d\n", w, h);
		glViewport(0, 0, w, h);

		glFlush();
		
		glMatrixMode(GL_PROJECTION);
		Affinef afProj(Vec3f(0,0,500), Vec2f(w, h), 1, 10000);
		glLoadMatrixf(afProj);
	}
	virtual void OnClose(){
		printf("OnClose\n");
	}
	virtual void OnMouse(Button b, bool s, int x, int y){
		printf("OnMouse %s %d %d\n", s ? "on" : "off", x, y);
	}
	virtual void OnKeyboard(int code, int x, int y){
		TPolygons (&polys)[2] = calcPen.polys;
		printf("OnKey %d\n", code);
		switch(code){
		case 0x1b:
			Exit();
			break;
		case 'X':
			polys[1].vtxs.af = polys[1].vtxs.af * Affinef(Rad(5), 'x');
			break;
		case 'x':
			polys[1].vtxs.af = polys[1].vtxs.af * Affinef(Rad(-5), 'x');
			break;
		case ' ':
			calcPen.normal = (polys[0].vtxs.af.Ey() + polys[1].vtxs.af.Ey()).unit();
			calcPen.force = Vec3f(); calcPen.torque = Vec3f();
			calcPen.CalcPenalty();
			break;
		}
	}
	void Build(){
		TPolygons (&polys)[2] = calcPen.polys;
		polys[0].vtxs.clear();
		polys[0].vtxs.push_back( Vec3f(0,0,0) );
		polys[0].vtxs.push_back( Vec3f(1,0,0) );
		polys[0].vtxs.push_back( Vec3f(1,0,1) );
		polys[0].vtxs.push_back( Vec3f(0,0,1) );
		polys[1].vtxs = polys[0].vtxs;
		polys[0].vtxs.id = "v";
		polys[1].vtxs.id = "w";
		polys[1].vtxs.af = Affinef(-0.2f, -0.1f, -0.2f);
//		polys[1].vtxs.af = Affinef(0, -0.1f, 0);
		polys[0].faces.clear();
		TFace face;
		face.push_back(0);
		face.push_back(1);
		face.push_back(2);
		face.push_back(3);
		polys[0].faces.push_back(face);
		polys[1].faces = polys[0].faces;
		calcPen.normal = Vec3f(0,1,0);
	}
};


void main(){
	TestView view;
	view.WindowPosition(10,10, 300, 200);
	view.Init("OpenGL Window");
	view.Run();
}
