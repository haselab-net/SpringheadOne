#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "CharacterManager.h"

//debugYI ���܂�G���K���g����Ȃ����ǂƂ肠����
#define PH_LANG_JAPANESE 0
#define PH_LANG_ENGLISH  1
#define PH_LANG_SPANISH  2
#define PH_LANG_KOREAN   3
#define PH_LANG_CHINESE  4


/*

  Scene1: ����I���ƃL�����u���[�V����
	�\��
	"Choose your language"�̃C���[�W
	
	����
	j : Japanese
	e : English
	s : Spanish
	k : Korean
	b : Chinese

	c : Calibration
	l : ������̓���
	r : �E��O�̓���
	ESC exit



*/

class GameView;
class SceneManager{

private:

	CharacterManager m_chrManager;
	int m_nCurScene; // ���݂̃V�[���ԍ�
	GameView *m_view;
	int m_lang; // �C���X�g���N�V�����ŕ\������錾��
public:

	void Loop(); //����̕`�惋�[�v�ŉ���Ă���

	bool SetScene(int n); // �V�[��n�Ɉړ�

	int GetCurScene(){ return m_nCurScene; } // ���݂̃V�[���ԍ����擾

	void KeyPressed(int code); // �L�[���͂����������Ƃ�GameView����Ă΂��
	void EventFunc(int n); // �C�x���g�����������Ƃ�Game����Ă΂�� (�Q�[���I�[�o�[�Ƃ�)

	void SetGameView(GameView *view);
};

#endif // #ifdef SCENEMANAGER_H