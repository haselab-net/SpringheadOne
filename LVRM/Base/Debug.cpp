#include "Debug.h"
#include "StrBufBase.h"
#include <stdio.h>
#include <iostream>

namespace LVRM {


typedef void (DebugOutFunc)(const char*);
static DebugOutFunc* debugOutFunc;

DebugPrintf::PrintfFunc::operator ()(const char* fmt, ...){
	int rv = 0;
	if (debugOutFunc){
		va_list list;
		va_start(list, fmt);
		char buf[1024*10];
		rv = vsprintf(buf, fmt, list);
		debugOutFunc(buf);
		va_end(list);
	}
	return rv;
}
void DebugPrintf::Set(void (*out)(const char*)){
	debugOutFunc = out;
}

#ifdef _WIN32
#include <winbase.h>
void defDebugOutFunc(const char* str){
	OutputDebugString(str);
}
#else
void defDebugOutFunc(const char* str){
	printf("%s", str);
}
#endif
DebugPrintf* DebugPrintf::GetInstance(){
	static DebugPrintf dpf;
	if (!debugOutFunc) dpf.Set(defDebugOutFunc);
	return &dpf;
}

class DPFStreambuf:public UTStreambufBase{
public:
	DPFStreambuf(char* gb, size_t gl, char* pb, size_t pl):UTStreambufBase(gb, gl, pb ,pl){
	}
	virtual unsigned int read(void* buf, unsigned int bufLen){
		return 0;
	}
	virtual unsigned int write(void* src, unsigned int len){
		if (debugOutFunc){
			char buf[4096];
			if (len > sizeof buf - 1) len = sizeof buf -1;
			memcpy(buf, src, len);
			buf[len] = '\0';
			debugOutFunc(buf);
		}else{
			len = 0;
		}
		return len;
	}
};

using namespace std;
class DPFStream:public std::ostream{
	char getbuf[1024];
	char putbuf[1024];
	DPFStreambuf buf;
public:
	DPFStream():
	buf(getbuf, sizeof getbuf, putbuf, sizeof putbuf),
	ostream(NULL)
	{
		init(&buf);
		std::streambuf* sbuf = rdbuf();
	}
};

std::ostream& DebugPrintf::Stream(){
	static DPFStream dpfStr;
	dpfStr.flush();
	dpfStr.clear();
	return dpfStr;
}

}	//	namespace LVRM
