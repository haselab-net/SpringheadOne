#ifndef WINCMP_SOCKSTR_H
#define WINCMP_SOCKSTR_H
#include "WBWin.h"
#include <Base/StrBufBase.h>
#include <winsock2.h>

namespace LVRM {
class WBSockStream;
///	TCP/IPソケットを iostream の streambuffer に結びつけたもの．WBSockStream が使用する．
class WBSockStreambuf:public UTStreambufBase{
	friend WBSockStream;
protected:
	SOCKET sock;				///<	通信に使用するソケット
	SOCKET sockListen;			///<	サーバになった場合，接続待ち(listen) に使用するソケット
	UINT port;					///<	接続・接続待ちに使用するポート番号
	SOCKADDR_IN oppAdr;			///<	接続相手のアドレス
	int lastError;				///<	エラー番号
	const char* lastErrorStr;	///<	エラーメッセージ
	bool bConnected;			///<	接続しているかどうか
	bool bListening;			///<	listen しているかどうか
	bool bServer;				///<	サーバかどうか
	WSAEVENT closeEvent;		///<	相手がソケットを閉じたことを検出するためのイベント
public:
	///	コンストラクタ
	WBSockStreambuf(char* gb, int gl, char* pb, int pl);
	///	デストラクタ
	~WBSockStreambuf();
	///	接続しているかどうか
	bool IsConnected();
	///	listenしているかどうか
	bool IsListening(){return bListening;}
	///	サーバかどうか
	bool IsServer(){return bServer;}
	///	サーバになってListen(接続要求待ち) する
	bool Listen(UINT port);
	///	接続要求を受け入れる
	bool Accept(bool bBlock);
	///	クライアントになって接続要求をする．
	bool Connect(const char* adr, UINT port);
	///	接続を閉じる
	void Close();
protected:
	UINT read(void* buf, UINT bufLen);
	UINT write(void* buf, UINT bufLen);
	virtual int in_avail_stream();
	const char* ErrorMessage();
	const char* ErrorNumToMessage(int errorNum);
	void GetError();
	int AddSockRef();
	int ReleaseSockRef();
	static int sockRefCount;
};

///	TCP/IPソケットを iostream に結びつけたもの
class WBSockStream:public std::iostream{
protected:
	WBSockStreambuf buf;		///<	socket 用 streambuffer
	enum{BUFFERLEN = 1000};		///<	iostreamのバッファの大きさ
	char getBuffer[BUFFERLEN];	///<	iostreamの入力バッファ
	char putBuffer[BUFFERLEN];	///<	iostreamの出力バッファ
public:
	WBSockStream();				///<	コンストラクタ
	~WBSockStream();			///<	デストラクタ
	///	エラーメッセージ
	const char* ErrorMessage(){ return buf.ErrorMessage(); }
	///	クライアントになって接続しに行く．
	bool connect(const char* addr, UINT port){ return Connect(addr, port); }
	bool Connect(const char* addr, UINT port){
		bool rv =  buf.Connect(addr, port);
		if (rv) clear();
		return rv;
	}
	///	サーバーとなって接続を待つ．
	bool listen(UINT port){ return Listen(port); }
	bool Listen(UINT port){ return buf.Listen(port); }
	///	接続を受け付ける．
	bool accept(bool bBlock = false){ return Accept(bBlock); }
	bool Accept(bool bBlock = false){
		bool rv = buf.Accept(bBlock);
		if (rv) clear();
		return rv;
	}
	///	接続しているかどうか
	bool IsConnected(){ return buf.IsConnected(); }
	///	listenしているかどうか
	bool IsListening(){ return buf.IsListening(); }
	///	すぐに入力可能なバイト数
	int avail(){ return Avail(); }
	int Avail(){ return buf.in_avail() + buf.in_avail_stream(); }
	///	 接続を切断する．
	void close(){ Close(); }
	void Close(){ buf.Close(); clear(std::ios::badbit); }
};
}	//	namespace LVRM
#endif