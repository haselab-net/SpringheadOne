#include "WXAMSockSink.h"
#include "WXAMBaseClasses.h"
#include "WXAMGuidInfo.h"
#include "WXAMSockDef.h"
#include <DVDMedia.h>
#include <WinBasis/WBSockStr.h>

namespace Spr {;
using namespace Spr;

//-----------------------------------------------------------------------------
//	WXAMSockSinkSender
inline std::ostream& operator << (std::ostream& os, REFERENCE_TIME t){
	os.width(8);
	std::ios::fmtflags f = os.flags();
	os.flags(std::ios::hex | std::ios::uppercase);
	os.fill('0');
	os << (long)(t << 32);
	os.width(8);
	os.flags(std::ios::hex | std::ios::uppercase);
	os.fill('0');
	os << (long)t;
	os.flags(f);
	return os;
}
class WXAMSockSinkImp;
class WXAMSockSinkSender:public UTRefCount{
	char buffer[1024 * 640];	//	����o�b�t�@
public:
	WBSockStream sock;
	int cur;					//	�o�b�t�@�̈ʒu
	REFERENCE_TIME lastTime;
	WXAMSockSinkSender(){
		cur = 4;
		sock.Connect("localhost", 11000);
		lastTime = 0;
	}
	void Send(void* ptr, unsigned int sz){
		memcpy(buffer+cur, ptr, sz);
		cur += sz;
	}
	void Flush(){
		if (cur > 4){
			int len = cur - 4;
			memcpy(buffer, &len, 4);
			sock.write(buffer, cur);
			sock.flush();
			cur = 4;
		}
	}
};
class WXAMSockSinkInputPin:public CRendererInputPin{
public:
    WXAMSockSinkInputPin(CBaseRenderer *pRend, HRESULT *phr, LPCWSTR name):CRendererInputPin(pRend, phr, name){
	}
	STDMETHODIMP NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
};
//-----------------------------------------------------------------------------
//	WXAMSockSinkImp
static const GUID CLSID_AMSockSink = { 0x9AEC363B, 0x8AC6, 0x4A6D, { 0xB7, 0x2D, 0x89, 0x94, 0xC5, 0x37, 0xBA, 0xE5 } };
class WXAMSockSinkImp: public CBaseRenderer{
private:
	unsigned int count;
	CMediaType  mediaType;							//	���͂̃��f�B�A�^�C�v
	const GUID* type;								//	�󂯓���郁�f�B�A�^�C�v
	const GUID* subtype;							//	�󂯓���郁�f�B�A�T�u�^�C�v
	UTRef<WXAMSockSinkSender> sender;				//	���M�I�u�W�F�N�g
	
	friend class WXAMSockSink;

public:
	WXAMSockSinkImp(WXAMSockSinkSender* s=NULL);
    virtual ~WXAMSockSinkImp();
	//	CBaseVideoRenderer�����o�̃I�[�o�[���C�h
	HRESULT CheckMediaType(const CMediaType* mt);	// ���͂����e�ł���t�H�[�}�b�g����������
	HRESULT SetMediaType(const CMediaType* mt);		// �t�H�[�}�b�g���w�肷��
	HRESULT DoRenderSample(IMediaSample* ms);		// �r�f�I�̐V�����摜�������Ă���
	CBasePin* GetPin(int n);
	HRESULT NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
};

HRESULT WXAMSockSinkInputPin::NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate){
	WXAMSockSinkImp* render = (WXAMSockSinkImp*)m_pRenderer;
	return render->NewSegment(tStart, tStop, dRate);
}

WXAMSockSinkImp::WXAMSockSinkImp(WXAMSockSinkSender* s):
CBaseRenderer(CLSID_AMSockSink, NAME("Callback Renderer"), NULL, NULL){
	type = NULL;
	subtype = NULL;
	sender = s;
	count = 0;
	sender->lastTime = 0;
}
WXAMSockSinkImp::~WXAMSockSinkImp(){
}
CBasePin* WXAMSockSinkImp::GetPin(int n){
	CAutoLock cObjectCreationLock(&m_ObjectCreationLock);
	// Should only ever be called with zero
	ASSERT(n == 0);
	if (n != 0) {
		return NULL;
	}
	// Create the input pin if not already done so
	if (m_pInputPin == NULL) {
		HRESULT hr = NOERROR;
		m_pInputPin = new WXAMSockSinkInputPin(this,&hr,L"In");
		if (NULL == m_pInputPin) {
			return NULL;
		}
		if (FAILED(hr)) {
			delete m_pInputPin;
			m_pInputPin = NULL;
			return NULL;
		}
	}
	return m_pInputPin;
}

HRESULT WXAMSockSinkImp::CheckMediaType(const CMediaType* mt){
	if ((!type || *type==*mt->Type()) && (!subtype || *subtype==*mt->Subtype())){
		return NOERROR;
	}
	return E_FAIL;
}
HRESULT WXAMSockSinkImp::SetMediaType(const CMediaType* mt){
	mediaType = *mt;
	return NOERROR;
}
HRESULT WXAMSockSinkImp::NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate){
	//	packetID �𑗂�
	int pid = PID_NEWSEGMENT;
	sender->Send(&pid, sizeof(pid));
	//	pin�̃A�h���X�����ʗp�ɑ���
	sender->Send(&m_pInputPin, sizeof(m_pInputPin));
	WXAMSockPacketNewSegment data;
	data.tStart = tStart;
	data.tStop = tStop;
	data.dRate = dRate;
	sender->Send(&data, sizeof(data));
	return S_OK;
}
HRESULT WXAMSockSinkImp::DoRenderSample(IMediaSample* sample){
	IMediaSample2* ms2 = NULL;
	sample->QueryInterface(IID_IMediaSample2, (void**)&ms2);
	AM_SAMPLE2_PROPERTIES prop;
	memset(&prop, 0, sizeof(prop));
	HRESULT hr =ms2->GetProperties(sizeof(AM_SAMPLE2_PROPERTIES), (unsigned char*)&prop);
	assert(hr == S_OK);

	//	�����_�����O�^�C���̐ݒ�
/*
	if (!(prop.dwSampleFlags&AM_SAMPLE_TIMEVALID)){
		prop.dwSampleFlags |= AM_SAMPLE_TIMEVALID;
		prop.tStart = sender->lastTime;
	}
	if (!(prop.dwSampleFlags&AM_SAMPLE_STOPVALID)){
		prop.dwSampleFlags |= AM_SAMPLE_STOPVALID;
		prop.tStop = sender->lastTime+10;
	}
*/
	//	�p�P�b�gID�̑��M
	int pid = PID_MEDIASAMPLE;
	sender->Send(&pid, sizeof(pid));
	//	���M�s�����ʂ̂��߁Cpin�̃A�h���X��ID�Ƃ��đ���D
	REFERENCE_TIME sendTime = prop.tStop;
	sender->Send(&m_pInputPin, sizeof(m_pInputPin));
	//	100���1��̓��f�B�A�^�C�v�𑗐M�������̂ŁC�����I�ɐݒ�
	if (!prop.pMediaType && count%100 == 0){
		prop.pMediaType = &mediaType;
	}
	count ++;
	//	���f�B�A�T���v���̃v���p�e�B�̑��M�D
	sender->Send(&prop, sizeof(prop));
	if (prop.pMediaType){	//	���f�B�A�^�C�v������΂�������M
		sender->Send(prop.pMediaType, sizeof(AM_MEDIA_TYPE));
		if(prop.pMediaType->formattype == FORMAT_MPEGVideo){
			MPEG1VIDEOINFO* fmt = (MPEG1VIDEOINFO*)prop.pMediaType->pbFormat;
			int fmtLen = sizeof(MPEG1VIDEOINFO) + fmt->cbSequenceHeader - 1;
			sender->Send(fmt, fmtLen);
		}else if(prop.pMediaType->formattype == FORMAT_MPEG2Video){
			MPEG2VIDEOINFO* fmt = (MPEG2VIDEOINFO*)prop.pMediaType->pbFormat;
			int fmtLen = sizeof(MPEG2VIDEOINFO) + fmt->cbSequenceHeader - 1;
			sender->Send(fmt, fmtLen);
		}else if(prop.pMediaType->formattype == FORMAT_VideoInfo){
			VIDEOINFOHEADER* fmt = (VIDEOINFOHEADER*)prop.pMediaType->pbFormat;
			int fmtLen = sizeof(VIDEOINFOHEADER);
			sender->Send(fmt, fmtLen);
		}else if(prop.pMediaType->formattype == FORMAT_VideoInfo2){
			VIDEOINFOHEADER2* fmt = (VIDEOINFOHEADER2*)prop.pMediaType->pbFormat;
			int fmtLen = sizeof(VIDEOINFOHEADER2);
			sender->Send(fmt, fmtLen);
		}else if(prop.pMediaType->formattype == FORMAT_WaveFormatEx){
			WAVEFORMATEX* fmt = (WAVEFORMATEX*)prop.pMediaType->pbFormat;
			int fmtLen = sizeof(WAVEFORMATEX) + fmt->cbSize;
			sender->Send(fmt, fmtLen);
		}
	}
	//	�f�[�^�̑��M
	if (!(prop.dwSampleFlags&AM_SAMPLE_STOPVALID)){
		prop.tStop = 0;
	}
	if (!(prop.dwSampleFlags&AM_SAMPLE_TIMEVALID)){
		prop.tStop = 0;
		prop.tStart = 0;
	}
	if (prop.cbBuffer>0){
		sender->Send(prop.pbBuffer, prop.cbBuffer);
		std::cout << "Send " << (int)prop.tStart << ":";
		std::cout << (int)prop.tStop;
		std::cout << " size:" << sender->cur-4;
		HRESULT hr = sample->IsSyncPoint();
		std::cout << (hr==S_OK ? " SYNC" : " CONT") << std::endl;
	}
	ms2->Release();
	sender->Flush();
	Sleep(20);
	return NOERROR;
}

//-----------------------------------------------------------------------------
//	WXAMSockSink
//
WXAMSockSink::WXAMSockSink(){
	wcscpy(name, L"WXAMSockSink");
	imp = new WXAMSockSinkImp(new WXAMSockSinkSender);
	imp->NonDelegatingQueryInterface(IID_IBaseFilter, (void**)&Intf());
};
WXAMSockSink::WXAMSockSink(WXAMSockSink& ref){
	imp->sender = ref.imp->sender;
}
WXAMSockSink::~WXAMSockSink(){
	imp = NULL;
}
void WXAMSockSink::MediaType(const GUID& t){
	if (imp) imp->type = &t;
}
void WXAMSockSink::MediaSubtype(const GUID& t){
	if (imp) imp->subtype = &t;
}

}	//	namespace Spr;
