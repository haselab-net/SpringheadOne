#ifndef LVRM_WXD3D_WXD3Dev_H
#define LVRM_WXD3D_WXD3Dev_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "../DDraw/WXDDraw.h"
#include "../DDraw/WXDDFSurf.h"
#include "WXD3Affine.h"
#include <d3d.h>

namespace LVRM {

//----------------------------------------------------------------------------
//	WXD3Driver
///	3Dレンダリングドライバに対応するクラス
class WXD3Driver{
public:
	///	3DレンダリングドライバのGUID
	GUID guid;
	///	3Dレンダリングドライバの説明
	std::string about; 
	///	3Dレンダリングドライバの名前
	std::string name; 
	///	HALかどうか
	bool bHAL;
	///	desc7を使用するかどうか
	bool bDesc7;
private:
#if DIRECTDRAW_VERSION >= 0x700
	///	作成できるサーフェイスについての記述(DirectX7以降)
	D3DDEVICEDESC7 desc7;
#endif
	///	作成できるサーフェイスについての記述
	D3DDEVICEDESC desc;

public:
	WXD3Driver();
	int ZBufferDepth(int max=16);
	/**	指定された色数のサーフェイスにレンダリング可能か
		@param	bitPerPixel	1ピクセルのビット数.
		@result	可能なら true.		*/
	bool CheckRendDepth(int bitPerPixel);
	void SetDesc(D3DDEVICEDESC& d);
	D3DDEVICEDESC* GetDesc(){ return bDesc7 ? NULL : &desc; }
#if DIRECTDRAW_VERSION >= 0x700
	void SetDesc(D3DDEVICEDESC7& d7);
	D3DDEVICEDESC7* GetDesc7(){ return bDesc7 ? &desc7 : NULL; }
#endif
protected:
	DWORD ZBufferDepthMask();
};
//----------------------------------------------------------------------------
//	WXD3Drivers

///	3Dレンダリングドライバの配列
class WXD3Drivers: public std::vector<WXD3Driver>{
protected:
	int cur;
	bool Select(int rendDepth, bool bHALOK=true);
	bool Select();
public:
	///	使いたい3Dレンダラを指定
	int prefer;
	WXD3Drivers();
	int Cur(){return cur;}
	///	現在使用中の3Dレンダラ
	WXD3Driver& CurDriver();
	///	使用する3Dレンダラを id にする。(できないこともある)
	void Set(int id);
#if DIRECTDRAW_VERSION >= 0x700
	static HRESULT PASCAL EnumDevicesCallback(char* desc, char* name, D3DDEVICEDESC7* devDesc, void* pContext);
#endif
	static HRESULT PASCAL EnumDevicesCallback(GUID* pGuid, char* a, char* n, D3DDEVICEDESC* pHAL, D3DDEVICEDESC* pHEL, void* pContext);
	friend class WXD3Device;
	friend class WXD3DPDevice;
};

//----------------------------------------------------------------------------
//	WXD3Device
//
///	3D描画可能なサーフェイス
class WXD3Device:public WXDDFlipSurface{
public:
#if DIRECTDRAW_VERSION >= 0x0500
	typedef IDirect3DDevice2 intf_type;
#else
	typedef IDirect3DDevice intf_type;
#endif
	///	使用する3Dレンダラ
	WXD3Drivers drivers;
	///	デバイスのCOMインタフェイス
	intf_type* pD3DDev;
	intf_type* operator ->() const {return pD3DDev;}
	intf_type* operator ->() {
		if (!pD3DDev) Create();
		return pD3DDev;
	}
	///
	WXD3Device();
	~WXD3Device();
	///	描画先ディスプレイカードの設定
	virtual void DDraw(WXDDraw& dd);
	///	使用可能な3Dレンダラを列挙
	virtual bool EnumDrivers();
	///	サーフェイスの作成
	virtual bool Create(){return Create(true);}
	///	サーフェイスの開放
	virtual void Release(){Release(true);}
	///	サーフェイスが失われていれば復元する
	virtual bool Restore();

protected:
	virtual bool Create(bool bCallCallback, int nBackBuffer);
	virtual bool Create(bool bCallCallback){ return Create(bCallCallback, 1); }
	virtual void Release(bool bCallCallback);
	class CZSurface:public WXDDSurface {
		virtual bool Create(){ return Create(true); }
		virtual bool Create(bool bCallCallback){ return Create(16, true, true); }
		virtual bool Create(DWORD depth, bool bHardWare, bool bCallCallback=true);
		friend WXD3Device;
	}zBuffer;
	friend class WXRM;
	friend class WXD3IM;
};
}	//	namespace LVRM
#endif
