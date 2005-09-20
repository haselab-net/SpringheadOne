#include "Environment.h"
#include "SceneManager.h"
#include "GameView.h"

#include <windows.h>
#include <winbase.h>


// �V�[��n�Ɉړ�
bool SceneManager::SetScene(int n)
{
	switch(n){
	case 0:
		// ������
		m_chrManager.SetCharVisible(PENGUIN1, false);
		m_chrManager.SetCharVisible(PENGUIN2, false);
		m_chrManager.SetCharVisible(PENGUIN3, false);
		m_chrManager.SetCharVisible(PUCK1, false);
		m_chrManager.SetCharVisible(PUCK2, false);
		m_chrManager.SetCharVisible(PUCK3, false);
		m_chrManager.SetCharVisible(PUCK4, false);
		//[AS]:���������͑S���s���ɂ�����Ă̂����������D
		m_chrManager.SetCharVisible(EXPERIMENT0, false);
		env.iLanguage = PH_LANG_JAPANESE;

		break;

	case 1://����I�����
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
		
		//debugYI ���Ԃ�objects�̕��̈ʒu�����������Ȃ����Ⴂ���Ȃ�
		//�܂�A������m_chrManager�Ŋe�L�����N�^�̈ʒu��ݒ肷��΂悢�H

		m_chrManager.SetCharVisible(PENGUIN1, true);
		m_chrManager.SetCharVisible(PENGUIN2, true);
		m_chrManager.SetCharVisible(PENGUIN3, true);
		m_chrManager.SetCharVisible(PUCK1, true);
		m_chrManager.SetCharVisible(PUCK2, true);
		m_chrManager.SetCharVisible(PUCK3, true);
		m_chrManager.SetCharVisible(PUCK4, true);
		
		m_view->game.InitArrangement(); // �L�����N�^�̈ʒu��������
		break;

	case 9: //�f���L�����u���[�V�������
		cout << "setscene 9 " << endl;
		break;
	default:
		break;
	}

	//debug �Ƃ肠����

	m_nCurScene = n;

	return true;
}

// �L�[���͂����������Ƃ�
void SceneManager::KeyPressed(int code)
{
	//Scene�Ɋ֌W�Ȃ������L�[
	switch(code) {

	case 13: //[Enter]
		SetScene(0);
		break;

	case 32: //[Space] �^�C�g����ʁ�����I��
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

	case 'g': //�Ƃ肠����PH�𑖂点��
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

	case 'v': //�f���L�����u���[�V�������
		SetScene(9); 
		break;

	default:
		break;
	}

}

// �C�x���g�����������Ƃ�
void SceneManager::EventFunc(int n)
{
}

void SceneManager::SetGameView(GameView *view)
{
	m_view = view;
	m_chrManager.SetGame(&(view->game));
}

//����̕`�惋�[�v�ŉ���Ă���
void SceneManager::Loop() {

	env.view->game.Draw();

	// ���E�����ŃN���b�s���O
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
