#ifndef __KTDSHOW_H
#define __KTDSHOW_H

#include <dshow.h>
#include <qedit.h>
#include <vector>

#include <stdio.h>

#ifndef KT_RELEASE
#define KT_RELEASE(x) if((x)){ (x)->Release(); (x)=NULL; }
#endif //KT_RELEASE

using namespace std;

class KtDShowBasic;
class KtDShowCapture;
class KtCaptureDevice;

//----------------------------------------------------------
// KtDShowBasic
// DirectShowのファイル再生を行うクラス
class KtDShowBasic
{
private:
	IGraphBuilder *m_pGraph;
	IMediaControl *m_pMediaControl;
	IVideoWindow *m_pVidWin;
	IBasicVideo *m_pBasicVideo;
	DWORD m_dwRegister; // AddToROT,RemoveFromROTで使用
	WCHAR m_inputFilename[MAX_PATH];
	HWND m_hOutputWindow;
	char *m_errorMsg;
	bool m_bAddToRot;

	bool AddToRot();
	void RemoveFromRot();

protected:
	virtual bool Open();
	virtual void Close();
	IGraphBuilder *GetGraphBuilder(){ return m_pGraph; }
	IPin* GetPin( IBaseFilter *pFilter,PIN_DIRECTION PinDir );
    IPin* GetPinByName( IBaseFilter *pFilter,WCHAR *pinname );
	void SetErrorMsg( char* msg ){ m_errorMsg = msg; }

public:
	KtDShowBasic();
	~KtDShowBasic();
	void SetInputFile( char *filename );
	WCHAR* GetInputFile(){ return m_inputFilename; }
	void SetOutputWindow( HWND hwnd );
	void SetDrawPosition();
	void SetDrawPosition( int left,int top,int right,int bottom );
	virtual bool Render();
	bool HaveVideoData();
	long GetVideoWidth();
	long GetVideoHeight();
	void SetAddToRot(bool b){ m_bAddToRot = b; }
	virtual bool Run();
	virtual bool Stop();
//	void ResultMBox( hr,char *text,char *caption );
	char *ErrorMsg(){ return m_errorMsg; }
	char *GetResultErrorString( HRESULT hr );
};

//----------------------------------------------------------
// KtDShowCapture
// DirectShowでカメラからキャプチャを行うクラス
class KtDShowCapture : public KtDShowBasic
{
private:
	ICaptureGraphBuilder2 *m_pCapture;
	ISampleGrabber *m_pGrabber;
	char m_outputFilename[MAX_PATH];
	int m_captureDeviceIndex;
	bool m_bGetSample;
	vector<KtCaptureDevice*> m_captureDevices;
	BYTE *m_sampleBuffer;
	long m_sampleBufferSize;
	HBITMAP m_hSampleBitmap;

protected:
	virtual bool Open();
	virtual void Close();

public:
	KtDShowCapture();
	~KtDShowCapture();
	virtual void FindCaptureDevices();
	void SetOutputFile( char *filename );
	void SetGetSample( bool b ){ m_bGetSample = b; }
	int GetNumOfCaptureDevices();
	bool SetCaptureDeviceIndex(int index);
	KtCaptureDevice* GetCaptureDevice(int index);
	BYTE* GetCurrentBuffer();
	int GetCurrentBufferSize(){ return m_sampleBufferSize; }
	HBITMAP GetCurrentBitmap();
	virtual bool Render();
	virtual bool Run();
	virtual bool Stop();
	bool ShowCaptureDeviceProperty();
	bool ShowCaptureOutputProperty();
};

//----------------------------------------------------------
// KtCaptureDevice
// DirectShowのキャプチャデバイスを表すクラス
class KtCaptureDevice
{
private:
	IBaseFilter *m_pBaseFilter;
	char m_name[256];
public:
	KtCaptureDevice(IMoniker *pMoniker);
	~KtCaptureDevice();
	char* GetName(){ return m_name; }
	IBaseFilter* GetIBaseFilter(){ return m_pBaseFilter; };
};

#endif //__KTDSHOW_H
