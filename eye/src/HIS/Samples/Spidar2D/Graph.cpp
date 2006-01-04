// Graph.cpp: Graph クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "Graph.h"
#include <math.h>
#include <conio.h>
#include "Window.h"
#include "Spline.h"
#include <stdio.h>

// グラフを作るため ----
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream.h>
#include <iostream.h>
//----------------------

int X_object_point;
int Y_object_point;
int X_mouse_pos=320;
int Y_mouse_pos=240;
static int* x_target_point_buf;
static int* y_target_point_buf;
double spring_modulus=0.5;
double damper_modulus=15.0; 


Graph::Graph()
{
	x_target_point_buf  = new int [point_of_spline+1];
	y_target_point_buf  = new int [point_of_spline+1];

	for(int i=0;i<point_of_spline+1;i++)
	{
		x_target_point_buf[i]=-1;
		y_target_point_buf[i]=-1;
	}
	//x_target_point = 100;
	//y_target_point = 100;
}

Graph::~Graph()
{

}

void Graph::Window_Create_Plot()
{

	hInstance = (HINSTANCE)GetWindowLong(HWND_DESKTOP,GWL_HINSTANCE);

				//☆☆☆☆☆☆　スレッド用　☆☆☆☆☆☆	
				WNDCLASSEX wc_thread;
				memset(&wc_thread,0,sizeof(wc_thread));
				wc_thread.cbSize		= sizeof(WNDCLASSEX);
				wc_thread.lpfnWndProc	= wproc_thread;
				wc_thread.hInstance		= hInstance;
				wc_thread.hCursor		= LoadCursor(NULL,IDC_ARROW);
				wc_thread.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
				wc_thread.lpszClassName	= "wc00";

				if(! RegisterClassEx(&wc_thread)) (int)0;//msg.wParam;
				
				DWORD id_thread;
				CreateThread(
							NULL,
							0,
							tproc_tread,
							NULL,
							0,
							&id_thread
							);

			//★★★★★★　スレッド用　★★★★★★
/*				HWND CreateDialog(
					HINSTANCE hInstance,	// インスタンスハンドル
					LPCTSTR lpTemplate,		// ダイアログボックスの名前 
					HWND hWnd,		// 親ウィンドウのハンドル
					DLGPROC &id_thread	// プロシージャ
					);
*/
}


DWORD WINAPI tproc_tread(LPVOID)
{
	MSG msg;
	CreateWindow("wc00",
				 "グラフの概形",
				 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				 CW_USEDEFAULT,CW_USEDEFAULT,
				 coordinate_X_max+10,coordinate_Y_max+100,//(640,480)
				 HWND_DESKTOP,
				 NULL, hInstance,NULL
				);
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK wproc_thread(HWND hWnd, UINT imsg, WPARAM wp, LPARAM lp)
{
	static HWND hWnd_spring_modulus;
	static HWND hWnd_damper_modulus;
	char spring_modulus_char[100];
	char damper_modulus_char[100];
	static bool graph_write = false;

	if(graph_write==true)
	{
		static int count=0;
		ofstream fout;
		fout.open("tmp.dat",ios::app);
		int difference_of_position=X_mouse_pos-X_object_point;
		//fprintf(fp, "%d %d\n", count,difference_of_position);
		fout << count << " " << difference_of_position << endl;
		fout.close();
		count++;
		if(count > 100)	graph_write= false;
	}

	switch(imsg)
	{
	case WM_CREATE:
	   {
		// バネ係数ダンパ係数をセットする
		CreateWindow("BUTTON","係数をセット",
				BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
				350,coordinate_Y_max+10,140,50,
				hWnd,(HMENU)IDB_SetNumber,hInstance,NULL);

		// スプライン描画
/*		CreateWindow("BUTTON","スプラインで描画",
				BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
				350,coordinate_Y_max+10,140,50,
				hWnd,(HMENU)IDB_Spline,hInstance,NULL);
*/
		CreateWindow("static", "バネ係数", WS_CHILD|WS_VISIBLE|SS_CENTER,
            10,coordinate_Y_max+10,150, 18, hWnd, NULL, hInstance, NULL);
			
		_gcvt(spring_modulus,4,spring_modulus_char);
		hWnd_spring_modulus = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", spring_modulus_char,
				WS_CHILD|WS_VISIBLE, 10, coordinate_Y_max+25+10, 150, 25,
				hWnd, (HMENU)IDE_spring_modulus, hInstance, NULL);
		
		CreateWindow("static", "ダンパ係数", WS_CHILD|WS_VISIBLE|SS_CENTER,
            180,coordinate_Y_max+10,150, 18, hWnd, NULL, hInstance, NULL);
			
		_gcvt(damper_modulus,4,damper_modulus_char);
		hWnd_damper_modulus = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", damper_modulus_char,
				WS_CHILD|WS_VISIBLE, 180, coordinate_Y_max+25+10, 150, 25,
				hWnd, (HMENU)IDE_damper_modulus, hInstance, NULL);

		// グラフを表示する
		CreateWindow("BUTTON","グラフ",
				BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
				500,coordinate_Y_max+10+26,100,24,
				hWnd,(HMENU)IDB_GraphLine,hInstance,NULL);

		// 色変更
		CreateControlWindow(hWnd,500,coordinate_Y_max+10,100,24,0,
				BS_PUSHBUTTON,"色変更","BUTTON",(HMENU)IDB_COLOR,
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE));  
        Colors=GetSysColor(COLOR_DESKTOP);    
	
		break;
	   }
	case WM_COMMAND:
		{
			if (LOWORD(wp)==IDB_COLOR)
			{
				//カラー値選択ダイアログを表示  
				if (ColorDiaog(hWnd,CC_RGBINIT,&Colors)) InvalidateRect(hWnd,NULL,FALSE);
			}
			switch(wp)
			{	
				case IDB_Spline:
					{
						Spline mySpline();
					return 0L;
					}
				case IDB_SetNumber:
					{
						GetWindowText(hWnd_spring_modulus, spring_modulus_char, sizeof(spring_modulus_char)-1);
						spring_modulus = atof(spring_modulus_char);

						GetWindowText(hWnd_damper_modulus, damper_modulus_char, sizeof(damper_modulus_char)-1);
						damper_modulus = atof(damper_modulus_char);
					return 0L;
					}
				case IDB_GraphLine:
					{
						system("wgnuplot -persist setting.plt");					
					return 0L;
					}
			}
		}
	case WM_PAINT:
		{
			return OnPaint(hWnd, imsg, wp, lp);		// 描画処理ルーチン
		}
	case WM_TIMER:
		{
			InvalidateRect(hWnd, NULL, TRUE);		// 更新領域追加	
			return 0L;
		}
	case WM_LBUTTONDOWN:
		{
			graph_write=true;
			X_mouse_pos=(int)LOWORD(lp);
			Y_mouse_pos=(int)HIWORD(lp);
			
			InvalidateRect(hWnd, NULL, FALSE);		// 更新領域追加	
			return 0L;
		}
	case WM_RBUTTONDOWN:
		{
			int X_mouse_pos_buf;
			int Y_mouse_pos_buf;

			X_mouse_pos_buf=(int)LOWORD(lp);
			Y_mouse_pos_buf=(int)HIWORD(lp);
			if(repetition_n < point_of_spline)
			{
				x_target_point_buf[repetition_n]=X_mouse_pos_buf;
				y_target_point_buf[repetition_n]=Y_mouse_pos_buf;
			}
			if(repetition_n >= point_of_spline)
			{
				for(int i=0;i<point_of_spline-1;i++)
				{
					x_target_point_buf[i]=x_target_point_buf[i+1];
					y_target_point_buf[i]=y_target_point_buf[i+1];
				}
				x_target_point_buf[point_of_spline-1]=X_mouse_pos_buf;
				y_target_point_buf[point_of_spline-1]=Y_mouse_pos_buf;
			}
			repetition_n++;
			InvalidateRect(hWnd, NULL, FALSE);		// 更新領域追加	
			return 0L;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		break;
		}
	default:
		{
			return DefWindowProc(hWnd,imsg,wp,lp);
		}
	}
	return 0L;
}


LRESULT CALLBACK OnPaint(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
{
	HDC hDC;											// デバイスコンテキスト
    
    PAINTSTRUCT ps;										// PAINTSTRUCT構造体

    hDC=BeginPaint(hWnd, &ps);							// 描画開始
		HPEN hPen_target = CreatePen(PS_SOLID, 1, Colors);// RGB(255, 0, 0));
		HPEN hPen_axis = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));//軸描画
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

		// 座標を描く------------------------------------------
		SelectObject(hDC, hPen_axis);
		for(int i=0;i<=coordinate_X_max/40;i++)
		{
			MoveToEx(hDC,40*i,0,NULL);
			LineTo(hDC,40*i,coordinate_Y_max);
		}
		for(i=0;i<=coordinate_Y_max/40;i++)
		{
			MoveToEx(hDC,0,40*i,NULL);
			LineTo(hDC,coordinate_X_max,40*i);
		}
		DeleteObject(hPen_axis);
		//-----------------------------------------------------
		SelectObject(hDC, hPen_target);
		for(i=0;i<7;i++)
		{
			MoveToEx(hDC,X_mouse_pos-3,Y_mouse_pos-3+i,NULL);
			LineTo(hDC,X_mouse_pos+4,Y_mouse_pos-3+i);
		}
		if(repetition_n < 5)
		{
			for(i=0;i<repetition_n;i++)
			{	
				for(int j=0;j<7;j++)
				{
					MoveToEx(hDC,x_target_point_buf[i]-3,y_target_point_buf[i]-3+j,NULL);
					LineTo(hDC,x_target_point_buf[i]+4,y_target_point_buf[i]-3+j);
				}
				if(i>0)
				{
					MoveToEx(hDC,x_target_point_buf[i-1],y_target_point_buf[i-1],NULL);
					LineTo(hDC,x_target_point_buf[i],y_target_point_buf[i]);
				}
			}
		}
		if(repetition_n >= 5)
		{
			for(i=0;i<point_of_spline;i++)
			{	
				for(int j=0;j<7;j++)
				{
					MoveToEx(hDC,x_target_point_buf[i]-3,y_target_point_buf[i]-3+j,NULL);
					LineTo(hDC,x_target_point_buf[i]+4,y_target_point_buf[i]-3+j);
				}
				if(i>0)
				{
					MoveToEx(hDC,x_target_point_buf[i-1],y_target_point_buf[i-1],NULL);
					LineTo(hDC,x_target_point_buf[i],y_target_point_buf[i]);
				}
			}
		}
		DeleteObject(hPen_target);

		SelectObject(hDC, hPen);
		for(i=0;i<7;i++)
		{
			MoveToEx(hDC,X_object_point-3,Y_object_point-3+i,NULL);
			LineTo(hDC,X_object_point+4,Y_object_point-3+i);
		}
		
		DeleteObject(hPen);
		

    EndPaint(hWnd, &ps);								// 描画終了
	SetTimer(hWnd, 1, 100, NULL);						// タイマセット(200m秒間隔)
	//RECT d={0,0,200,500};
    //InvalidateRect(hWnd, NULL/* &d */, FALSE);

	return 0L;
}


//カラーダイアログ作成
HWND CreateControlWindow(HWND hwndParent, int Left, int Top,int Width, int Height,int dwExStyle ,int dwFlag ,LPCTSTR Caption, LPCTSTR ClassName,HMENU ChildID,HINSTANCE hInstance)
{ 
	return CreateWindowEx(dwExStyle,ClassName,Caption,WS_CHILD | WS_VISIBLE | dwFlag    ,
                         Left, Top, Width, Height, hwndParent, ChildID, hInstance, NULL);
}
//カラーダイアログの中身
int ColorDiaog(HWND hwndOwner,DWORD Flags,COLORREF *Colors)
{
 CHOOSECOLOR CHC;
 DWORD buf[16]; //作成した色を保存する場合はこの変数をグローバル変数にして下さい。

 //初期化
 memset(buf,255,sizeof(DWORD)*16);
 ZeroMemory(&CHC,sizeof(CHOOSECOLOR));

 CHC.lStructSize=sizeof(CHOOSECOLOR);
 CHC.hwndOwner    =hwndOwner;
 CHC.Flags=Flags;
 CHC.rgbResult=(DWORD)*Colors;
 CHC.lpCustColors=    buf;

 if (ChooseColor(&CHC))
 {
    *Colors= CHC.rgbResult;
     return(1);
 }
 else
     return(0);
}

