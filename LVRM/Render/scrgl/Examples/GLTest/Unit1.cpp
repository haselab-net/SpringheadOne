//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <Base/Affinef.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scrgl"
#pragma resource "*.dfm"
TForm1 *Form1;
Affinef afCam1;
Affinef afCam2;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SglPanel1GLInit(TObject *Sender)
{
	SglPanel1->SetCurrent();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);  // Superimpose smaller Z values over larger ones
	glShadeModel(GL_SMOOTH); // Smooth shading
	glFrontFace(GL_CCW);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GEQUAL, 0.07f);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); // Z buffering is go...
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//	�J�����̏����ʒu
	afCam1.Pos(Vec3f(-250,100,-1500));
	afCam1 = afCam1.LookAtGL(Vec3f(0,0,0));
	//	�����ϊ��̐ݒ� �X�N���[���ʒu, �T�C�Y, frontClip, rearClip
	Affinef afProj(Vec3f(0,0,1500), Vec2f(300,300), 10, 4000);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(afProj);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SglPanel2GLInit(TObject *Sender)
{
	SglPanel2->SetCurrent();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);  // Superimpose smaller Z values over larger ones
	glShadeModel(GL_SMOOTH); // Smooth shading
	glFrontFace(GL_CCW);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GEQUAL, 0.07f);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); // Z buffering is go...
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//	�J�����̏����ʒu
	afCam2.Pos(Vec3f(250,100,-1500));
	afCam2 = afCam2.LookAtGL(Vec3f(0,0,0));
	//	�����ϊ��̐ݒ� �X�N���[���ʒu, �T�C�Y, frontClip, rearClip
	Affinef afProj(Vec3f(0,0,1500), Vec2f(300,300), 10, 4000);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(afProj);
}
//---------------------------------------------------------------------------
void Draw(){
	//	�o�b�t�@�̃N���A
	glClearColor(0.1f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	���̕`��
	float vtxs[][3] = {
		100,0,0,
		0,100,0,
		-100,0,0
	};
	glNormal3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
	for(int i=0; i<3; i++) glVertex3fv(vtxs[i]);
	glEnd();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SglPanel1GLPaint(TObject *Sender)
{
	SglPanel1->SetCurrent();
	//	�J�����ʒu�̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(afCam1.Inv());
	//	�`��
	Draw();
	//	�o�b�t�@�̍X�V
	SglPanel1->SwapBuffers();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SglPanel2GLPaint(TObject *Sender)
{
	SglPanel2->SetCurrent();
	//	�J�����ʒu�̐ݒ�
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(afCam2.Inv());
	//	�`��
	Draw();
	//	�o�b�t�@�̍X�V
	SglPanel2->SwapBuffers();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	afCam1 = Affinef(Rad(2), 'Y') * afCam1;
	afCam2 = Affinef(Rad(2), 'Y') * afCam2;
	SglPanel1->Invalidate();
	SglPanel2->Invalidate();
}

//---------------------------------------------------------------------------

