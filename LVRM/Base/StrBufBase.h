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
/**	@file strbufBase.h iostream ��h������Ƃ��Ɏg���N���X�̒�` */

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
	//	�V�����X�g���[����h��������ɂ́A�ȉ��̂R�̊֐����I�[�o�[���[�h���Ă��������B
	virtual unsigned int read(void* buf, unsigned int bufLen)=0;	//	bufLen �ȓ��̕��������͂� buf �Ɋi�[����B���͕�������Ԃ��B
	virtual unsigned int write(void* buf, unsigned int bufLen)=0;	//	buf �Ɋi�[���ꂽ bufLen �����̃f�[�^���o�͂��A�o�͂�����������Ԃ��B
	virtual int in_avail_stream(){return 0;}		//	���͉\�ȕ�������Ԃ��B
	};

}	//	namespace LVRM
#endif