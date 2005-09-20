#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include "game.h"
#include "DispNode.h"
#include "physics.h"
#include "SndClass.h"
#include <Render/RDGlutView.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//	GLUT callback functions

class GameView: public RDGlutView{
	float	scale; 
	float	spinX;
	int mouseX, mouseY, mouseButton;
	Vec2f arrowKey;
	enum ArrowState{
		MOVE_CAM,		//	�J�����̕��s�ړ�
		ZOOMROTATE_CAM,		//	�J������������ɉ�]������D
		MOVE_CLIPL,		//	�N���b�s���O�I�u�W�F�N�g
		MOVE_CLIPR,		//	�N���b�s���O�I�u�W�F�N�g
		MOVE_SCREEN,	//	�X�N���[���̈ړ�
	};
	ArrowState arrowMode;
	float delta;
public:
	Game game;
	GameSndSystem *gssystem;
	GameView();
	void Init(char* win);
	virtual void OnIdle();
	virtual void OnDraw();
	void RotateView();
	virtual void OnSize(int width, int height);
	virtual void OnOpen();
	virtual void OnMouse(Button button, bool state, int x, int y);
	void OnMouseMove(bool on, int x, int y);
	virtual void OnKeyboard(int code, int x, int y);
	virtual void OnSpecialKey(int code, int x, int y);
	static void MenuFuncEntry(int key);
	void MenuFunc(int key);
	void OnArrowKey();
private:
//	bool bFullscreen;
};

#endif	//	 GAMEVIEW_H
