#include "GameView.h"
#include "Physics.h"
#include <fstream>

static GLfloat	spinY = 0;
extern Vec3f	fakeSpidarPos;		//�}�E�X����
extern bool		bSpidarExist;	//SPIDAR�����݂��邩
extern bool		bUseSound; //�T�E���h���g�p����
Game *theGame;	//�O���[�o���Q�[���|�C���^

////////////////////////////////////////////////////////////////////////////////
//	Game Management functions

////////////////////////////////////////////////////////////////////////////////
//	GLUT callback functions

GameView::GameView(){
	scale = 1;
	spinX = 0;
	mouseX = 0;
	mouseY = 0;
	mouseButton = -1;
	game.view = this;
	theGame = &game;
	delta = 1.0f;
}
void GameView::Init(char* win){
	WindowPosition(0, 0, 600, 450);
	RDGlutView::Init(win);

	// �T�E���h�֘A�̏�����
//	if (game.mode == Game::SERVER){ //�T�[�o�[�Ȃ�A�ł͂Ȃ�bUseSound�Őݒ�B2003/1/31 AS
	if ( bUseSound ) {
		gssystem = new GameSndSystem;
		gssystem->Init( win );
	}

	glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	
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
	
	glShadeModel(GL_SMOOTH);
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
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void GameView::OnOpen(){
	memories.graphicRead = memories.graphicShouldRead ? 0 : 1;
}
void GameView::OnIdle(){
	if (!this) return;
	//	game.pen1.afi = penguins.penguinGroup[0].penguinRoot->GetPosture();
	//	cout << game.FPS++ << endl;
	game.Step();
//	if (game.mode == Game::SERVER) gssystem->CheckMakeSound();	// �T�E���h�̏���
	if ( bUseSound ) {// �T�E���h�̏����͕K�v�Ȃ�N���C�A���g�ł����s���� 2003/1/31 AS
		gssystem->CheckMakeSound();	
	}
}
void GameView::OnDraw(){
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	game.Draw();

	glDisable(GL_DEPTH_TEST);
	glLoadMatrixf(game.afClipL);
	glMaterialfv(GL_FRONT, GL_EMISSION , Vec4f(0,0,0,1));
	glMaterialfv(GL_FRONT, GL_DIFFUSE , Vec4f(0,0,0,1));
	glMaterialfv(GL_FRONT, GL_AMBIENT , Vec4f(0,0,0,1));
	glBegin(GL_POLYGON);
		glVertex3fv(Vec3f(-10,-10, -1));
		glVertex3fv(Vec3f(0,-10, -1));
		glVertex3fv(Vec3f(0,10, -1));
		glVertex3fv(Vec3f(-10,10, -1));
	glEnd();
	glLoadMatrixf(game.afClipR);
	glBegin(GL_POLYGON);
		glVertex3fv(Vec3f(0,-10, -1));
		glVertex3fv(Vec3f(10,-10, -1));
		glVertex3fv(Vec3f(10,10, -1));
		glVertex3fv(Vec3f(0,10, -1));
	glEnd();
	glEnable(GL_DEPTH_TEST);
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
	mouseButton = button;
	mouseX = x;	mouseY = y;
	if(!state){
		mouseButton = -1;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case GLUT_MIDDLE_BUTTON:
		if(state)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}
}
	void GameView::OnMouseMove(bool on, int x, int y){
		switch(mouseButton){
		case GLUT_LEFT_BUTTON:
			if(bSpidarExist){	// SPIDARExist���Ђ���Ă���Ȃ��̂ŁA�}�E�X�ŉ�]���������ꍇ��true
				if(x == mouseX && y == mouseY)
					return;
				spinY += (x - mouseX);	spinX += (y - mouseY);
				glutPostRedisplay();
			}else{
				fakeSpidarPos.X() = (float)(x-320) / 640.0f;
				fakeSpidarPos.Y() = -0.30f;
				fakeSpidarPos.Z() = (float)(y-480) / 480.0f;
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if(y == mouseY)
				return;
			if(y < mouseY)
				scale *= 1 + sqrt(mouseY - y)/20;
			else
				scale /= 1 + sqrt(y - mouseY)/20;
			glutPostRedisplay();
			break;
		}
		mouseX = x;	mouseY = y;
	}
	void GameView::OnKeyboard(int code, int x, int y){
		switch(code){
		case 0x1b:				//	�I��
			CleanupPhysics();
			delete( gssystem );	// �T�E���h�̌㏈��
			Exit();
			break;
		case 'c':
			CalibSpidar();
			break;
		case 'P':				//	���ʃ��[�h
			game.Front();
			OnSize(game.screenSize.X(), game.screenSize.Y());
			break;
		case 'f':				//	�t���X�N���[�����[�h
			glutFullScreen();
			break;
		case 'w':				//	�E�B���h�E���[�h
			glutReshapeWindow(400,300);
			break;
		case 'p':				//	�f�o�b�O�p�m�[�h�̃_���v
			game.dnTree->Dump(std::cout);
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
		case 'L':{
			ifstream file("viewpoint.txt");
			file >> game.afCam;
			file >> game.screenPos;
			file >> game.screenSize;
			file >> game.afClipL;
			file >> game.afClipR;
			OnSize(game.screenSize.X(), game.screenSize.Y());
				}
			break;
		case 'l':
			arrowMode = MOVE_CLIPL;		//	�N���b�s���O�I�u�W�F�N�g
			break;
		case 'r':
			arrowMode = MOVE_CLIPR;		//	�N���b�s���O�I�u�W�F�N�g
			break;
		case 'm':
			arrowMode = MOVE_CAM;		//	�J�����̕��s�ړ�
			break;
		case 'z':
			arrowMode = ZOOMROTATE_CAM;	//	�J������������ɉ�]������D
			break;
		case 's':
			arrowMode = MOVE_SCREEN;	//	�X�N���[���̈ړ�
			break;
		
		case '1':
			delta = 0.001f;
			break;
		case '2':
			delta = 0.003f;
			break;
		case '3':
			delta = 0.01f;
			break;
		case '4':
			delta = 0.03f;
			break;
		case '5':
			delta = 0.1f;
			break;
		case '6':
			delta = 0.3f;
			break;
		case '7':
			delta = 1.0f;
			break;
		case '8':
			delta = 3.0f;
			break;
		case '9':
			delta = 10.0f;
			break;
		default:
			MenuFunc(-code);
			break;
		}
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
