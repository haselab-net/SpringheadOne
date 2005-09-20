//	VRML scene graph builder
//

//#include "GLView.h"
#include "RenderView.h" //2004.10
//FOR VRML
#include "GeoNode.h"

#include <crtdbg.h>
#include <conio.h>

//MPI
#include "mpi/mpi.h"
using namespace LVRM;
using namespace std;

enum modeSelect{
	receiver,
	sender,
	twoWay
}nowMode;

int main(int argc, char *argv[]){
	//メモリリーク検査用
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);
//	if (argc <= 1) {
//		fprintf(stderr, "usage:	%s VRML2.0_File\n", argv[0]);
//		return 1;
//	}

   //MPI
	int myrank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);  /* プロセッサ番号の取得 */

	cout << "myrank : " << myrank << flush << endl;

	nowMode = receiver;
	//if (argc > 2){ update jeong
	if (argc > 2 && stricmp( argv[1], "1" ) == 0 ){
		nowMode = sender;
		if (*argv[argc - 1] == 'b'){
			nowMode = twoWay;
		}
	}

	TGLView view(nowMode);
	//MPI
	view.myrank = myrank; 
		//MPI031021
	if(argc >= 2){
		view.diffMotionDatFrm = atoi(argv[1]);
	}else{
		view.diffMotionDatFrm = 0;
	}

	cout << "VPC00～VPC03とそれ以外のVPCのフレーム差"<< view.diffMotionDatFrm << endl;
	view.Init("Demostration Virtual Human");

	{
		TGeoNodeBuilder builder[3];

		std::cout << "Loading " << "gbmcb.wrl" << "...";
		UTRef<TSts> root = LoadWrl("gbmcb.wrl");
		InitList(root);
		std::cout << "Loading " << "left_hand.wrl" << "...";
		UTRef<TSts> root1 = LoadWrl("lefthand.wrl");
		InitList(root1);
		std::cout << "Loading " << "right_hand.wrl" << "...";
		UTRef<TSts> root2 = LoadWrl("righthand.wrl");
		InitList(root2);
		std::cout << "done." << std::endl;
		std::cout << "Building TGeoNode ... ";
		if (argc > 3) builder[0].DontDisplay();
		builder[0].Build(root);
        builder[1].Build(root1);
		builder[2].Build(root2);
		for(int i=0; i<3; i++)
		{
			view.allNodes[i] = builder[i].allNodes;
			view.clearColor[i] = builder[i].clearColor;
		}
		
		std::cout << "done." << std::endl;
	
	}

	/*view.InitCollisionDetector();*/
	view.Run();

	//MPI
	MPI_Finalize();

	return 0;
}
