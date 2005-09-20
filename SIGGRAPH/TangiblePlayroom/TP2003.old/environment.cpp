#include "environment.h"
#include <fstream>
#include <conio.h>
#include <iostream>
#include <string.h>

#include <windows.h>
#include <winbase.h>

/*
 environment.cpp
 実験環境設定クラス
*/

using namespace std;

// コンストラクタ
Environment::Environment(){
	bEmergency = false;
	iGraspFrame = 0;
	iStaticFrame = 0;
//	viewscale = Vec3f(0.5f,0.5f,0.0f); //iniファイルから読み込みたいぞ
	viewscale = Vec3f(1.0f,1.0f,1.0f); //iniファイルから読み込みたいぞ
	iLanguage = 0; //{日本語}
}

//環境ファイルの読み込み
bool Environment::Load() {
/*
	char temp[255],test[]="UseSound";

	//デフォルト値。そのうちファイル読み込み予定。
	bFullscreen = false;  //フルスクリーン描画
	bHaptics = true;     //力覚モード


	//環境設定ファイルを読み込み

	ifstream file(filename);
	if (file) {
		file >> idClient ; //クライアントID
		if (idClient==0) {
			bClient = false;
		} else {
			bClient = true;
		}
		std::cout << "icClient = " << idClient << std::endl;
		file >> nameServer; //サーバマシン名かIP
		std::cout <<"nameServer = " << nameServer << std::endl;
		file >> temp;
		if (strcmp(temp,"UseSound")==0) {
			bUseSound = true; //サウンドを使用する
			cout <<"サウンドを使用" << endl;
		}
		file >> temp;
		if (strcmp(temp,"UseGraphics")==0) {
			bUseGraphics = true; //グラフィックスを使用する
			cout <<"グラフィックスを使用" << endl;
		}
		file >> temp;
		if (strcmp(temp,"MOUSE")==0) {
			iSpidar_type = Env_NO_SPIDAR;
		} else if (strcmp(temp,"ISA_BIG_SPIDAR")==0) {
			iSpidar_type = Env_ISA_BIG_SPIDAR;
		} else if (strcmp(temp,"USB_SPIDAR_G")==0) {
			iSpidar_type = Env_USB_SPIDAR_G;
		} else {
			cout << temp << "→未知のSPIDARタイプ" <<endl;
			iSpidar_type = 0;
		}
		switch (iSpidar_type)	{
			case Env_NO_SPIDAR:
				cout <<"SPIDARタイプ = MOUSE" << endl;
				break;
			case Env_ISA_BIG_SPIDAR:
				cout <<"SPIDARタイプ = ISA_BIG_SPIDAR" << endl;
				break;
			case Env_USB_SPIDAR_G:
				cout <<"SPIDARタイプ = USB_SPIDAR_G" << endl;
				break;
		}

		file >> px; //SPIDARが取り付けられているX辺の長さ/2
		file >> py; //SPIDARが取り付けられているY辺の長さ/2
		file >> pz; //SPIDARが取り付けられているZ辺の長さ/2
		cout << "(SPIDARのサイズ)/2 = ( " << px << ", " << py << ", " << pz << ") " << endl;

		file >> idExperiment; //実験ID
		cout << "実験ID = " << idExperiment << endl;

		file.close();
		return true;
	} else {
		cout << "環境設定ファイル " << filename << " が見つかりません。" <<endl;
		return false;
	}
*/

	
	char temp[255];
	char szPath[255];

	GetCurrentDirectory(255, szPath);
	sprintf(szPath, "%s\\%s", szPath, filename);
	
	cout << "FileName : " << szPath << endl;
	
	// ファイルが存在するか否か
    bool bRet = false;
    WIN32_FIND_DATA	FindData;
    HANDLE hFile = FindFirstFile(szPath, &FindData );
    if(hFile != INVALID_HANDLE_VALUE)
        bRet = true;
    FindClose( hFile );

	// ファイルが存在しない
	if(!bRet){
		cout << "環境設定ファイル " << filename << " が見つかりません。" <<endl;
		return false;
	}

	
	// クライアントID
	idClient = GetPrivateProfileInt("PenguinHockey", "ClientID", 0, szPath);
	if(idClient == 1){
		bClient = true;
	}else{
		bClient = false; // クライアントIDが0のとき、サーバーモード
	}
	cout << "ClientID = " << idClient << endl;


	// サーバーマシン名 or IP
	GetPrivateProfileString("PenguinHockey", "ServerName", "", nameServer, 255, szPath);
	cout << "ServerName = " << nameServer << endl;

	// サウンドを使用するか否か
	if(GetPrivateProfileInt("PenguinHockey", "UseSound", 0, szPath) == 1){
		bUseSound = true;
		cout << "UseSound = true" << endl;
	}else{
		bUseSound = false;
		cout << "UseSound = false" << endl;
	}
	

	// グラフィックスを使用するか否か
	if(GetPrivateProfileInt("PenguinHockey", "UseGraphics", 0, szPath) == 1){
		bUseGraphics = true;
		cout << "UseGraphics = true" << endl;
	}else{
		bUseGraphics = false;
		cout << "UseGraphics = false" << endl;
	}

	// SPIDARの種類
	GetPrivateProfileString("PenguinHockey", "Device", "", temp, 255, szPath);
	if(strcmp(temp,"MOUSE") == 0){
			iSpidar_type = Env_NO_SPIDAR;
			cout <<"SPIDARタイプ = MOUSE" << endl;
	}else if(strcmp(temp,"ISA_BIG_SPIDAR") == 0){
		iSpidar_type = Env_ISA_BIG_SPIDAR;
		cout <<"SPIDARタイプ = ISA_BIG_SPIDAR" << endl;
	}else if(strcmp(temp,"USB_SPIDAR_G") == 0){
		iSpidar_type = Env_USB_SPIDAR_G;
		cout <<"SPIDARタイプ = USB_SPIDAR_G" << endl;
	}else{
		cout << temp << "→未知のSPIDARタイプ" <<endl;
		iSpidar_type = 0;
	}

	// SPIDARが取り付けられている各辺の長さ/2
	GetPrivateProfileString("PenguinHockey", "Device-X-Length", "", temp, 255, szPath);
	px = strtod(temp, NULL);
	GetPrivateProfileString("PenguinHockey", "Device-Y-Length", "", temp, 255, szPath);
	py = strtod(temp, NULL);
	GetPrivateProfileString("PenguinHockey", "Device-Z-Length", "", temp, 255, szPath);
	pz = strtod(temp, NULL);
	cout << "(SPIDARのサイズ)/2 = ( " << px << ", " << py << ", " << pz << ") " << endl;

	// 実験ID
	idExperiment = GetPrivateProfileInt("PenguinHockey", "ExperimentID", 0, szPath);
	cout << "実験ID = " << idExperiment << endl;

	// フルスクリーンモード or ウィンドウモード
	if(GetPrivateProfileInt("PenguinHockey", "FullScreen", 0, szPath) == 1){
		bFullscreen = true;
		cout << "FullScreen = true" << endl;
	}else{
		bFullscreen = false;
		cout << "FullScreen = false" << endl;
	}

	// 力覚ありか否か
	if(GetPrivateProfileInt("PenguinHockey", "Haptics", 0, szPath) == 1){
		bHaptics = true;
		cout << "Haptics = true" << endl;
	}else{
		bHaptics = false;
		cout << "Haptics = false" << endl;
	}

	env.bTallNone = false; //高さ無視モードOFF
	bRoll90 = true; //プロジェクタ系とSPIDAR系を90度回転

	return true;
}

