#include "WinBasis.h"
#pragma hdrstop
#include "WBSockStr.h"
#include <strstream>
namespace Spr {
using namespace std;

WBSockStreambuf::WBSockStreambuf(char* gb, int gl, char* pb, int pl):
	UTStreambufBase(gb, gl, pb, pl){
	bConnected = false;
	bListening = false;
	bServer = false;
	lastError = 0;
	lastErrorStr = NULL;
	sock = INVALID_SOCKET;
	sockListen = INVALID_SOCKET;
	AddSockRef();
}
BCC_CDECL WBSockStreambuf::~WBSockStreambuf(){
	ReleaseSockRef();
}
int WBSockStreambuf::sockRefCount;
int WBSockStreambuf::AddSockRef(){
	if (sockRefCount == 0){
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD( 2, 2 );
		if (WSAStartup( wVersionRequested, &wsaData )){
			DPF("Cannot load winsock\n");
		}
	}
	sockRefCount ++;
	return sockRefCount;
}
int WBSockStreambuf::ReleaseSockRef(){
	sockRefCount --;
	if (sockRefCount == 0){
		WSACleanup();
	}
	return sockRefCount;
} 
void WBSockStreambuf::GetError(){
		lastError = WSAGetLastError();
		lastErrorStr = ErrorNumToMessage(lastError);
}

bool WBSockStreambuf::Listen(UINT port){
	if (bListening) return true;
	Close();
	sockListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sockListen == INVALID_SOCKET){						//	ソケット作成失敗
		GetError();
		return false;
    }
	SOCKADDR_IN adr;
    ZeroMemory(&adr, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_port = htons(port);
    adr.sin_addr.s_addr = htonl(INADDR_ANY);		//	アドレス

    if(bind(sockListen, (LPSOCKADDR)&adr, sizeof(adr))==SOCKET_ERROR){
		GetError();
        closesocket(sockListen);
        sockListen = INVALID_SOCKET;
        return false;
    }
	if(listen(sockListen, 1) == SOCKET_ERROR){			//	接続待ち失敗
		GetError();
        closesocket(sockListen);
        sockListen = INVALID_SOCKET;
        return false;
    }
	bServer = true;
	bListening = true;
	return true;
}

bool WBSockStreambuf::Accept(bool bBlock){
	if (!bBlock){
		WSAEVENT event;
		event = WSACreateEvent();
		WSAEventSelect(sockListen, event, FD_ACCEPT);
		if (WSAWaitForMultipleEvents(1, &event, false, 0, false) == WSA_WAIT_EVENT_0){
			sock = accept(sockListen, (SOCKADDR*)&oppAdr, NULL);
		}
		WSACloseEvent(event);
	}else{
		sock = accept(sockListen, (SOCKADDR*)&oppAdr, NULL);
	}
	if (sock == INVALID_SOCKET){
		GetError();
		return false;
	}
	closesocket(sockListen);
	sockListen = INVALID_SOCKET;
	bListening = false;
	bConnected = true;
	closeEvent = WSACreateEvent();
	WSAEventSelect(sock, closeEvent, FD_CLOSE);
	return true;
}
bool WBSockStreambuf::Connect(const char* adr, UINT port){
	Close();
	SOCKADDR_IN cl_sin;
    //ソケットを開く
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if(sock==INVALID_SOCKET){
		GetError();
		return false;
	}

    ZeroMemory(&cl_sin, sizeof(cl_sin));
    cl_sin.sin_family = AF_INET;						//	インターネット
    cl_sin.sin_port   = htons(port);					//	ポート番号指定

	HOSTENT* phe=NULL;
    if((phe=gethostbyname(adr))==NULL){					//	アドレス取得
		GetError();
        return false;
    }
    memcpy(&cl_sin.sin_addr, phe->h_addr, phe->h_length);   //アドレス値格納

	//	接続処理
	if(connect(sock, (LPSOCKADDR)&cl_sin, sizeof(cl_sin))==SOCKET_ERROR){
		GetError();
		return false;
	}
	closeEvent = WSACreateEvent();
	WSAEventSelect(sock, closeEvent, FD_CLOSE);
	bConnected = true;
    return true;
}
void WBSockStreambuf::Close(){
	if (bConnected){
		WSACloseEvent(closeEvent);
		closesocket(sock);
		bConnected = false;
	}
	if (bListening){
		closesocket(sockListen);
		bListening = false;
	}
	sock = INVALID_SOCKET;
	sockListen = INVALID_SOCKET;
	bServer = false;
}

#undef MIN
#define MIN(x,y)	((x)<(y) ? (x) : (y))
UINT WBSockStreambuf::read(void* buf, UINT bufLen){
	if (!IsConnected()) return EOF;
	DWORD avail=0;
	ioctlsocket(sock, FIONREAD, &avail);
	return recv(sock, (char*)buf, MIN(bufLen, avail), 0);
}
UINT WBSockStreambuf::write(void* buf, UINT bufLen){
	if (!IsConnected()) return EOF;
	return send(sock, (char*)buf, bufLen, 0);
}
int WBSockStreambuf::in_avail_stream(){
	if (!IsConnected()) return EOF;
	DWORD avail=0;
	ioctlsocket(sock, FIONREAD, &avail);
	return avail;
}
bool WBSockStreambuf::IsConnected(){
	if (!bConnected) return false;
	DWORD r = WSAWaitForMultipleEvents(1, &closeEvent, false, 0, false);
	if (r & WSA_WAIT_EVENT_0) Close();
	return bConnected;
}
const char* WBSockStreambuf::ErrorNumToMessage(int errorNum){
	char* rtv = "";
	switch(errorNum)
		{
		case WSANOTINITIALISED:	rtv = "A successful WSAStartup must occur before using this function.";	break;
		case WSAENETDOWN:		rtv = "The Windows Sockets implementation has detected that the network subsystem has failed.";	break;
		case WSAEADDRINUSE:		rtv = "An attempt has been made to listen on an address in use.";	break;
		case WSAEINPROGRESS:	rtv = "A blocking Windows Sockets operation is in progress.";	break;
		case WSAEINVAL:			rtv = "The socket has not been bound with bind or is already connected.";	break;
		case WSAEISCONN:		rtv = "The socket is already connected.";	break;
		case WSAEMFILE:			rtv = "No more file descriptors are available.";	break;
		case WSAENOBUFS:		rtv = "No buffer space is available.";	break;
		case WSAENOTSOCK:		rtv = "The descriptor is not a socket.";	break;
		case WSAEOPNOTSUPP:		rtv = "The referenced socket is not of a type that supports the listen operation.";	break;
		case WSAEFAULT:			rtv = "The addrlen argument is too small (less than the size of a sockets address structure).";	break;
		case WSAEINTR:			rtv = "The (blocking) call was canceled using WSACancelBlockingCall.";	break;
		case WSAEWOULDBLOCK:	rtv = "The socket is marked as nonblocking and no connections are present to be accepted.";	break;
		case WSAEADDRNOTAVAIL:	rtv = "The specified address is not available from the local computer.";	break;
		case WSAEAFNOSUPPORT:	rtv = "Addresses in the specified family cannot be used with this socket.";	break;
		case WSAECONNREFUSED:	rtv = "The attempt to connect was forcefully rejected.";	break;
		case WSAENETUNREACH:	rtv = "The network can't be reached from this host at this time.";	break;
		case WSAETIMEDOUT:		rtv = "Attempt to connect timed out without establishing a connection.";	break;
		case WSAEHOSTUNREACH:	rtv = "The host can't be reached from this host."; break;
		default:
			{
			static char buf[256];
			ostrstream ostr(buf, 256);
			ostr.setf(ios::dec);
			ostr << "Unknown error #" << errorNum << " occuered." << '\0';
			rtv = buf;
			}break;
		}
	return rtv;
}

WBSockStream::WBSockStream():
	buf(getBuffer, sizeof getBuffer, putBuffer, sizeof putBuffer), iostream(NULL)
{
	init(&buf);
	clear(ios::badbit);
}
BCC_CDECL WBSockStream::~WBSockStream(){
}

}	//	namespace Spr
