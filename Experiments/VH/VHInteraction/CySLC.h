/*######################################################*/
//CySLC.h
//	ステレオラベリングカメラ用API集
//作成者
//	CyVerse Corp.
/*######################################################*/

#ifndef __CYSLC_H__
#define __CYSLC_H__

#define SLCOBJECT_MARKERMAX	64 

#define SLCERRORCODE_NON			0	//エラーなし
#define SLCERRORCODE_PARAM			1	//関数パラメータエラー
#define SLCERRORCODE_ALLOCHNDL		2	//ハンドルの確保に失敗（Open時）
#define SLCERRORCODE_GETHNDL		3	//ハンドルの取得に失敗
#define SLCERRORCODE_CAMCONFIG		4	//カメラ設定の取得エラー
#define SLCERRORCODE_CREATETHREAD	5	//スレッド作成エラー
#define SLCERRORCODE_USBCONNECT		6	//ＵＳＢ接続エラー

typedef struct
{
	double	dX;			//物体のX座標（cm）
	double	dY;			//物体のY座標（cm）
	double	dZ;			//物体のZ座標（cm）
	int		nLabel;		//0以上
	int		nStatus;	//1:有効（最新）　2:有効（最新でない：取得済み）
						//0:無効（マーカ消失等）
}SLCOBJECT;


extern "C" __declspec(dllexport) HANDLE __cdecl OpenSLC(const char* lpszCameraID,HWND hWnd,UINT nMsg);
extern "C" __declspec(dllexport) void __cdecl CloseSLC(HANDLE hDevice);
extern "C" __declspec(dllexport) BOOL __cdecl GetSLCData(HANDLE hDevice,SLCOBJECT *lpsSLCOBJ,int* lpnSize);
extern "C" __declspec(dllexport) BOOL __cdecl SetSLCDebugItem(HANDLE hDevice,HWND hWnd,UINT nMsg);
extern "C" __declspec(dllexport) int __cdecl GetSLCErrorCode(void);

extern "C" __declspec(dllexport) BOOL __cdecl SetSLCThresValue(HANDLE hDevice,UINT nLRcamNum,UINT nValue);
extern "C" __declspec(dllexport) int __cdecl GetSLCThresValue(HANDLE hDevice,UINT nLRcamNum);
extern "C" __declspec(dllexport) BOOL __cdecl SetSLCEnRectValue(HANDLE hDevice,UINT nLRcamNum,UINT nEnRectNum,RECT* lpEnableRect);
extern "C" __declspec(dllexport) BOOL __cdecl GetSLCEnRectValue(HANDLE hDevice,UINT nLRcamNum,UINT nEnRectNum,RECT* lpEnableRect);
extern "C" __declspec(dllexport) BOOL __cdecl SetSLCVideoOut(HANDLE hDevice,UINT nSetParam,UINT nValue);
extern "C" __declspec(dllexport) int  __cdecl GetSLCVideoOut(HANDLE hDevice,UINT nSetParam);

extern "C" __declspec(dllexport) BOOL __cdecl SetSLCCamSubCfg(HANDLE hDevice,void* lpCfgObj);
extern "C" __declspec(dllexport) BOOL __cdecl GetSLCCamSubCfg(HANDLE hDevice,void* lpCfgObj);
extern "C" __declspec(dllexport) BOOL __cdecl SaveSLCCamSubCfg(HANDLE hDevice,LPCTSTR lpFPath);
extern "C" __declspec(dllexport) BOOL __cdecl LoadSLCCamSubCfg(HANDLE hDevice,LPCTSTR lpFPath);

extern "C" __declspec(dllexport) BOOL __cdecl DlgSLCCamSubCfgVideoOut(HANDLE hDevice);
extern "C" __declspec(dllexport) BOOL __cdecl DlgSLCCamSubCfgLabeling(HANDLE hDevice);
extern "C" __declspec(dllexport) BOOL __cdecl DlgSLC2DMonitorOpen(HANDLE hDevice,HWND hWnd);
extern "C" __declspec(dllexport) BOOL __cdecl DlgSLC2DMonitorClose(HANDLE hDevice);

extern "C" __declspec(dllexport) HANDLE __cdecl OpenSLCforDebug(const char* lpszCameraID,HWND hWnd,UINT nMsg);


#endif