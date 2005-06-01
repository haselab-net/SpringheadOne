#include <WinDX/WinDX.h>
#pragma hdrstop
namespace Spr{;

IUnknown* WXD3D::Create(){
#if SPR_DXVER == 8
	return Direct3DCreate8(D3D_SDK_VERSION);
#elif SPR_DXVER == 9
	return Direct3DCreate9(D3D_SDK_VERSION);
#endif
}
WXD3D::WXD3D(){
	Prepare();
}


WXD3DPresentParameters::WXD3DPresentParameters(){
	memset(this, 0, sizeof(D3DPRESENT_PARAMETERS));
	BackBufferFormat = D3DFMT_X8R8G8B8;
	WXD3D d3d;
	D3DDISPLAYMODE d3ddm;
	if (WXCHECK(d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))){
		BackBufferFormat = d3ddm.Format;
	}
	BackBufferCount = 1;
	MultiSampleType = D3DMULTISAMPLE_NONE;
	SwapEffect = D3DSWAPEFFECT_DISCARD;
	Windowed = true;
	EnableAutoDepthStencil = true;
	AutoDepthStencilFormat = D3DFMT_D16_LOCKABLE;
}
void WXD3DPresentParameters::Window(int adapterID, int width, int height){
	WXD3D d3d;
	D3DDISPLAYMODE disp;
	d3d->GetAdapterDisplayMode(adapterID, &disp);
	BackBufferFormat = disp.Format;
	BackBufferWidth = width;
	BackBufferHeight = height;
	BackBufferCount = 1;
	Windowed = true;
	SwapEffect = D3DSWAPEFFECT_COPY;
}
void WXD3DPresentParameters::Fullscreen(int adapterID){
	WXD3D d3d;
	D3DDISPLAYMODE disp;
	d3d->GetAdapterDisplayMode(adapterID, &disp);
	BackBufferFormat = disp.Format;
	BackBufferWidth = disp.Width;
	BackBufferHeight = disp.Height;
	FullScreen_RefreshRateInHz = disp.RefreshRate;
	Windowed = false;
	SwapEffect = D3DSWAPEFFECT_FLIP;
	BackBufferCount = 2;
}
bool WXD3DDevice::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil){
	if (!bInit) return false;
	return WXCHECK( Intf()->Clear(Count, pRects, Flags, Color, Z, Stencil) );
}
bool WXD3DDevice::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion){
	if (!bInit) return false;
	if (IsFullscreen()){
		return WXCHECK( Intf()->Present(NULL, NULL, NULL, NULL) );
	}else{
		if (!hDestWindowOverride) hDestWindowOverride = presentParameters.hDeviceWindow;
		RECT rcSrc, rcDest;
		if (!pSourceRect){
			pSourceRect = &rcSrc;
			D3DVIEWPORT_SPR vp;
			Intf()->GetViewport(&vp);
			rcSrc.left = vp.X;				rcSrc.top = vp.Y;
			rcSrc.right = vp.X+vp.Width;	rcSrc.bottom = vp.Y+vp.Height;
		}
		if (!pDestRect){
			pDestRect = &rcDest;
			GetClientRect(hDestWindowOverride, &rcDest);
		}
		return WXCHECK( Intf()->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion) );
	}
}
bool WXD3DDevice::SetViewport(HWND hw){
	if (!IsFullscreen()){
		RECT rc;
		GetClientRect(hw, &rc);
		if (SetViewport(Vec2f(float(rc.right-rc.left), float(rc.bottom-rc.top)), Vec2f(float(rc.left), float(rc.top)))){
			presentParameters.hDeviceWindow = hw;
			winMan.HWnd(hw);
			return true;
		}
	}
	return false;
}
bool WXD3DDevice::SetViewport(Vec2f sz, Vec2f pos, float minZ, float maxZ){
	D3DVIEWPORT_SPR vp;
	Intf()->GetViewport(&vp);
	if (vp.X == (DWORD)pos.X() && vp.Y == (DWORD)pos.Y() && vp.Width == (DWORD)sz.X() && vp.Height == (DWORD)sz.Y()) return true;
	Vec2f tsz = GetSurfaceSize();
	Vec2f rsz = pos+sz;
	if (rsz.X() > tsz.X() || rsz.Y() > tsz.Y()){
		if (!Resize(rsz)) return false;
	}

	vp.X=(DWORD)pos.X();
	vp.Y=(DWORD)pos.Y();
	vp.Width=(DWORD)sz.X();
	vp.Height=(DWORD)sz.Y();
	vp.MinZ=minZ;
	vp.MaxZ=maxZ;
	if (!WXCHECK(Intf()->SetViewport(&vp))) return false;
	return true;
}

bool WXD3DDevice::Create(HWND hDW, UINT adapter, D3DDEVTYPE deviceType,  DWORD behaviorFlags){
	Release();
	hFocusWindow = hDW;
	while (1){
		HWND hw = GetParent(hFocusWindow);
		if (hw) hFocusWindow = hw;
		else break;
	}
	WXD3D d3d;
	if (hDW && !presentParameters.hDeviceWindow) presentParameters.hDeviceWindow = hDW;
	HRESULT hr = d3d->CreateDevice(adapter, deviceType, hFocusWindow, behaviorFlags, &presentParameters, &Intf());
	if(hr && D3DCREATE_HARDWARE_VERTEXPROCESSING){
		behaviorFlags &= ~(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
		behaviorFlags |= D3DCREATE_MIXED_VERTEXPROCESSING;
		hr = d3d->CreateDevice(adapter, deviceType, hFocusWindow, behaviorFlags, &presentParameters, &Intf());
	}
	if (hr && D3DCREATE_MIXED_VERTEXPROCESSING){
		behaviorFlags &= ~(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
		behaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		hr = d3d->CreateDevice(adapter, deviceType, hFocusWindow, behaviorFlags, &presentParameters, &Intf());
	}
	if (!WXCHECK(hr)) return false;
	Intf()->SetRenderState(D3DRS_ZENABLE,TRUE);
	Intf()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	Intf()->SetRenderState(D3DRS_LIGHTING,TRUE);
    Intf()->SetRenderState(D3DRS_AMBIENT, 0x00000000);
	bInit = true;
	bCanDraw = false;
	return true;
}
Vec2f WXD3DDevice::GetSurfaceSize(){
	WXINTFV(Direct3DSurface) surf;
#if SPR_DXVER == 8
	Intf()->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &surf.Intf());
#elif SPR_DXVER == 9
	Intf()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surf.Intf());
#endif
	D3DSURFACE_DESC desc;
	surf->GetDesc(&desc);
	return Vec2f((float)desc.Width, (float)desc.Height);
}
Vec2f WXD3DDevice::GetViewportSize(){
	D3DVIEWPORT_SPR vp;	
	Intf()->GetViewport(&vp);
	return Vec2f((float)vp.Width, (float)vp.Height);
}

bool WXD3DDevice::Resize(Vec2f size, HWND wnd){
	if ((!wnd || wnd == presentParameters.hDeviceWindow) && size == GetSurfaceSize()) return true;
    EndScene();
	presentParameters.Window(0, (int)size.X(), (int)size.Y());
	if (wnd) presentParameters.hDeviceWindow = wnd;
	WXCHECK(Intf()->Reset(&presentParameters));

	winMan.HWnd(presentParameters.hDeviceWindow);
	return true;
}
void WXD3DDevice::Fullscreen(){
	if (!bFullscreen){
		winMan.SaveWindowPlace();
		bFullscreen = true;
	}
	WXD3DPresentParameters d3dpp(presentParameters);
	d3dpp.Fullscreen();
	d3dpp.hDeviceWindow = hFocusWindow;
	WXCHECK(Intf()->Reset(&d3dpp));
	bCanDraw = false;
}
void WXD3DDevice::Window(){
	if (bFullscreen){
		winMan.RestoreWindowPlace();
		bFullscreen = false;
	}
	WXCHECK(Intf()->Reset(&presentParameters));
	bCanDraw = false;
}
void WXD3DDevice::BeginScene(){
	if (bCanDraw) return;
	if (Intf()->BeginScene() == D3D_OK){
		bCanDraw = true;
	}
}
void WXD3DDevice::EndScene(){
	if (bCanDraw){
		bCanDraw = false;
		Intf()->EndScene();
		Intf()->SetTexture(0,NULL);
	}
}



IUnknown* WXDXFile::Create(){
	IDirectXFile* dx=NULL;
	DirectXFileCreate(&dx);
	char src[] =
	"xof 0303txt 0032\n"
	"template Header {\n"
	" <3D82AB43-62DA-11cf-AB39-0020AF71E433>\n"
	" WORD major;\n"
	" WORD minor;\n"
	" DWORD flags;\n"
	"}\n"
	"template Vector {\n"
	" <3D82AB5E-62DA-11cf-AB39-0020AF71E433>\n"
	" FLOAT x;\n"
	" FLOAT y;\n"
	" FLOAT z;\n"
	"}\n"
	"template Coords2d {\n"
	" <F6F23F44-7686-11cf-8F52-0040333594A3>\n"
	" FLOAT u;\n"
	" FLOAT v;\n"
	"}\n"
	"template Matrix4x4 {\n"
	" <F6F23F45-7686-11cf-8F52-0040333594A3>\n"
	" array FLOAT matrix[16];\n"
	"}\n"
	"template ColorRGBA {\n"
	" <35FF44E0-6C7C-11cf-8F52-0040333594A3>\n"
	" FLOAT red;\n"
	" FLOAT green;\n"
	" FLOAT blue;\n"
	" FLOAT alpha;\n"
	"}\n"
	"template ColorRGB {\n"
	" <D3E16E81-7835-11cf-8F52-0040333594A3>\n"
	" FLOAT red;\n"
	" FLOAT green;\n"
	" FLOAT blue;\n"
	"}\n"
	"template IndexedColor {\n"
	" <1630B820-7842-11cf-8F52-0040333594A3>\n"
	" DWORD index;\n"
	" ColorRGBA indexColor;\n"
	"}\n"
	"template Boolean {\n"
	" <4885AE61-78E8-11cf-8F52-0040333594A3>\n"
	" WORD truefalse;\n"
	"}\n"
	"template Boolean2d {\n"
	" <4885AE63-78E8-11cf-8F52-0040333594A3>\n"
	" Boolean u;\n"
	" Boolean v;\n"
	"}\n"
	"template MaterialWrap {\n"
	" <4885AE60-78E8-11cf-8F52-0040333594A3>\n"
	" Boolean u;\n"
	" Boolean v;\n"
	"}\n"
	"template TextureFilename {\n"
	" <A42790E1-7810-11cf-8F52-0040333594A3>\n"
	" STRING filename;\n"
	"}\n"
	"template Material {\n"
	" <3D82AB4D-62DA-11cf-AB39-0020AF71E433>\n"
	" ColorRGBA faceColor;\n"
	" FLOAT power;\n"
	" ColorRGB specularColor;\n"
	" ColorRGB emissiveColor;\n"
	" [...]\n"
	"}\n"
	"template MeshFace {\n"
	" <3D82AB5F-62DA-11cf-AB39-0020AF71E433>\n"
	" DWORD nFaceVertexIndices;\n"
	" array DWORD faceVertexIndices[nFaceVertexIndices];\n"
	"}\n"
	"template MeshFaceWraps {\n"
	" <4885AE62-78E8-11cf-8F52-0040333594A3>\n"
	" DWORD nFaceWrapValues;\n"
	" Boolean2d faceWrapValues;\n"
	"}\n"
	"template MeshTextureCoords {\n"
	" <F6F23F40-7686-11cf-8F52-0040333594A3>\n"
	" DWORD nTextureCoords;\n"
	" array Coords2d textureCoords[nTextureCoords];\n"
	"}\n"
	"template MeshMaterialList {\n"
	" <F6F23F42-7686-11cf-8F52-0040333594A3>\n"
	" DWORD nMaterials;\n"
	" DWORD nFaceIndexes;\n"
	" array DWORD faceIndexes[nFaceIndexes];\n"
	" [Material]\n"
	"}\n"
	"template MeshNormals {\n"
	" <F6F23F43-7686-11cf-8F52-0040333594A3>\n"
	" DWORD nNormals;\n"
	" array Vector normals[nNormals];\n"
	" DWORD nFaceNormals;\n"
	" array MeshFace faceNormals[nFaceNormals];\n"
	"}\n"
	"template MeshVertexColors {\n"
	" <1630B821-7842-11cf-8F52-0040333594A3>\n"
	" DWORD nVertexColors;\n"
	" array IndexedColor vertexColors[nVertexColors];\n"
	"}\n"
	"template Mesh {\n"
	" <3D82AB44-62DA-11cf-AB39-0020AF71E433>\n"
	" DWORD nVertices;\n"
	" array Vector vertices[nVertices];\n"
	" DWORD nFaces;\n"
	" array MeshFace faces[nFaces];\n"
	" [...]\n"
	"}\n"
	"template FrameTransformMatrix {\n"
	" <F6F23F41-7686-11cf-8F52-0040333594A3>\n"
	" Matrix4x4 frameMatrix;\n"
	"}\n"
	"template Frame {\n"
	" <3D82AB46-62DA-11cf-AB39-0020AF71E433>\n"
	" [...]\n"
	"}\n"
	"template FloatKeys {\n"
	" <10DD46A9-775B-11cf-8F52-0040333594A3>\n"
	" DWORD nValues;\n"
	" array FLOAT values[nValues];\n"
	"}\n"
	"template TimedFloatKeys {\n"
	" <F406B180-7B3B-11cf-8F52-0040333594A3>\n"
	" DWORD time;\n"
	" FloatKeys tfkeys;\n"
	"}\n"
	"template AnimationKey {\n"
	" <10DD46A8-775B-11cf-8F52-0040333594A3>\n"
	" DWORD keyType;\n"
	" DWORD nKeys;\n"
	" array TimedFloatKeys keys[nKeys];\n"
	"}\n"
	"template AnimationOptions {\n"
	" <E2BF56C0-840F-11cf-8F52-0040333594A3>\n"
	" DWORD openclosed;\n"
	" DWORD positionquality;\n"
	"}\n"
	"template Animation {\n"
	" <3D82AB4F-62DA-11cf-AB39-0020AF71E433>\n"
	" [...]\n"
	"}\n"
	"template AnimationSet {\n"
	" <3D82AB50-62DA-11cf-AB39-0020AF71E433>\n"
	" [Animation]\n"
	"}\n";
	WXCHECK(dx->RegisterTemplates(src, strlen(src)));
	return dx;
}
WXDXFile::WXDXFile(){
	Prepare();
}


}
