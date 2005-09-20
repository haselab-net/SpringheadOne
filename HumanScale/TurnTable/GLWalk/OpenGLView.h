// OpenGLView.h : COpenGLView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glut.h"
#include <mmsystem.h>
#include <sys\timeb.h>
#include "turn.h"
#include "step.h"

#if !defined(AFX_OPENGLVIEW_H__1BF8B86C_8A1C_11D5_BE31_0800460222F0__INCLUDED_)
#define AFX_OPENGLVIEW_H__1BF8B86C_8A1C_11D5_BE31_0800460222F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIDTH 2.0f
#define HEIGHT 1.0f
#define LENGTH 2000.0f
#define PI 3.1415926535897932

class COpenGLView : public CView
{

//add for openGL
public:
virtual BOOL SetupPixelFormat(void);

Turn turn;
Step step;

private:
BOOL InitializeOpenGL();

HGLRC m_hRC;
CDC* m_pDC;

int gcx,gcy;
UINT gnType;

double radian,ex,ez; 
double angle,pos; 
int dwStart;

protected: // シリアライズ機能のみから作成します。
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// アトリビュート
public:
	COpenGLDoc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COpenGLView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(COpenGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // OpenGLView.cpp ファイルがデバッグ環境の時使用されます。
inline COpenGLDoc* COpenGLView::GetDocument()
   { return (COpenGLDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPENGLVIEW_H__1BF8B86C_8A1C_11D5_BE31_0800460222F0__INCLUDED_)
