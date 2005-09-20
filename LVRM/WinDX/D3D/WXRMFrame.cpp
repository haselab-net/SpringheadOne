#include "WXRM.h"
#include "WXRMVisual.h"
#pragma hdrstop
#define CKERR CKRMERR
#define CKERRFILE CKRMERRFILE
#define ERRMSG RMERRMSG

namespace LVRM {
//-----------------------------------------------------------------------------
//	WXRMObject
//
WXRMObject::WXRMObject(const WXRMObject& o):WXUnknown(o){
	name = NULL;
	if(o.intf){
		BeforeCreate();
		AfterCreate();
	}
}
WXRMObject::WXRMObject(IDirect3DRMObject* o):WXUnknown(o){
	name = NULL;
	if(o){
		BeforeCreate();
		AfterCreate();
	}
}
WXRMObject::~WXRMObject(){
	Release();
}
IDirect3DRMObject* WXRMObject::Clone() const
	{
	IDirect3DRMObject* rtv = NULL;
	if (PRMObj()) CKERR(PRMObj()->Clone(NULL, InterfaceID(), (void**)&rtv));
	return rtv;
	}
void WXRMObject::DestroyCallback(IDirect3DRMObject* , void* pArg)
	{
	WXRMObject* pObj = (WXRMObject*)pArg;
	pObj->PRMObj() = NULL;
	}
bool WXRMObject::Create(){
	if (PRMObj()) return true;
	BeforeCreate();
	if (ClassID() != WXUnknown::ClassID())
		CKRMERR( WXRM::pD3DRM->CreateObject(ClassID(), NULL, InterfaceID(), (void**)&PRMObj()) );
	return AfterCreate();
	}
void WXRMObject::BeforeCreate(){
	WXRM::CreateD3DRM();
}
bool WXRMObject::AfterCreate(){
	if (!PRMObj())
		{
		ERRMSG(2, "Create RM object before call WXRMObject::AfterCreate.");
		return false;
		}
	if (name) CKERR(PRMObj()->SetName(name));
	return !CKERR(PRMObj()->AddDestroyCallback(DestroyCallback, this));
}
void WXRMObject::Release(){
	delete name;
	name = NULL;
	if (PRMObj()){
		CKERR(PRMObj()->DeleteDestroyCallback(DestroyCallback, this));
		WXUnknown::Release();
		WXRM::ReleaseD3DRM();
	}
}
const char* WXRMObject::Name(){
	if (name) return name;
	if (!PRMObj()){
		ERRMSG(2, "NULL object used.");
		return NULL;
	}
	DWORD size;
	CKERR(PRMObj()->GetName(&size, NULL));
    if (size == 0){
        name = new char[1];
        name[0] = '\0';
        return name;
	}
	name = new char[size];
	CKERR(PRMObj()->GetName(&size, name));
	return name;
}
void WXRMObject::Name(const char* n)
	{
	delete name;
	name = NULL;
	if (n && strlen(n))
		{
		name = new char[strlen(n) + 1];
		strcpy(name, n);
		}
	if (!PRMObj()) return;
	CKERR(PRMObj()->SetName(name));
	}
const WXRMObject& WXRMObject::operator = (const WXRMObject& o)
	{
	Release();
	VRASSERT(!o.intf || Check(o.intf));
	intf = o.intf;
	if (intf)
		{
		BeforeCreate();
		intf->AddRef();
		AfterCreate();
		}
	return *this;
	}
const IID& WXRMObject::InterfaceID() const
	{
	return IID_IDirect3DRMObject;
	}
bool WXRMObject::Check(IUnknown* pUk) const
	{
	if (!pUk) return false;
	IUnknown* newIf=NULL;
	pUk->QueryInterface(InterfaceID(), (void**)&newIf);
	if (newIf)
		{
		newIf->Release();
		return true;
		}
	return false;
	}
//-----------------------------------------------------------------------------
//	WXRMFrame
//
WXRMFrame::WXRMFrame(const char* fname){
	Load(fname);
}
void WXRMFrame::CreateFrame(){
	BeforeCreate();
	bool err = CKERR( WXRM::pD3DRM->CreateFrame(NULL, &PFrame()) );
	VRASSERT(!err);
	AfterCreate();
}
bool WXRMFrame::Load(const char* fname)
	{
	Create();
	return !CKERR(PFrame()->Load((void*)fname, NULL, D3DRMLOAD_INSTANCEBYREFERENCE, NULL, NULL));
	}
bool WXRMFrame::Find(const WXRMFrame& frOrg)
	{
	if (!PFrame()) return false;
	WXRMFrame fr(frOrg);
	while(fr)
		{
		if (fr == *this) break;
		fr = fr.Parent();
		}
	if (fr) return true;
	return false;
	}
WXRMFrame WXRMFrame::Find(const char* n)
	{
	if (!PFrame()) return WXRMFrame();
	if (strcmp(Name(), n) == 0) return *this;
	int nCh = NChild();
	WXRMFrame fr;
	for(int i=0; i<nCh; i++)
		{
		fr = Child(i).Find(n);
		if (fr) return fr;
		};
	return fr;
	}
WXRMFrame WXRMFrame::Parent(){
#if DIRECTDRAW_VERSION == 0x500
	IDirect3DRMFrame* f = NULL;
#else
	intf_type* f = NULL;
#endif
	if (PFrame()) CKERR(PFrame()->GetParent(&f));
	if (f){
		int ref = f->Release();
		TRACEALL("WXRMFrame::Parent ref = %d\n", ref);
	}
#if DIRECTDRAW_VERSION == 0x500
	intf_type* rv = NULL;
	if (f) f->QueryInterface(InterfaceID(), (void**)&rv);
	return rv;
#else
	return f;
#endif
}
void WXRMFrame::Parent(intf_type* pf){
	intf_type* parent = Parent();
	if (  parent && CKERR( parent->DeleteChild(PFrame()) )  ) return;
	if (pf) CKERR(pf->AddChild(PFrame()));
}
void WXRMFrame::Add(WXRMFrame& f)
	{
	Create();
	f.Create();
	CKERR(PFrame()->AddChild(f));
	}
void WXRMFrame::Del(const WXRMFrame& f)
	{
	if (PFrame()) CKERR(PFrame()->DeleteChild(f));
	}
void WXRMFrame::DelThis()
	{
	if (Parent()) Parent().Del(*this);
	}
WXRMFrame WXRMFrame::Child(int id)
	{
	IDirect3DRMFrameArray* fa=NULL;
	if (CKERR(PFrame()->GetChildren(&fa))) return WXRMFrame();
	IDirect3DRMFrame* f=NULL;
	if (!CKERR(fa->GetElement(id, &f)) && f)
		{
		int ref = f->Release();
		TRACEALL("WXRMFrame::Parent ref = %d\n", ref);
		}
	RELEASE(fa);
	intf_type* ri=NULL;
	CKRMERR(f->QueryInterface(InterfaceID(), (void**)&ri));
	ri->Release();
	return ri;
	}
int WXRMFrame::NChild()
	{
	if (!PFrame()) return 0;
	IDirect3DRMFrameArray* fa=NULL;
	if (CKERR(PFrame()->GetChildren(&fa))) return 0;
	int rtv = fa->GetSize();
	RELEASE(fa);
	return rtv;
	}

void WXRMFrame::Add(const WXRMLight& light){
	Create();
	CKERR(PFrame()->AddLight(light));
}
void WXRMFrame::Del(const WXRMLight& light)
	{
	Create();
	CKERR(PFrame()->DeleteLight(light));
	}
WXRMLight WXRMFrame::Light(int id)
	{
	IDirect3DRMLightArray* la=NULL;
	if (CKERR(PFrame()->GetLights(&la))) return WXRMLight();
	IDirect3DRMLight* l=NULL;
	if (!CKERR(la->GetElement(id, &l)) && l)
		{
		int ref = l->Release();
		TRACEALL("WXRMFrame::Light ref = %d\n", ref);
		}
	RELEASE(la);
	return l;
	}
WXRMLight WXRMFrame::Light(const char* name)
	{
	IDirect3DRMLightArray* la=NULL;
	if (CKERR(PFrame()->GetLights(&la))) return WXRMLight();
	for(unsigned int i=0; i<la->GetSize(); i++)
		{
		IDirect3DRMLight* l=NULL;
		if (!CKERR(la->GetElement(i, &l)) && l)
			{
			int ref = l->Release();
			TRACEALL("WXRMFrame::Light ref = %d\n", ref);
			DWORD nlen=0;
			l->GetName(&nlen, NULL);
			char* n = new char [nlen];
			l->GetName(&nlen, n);
			if (strcmp(n, name) == 0)
				{
				delete n;
				RELEASE(la);
				return l;
				}
			delete n;
			}
		}
	RELEASE(la);
	return WXRMLight();
	}
int WXRMFrame::NLight()
	{
	if (!PFrame()) return 0;
	IDirect3DRMLightArray* la=NULL;
	if (CKERR(PFrame()->GetLights(&la))) return 0;
	int rtv = la->GetSize();
	RELEASE(la);
	return rtv;
	}

void WXRMFrame::Add(WXRMVisual& vis)
	{
	Create();
	vis.Create();
	CKERR(PFrame()->AddVisual(vis.PVisual()));
	}
void WXRMFrame::Add(const WXRMVisual& vis)
	{
	WXRMVisual v = vis;
	Add(v);
	}
void WXRMFrame::Del(const WXRMVisual& vis)
	{
	if (!vis.PVisual()) return;
	if (!PFrame()) return;
	CKERR(PFrame()->DeleteVisual(vis.PVisual()));
	}
WXRMVisual WXRMFrame::Visual(int id)
	{
	IDirect3DRMVisualArray* va=NULL;
#if DIRECTDRAW_VERSION >= 0x0600	
	if (CKERR(PFrame()->GetVisuals(NULL, (IUnknown**)&va))) return WXRMVisual();
#else
	if (CKERR(PFrame()->GetVisuals(&va))) return WXRMVisual();
#endif
	IDirect3DRMVisual* v=NULL;
	if (!CKERR(va->GetElement(id, &v)) && v)
		{
		int ref = v->Release();
		TRACEALL("WXRMFrame::Visual ref = %d\n", ref);
		}
	RELEASE(va);
	return v;
	}
WXRMVisual WXRMFrame::Visual(const char* name)
	{
	IDirect3DRMVisualArray* va=NULL;
#if DIRECTDRAW_VERSION >= 0x0600	
	if (CKERR(PFrame()->GetVisuals(NULL, (IUnknown**)&va))) return WXRMVisual();
#else
	if (CKERR(PFrame()->GetVisuals(&va))) return WXRMVisual();
#endif
	for(unsigned int i=0; i<va->GetSize(); i++)
		{
		IDirect3DRMVisual* v=NULL;
		if (!CKERR(va->GetElement(i, &v)) && v)
			{
			int ref = v->Release();
			TRACEALL("WXRMFrame::Visual ref = %d\n", ref);
			DWORD nlen=0;
			v->GetName(&nlen, NULL);
			char* n = new char [nlen];
			v->GetName(&nlen, n);
			if (strcmp(n, name) == 0)
				{
				delete n;
				RELEASE(va);
				return v;
				}
			delete n;
			}
		}
	RELEASE(va);
	return WXRMVisual();
	}
int WXRMFrame::NVisual()
	{
	if (!PFrame()) return 0;
	IDirect3DRMVisualArray* va=NULL;
#if DIRECTDRAW_VERSION >= 0x0600	
	if (CKERR(PFrame()->GetVisuals(NULL, (IUnknown**)&va))) return 0;
#else
	if (CKERR(PFrame()->GetVisuals(&va))) return 0;
#endif
	int rtv = va->GetSize();
	RELEASE(va);
	return rtv;
	}
void WXRMFrame::Clear()
	{
	Create();
	int i;
	for(i=NChild()-1; i>=0; i--) Del(Child(i));
	for(i=NVisual()-1; i>=0; i--) Del(Visual(i));
	for(i=NLight()-1; i>=0; i--) Del((IDirect3DRMLight*)Light(i));
	}
REAL WXRMFrame::RotAngle()
	{
	Create();
	D3DVECTOR v;
	REAL r;
	CKERR(PFrame()->GetRotation(Parent(), &v, &r));
	return r;
	}
WXD3Vec3 WXRMFrame::RotAxis()
	{
	Create();
	D3DVECTOR v;
	REAL r;
	CKERR(PFrame()->GetRotation(Parent(), &v, &r));
	return (WXD3Vec3&)v;
	}
void WXRMFrame::Rot(const WXD3Vec3& axis, D3DVALUE rVel)
	{
	Create();
	CKERR(PFrame()->SetRotation(Parent(), axis.X(), axis.Y(), axis.Z(), rVel));
	}
WXD3Vec3 WXRMFrame::Vel()
	{
	Create();
	D3DVECTOR v;
	BOOL b=false;
	CKERR(PFrame()->GetVelocity(Parent(), &v, b));
	return (WXD3Vec3&)v;
	}
void WXRMFrame::Vel(const WXD3Vec3& vel)
	{
	Create();
	CKERR(PFrame()->SetVelocity(Parent(), vel.X(), vel.Y(), vel.Z(), false));
	}
void WXRMFrame::Ori(const WXD3Vec3& ez, const WXD3Vec3& ey, const WXRMFrame& frRef)
	{
	Create();
	CKERR(PFrame()->SetOrientation(frRef, ez.X(), ez.Y(), ez.Z(), ey.X(), ey.Y(), ey.Z()));
	}
void WXRMFrame::Pos(const WXD3Vec3& v, const WXRMFrame& frRef)
	{
	Create();
	CKERR(PFrame()->SetPosition(frRef, v.X(), v.Y(), v.Z()));
	}
WXD3Affine WXRMFrame::AfParent()
	{
	Create();
    WXD3Affine af;
#if DIRECTDRAW_VERSION >= 0x0600	
	CKERR(PFrame()->GetTransform(Parent(), af));
#else
	CKERR(PFrame()->GetTransform(af));
#endif
	return af;
	}
WXD3Affine WXRMFrame::AfWorld()
	{
	Create();
	if (Parent()) return Parent().AfWorld() * AfParent();
	return AfParent();
	}
void WXRMFrame::AfWorld(const WXD3Affine& afW)
	{
	Create();
	AfParent(WXRMFrame(Parent()).AfWorld().inv() * afW);
	}
void WXRMFrame::AfParent(const WXD3Affine& af)
	{
	Create();
    D3DRMMATRIX4D& m = (D3DRMMATRIX4D&)af;
	CKERR(PFrame()->AddTransform(D3DRMCOMBINE_REPLACE, m));
	}
const IID& WXRMFrame::InterfaceID() const
	{
	return IID_IDirect3DRMFrame;
	}
const CLSID& WXRMFrame::ClassID() const
	{
	return CLSID_CDirect3DRMFrame;
	}
}	//	namespace LVRM
