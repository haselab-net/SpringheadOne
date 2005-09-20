#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include "../Share/WalkThroughTools.h"

#include <Render/RDGlutView.h>
#include <vector>

using namespace std;
using namespace LVRM;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	
	//	衝突判定
	float radius;				//	アバターの半径
	TClosestPointDetectors cpds;//	最近傍点の検出器
	TReflectNormalCalculator rnc;//	反射面の法線の計算機
	TPenaltyCanceler penCancel;	//	
	//	アバターの属性
	Affinef afBody;				//	アバターの位置
	Affinef afEye;				//	アバターから見た目の位置
	Vec3f vel;					//	アバターの速度
	Vec3f dvel;					//	アバターの速度の変化
	
	bool bDrawNormal;
	bool bDrawCollision;
public:
	TGeoNodes allNodes;
	Vec3f clearColor;			//	空の色
	
	TGLView();
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	bool OnTimer();
	void Step();

	void InitCollisionDetector();
};

#endif

