#ifndef LVRM_WXD3D_WXD3DPDev_H
#define LVRM_WXD3D_WXD3DPDev_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "WXD3Dev.h"

namespace LVRM {


#if DIRECTDRAW_VERSION >= 0x700
//----------------------------------------------------------------------------
//	WXD3Light
//
///	WXD3D7用の光源
class WXD3Light:public D3DLIGHT7{
public:
	WXD3Light();
};

//----------------------------------------------------------------------------
//	WXD3Viewport
//
///	WXD3D7用のViewport(視点と描画先)
class WXD3Viewport:public D3DVIEWPORT7{
	WXD3Vec3 begin;	//	ビューポートの左上手前の点, 0..1で指定
	WXD3Vec3 end;	//	ビューポートの右下奥の点, 0..1で指定
	WXD3Vec3 screen;//	カメラから見たビューポートの中心の位置
	D3DVALUE front;	//	手前のクリッピング平面とカメラの距離
	D3DVALUE back;	//	奥のクリッピング平面とカメラの距離
public:
	WXD3Viewport(WXD3Vec3 be, WXD3Vec3 e, WXD3Vec3 s, D3DVALUE f=1.0f, D3DVALUE bk=10000.0f);
	WXD3Viewport(WXD3Vec3 s, D3DVALUE f=1.0f, D3DVALUE bk=10000.0f);
	WXD3Viewport(D3DVALUE f=1.0f, D3DVALUE bk=10000.0f);
	WXD3Viewport(const RECT& rc, D3DVALUE minZ=0.0f, D3DVALUE maxZ=1.0f);
	WXD3Viewport(const D3DVIEWPORT7& vp);
	void TargetSize(SIZE sz);
	WXD3Affine Projection();
};

//----------------------------------------------------------------------------
//	WXD3DPDevice
//
///	3D描画可能なサーフェイス
class WXD3DPDevice:public WXDDFlipSurface{
protected:
	class CZSurface:public WXDDSurface {
		bool Create(){ return Create(true); }
		bool Create(bool bCallCallback){ return Create(16, true, bCallCallback); }
		bool Create(DWORD depth, bool bHardWare, bool bCallCallback=true);
		friend WXD3DPDevice;
	} zBuffer;
public:
	typedef WXDDFlipSurface baseclass;
	typedef IDirect3DDevice7 intf_type;
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
	WXD3DPDevice();
	~WXD3DPDevice();
	///	描画先ディスプレイカードの設定
	virtual void DDraw(WXDDraw& dd);
	///	使用可能な3Dレンダラを列挙
	virtual bool EnumDrivers();
	///	サーフェイスの作成
	bool Create(){return Create(true);}
	///	サーフェイスの開放
	void Release(){Release(true);}
	///	サーフェイスが失われていれば復元する
	virtual bool Restore();
	WXD3Viewport viewport;
	void Viewport(const WXD3Viewport& vp);
	const WXD3Viewport& Viewport(){ return viewport; }

protected:
	virtual bool Create(bool bCallCallback){ return Create(bCallCallback, 1); }
	virtual bool Create(bool bCallCallback, int nBackBuffer);
	virtual void Release(bool bCallCallback);
	bool ClearImp(POINT dest, SIZE sz, DWORD color, DWORD flags);
	friend class WXRM;
};
#endif

}	//	namespace LVRM
#endif
