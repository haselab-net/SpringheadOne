#include <WinDX/DShow/WXAMGraph.h>
#include <WinDX/DShow/WXAMFileSource.h>
#include <WinDX/DShow/WXAMFilterEnum.h>
#include <WinDX/DShow/WXAMStream.h>
#include <WinDX/DDraw/WXDDraw.h>
#include <conio.h>

using namespace LVRM;

//	Media Streamを通してDirectShowを使い，DirectDrawサーフェイスに
//	ビットマップを取得するサンプル．
void main(){
	WXDDraws ddraws;
	WXAMMultiStream mstr;
	mstr.AddVideo();
	WXAMFilterEnum filters;
	if (filters.Enum(CLSID_VideoInputDeviceCategory)){
		mstr.OpenMoniker(filters[filters.size()-1].moniker);
	}else{
		mstr.OpenFile("MPEG1Test.mpg");
	}
	WXAMDDStream vid = mstr.Video();

	WXDDSurface surf;
//	ATI All in Wonder 128 を使う場合は ATIAllinWonder を定義する．
//	#define ATIAllinWonder
#ifndef  ATIAllinWonder
	if(vid.desc.Size().cx != 0){
		surf.desc = vid.desc;
	}else{
		SIZE sz={320, 240};
		surf.Size(sz);
		surf.Caps(surf.Caps() | DDSCAPS_OFFSCREENPLAIN);
	}
#else
	SIZE sz={640, 480};
	surf.Size(sz);
//	ATI All in Wonder 128 を使う場合は All in Wonder の PixelFormat を
//	GraphEdit で "パックされたYUY2" に設定する．
	surf.Caps(surf.Caps() | DDSCAPS_OFFSCREENPLAIN);
	surf.PixelFormat(WXDDSurface::YUY2);
#endif
	surf.Create();
	if (!surf.pDDS) return;

	vid.Surf(surf);
	mstr.graph.Dump(DSTR);
	ddraws.Primary().surfPrimary.Create();
	mstr.Run();
	for(int i=0; i<10*30; i++){
		vid.Update();
		ddraws.Primary().surfPrimary.Blt(surf);
	}
	mstr.Stop();
}

#if 0
//	DirectShowのVideoRendererの代わりを自分で作り，受け取った
//	メモリサンプルを読み出すサンプル．
//	c:\mssdk\samples\MultiMedia\DirectShow\baseClasses\baseClasses.dsw
//	をビルドして，strmbasd.lib か strmbase.lib をつくり，リンクする必要がある．
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
void main(){
	WXDDraws ddraws;
	WXAMFilterEnum filters;
	WXAMFileSource fileSource;
	WXAMFilterBase& input = fileSource;
	if (filters.Enum(CLSID_VideoInputDeviceCategory)){
		filters[filters.size()-1].Create();
		input = filters[filters.size()-1];
	}else{
		fileSource.Load("MPEG1Test.mpg");
	}
	WXAMCallbackRenderer renderer;
	renderer.MediaType(MEDIASUBTYPE_RGB565);
//	renderer.MediaType(MEDIASUBTYPE_YUY2);
	renderer.Callback(paint);

	WXAMGraph graph;
	graph.Connect(input, renderer);
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
}
#endif
