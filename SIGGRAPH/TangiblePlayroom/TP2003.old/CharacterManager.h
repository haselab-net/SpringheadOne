#ifndef CHARACTERMANAGER_H
#define CHARACTERMANAGER_H

#include "Game.h" 

#include "physics.h" // キャラクタの衝突判定の有無、位置の設定をするためにobjectsを参照

#include <Base/Utilities.h>


extern HaObjects objects;

//debugYI エレガントじゃないがとりあえず
// キャラクタを表す定数
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
	
	// キャラクタの衝突判定の有無の切り替え	
	bool SetCharCollisionable(int id, bool b); 
	
	// キャラクタの位置設定
	bool SetCharPosition(int id, Affinef af); 
	
	// キャラクタの表示非表示の切り替え
	bool SetCharVisible(int id, bool b); 

	void SetGame(Game *game);
};


#endif // #ifndef CHARACTERMANAGER_H

