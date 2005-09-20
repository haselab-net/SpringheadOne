#ifndef MMTIMER_H
#define MMTIMER_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "WBWin.h"
namespace LVRM {

/**	�}���`���f�B�A�^�C�}�[.
	�^�C�}�[���쒆�Ƀf�o�b�K���g����OS��������̂ŁC�f�o�b�O�p��
	�X���b�h�ɂ��[�����샂�[�h��p�ӂ��Ă���D	*/
class WBMMTimer{
	static int count;
	int tick;
	public:
	typedef void MMTimerFunc(void* arg);

	protected:
	static UINT resolution;		///<	�^�C�}�[�̎��Ԑ��x
	UINT interval;				///<	�^�C�}�[�̎��ԊԊu
	MMTimerFunc* func;			///<	���Ԃ����邽�тɌĂ΂��R�[���o�b�N�֐��D
	void* arg;					///<	�R�[���o�b�N�֐��ɓn������
	UINT timerID;
	volatile bool bCreated;		///<	�^�C�}�[�����삵�Ă��邩�ǂ���
	volatile bool bThread;		///<	�^�C�}�[���X���b�h�Ƃ��ċ[�����삵�Ă��邩�ǂ���
    volatile bool bRun;			///<	�R�[���o�b�N�֐������s�����ǂ���
	HANDLE hThread;				///<	�[������p�X���b�h
	static void CALLBACK TimerCallback(UINT uID, UINT, DWORD dwUser, DWORD, DWORD);
	static DWORD WINAPI ThreadCallback(void* arg);
	void BeginPeriod();
	void EndPeriod();

	public:
	volatile int heavy;

	WBMMTimer();					///<	�R���X�g���N�^
	~WBMMTimer();					///<	�f�X�g���N�^
	UINT Resolution();				///<	�^�C�}�[�̎��Ԑ��x
	void Resolution(UINT res);		///<	�^�C�}�[�̎��Ԑ��x�̐ݒ�
	UINT Interval();				///<	�^�C�}�[�̎��ԊԊu
	void Interval(UINT i);			///<	�^�C�}�[�̎��ԊԊu�̐ݒ�
	void Set(MMTimerFunc* f, void* arg);
									///<	�R�[���o�b�N�֐��̐ݒ�
	bool Create();					///<	�^�C�}�[����J�n
	bool Thread();					///<	�^�C�}�[�[������J�n
	bool IsCreated(){return bCreated;}
									///<	�^�C�}�[�����삵�Ă��邩�ǂ���
	bool IsThread(){return bThread;}
									///<	�^�C�}�[���[�����삵�Ă��邩�ǂ���
	void Release();					///<	�^�C�}�[�̒�~
};

}	//	namespace LVRM
#endif
