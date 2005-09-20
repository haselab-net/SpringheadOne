#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "CharacterManager.h"

//debugYI あまりエレガントじゃないけどとりあえず
#define PH_LANG_JAPANESE 0
#define PH_LANG_ENGLISH  1
#define PH_LANG_SPANISH  2
#define PH_LANG_KOREAN   3
#define PH_LANG_CHINESE  4


/*

  Scene1: 言語選択とキャリブレーション
	表示
	"Choose your language"のイメージ
	
	入力
	j : Japanese
	e : English
	s : Spanish
	k : Korean
	b : Chinese

	c : Calibration
	l : 左下後の入力
	r : 右上前の入力
	ESC exit



*/

class GameView;
class SceneManager{

private:

	CharacterManager m_chrManager;
	int m_nCurScene; // 現在のシーン番号
	GameView *m_view;
	int m_lang; // インストラクションで表示される言語
public:

	void Loop(); //毎回の描画ループで回ってくる

	bool SetScene(int n); // シーンnに移動

	int GetCurScene(){ return m_nCurScene; } // 現在のシーン番号を取得

	void KeyPressed(int code); // キー入力が発生したときGameViewから呼ばれる
	void EventFunc(int n); // イベントが発生したときGameから呼ばれる (ゲームオーバーとか)

	void SetGameView(GameView *view);
};

#endif // #ifdef SCENEMANAGER_H