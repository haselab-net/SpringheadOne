#ifdef USE_SOUND
#ifndef __EASYDS_H_
#define __EASYDS_H_

#include <windows.h>
//#include <stdio.h>
#include <mmsystem.h>
#include <dsound.h>
#include <vector>

class CEasyDS;
class CEasyDSWaveBuffer;

class CEasyDS
{
	HWND hwnd;
	char *errorMsg;
	bool available;
	int waveMax;

	// �_�C���N�g�T�E���h�I�u�W�F�N�g
	LPDIRECTSOUND8 DSObject;
	CEasyDSWaveBuffer **waveObjects;

	// �l�h�c�h�Đ��p�ϐ�
//	char MciBuffer[32];
//	bool PlayFlg;

	// ���֐�
	void RELEASE_COM(IUnknown *x) { x->Release(); x=NULL; }

	bool DS_INIT();
	void DS_END();

public:
	CEasyDS(HWND _hwnd,int _waveMax);
	~CEasyDS();
	bool IsAvailable(){ return available; }
	char* ErrorMsg(){ return errorMsg; }
	CEasyDSWaveBuffer *Waves(int index);
};

class CEasyDSWaveBuffer
{
	LPDIRECTSOUND8 DSObject;
	std::vector<LPDIRECTSOUNDBUFFER> buffers;
	int synchronous; //�ő哯��������
	int bufferPoint;
	char *errorMsg;

	void RELEASE_COM(IUnknown *x) { x->Release(); x=NULL; }
	bool CreateDuplexBuffers(int index);

public:
	CEasyDSWaveBuffer(LPDIRECTSOUND8 _DSObject);
	~CEasyDSWaveBuffer();
//	bool IsAvailable(){ return available; }	// DirectSound�𗘗p�\��
	char* ErrorMsg(){ return errorMsg; }	// �G���[���b�Z�[�W��Ԃ�
	bool LoadFromFile( char *filename );	// Wave�t�@�C����ǂݍ���Ńo�b�t�@�Ɋi�[
	int GetSynchronous(){ return buffers.size(); } // ������������Ԃ�
	bool SetSynchronous(int synchronous);	// ������������ݒ肷��
	bool Play();							// �Đ�����
	void Clear();							// �ۑ�����Ă���o�b�t�@����������
};


#endif // end of __EASYDS_H_
#endif // USE_SOUND