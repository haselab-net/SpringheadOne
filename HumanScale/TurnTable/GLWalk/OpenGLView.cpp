// OpenGLView.cpp : COpenGLView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "OpenGL.h"

#include "OpenGLDoc.h"
#include "OpenGLView.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	//{{AFX_MSG_MAP(COpenGLView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView �N���X�̍\�z/����

COpenGLView::COpenGLView()
{
	// TODO: ���̏ꏊ�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B

	angle = 0;
	pos = 0.0f;
	ez = -5.0f;ex=0;
}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView �N���X�̕`��

void COpenGLView::OnDraw(CDC* pDC)
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B

	GLfloat sphere_dif[] = { 1.0, 1.0, 1.0, 1.0 };			//�g�U���ˌW��
	GLfloat sphere_shine[] = { 100 };						//�n�C���C�g
	GLfloat sphere_spec[] = { 0.5, 0.5, 0.5, 1.0 };			//���ʔ��ˌW��
	GLfloat sphere_amb[] = { 0.5, 0.5, 0.5, 1.0 };			//�������ˌW��


	::glClearColor(0.9,1.0,1.0,0.0);
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	::glPushMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, sphere_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sphere_spec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, sphere_amb);
	glMaterialfv(GL_FRONT, GL_SHININESS, sphere_shine);

	::glRotatef((GLfloat)angle,0.0,1.0,0.0);
	::glTranslatef(ex,1.0,ez);

	glColor3f( 1.0f, 1.0f, 0.0f );
	::glutSolidCube(1.0);

	glBegin(GL_POLYGON);
	glColor3f( 0.5f, 0.5f, 0.5f );
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(100.0f,-1.0f,100.0f);
	glVertex3f(-100.0f,-1.0f,100.0f);
	glVertex3f(-100.0f,-1.0f,-100.0f);
	glVertex3f(100.0f,-1.0f,-100.0f);
	glEnd();

	for(int i=-100;i<100;i+=10){
		glLineWidth(2.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_LINES);
		glVertex3f(100.0f,-1.0f,-(float)i);
		glVertex3f(-100.0f,-1.0f,-(float)i);
		glEnd();
	}

	::glPopMatrix();
	::glFlush();
	::SwapBuffers(m_pDC -> GetSafeHdc());


}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView �N���X�̐f�f

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView �N���X�̃��b�Z�[�W �n���h��

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	turn.Init();
	InitializeOpenGL();
	SetTimer(1, 20, NULL);    // �^�C�}�[���쐬
	return 0;
}

void COpenGLView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	turn.Uninit();
	::wglMakeCurrent(0,0);
	::wglDeleteContext(m_hRC);
	delete m_pDC; 

}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
//	return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	if (nChar == 82)turn.ResetAngle();
	if (nChar == VK_RIGHT) angle+=2.0;
	if (nChar == VK_LEFT) angle-=2.0;
//	OnDraw(m_pDC);
	if (nChar == VK_UP) 
	{
		pos=1.0;
		radian = (double)angle * PI / 180;
		ex -= pos * sin(radian);
		ez += pos * cos(radian);
	}

	if (nChar == VK_DOWN) 
	{
		pos=-1.0;
		radian = (double)angle * PI / 180;
		ex -= pos * sin(radian);
		ez += pos * cos(radian);
	}
	OnSize(gnType,gcx,gcy);
	OnDraw(m_pDC);



	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	
	gnType=nType;gcx=cx;gcy=cy;

	::glViewport(0, 0, cx, cy);

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	::gluPerspective(60.0f,(GLdouble)cx/(GLdouble)cy, 0.1f, 200.0f);
//	::gluLookAt(4.0,2.0,4.0 , 2.0,2.0,2.0, 0.0,1.0,0.0);
//	::glRotatef((GLfloat)angle/10,0.0,1.0,0.0);
//	::glTranslatef(0.0,1.0,pos);

	::gluLookAt(0.0,2.0,4.0 , 0.0,2.0,-2.0, 0.0,1.0,0.0);
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();

//	::gluLookAt(10.0,pos,10.0 , 2.0,2.0,2.0, 0.0,1.0,0.0);


}


BOOL COpenGLView::InitializeOpenGL()
{
	m_pDC = new CClientDC(this);
	SetupPixelFormat();
	m_hRC =	::wglCreateContext(m_pDC -> GetSafeHdc());
	::wglMakeCurrent(m_pDC -> GetSafeHdc(), m_hRC );

//OpenGL�R�}���h
	::glClear(GL_COLOR_BUFFER_BIT);

	::GLfloat light_position[] = { 0.0, 10.0, -10.0, 0.0 }; 
    ::GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };    
    ::GLfloat light_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f }; 
    ::GLfloat lmodel_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f }; 
    /* ����No.0 */
    ::glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    ::glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    ::glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    ::glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//    ::glEnable(GL_LIGHTING);
    ::glEnable(GL_LIGHT0);
    
    ::glDepthFunc(GL_LEQUAL);
    ::glEnable(GL_DEPTH_TEST);
	return TRUE;
}

BOOL COpenGLView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW |
			 PFD_SUPPORT_OPENGL |
			 PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,
			0,
			0,
			0,
			0,0,0,0,
			16,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};
	int pixelformat;

	pixelformat =::ChoosePixelFormat(m_pDC -> GetSafeHdc() , &pfd);
	::SetPixelFormat(m_pDC -> GetSafeHdc(),	pixelformat , &pfd);

	return TRUE;
}



void COpenGLView::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
		turn.GetAngle();
		angle += (double)turn.dangle * 180 / PI;
		step.WalkInStep();
		pos = step.walkvel * 0.02;
		radian = (double)angle * PI / 180;
		ex -= pos * sin(radian);
		ez += pos * cos(radian);

		OnSize(gnType,gcx,gcy);
		OnDraw(m_pDC);


	CView::OnTimer(nIDEvent);
}
