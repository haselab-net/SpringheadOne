#include "udprecv.h"
#include <stdio.h>

//#define UDPRECV_DEBUG

using namespace std;
using namespace LVRM;

void ReceiveFunc(ReceiveInfo *info)
{
	WBSocket sock(AF_INET, SOCK_DGRAM, 0);
	WBSockAddr adr;
	adr.AdrIn().sin_family = AF_INET;
	adr.AdrIn().sin_port = htons(info->port);
	adr.AdrIn().sin_addr.s_addr = htonl(INADDR_ANY);		//	アドレス
	if(bind(sock, (LPSOCKADDR)&adr, sizeof(adr))==SOCKET_ERROR){
		closesocket(sock);
		sock = INVALID_SOCKET;
		return;
	}

	bool avail;
	DWORD time;
	int rlen;
	int tmpX,tmpY,tmpCaptured;
	while( ! info->stop ){
		time = timeGetTime();
		avail = sock.CanRecv();
		if( avail ){
			WBSockAddr a;
			rlen = sock.RecvFrom(info->buf,info->bufSize, a);

#ifdef UDPRECV_DEBUG
			printf( "DATA:%s\n",info->buf );
#endif
			// 受信した文字列を解析
			sscanf( info->buf,"%d,%d,%d",&tmpX,&tmpY,&tmpCaptured );
			// ↓info->captured = (bool)tmpCaptured だと警告出るんだよね。動作上は問題ないのだけど。
			info->captured = tmpCaptured ? true : false;
			if( tmpCaptured ){
				info->x = tmpX;
				info->y = tmpY;
			}
		}

		if( timeGetTime() < time + info->sleepTime ){  
			Sleep( time + info->sleepTime - timeGetTime() );
		}
	}
}

CUdpRecv::CUdpRecv(int port,int sleepTime)
{
	m_recvinfo.port = port;
	m_recvinfo.sleepTime = sleepTime;
	m_recvinfo.bufSize = sizeof(m_buf);
	m_recvinfo.buf = m_buf;
	m_recvinfo.stop = false;
	m_recvinfo.x = 0;
	m_recvinfo.y = 0;
	m_recvinfo.captured = false;

	ZeroMemory( m_buf,sizeof(m_buf) );
	m_threadID = 0;
	m_threadHandle = NULL;
}

CUdpRecv::~CUdpRecv()
{
	Stop();
}

bool CUdpRecv::Start()
{
	if( m_threadHandle ){
		Stop();
	}
	m_recvinfo.stop = false;
	m_threadHandle = 
		CreateThread( NULL,0,(LPTHREAD_START_ROUTINE)ReceiveFunc,&m_recvinfo,0,&m_threadID );
	if( ! m_threadHandle ) return false;
	return true;
}

void CUdpRecv::Stop()
{
	if( m_threadHandle == NULL ) return;

	m_recvinfo.stop = true;
	CloseHandle( m_threadHandle );
	m_threadHandle = NULL;
}

int CUdpRecv::GetX()
{
	return m_recvinfo.x;
}

int CUdpRecv::GetY()
{
	return m_recvinfo.y;
}

bool CUdpRecv::IsCaptured()
{
	return m_recvinfo.captured;
}
POINT CUdpRecv::GetPoint()
{
	POINT point;
	point.x = GetX();
	point.y = GetY();
	return point;
}

