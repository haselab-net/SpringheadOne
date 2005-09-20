#include "EasyDS.h"

CEasyDS::CEasyDS(HWND _hwnd,int _waveMax)
{
	hwnd = _hwnd;
	waveMax = _waveMax;
	available = false;
	errorMsg = NULL;
//	PlayFlag = false;
	DSObject = NULL;
	waveObjects = NULL;

	bool result = DS_INIT();
	if( !result ){
		return;
	}

	waveObjects = new CEasyDSWaveBuffer*[waveMax];
	for( int i=0; i<waveMax; i++ ){
		waveObjects[i] = new CEasyDSWaveBuffer( DSObject );
	}

	available = true;
}

CEasyDS::~CEasyDS()
{
	for( int i=0; i<waveMax; i++ ){
		if( waveObjects[i] ){
			delete waveObjects[i];
		}
	}

	DS_END();
}

bool CEasyDS::DS_INIT()
{
	int i;

	// DirectSound�̐���
	if(DirectSoundCreate8(NULL, &DSObject, NULL) != DS_OK){
		errorMsg = "DirectSound�I�u�W�F�N�g�̐����Ɏ��s���܂����B";
		return false;
	}

	// DirectSound�̋������x���̐ݒ�
	if(DSObject->SetCooperativeLevel(hwnd, DSSCL_NORMAL) != DS_OK){
		errorMsg = "DirectSound�̋������x���̐ݒ�Ɏ��s���܂���";
		return false;
	}

	return true;
}

void CEasyDS::DS_END()
{
	//�_�C���N�g�T�E���h�I�u�W�F�N�g�̊J��
	if(DSObject != NULL) RELEASE_COM(DSObject);
}

CEasyDSWaveBuffer *CEasyDS::Waves(int index)
{
	if( index >= waveMax ){
		// ��O������Ƃ��������B
		return NULL;
	}

	return waveObjects[index];
}

CEasyDSWaveBuffer::CEasyDSWaveBuffer(LPDIRECTSOUND8 _DSObject)
{
	DSObject = _DSObject;
	bufferPoint = 0;
	errorMsg = NULL;
}

CEasyDSWaveBuffer::~CEasyDSWaveBuffer()
{
	Clear();
}

bool CEasyDSWaveBuffer::LoadFromFile( char *filename )
{
	HMMIO	hmfr;
	MMCKINFO parent,child;
	WAVEFORMATEX wfmtx;
	LPDIRECTSOUNDBUFFER pDSBuffer = NULL;

	// WAVE�t�@�C�����}���`���f�B�AI/O�֐��ŊJ���܂�
	hmfr = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF);
	if(hmfr == NULL) {
		errorMsg = "�t�@�C���I�[�v���Ɏ��s���܂���";
		return false;
	}

	parent.ckid         = (FOURCC)0;
	parent.cksize       = 0;
	parent.fccType      = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags      = 0;
	child               = parent;

	parent.fccType = mmioFOURCC('W','A','V','E');
	if(mmioDescend(hmfr, &parent, NULL, MMIO_FINDRIFF)) {
		mmioClose(hmfr,0);
		errorMsg = "Wave�t�@�C����������܂���";
		return FALSE;
	}

	child.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmfr, &child, &parent, 0)) {
		mmioClose(hmfr,0);
		errorMsg = "Wave�t�@�C����������܂���";
		return false;
	}

	// WAV�̃t�H�[�}�b�g����ǂݍ��݂܂�
	if(mmioRead(hmfr, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx)) {
		mmioClose(hmfr, 0);
		errorMsg = "�E�G�C�u�t�H�[�}�b�g��ǂݍ��߂܂���";
		return false;
	}

	// ���̃t�@�C����WAVE�t�@�C���ł��邱�Ƃ��m�F���܂�
	if(wfmtx.wFormatTag != WAVE_FORMAT_PCM) {
		mmioClose(hmfr,0);
		errorMsg = "�s���ȃE�G�C�u�t�H�[�}�b�g�ł�";
		return false;
	}

	if(mmioAscend(hmfr, &child, 0)) {
		mmioClose(hmfr,0);
		errorMsg = "�E�G�C�u�t�@�C���ɃA�N�Z�X�ł��܂���";
		return false;
	}

	// �`�����N���\���̂ɓǂݍ��݂܂�
	child.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmfr,&child, &parent, MMIO_FINDCHUNK)) {
		mmioClose(hmfr,0);
		errorMsg = "�E�G�C�u�t�@�C���Ƀf�[�^������܂���";
		return false;
	}

	// �����������蓖�āA�f�[�^��ǂݍ��݂܂�
	BYTE *pBuffer= new BYTE[child.cksize];
	if((DWORD)mmioRead(hmfr, (char *)pBuffer, child.cksize) != child.cksize) {
		mmioClose(hmfr,0);
		errorMsg = "�E�G�C�u�f�[�^�ǂݍ��݂Ɏ��s���܂���";
		delete [] pBuffer;
		return false;
	}

	// WAVE�t�@�C������܂�
	mmioClose(hmfr,0);

	// DirectSound�o�b�t�@���쐬���܂�
	DSBUFFERDESC dsbdesc;
	WAVEFORMATEX wformat;
	
	memset(&wformat,0,sizeof(WAVEFORMATEX));
	wformat.wFormatTag      = WAVE_FORMAT_PCM;
	wformat.nChannels	     = wfmtx.nChannels;
	wformat.nSamplesPerSec	 = wfmtx.nSamplesPerSec;
	wformat.nAvgBytesPerSec = wfmtx.nAvgBytesPerSec;
	wformat.nBlockAlign     = wfmtx.nBlockAlign;
	wformat.wBitsPerSample     = wfmtx.wBitsPerSample;
	wformat.cbSize = 0;
/*
	PCMWAVEFORMAT pcmwf;

	memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
	pcmwf.wf.wFormatTag      = WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels	     = wfmtx.nChannels;
	pcmwf.wf.nSamplesPerSec	 = wfmtx.nSamplesPerSec;
	pcmwf.wf.nBlockAlign     = wfmtx.nBlockAlign;
	pcmwf.wf.nAvgBytesPerSec = wfmtx.nAvgBytesPerSec;
	pcmwf.wBitsPerSample     = wfmtx.wBitsPerSample;
*/

	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize        = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags       = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME
								| DSBCAPS_CTRLPAN;

	dsbdesc.dwBufferBytes = child.cksize;
//	dsbdesc.lpwfxFormat   = (LPWAVEFORMATEX)&pcmwf;
	dsbdesc.lpwfxFormat   = &wformat;
	dsbdesc.guid3DAlgorithm = GUID_NULL;

	// ���łɃo�b�t�@������ꍇ�͂����ɏ�������
	//if( ������ӂ����B
	// ...
	// �����łȂ��ꍇ�͐V�K�쐬����buffer�ɒǉ�
	if( DSObject->CreateSoundBuffer(&dsbdesc, &pDSBuffer, NULL) != DS_OK) {
		errorMsg = "�T�E���h�o�b�t�@�̐����Ɏ��s���܂���";
		delete [] pBuffer;
		return false;
	}

	// WAV�t�@�C���̃f�[�^���o�b�t�@�ɓ]�����܂�
	// �o�b�t�@�̐������J��Ԃ����ق����ǂ��B
	LPVOID written1, written2;
	DWORD length1,length2;
	if(pDSBuffer->Lock(0, child.cksize, &written1, &length1, &written2, &length2, 0) == DSERR_BUFFERLOST) {
		pDSBuffer->Restore();
		pDSBuffer->Lock(0, child.cksize, &written1, &length1, &written2, &length2, 0);
	}

	CopyMemory(written1, pBuffer, length1);

	if(written2 != NULL) CopyMemory(written2, pBuffer+length1, length2);

	pDSBuffer->Unlock(written1, length1, written2, length2);

	// ��Ɨp�̃o�b�t�@��������܂�
	delete [] pBuffer;

	// buffers�̒��g����ɂ��Ēǉ��B
	Clear();
	buffers.push_back(pDSBuffer);

	return true;
}

bool CEasyDSWaveBuffer::SetSynchronous(int synchronous)
{
	// Wave�t�@�C�����ǂݍ��܂�Ă��Ȃ��ꍇ��false��Ԃ�
	if( buffers.size() == 0 ) return false;

	// �p�����[�^�`�F�b�N
	if( synchronous < 1 ){
		errorMsg = "�ő哯����������1�ȏ�ɂ��Ă��������B";
		return false;
	}

	// �������K�v�Ƃ���铯��������
	int require = synchronous - buffers.size();
	if( require == 0 ) return true;

	// ���݂̃o�b�t�@�̍Đ����~�߂�
	int i;
	for( i=0; i<buffers.size(); i++ ){
		buffers[i]->Stop();
	}

	// �o�b�t�@�����Ȃ��ꍇ�͑��₷
	LPDIRECTSOUNDBUFFER tempBuffer;
	std::vector<LPDIRECTSOUNDBUFFER> addBuffers;
	while( require > 0 ){
		tempBuffer = NULL;
		// Buffer�𕡐�
		if( DSObject->DuplicateSoundBuffer( buffers[0],&tempBuffer ) != DS_OK ){
			// �����Ɏ��s�����ꍇ��addBuffer���J������false��Ԃ��B
			while( addBuffers.size() ){
				if( addBuffers.back() ){
					addBuffers.back()->Release();
				}
				addBuffers.pop_back();
			}
			return false;
		}
		addBuffers.push_back( tempBuffer );
		require--;
	}
	for( i=0; i<addBuffers.size(); i++ ){
		buffers.push_back( addBuffers[i] );
	}

	// �o�b�t�@�������ꍇ�͌��炷
	while( require < 0 ){
		if( buffers.back() ){
			buffers.back()->Release();
		}
		buffers.pop_back();
		require++;
	}

	// bufferPoint��0�ɁB
	bufferPoint = 0;

	return true;
}

bool CEasyDSWaveBuffer::Play()
{
	if( buffers.size() == 0 ){
		return false;
	}

	buffers[bufferPoint]->SetCurrentPosition(0);
	buffers[bufferPoint]->Play(0,0,0);
	bufferPoint++;
	if( bufferPoint > buffers.size()-1 ){
		bufferPoint = 0;
	}

	return true;
}

void CEasyDSWaveBuffer::Clear()
{
	// DirectSound�o�b�t�@�̊J��
	for( int i=0; i<buffers.size(); i++ ){
		if( buffers[i] ){
			buffers[i]->Stop(); //�Đ����Ƃ߂�
			RELEASE_COM( buffers[i] );
		}
	}

	// vector�I�u�W�F�N�g�����
	buffers.clear();
}
