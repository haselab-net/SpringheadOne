// test1.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "../../KtDShow.h"

int main(int argc, char* argv[])
{
	CoInitialize(NULL);
	KtDShowBasic *dshow = new KtDShowBasic();
//	dshow->SetOutputWindow( Handle ); //�����̃E�B���h�E�ɕ\������ꍇ
	dshow->SetOutputWindow( NULL ); //�V�K�ɃE�B���h�E���쐬����ꍇ
	dshow->SetAddToRot( true );

	dshow->SetInputFile( "..\\..\\sample.avi" );
	dshow->Render();
	dshow->Run();

	int key;
	key = getchar();

	delete dshow;
	CoUninitialize();

	return 0;
}
