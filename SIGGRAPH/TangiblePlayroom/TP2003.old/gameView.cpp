#include "SceneManager.h"
#include "GameView.h"
#include "Physics.h"
#include <fstream>
#include "environment.h"


extern Vec3f	fakeSpidarPos; // �}�E�X����(�f�o�b�O�p)

Game *theGame;	//�O���[�o���Q�[���|�C���^

//debugYI �����GameView�̃����o�ϐ��ł悢�̂ł�
//static GLfloat	spinY = 0; // �J�����̏����p�x(pan) 


GameView::GameView(){
	scale = 1;
	spinX = 0;
//	mouseX = 0;
//	mouseY = 0;
//	mouseButton = -1;
	game.view = this;
	theGame = &game;
	delta = 1.0f;
//	bFullscreen = false;
}

void GameView::LoadCamera(void) {
	ifstream file("viewpoint.txt");
	file >> game.afCam;
	file >> game.screenPos;
	file >> game.screenSize;
	file >> game.afClipL;
	file >> game.afClipR;
	OnSize(game.screenSize.X(), game.screenSize.Y());
}


void GameView::Init(char* title){

	WindowPosition(224, 0, 800, 600);
	RDGlutView::Init(title);


	//debugYI View�ŃT�E���h�֘A�̏���������ׂ����H

	// �T�E���h�֘A�̏�����
	if ( env.bUseSound ) {
		gssystem = new GameSndSystem;
		gssystem->Init(title);
	}

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);	//��������GLUT_CURSOR_NONE 

	
	float	lmamb[] = {.7, .7, .7, 1},
		l0amb[] = {0, 0, 0, 1},
		l0dif[] = {1, 1, 1, 1},
		l0spc[] = {1, 1, 1, 1},
		l0pos[] = {0, 0, 10, 0};
			
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

#ifdef PH_GRAPHIC_LOW_DETAIL
	glShadeModel(GL_FLAT);
#else
	glShadeModel(GL_SMOOTH);
#endif
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmamb);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0spc);
	glLightfv(GL_LIGHT0, GL_POSITION, l0pos);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128 * .2);
			
	glutCreateMenu(MenuFuncEntry);
	glutAddMenuEntry("Zoom In 	[PgUp]", GLUT_KEY_PAGE_UP);
	glutAddMenuEntry("Zoom Out	[PgDn]", GLUT_KEY_PAGE_DOWN);
	glutAddMenuEntry("Initialize ViewPoint	[Home]", GLUT_KEY_HOME);
	glutAddMenuEntry("------------------", 9999);
	glutAddMenuEntry("Full Screen	[F9]", GLUT_KEY_F9);
	glutAddMenuEntry("Screen Snapshot	[F12]", GLUT_KEY_F12);
	glutAddMenuEntry("------------------", 9999);
	glutAddMenuEntry("Quit  	[Esc]", GLUT_KEY_F4);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

// =====================================================
// �l�b�g���[�N�ڑ��̏�����
//debguYI �l�b�g���[�N�̏�������������Ȃ��āAInit()���Ăяo���Ă���̂Œ���
// =====================================================
bool GameView::InitNetwork(){
	
	//�l�b�g���[�N�ڑ�
	if (env.bClient){
		game.mode = Game::CLIENT;
		game.csock.server = env.nameServer;
		cout << "�N���C�A���g���[�h�� " << env.nameServer << " �ɐڑ���...";
		for(int i=0; i<Game::NCLIENT; ++i){
			game.csock.port = Game::PORTBASE + i;
			game.csock.Connect();
		}
		if (!game.csock.sock.IsConnected()) {
			cout << "���s���܂����I"<< endl <<"�v���O�������I�����܂��B" << endl;
			return false;
		} else {
			cout << "�ڑ�����"<< endl;
			Init("Display Client");  // Init�̈����̓E�B���h�E�^�C�g��
			return true;
		}
	}else{
		cout << "�T�[�o�[���[�h�ŋN����...";
		game.mode = Game::SERVER;
		Init("Game Server"); // Init�̈����̓E�B���h�E�^�C�g��
		return true;
	}
}


void GameView::OnOpen(){
	memories.graphicRead = memories.graphicShouldRead ? 0 : 1;
}

void GameView::OnIdle(){
	if (!this) return;
	//	game.pen1.afi = penguins.penguinGroup[0].penguinRoot->GetPosture();
	//	cout << game.FPS++ << endl;


	game.Step();
	if ( env.bUseSound ) {// �T�E���h�̏����͕K�v�Ȃ�N���C�A���g�ł����s���� 2003/1/31 AS
		gssystem->CheckMakeSound();	
	}
//	cout << timeGetTime() << endl;
}

void GameView::OnDraw(){

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//	game.Draw();
	env.view->sceneManager.Loop(); //�V�[���}�l�[�W���ɂ��X�V�ƕ`��

	Flip();

#ifdef _DEBUG
	Sleep(10);
#endif
}
void GameView::RotateView(){
		glScalef(scale, scale, scale);
		glRotatef(spinX, 1, 0, 0);
		glRotatef(spinY, 0, 1, 0);
	}
void GameView::OnSize(int width, int height){
	game.screenSize.X() = width;
	game.screenSize.Y() = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(180/M_PI * fieldOfView, (GLfloat)width / height, 0.01, 100);
	///	�X�N���[���̈ʒu,	�X�N���[���̃T�C�Y,	�t�����g�N���b�s���O, �o�b�N�N���b�s���O
	Affinef afProj(game.screenPos, game.screenSize, 0.1f, 100.0f);
	glLoadMatrixf(afProj);
	glMatrixMode(GL_MODELVIEW);
}
void GameView::OnMouse(Button button, bool state, int x, int y){
	cout << "bt:" << button << " st:" << state << "x:" << x << "y:" << y << endl;
	int i;
	for (i=1; i<MOUSE_SAMPLE; i++) {
		env.mousez[i] = env.mousez[i-1];
	}
	switch(button){
	case 1://GLUT_LEFT_BUTTON:
		env.mousez[0] -= 1;
		if (env.mousez[0]<-20) env.mousez[0] = -20;
		break;
	case 3://GLUT_RIGHT_BUTTON:
		env.mousez[0] += 1;
		if (env.mousez[0]>+20) env.mousez[0] = 20;
		break;
	}
	cout << "mouseZ ="<<env.mousez[0] << endl;
}

void GameView::OnMouseMove(bool on, int x, int y){
	if (env.iSpidar_type == Env_NO_SPIDAR) {
		int i;
		for (i=1; i<MOUSE_SAMPLE; i++) {
			env.mousex[i] = env.mousex[i-1];
			env.mousey[i] = env.mousey[i-1];
		}
		env.mousex[0] = x;
		env.mousey[0] = y;
	}
//	mouseX = x;	mouseY = y;
}

void GameView::OnKeyboard(int code, int x, int y){

		// �ǂ̃V�[���ł������L�[�o�C���h�̂��̂́��ɏ���
		switch(code){
		case 0x1b:				//	�I��
			CleanupPhysics();
			Exit();
			break;

		case 'f':				//	�t���X�N���[�����[�h�؂�ւ�
			if (env.bFullscreen) {
				glutReshapeWindow(400,300);
				env.bFullscreen = false;
			} else {
				glutFullScreen();
				env.bFullscreen = true;
			}
			break;

		case 'h':				//	�͊o���[�h�؂�ւ�
			if (env.bHaptics) {
				env.bHaptics = false;
			} else {
				env.bHaptics = true;
			}
			break;

		case 't':				//	���ʒ���t�����[�hOFF
				env.bTallNone = false;
			break;
		case 'T':				//	���ʒ���t�����[�hON
				env.bTallNone = true;
			break;

			
		case 'c':
			CalibSpidar();
			break;


		case 'L':
			LoadCamera();
			break;

		case 'l':
			arrowMode = MOVE_CLIPL;		//	�N���b�s���O�I�u�W�F�N�g
			break;
		case 'm':
			arrowMode = MOVE_CAM;		//	�J�����̕��s�ړ�
			break;
		case 'P':				//	���ʃ��[�h
			game.Front();
			OnSize(game.screenSize.X(), game.screenSize.Y());
			break;
		case 'd':				//	�f�o�b�O�p�m�[�h�̃_���v
			game.dnTree->Dump(std::cout);
			break;
		case 'r':
			arrowMode = MOVE_CLIPR;		//	�N���b�s���O�I�u�W�F�N�g
			break;
		case 'S':{
			ofstream file("viewpoint.txt");
			file << game.afCam;
			file << game.screenPos;
			file << game.screenSize;
			file << game.afClipL;
			file << game.afClipR;
				 }
			break;
		case 's':
			arrowMode = MOVE_SCREEN;	//	�X�N���[���̈ړ�
			break;
		case 'z':
			arrowMode = ZOOMROTATE_CAM;	//	�J������������ɉ�]������D
			break;

		case '.':{
			SYSTEMTIME  STime;
			int fileNameTime;
			char dataFileName[255];
			fileNameTime = timeGetTime();
			GetLocalTime(&STime);
			sprintf(dataFileName,"%04d%02d%02d_%010d.txt",STime.wYear,STime.wMonth,STime.wDay,fileNameTime);
			ofstream dataFile(dataFileName);
			dataFile << "Now Setting Data..." << timeGetTime() <<std::endl;
			dataFile.close();

				 }
			break;
		
	    // �������p�p�����[�^�̐ݒ�		
		case '1': delta = 0.001f; break;
		case '2': delta = 0.003f; break;
		case '3': delta = 0.01f;  break;
		case '4': delta = 0.03f;  break;
		case '5': delta = 0.1f;   break;
		case '6': delta = 0.3f;   break;
		case '7': delta = 1.0f;   break;
		case '8': delta = 3.0f;   break;
		case '9': delta = 10.0f;  break;

		default: 
			MenuFunc(-code);
			break;
		}
		
		// SceneManager�ɃL�[���͂�n��
		sceneManager.KeyPressed(code);

}


void GameView::OnSpecialKey(int code, int x, int y){
	
	int	mdf = glutGetModifiers();
	if(mdf & GLUT_ACTIVE_CTRL){
		switch (code){
		case GLUT_KEY_UP:
			MenuFunc(GLUT_KEY_PAGE_UP);
			break;
		case GLUT_KEY_DOWN:
			MenuFunc(GLUT_KEY_PAGE_DOWN);
			break;
		}
	}else{
		switch (code){
		case GLUT_KEY_UP:
			arrowKey.Y() += delta;
			OnArrowKey();
			break;
		case GLUT_KEY_DOWN:
			arrowKey.Y() -= delta;
			OnArrowKey();
			break;
		case GLUT_KEY_LEFT:
			arrowKey.X() -= delta;
			OnArrowKey();
			break;
		case GLUT_KEY_RIGHT:
			arrowKey.X() += delta;
			OnArrowKey();
			break;
		default:
			MenuFunc(code);
			break;
		}
	}
	
	// SceneManager�ɃL�[���͂�n��
	//debugYI
	//sceneManager->KeyPressed(code);

}
void GameView::MenuFuncEntry(int key){
	if (TheRender()) ((GameView*)TheRender())->MenuFunc(key);
}
void GameView::MenuFunc(int key){
	switch (key) {
	case GLUT_KEY_UP:
		spinX -= 5;
		break;
	case GLUT_KEY_DOWN:
		spinX += 5;
		break;
	case GLUT_KEY_LEFT:
		spinY -= 5;
		break;
	case GLUT_KEY_RIGHT:
		spinY += 5;
		break;
	case GLUT_KEY_PAGE_UP:
		scale *= 1.05;
		break;
	case GLUT_KEY_PAGE_DOWN:
		scale /= 1.05;
		break;
	case GLUT_KEY_HOME:
		spinX = spinY = 0;
		scale = 1;
		break;
	case GLUT_KEY_F9:
		glutFullScreen();
		break;
	case GLUT_KEY_F4:
	case 0x1b:
		CleanupPhysics();
		exit(0);
		break;
	default:
		return;
	}
	glutPostRedisplay();
}
void GameView::OnArrowKey(){
	switch(arrowMode){
	case MOVE_CAM:
		game.afCam.Pos().X() += arrowKey.X() / 30.0f;
		game.afCam.Pos().Z() += arrowKey.Y() / 30.0f;
		break;
	case ZOOMROTATE_CAM:
		game.afCam = game.afCam * Affinef(Rad(arrowKey.X()), 'z');
//		game.afCam.Pos().Y() += arrowKey.Y() / 30.0f;
		game.screenPos.Z() += arrowKey.Y() * 10.0f;
		OnSize(game.screenSize.X(), game.screenSize.Y());
		break;
	case MOVE_CLIPL:
		game.afClipL.Pos().X() += arrowKey.X() / 30.0f;
		game.afClipL = game.afClipL * Affinef(Rad(arrowKey.Y()), 'z');
		break;
	case MOVE_CLIPR:
		game.afClipR.Pos().X() += arrowKey.X() / 30.0f;
		game.afClipR = game.afClipR * Affinef(Rad(arrowKey.Y()), 'z');
		break;
	case MOVE_SCREEN:
		game.screenPos.X() += arrowKey.X()*100;
		game.screenPos.Y() += arrowKey.Y()*100;
		OnSize(game.screenSize.X(), game.screenSize.Y());
		break;
	}
	arrowKey = Vec2f();
}
