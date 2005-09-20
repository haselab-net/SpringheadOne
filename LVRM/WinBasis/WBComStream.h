#ifndef COMSTREAM_H
#define COMSTREAM_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Base/StrbufBase.h>
#include "WBWin.h"
#include <iostream>

namespace LVRM {
class WBComStream;
class WBComStreambuf;
struct WBComStatus
	{
	enum {FC_NONE, FC_DTRDSR, FC_RTSCTS, FC_HARDWARE,
		FC_XONXOFF, FC_SOFTWARE=FC_XONXOFF} flowControl;
	int		port;
	DWORD	boudRate;
	BYTE	byteSize;
	BYTE	parity;
	BYTE	stopBits;
	WBComStatus();
	};
class WBComStreambuf:public UTStreambufBase, public WBComStatus
	{
	friend WBComStream;
	public:
	enum {COMBUFLEN = 4096};	
	enum {ASCII_XON = 0x11, ASCII_XOFF = 0x13};
	WBComStreambuf(const WBComStatus& cs,
		char* gb, int gl, char* pb, int pl);
	~WBComStreambuf();
	protected:
	HANDLE hFile;
	bool init();
	bool init(const WBComStatus& cs);
	bool openConnection();
	bool setupConnection();
	void closeConnection();
	void cleanup();
	UINT read(void* buf, UINT bufLen);
	UINT write(void* buf, UINT bufLen);
	UINT comAvail();
	UINT comWaiting();
	virtual int in_avail_stream()
		{
		return comAvail();
		}
	};
class WBComStream:public std::iostream{
	char getbuf[1024];
	char putbuf[1024];
	public:
	WBComStatus comStatus;
	WBComStream(const WBComStatus& cs);
	WBComStream();
	~WBComStream();
	WBComStreambuf buf;
	int avail();
	bool init(const WBComStatus& cs);
	};
}	//	namespace LVRM
#endif