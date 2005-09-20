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
using namespace LVRM;
using namespace std;

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
			}else if (key == "}"){
				break;
			}else{
				cout << "Unexpected word '" << key << "' was found." << endl;
			}
		}
	}
}

int main(int argc, char *argv[]){
//	���������[�N�����p
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
	fprintf(stderr, "vrml2loader 1.3.6+0.1: Copyright (c)2001 Shoichi Hasegawa\n");

	if (argc <= 1) {
		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
		return 1;
	}

	//	VRML�t�@�C���̃��[�h(DOM�̂悤�ȕ��̍쐬)
	std::cout << "Loading " << argv[1] << "...";
	UTRef<TSts> root = LoadWrl(argv[1]);
	std::cout << "done." << std::endl;
	std::cout << "Initializing loaded tree ...";
	InitList(root);
	std::cout << "done." << std::endl;

	//	TGeoNode�̃c���[(�A�v���Ŏg���V�[���O���t)�̍쐬
	TGLView view;
	view.Init("WrlPhys");	//	GL�̏��������ɂ���K�v����
	{
		std::cout << "Building TGeoNode ... ";
		TGeoNodeBuilder builder;
		builder.Build(root);
		view.allNodes = builder.allNodes;
		view.clearColor = builder.clearColor;
		std::cout << "done." << std::endl;
	}
	
	//	�V�[���O���t���̃I�u�W�F�N�g�̕����I�Ȑ���(���ʁE�������[�����g) �̓ǂݏo��
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
