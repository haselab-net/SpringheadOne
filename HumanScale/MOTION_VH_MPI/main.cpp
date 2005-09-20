//	VRML scene graph builder
//

#include "GLView.h"
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
	//cout << "VPC00～VPC03とそれ以外のVPCのフレーム差"<< view.diffMotionDatFrm << endl;

	view.Init("Motion virtual Human VM test");
	{
		TGeoNodeBuilder builder[20];

		std::cout << "Loading " << "gbmcb.wrl" << "...";
		UTRef<TSts> root = LoadWrl("gbmcb.wrl");
		InitList(root);
		std::cout << "Loading " << "Lefthand.wrl" << "...";
		UTRef<TSts> root1 = LoadWrl("lefthand.wrl");
		InitList(root1);
		std::cout << "Loading " << "Righthand.wrl" << "...";
		UTRef<TSts> root2 = LoadWrl("righthand.wrl");
		InitList(root2);

		std::cout << "Loading " << "test.wrl" << "..."<< endl;
		UTRef<TSts> root3 = LoadWrl("test.wrl");
		InitList(root3);

		std::cout << "Loading " << "head.wrl" << "..."<< endl;
		UTRef<TSts> root4 = LoadWrl("head.wrl");
		InitList(root4);
		std::cout << "Loading " << "kubi.wrl" << "..."<< endl;
		UTRef<TSts> root5 = LoadWrl("kubi.wrl");
		InitList(root5);
		std::cout << "Loading " << "kubi2.wrl" << "..."<< endl;
		UTRef<TSts> root6 = LoadWrl("kubi2.wrl");
		InitList(root6);
		std::cout << "Loading " << "rkata.wrl" << "..."<< endl;
		UTRef<TSts> root7 = LoadWrl("rkata.wrl");
		InitList(root7);
		std::cout << "Loading " << "rude.wrl" << "..."<< endl;
		UTRef<TSts> root8 = LoadWrl("rude.wrl");
		InitList(root8);
		std::cout << "Loading " << "rte.wrl" << "..."<< endl;
		UTRef<TSts> root9 = LoadWrl("rte.wrl");
		InitList(root9);
		std::cout << "Loading " << "kosi.wrl" << "..."<< endl;
		UTRef<TSts> root10 = LoadWrl("kosi.wrl");
		InitList(root10);
		std::cout << "Loading " << "rmomo.wrl" << "..."<< endl;
		UTRef<TSts> root11 = LoadWrl("rmomo.wrl");
		InitList(root11);
		std::cout << "Loading " << "rasi.wrl" << "..."<< endl;
		UTRef<TSts> root12 = LoadWrl("rasi.wrl");
		InitList(root12);
		std::cout << "Loading " << "rtoe.wrl" << "..."<< endl;
		UTRef<TSts> root13 = LoadWrl("rtoe.wrl");
		InitList(root13);
		std::cout << "Loading " << "lmomo.wrl" << "..."<< endl;
		UTRef<TSts> root14 = LoadWrl("lmomo.wrl");
		InitList(root14);
		std::cout << "Loading " << "lasi.wrl" << "..."<< endl;
		UTRef<TSts> root15 = LoadWrl("lasi.wrl");
		InitList(root15);
		std::cout << "Loading " << "ltoe.wrl" << "..."<< endl;
		UTRef<TSts> root16 = LoadWrl("ltoe.wrl");
		InitList(root16);
		std::cout << "Loading " << "lkata.wrl" << "..."<< endl;
		UTRef<TSts> root17 = LoadWrl("lkata.wrl");
		InitList(root17);
		std::cout << "Loading " << "lude.wrl" << "..."<< endl;
		UTRef<TSts> root18 = LoadWrl("lude.wrl");
		InitList(root18);
		std::cout << "Loading " << "lte.wrl" << "..."<< endl;
		UTRef<TSts> root19 = LoadWrl("lte.wrl");
		InitList(root19);



		std::cout << "done." << std::endl;
		std::cout << "Building TGeoNode ... ";
		if (argc > 3) builder[0].DontDisplay();
		builder[0].Build(root);
        builder[1].Build(root1);
		builder[2].Build(root2);
		builder[3].Build(root3);
		builder[4].Build(root4);
		builder[5].Build(root5);
		builder[6].Build(root6);
		builder[7].Build(root7);
		builder[8].Build(root8);
		builder[9].Build(root9);
		builder[10].Build(root10);
		builder[11].Build(root11);
		builder[12].Build(root12);
		builder[13].Build(root13);
		builder[14].Build(root14);
		builder[15].Build(root15);
		builder[16].Build(root16);
		builder[17].Build(root17);
		builder[18].Build(root18);
		builder[19].Build(root19);
		for(int i=0; i<20; i++)
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
