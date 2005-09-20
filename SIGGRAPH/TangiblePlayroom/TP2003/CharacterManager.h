#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include "Game.h" 

#include "physics.h" // �L�����N�^�̏Փ˔���̗L���A�ʒu�̐ݒ�����邽�߂�objects���Q��

#include <Base/Utilities.h>


extern HaObjects objects;

//debugYI �G���K���g����Ȃ����Ƃ肠����
// �L�����N�^��\���萔
#define PENGUIN1		0
#define PENGUIN2		1
#define PENGUIN3		2
#define PUCK1			10
#define PUCK2			11
#define PUCK3			12
#define PUCK4			13
#define RACKET			20
/*
#define SPRITE0			50
#define SPRITE1			51
#define SPRITE2			52
#define SPRITE3			53
#define SPRITE4			54
*/
//add by AS
#define EXPERIMENT0		60

class CharacterManager{

private:
	Game *m_game;

public:
	
	// �L�����N�^�̏Փ˔���̗L���̐؂�ւ�	
	bool SetCharCollisionable(int id, bool b); 
	
	// �L�����N�^�̈ʒu�ݒ�
	bool SetCharPosition(int id, Affinef af); 
	
	// �L�����N�^�̕\����\���̐؂�ւ�
	bool SetCharVisible(int id, bool b); 

	void SetGame(Game *game);
};


#endif // #ifndef CHARACTERMANAGER_H

