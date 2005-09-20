#ifndef LVRM_DDRAW_WXDDVIDROPORT_H
#define LVRM_DDRAW_WXDDVIDROPORT_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WXDDraw.h"
#include <dvp.h>

namespace LVRM {

class WXDDVideoPorts;
class WXDDVideoPort:public WXInterface<IDirectDrawVideoPort, &IID_IDirectDrawVideoPort>{
public:
	class InPixelFormat:public DDPIXELFORMAT{
	public:
		std::vector<DDPIXELFORMAT> outFormats;
		InPixelFormat(const DDPIXELFORMAT& f):DDPIXELFORMAT(f){}
	};
protected:
	WXDDVideoPorts* ports;
	DDVIDEOPORTCAPS caps;
	int connectID;
public:
	std::vector<InPixelFormat> formats;
	std::vector<DDVIDEOPORTCONNECT> connects;
	WXDDVideoPort();
	bool Create(int connectID);
	void EnumConnect();
	bool SetSurface(WXDDSurface& surf);
	bool Start();
	friend class WXDDVideoPorts;
};

class WXDDVideoPorts:public WXInterface<IDDVideoPortContainer, &IID_IDDVideoPortContainer>
{
public:
	typedef std::vector<WXDDVideoPort> Ports;
	Ports ports;
	bool Create();
protected:
	static HRESULT WINAPI EnumVideoPortsCallback(DDVIDEOPORTCAPS* caps, void* context);
	friend class WXDDVideoPort;
};

}	//	namespace LVRM

#endif
