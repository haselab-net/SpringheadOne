#ifndef WBWINDRIVER_H
#define WBWINDRIVER_H


#include "WBPath.h"
#include <base/BaseUtility.h>
#include <winsvc.h>

namespace Spr {
///	Windows�̃f�o�C�X�h���C�o���J���N���X
class SPR_DLL WBLoadWindowsDriver{
protected:
	WBPath path;
	std::string drvName;
	SC_HANDLE hSCMan;
	SC_HANDLE hService;
public:
	///
	WBLoadWindowsDriver();
	///
	~WBLoadWindowsDriver();

	///	�f�o�C�X�h���C�o�̃p�X��
	const char* Path(){ return path.Path().c_str(); }
	///	�f�o�C�X�h���C�o�̃p�X���̐ݒ�
	void Path(const char* p);
	///	�f�o�C�X�h���C�o��
	const char* Name(){ return drvName.c_str(); }
	///	�t�@�C�����Ńf�o�C�X�h���C�o�̃p�X����ݒ�
	void File(const char* f);
	///	�f�o�C�X�h���C�o�̃C���X�g�[��
	void Install();
	///	�T�[�r�X�̊J�n
	void Start();
	///	�T�[�r�X�̒�~
	void Stop();
	///	�f�o�C�X�h���C�o�̍폜
	void Remove();
	///	�J���Ă��邩�ǂ���
	bool IsOpen();
	///	�J�n���Ă��邩�ǂ���
	bool IsStart();
protected:
	///	Windows �� local service driver �ɐڑ�����B
	void Init();
	///	Windows �� local service driver �Ƃ̐ڑ�����������B
	void Cleanup();
	///	service ���J���B
	void Open();
};

}	//	namespace Spr

#endif

