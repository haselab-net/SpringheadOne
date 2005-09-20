#include "WXRM.h"
#include <d3drm.h>
#include <mmsystem.h>
#include <dsound.h>

namespace LVRM {
WXRM* WXRM::last;
WXRM::intf_type* WXRM::pD3DRM;
//----------------------------------------------------------------------------
//	WXRMViewFrustum
//
WXRMViewFrustum::WXRMViewFrustum(REAL f, REAL b, Vec2f sz, Vec2f p)
	{
	front = f;
	back = b;
	bgn = p - sz/2;
	end = p + sz/2;
	aspect = 1;
	}
WXRMViewFrustum::WXRMViewFrustum(REAL f, REAL b, REAL w, REAL asp)
	{
	front = f;
	back = b;
	bgn.X() = -w/2;
	end.X() = w/2;
	aspect = asp;
	}
//----------------------------------------------------------------------------
//	WXRMViewport
//
WXRMViewport::WXRMViewport(const WXRMViewport& v)
	{
	pRMDev = v.pRMDev;
	if (pRMDev) pRMDev->AddRef();
	pRMView = v.pRMView;
	if (pRMView) pRMView->AddRef();
	}
WXRMViewport::WXRMViewport(dev_intf_type* d, intf_type* v){
	pRMDev = d;
	if (pRMDev) pRMDev->AddRef();
	pRMView = v;
	if (pRMView) pRMView->AddRef();
	}
WXRMViewport::WXRMViewport()
	{
	pRMDev = NULL;
	pRMView = NULL;
	}
void WXRMViewport::Release()
	{
	RELEASE(pRMView);
	RELEASE(pRMDev);
	}
WXRMViewport::~WXRMViewport()
	{
	Release();
	}
bool WXRMViewport::SetRenderQuality(D3DRMRENDERQUALITY rq)
	{
	if (pRMDev)
		return CKRMERR(pRMDev->SetQuality(rq));
	return false;
	}
bool WXRMViewport::SetTextureQuality(D3DRMTEXTUREQUALITY tq)
	{
	if (pRMDev)
		return CKRMERR(pRMDev->SetTextureQuality(tq));
	return false;
	}

bool WXRMViewport::SetViewport(Vec2f viewPos, Vec2f viewSize)
	{
	if (pRMView)
		{
		SIZE szView;
		szView.cx = pRMDev->GetWidth();
		szView.cy = pRMDev->GetHeight();
		POINT bgnView;
		bgnView.x = szView.cx * viewPos.X() + 0.5f;
		bgnView.y = szView.cy * viewPos.Y() + 0.5f;
		POINT endView;
		endView.x = szView.cx * (viewPos.X()+viewSize.X()) + 0.5f;
		endView.y = szView.cy * (viewPos.Y()+viewSize.Y()) + 0.5f;
		return CKRMERR(pRMView->Configure(bgnView.x, bgnView.y, endView.x - bgnView.x, endView.y - bgnView.y));
		}
	return false;
	}
bool WXRMViewport::SetViewFrustum(const WXRMViewFrustum & vf)
	{
	bool err = true;
    if (pRMView)
        {
    	err = CKRMERR(pRMView->SetUniformScaling(false));
    	err |= CKRMERR(pRMView->SetFront(vf.front));
    	err |= CKRMERR(pRMView->SetBack(vf.back));
    	err |= CKRMERR(pRMView->SetPlane(vf.bgn.X(), vf.end.X(), vf.bgn.Y(), vf.end.Y()));
        }
	return !err;
	}
bool WXRMViewport::SetCamera(WXRMFrame::intf_type* cam)
	{
	if (pRMView) 
		return !CKRMERR(pRMView->SetCamera(cam));
	return false;
	}
bool WXRMViewport::ForceUpdate()
	{
	if (pRMView) 
		{
		int x = pRMView->GetX();
		int y = pRMView->GetY();
		return CKRMERR(pRMView->ForceUpdate( x, y, x+pRMView->GetWidth(), y+pRMView->GetHeight() ));
		}
	return false;
	}
//----------------------------------------------------------------------------
//	WXRMError
//
bool WXRMError::Check(DWORD n, const char* msg, const char* fn, int ln)
	{
	if (n==DD_OK) return false;
	if (n==D3D_OK) return false;
	if (n==D3DRM_OK) return false;
	if (n==0) return false;
	if (RMErrorToString(err, desc, n)
		|| DDErrorToString(err, desc, n)
		|| D3DErrorToString(err, desc, n)){
		return WXDXError::Check(n, msg, fn, ln);
	}
	return false;
	}
bool WXRMError::RMErrorToString(char*& err, char*& desc, HRESULT error)
	{
	err = "";
	desc = "";
    switch(error) 
    	{
        case D3DRMERR_BADOBJECT:
			err = "D3DRMERR_BADOBJECT\0";
			return true;
        case D3DRMERR_BADTYPE:
			err = "D3DRMERR_BADTYPE\0";
			return true;
        case D3DRMERR_BADALLOC:
			err = "D3DRMERR_BADALLOC\0";
			return true;
        case D3DRMERR_FACEUSED:
			err = "D3DRMERR_FACEUSED\0";
			return true;
        case D3DRMERR_NOTFOUND:
			err = "D3DRMERR_NOTFOUND\0";
			return true;
        case D3DRMERR_NOTDONEYET:
			err = "D3DRMERR_NOTDONEYET\0";
			return true;
        case D3DRMERR_FILENOTFOUND:
			err = "D3DRMERR_FILENOTFOUND\0";
			desc = "The file was not found.";
			return true;
        case D3DRMERR_BADFILE:
			err = "D3DRMERR_BADFILE\0";
			return true;
        case D3DRMERR_BADDEVICE:
			err = "D3DRMERR_BADDEVICE\0";
			return true;
        case D3DRMERR_BADVALUE:
			err = "D3DRMERR_BADVALUE\0";
			return true;
        case D3DRMERR_BADMAJORVERSION:
			err = "D3DRMERR_BADMAJORVERSION\0";
			return true;
        case D3DRMERR_BADMINORVERSION:
			err = "D3DRMERR_BADMINORVERSION\0";
			return true;
        case D3DRMERR_UNABLETOEXECUTE:
			err = "D3DRMERR_UNABLETOEXECUTE\0";
			return true;
        default:
			desc = "d3drm unknown.";
            return false;
		}
	}
//----------------------------------------------------------------------------
//	WXRM
//
WXRMError* WXRM::error;
WXRMError& WXRM::Error()	{
	if (!error) error = new WXRMError;
	return *error;
}
WXRM::WXRM()
	{
	bD3DRMCreated = false;
	pD3RMBase = NULL;
	pD3Dev = NULL;
	pRMDev = NULL;
	pRMView = NULL;
	renderQuality = D3DRMLIGHT_ON | D3DRMFILL_SOLID | D3DRMSHADE_GOURAUD;
	textureQuality = D3DRMTEXTURE_NEAREST;
	hWnd = NULL;
	pClipper = NULL;
	viewPos = Vec2f(0,0);
	viewSize = Vec2f(1,1);
	CreateRM();
	}
WXRM::~WXRM()
	{
 	Release();
	ReleaseRM();
	}
void WXRM::CreateCallback(WXDDSurface* surf){
	TRACEALL("WXRM::CreateCallback\n");
	Create(false);
}
void WXRM::ReleaseCallback(WXDDSurface* surf){
	TRACEALL("WXRM::ReleaseCallback\n");
	Release(false);
}
void WXRM::DeleteCallback(WXDDSurface* surf){
	TRACEALL("WXRM::DeleteCallback\n");
	Release(true);
	pD3Dev = NULL;
}

ULONG WXRM::CreateD3DRM(){
	ULONG rtv=1;
	if (!pD3DRM){
		IDirect3DRM* pD3DRMOld=NULL;
		Direct3DRMCreate(&pD3DRMOld);
		if (!pD3DRMOld){
			TRACE("%d:%d   Direct3DRMCreate() failed.\n", __FILE__, __LINE__);
			abort();
		}
		pD3DRMOld->QueryInterface(WXRM::InterfaceID(), (void**)&pD3DRM);
		pD3DRMOld->Release();
		if (!pD3DRM){
			TRACE("%d:%d   QueryInterface(IID_IDirect3DRM3,  ) failed.\n", __FILE__, __LINE__);
			abort();
		}
	}else{
		rtv = pD3DRM->AddRef();
	}
	TRACEALL("pD3DRM's refcount ^%d\n", rtv);
	return rtv;
}
ULONG WXRM::ReleaseD3DRM(){
	if (!pD3DRM){
		TRACE("%s:%d   Can't release Direct3DRM.\n", __FILE__, __LINE__);
		return 0;
	}
	ULONG rtv = pD3DRM->Release();
	if (rtv == 0){
		pD3DRM = NULL;
		delete error;
		error = NULL;
	}
	TRACEALL("pD3DRM's refcount _%d\n", rtv);
	return rtv;
}
bool WXRM::CreateRM()
	{
	if (!bD3DRMCreated) CreateD3DRM();
	bD3DRMCreated = true;
	last = this;

	//	カメラがシーンの子孫でない場合は、カメラをシーンの子にする
	if (!scene.Find(camera)) scene.Add(camera);
	SetCamera(camera);
	return true;
	}
bool WXRM::Create(bool bAddCallback)
	{
	TRACEALL("WXRM::Create()\n");
	if (!CreateRM()) return false;
	if (!pRMDev){
		//	create device from D3DDevice
		if (pD3Dev){
			//	Check surface
			if (!pD3Dev->Restore()) return false;
			//	Set callbacks to the surface
			if (bAddCallback){
				pD3Dev->createCallbacks.Add(new WXCallback<WXDDSurface, WXRM>(this, &WXRM::CreateCallback));
				pD3Dev->releaseCallbacks.Add(new WXCallback<WXDDSurface, WXRM>(this, &WXRM::ReleaseCallback));
				pD3Dev->deleteCallbacks.Add(new WXCallback<WXDDSurface, WXRM>(this, &WXRM::DeleteCallback));
			}
			//	create RM device
			if (CKRMERR(pD3DRM->CreateDeviceFromD3D(pD3Dev->PWXDD()->pD3D, pD3Dev->pD3DDev, &pRMDev))) return false;
			if (CKRMERR(pRMDev->SetBufferCount(pD3Dev->IsRealFlip() ? 2 : 1))) return false;
		}
		//	get device from pD3RMBase(other WXRM)
		else if (pD3RMBase){
			if (!pD3RMBase->pRMDev) pD3RMBase->Create();
			pRMDev = pD3RMBase->pRMDev;
			if (!pRMDev) return false;
			pRMDev->AddRef();
			pD3Dev = pD3RMBase->pD3Dev;
			hWnd = pD3RMBase->hWnd;
			if (bAddCallback && pD3Dev){
				pD3Dev->createCallbacks.Add(new WXCallback<WXDDSurface, WXRM>(this, &WXRM::CreateCallback));
				pD3Dev->releaseCallbacks.Add(new WXCallback<WXDDSurface, WXRM>(this, &WXRM::ReleaseCallback));
				pD3Dev->deleteCallbacks.Add(new WXCallback<WXDDSurface, WXRM>(this, &WXRM::DeleteCallback));
			}
		}else if (hWnd){		//	create device from hWnd and Clipper
			if (!pClipper){
				if (CKRMERR(DirectDrawCreateClipper(0, &pClipper, NULL))) return false;
			}
			pClipper->SetHWnd(0, hWnd);
			RECT rc;
			GetClientRect(hWnd, &rc);
			if (CKRMERR(pD3DRM->CreateDeviceFromClipper(pClipper, NULL, rc.right, rc.bottom, &pRMDev))) return false;
		}
	}
	if (!pRMDev) return false;
	int devWidth, devHeight;
	devWidth = pRMDev->GetWidth();
	devHeight = pRMDev->GetHeight();
	if (!pRMView && CKRMERR(pD3DRM->CreateViewport(pRMDev, camera, 0, 0, devWidth, devHeight ,&pRMView))){
		RELEASE(pRMDev);
		return false;
	}
	SetViewport(viewPos, viewSize);
	ForceUpdate();
	SetRenderQuality(renderQuality);
	SetTextureQuality(textureQuality);
	SetViewFrustum();
	return true;
}
void WXRM::ReleaseRM()
	{
	Release(true);
	RELEASE(pClipper);
	if (bD3DRMCreated) ReleaseD3DRM();
	bD3DRMCreated = false;
	}
void WXRM::Release(bool bDelCallback)
	{
	TRACEALL("WXRM::Release\n");
	if (bDelCallback && pD3Dev)
		{
		pD3Dev->createCallbacks.Del(WXCallback<WXDDSurface, WXRM>(this, &WXRM::CreateCallback));
		pD3Dev->releaseCallbacks.Del(WXCallback<WXDDSurface, WXRM>(this, &WXRM::ReleaseCallback));
		pD3Dev->deleteCallbacks.Del(WXCallback<WXDDSurface, WXRM>(this, &WXRM::DeleteCallback));
		}
	WXRMViewport::Release();
	}
bool WXRM::FitSurface()
	{
	if (pD3Dev)
		return pD3Dev->FitSurface();
	if (hWnd)
		{
		RECT rc;
		GetClientRect(hWnd, &rc);
		if ((int)pRMDev->GetWidth() != rc.right-rc.left || (int)pRMDev->GetHeight() != rc.bottom-rc.top)
			{
			Release();
			Create();
			return true;
			}
		}
	return true;
	}
bool WXRM::Clear()
	{
	if (pD3Dev && pD3Dev->IsLost()) pD3Dev->Restore();
	if (!pRMView){
		RMERRMSG(2, "There is no WXD3DRMViewport.");
		return false;
	}
#if DIRECTDRAW_VERSION >= 0x0600
	bool rtv = !CKRMERR(pRMView->Clear(D3DRMCLEAR_ALL));
#else
	bool rtv = !CKRMERR(pRMView->Clear());
#endif
	return rtv;
}
bool WXRM::Render(){
	bool rtv = false;
	if (pD3Dev && pD3Dev->IsLost()) pD3Dev->Restore();
	if (!pRMView){
		RMERRMSG(2, "There is no D3DRMViewport.");
		goto leaveReturn;
	}
	if (!scene){
		RMERRMSG(2, "There is no scene to render.");
		goto leaveReturn;
	}
	rtv = !CKRMERR(pRMView->Render(scene));
leaveReturn:
	return rtv;
}
bool WXRM::Update(){
	bool rtv = false;
	if (pD3Dev && pD3Dev->IsLost())
    	if (!pD3Dev->Restore()) goto leave;
		
	if (pRMDev) rtv = !CKRMERR(pRMDev->Update());
leave:
	return rtv;
}
bool WXRM::Flip(){
	if (!pD3RMBase && pD3Dev) return pD3Dev->Flip();
	return false;
}
bool WXRM::Step(){
	bool rtv = true;
	FitSurface();
	rtv &= Clear();
	rtv &= Render();
	rtv &= Update();
	rtv &= Flip();
	return rtv;
}
void WXRM::RMBase(WXRM& rm){
	pD3RMBase = &rm;
}
void WXRM::Surf(WXD3Device& dev){
	Release();
	hWnd = NULL;
	pD3Dev = &dev;
	if (!pD3Dev->pWXDD) pD3Dev->pWXDD = &WXDDraws::GetLastCreate()->Primary();
}
void WXRM::HWnd(HWND wnd){
	Release();
	hWnd = wnd;
	pD3Dev = NULL;
}
bool WXRM::ViewFrustum(const WXRMViewFrustum& v)
	{
	viewFrustum = v;
	return SetViewFrustum();
	}
bool WXRM::SetViewFrustum()
	{
	if (viewFrustum.front == 0) viewFrustum.front = REAL(1);
	if (viewFrustum.back == 0) viewFrustum.back = REAL(1000);
	if (viewFrustum.bgn.X() == 0) viewFrustum.bgn.X() = -viewFrustum.front / 2;
	if (viewFrustum.end.X() == 0) viewFrustum.end.X() = viewFrustum.front / 2;
	REAL w = 1, h = 1;
    if (pRMView)
        {
        w=pRMView->GetWidth();
        h=pRMView->GetHeight();
        }
	REAL asp = h/w * viewFrustum.aspect;
	REAL top = viewFrustum.bgn.Y();
	REAL bottom = viewFrustum.end.Y();
	if (viewFrustum.bgn.Y() == 0) top = viewFrustum.bgn.X() * asp;
	if (viewFrustum.end.Y() == 0) bottom = viewFrustum.end.X() * asp;
	curViewFrustum.front = viewFrustum.front;
	curViewFrustum.back = viewFrustum.back;
	curViewFrustum.bgn.X() = viewFrustum.bgn.X();
	curViewFrustum.end.X() = viewFrustum.end.X();
	curViewFrustum.bgn.Y() = top;
	curViewFrustum.end.Y() = bottom;
	return WXRMViewport::SetViewFrustum(curViewFrustum);
	}

void WXRM::Viewport(Vec2f vPos, Vec2f vSize)
	{
	viewPos = vPos;
	viewSize = vSize;
	SetViewport(viewPos, viewSize);
	}
void WXRM::Scene(const WXRMFrame& s)
	{
	WXRMFrame fr;
	fr = s;
	Scene(fr);
	}
void WXRM::Scene(WXRMFrame& s)
	{
	//	シーンフレームを作成し設定
	s.Create();
	scene = s;
	//	カメラがシーンの子孫でない場合は、カメラをシーンの子にする
	if (!scene.Find(camera)) scene.Add(camera);
	}
void WXRM::Camera(const WXRMFrame& c)
	{
	WXRMFrame fr;
	fr = c;
	Camera(fr);
	}
void WXRM::Camera(WXRMFrame& c)
	{
	c.Create();
	camera = c;
	//	カメラがシーンの子孫でない場合は、カメラをシーンの子にする
	if (!scene.Find(camera)) scene.Add(camera);
	SetCamera(camera);
	}
void WXRM::SceneCamera(WXRMFrame&s, WXRMFrame& c)
	{
	if (!s.Find(c)) s.Add(c);
	scene = s;
	camera = c;
	SetCamera(camera);
	}
void WXRM::RenderQuality(D3DRMRENDERQUALITY rq)
	{
	renderQuality = rq;
	SetRenderQuality(rq);
	}
void WXRM::TextureQuality(D3DRMTEXTUREQUALITY tq)
	{
	textureQuality = tq;
	SetTextureQuality(tq);
	}

WXRM& WXRM::GetLastCreate()
	{
	VRASSERT(last);
	return *last;
	}

WXRMObject WXRM::Find(const char* name)
	{
	IDirect3DRMObject* pObj=NULL;
	CKRMERR(pD3DRM->GetNamedObject(name, &pObj));
	return pObj;
	}
WXRM::intf_type* WXRM::operator ->() const
	{
	return pD3DRM;
	}
WXRM::intf_type* WXRM::operator *() const
	{
	return pD3DRM;
	}
static WXD3Vec3 ConvVec3(D3DRMVECTOR4D sch)
	{
	WXD3Vec3 sco;
	if (sch.w)
		{
		REAL rhw = 1/sch.w;
		sco.X() = sch.x * rhw;
		sco.Y() = sch.y * rhw;
		sco.Z() = sch.z * rhw;
		}
	return sco;
	}
WXD3Vec3 WXRM::WorldToView(const WXD3Vec3& w)
	{
	D3DRMVECTOR4D sch;
	VRASSERT(pRMView);
    D3DVECTOR v3w = (D3DVECTOR&)w;
	CKRMERR(pRMView->Transform(&sch, &v3w));
	WXD3Vec3 sc;
	if (sch.w)
		{
		REAL rhw = 1/sch.w;
		sc.X() = sch.x * rhw;
		sc.Y() = sch.y * rhw;
		sc.Z() = sch.z * rhw;
		}
	return sc;
	}
WXD3Vec3 WXRM::ViewToWorld(const WXD3Vec3& v){
	D3DRMVECTOR4D sch;
	sch.x = v.X();
	sch.y = v.Y();
	sch.z = v.Z();
	sch.w = 1;
	D3DVECTOR w;
	VRASSERT(pRMView);
	CKRMERR(pRMView->InverseTransform(&w, &sch));
	return (WXD3Vec3&)w;
}

//////////////////////////////////////////////////////////////

/*
#include <Hase/WXD3D/WXD3DMacs.h>
bool WXRM::SetClip()
	{
	WXD3DEXECUTEBUFFERDESC debDesc;
	WXD3DEXECUTEDATA d3dExData;
	LPDIRECT3DEXECUTEBUFFER pD3DExCmdBuf = NULL;
	LPVOID lpBuffer, lpInsStart;
	size_t size;
	if (!pD3Dev) return true;
	//	there is no WXD3D Viewport, we must return true because it is not required.
    IDirect3DViewport* pD3DView = NULL;
    pRMView->GetDirect3DViewport(&pD3DView);
	if (!pD3DView) return true;
	//	Create an execute buffer of the required size
	size = 0;
	size += sizeof(WXD3DINSTRUCTION) * 2;
	size += sizeof(WXD3DSTATUS) * 1;
	memset(&debDesc, 0, sizeof(WXD3DEXECUTEBUFFERDESC));
	debDesc.dwSize = sizeof(WXD3DEXECUTEBUFFERDESC);
	debDesc.dwFlags = D3DDEB_BUFSIZE;
	debDesc.dwBufferSize = size;
	if (CKRMERR(pD3Dev->pD3DDev->CreateExecuteBuffer(&debDesc, &pD3DExCmdBuf, NULL))) goto exit_with_error;
	//	Lock the execute buffer so it can be filled
	if (CKRMERR(pD3DExCmdBuf->Lock(&debDesc))) goto exit_with_error;
	memset(debDesc.lpData, 0, size);
	lpInsStart = debDesc.lpData;
	lpBuffer = lpInsStart;
	//	Set render state
	OP_SET_STATUS(
		WXD3DSETSTATUS_ALL,
		WXD3DSTATUS_DEFAULT | WXD3DCLIP_BACK | WXD3DCLIP_BOTTOM | WXD3DCLIP_FRONT | WXD3DCLIP_LEFT | WXD3DCLIP_RIGHT | WXD3DCLIP_TOP,
		0,0,0,0, lpBuffer);
	OP_EXIT(lpBuffer);
	if (CKRMERR(pD3DExCmdBuf->Unlock())) goto exit_with_error;
	//	Set the execute data and exectue the buffer
	memset(&d3dExData, 0, sizeof(WXD3DEXECUTEDATA));
	d3dExData.dwSize = sizeof(WXD3DEXECUTEDATA);
	d3dExData.dwInstructionOffset = (ULONG) 0;
	d3dExData.dwInstructionLength = (ULONG) ((char*)lpBuffer - (char*)lpInsStart);
	pD3DExCmdBuf->SetExecuteData(&d3dExData);
	if (CKRMERR(pD3Dev->pD3DDev->BeginScene())) goto exit_with_error;
	if (CKRMERR(pD3Dev->pD3DDev->Execute(pD3DExCmdBuf,pD3DView,WXD3DEXECUTE_UNCLIPPED))) goto exit_with_error;
	if (CKRMERR(pD3Dev->pD3DDev->EndScene())) goto exit_with_error;
	//	We are done with the execute buffer, so release it.
	pD3DExCmdBuf->Release();
	return true;

exit_with_error:
	RELEASE(pD3DExCmdBuf);
	return false;
	}
*/
}	//	namespace LVRM
