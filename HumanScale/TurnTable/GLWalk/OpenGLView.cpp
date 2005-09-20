// OpenGLView.cpp : COpenGLView クラスの動作の定義を行います。
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
// COpenGLView クラスの構築/消滅

COpenGLView::COpenGLView()
{
	// TODO: この場所に構築用のコードを追加してください。

	angle = 0;
	pos = 0.0f;
	ez = -5.0f;ex=0;
}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView クラスの描画

void COpenGLView::OnDraw(CDC* pDC)
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: この場所にネイティブ データ用の描画コードを追加します。

	GLfloat sphere_dif[] = { 1.0, 1.0, 1.0, 1.0 };			//拡散反射係数
	GLfloat sphere_shine[] = { 100 };						//ハイライト
	GLfloat sphere_spec[] = { 0.5, 0.5, 0.5, 1.0 };			//鏡面反射係数
	GLfloat sphere_amb[] = { 0.5, 0.5, 0.5, 1.0 };			//環境光反射係数


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
// COpenGLView クラスの診断

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenGLView クラスのメッセージ ハンドラ

int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	turn.Init();
	InitializeOpenGL();
	SetTimer(1, 20, NULL);    // タイマーを作成
	return 0;
}

void COpenGLView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	turn.Uninit();
	::wglMakeCurrent(0,0);
	::wglDeleteContext(m_hRC);
	delete m_pDC; 

}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
//	return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
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
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	
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

//OpenGLコマンド
	::glClear(GL_COLOR_BUFFER_BIT);

	::GLfloat light_position[] = { 0.0, 10.0, -10.0, 0.0 }; 
    ::GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };    
    ::GLfloat light_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f }; 
    ::GLfloat lmodel_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f }; 
    /* 光源No.0 */
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
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
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
