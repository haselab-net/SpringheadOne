// UdpRecvTest.cpp : コンソール アプリケーション用のエントリ ポイントの定義
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
//		SetCursorPos( udp.GetX(),udp.GetY() ); //おまけ。マウスカーソルの場所を移動。
		Sleep(100);
	}

	udp.Stop();
	return 0;
}
