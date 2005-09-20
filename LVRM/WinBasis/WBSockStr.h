#ifndef WINCMP_SOCKSTR_H
#define WINCMP_SOCKSTR_H
#include "WBWin.h"
#include <Base/StrBufBase.h>
#include <winsock2.h>

namespace LVRM {
class WBSockStream;
///	TCP/IP�\�P�b�g�� iostream �� streambuffer �Ɍ��т������́DWBSockStream ���g�p����D
class WBSockStreambuf:public UTStreambufBase{
	friend WBSockStream;
protected:
	SOCKET sock;				///<	�ʐM�Ɏg�p����\�P�b�g
	SOCKET sockListen;			///<	�T�[�o�ɂȂ����ꍇ�C�ڑ��҂�(listen) �Ɏg�p����\�P�b�g
	UINT port;					///<	�ڑ��E�ڑ��҂��Ɏg�p����|�[�g�ԍ�
	SOCKADDR_IN oppAdr;			///<	�ڑ�����̃A�h���X
	int lastError;				///<	�G���[�ԍ�
	const char* lastErrorStr;	///<	�G���[���b�Z�[�W
	bool bConnected;			///<	�ڑ����Ă��邩�ǂ���
	bool bListening;			///<	listen ���Ă��邩�ǂ���
	bool bServer;				///<	�T�[�o���ǂ���
	WSAEVENT closeEvent;		///<	���肪�\�P�b�g��������Ƃ����o���邽�߂̃C�x���g
public:
	///	�R���X�g���N�^
	WBSockStreambuf(char* gb, int gl, char* pb, int pl);
	///	�f�X�g���N�^
	~WBSockStreambuf();
	///	�ڑ����Ă��邩�ǂ���
	bool IsConnected();
	///	listen���Ă��邩�ǂ���
	bool IsListening(){return bListening;}
	///	�T�[�o���ǂ���
	bool IsServer(){return bServer;}
	///	�T�[�o�ɂȂ���Listen(�ڑ��v���҂�) ����
	bool Listen(UINT port);
	///	�ڑ��v�����󂯓����
	bool Accept(bool bBlock);
	///	�N���C�A���g�ɂȂ��Đڑ��v��������D
	bool Connect(const char* adr, UINT port);
	///	�ڑ������
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

///	TCP/IP�\�P�b�g�� iostream �Ɍ��т�������
class WBSockStream:public std::iostream{
protected:
	WBSockStreambuf buf;		///<	socket �p streambuffer
	enum{BUFFERLEN = 1000};		///<	iostream�̃o�b�t�@�̑傫��
	char getBuffer[BUFFERLEN];	///<	iostream�̓��̓o�b�t�@
	char putBuffer[BUFFERLEN];	///<	iostream�̏o�̓o�b�t�@
public:
	WBSockStream();				///<	�R���X�g���N�^
	~WBSockStream();			///<	�f�X�g���N�^
	///	�G���[���b�Z�[�W
	const char* ErrorMessage(){ return buf.ErrorMessage(); }
	///	�N���C�A���g�ɂȂ��Đڑ����ɍs���D
	bool connect(const char* addr, UINT port){ return Connect(addr, port); }
	bool Connect(const char* addr, UINT port){
		bool rv =  buf.Connect(addr, port);
		if (rv) clear();
		return rv;
	}
	///	�T�[�o�[�ƂȂ��Đڑ���҂D
	bool listen(UINT port){ return Listen(port); }
	bool Listen(UINT port){ return buf.Listen(port); }
	///	�ڑ����󂯕t����D
	bool accept(bool bBlock = false){ return Accept(bBlock); }
	bool Accept(bool bBlock = false){
		bool rv = buf.Accept(bBlock);
		if (rv) clear();
		return rv;
	}
	///	�ڑ����Ă��邩�ǂ���
	bool IsConnected(){ return buf.IsConnected(); }
	///	listen���Ă��邩�ǂ���
	bool IsListening(){ return buf.IsListening(); }
	///	�����ɓ��͉\�ȃo�C�g��
	int avail(){ return Avail(); }
	int Avail(){ return buf.in_avail() + buf.in_avail_stream(); }
	///	 �ڑ���ؒf����D
	void close(){ Close(); }
	void Close(){ buf.Close(); clear(std::ios::badbit); }
};
}	//	namespace LVRM
#endif