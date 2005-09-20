#include "glut_test.h"
#include <HIS/HISpidar4anglequad.h>
#include <conio.h>

using namespace LVRM;

void Glut_test::OnDraw()
{
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


//	glPushMatrix();
//	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
//	glColor3f(1,0,0);	
	glColor3f(0,1,1);
	

	glBegin(GL_QUADS);
	glVertex3f(sp->GetPos(0).X(),sp->GetPos(0).Y(),sp->GetPos(0).Z());
	glVertex3f(sp->GetPos(3).X(),sp->GetPos(3).Y(),sp->GetPos(3).Z());
	glVertex3f(sp->GetPos(1).X(),sp->GetPos(1).Y(),sp->GetPos(1).Z());
	glVertex3f(sp->GetPos(2).X(),sp->GetPos(2).Y(),sp->GetPos(2).Z());
	glEnd();

	glColor3f(1,0.5,0.2);
	glBegin(GL_QUADS);
	glVertex3f(sp->GetPos(2).X(),sp->GetPos(2).Y(),sp->GetPos(2).Z());
	glVertex3f(sp->GetPos(1).X(),sp->GetPos(1).Y(),sp->GetPos(1).Z());
	glVertex3f(sp->GetPos(3).X(),sp->GetPos(3).Y(),sp->GetPos(3).Z());
	glVertex3f(sp->GetPos(0).X(),sp->GetPos(0).Y(),sp->GetPos(0).Z());
	glEnd();

	//線の描画
	glColor3d(1.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f((sp->GetPos(0).X()+sp->GetPos(3).X())/2.0, (sp->GetPos(0).Y()+sp->GetPos(3).Y())/2.0 ,(sp->GetPos(0).Z()+sp->GetPos(3).Z())/2.0);
	glVertex3f((sp->GetPos(1).X()+sp->GetPos(2).X())/2.0, (sp->GetPos(1).Y()+sp->GetPos(2).Y())/2.0 ,(sp->GetPos(1).Z()+sp->GetPos(2).Z())/2.0);
	glVertex3f((sp->GetPos(0).X()+sp->GetPos(2).X())/2.0, (sp->GetPos(0).Y()+sp->GetPos(2).Y())/2.0 ,(sp->GetPos(0).Z()+sp->GetPos(2).Z())/2.0);
	glVertex3f((sp->GetPos(3).X()+sp->GetPos(1).X())/2.0, (sp->GetPos(3).Y()+sp->GetPos(1).Y())/2.0 ,(sp->GetPos(3).Z()+sp->GetPos(1).Z())/2.0);

	glEnd();

//	glVertex3f(sp->GetPos(0).X(), sp->GetPos(0).Y() ,sp->GetPos(0).Z());
//	glVertex3f(sp->GetPos(1).X(), sp->GetPos(1).Y() ,sp->GetPos(1).Z());
//	glVertex3f(sp->GetPos(3).X(), sp->GetPos(3).Y() ,sp->GetPos(3).Z());
//	glVertex3f(sp->GetPos(2).X(), sp->GetPos(2).Y() ,sp->GetPos(2).Z());
	
	//軸の表示
	glColor3d(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(5.0,0.0,0.0);
	glVertex3f(-5.0,0.0,0.0);
	glVertex3f(0.0,5.0,0.0);
	glVertex3f(0.0,-5.0,0.0);
	glVertex3f(0.0,0.0,5.0);
	glVertex3f(0.0,0.0,-5.0);
	glEnd();
	
	//地面の描画
	glColor3d(0.1,0.5,0.1);
	glBegin(GL_QUADS);
	glVertex3f(-2.0, -2.0, 2.0);
	glVertex3f(2.0, -2.0, 2.0);
	glVertex3f(2.0, -2.0, -2.0);
	glVertex3f(-2.0, -2.0,-2.0);
	glEnd();
//	glPopMatrix();
//	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
		
	Flip();
}

void Glut_test::OnSize(int width, int height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)(width)/(float)(height) , 0.1f , 1000.0f);
	gluLookAt(0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
}

//この関数がループしている
bool Glut_test::OnTimer(){

		if (kbhit()){
			int ch = getch();
			switch(ch){
			//case 0x1b:
			//	goto end;
			//	break;
			case  0x36:
				sp->Calib();
				break;
			}
		}
	//SPIDARの位置を更新している
//		sp->Update();

//		std::cout << sp->GetPos(4).X() << std::endl;

	Invalidate();
	return true;
}

void Glut_test::OnIdle(){
//	sp->Update();

}

void Glut_test::OnOpen(){
	SetTimer(10, &Glut_test::OnTimer);
}
