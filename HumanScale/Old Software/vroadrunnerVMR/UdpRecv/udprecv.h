#ifndef __UDPRECV_H
#define __UDPRECV_H

#include <WinBasis/WBSocket.h>

struct ReceiveInfo{
	int port;
	DWORD sleepTime;
	int bufSize;
	char *buf;
	bool stop;
	int x;
	int y;
	bool captured;
};

extern "C" void ReceiveFunc(ReceiveInfo* info);

class CUdpRecv
{
	ReceiveInfo m_recvinfo;
	char m_buf[1024];
	DWORD m_threadID;
	HANDLE m_threadHandle;
public:
	CUdpRecv(int port,int sleepTime); // ポートとデータを受信する間隔(ms)を指定
	~CUdpRecv();
	bool Start(); // 受信開始
	void Stop(); // 受信停止
	int GetX(); // 受信した最新のX座標を得る
	int GetY(); // 受信した最新のY座標を得る
	bool IsCaptured(); // レーザーポインタをキャプチャしたかどうか
	POINT GetPoint(); // 受信した最新の座標をPOINT構造体で得る(必要かは不明)
};

#endif // __UDPRECV_H
