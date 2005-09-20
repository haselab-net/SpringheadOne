#ifndef TGLView_H
#define TGLView_H

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
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	
	//	衝突判定
	double radius;				//	アバターの半径
	TPenaltyDetectors penChekers;//	侵入量チェッカー
	std::vector<Shape*> shapes;	//	形状たち

	//	アバターの属性
	Affined afBody;				//	アバターの位置
	Affined afEye;				//	アバターから見た目の位置
	Vec3d vel;					//	アバターの速度
	Vec3d dvel;					//	アバターの速度の変化
	Sphere* sphere;
		
	bool bDrawCollision;
public:
	TGeoNodes allNodes;
	Vec3d clearColor;			//	空の色
	
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
	bool CalcPenaltySwitch(TPolytope* p1, TPolytope* p2, Vec3d interior);
	void CalcPenalty(Vec3d& force, Vec3d& torque, double& area, Vec3d& normal, Vec3d interior,
		TGeoNode* g1, TPolytope* p1, TSupport* s1, Vec3d near1,
		TGeoNode* g2, TPolytope* p2, TSupport* s2, Vec3d near2);
/*	Vec3d CalcFriction(Vec3d& force, Vec3d& torque, TPenaltyDetector& pc, 
		TGeoNode* g1, TGeoNode* g2);	//(5/6追加)*/
	void FindInterface(TCollideInterface& rv, TPolytope* poly, TSupport* s, const Vec3d& normal);
};

#endif

