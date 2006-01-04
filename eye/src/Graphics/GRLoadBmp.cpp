#ifndef GRBMPLOADER_H
#define GRBMPLOADER_H
#include <WinBasis/WBDllLoader.h>

namespace Spr{;

static WBDllLoader dllLoader("loadBmp.dll", ".;$(SPRINGHEAD)\\bin");	//	�O���[�o���ϐ��Ń��[�_�[�����D

#define DLLFUNC_CTYPE	__cdecl					//	�Ăяo���K��
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	LoadBmpCreate			//	�֐���
#define DLLFUNC_STR		"LoadBmpCreate"			//	�֐���
#define DLLFUNC_ARGDEF	(const char* arg)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(arg)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	�Ăяo���K��
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	LoadBmpGetBmp			//	�֐���
#define DLLFUNC_STR		"LoadBmpGetBmp"			//	�֐���
#define DLLFUNC_ARGDEF	(int h, char* arg)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(h, arg)				//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	�Ăяo���K��
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	LoadBmpRelease			//	�֐���
#define DLLFUNC_STR		"LoadBmpRelease"			//	�֐���
#define DLLFUNC_ARGDEF	(int arg)				//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(arg)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	�Ăяo���K��
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	LoadBmpGetWidth			//	�֐���
#define DLLFUNC_STR		"LoadBmpGetWidth"		//	�֐���
#define DLLFUNC_ARGDEF		(int arg)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(arg)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	�Ăяo���K��
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	LoadBmpGetHeight		//	�֐���
#define DLLFUNC_STR		"LoadBmpGetHeight"		//	�֐���
#define DLLFUNC_ARGDEF		(int arg)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(arg)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	�Ăяo���K��
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	LoadBmpGetBytePerPixel	//	�֐���
#define DLLFUNC_STR		"LoadBmpGetBytePerPixel"	//	�֐���
#define DLLFUNC_ARGDEF		(int arg)		//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(arg)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

#define DLLFUNC_CTYPE	__cdecl					//	�Ăяo���K��
#define DLLFUNC_RTYPE	int						//	�Ԃ�l�̌^ void�̏ꍇ�͒�`���Ă͂Ȃ�Ȃ��D
#define DLLFUNC_NAME	LoadBmpGetSize			//	�֐���
#define DLLFUNC_STR		"LoadBmpGetSize	"		//	�֐���
#define DLLFUNC_ARGDEF		(int arg)			//	�֐��錾���̈���
#define DLLFUNC_ARGCALL	(arg)					//	�֐��Ăяo���̈���
#include <WinBasis/WBDllLoaderImpl.h>

}

#endif
