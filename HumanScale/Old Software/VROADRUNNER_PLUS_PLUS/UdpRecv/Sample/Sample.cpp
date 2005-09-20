// UdpRecvTest.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//
#include "..\udprecv.h"
#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	CUdpRecv udp(10000,100);
	bool res;
	res = udp.Start();

	if( ! res ) printf( "Start fail.\n" );

	while( ! GetAsyncKeyState(VK_ESCAPE) ){
		printf( "X:%d , Y:%d , IsCaptured:%d\n",udp.GetX(),udp.GetY(),udp.IsCaptured() );
//		SetCursorPos( udp.GetX(),udp.GetY() ); //���܂��B�}�E�X�J�[�\���̏ꏊ���ړ��B
		Sleep(100);
	}

	udp.Stop();
	return 0;
}
