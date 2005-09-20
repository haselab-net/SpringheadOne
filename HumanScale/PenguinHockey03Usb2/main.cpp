// wrl2view: VRML2.0 file viewer	Copyright (c)1999 Mamoru Shiroki
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "game.h"
#include "gameView.h"
#include "DispNode.h"
#include "physics.h"
#include "environment.h"
#include <Render/RDGlutView.h>

using namespace std;

static GLfloat	spinY = 0;

//static GLfloat	spinY = 180;/* �J�����̏����p�x�ipan�j*/

Environment env; //���ݒ�N���X
GameView view;


////////////////////////////////////////////////////////////////////////////////
//	Game Management functions


////////////////////////////////////////////////////////////////////////////////

//�l�b�g���[�N�ڑ��̏�����
bool InitNetworks()	{
	//�l�b�g���[�N�ڑ�
	if (env.bClient){
		view.game.mode = Game::CLIENT;
		view.game.csock.server = env.nameServer;
		cout << "�N���C�A���g���[�h�� " << env.nameServer << " �ɐڑ���...";
		for(int i=0; i<Game::NCLIENT; ++i){
			view.game.csock.port = Game::PORTBASE + i;
			view.game.csock.Connect();
		}
		if (!view.game.csock.sock.IsConnected()) {
			cout << "���s���܂����I"<< endl <<"�v���O�������I�����܂��B" << endl;
			return false;
		} else {
			cout << "�ڑ�����"<< endl;
			view.Init("Display Client");
			return true;
		}
	}else{
		cout << "�T�[�o�[���[�h�ŋN����...";
		view.game.mode = Game::SERVER;
		view.Init("Game Server");
		return true;
	}
}


//	main

int main(int argc, char *argv[])
{
	//���ݒ�t�@�C���ǂݍ���
	if (argc >= 2){
		env.filename = argv[1];
	} else {
		cout << "�N���I�v�V�����Ɋ��ݒ�t�@�C�������w�肳��Ȃ������̂� env.txt ���g�p���܂��B" << endl;
		env.filename = "env.txt";
	}
	if (env.Load()) {
		cout << "���ݒ�t�@�C�� " << env.filename << " �ǂݍ��݊���" <<endl;
	} else {
		exit(0);	
	}

	//�l�b�g���[�N�ڑ��̏�����
	if (!InitNetworks()) {
		return -1;
	}

	if (env.bUseGraphics) { //���󂱂̃p�����[�^�͏��TRUE�����ҁB
//		GameView view;//�O���[�o����
		view.WindowPosition(10,10,300,200);
		//�ߋ��ł͂����Ƀl�b�g���[�N�ڑ��̏����������݁B
		//�V�[���̏�����
		UTRef<TSts> root = LoadWrl("media/world.wrl");
		InitList(root);
		DispNodeBuilder builder;
		builder.Build(root);
		view.game.dnTree = builder.Root();
		view.game.dnTree->Dump(cout);
		view.game.Generation(); //�����ʒu

		if (!env.bClient) {
//			InitPhysics(false);
			InitHaptics();
		}

		view.Run();

//#else
	} else {
		InitPhysics(true); //����̓e�X�g���[�h�̃O���t�B�b�N�X������
	}	//env.bUseGraphics
//#endif
	
	return 0;
}