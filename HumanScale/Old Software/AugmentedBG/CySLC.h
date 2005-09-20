/*######################################################*/
//CySLC.h
//	�X�e���I���x�����O�J�����pAPI�W
//�쐬��
//	CyVerse Corp.
/*######################################################*/

#ifndef __CYSLC_H__
#define __CYSLC_H__

#define SLCOBJECT_MARKERMAX	64 

#define SLCERRORCODE_NON			0	//�G���[�Ȃ�
#define SLCERRORCODE_PARAM			1	//�֐��p�����[�^�G���[
#define SLCERRORCODE_ALLOCHNDL		2	//�n���h���̊m�ۂɎ��s�iOpen���j
#define SLCERRORCODE_GETHNDL		3	//�n���h���̎擾�Ɏ��s
#define SLCERRORCODE_CAMCONFIG		4	//�J�����ݒ�̎擾�G���[
#define SLCERRORCODE_CREATETHREAD	5	//�X���b�h�쐬�G���[
#define SLCERRORCODE_USBCONNECT		6	//�t�r�a�ڑ��G���[

typedef struct
{
	double	dX;			//���̂�X���W�icm�j
	double	dY;			//���̂�Y���W�icm�j
	double	dZ;			//���̂�Z���W�icm�j
	int		nLabel;		//0�ȏ�
	int		nStatus;	//1:�L���i�ŐV�j�@2:�L���i�ŐV�łȂ��F�擾�ς݁j
						//0:�����i�}�[�J�������j
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