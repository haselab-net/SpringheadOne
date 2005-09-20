#ifndef BASE_DEBUG_H
#define BASE_DEBUG_H
#include "Env.h"
#include <stdarg.h>
#include <iostream>

/**	@file Debug.h デバッグ用ユーティリティークラス・関数の定義.	*/

/**	デバッグ用 printf 関数.
	@verbatim
	DPF("メッセージ:%s", msg);@endverbatim
	の様に使う．							*/
#define DPF	LVRM::DebugPrintf::GetInstance()->FileLine(__FILE__, __LINE__)
/**	デバッグ用 出力ストリーム.
	@verbatim
	DSTR << "メッセージ:" << msg;@endverbatim
	の様に使う．							*/
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

#if 0	//	プログラムの動作を詳細に報告させるなら 1
 #define TRACEALL DebugPrintf
#else
 #define TRACEALL (void*)
#endif

}	//	namespace LVRM

#endif
