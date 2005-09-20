#include "environment.h"
#include <fstream>
#include <conio.h>
#include <iostream>
#include <string.h>

/*
 environment.cpp
 �������ݒ�N���X
*/

using namespace std;

//���t�@�C���̓ǂݍ���
bool Environment::Load() {
	char temp[255],test[]="UseSound";

	//�f�t�H���g�l�B���̂����t�@�C���ǂݍ��ݗ\��B
	bFullscreen = false;  //�t���X�N���[���`��
	bHaptics = true;     //�͊o���[�h


	//���ݒ�t�@�C����ǂݍ���

	ifstream file(filename);
	if (file) {
		file >> idClient ; //�N���C�A���gID
		if (idClient==0) {
			bClient = false;
		} else {
			bClient = true;
		}
		std::cout << "icClient = " << idClient << std::endl;
		file >> nameServer; //�T�[�o�}�V������IP
		std::cout <<"nameServer = " << nameServer << std::endl;
		file >> temp;
		if (strcmp(temp,"UseSound")==0) {
			bUseSound = true; //�T�E���h���g�p����
			cout <<"�T�E���h���g�p" << endl;
		}
		file >> temp;
		if (strcmp(temp,"UseGraphics")==0) {
			bUseGraphics = true; //�O���t�B�b�N�X���g�p����
			cout <<"�O���t�B�b�N�X���g�p" << endl;
		}
		file >> temp;
		if (strcmp(temp,"MOUSE")==0) {
			iSpidar_type = Env_NO_SPIDAR;
		} else if (strcmp(temp,"ISA_BIG_SPIDAR")==0) {
			iSpidar_type = Env_ISA_BIG_SPIDAR;
		} else if (strcmp(temp,"USB_SPIDAR_G")==0) {
			iSpidar_type = Env_USB_SPIDAR_G;
		} else {
			cout << temp << "�����m��SPIDAR�^�C�v" <<endl;
			iSpidar_type = 0;
		}
		switch (iSpidar_type)	{
			case Env_NO_SPIDAR:
				cout <<"SPIDAR�^�C�v = MOUSE" << endl;
				break;
			case Env_ISA_BIG_SPIDAR:
				cout <<"SPIDAR�^�C�v = ISA_BIG_SPIDAR" << endl;
				break;
			case Env_USB_SPIDAR_G:
				cout <<"SPIDAR�^�C�v = USB_SPIDAR_G" << endl;
				break;
		}

		file >> px; //SPIDAR�����t�����Ă���X�ӂ̒���/2
		file >> py; //SPIDAR�����t�����Ă���Y�ӂ̒���/2
		file >> pz; //SPIDAR�����t�����Ă���Z�ӂ̒���/2
		cout << "(SPIDAR�̃T�C�Y)/2 = ( " << px << ", " << py << ", " << pz << ") " << endl;

		file >> idExperiment; //����ID
		cout << "����ID = " << idExperiment << endl;

		file.close();
		return true;
	} else {
		cout << "���ݒ�t�@�C�� " << filename << " ��������܂���B" <<endl;
		return false;
	}
}

