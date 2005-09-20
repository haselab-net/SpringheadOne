#include <Device/DVDeviceManager.h>
#include <Device/DRTokin3D.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <WinBasis/WBWin.h>
#include <Render/RDGlutView.h>
#include <gl/glut.h>

using namespace LVRM;

class TView: public RDGlutView{
public:
	DVDeviceManager devMan;							//	デバイス管理クラス．
	DVOrientationBase* sensor;
	Affinef afCam;
	Matrix3f ori, offsetInv;
	void OnOpen(){
		afCam=Affinef(0,0,10);
		afCam.LookAtGL(Vec3f());

		devMan.RPool().Register(new DRTokin3D);		//	Tokin3Dをデバイス管理クラスに登録
		devMan.Init();								//	デバイスの初期化
		std::cout << devMan;						//	結果の表示
		devMan.VPool().Rent(sensor);
	}
	void OnIdle(){
		sensor->Update();
		sensor->GetMatrix(ori);
		std::cout << ori * offsetInv << std::endl;
		Invalidate();
	}
	void OnDraw(){
		glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
		glColor3f(0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(afCam.inv());

		Affinef af;
		af.Rot() = offsetInv * ori * Matrix3f(Rad(180), 'z');
		glMultMatrixf(af);
		glColor3f(1.0, 1.0, 1.0);
		glutSolidTeapot(-1);
		Flip();
	}
	virtual void OnKeyboard(int code, int x, int y){
		if (code == 0x1b) Exit();
		if (code == ' '){
			offsetInv = ori.inv();
		}
	}
	virtual void OnSize(int w, int h){
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		Affinef afProj(Vec3f(0,0,500), Vec2f(w, h), 1, 10000);
		glLoadMatrixf(afProj);
	}
};

void main(){
	TView view;
	view.Init("Tokin3D");
	view.Run();
}
