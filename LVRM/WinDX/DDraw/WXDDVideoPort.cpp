#include "WXDDVideoPort.h"

namespace LVRM {

#if DIRECTDRAW_VERSION >= 0x700

HRESULT WINAPI WXDDVideoPorts::EnumVideoPortsCallback(DDVIDEOPORTCAPS* caps, void* context){
	WXDDVideoPorts* ddvp = (WXDDVideoPorts*)context;
	WXDDVideoPort newvp;
	newvp.caps = *caps;
	newvp.ports = ddvp;
	ddvp->ports.push_back(newvp);
	return DDENUMRET_OK;
}
bool WXDDVideoPorts::Create(){
	WXDDraws::GetLastCreate()->Primary().Create();

	WXDDraws::GetLastCreate()->Primary().pDDraw7->QueryInterface(IID_IDDVideoPortContainer, (void**)&Intf() );
	if (Intf()){
		Intf()->EnumVideoPorts(0, NULL, this, EnumVideoPortsCallback);
		return true;
	}
	return false;
}

WXDDVideoPort::WXDDVideoPort(){
	ports = NULL;
	connectID = -1;
}
void WXDDVideoPort::EnumConnect(){
	if (!connects.size()){
		DWORD nConnect;
		DDVIDEOPORTCONNECT connect[256];
		nConnect = sizeof(connect) / sizeof(connect[0]);
		(*ports)->GetVideoPortConnectInfo(caps.dwVideoPortID, &nConnect, connect);
		for(DWORD i=0; i<nConnect; i++){
			connects.push_back(connect[i]);
		}
	}
}
bool WXDDVideoPort::Create(int cID){
	connectID = cID;
	EnumConnect();
	DDVIDEOPORTDESC	desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);
//	desc.dwFieldWidth = caps.dwMaxWidth;
//	desc.dwVBIWidth = caps.dwMaxVBIWidth;
//	desc.dwFieldHeight = caps.dwMaxHeight;
	desc.dwFieldWidth = 320;
	desc.dwFieldHeight = 240;
	desc.dwMicrosecondsPerField = 1000 / 60;
	desc.dwMaxPixelsPerSecond = 320 * 240 * 30;
	desc.dwVideoPortID = caps.dwVideoPortID;
	if (connectID < 0) connectID = 0;
	if (connectID > (int)connects.size()-1) connectID = connects.size()-1;
	desc.VideoPortType = connects[connectID];
	(*ports)->CreateVideoPort(NULL, &desc, &Intf(), NULL);
	if (Intf()){
		DDPIXELFORMAT fmt[256];
		DWORD nFmt = sizeof(fmt) / sizeof(fmt[0]);
		Intf()->GetInputFormats(&nFmt, fmt, DDVPFORMAT_VIDEO);
		for(DWORD i=0; i<nFmt; i++) formats.push_back(fmt[i]);

		nFmt = sizeof(fmt) / sizeof(fmt[0]);

		for(DWORD i=0; i<formats.size(); i++){
			Intf()->GetOutputFormats(&formats[i], &nFmt, fmt, DDVPFORMAT_VIDEO);
			for(DWORD o=0; o<nFmt; o++) formats[i].outFormats.push_back(fmt[o]);
		}
		return true;
	}
	return false;
}
bool WXDDVideoPort::SetSurface(WXDDSurface& surf){
	HRESULT hr = Intf()->SetTargetSurface(surf.pDDS, DDVPTARGET_VIDEO);
	return hr == NOERROR;
}
bool WXDDVideoPort::Start(){
	DDVIDEOPORTINFO info;
	ZeroMemory(&info, sizeof(info));
	info.dwSize = sizeof(info);
	info.lpddpfInputFormat = &formats[0];
	info.dwOriginX = 0;
	info.dwOriginY = 0;
//	info.dwVPFlags = DDVP_AUTOFLIP;
	HRESULT hr = Intf()->StartVideo(&info);
	return hr == NOERROR;
}

#endif
}	//	namespace LVRM

