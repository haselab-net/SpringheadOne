#ifndef TGLView_H
#define TGLView_H

#include <Solid/Solid.h>
#include <Penalty.h>

#include "GeoNode.h"
#include <Render/RDGlutView.h>
#include <vector>
#include "communication.h"

using namespace std;
using namespace LVRM;

class TGLView: public RDGlutView{
	int mouseX, mouseY;
	int spinX, spinY;
	Button mouseButton;
	double setting[6];
	Affinef R;
	double fov_v;

	//	�Փ˔���
	float radius;				//	�A�o�^�[�̔��a
	TPenaltyCheckers penChekers;//	�y�i���e�B�[�`�F�b�J�[
	std::vector<Vec3d> normals;	//	�Փ˖ʂ̖@���x�N�g������
	Vec3d reflect;				//	���x�̔��˃x�N�g���D
	//	�A�o�^�[�̑���
	Affinef afEye;				//	�A�o�^�[���猩���ڂ̈ʒu
	Vec3f vel;					//	�A�o�^�[�̑��x
	Vec3f dvel;					//	�A�o�^�[�̑��x�̕ω�
	
	bool bDrawNormal;
	bool bDrawCollision;
public:
	Affinef afBody;				//	�A�o�^�[�̈ʒu\begin{abstract}\end{abstract}
	float aspect_x,aspect_y;
	TGeoNodes allNodes;
	Vec3f clearColor;			//	��̐F
	bool bSender;
	CmdSocket cmdSocket;

	TGLView();
	void OnDraw();
	void OnSize(int width, int height);
	void OnKeyboard(int key, int x, int y);
	void OnMouse(Button button, bool on, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	void OnOpen();
	bool OnTimer();
	void Step();
	void SetParam();
	bool LoadSettings();
	void InitCollisionDetector();
};

#endif

