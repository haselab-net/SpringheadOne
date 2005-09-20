// test2.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "../../KtDShow.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	CoInitialize(NULL);
	KtDShowCapture *dshow = new KtDShowCapture();
	dshow->SetOutputWindow( NULL );
	dshow->SetAddToRot( true );

	// カメラを得る
	dshow->FindCaptureDevices();
	if( dshow->GetNumOfCaptureDevices() ){
		printf("%s\n",dshow->GetCaptureDevice(0)->GetName());
	}else{
		printf("NoCaptureDevice\n");
		return 0;
	}

	// カメラの画像を表示
	dshow->Render();
	dshow->Run();

	// コンソールでキー入力されるまで待つ
	printf("press any key to Capture\n");
	int key = getchar();
	
	// 映像を取得して各ピクセルのRGB値を得る。
	int bufferSize = dshow->GetCurrentBufferSize();
	printf("CaptureSize:%d\n",bufferSize);
	BYTE *buffer = dshow->GetCurrentBuffer();

	BYTE r,g,b;
	BYTE *pBuffer = buffer;
	for(int y=dshow->GetVideoHeight()-1;y>=0;y--){
		for(int x=0; x<dshow->GetVideoWidth();x++){
			b = *pBuffer;
			g = *(pBuffer+1);
			r = *(pBuffer+2);
			/* なんか処理をする */

			pBuffer+=3;
		}
	}

	delete dshow;
	CoUninitialize();

	return 0;
}
