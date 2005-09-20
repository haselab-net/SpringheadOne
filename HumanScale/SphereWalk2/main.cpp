//	VRML scene graph builder
//
// This program is free software.
// You can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation.

#include "GLView.h"
#include "GeoNode.h"

#include <crtdbg.h>
#include <conio.h>
using namespace LVRM;
using namespace std;

int main(int argc, char *argv[]){
//	���������[�N�����p
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	long ba=-1;
//	_CrtSetBreakAlloc(ba);

	
	int myrank = -1;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);  /* �v���Z�b�T�ԍ��̎擾 */
	cout << "myrank : " << myrank << flush << endl;

	if(myrank == 0) argc++; 
	TGLView view(argc > 1);
	view.Init("SphereWalk");	//	GL�̏��������ɂ���Ђ悤����
	view.myrank = myrank;
	view.Run();

	MPI_Finalize();
	return 0;
}
