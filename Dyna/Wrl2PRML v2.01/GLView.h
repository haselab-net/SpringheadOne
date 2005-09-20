#ifndef TGLView_H
#define TGLView_H

#include <fstream>
//#include "GeoNode.h"
#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>

using namespace std;
using namespace LVRM;

class Sphere;
class TGLView: public RDGlutView{
	ofstream fout;
	int writeCounter;

	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	
	//	�Փ˔���
	double radius;				//	�A�o�^�[�̔��a
	TPenaltyDetectors penChekers;//	�N���ʃ`�F�b�J�[
	std::vector<Shape*> shapes;	//	�`�󂽂�

	//	�A�o�^�[�̑���
	Affined afBody;				//	�A�o�^�[�̈ʒu
	Affined afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3d vel;					//	�A�o�^�[�̑��x
	Vec3d dvel;					//	�A�o�^�[�̑��x�̕ω�
	Sphere* sphere;
		
	bool bDrawCollision;
public:
	TGeoNodes allNodes;
	Vec3d clearColor;			//	��̐F
	bool initialFullScreen;
	bool initialWideScreen;

	
	TGLView();
	~TGLView();
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	bool OnTimer();
	void Step();

	void InitCollisionDetector();
	void UsoPenalty(double dt, Vec3d pen, TPenaltyDetector& pc, TGeoNode* g1, TGeoNode* g2, Vec3d near1, Vec3d near2, Vec3d& normal);
	bool CalcPenaltySwitch(TPolytope* p1, TPolytope* p2, Vec3d interior);
	void CalcPenalty(Vec3d& pen, Vec3d& penMom, Vec3d& vel, Vec3d& velMom, double& area, Vec3d& normal, Vec3d interior,
		TGeoNode* g1, TPolytope* p1, TSupport* s1, Vec3d near1,
		TGeoNode* g2, TPolytope* p2, TSupport* s2, Vec3d near2);
	void FindInterface(TCollideInterface& rv, TPolytope* poly, TSupport* s, const Vec3d& normal);
};

#endif

