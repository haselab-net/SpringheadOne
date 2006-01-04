
#if !defined(SPLINE_INCLUDED_)
#define SPLINE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Spline
{
public:
	void initialization();
	void run(int,double*,double*);
	double *h, *b, *d, *g, *u, *r, *q, *s, *x, *y;
	int n;

	Spline();
	virtual ~Spline();
	double value(double);
/*
	HWND hWnd;
	UINT uMsg;
	WPARAM wP;
	LPARAM IP;
*/

//	DWORD WINAPI tproc_tread(LPVOID);
//	LRESULT CALLBACK wproc_thread(HWND, UINT, WPARAM, LPARAM);
};

#endif //SPLINE_INCLUDED_
