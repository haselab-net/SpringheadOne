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

	// DirectSoundの生成
	if(DirectSoundCreate8(NULL, &DSObject, NULL) != DS_OK){
		errorMsg = "DirectSoundオブジェクトの生成に失敗しました。";
		return false;
	}

	// DirectSoundの協調レベルの設定
	if(DSObject->SetCooperativeLevel(hwnd, DSSCL_NORMAL) != DS_OK){
		errorMsg = "DirectSoundの協調レベルの設定に失敗しました";
		return false;
	}

	return true;
}

void CEasyDS::DS_END()
{
	//ダイレクトサウンドオブジェクトの開放
	if(DSObject != NULL) RELEASE_COM(DSObject);
}

CEasyDSWaveBuffer *CEasyDS::Waves(int index)
{
	if( index >= waveMax ){
		// 例外投げるといいかも。
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

	// WAVEファイルをマルチメディアI/O関数で開きます
	hmfr = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF);
	if(hmfr == NULL) {
		errorMsg = "ファイルオープンに失敗しました";
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
		errorMsg = "Waveファイルが見つかりません";
		return FALSE;
	}

	child.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(hmfr, &child, &parent, 0)) {
		mmioClose(hmfr,0);
		errorMsg = "Waveファイルが見つかりません";
		return false;
	}

	// WAVのフォーマット情報を読み込みます
	if(mmioRead(hmfr, (char *)&wfmtx, sizeof(wfmtx)) != sizeof(wfmtx)) {
		mmioClose(hmfr, 0);
		errorMsg = "ウエイブフォーマットを読み込めません";
		return false;
	}

	// このファイルがWAVEファイルであることを確認します
	if(wfmtx.wFormatTag != WAVE_FORMAT_PCM) {
		mmioClose(hmfr,0);
		errorMsg = "不正なウエイブフォーマットです";
		return false;
	}

	if(mmioAscend(hmfr, &child, 0)) {
		mmioClose(hmfr,0);
		errorMsg = "ウエイブファイルにアクセスできません";
		return false;
	}

	// チャンクを構造体に読み込みます
	child.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(hmfr,&child, &parent, MMIO_FINDCHUNK)) {
		mmioClose(hmfr,0);
		errorMsg = "ウエイブファイルにデータがありません";
		return false;
	}

	// メモリを割り当て、データを読み込みます
	BYTE *pBuffer= new BYTE[child.cksize];
	if((DWORD)mmioRead(hmfr, (char *)pBuffer, child.cksize) != child.cksize) {
		mmioClose(hmfr,0);
		errorMsg = "ウエイブデータ読み込みに失敗しました";
		delete [] pBuffer;
		return false;
	}

	// WAVEファイルを閉じます
	mmioClose(hmfr,0);

	// DirectSoundバッファを作成します
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

	// すでにバッファがある場合はそこに書き込む
	//if( ここら辺かく。
	// ...
	// そうでない場合は新規作成してbufferに追加
	if( DSObject->CreateSoundBuffer(&dsbdesc, &pDSBuffer, NULL) != DS_OK) {
		errorMsg = "サウンドバッファの生成に失敗しました";
		delete [] pBuffer;
		return false;
	}

	// WAVファイルのデータをバッファに転送します
	// バッファの数だけ繰り返したほうが良い。
	LPVOID written1, written2;
	DWORD length1,length2;
	if(pDSBuffer->Lock(0, child.cksize, &written1, &length1, &written2, &length2, 0) == DSERR_BUFFERLOST) {
		pDSBuffer->Restore();
		pDSBuffer->Lock(0, child.cksize, &written1, &length1, &written2, &length2, 0);
	}

	CopyMemory(written1, pBuffer, length1);

	if(written2 != NULL) CopyMemory(written2, pBuffer+length1, length2);

	pDSBuffer->Unlock(written1, length1, written2, length2);

	// 作業用のバッファを解放します
	delete [] pBuffer;

	// buffersの中身を空にして追加。
	Clear();
	buffers.push_back(pDSBuffer);

	return true;
}

bool CEasyDSWaveBuffer::SetSynchronous(int synchronous)
{
	// Waveファイルが読み込まれていない場合はfalseを返す
	if( buffers.size() == 0 ) return false;

	// パラメータチェック
	if( synchronous < 1 ){
		errorMsg = "最大同時発音数は1以上にしてください。";
		return false;
	}

	// 増減が必要とされる同時発音数
	int require = synchronous - buffers.size();
	if( require == 0 ) return true;

	// 現在のバッファの再生を止める
	int i;
	for( i=0; i<buffers.size(); i++ ){
		buffers[i]->Stop();
	}

	// バッファが少ない場合は増やす
	LPDIRECTSOUNDBUFFER tempBuffer;
	std::vector<LPDIRECTSOUNDBUFFER> addBuffers;
	while( require > 0 ){
		tempBuffer = NULL;
		// Bufferを複製
		if( DSObject->DuplicateSoundBuffer( buffers[0],&tempBuffer ) != DS_OK ){
			// 複製に失敗した場合はaddBufferを開放してfalseを返す。
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

	// バッファが多い場合は減らす
	while( require < 0 ){
		if( buffers.back() ){
			buffers.back()->Release();
		}
		buffers.pop_back();
		require++;
	}

	// bufferPointを0に。
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
	// DirectSoundバッファの開放
	for( int i=0; i<buffers.size(); i++ ){
		if( buffers[i] ){
			buffers[i]->Stop(); //再生をとめる
			RELEASE_COM( buffers[i] );
		}
	}

	// vectorオブジェクトを空に
	buffers.clear();
}
