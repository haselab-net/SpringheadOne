/**	WBDllLoader.h ���g�p����ۂɁCDLL�֐��̃X�^�u���C���v�������g����ۂɎg�p����w�b�_�D
	�ڍׂ́C WBDllLoader.h ���Q��
*/
#ifndef DLLLOADER_ReportError
#define DLLLOADER_ReportError
static void ReportError(const char* pname){
	DSTR << "Fail to find '" << pname << "'" << std::endl;
}
#endif

#ifdef DLLFUNC_RTYPE
	DLLFUNC_RTYPE DLLFUNC_CTYPE DLLFUNC_NAME DLLFUNC_ARGDEF{
		typedef DLLFUNC_RTYPE DLLFUNC_CTYPE functype DLLFUNC_ARGDEF;
		static functype* func=NULL;
		static bool bFirst = true;
		if (!func){
			func = (functype*)dllLoader.GetProc(DLLFUNC_STR);
			if (!func){
				if (bFirst) ReportError(DLLFUNC_STR);
				bFirst = false;
				return DLLFUNC_RTYPE();
			}
		}
		return func DLLFUNC_ARGCALL;
	}
	#undef DLLFUNC_RTYPE
#else
	void DLLFUNC_CTYPE DLLFUNC_NAME DLLFUNC_ARGDEF{
		typedef void DLLFUNC_CTYPE functype DLLFUNC_ARGDEF;
		static functype* func=NULL;
		bool bFirst = true;
		if (!func){
			func = (functype*)dllLoader.GetProc(DLLFUNC_STR);
			if (!func){
				if (bFirst) ReportError(DLLFUNC_STR);
				bFirst = false;
				return;
			}
		}
		func DLLFUNC_ARGCALL;
	}
#endif

#undef DLLFUNC_NAME
#undef DLLFUNC_STR
#undef DLLFUNC_ARGDEF
#undef DLLFUNC_ARGCALL
#undef DLLFUNC_CTYPE 
#undef tokenToString
