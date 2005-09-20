#include "Environment.h"
#include "SceneManager.h"
#include "GameView.h"

#include <windows.h>
#include <winbase.h>


// シーンnに移動
bool SceneManager::SetScene(int n)
{
	switch(n){
	case 0:
		// 初期化
		m_chrManager.SetCharVisible(PENGUIN1, false);
		m_chrManager.SetCharVisible(PENGUIN2, false);
		m_chrManager.SetCharVisible(PENGUIN3, false);
		m_chrManager.SetCharVisible(PUCK1, false);
		m_chrManager.SetCharVisible(PUCK2, false);
		m_chrManager.SetCharVisible(PUCK3, false);
		m_chrManager.SetCharVisible(PUCK4, false);
		//[AS]:初期化時は全部不可視にするってのがいいかも．
		m_chrManager.SetCharVisible(EXPERIMENT0, false);
		env.iLanguage = PH_LANG_JAPANESE;

		break;

	case 1://言語選択画面
		break;
	
	case 2:
		env.iGraspFrame = 0;
		break;

	case 3:
		m_chrManager.SetCharVisible(EXPERIMENT0, true);
		break;

	case 4:
		break;


	case 5:
		break;
	case 8: //PenguinHockey
		
		//debugYI たぶんobjectsの方の位置も初期化しなくちゃいけない
		//つまり、ここでm_chrManagerで各キャラクタの位置を設定すればよい？

		m_chrManager.SetCharVisible(PENGUIN1, true);
		m_chrManager.SetCharVisible(PENGUIN2, true);
		m_chrManager.SetCharVisible(PENGUIN3, true);
		m_chrManager.SetCharVisible(PUCK1, true);
		m_chrManager.SetCharVisible(PUCK2, true);
		m_chrManager.SetCharVisible(PUCK3, true);
		m_chrManager.SetCharVisible(PUCK4, true);
		
		m_view->game.InitArrangement(); // キャラクタの位置を初期化
		break;

	case 9: //映像キャリブレーション画面
		cout << "setscene 9 " << endl;
		break;
	default:
		break;
	}

	//debug とりあえず

	m_nCurScene = n;

	return true;
}

// キー入力が発生したとき
void SceneManager::KeyPressed(int code)
{
	//Sceneに関係なく働くキー
	switch(code) {

	case 13: //[Enter]
		SetScene(0);
		break;

	case 32: //[Space] タイトル画面→言語選択
		SetScene(1); 
		break;

	case 'c':
		if(m_nCurScene == 0) CalibSpidar();		
		break;

	case 'e':
		m_lang = PH_LANG_ENGLISH;
		env.iLanguage = PH_LANG_ENGLISH;
		SetScene(2);
		break;

	case 'g': //とりあえずPHを走らせる
		cout << "PH" << code << endl;
		SetScene(8);
		break;

	case 'p':
		m_lang = PH_LANG_CHINESE;
		env.iLanguage = PH_LANG_CHINESE;
		SetScene(2);
		break;

	case 'i':
		SetScene(0); //debug
		break;

	case 'j':
		m_lang = PH_LANG_JAPANESE;
		env.iLanguage = PH_LANG_JAPANESE;
		SetScene(2);
		break;

	case 'k':
		m_lang = PH_LANG_KOREAN;
		env.iLanguage = PH_LANG_KOREAN;
		SetScene(2);
		break;

	case 's':
		m_lang = PH_LANG_SPANISH;
		env.iLanguage = PH_LANG_SPANISH;
		SetScene(2);
		break;

	case 'v': //映像キャリブレーション画面
		SetScene(9); 
		break;

	default:
		break;
	}

}

// イベントが発生したとき
void SceneManager::EventFunc(int n)
{
}

void SceneManager::SetGameView(GameView *view)
{
	m_view = view;
	m_chrManager.SetGame(&(view->game));
}

//毎回の描画ループで回ってくる
void SceneManager::Loop() {

	env.view->game.Draw();

	// 左右を黒でクリッピング
	glDisable(GL_DEPTH_TEST);
	glLoadMatrixf(env.view->game.afClipL);
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(0,0,0,1));
	glMaterialfv(GL_FRONT, GL_DIFFUSE , Vec4f(0,0,0,1));
	glMaterialfv(GL_FRONT, GL_AMBIENT , Vec4f(0,0,0,1));
	glBegin(GL_POLYGON);
	glVertex3fv(Vec3f(-10,-10, -1));
	glVertex3fv(Vec3f(0,-10, -1));
	glVertex3fv(Vec3f(0,10, -1));
	glVertex3fv(Vec3f(-10,10, -1));
	glEnd();
	glLoadMatrixf(env.view->game.afClipR);
	glBegin(GL_POLYGON);
	glVertex3fv(Vec3f(0,-10, -1));
	glVertex3fv(Vec3f(10,-10, -1));
	glVertex3fv(Vec3f(10,10, -1));
	glVertex3fv(Vec3f(0,10, -1));
	glEnd();
	glEnable(GL_DEPTH_TEST);
	
}
