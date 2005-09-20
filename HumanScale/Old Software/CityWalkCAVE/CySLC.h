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

extern "C" __declspec(dllexport) HANDLE OpenSLC(const char* lpszCameraID,HWND hWnd,UINT nMsg);
extern "C" __declspec(dllexport) void CloseSLC(HANDLE hDevice);
extern "C" __declspec(dllexport) BOOL GetSLCData(HANDLE hDevice,SLCOBJECT *lpsSLCOBJ,int* lpnSize);
extern "C" __declspec(dllexport) BOOL SetSLCDebugItem(HANDLE hDevice,HWND hWnd,UINT nMsg);
extern "C" __declspec(dllexport) int GetSLCErrorCode(void);

extern "C" __declspec(dllexport) BOOL SetSLCThresValue(HANDLE hDevice,UINT nLRcamNum,UINT nValue);
extern "C" __declspec(dllexport) int GetSLCThresValue(HANDLE hDevice,UINT nLRcamNum);
extern "C" __declspec(dllexport) BOOL SetSLCEnRectValue(HANDLE hDevice,UINT nLRcamNum,UINT nEnRectNum,RECT* lpEnableRect);
extern "C" __declspec(dllexport) BOOL GetSLCEnRectValue(HANDLE hDevice,UINT nLRcamNum,UINT nEnRectNum,RECT* lpEnableRect);
extern "C" __declspec(dllexport) BOOL SetSLCVideoOut(HANDLE hDevice,UINT nSetParam,UINT nValue);
extern "C" __declspec(dllexport) int  GetSLCVideoOut(HANDLE hDevice,UINT nSetParam);

extern "C" __declspec(dllexport) BOOL SetSLCCamSubCfg(HANDLE hDevice,void* lpCfgObj);
extern "C" __declspec(dllexport) BOOL GetSLCCamSubCfg(HANDLE hDevice,void* lpCfgObj);
extern "C" __declspec(dllexport) BOOL SaveSLCCamSubCfg(HANDLE hDevice,LPCTSTR lpFPath);
extern "C" __declspec(dllexport) BOOL LoadSLCCamSubCfg(HANDLE hDevice,LPCTSTR lpFPath);

extern "C" __declspec(dllexport) BOOL DlgSLCCamSubCfgVideoOut(HANDLE hDevice);
extern "C" __declspec(dllexport) BOOL DlgSLCCamSubCfgLabeling(HANDLE hDevice);
extern "C" __declspec(dllexport) BOOL DlgSLC2DMonitorOpen(HANDLE hDevice,HWND hWnd);
extern "C" __declspec(dllexport) BOOL DlgSLC2DMonitorClose(HANDLE hDevice);

extern "C" __declspec(dllexport) HANDLE OpenSLCforDebug(const char* lpszCameraID,HWND hWnd,UINT nMsg);


#endif