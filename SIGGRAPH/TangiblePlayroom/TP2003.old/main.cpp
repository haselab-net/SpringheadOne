
// ====================================================
// PenguinHockey��main()
// ====================================================

#include "game.h"
#include "SceneManager.h"
#include "gameView.h"
#include "DispNode.h"
#include "physics.h"
#include "environment.h"
#include <Render/RDGlutView.h>

using namespace std;

Environment env; //���ݒ�N���X
GameView view;

// =====================================================
//	main()
// =====================================================
int main(int argc, char *argv[])
{
	//���ݒ�t�@�C���ǂݍ���
	if(argc >= 2){
		env.filename = argv[1];
	}else{
		cout << "�N���I�v�V�����Ɋ��ݒ�t�@�C�������w�肳��Ȃ������̂� env.ini ���g�p���܂��B" << endl;
		env.filename = "env.ini";
	}
	if(env.Load()){
		cout << "���ݒ�t�@�C�� " << env.filename << " �ǂݍ��݊���" <<endl;
	}else{
		CleanupPhysics();
		exit(0);
	}

	//view��n��
	env.view = &view;


	//�l�b�g���[�N�ڑ��̏�����
	if(!view.InitNetwork()){
		cout << "�l�b�g���[�N�ڑ��̏������Ɏ��s���܂���" <<endl;
		return -1;
	}

	if(env.bUseGraphics){ //���󂱂̃p�����[�^�͏��TRUE�����ҁB

		//debugYI ������
		// GameView view
		// �������Ă��悭�Ȃ��H

		//�V�[���O���t�̏�����
		UTRef<TSts> root = LoadWrl("media/world.wrl"); // wrl�t�@�C���̓ǂݍ���
		InitList(root);
		DispNodeBuilder builder;
		builder.Build(root);
		
		view.game.dnTree = builder.Root();
		view.game.dnTree->Dump(cout);
		view.game.Generation(); // �L�����N�^�̏�����

		if(!env.bClient){
			//debugYI �폜��
			//InitPhysics(false);
			InitHaptics();
		}
		view.sceneManager.SetGameView(&view);
		view.sceneManager.SetScene(0); // �ŏ��̃V�[���ɏ�����
		view.Run();
	}	
/*
	else{
		//debugYI �폜��
		//InitPhysics(true); //����̓e�X�g���[�h�̃O���t�B�b�N�X������
		
		InitHaptics();

		// �e�X�g���[�h�̃O���t�B�b�N�X
		TestView test_view;
		test_view.WindowPosition(10,10, 300, 200);
		test_view.Init("OpenGL Window");
		test_view.Run();
	}
*/
  //env.bUseGraphics
	
	return 0;
}