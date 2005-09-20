#include "HIS/HISpidar4quad.h"
#include "Device/DRContecIsaDa.h"
#include "Device/DRContecIsaCounter.h"
#include <conio.h>
#include <iostream>
#include <Render/RDGlutView.h>
#include <Base/Debug.h>
#include <WinCmp/MultiMedia/CMMTimer.h>
#include <GL/glut.h>

//#lib "glut32.lib"
Affinef afCam;

using namespace VRRend;
using namespace WinCmp;
using namespace VRHaptic;
using namespace VRBase;

		static Affinef afObj;

		static Vec3f vel(0,0,0);
		static Vec3f acc(0,0,0);
		static Vec3f force(0,0,0);
		static Vec3f omega(Rad(0.2),Rad(0),Rad(0.3));  //�p���x�����l

		const Vec3f g(0,-9.8,0);    //�d�͉����x
		const float r1=0.03;
		const float r2=0.05;
		const float m1=10*r1*r1*3.14;
		const float m2=10*r2*r2*3.14;
		const float dt = 0.001;
		static float l1=(r1+r2)*(m2/(m1+m2));     //�e�~�̒��S����d�S�܂ł̋���
		static float l2=(r1+r2)*(m1/(m1+m2));
        static float Ixx=(m1*l1*l1+m2*l2*l2)+2*(m1*r1*r1+m2*r2*r2)/5;  //�e���̊������[�����g�v�Z
        static float Izz=(m1*l1*l1+m2*l2*l2)+2*(m1*r1*r1+m2*r2*r2)/5;
        static float Iyy=2*(m1*r1*r1+m2*r2*r2)/5;
        static Vec3f ex(Ixx,0,0);
		static Vec3f ey(0,Iyy,0);
		static Vec3f ez(0,0,Izz);
		static Matrix3f I(ex,ey,ez);
		static Vec3f Iw0=I * omega;
        const float wall=0.3;    //�e�ǂ̌��_����̋���
		const float k = 500;
		const float kSP = 100;
		float ratio=0.2;
		Vec3f SPos;
		float SpR=0.03;   //�X�p�C�_�[�̃|�C���^�[�̋��̔��a
		
		GLfloat red[]={1.0,0.0,0.0,1.0};
		GLfloat white[]={1.0,1.0,1.0,1.0};
		GLfloat light0pos[] = {3.0, 3.0, 3.0, 1.0};
		
		HISpidar4quad spidar;
		
class TestView: public RDGlutView{
public:
	virtual void OnOpen(){
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		afCam.PosZ() = 1.8f;
		afCam.PosY() = 1.0f;
		afCam.LookAtGL(Vec3f());
	}
	virtual void OnIdle(){
		Invalidate();
	}
	virtual void OnDraw(){

//�`�敔��OpenGL
		
		glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
		glColor3f(0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(afCam.inv());

		glLightfv(GL_LIGHT0,GL_POSITION,light0pos);

		glPushMatrix();
			glMultMatrixf(afObj);
			glPushMatrix();
				glMultMatrixf(Affinef(0,-l1,0));
				glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,white);
				glutSolidSphere(r1, 32, 16);
			glPopMatrix();
			glMultMatrixf(Affinef(0,l2,0));
			glutSolidSphere(r2, 32, 16);
		glPopMatrix();

		glPushMatrix();
			glBegin(GL_QUADS);
			    glVertex3d(wall,-wall,wall);
				glVertex3d(wall,-wall,-wall);
			    glVertex3d(-wall,-wall,-wall);
			    glVertex3d(-wall,-wall,wall);
			glEnd();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(SPos.X(),SPos.Y(),SPos.Z());
			glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,red);
			glutSolidSphere(SpR,32,16);
		glPopMatrix();
			
		Flip();

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
volatile int c=0;
CMMTimer timer;
void timerFunc(void* arg){
#if 1

		spidar.Update();
		Vec3f pos = spidar.GetPos();
		SPos = pos;

		Matrix3f dangle(dt*omega.norm(), omega.unit());  //�p���x�̕ω������p�x�̕ω����Ɍv�Z���Ȃ���
		Affinef afObjNext;
		Matrix3f rot = afObj.Rot();              //�A�t�B���s��̉�]���������o��

		afObjNext.Rot() = dangle * rot;          //�A�t�B���s��̉�]�����X�V
		afObjNext.Pos() = afObj.Pos() + vel*dt;  //�A�t�B���s��̕��s�ړ������X�V
		afObj = afObjNext;                       //�ЂƂO�̃A�t�B���s��Ɍ��݂̂���
		rot = afObj.Rot();                       //�A�t�B���s��̉�]���������o��
	

        
		Vec3f Ca = afObj * Vec3f(0,l2,0);    //��a�̒��S��world���W���v�Z
		Vec3f Cb = afObj * Vec3f(0,-l1,0);   //��b�̒��S��world���W���v�Z

		Vec3f CaDis = pos-Ca;    //��a�̒��S�X�p�C�_�[�Ƃ̋���
		Vec3f CbDis = pos-Cb;    //��b�̒��S�X�p�C�_�[�Ƃ̋���
		Vec3f forceS = Vec3f(0,0,0);  //�͂͌v�Z����O�̓[��
		Vec3f forceT;
		force = Vec3f(0,0,0);
		Matrix3f Id = rot * I * rot.trans();


//�ȉ��A�ǂƂ���܂̂����蔻��		
//-X a
		if (Ca.X() <= -wall+r2){
			force = k * Vec3f(-Ca.X()-wall+r2,0,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,-l2,0))) * dt;
		}
//-X b	    
		if (Cb.X() <= -wall+r1){
			force = k * Vec3f(-Cb.X()-wall+r1,0,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,l1,0))) * dt;
		}
//+X a
		if (Ca.X() >= wall-r2){
			force = k * Vec3f(-Ca.X()+wall-r2,0,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,-l2,0))) * dt;
		}
//+X b	    
		if (Cb.X() >= wall-r1){
			force = k * Vec3f(-Cb.X()+wall-r1,0,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,l1,0))) * dt;
		}
		
//-Y a		
		if (Ca.Y() <= -wall+r2){
			force = k * Vec3f(0,-Ca.Y()-wall+r2,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,-l2,0))) * dt;

		}
//-Y b	    
		if (Cb.Y() <= -wall+r1){
			force = k * Vec3f(0,-Cb.Y()-wall+r1,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,l1,0))) * dt;
		}
		
//+Y a		
		if (Ca.Y() >= wall-r2){
			force = k * Vec3f(0,-Ca.Y()+wall-r2,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,-l2,0))) * dt;
		}
//+Y b	    
		if (Cb.Y() >= wall-r1){
			force = k * Vec3f(0,-Cb.Y()+wall-r1,0);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,l1,0))) * dt;
		}
//-Z a
		if (Ca.Z() <= -wall+r2){
			force = k * Vec3f(0,0,-Ca.Z()-wall+r2);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,-l2,0))) * dt;
		}
//-Z b
		if (Cb.Z() <= -wall+r1){
			force = k * Vec3f(0,0,-Cb.Z()-wall+r1);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,l1,0))) * dt;
		}

//+Z a
		if (Ca.Z() >= wall-r2){
			force = k * Vec3f(0,0,-Ca.Z()+wall-r2);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,-l2,0))) * dt;
		}
//+Z b
		if (Cb.Z() >= wall-r1){
			force = k * Vec3f(0,0,-Cb.Z()+wall-r1);
			forceT += force;
			Iw0 += (force ^ (rot * Vec3f(0,l1,0))) * dt;
		}


   //�X�p�C�_�[�Ƃ���܂̂����蔻��

		if (CaDis.norm()<r2+SpR){
			force = -kSP * CaDis.unit() * (r2 +SpR - CaDis.norm());  //�߂荞�ݗʂɔ�Ⴕ���͂�Ԃ�
			Iw0 += (force ^ (rot * Vec3f(0,-l2,0))) * dt;
			forceS = -1 * force;
			forceT += force;


		}
		
		if (CbDis.norm()<r1+SpR){
			force = -kSP * CbDis.unit() * (r1 +SpR - CbDis.norm());
			Iw0 += (force ^ (rot * Vec3f(0,l1,0))) * dt;
			forceS = -1 * force;
			forceT += force;

		}
		acc = forceT/(m1+m2) + g;  //����܂̉����x�v�Z
		vel += acc*dt;     //����܂̑��x�v�Z

		omega = Id.inv() * Iw0;
		
		if(pos.Y()<-wall)  forceS += Vec3f(0,1000,0)*(-wall-pos.Y());  //�X�p�C�_�[�ɕԂ��͂��v�Z
		if(pos.Y()>wall)  forceS += Vec3f(0,-1000,0)*(-wall+pos.Y());  //�X�p�C�_�[�ɕԂ��͂��v�Z
		if(pos.X()<-wall)  forceS += Vec3f(1000,0,0)*(-wall-pos.X());
		if(pos.X()>wall)  forceS += Vec3f(-1000,0,0)*(-wall+pos.X());
		if(pos.Z()<-wall)  forceS += Vec3f(0,0,1000)*(-wall-pos.Z());
		spidar.SetForce(forceS,ratio);  //�X�p�C�_�[�̗͂��o��
		
		vel *= 0.9999;   //���C�炵������
		Iw0 *= 0.9999;


#endif

}
void main(){
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));
	devMan.RPool().Register(new DRContecIsaCounter(0x210));
	devMan.Init();
	
	std::cout << devMan;
	
    //�X�p�C�_�[������
	//	���[�^�̎��t���ʒu. ���[�^�������̂Ɏ��t�����Ă���ꍇ�́C
	#define PX	0.265f	//	x�����̕ӂ̒���/2
	#define PY	0.265f	//	y�����̕ӂ̒���/2
	#define PZ	0.265f	//	z�����̕ӂ̒���/2
	Vec3f motorPos[][4] = {	//	���[�^�̎��t���ʒu(���S�����_�Ƃ���)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)},
		{Vec3f( PX, PY, PZ),	Vec3f(-PX, PY,-PZ), Vec3f( PX,-PY,-PZ), Vec3f(-PX,-PY, PZ)}
	};

	spidar.Init(devMan, motorPos[0], 0.5776f, 2.924062107079e-5f, 0.5f, 20.0f);

	spidar.Calib();

	afObj.Pos() = Vec3f(0.1,0.18,0);  // ����܂̈ʒu���W�����l

	timer.Set(timerFunc, NULL);  //�^�C�}�[���Z�b�g
	timer.Resolution(1);     //�^�C�}�[�̑��x
	timer.Interval(1);
//	timer.Create();
	timer.Thread();
//	for(int i=0; i<1000;i++){
//		std::cout << c << std::endl;
//	}
	TestView view;
	view.WindowPosition(10,10, 300, 200);
	view.Init("OpenGL Window");
	view.Run();

	
}
