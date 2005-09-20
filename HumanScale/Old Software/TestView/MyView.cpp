#include "MyNode.h"
#include "MyView.h"

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <crtdbg.h>
#include <iostream>
#include <strstream>
#include "Communication.h"

#define KEY_ESC -27

void MyView::OnIdle(){
	static int count;
	count ++;
	if (count > 10){
		if (_access("stop", 00) != -1){
			Exit();
		}
		count = 0;
	}
}

void MyView::setAfCam(Affinef af){
	afCam = af;
}

Affinef MyView::refAfCam(){
	return afCam;
}

void MyView::LoadSettings(){
	char* cname = getenv("COMPUTERNAME");
	char* p = cname;
	while(*p && !isdigit(*p)) p++;
	int cpNum = atoi(p);
	std::map<int,int> cpProj;
	cpProj[0]=0;
	cpProj[1]=1;
	cpProj[2]=2;
	cpProj[3]=3;
	cpProj[4]=0;
	cpProj[5]=1;
	cpProj[6]=2;
	cpProj[7]=3;

	cpProj[11]=8;
	cpProj[20]=9;
	cpProj[21]=10;
	cpProj[15]=11;
	cpProj[10]=12;
	cpProj[14]=13;
	cpProj[13]=14;
	cpProj[17]=15;
	
	cpProj[12]=16;
	cpProj[30]=17;
	cpProj[31]=18;
	cpProj[16]=19;
	cpProj[22]=9;
	cpProj[23]=10;
	cpProj[32]=17;
	cpProj[33]=18;

	int projNum = cpProj[cpNum];
	char filename[1024];
	ostrstream ostrfn(filename, sizeof(filename));
	ostrfn << "sf" << projNum << '\0';

	FILE *fp;
	int number;
	int set = 0;
	char ch[128];

    
    if((fp = fopen(filename, "r")) == NULL){
        cout << "Can not read or find file sf1" << endl;
 		return ;
   }
    fgets(ch,sizeof(ch),fp);

	std::istrstream cStrStrm(ch, sizeof(ch));

//	cout << ch <<endl;
	cStrStrm >> number >> setting[0] >> setting[1] >> setting[2] >> setting[3]
		>> setting[4] >> setting[5] >> setting[6];
//	cout << number << endl;
//        	for(int j=0;j<6;j++)
//	cout << setting[j] << endl;
	SetParam();
    fclose(fp);

}

void MyView::SetParam(){
	double angleA,angleB,angleC;
	angleB = asin(setting[1]);
	angleA = asin(setting[0]/cos(angleB));
	angleC = setting[5]/180 * M_PI;
	Affinef Rzx,Rzy,Rxy;
	Rzx.Ex() = Vec3f(cos(angleA),0,-sin(angleA));
	Rzx.Ey() = Vec3f(0,1,0);
	Rzx.Ez() = Vec3f(sin(angleA),0,cos(angleA));
	Rzy.Ex() = Vec3f(1,0,0);
	Rzy.Ey() = Vec3f(0,cos(angleB),-sin(angleB));
	Rzy.Ez() = Vec3f(0,sin(angleB),cos(angleB));
	Rxy.Ex() = Vec3f(cos(angleC),sin(angleC),0);
	Rxy.Ey() = Vec3f(-sin(angleC),cos(angleC),0);
	Rxy.Ez() = Vec3f(0,0,1);
	cout << angleB ;
	R = Rzx*Rzy*Rxy;
	fov_v = setting[3] * 2;
}

void MyView::OnDraw()
{
	glColor3f(1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

//	glRotatef(180/M_PI * vieworient[3], vieworient[0], vieworient[1], vieworient[2]);
//	glTranslatef(-viewpos[0], -viewpos[1], -viewpos[2]);
//	glMultMatrixf(R);
	
	static Affinef R180(Rad(180),'y');
	
	afCam = csSelect.eye;
	afProjector = afCam * R180 * R * R180;
	glLoadMatrixf(afProjector.inv());

	glutWireSphere(100, 72, 72);
	glutSwapBuffers();
}

void MyView::OnSize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov_v , 4 / 3 , viewNear , viewFar);
	glMatrixMode(GL_MODELVIEW);
}



void MyView::OnKeyboard(int key, int x, int y)
{
	static	int		nPenSuffix = 0;

	switch(key){
	case 'b':
		glutPostRedisplay();
		break;
	case 'f':
		glutFullScreen();
		break;
	case 'w':
		glutReshapeWindow(400,300);
		break;
	default:
//		MenuFunc(-key);
	break;
	}
}

void MyView::OnSpecialKey(int key, int x, int y)
{
	glMatrixMode(GL_MODELVIEW);
//	int	mdf = glutGetModifiers();
//	if(mdf & GLUT_ACTIVE_CTRL){
	float delta = 10;
	switch (key){
		case GLUT_KEY_UP:
			csSelect.eye.Pos() -= csSelect.eye.Ez()*delta;
			Invalidate();
			break;
		case GLUT_KEY_DOWN:
			csSelect.eye.Pos() += csSelect.eye.Ez()*delta;
			Invalidate();
			break;
		case GLUT_KEY_HOME:
			csSelect.eye.Pos() -= csSelect.eye.Ey()*delta;
			Invalidate();
			break;
		case GLUT_KEY_END:
			csSelect.eye.Pos() += csSelect.eye.Ey()*delta;
			Invalidate();
			break;
		case GLUT_KEY_LEFT:
			csSelect.eye.Rot() =  csSelect.eye.Rot() * Affinef(Rad(5), 'y').Rot();
			Invalidate();
			break;
		case GLUT_KEY_RIGHT:
			csSelect.eye.Rot() =  csSelect.eye.Rot() * Affinef(Rad(-5), 'y').Rot();
			Invalidate();
			break;
		case GLUT_KEY_PAGE_UP:
			csSelect.eye.Rot() =  csSelect.eye.Rot() * Affinef(Rad(-5), 'x').Rot();
			Invalidate();
			break;
		case GLUT_KEY_PAGE_DOWN:
			csSelect.eye.Rot() = csSelect.eye.Rot() * Affinef(Rad(5), 'x').Rot();
			Invalidate();
			break;
		}
//	}else{
//	}

}

void MyView::OnMouse(Button button, bool on, int x, int y)
{
	mouseButton = button;
	mouseX = x;	mouseY = y;
	if(!on){
		mouseButton = NONE;
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
	switch(button){
	case LEFT:
		if(on)
			glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	case MIDDLE:
		if(on)
			glutSetCursor(GLUT_CURSOR_UP_DOWN);
		break;
	}
}

void MyView::OnMouseMove(bool on, int x, int y)
{
	switch(mouseButton){
	case LEFT:
		if(x == mouseX && y == mouseY)
			return;
		spinY += (x - mouseX);	spinX += (y - mouseY);
		glutPostRedisplay();
		break;
	case MIDDLE:
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
void MyView::OnOpen(void)
{
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
	glutFullScreen();
}

void MyView::RotateView(void)
{
	glScalef(scale, scale, scale);
	glRotatef(spinX, 1, 0, 0);
	glRotatef(spinY, 0, 1, 0);
}
