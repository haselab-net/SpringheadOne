//	VRML scene graph builder
//

#include "GLView.h"
//FOR VRML
#include "GeoNode.h"

#include <crtdbg.h>
#include <conio.h>

using namespace LVRM;
using namespace std;

enum modeSelect{
	receiver,
	sender,
	twoWay
}nowMode;

int main(int argc, char *argv[]){
	//ÉÅÉÇÉäÉäÅ[ÉNåüç∏óp
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
//	if (argc <= 1) {
//		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
//		return 1;
//	}
  
	nowMode = receiver;
	//if (argc > 2){ update jeong
	if (argc > 2 && stricmp( argv[1], "1" ) == 0 ){
		nowMode = sender;
		if (*argv[argc - 1] == 'b'){
			nowMode = twoWay;
		}
	}

	TGLView view(nowMode);

	view.Init("Motion Reactive virtual Human");

	{
		TGeoNodeBuilder builder[5];

		std::cout << "Loading " << "gbmcb.wrl" << "...";
		UTRef<TSts> root = LoadWrl("bob_head.wrl");
		InitList(root);
		std::cout << "Loading " << "Lefthand.wrl" << "...";
		UTRef<TSts> root1 = LoadWrl("Bob_left_hand.wrl");
		InitList(root1);
		std::cout << "Loading " << "Righthand.wrl" << "...";
		UTRef<TSts> root2 = LoadWrl("Bob_right_hand.wrl");
		InitList(root2);
		std::cout << "Loading " << "Leftfoot.wrl" << "...";
		UTRef<TSts> root3 = LoadWrl("Bob_left_foot.wrl");
		InitList(root3);
		std::cout << "Loading " << "Rightfoot.wrl" << "...";
		UTRef<TSts> root4 = LoadWrl("Bob_right_foot.wrl");
		InitList(root4);
		std::cout << "done." << std::endl;
		std::cout << "Building TGeoNode ... ";
		if (argc > 3) builder[0].DontDisplay();
		builder[0].Build(root);
        builder[1].Build(root1);
		builder[2].Build(root2);
		builder[3].Build(root3);
		builder[4].Build(root4);
		for(int i=0; i<5; i++)
		{
			view.allNodes[i] = builder[i].allNodes;
			view.clearColor[i] = builder[i].clearColor;
		}
		
		std::cout << "done." << std::endl;
	
	}

	/*view.InitCollisionDetector();*/
	view.Run();

	return 0;
}
