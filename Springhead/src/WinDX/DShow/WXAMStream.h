#ifndef Spr_DSHOW_WXAMSTREAM_H
#define Spr_DSHOW_WXAMSTREAM_H
#include <WinDX/Base/WXComBase.h>
#include "../DDraw/WXDDSurf.h"
#include "WXAMFilterEnum.h"
#include "WXAMGraph.h"
#include <mmstream.h>
#include <amstream.h>
#include <ddstream.h>


namespace Spr {;
class WXAMStream:public WXInterface<IMediaStream, &IID_IMediaStream>{
public:
	friend class WXAMMultiStream;
};
class WXAMDDStream:public WXAMStream{
public:
	WXInterface<IDirectDrawMediaStream, &IID_IDirectDrawMediaStream> ddStream;
	WXInterface<IDirectDrawStreamSample, &IID_IDirectDrawStreamSample> sample;
	WXDDSurfaceDesc desc;
	bool Surf(IDirectDrawSurface* pSurf);
	bool Update(DWORD flags=0);
protected:
	bool CreateDDStream();
	bool GetSurfaceDesc();
	friend class WXAMMultiStream;
};

class WXAMMultiStream:public WXInterface<IAMMultiMediaStream, &IID_IAMMultiMediaStream>{
	WXComInit comInit;
public:
	WXAMGraph graph;
	WXAMMultiStream(DWORD flags = 0);
	bool OpenMoniker(CMoniker m);
	bool OpenFile(const char* fname);
	bool Run();
	bool Stop();
	bool AddVideo();
	WXAMDDStream Video();
};


}	//	namespace Spr;

#endif
