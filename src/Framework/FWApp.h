#ifndef FWAPP_H
#define FWAPP_H
#include "FWPointer.h"
#include <SceneGraph/SGScene.h>
#include <Graphics/GRRender.h>
#include <HIS/HIMouse.h>
#include <Device/DVDeviceManager.h>
#include <WinBasis/WBCriticalSection.h>
#include <WinBasis/WBMMTimer.h>

namespace Spr{;

class FWApp:public UTRefCount{
protected:
	HWND hWnd;							//	�E�B���h�E�n���h��

	WBCriticalSection cs;				//	�N���e�B�J���Z�N�V����
	Spr::WBMMTimer timer;				//	�V�~�����[�V�����p�}���`���f�B�A�^�C�}�[
	UINT simuTimerId;					//	�V�~�����[�V�����pWM�^�C�}�[��ID
	float defaultTimeStep;				//	�V�[���Ɏ��ԍ��݂������ĂȂ��Ƃ��Ɏg�����ԍ���
	bool bPause;						//	�ꎞ��~���t���O
	volatile bool bBusy;				//	�v�Z���Ԃɍ���Ȃ��Ƃ� true �ɂȂ�D
	bool bOutForce;						//	�͊o�񎦂��s�����ǂ���
	float simuTimerPeriod;
	//	
	UTString initialLoadFile;			//	�V�[���������ł���O�ɁC���[�h���Ă΂ꂽ�ꍇ�C�t�@�C�����������Ɋo���Ă���
	UTString lastLoadFile;				//	�Ō�Ƀ��[�h�����t�@�C����
	std::vector<std::string> fileList;	//	�t�@�C����؂�ւ���ꍇ�̃t�@�C�����X�g
	int fileListCur;					//	�t�@�C�����X�g���ł̈ʒu
public:
	enum {
		TIMER_SIMU = 1,
		TIMER_DRAW = 2,
	};
	enum {TIMERTICK = 1000};
	UTRef<SGScene> scene;
	UTRef<GRRender> render; 
	FWPointers pointers;
	UTRef<HIMouse> mouse;
	DVDeviceManager devMan;

	FWApp();
	virtual ~FWApp();

	void InitDevice();
	float GetSimuTimerPeriod(){return simuTimerPeriod;}
	virtual void Init(HWND hWnd);
	virtual void Step();
	virtual bool AddHis(const char* type);
	virtual void ClearHis();
	virtual void ConnectHis();

	virtual bool PreviewMessage(MSG* pMsg);

	virtual void Draw();
	virtual void CreateD3DRender();
	virtual void CreateGLRender();
	virtual void LoadList();
	virtual void Load(UTString fn);
	virtual void LoadImp(UTString fn){}
	virtual void Save(UTString fn);
	virtual void SaveImp(UTString fn){}
	virtual bool OnKeyDown(unsigned int key);
	virtual void Reset();
protected:
	static void TimerFunc(void* arg);
	static void CALLBACK WMTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);

};

}
#endif
