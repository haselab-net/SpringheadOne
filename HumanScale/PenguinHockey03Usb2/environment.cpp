#include "environment.h"
#include <fstream>
#include <conio.h>
#include <iostream>
#include <string.h>

/*
 environment.cpp
 実験環境設定クラス
*/

using namespace std;

//環境ファイルの読み込み
bool Environment::Load() {
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
}

