#include "WinBasis.h"
#pragma hdrstop
namespace Spr{

//----------------------------------------------------------------------------
//	WBWindowMan
//
WBWindowMan::WBWindowMan()
	{
	pWndPlaceOld = NULL;
	pMainWndPlaceOld = NULL;
	}
WBWindowMan::~WBWindowMan()
	{
	delete pWndPlaceOld;
	delete pMainWndPlaceOld;
	}
	
static WNDPROC wprocOld;
static LRESULT CALLBACK WpcMinMaxInfo( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	if (uMsg == WM_GETMINMAXINFO)
		{
		LPMINMAXINFO pmmi = (LPMINMAXINFO) lParam;
		pmmi->ptMaxTrackSize.x = pmmi->ptMaxSize.x * 2;
		pmmi->ptMaxTrackSize.y = pmmi->ptMaxSize.y * 2;
		return 0;
		}
	if (wprocOld) return wprocOld(hwnd, uMsg, wParam, lParam);
	return NULL;
	} 
bool WBWindowMan::FitWindow(POINT ptTo, SIZE szTo)
	{
	if (!hWnd)
		{
		DSTR << "This operation needs hWnd." << std::endl;
		return false;
		}
	HWND hWndParent = hWnd;
	HWND hWndMain;
	while(hWndParent)
		{
		hWndMain = hWndParent;
		hWndParent = GetParent(hWndMain);
		}

	//	set minmaxinfo handler
	if (wprocOld) 
		{
	
		return false;
		}
	wprocOld = (WNDPROC)GetWindowLong(hWndMain, GWL_WNDPROC);
	SetWindowLong(hWndMain, GWL_WNDPROC, (LONG)WpcMinMaxInfo);

	bool rtv = true;
	int i;
	for(i=0; i<4; i++)
		{
		RECT rcClient, rcMain;
		GetWindowRect(hWndMain, &rcMain);
		GetClientRect(hWnd, &rcClient);

		RECT rcScCl;
		POINT pt;
		pt.x = rcClient.left;
		pt.y = rcClient.top;
		::ClientToScreen(hWnd, &pt);
		rcScCl.left		= pt.x;
		rcScCl.top		= pt.y;
		pt.x = rcClient.right;
		pt.y = rcClient.bottom;
		::ClientToScreen(hWnd, &pt);
		rcScCl.right	= pt.x;
		rcScCl.bottom	= pt.y;
		TRACEALL("rcMain (%d,%d) (%d,%d)\n", rcMain.left, rcMain.top, rcMain.right, rcMain.bottom);
		TRACEALL("rcScCl (%d,%d) (%d,%d)\n", rcScCl.left, rcScCl.top, rcScCl.right, rcScCl.bottom);
		
		rcMain.left += ptTo.x - rcScCl.left;
		rcMain.top += ptTo.y - rcScCl.top;
		rcMain.right += szTo.cx - (rcScCl.right - rcScCl.left);
		rcMain.bottom += szTo.cy - (rcScCl.bottom - rcScCl.top);
		MoveWindow(hWndMain, rcMain.left, rcMain.top, rcMain.right-rcMain.left, rcMain.bottom-rcMain.top, false);
		if (rcScCl.left == ptTo.x &&
			rcScCl.top == ptTo.y &&
			rcScCl.right == ptTo.x+szTo.cx &&
			rcScCl.bottom == ptTo.y+szTo.cy) break;
		}	
	if (i==4) rtv = false;
	//	restore handler
	SetWindowLong(hWndMain, GWL_WNDPROC, (LONG)wprocOld);
	wprocOld = NULL;

	InvalidateRect(hWndMain, NULL, false);
	return rtv;
	}
bool WBWindowMan::RestoreWindowPlace()
	{
	if (!hWnd || !pWndPlaceOld || !pMainWndPlaceOld) return false;
	HWND hWndParent = hWnd;
	HWND hWndMain;
	while(hWndParent)
		{
		hWndMain = hWndParent;
		hWndParent = GetParent(hWndMain);
		}
	SetWindowPlacement(hWnd, pWndPlaceOld);
	SetWindowPlacement(hWndMain, pMainWndPlaceOld);
	delete pWndPlaceOld;
	pWndPlaceOld = NULL;
	delete pMainWndPlaceOld;
	pMainWndPlaceOld = NULL;
	return true;
	}
void WBWindowMan::SaveWindowPlace()
	{
	if (!hWnd) return;
	HWND hWndParent = hWnd;
	HWND hWndMain;
	while(hWndParent)
		{
		hWndMain = hWndParent;
		hWndParent = GetParent(hWndMain);
		}
	if (!pWndPlaceOld) pWndPlaceOld = new WINDOWPLACEMENT;
	pWndPlaceOld->length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd, pWndPlaceOld);
	if (!pMainWndPlaceOld) pMainWndPlaceOld = new WINDOWPLACEMENT;
	pMainWndPlaceOld->length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWndMain, pMainWndPlaceOld);
	}
void WBWindowMan::ClientToScreen(POINT* pt, SIZE* sz)
	{
	POINT ptClient = *pt;
	::ClientToScreen(hWnd, pt);
	RECT rc;
	::GetClientRect(hWnd, &rc);
	SIZE szMax;
	szMax.cx = rc.right - rc.left - ptClient.x;
	szMax.cy = rc.bottom - rc.top - ptClient.y;
	if (szMax.cx < 0) szMax.cx = 0;
	if (szMax.cy < 0) szMax.cy = 0;
	if (sz->cx < 0 || sz->cx > szMax.cx) sz->cx = szMax.cx;
	if (sz->cy < 0 || sz->cy > szMax.cy) sz->cy = szMax.cy;
	}
SIZE WBWindowMan::Size()
	{
	SIZE sz;
	RECT rc;
	::GetClientRect(hWnd, &rc);
	sz.cx = rc.right - rc.left;
	sz.cy = rc.bottom - rc.top;
	return sz;
	}

}	//	namespace Spr