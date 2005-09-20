#include <conio.h>
#include "Method.h"
#include "LabelingCamera.h"

//#include <iostream.h>
//#include <iomanip.h>

void CALLBACK GetDataTimerProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD);
void CALLBACK ShowDataTimerProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD);

int main(int argc, char *argv[])
{

	int* MMainEyeAngle = ReturnAngle();

	SLCamera slcamera;  //ステレオラベリングカメラ
	slcamera.InitSLC(MMainEyeAngle);

	UINT GetDataTimerId;
	UINT ShowDataTimerId;
	timeBeginPeriod(1);
	GetDataTimerId = timeSetEvent(10, 1, GetDataTimerProc, (DWORD)&slcamera, TIME_PERIODIC);
	ShowDataTimerId = timeSetEvent(500, 1, ShowDataTimerProc, (DWORD)&slcamera, TIME_PERIODIC);


	////// Open GL start//////
	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_RGBA);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
//	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
//	glutIdleFunc(idle);

	init();
	glutMainLoop();
	// Open GL end//

	timeKillEvent(GetDataTimerId);
	timeKillEvent(ShowDataTimerId);
	timeEndPeriod(1);

	slcamera.CloseSLCamera();
	return 0;
}



void CALLBACK GetDataTimerProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD)
{
	((SLCamera*)dwUser)->Step();
	if(kbhit() && getch() == ' '){
		exit(0);
	}

}

void CALLBACK ShowDataTimerProc(UINT uID, UINT, DWORD dwUser, DWORD, DWORD){
	((SLCamera*)dwUser)->ShowData();
}

