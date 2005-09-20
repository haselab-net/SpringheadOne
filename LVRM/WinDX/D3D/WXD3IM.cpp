#include "WXD3IM.h"
#include <d3d.h>
#pragma hdrstop
#define CKERR(hr)			pIM->CKIMERR(hr)
#define CKERRMSG(hr,msg)	pIM->CKIMERRMSG(hr,msg)
/*--------------*/
/*	CIMObject	*/
/*--------------*/
namespace LVRM {
CIMObject::CIMObject():
	CDXObject()
	{
	pIM = NULL;
	handle = NULL;
	bCreated = false;
	}
CIMObject::CIMObject(CIMObject& o):
	CDXObject(o)
	{
	pIM = NULL;
	handle = NULL;
	bCreated = false;
	}
CIMObject::CIMObject(IUnknown* o):
	CDXObject(o)
	{
	pIM = NULL;
	handle = NULL;
	bCreated = false;
	}
void CIMObject::CheckIM()
	{
	if (pIM) return;
	pIM = &WXD3IM::GetLastCreate();
	}
bool CIMObject::Create()
	{
	CheckIM();
	if (bCreated) return true;
	pIM->objs.Add(*this);
	bCreated = true;
	bRelease = true;
	return true;
	}
void CIMObject::Release()
	{
	if (bCreated)
		{
		pIM->objs.Del(*this);
		bCreated = false;
		}
	RELEASE(pObject);
	}
/*--------------*/
/*	CIMMatrix	*/
/*--------------*/
CIMMatrix::CIMMatrix()
	{
	}
CIMMatrix::CIMMatrix(REAL front, REAL back, WXD3Vec2 center, WXD3Vec2 size)
	{
	REAL tmp = back/(back-front);
	REAL invN = 1/front;
	mat._11 = 1/size.X();			mat._12 = 0;				mat._13 = 0;		mat._14=0;
	mat._21 = 0;				mat._22 = 1/size.Y();			mat._23 = 0;		mat._24=0;
	mat._31 = center.X()*invN;	mat._32 = center.Y()*invN;	mat._33 = tmp*invN;	mat._34 = invN;
	mat._41 = 0;				mat._42 = 0;				mat._43 = -tmp;		mat._44 = 0;
	}
CIMMatrix::CIMMatrix(const WXD3Affine& af)
	{
	mat = af;
	}
CIMMatrix::CIMMatrix(const CIMMatrix& m)
	{
	mat = m.mat;
	}
CIMMatrix::~CIMMatrix()
	{
	Release();
	}
bool CIMMatrix::Create()
	{
	CIMObject::Create();
	if (handle) return true;
	if (!pIM->PDev()) pIM->PCDev()->Create();
	AfterDevCreate();
	return handle!=0;
	}
void CIMMatrix::Release()
	{
	CIMObject::Release();
	if (!handle) return;
	CKERR(pIM->PDev()->DeleteMatrix(handle));
	handle = NULL;
	}
CIMMatrix::operator WXD3Affine()
	{
	return mat;
	}
CIMMatrix CIMMatrix::operator = (const WXD3Affine& af)
	{
	mat = af;
	if (handle) Set();
	return *this;
	}
CIMMatrix CIMMatrix::operator = (const CIMMatrix& m)
	{
	mat = m.mat;
	if (handle) Set();
	return *this;
	}
void CIMMatrix::AfterDevCreate()
	{
	CheckIM();
	if (CKERR(pIM->PDev()->CreateMatrix(&handle))) Release();
	if (CKERR(pIM->PDev()->SetMatrix(handle, &mat))) Release();
	}
bool CIMMatrix::Get()
	{
	if (!handle) return false;
	return !CKERR(pIM->PDev()->GetMatrix(handle, &mat));
	}
bool CIMMatrix::Set()
	{
	if (!handle)
		{
		return Create();
		}
	return !CKERR(pIM->PDev()->SetMatrix(handle, &mat));
	}
/*--------------*/
/*	CIMLight	*/
/*--------------*/
CIMLight::CIMLight(const CIMLight& l):
	CIMObject(l)
	{
	*(WXD3DLIGHT*)this = *(WXD3DLIGHT*)&l;
	}
CIMLight::CIMLight(IDirect3DLight* l):
	CIMObject(l)
	{
	memset((WXD3DLIGHT*)this, 0, sizeof(WXD3DLIGHT));
	dwSize = sizeof(WXD3DLIGHT);
	if (PLight()) Get();
	}
CIMLight::~CIMLight()
	{
	Release();
	}
bool CIMLight::Create()
	{
	CIMObject::Create();
	if (PLight()) return true;
	CheckIM();
	if (CKERR(pIM->PWXD3D()->CreateLight(&PLight(), NULL)) ) return false;
	return true;
	}
void CIMLight::Release()
	{
	CIMObject::Release();
	if (!PLight()) return;
	}
void CIMLight::AfterDevCreate()
	{
	Set();
	}
bool CIMLight::Set()
	{
	Create();
	return !CKERR(PLight()->SetLight(this));
	}
bool CIMLight::Get()
	{
	Create();
	return !CKERR(PLight()->GetLight(this));
	}
/*--------------*/
/*	CIMMaterial	*/
/*--------------*/
CIMMaterial::CIMMaterial(const CIMMaterial& m):
	CIMObject(m)
	{
	*(WXD3DMATERIAL*)this = *(WXD3DMATERIAL*)&m;
	}
CIMMaterial::CIMMaterial(IDirect3DMaterial* m):
	CIMObject(m)
	{
	memset((WXD3DMATERIAL*)this, 0, sizeof(WXD3DMATERIAL));
	dwSize = sizeof(WXD3DMATERIAL);
	if (PMaterial()) Get();
	}
CIMMaterial::CIMMaterial(D3DCOLORVALUE di, D3DCOLORVALUE am, D3DCOLORVALUE sp, D3DCOLORVALUE em, REAL pw)
	{
	memset((WXD3DMATERIAL*)this, 0, sizeof(WXD3DMATERIAL));
	dwSize = sizeof(WXD3DMATERIAL);
	Set(di, am, sp, em, pw);
	}
CIMMaterial::CIMMaterial(D3DCOLORVALUE c, REAL pw)
	{
	memset((WXD3DMATERIAL*)this, 0, sizeof(WXD3DMATERIAL));
	dwSize = sizeof(WXD3DMATERIAL);
	Set(c, pw);
	}
CIMMaterial::~CIMMaterial()
	{
	Release();
	}
bool CIMMaterial::Create()
	{
	CIMObject::Create();
	if (PMaterial()) return true;
	CheckIM();
	if ( !PMaterial() && CKERR(pIM->PWXD3D()->CreateMaterial(&PMaterial(), NULL)) ) return false;
	return true;
	}
void CIMMaterial::Release()
	{
	CIMObject::Release();
	if (!PMaterial()) return;
	RELEASE(PMaterial());
	}
void CIMMaterial::AfterDevCreate()
	{
	CheckIM();
	CKERR( PMaterial()->GetHandle(pIM->PCDev()->pD3DDev, &handle) );
	}
void CIMMaterial::Set(D3DCOLORVALUE c, REAL pw)
	{
	D3DCOLORVALUE dcvZero = {WXD3DVAL(0), WXD3DVAL(0), WXD3DVAL(0), WXD3DVAL(0)};
	Set(c, c, c, dcvZero, pw);
	}
void CIMMaterial::Set(D3DCOLORVALUE di, D3DCOLORVALUE am, D3DCOLORVALUE sp, D3DCOLORVALUE em, REAL pw)
	{
	Create();
	diffuse = di;
	ambient = am;
	specular = sp;
	emissive = em;
	power = pw;
	Set();
	}
bool CIMMaterial::Set()
	{
	Create();
	return !CKERR(PMaterial()->SetMaterial(this));
	}
bool CIMMaterial::Get()
	{
	return !CKERR(PMaterial()->GetMaterial(this));
	}

/*----------------------*/
/*	CIMExecuteBuffer	*/
/*----------------------*/
CIMExecuteBuffer::CIMExecuteBuffer():
	CIMObject()
	{
	bLock = false;
	pCur = NULL;
	memset(&desc, 0, sizeof(desc));
	desc.dwSize = sizeof(desc);
	memset(&data, 0, sizeof(data));
	data.dwSize = sizeof(data);
	}
bool CIMExecuteBuffer::Create()
	{
	CIMObject::Create();
	if (PBuf()) return true;
	CheckIM();
	if (!pIM->PCDev()) return false;
	if (!pIM->PCDev()->Restore()) return false;
	if (PBuf()) return true;
	descOld = desc;
	if (CKERR(pIM->PDev()->CreateExecuteBuffer(&desc, &PBuf(), NULL))) return false;
	return true;
	}
void CIMExecuteBuffer::AfterDevCreate()
	{
	Create();
	}
void CIMExecuteBuffer::Release()
	{
	CIMObject::Release();
	BeforeDevRelease();
	}
void CIMExecuteBuffer::BeforeDevRelease()
	{
	if (!PBuf()) return;
	Unlock();
	RELEASE(PBuf());
	desc = descOld;
	}
DWORD CIMExecuteBuffer::Size()
	{
	return desc.dwBufferSize;
	}
void CIMExecuteBuffer::Size(size_t sz)
	{
	desc.dwFlags |= D3DDEB_BUFSIZE;
	desc.dwBufferSize = sz;
	}
DWORD CIMExecuteBuffer::Caps()
	{
	return desc.dwCaps;
	}
void CIMExecuteBuffer::Caps(DWORD c)
	{
	desc.dwFlags |= D3DDEB_CAPS;
	desc.dwCaps = c;
	}
bool CIMExecuteBuffer::Lock()
	{
	if (bLock) return true;
	WXDD_ENTER_CRITICAL;	
	if (!Create())
		{
		WXDD_LEAVE_CRITICAL;	
		return false;
		}
	if (CKERR(PBuf()->Lock(&desc)))
		{
		WXDD_LEAVE_CRITICAL;	
		return false;
		}
	bLock = true;
	pCur = (BYTE*)desc.lpData;
    data.dwVertexCount = 0;
    data.dwInstructionOffset = 0;
    data.dwInstructionLength = 0;
	return true;
	}
void CIMExecuteBuffer::Unlock()
	{
	if (!bLock) return;
	CKERR(PBuf()->Unlock());
	bLock = false;
	pCur = NULL;
	CKERR(PBuf()->SetExecuteData(&data));
	WXDD_LEAVE_CRITICAL;
	}
void CIMExecuteBuffer::EndVertex()
	{
    data.dwVertexCount = PVertex() - (WXD3DVERTEX*)desc.lpData;
    data.dwInstructionOffset = pCur - (BYTE*)desc.lpData;
	}
void CIMExecuteBuffer::EndInst()
	{
    data.dwInstructionLength = (pCur - (BYTE*)desc.lpData) - data.dwInstructionOffset;
	}
/*----------------------*/
/*	WXD3IM::CIMObjects	*/
/*----------------------*/
void WXD3IM::CIMObjects::AfterDevCreate()
	{
	if (End())
		for(iterator it = End()-1; it>=Bgn(); it--)
			((CIMObject*)(*it))->AfterDevCreate();
	}
void WXD3IM::CIMObjects::BeforeDevRelease()
	{
	if (End())
		for(iterator it = End()-1; it>=Bgn(); it--)
			((CIMObject*)(*it))->BeforeDevRelease();
	}
/*------------------*/
/*		WXD3IM		*/
/*------------------*/
DLLCLASS WXD3IM* WXD3IM::last;
WXD3IM::WXD3IM()
	{
	pView = NULL;
	pD3Dev = NULL;
	}
WXD3IM::~WXD3IM()
	{
	Release();
	}
void WXD3IM::Surf(WXD3Device& dev)
	{
	if (PView())
		{
		if (pD3Dev->pDDS) ReleaseCallback(*pD3Dev, this);
		DeleteCallback(*pD3Dev, this);
		pD3Dev = &dev;
		UpdateViewport();
		pD3Dev->createCallbacks.Add(CreateCallback, this);
		pD3Dev->createCallbacks.Add(ReleaseCallback, this);
		pD3Dev->deleteCallbacks.Add(DeleteCallback, this);
		pD3Dev->Create();
		}
	else
		{
		pD3Dev = &dev;
		}
	}
bool WXD3IM::Create()
	{
	last = this;
	if (pView) return true;
	WXDD_ENTER_CRITICAL;
	if (!pD3Dev)
		{
		IMERRMSG(2, "Call Set(WXD3Device&) before calling Create().");
		goto leave;
		}
	if (!pD3Dev->Create()) goto leave;
	if (!exbuf.Create()) goto leave;
	if (CKIMERR( pD3Dev->PWXDD()->pD3D->CreateViewport(&pView, NULL) )) goto leave;
	UpdateViewport();
	pD3Dev->createCallbacks.Add(CreateCallback, this);
	pD3Dev->releaseCallbacks.Add(ReleaseCallback, this);
	pD3Dev->deleteCallbacks.Add(DeleteCallback, this);
	WXDD_LEAVE_CRITICAL;
	return true;
leave:
	RELEASE(pD3Dev);
	exbuf.Release();
	RELEASE(pView);
	WXDD_LEAVE_CRITICAL;
	return false;
	}
void WXD3IM::Release()
	{
	if (!pView) return;
	WXDD_ENTER_CRITICAL;
	pD3Dev->createCallbacks.Del(CreateCallback, this);
	pD3Dev->releaseCallbacks.Del(ReleaseCallback, this);
	pD3Dev->deleteCallbacks.Del(DeleteCallback, this);
	objs.Release();
	RELEASE(pView);
	WXDD_LEAVE_CRITICAL;
	}
bool WXD3IM::Clear(DWORD mtr)
	{
	if (!PView() && !Create()) return false;
	if (!PCDev()) return false;
	PView()->SetBackground(mtr);
	WXD3DRECT rc;
	rc.x1 = 0;
	rc.y1 = 0;
	rc.x2 = PCDev()->Size().cx;
	rc.y2 = PCDev()->Size().cy;
	if (PView()->Clear(1, &rc, WXD3DCLEAR_TARGET | WXD3DCLEAR_ZBUFFER) != D3D_OK)
		{
		if (!PCDev()->Restore()) return false;
		CKIMERR(PView()->Clear(1, &rc, WXD3DCLEAR_TARGET | WXD3DCLEAR_ZBUFFER));
		}
	return true;
	}
bool WXD3IM::Execute(DWORD dwClip)
	{
	if (!PView() && !Create()) return false;
	if (!PCDev()) return false;
	if (PDev()->Execute(exbuf, pView, dwClip) != D3D_OK)
		{
		if (!PCDev()->Restore()) return false;
		return !CKIMERR(PDev()->Execute(exbuf, pView, dwClip));
		}
	return true;
	}
bool WXD3IM::BeginScene()
	{
	if (!PCDev()->Create()) return false;
	return !CKIMERR(PDev()->BeginScene());
	}
bool WXD3IM::EndScene()
	{
	if (!PCDev()->Create()) return false;
	return !CKIMERR(PDev()->EndScene());
	}
void WXD3IM::CreateCallback(WXDDSurface& surf, void* pContext)
	{
	WXD3IM* pIM = (WXD3IM*)pContext;
	if (!pIM->UpdateViewport()) pIM->Release();
	pIM->objs.AfterDevCreate();
	}
void WXD3IM::ReleaseCallback(WXDDSurface& surf, void* pContext)
	{
	WXD3IM* pIM = (WXD3IM*)pContext;
	pIM->objs.BeforeDevRelease();
	}
void WXD3IM::DeleteCallback(WXDDSurface& surf, void* pContext)
	{
	WXD3IM* pIM = (WXD3IM*)pContext;
	pIM->Release();
	}
bool WXD3IM::UpdateViewport()
	{
	if (CKIMERR( pD3Dev->pD3DDev->AddViewport(pView) )) return false;
	WXD3DVIEWPORT d3dViewport;
    ZeroMemory(&d3dViewport, sizeof(d3dViewport));
    d3dViewport.dwSize   = sizeof(d3dViewport);
    d3dViewport.dwX      = 0UL;
    d3dViewport.dwY      = 0UL;
    d3dViewport.dwWidth  = PCDev()->Size().cx;
    d3dViewport.dwHeight = PCDev()->Size().cy;
    d3dViewport.dvScaleX = WXD3DVAL(d3dViewport.dwWidth);
    d3dViewport.dvScaleY = WXD3DVAL(d3dViewport.dwHeight);
    d3dViewport.dvMaxX   = WXD3DVAL(1.0);
    d3dViewport.dvMaxY   = WXD3DVAL(1.0);
    if (CKIMERR(PView()->SetViewport(&d3dViewport))) return false;
	return true;
	}
}	//	namespace LVRM
