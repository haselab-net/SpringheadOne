// test2.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
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

	// �J�����𓾂�
	dshow->FindCaptureDevices();
	if( dshow->GetNumOfCaptureDevices() ){
		printf("%s\n",dshow->GetCaptureDevice(0)->GetName());
	}else{
		printf("NoCaptureDevice\n");
		return 0;
	}

	// �J�����̉摜��\��
	dshow->Render();
	dshow->Run();

	// �R���\�[���ŃL�[���͂����܂ő҂�
	printf("press any key to Capture\n");
	int key = getchar();
	
	// �f�����擾���Ċe�s�N�Z����RGB�l�𓾂�B
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
			/* �Ȃ񂩏��������� */

			pBuffer+=3;
		}
	}

	delete dshow;
	CoUninitialize();

	return 0;
}
