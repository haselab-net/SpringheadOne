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
	
	//	�Փ˔���
	float radius;				//	�A�o�^�[�̔��a
	TClosestPointDetectors cpds;//	�ŋߖT�_�̌��o��
	TReflectNormalCalculator rnc;//	���˖ʂ̖@���̌v�Z�@
	TPenaltyCanceler penCancel;	//	
	//	�A�o�^�[�̑���
	Affinef afBody;				//	�A�o�^�[�̈ʒu
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3f vel;					//	�A�o�^�[�̑��x
	Vec3f dvel;					//	�A�o�^�[�̑��x�̕ω�
	
	bool bDrawNormal;
	bool bDrawCollision;
public:
	TGeoNodes allNodes;
	Vec3f clearColor;			//	��̐F
	
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

