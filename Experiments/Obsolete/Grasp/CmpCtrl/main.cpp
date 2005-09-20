#include <HIS/HISpidar8.h>
#include <HIS/HISpidar4Quad.h>
#include <Device/DRContecIsaDa.h>
#include <Device/DRContecIsaCounter.h>
#include <WinBasis/WBMMTimer.h>

#include "app.h"

#include <Render/RDGLUTView.h>
#include <WinBasis/WBWin.h>
#include <algorithm>
#include <gl/gl.h>


#pragma hdrstop

class TestView: public RDGlutView{
public:
	UTRef<THapticApp> app;
	Affinef afCam;
	float nRatio[2];
	float pRatio[2];
	int curDevice, curMode;
	TestView():curDevice(0), curMode(0){
		app = new THapticApp;
		for(int i=0; i<2; i++){
			nRatio[i] = 0;
			pRatio[i] = 0;
		}
	}
	virtual void OnOpen(){
		using namespace PTM;
		glEnable(GL_LIGHT0);
		TVector<4, float> lightPos = VecInit << 0.0f << 1.0f << 1.0f << 0.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

		afCam.PosZ() = 0.26f;
		afCam.PosY() = 0.0f;
		afCam.LookAtGL(Vec3f());
	}
	virtual void OnIdle(){
		static int i;
		i++;
		if (i>10){
			Invalidate();
			i = 0;
		}
	}
	virtual void OnDraw(){
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		Affinef af = afCam.inv();
		af.Ez() *= 0.0001f;
		glLoadMatrixf(af);
		app->Draw();
		Flip();
	}
	virtual void OnSize(int w, int h){
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		Affinef afProj(Vec3f(0,0,500), Vec2f(w, h), 0.1f, 10);
		glLoadMatrixf(afProj);
		glMatrixMode(GL_MODELVIEW);
	}
	virtual void OnClose(){
		printf("OnClose\n");
	}
	virtual void OnMouse(Button b, bool s, int x, int y){
		printf("OnMouse %s %d %d\n", s ? "on" : "off", x, y);
	}
	virtual void OnKeyboard(int code, int x, int y){
		if (code == 0x1b){
			app = NULL;
			Exit();
		}else if (code == 'c'){
			app->Reset();
			std::cout << "c: Reset object position" << std::endl;
		}else if (code == 'C'){
			app->Calib();
			std::cout << "C: Calibration" << std::endl;
		}else if (code == 't'){
			app->timer.Create();
			std::cout << "t: Multi Medeia Timer" << std::endl;
		}else if (code == 'T'){
			app->timer.Thread();
			std::cout << "T: Thread" << std::endl;
		}else if (code == 'n' || code == 'P'){
			app->devices.current++;
			if (app->devices.current > app->devices.size()-1) app->devices.current = 0;
		}else if (code == 'p' || code == 'N'){
			app->devices.current--;
			if (app->devices.current < 0) app->devices.current = app->devices.size()-1;
		}else if (code == 'g'){
			std::cout << "g: toggle Gravity ";
			if (app->gravity.norm()){
				app->gravity = Vec3f();
				std::cout << "off";
			}else{
				app->gravity.Y() = -1.0f;
				std::cout << "on";
			}
			std::cout << std::endl;
		}else if (code == 'm'){
			app->NormalMode();
			std::cout << "Normal Mode" << std::endl;
		}else if (code == 'k'){
			app->KawaiYoshikawaMode();
			std::cout << "Kawai and Yoshikawa's Mode" << std::endl;
		}else if (code == 'r' || code == 'R' || code == 'l' || code == 'L'){
			if (code == 'l'){ curDevice = 0; curMode = 0;}
			if (code == 'L'){ curDevice = 0; curMode = 1;}
			if (code == 'r'){ curDevice = 1; curMode = 0;}
			if (code == 'R'){ curDevice = 1; curMode = 1;}
			std::cout << "Current device is " << curDevice << "mode:"
				<< (curMode==1 ? "parallel" : "normal") << std::endl;
		}else if ('0' <= code && code <='9' || code == '-' || code == '!'
			|| code == '"' || code == '#' || code == '$' || code == '%'){
			float r = 1;
			if ('1' <= code && code <= '9') r = float(code - '0') / 10.0f;
			if (code == '-') r = 0;
			if (code == '!') r = 1.1f;
			if (code == '"') r = 1.2f;
			if (code == '#') r = 1.3f;
			if (code == '$') r = 1.4f;
			if (code == '%') r = 1.5f;
			if (curMode==1){
				pRatio[curDevice] = r;
			}else{
				nRatio[curDevice] = r;
			}
		}
		if (code == 'h'){
			app->HaseMode(nRatio, pRatio);
			std::cout << "HaseMode ";
			std::cout << "n0:" << nRatio[0] << " p0:" << pRatio[0];
			std::cout << "  n1:" << nRatio[1] << " p1:" << pRatio[1] << std::endl;
		}
		const float delta = 0.01f;
		Vec3f devPos = (Vec3f&)app->devices[app->devices.current]->devicePosOrg;
		if (code == 'x'){
			devPos.X() += delta;
		}
		if (code == 'X'){
			devPos.X() -= delta;
		}
		if (code == 'y'){
			devPos.Y() += delta;
		}
		if (code == 'Y'){
			devPos.Y() -= delta;
		}
		if (code == 'z'){
			devPos.Z() += delta;
		}
		if (code == 'Z'){
			devPos.Z() -= delta;
		}
		(Vec3f&)app->devices[app->devices.current]->devicePosOrg = devPos;

		if (code == 's'){
			app->StartSave();
		}
		if (code == 'e'){
			app->StopSave();
		}
	}

};

void main(){
	TestView view;
	view.WindowPosition(10,10, 500, 400);
	view.Init("OpenGL Window");
	view.Run();
}
