//	VRML scene graph builder
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "GeoNode.h"
#include "GLView.h"

#include <crtdbg.h>
#include <conio.h>
#include <string>
#include <fstream>

//#include "His/HISpidar4Quad.h"
//#include "Device/DRContecIsaDa.h"
//#include "Device/DRContecIsaCounter.h"
//#include <Device/DRUsbH8Simple.h>

using namespace LVRM;
using namespace std;

HISpidar4quad* spidar;

void LoadPhys(TGeoNodes& nodes, const char* fname){
	ifstream file(fname);
	char name[1024];
	while(file.good()){
		file >> name;
		TGeoNode* node = NULL;
		for(TGeoNodes::iterator it = nodes.begin(); it != nodes.end(); ++it){
			if ((*it)->Name() == name){
				node = *it;
				break;
			}
		}
		while(file.good()){
			char ch;
			file >> ch;
			if (ch == '{') break;
			cout << "Unexpected character '" << ch << "' was found." << endl;
		}
		while(file.good()){
			string key;
			file >> key;
			if (key == "mass"){
				char buf[1024];
				file >> buf;
				if (strcmp(buf, "1.#INF")==0){
					node->mass = HUGE_VAL;
				}else{
					node->mass = atof(buf);
				}
			}else if (key == "inertia"){
				file >> node->inertia;
			}else if (key == "center"){
				file >> node->center;
			}else if (key == "momentum"){
				file >> node->momentum;
			}else if (key == "angularMomentum"){
				file >> node->angularMomentum;
			}else if (key == "}"){
				break;
			}else{
				cout << "Unexpected word '" << key << "' was found." << endl;
			}
		}
	}
}

int main(int argc, char *argv[]){
	
	DVDeviceManager devMan;
	devMan.RPool().Register(new DRUsbH8Simple(0));
	devMan.RPool().Register(new DRContecIsaDa(0x300));
	devMan.RPool().Register(new DRContecIsaCounter(0x200));

	devMan.Init();
//	HISpidar4quad spidar;
	std::cout << devMan;

	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	#define PX	0.265f	//	x方向の辺の長さ/2
	#define PY	0.265f	//	y方向の辺の長さ/2
	#define PZ	0.265f	//	z方向の辺の長さ/2
	Vec3f motorPos[][4] = {	//	モータの取り付け位置(中心を原点とする)
		{Vec3f(-PX, PY, PZ),	Vec3f( PX, PY,-PZ), Vec3f(-PX,-PY,-PZ), Vec3f( PX,-PY, PZ)}
	};
	
	HISpidar4quad spidar_;
	spidar_.Init(devMan, motorPos[0], 0.365296803653f, 2.924062107079e-5f, 1.0f, 20.0f);
	spidar = &spidar_;

//	メモリリーク検査用
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
	fprintf(stderr, "vrml2loader 1.3.6+0.1: Copyright (c)2001 Shoichi Hasegawa\n");

	if (argc <= 1) {
		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
		return 1;
	}

	//	VRMLファイルのロード(DOMのような物の作成)
	std::cout << "Loading " << argv[1] << "...";
	UTRef<TSts> root = LoadWrl(argv[1]);
	std::cout << "done." << std::endl;
	std::cout << "Initializing loaded tree ...";
	InitList(root);
	std::cout << "done." << std::endl;

	//	TGeoNodeのツリー(アプリで使うシーングラフ)の作成
	TGLView view;
	view.Init("WrlPhys");	//	GLの初期化を先にする必要あり
	{
		std::cout << "Building TGeoNode ... ";
		TGeoNodeBuilder builder;
		builder.Build(root);
		view.allNodes = builder.allNodes;
		view.clearColor = builder.clearColor;
		std::cout << "done." << std::endl;
	}
	if (argc > 2) view.initialFullscreen = true;
	
	//	シーングラフ中のオブジェクトの物理的な性質(質量・慣性モーメント) の読み出し
	char pfname[1024];
	strcpy(pfname, argv[1]);
	char* p = strrchr(pfname, '.');
	if (p) *p = '\0';
	strcat(pfname, ".phy");
	LoadPhys(view.allNodes, pfname);
	view.InitCollisionDetector();
	view.Run();
	return 0;
}
