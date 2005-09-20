#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H
#include "Env.h"
#include <stdarg.h>
#include <iostream>

/**	@file Debug.h �f�o�b�O�p���[�e�B���e�B�[�N���X�E�֐��̒�`.	*/

/**	�f�o�b�O�p printf �֐�.
	@verbatim
	DPF("���b�Z�[�W:%s", msg);@endverbatim
	�̗l�Ɏg���D							*/
#define DPF	LVRM::DebugPrintf::GetInstance()->FileLine(__FILE__, __LINE__)
/**	�f�o�b�O�p �o�̓X�g���[��.
	@verbatim
	DSTR << "���b�Z�[�W:" << msg;@endverbatim
	�̗l�Ɏg���D							*/
#define DSTR LVRM::DebugPrintf::GetInstance()->Stream()

#include <assert.h>
#define VRASSERT assert

namespace LVRM {

class DebugPrintf{
public:
	static DebugPrintf* GetInstance();
	struct PrintfFunc{
		const char* file;
		int line;
		PrintfFunc(const char* f, int l):file(f), line(l){}
		int operator() (const char*, ...);
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

}	//	namespace LVRM

#endif
