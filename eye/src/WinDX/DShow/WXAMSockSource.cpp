#include "WXAMSockSource.h"
#include "WXAMSockDef.h"
#include "WXAMBaseClasses.h"
#include <WinBasis/WBSockStr.h>
#include <DVDMedia.h>

namespace Spr {;
using namespace Spr;

class CRecvBuf:public UTRefCount{
public:
	BYTE* buf;
	CRecvBuf():buf(NULL){}
	CRecvBuf(BYTE* b):buf(b){}
	~CRecvBuf(){ delete buf; }
};
//-----------------------------------------------------------------------------
//	WXAMSockSourceStream

/*
このピンは以下をサポートしなければならないらしい．
{56A868BF-0AD4-11CE-B03A-0020AF0BA770}
{IID_IMediaSeeking}
{IID_IMediaPosition}

*/
class WXAMSockSourceStream:public CSourceStream{
public:
	class CMediaSeeking:public IMediaSeeking{
	public:

	};
	
	volatile bool bShouldRender;
	int id;
	CMediaType mediaType;
	volatile WXAMSockPacketNewSegment* segment;
	AM_SAMPLE2_PROPERTIES* prop;
	UTRef<CRecvBuf> recvBuf;
	UTRef<CRecvBuf> segBuf;
	WXAMSockSourceStream(CSource* filter, HRESULT& hr):CSourceStream("WXAMSockSourceStream", &hr, filter, L"WXAMSockSourceStream"){
		id = 0;
		prop = NULL;
		segment = NULL;
		bShouldRender = false;
	}
	HRESULT GetMediaType(CMediaType *pMediaType){
		*pMediaType = mediaType;
		return S_OK;
	}
	HRESULT CheckMediaType(CMediaType *pMediaType){
		if (*pMediaType == mediaType) return S_OK;
		return E_FAIL;
	}
	HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProperties){
		if (!prop) return E_FAIL;
	    ASSERT(pAlloc);
	    ASSERT(pProperties);
		HRESULT hr = NOERROR;

		pProperties->cBuffers = 1;
		pProperties->cbBuffer = prop->cbBuffer + 256;

		ASSERT(pProperties->cbBuffer);

		// Ask the allocator to reserve us some sample memory, NOTE the function
		// can succeed (that is return NOERROR) but still not have allocated the
		// memory that we requested, so we must check we got whatever we wanted

		ALLOCATOR_PROPERTIES Actual;
		hr = pAlloc->SetProperties(pProperties,&Actual);
		if (FAILED(hr)) {
			return hr;
		}

		// Is this allocator unsuitable
		
		if (Actual.cbBuffer < pProperties->cbBuffer) {
			return E_FAIL;
		}

		// Make sure that we have only 1 buffer (we erase the ball in the
		// old buffer to save having to zero a 200k+ buffer every time
		// we draw a frame)
		
		ASSERT( Actual.cBuffers == 1 );
		return NOERROR;
	}
	HRESULT FillBuffer(IMediaSample *sample);
	STDMETHODIMP Notify(IBaseFilter * pSender, Quality q){
		// Adjust the repeat rate.
/*		if (q.Proportion<=0) {
			repeatTime = 1000;        // We don't go slower than 1 per second
		} else {
			repeatTime = repeatTime*1000/q.Proportion;
			if (repeatTime > 1000) {
				repeatTime = 1000;    // We don't go slower than 1 per second
			} else if (repeatTime < 1) {
				repeatTime = 1;      // We don't go faster than 1000/sec
			}
		}
		// skip forwards
		if (q.Late > 0) {
			sampleTime += q.Late;
		}
*/		return NOERROR;
	} // Notify
};
//-----------------------------------------------------------------------------
//	WXAMSockSourceImp
static const GUID CLSID_WXAMSockSourceImp = { 0x8B88F027, 0x8EB3, 0x4E78, { 0xB1, 0x25, 0xB7, 0x83, 0xA2, 0x78, 0x9F, 0x27 } };
class WXAMSockSourceImp: public CSource{
	friend class WXAMSockSource;
protected:
	WBSockStream sock;
	typedef std::vector<WXAMSockSourceStream*> TPins;
	WXAMSockSourceStream* pin;
	TPins pins;
public:
	volatile bool bQuit;
	
	WXAMSockSourceImp():CSource(NAME("Callback Renderer"), NULL, CLSID_WXAMSockSourceImp){
		bQuit = false;
	}
	virtual ~WXAMSockSourceImp(){}
	void ReceiveSocket(){
		int len=0;
		int cur=0;
		UTRef<CRecvBuf> recv;
		while(!bQuit){
			if (!sock.IsConnected()){
				sock.Listen(11000);
				sock.Accept();
			}
			if (!sock.IsConnected()){
				Sleep(1);
				continue;
			}
			int avail = sock.Avail();
			if (!len){
				if (avail >= 4){
					//	パケット長の取得
					sock.read((char*)&len, 4);
					avail -= 4;
					if (len > 0) recv = new CRecvBuf(new BYTE[len]);
				}
			}
			if (len <= 0){
				Sleep(1);
				continue;
			}
			if (avail > 0){
				int need = len - cur;
				int readLen = std::min(need, avail);
				sock.read((char*)recv->buf+cur, readLen);
				cur += readLen;
				if (cur < len){
					Sleep(10);
					continue;
				}
			}
			//	ここまでで，1パケット全体がバッファに入る．
//			std::cout << "WXAMSockSourceImp receives " << len << " bytes." << std::endl;
			BYTE* ptr = recv->buf;
			BYTE* end = recv->buf+len;
			cur = 0;
			len = 0;
			
			while(end > ptr){
				//	packetIDの読み取り
				if (end < ptr+sizeof(DWORD)) break;
				DWORD& packetID = *(DWORD*)ptr;
				ptr += sizeof(packetID);
				//	パケットの読み取り
				if (packetID == PID_NEWSEGMENT){
					ptr += ReceiveNewSegment(ptr, end, recv);
				}else if (packetID == PID_MEDIASAMPLE){
					ptr += ReceiveMediaSample(ptr, end, recv);
				}
			}
		}
	}
	int ReceivePinID(BYTE* start, BYTE* end){
		//	pinIDの読み取り
		BYTE* ptr = start;
		if (end < ptr+sizeof(DWORD)) return 0;
		DWORD& pinID = *(DWORD*)ptr;
		pin = NULL;
		for(TPins::iterator it = pins.begin(); it != pins.end(); ++it){
			if (pinID == (*it)->id){
				pin = *it;
				break;
			}
		}
		if (!pin){
			HRESULT hr;
			pins.push_back(new WXAMSockSourceStream(this, hr));
			pin = pins.back();
			pin->id = pinID;
		}
		return sizeof(pinID);
	}
	int ReceiveNewSegment(BYTE* start, BYTE* end, CRecvBuf* recv){
		BYTE* ptr = start;
		ptr += ReceivePinID(ptr, end);
		if (ptr+sizeof(WXAMSockPacketNewSegment) < end){
			WXAMSockPacketNewSegment* ns = (WXAMSockPacketNewSegment*)ptr;
			{
				CAutoLock lock(pStateLock());
				pin->segment = ns;
			}
			pin->segBuf = recv;
			ptr += sizeof(WXAMSockPacketNewSegment);
		}
		return ptr - start;
	}
	int ReceiveMediaSample(BYTE* start, BYTE* end, CRecvBuf* recv){
		BYTE* ptr = start;
		ptr += ReceivePinID(ptr, end);
		//	メディアサンプルのプロパティの読み取り
		if (end <= ptr+sizeof(AM_SAMPLE2_PROPERTIES)) return 0;
		AM_SAMPLE2_PROPERTIES& prop = *(AM_SAMPLE2_PROPERTIES*)ptr;
		ptr += sizeof(prop);
		if (prop.pMediaType){
			if(end <= ptr+sizeof(AM_MEDIA_TYPE)) return 0;
			prop.pMediaType = (AM_MEDIA_TYPE*)ptr;
			ptr += sizeof(AM_MEDIA_TYPE);
			if(prop.pMediaType->formattype == FORMAT_MPEGVideo){
				MPEG1VIDEOINFO& fmt = *(MPEG1VIDEOINFO*)ptr;
				prop.pMediaType->pbFormat = (BYTE*)&fmt;
				int fmtLen = sizeof(fmt) + fmt.cbSequenceHeader - 1;
				ptr += fmtLen;
			}else if(prop.pMediaType->formattype == FORMAT_MPEG2Video){
				MPEG2VIDEOINFO& fmt = *(MPEG2VIDEOINFO*)ptr;
				prop.pMediaType->pbFormat = (BYTE*)&fmt;
				int fmtLen = sizeof(fmt) + fmt.cbSequenceHeader - 1;
				ptr += fmtLen;
			}else if(prop.pMediaType->formattype == FORMAT_VideoInfo){
				VIDEOINFOHEADER& fmt = *(VIDEOINFOHEADER*)ptr;
				prop.pMediaType->pbFormat = (BYTE*)&fmt;
				ptr += sizeof(fmt);
			}else if(prop.pMediaType->formattype == FORMAT_VideoInfo2){
				VIDEOINFOHEADER2& fmt = *(VIDEOINFOHEADER2*)ptr;
				prop.pMediaType->pbFormat = (BYTE*)&fmt;
				ptr += sizeof(fmt);
			}else if(prop.pMediaType->formattype == FORMAT_WaveFormatEx){
				WAVEFORMATEX& fmt = *(WAVEFORMATEX*)ptr;
				prop.pMediaType->pbFormat = (BYTE*)&fmt;
				ptr += sizeof(fmt) + fmt.cbSize;
			}
			pin->mediaType = *prop.pMediaType;
		}
		//	バッファの読み取り
		if (prop.cbBuffer>0){
			if (end < ptr+prop.cbBuffer){
				std::cout << "Error:" << "The length of the buffer is not enough." << std::endl;
				return 0;
			}
			prop.pbBuffer = ptr;
			ptr += prop.cbBuffer;
			if (pin && pin->mediaType.IsValid()){
				int count = 0;
				while(!bQuit){
					{
						CAutoLock lock(pStateLock());
						if (!pin->bShouldRender){
							pin->prop = &prop;
							pin->recvBuf = recv;
							pin->bShouldRender = true;
							break;
						}
					}
					Sleep(10);
					count ++;
					if (count > 300) bQuit = true;
				}
			}
		}
		return ptr - start;
	}
	static DWORD WINAPI StartReceiveSocketEntry(void* a){
		WXAMSockSourceImp* imp = (WXAMSockSourceImp*)a;
		imp->ReceiveSocket();
		return 0;
	}
	bool StartReceiveSocket(){
		DWORD tid = 0;
		HANDLE hThread = CreateThread(NULL, 0, StartReceiveSocketEntry, this, CREATE_SUSPENDED, &tid);
		if (hThread){
			ResumeThread(hThread);
			return true;
		}
		return false;
	}
};
	
HRESULT WXAMSockSourceStream::FillBuffer(IMediaSample *sample){
	{	
		CAutoLock lock(m_pFilter->pStateLock());
		if (segment){
			DeliverNewSegment(segment->tStart, segment->tStop, segment->dRate);
			segment = NULL;
		}
	}
	if (!prop){
		Sleep(30);
		return S_OK;
	}
	while(1){
		{
			CAutoLock lock(m_pFilter->pStateLock());
			if (((WXAMSockSourceImp*)m_pFilter)->bQuit) return E_FAIL;
			if (bShouldRender){
				IMediaSample2* ms2 = NULL;
				sample->QueryInterface(IID_IMediaSample2, (void**)&ms2);
				if(ms2){
					BYTE* ptr = NULL;
					ms2->GetPointer(&ptr);
					long len = ms2->GetSize();
					assert(len >= prop->cbBuffer);
					memcpy(ptr, prop->pbBuffer, prop->cbBuffer);
					prop->pbBuffer = ptr;
					HRESULT hr = ms2->SetProperties(sizeof(AM_SAMPLE2_PROPERTIES)-8, (BYTE*)prop);
					memset(prop, 0, sizeof(*prop));
					hr = ms2->GetProperties(sizeof(AM_SAMPLE2_PROPERTIES), (BYTE*)prop);
					hr = ms2->IsSyncPoint();
					std::cout << (int)prop->tStart << ":" << (int)prop->tStop;
					std::cout << " " << prop->cbBuffer;
					std::cout << (hr==S_OK ? " SYNC" : " CONT") << std::endl;
				}
				ms2->Release();
				bShouldRender = false;
				return S_OK;
			}
		}
		Sleep(10);
	}
}
/*
		while(!bShouldRender){
			Sleep(10);
			count ++;
			if (count > 100) return S_OK;
		}

		CAutoLock lock(m_pFilter->pStateLock());
		IMediaSample2* ms2 = NULL;
		sample->QueryInterface(IID_IMediaSample2, (void**)&ms2);
		if(ms2){
			BYTE* ptr = NULL;
			ms2->GetPointer(&ptr);
			long len = ms2->GetSize();
			assert(len >= prop->cbBuffer);
			memcpy(ptr, prop->pbBuffer, prop->cbBuffer);
			prop->pbBuffer = ptr;
			HRESULT hr = ms2->SetProperties(sizeof(AM_SAMPLE2_PROPERTIES)-8, (BYTE*)prop);
			memset(prop, 0, sizeof(*prop));
			hr = ms2->GetProperties(sizeof(AM_SAMPLE2_PROPERTIES), (BYTE*)prop);
			hr = ms2->IsSyncPoint();
			std::cout << (int)prop->tStart << ":" << (int)prop->tStop;
			std::cout << " " << prop->cbBuffer;
			std::cout << (hr==S_OK ? " SYNC" : " CONT") << std::endl;
		}
		ms2->Release();
		bShouldRender = false;
		return S_OK;
	}	*/
//-----------------------------------------------------------------------------
//	WXAMSockSource
//
WXAMSockSource::WXAMSockSource(){
	wcscpy(name, L"WXAMSockSource");
	imp = new WXAMSockSourceImp;
	imp->NonDelegatingQueryInterface(IID_IBaseFilter, (void**)&Intf());
};
WXAMSockSource::~WXAMSockSource(){
	imp = NULL;
}
void WXAMSockSource::StartReceive(){
	imp->StartReceiveSocket();
}
int WXAMSockSource::NumStream(){
	int rv = 0;
	for(WXAMSockSourceImp::TPins::iterator it = imp->pins.begin(); it != imp->pins.end(); ++it){
		if ((*it)->mediaType.IsValid()){
			rv ++;
		}
	}
	return rv;
}

}	//	namespace Spr;
