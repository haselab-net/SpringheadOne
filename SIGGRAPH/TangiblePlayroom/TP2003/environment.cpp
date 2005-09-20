#include "environment.h"
#include <fstream>
#include <conio.h>
#include <iostream>
#include <string.h>

#include <windows.h>
#include <winbase.h>

/*
 environment.cpp
 �������ݒ�N���X
*/

using namespace std;

// �R���X�g���N�^
Environment::Environment(){
	bEmergency = false;
	iGraspFrame = 0;
	iStaticFrame = 0;
//	viewscale = Vec3f(0.5f,0.5f,0.0f); //ini�t�@�C������ǂݍ��݂�����
	viewscale = Vec3f(1.0f,1.0f,1.0f); //ini�t�@�C������ǂݍ��݂�����
	iLanguage = 0; //{���{��}
}

//���t�@�C���̓ǂݍ���
bool Environment::Load() {
/*
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
*/

	
	char temp[255];
	char szPath[255];

	GetCurrentDirectory(255, szPath);
	sprintf(szPath, "%s\\%s", szPath, filename);
	
	cout << "FileName : " << szPath << endl;
	
	// �t�@�C�������݂��邩�ۂ�
    bool bRet = false;
    WIN32_FIND_DATA	FindData;
    HANDLE hFile = FindFirstFile(szPath, &FindData );
    if(hFile != INVALID_HANDLE_VALUE)
        bRet = true;
    FindClose( hFile );

	// �t�@�C�������݂��Ȃ�
	if(!bRet){
		cout << "���ݒ�t�@�C�� " << filename << " ��������܂���B" <<endl;
		return false;
	}

	
	// �N���C�A���gID
	idClient = GetPrivateProfileInt("PenguinHockey", "ClientID", 0, szPath);
	if(idClient == 1){
		bClient = true;
	}else{
		bClient = false; // �N���C�A���gID��0�̂Ƃ��A�T�[�o�[���[�h
	}
	cout << "ClientID = " << idClient << endl;


	// �T�[�o�[�}�V���� or IP
	GetPrivateProfileString("PenguinHockey", "ServerName", "", nameServer, 255, szPath);
	cout << "ServerName = " << nameServer << endl;

	// �T�E���h���g�p���邩�ۂ�
	if(GetPrivateProfileInt("PenguinHockey", "UseSound", 0, szPath) == 1){
		bUseSound = true;
		cout << "UseSound = true" << endl;
	}else{
		bUseSound = false;
		cout << "UseSound = false" << endl;
	}
	

	// �O���t�B�b�N�X���g�p���邩�ۂ�
	if(GetPrivateProfileInt("PenguinHockey", "UseGraphics", 0, szPath) == 1){
		bUseGraphics = true;
		cout << "UseGraphics = true" << endl;
	}else{
		bUseGraphics = false;
		cout << "UseGraphics = false" << endl;
	}

	// SPIDAR�̎��
	GetPrivateProfileString("PenguinHockey", "Device", "", temp, 255, szPath);
	if(strcmp(temp,"MOUSE") == 0){
			iSpidar_type = Env_NO_SPIDAR;
			cout <<"SPIDAR�^�C�v = MOUSE" << endl;
	}else if(strcmp(temp,"ISA_BIG_SPIDAR") == 0){
		iSpidar_type = Env_ISA_BIG_SPIDAR;
		cout <<"SPIDAR�^�C�v = ISA_BIG_SPIDAR" << endl;
	}else if(strcmp(temp,"USB_SPIDAR_G") == 0){
		iSpidar_type = Env_USB_SPIDAR_G;
		cout <<"SPIDAR�^�C�v = USB_SPIDAR_G" << endl;
	}else{
		cout << temp << "�����m��SPIDAR�^�C�v" <<endl;
		iSpidar_type = 0;
	}

	// SPIDAR�����t�����Ă���e�ӂ̒���/2
	GetPrivateProfileString("PenguinHockey", "Device-X-Length", "", temp, 255, szPath);
	px = strtod(temp, NULL);
	GetPrivateProfileString("PenguinHockey", "Device-Y-Length", "", temp, 255, szPath);
	py = strtod(temp, NULL);
	GetPrivateProfileString("PenguinHockey", "Device-Z-Length", "", temp, 255, szPath);
	pz = strtod(temp, NULL);
	cout << "(SPIDAR�̃T�C�Y)/2 = ( " << px << ", " << py << ", " << pz << ") " << endl;

	// ����ID
	idExperiment = GetPrivateProfileInt("PenguinHockey", "ExperimentID", 0, szPath);
	cout << "����ID = " << idExperiment << endl;

	// �t���X�N���[�����[�h or �E�B���h�E���[�h
	if(GetPrivateProfileInt("PenguinHockey", "FullScreen", 0, szPath) == 1){
		bFullscreen = true;
		cout << "FullScreen = true" << endl;
	}else{
		bFullscreen = false;
		cout << "FullScreen = false" << endl;
	}

	// �͊o���肩�ۂ�
	if(GetPrivateProfileInt("PenguinHockey", "Haptics", 0, szPath) == 1){
		bHaptics = true;
		cout << "Haptics = true" << endl;
	}else{
		bHaptics = false;
		cout << "Haptics = false" << endl;
	}

	env.bTallNone = false; //�����������[�hOFF
	bRoll90 = true; //�v���W�F�N�^�n��SPIDAR�n��90�x��]

	return true;
}

