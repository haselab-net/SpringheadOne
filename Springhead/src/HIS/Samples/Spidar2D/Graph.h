// Graph.h: Graph クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__3A815D6B_EF20_4753_BA0E_08EA71F22F0A__INCLUDED_)
#define AFX_GRAPH_H__3A815D6B_EF20_4753_BA0E_08EA71F22F0A__INCLUDED_

#include <windows.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

DWORD WINAPI tproc_tread(LPVOID);
LRESULT CALLBACK wproc_thread(HWND hWnd, UINT imsg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK OnPaint(HWND, UINT, WPARAM, LPARAM);
// カラーダイアログの作成 -------------------------------------------
HWND CreateControlWindow(HWND hwndParent, 
						int Left, int Top,int Width, int Height,
						int dwExStyle ,int dwFlag ,
						LPCTSTR Caption, LPCTSTR ClassName,
						HMENU ChildID,HINSTANCE hInstance);
// カラーダイアログボタン押された時の処理
int  ColorDiaog(HWND hwndOwner,DWORD Flags,COLORREF *Colors);
//-------------------------------------------------------------------

static int repetition_n=0;
extern int X_mouse_pos;
extern int Y_mouse_pos;


static HINSTANCE hInstance;

static COLORREF Colors;

#define point_of_spline 5

#define coordinate_X_max 640
#define coordinate_Y_max 480

extern int X_mouse_pos;
extern int Y_mouse_pos;
extern int X_object_point;
extern int Y_object_point;
extern int* x_target_point_buf;
extern int* y_target_point_buf;

extern double spring_modulus;
extern double damper_modulus; 
class Graph
{
public:
	void Window_Create_Plot();

	Graph();
	virtual ~Graph();

};

#endif // !defined(AFX_GRAPH_H__3A815D6B_EF20_4753_BA0E_08EA71F22F0A__INCLUDED_)
