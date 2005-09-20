#include <WinDX/DShow/WXAMGraph.h>
#include <WinDX/DShow/WXAMFileSource.h>
#include <WinDX/DShow/WXAMFilterEnum.h>
#include <WinDX/DShow/WXAMStream.h>
#include <WinDX/DShow/WXAMSockSink.h>
#include <WinDX/DShow/WXAMSockSource.h>
#include <WinDX/DDraw/WXDDraw.h>
#include <conio.h>
#include <WinDX/DShow/WXAMCallbackRenderer.h>
BYTE bmp[640*480*2];
int linePitch;
SIZE bmpSize;
void paint(SIZE sz, BYTE* ptr, int pitch){
	bmpSize = sz;
	linePitch = pitch;
	if (bmpSize.cy > 480) bmpSize.cy = 480;
	if (bmpSize.cx > 640) bmpSize.cx = 640;
	for(int y=0; y<bmpSize.cy; y++){
		memcpy(bmp + y*640*2, ptr, bmpSize.cx * 2);
		ptr += pitch;
	}
}

using namespace LVRM;

#include <WinDX/DShow/WXAMSockSink.h>
void main(int argc, char** argv){
	WXDDraws ddraws;
	if (argc > 1){
		WXAMFileSource fileSource;
		WXAMFilterBase& input = fileSource;
		WXAMFilterEnum filters;
		if (filters.Enum(CLSID_VideoInputDeviceCategory)){
			filters[filters.size()-1].Create();
			input = filters[filters.size()-1];
		}else{
			fileSource.Load("MPEG1Test.mpg");
//			fileSource.Load("sig2k1.avi");
		}
		WXAMSockSink video, audio;
		video.MediaType(MEDIATYPE_Video);
//		video.MediaSubtype(MEDIASUBTYPE_RGB565);
		video.MediaSubtype(MEDIASUBTYPE_MPEG1Payload);
//		audio.MediaType(MEDIATYPE_Audio);
//		audio.MediaSubtype(MEDIASUBTYPE_MPEG1Payload);

		WXAMGraph graph;
//		graph.Render(input);
		graph.Connect(input, video);
//		graph.Join(audio);
		graph.Dump(DSTR);
		graph.Run();
		for(int i=0; i<33*4; ++i){
			Sleep(33);
		}
		while(!GetKeyState(VK_SPACE)){
			Sleep(10);
		}
	}else{
		WXAMSockSource source;
		source.StartReceive();
		while (source.NumStream() < 1) Sleep(30);
#if 1
		WXAMGraph graph;
		graph.Render(source);
		graph.Dump(DSTR);
		graph.Run();
		for(int i=0; i<33*4; ++i){
			Sleep(33);
		}
		while(!GetKeyState(VK_SPACE)){
			Sleep(10);
		}
#else
		WXDDraws ddraws;
		WXAMCallbackRenderer renderer;
		renderer.MediaType(MEDIASUBTYPE_RGB565);
		renderer.Callback(paint);

		WXAMGraph graph;
		graph.Connect(source, renderer);
		graph.Dump(DSTR);
		graph.Run();

		WXDDSurface& surf = ddraws.Primary().surfPrimary;
		surf.Create();
		for(int i=0; i<30*3; i++){
			surf.Lock();
			for(int y=0; y<bmpSize.cy; y++){
				memcpy(surf.Line(y), bmp+640*y*2, bmpSize.cx*2);
			}
			surf.Unlock();
			Sleep(10);
		}
#endif
	}
}
