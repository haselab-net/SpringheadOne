#ifndef WBPRECISETIMER_H
#define WBPRECISETIMER_H
#pragma once
#include <WinBasis/WBWin.h>


namespace Spr{;
/**	CPU�N���b�N�𗘗p�������m�ȃ^�C�}�[	
	����ڌ��̊��{���񂪍��ꂽ PreciseTimer.h �����ς��܂����D	*/

class WBPreciseTimer{
private:
	static DWORD freq;
	LARGE_INTEGER cycles2;
	DWORD stopWatch;
	bool startFlag;
public:
	WBPreciseTimer();
	void Init(int period = 100);	///<	�J�E���^�̏�����
	void WaitUS(int time);			///<	��s�P�ʂő҂�
	int CountUS();					///<	�O�񂩂�̌o�ߎ��Ԃ���s�P�ʂŌv��
	void CountAndWaitUS(int time);	///<	�O�񂩂�̌o�ߎ��Ԃ���s�P�ʂŌv�����C�S�̂Ƃ���us�P�ʂő҂i��胋�[�v�����p�j
	//	�X�g�b�v�E�H�b�`�@�\
	DWORD Start();					///<	�v���J�n
	DWORD Stop();					///<	�v����~
	DWORD Clear();					///<	�N���A
	static DWORD CPUFrequency(){	///<	CPU�̎��g����Ԃ��D
		return freq;
	}
};

}

#endif
