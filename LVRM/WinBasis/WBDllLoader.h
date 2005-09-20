#ifndef WBDLLLOADER_H
#define WBDLLLOADER_H
#include "WBWin.h"
#include <base/utilities.h>

namespace LVRM {;
/**	DLL�����[�h����N���X�D�ȉ��̂悤�ɂ���ƁC�ʏ�̃��C�u�����Ɠ��l��
	DLL�𓮓I�Ƀ��[�h���Ďg�p�ł���D

�g�����F
static WBDllLoader dllLoader("filename.dll");	//	�O���[�o���ϐ��Ń��[�_�[�����D

//	int DllFunc(int arg)  �̏ꍇ
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	DllFunc					//	�֐���
#define DLLFUNC_ARG		(int arg)				//	�֐��錾���̈���
#define DLLFUNC_CALL	(arg)					//	�֐��Ăяo���̈���
#include "WBDllLoaderImp.h"
*/
class WBDllLoader{
	HMODULE module;
	char dllName[1024];
	char addPath[1024];
public:
	WBDllLoader(){ Init(); }
	WBDllLoader(const char* dllName, const char* addPath=NULL){
		Init();
		Load(dllName, addPath);
	}
	~WBDllLoader(){ Cleanup(); }
	bool Load(const char* dllName=NULL, const char* addPath=NULL);
	void* GetProc(const char* name);
protected:
	void Init();
	void Cleanup();
};

}	//	namespace LVRM;

#endif
