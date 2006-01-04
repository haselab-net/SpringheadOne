#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H
#include "Env.h"
#include <stdarg.h>
#include <iostream>

/**	@file BaseDebug.h �f�o�b�O�p���[�e�B���e�B�[�N���X�E�֐��̒�`.	*/

/**	�f�o�b�O�p printf �֐�.
	@verbatim
	DPF("���b�Z�[�W:%s", msg);@endverbatim
	�̗l�Ɏg���D							*/
#define DPF	Spr::DebugPrintf::GetInstance()->FileLine(__FILE__, __LINE__)
/**	�f�o�b�O�p �o�̓X�g���[��.
	@verbatim
	DSTR << "���b�Z�[�W:" << msg;@endverbatim
	�̗l�Ɏg���D							*/
#define DSTR (Spr::DebugPrintf::GetInstance()->Stream())

#include <assert.h>

namespace Spr {

class SPR_DLL DebugPrintf{
public:
	static DebugPrintf* GetInstance();
	struct SPR_DLL PrintfFunc{
		const char* file;
		int line;
		PrintfFunc(const char* f, int l):file(f), line(l){}
		int SPR_CDECL operator() (const char*, ...);
	};
	PrintfFunc FileLine(const char* f=0, int l=-1){
		return PrintfFunc(f, l);
	}
	std::ostream& Stream();
	static void Set(void (*out)(const char*));
};

#if 0	//	�v���O�����̓�����ڍׂɕ񍐂�����Ȃ� 1
 #define TRACEALL DebugPrintf
#else
 #define TRACEALL (void*)
#endif

#if defined(_DEBUG) && !defined(NO_DEBUG_EVAL)
 #define DEBUG_EVAL(x) x
#else
 #define DEBUG_EVAL(x)
#endif
}	//	namespace Spr

#endif
