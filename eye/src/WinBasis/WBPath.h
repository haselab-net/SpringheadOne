#ifndef WINCMP_MISC_CPATH_H
#define WINCMP_MISC_CPATH_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WBWin.h"
#include <Base/BaseUtility.h>

namespace Spr {;

class SPR_DLL WBPath{
protected:
	UTString path;
public:
	///	�t�@�C���p�X�̎擾
	UTString Path(){ return path; }
	///	�t�@�C���p�X�̐ݒ�
	void Path(UTString p);
	///	�t�@�C���p�X�̃h���C�u��
	UTString Drive();
	///	�t�@�C���p�X�̃h���C�u��
	UTString Dir();
	///	�t�@�C���p�X�̃t�@�C������
	UTString File();
	///	�t�@�C���p�X�̃t�@�C�����̊g���q��������������Ԃ��D
	UTString Main();
	///	�t�@�C���p�X�̊g���q��Ԃ��D
	UTString Ext();
	///	�J�����g�f�B���N�g�����擾����
	UTString GetCwd();
	///	�J�����g�f�B���N�g����ݒ肷��
	bool SetCwd(UTString cwd);

	///	�t���p�X���擾����D
	UTString FullPath();
	///	���΃p�X���擾����D
	UTString RelPath();

	///	�f�B���N�g�����t���p�X��
	UTString FullDir();

	///	�t�@�C���������ϐ� PATH ���猟�����āC�t�@�C���p�X��ݒ�D
	bool Search(UTString file);
};

}	//	namespace Spr

#endif

