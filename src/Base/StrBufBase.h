#ifndef STRBUFBASE_H
#define STRBUFBASE_H
#if defined _MSC_VER && _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "Env.h"
#include <cstdio>
#include <iostream>
/**	@file StrBufBase.h iostream ��h������Ƃ��Ɏg���N���X�̒�` */
#ifdef __BORLANDC__
 #define BCC_CDECL __cdecl
#else
 #define BCC_CDECL
#endif

namespace Spr {

template <class T>
class UTPrintfStream:public T
	{
	public:
	explicit UTPrintfStream(std::streambuf *sb)
		:T(sb)
		{
		}
	int SPR_CDECL printf(char* fmt, ...)
		{
		char buf[4096];
		return _vsnprintf(buf, sizeof(buf), fmt, (char*)&fmt);
		}
	};
class SPR_DLL UTStreambufBase:public std::streambuf
	{
	char* pbuf,*gbuf;
	int plen, glen;
	public:
	int lahead;
#ifdef _MSC_VER
	UTStreambufBase(std::_Uninitialized);
#endif
	UTStreambufBase(char* gb, int gl, char* pb, int pl);
    BCC_CDECL ~UTStreambufBase();
	protected:
	virtual int BCC_CDECL overflow(int c = EOF);
	virtual int BCC_CDECL underflow();
	virtual int BCC_CDECL sync();
	virtual pos_type BCC_CDECL seekoff(off_type off, std::ios_base::seekdir way,
		std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
	//	�V�����X�g���[����h��������ɂ́A�ȉ��̂R�̊֐����I�[�o�[���[�h���Ă��������B
	virtual unsigned int read(void* buf, unsigned int bufLen)=0;	//	bufLen �ȓ��̕��������͂� buf �Ɋi�[����B���͕�������Ԃ��B
	virtual unsigned int write(void* buf, unsigned int bufLen)=0;	//	buf �Ɋi�[���ꂽ bufLen �����̃f�[�^���o�͂��A�o�͂�����������Ԃ��B
	virtual int in_avail_stream(){return 0;}		//	���͉\�ȕ�������Ԃ��B
	};

}	//	namespace Spr
#endif
