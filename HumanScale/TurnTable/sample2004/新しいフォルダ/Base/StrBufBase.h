#ifndef STRBUFBASE_H
#define STRBUFBASE_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "Env.h"
#ifdef _WIN32
#include "../WinBasis/WBWin.h"
#endif
#include <stdio.h>
#include <iostream>
/**	@file strbufBase.h iostream を派生するときに使うクラスの定義 */

namespace LVRM {

template <class T>
class UTPrintfStream:public T
	{
	public:
	explicit UTPrintfStream(std::streambuf *sb)
		:T(sb)
		{
		}
	int printf(char* fmt, ...)
		{
		char buf[4096];
		return _vsnprintf(buf, sizeof(buf), fmt, (char*)&fmt);
		}
	};
class UTStreambufBase:public std::streambuf
	{
//	typedef int int_type;
	char* pbuf,*gbuf;
	int plen, glen;
	public:
	int lahead;
	UTStreambufBase(char* gb, int gl, char* pb, int pl);
	protected:
	int_type overflow(int c);
	int_type underflow();
	int sync();
	virtual pos_type seekoff(off_type off, std::ios_base::seekdir way,
		std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
	//	新しいストリームを派生させるには、以下の３つの関数をオーバーロードしてください。
	virtual unsigned int read(void* buf, unsigned int bufLen)=0;	//	bufLen 以内の文字数入力し buf に格納する。入力文字数を返す。
	virtual unsigned int write(void* buf, unsigned int bufLen)=0;	//	buf に格納された bufLen 文字のデータを出力し、出力した文字数を返す。
	virtual int in_avail_stream(){return 0;}		//	入力可能な文字数を返す。
	};

}	//	namespace LVRM
#endif