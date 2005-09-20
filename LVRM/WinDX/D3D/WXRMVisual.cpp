#include "WXRM.h"
#include "WXRMVisual.h"
#include <strstream>
#pragma hdrstop
using namespace std;

namespace LVRM {
//-----------------------------------------------------------------------------
//	WXRMVisual
//
/*
bool WXRMVisual::IsExtend()
	{
	if (!PVisual()) return false;
	WXRMColMesh* pExt = (WXRMColMesh*)(WXRMObjExt::GetExt(PVisual()));
	return pExt != NULL;
	}
CColVisual& WXRMVisual::Ext()
	{
	CColVisual* pExt = (CColVisual*)(WXRMObjExt::GetExt(PVisual()));
	if (!pExt)
		{
		if (InterfaceID() == IID_IDirect3DRMMeshBuilder)
			pExt = new WXRMColMesh;
		else pExt = new WXRMColSphere;
		pExt->Set(PVisual());
		}
	return *pExt;
	}
void WXRMVisual::Ext(CColVisual* v)
	{
	v->Set(PVisual());
	}
*/
//-----------------------------------------------------------------------------
//	WXRMLight
//
WXRMLight::WXRMLight()
	{
	}
WXRMLight::WXRMLight(IDirect3DRMLight* l):
	WXRMObject(l)
	{
	}
WXRMLight::WXRMLight(const WXRMLight& l):
	WXRMObject(l)
	{
	}
WXRMLight::WXRMLight(D3DRMLIGHTTYPE lt, D3DCOLOR col)
	{
	Set(lt, col);
	}
void WXRMLight::Set(D3DRMLIGHTTYPE lt, D3DCOLOR col)
	{
	BeforeCreate();
	RELEASE(PLight());
	CKRMERR(WXRM::pD3DRM->CreateLight(lt, col, &PLight()));
	AfterCreate();
	}

//-----------------------------------------------------------------------------
//	WXRMTexture
//
WXRMTexture::WXRMTexture(const std::string fname, bool bTransp, D3DCOLOR colTransp, Vec2f decaleSize, Vec2f decaleOrigin)
	{
	Load(fname, bTransp, colTransp, decaleSize, decaleOrigin);
	}
bool WXRMTexture::Load(std::string fname, bool bTransp, D3DCOLOR colTransp, Vec2f decaleSize, Vec2f decaleOrigin)
	{
	Release();
	BeforeCreate();
	HRESULT hr = WXRM::pD3DRM->LoadTexture(fname.c_str(), &PTex());
	AfterCreate();
	if (hr != D3DRM_OK)
		{
		char buf[1024];
		ostrstream(buf, sizeof(buf)) << "Failed to load \'" << fname.c_str() << "\'.\n" << '\0';
		CKRMERRMSG(hr, buf);
		goto generic_error;
		}
	if (CKRMERR(PTex()->SetColors(16))) goto generic_error;
	if (CKRMERR(PTex()->SetShades(8))) goto generic_error;
	if (CKRMERR(PTex()->SetDecalTransparency(bTransp))) goto generic_error;
	if (CKRMERR(PTex()->SetDecalTransparentColor(colTransp))) goto generic_error;
	if (CKRMERR(PTex()->SetDecalSize(decaleSize.X(), decaleSize.Y()))) goto generic_error;
	if (CKRMERR(PTex()->SetDecalOrigin(decaleSize.X(), decaleSize.Y()))) goto generic_error;
	return true;
generic_error:
	Release();
	return false;
	}
//-----------------------------------------------------------------------------
//	WXRMMaterial
//
WXRMMaterial::WXRMMaterial(const WXD3Vec3& em, const WXD3Vec3& sp, REAL pw)
	{
	Set(em, sp, pw);
	}
void WXRMMaterial::Set(const WXD3Vec3& em, const WXD3Vec3& sp, REAL pw)
	{
	Release();
	BeforeCreate();
	CKRMERR(WXRM::pD3DRM->CreateMaterial(pw, &PMaterial()));
	AfterCreate();
	CKRMERR(PMaterial()->SetEmissive(em.X(), em.Y(), em.Z()));
	CKRMERR(PMaterial()->SetSpecular(sp.X(), sp.Y(), sp.Z()));
	}
const IID& WXRMMaterial::InterfaceID() const
	{
	return IID_IDirect3DRMMaterial;
	}
const CLSID& WXRMMaterial::ClassID() const
	{
	return CLSID_CDirect3DRMMaterial;
	}

//-----------------------------------------------------------------------------
//	WXRMMesh
//
WXRMMesh::WXRMMesh(const WXRMMesh& m):
	WXRMVisual(m)
	{
	vertices.pCMesh = this;
	faces.pCMesh = this;
	}
WXRMMesh::WXRMMesh(intf_type* m):
	WXRMVisual(m)
	{
	vertices.pCMesh = this;
	faces.pCMesh = this;
	}
WXRMMesh::WXRMMesh()
	{
	vertices.pCMesh = this;
	faces.pCMesh = this;
	}
WXRMMesh::WXRMMesh(const std::string fname):
	WXRMVisual()
	{
	vertices.pCMesh = this;
	faces.pCMesh = this;
	Load(fname);
	}
WXRMMesh::~WXRMMesh()
	{
	}
bool WXRMMesh::CheckCreate()
	{
	if (!PMeshB())
		{
		BeforeCreate();
		if (CKRMERR(WXRM::pD3DRM->CreateMeshBuilder(&PMeshB()))) return false;
		AfterCreate();
		}
	return true;
	}
bool WXRMMesh::Load(const std::string fname)
	{
	if (!CheckCreate()) return false;
	HRESULT hr = PMeshB()->Load((void*)fname.c_str(), NULL, D3DRMLOAD_FROMFILE, NULL, NULL);
	if (hr != D3DRM_OK)
		{
		char buf[1024];
		ostrstream(buf, sizeof(buf)) << "Failed to load \'" << fname.c_str() << "\'.\n" << '\0';
		CKRMERRMSG(hr, buf);
		return false;
		}
	return true;
	}
bool WXRMMesh::Save(const std::string fname, D3DRMXOFFORMAT fmt, D3DRMSAVEOPTIONS opt)
	{
	if (!CheckCreate()) return false;
	HRESULT hr = PMeshB()->Save(fname.c_str(), fmt, opt);
	if (hr != D3DRM_OK)
		{
		char buf[1024];
		ostrstream(buf, sizeof(buf)) << "Failed to save \'" << fname.c_str() << "\'.\n" << '\0';
		CKRMERRMSG(hr, buf);
		return false;
		}
	return true;
	}
bool WXRMMesh::Set(const WXRMTexture& tex, D3DRMWRAPTYPE wt, const WXD3Affine& af, Vec2f texO, Vec2f texS)
	{
	if (!CheckCreate()) return false;
	IDirect3DRMWrap* pWrap = NULL;
    WXD3Vec3 pos = af.Pos();
	if (CKRMERR(WXRM::pD3DRM->CreateWrap(wt, NULL,
		pos.X(), pos.Y(), pos.Z(),
		af.Ez().X(), af.Ez().Y(), af.Ez().Z(),
		af.Ey().X(), af.Ey().Y(), af.Ey().Z(),
		texO.X(), texO.Y(),
		texS.X(), texS.Y(), &pWrap)))
		{
		goto generic_error;
		}
	if (CKRMERR(pWrap->Apply((IDirect3DRMObject*)PMeshB())))
		goto generic_error;
	if (CKRMERR(PMeshB()->SetTexture(tex)))
		goto generic_error;
	RELEASE(pWrap);
	return true;
generic_error:
	RELEASE(pWrap);
	return false;
	}
#define TEXGAPMAG REAL(1.005)
bool WXRMMesh::CalcWrap(WXD3Affine& af, Vec2f& texS, D3DRMWRAPTYPE wt, char axis1, char axis2)
	{
	if (!CheckCreate()) return false;
	D3DRMBOX box;
	if (CKRMERR(PMeshB()->GetBox(&box)))
		return false;
	WXD3Vec3 ex(box.max.x - box.min.x, 0, 0);
	WXD3Vec3 ey(0, box.max.y - box.min.y, 0);
	WXD3Vec3 ez(0, 0, box.max.z - box.min.z);
	WXD3Vec3 pos;
	pos.X() = box.min.x;
	pos.Y() = box.min.y;
	pos.Z() = box.min.z;
	WXD3Vec3 a, b, c;
	axis1 = (char)toupper(axis1);
	axis2 = (char)toupper(axis2);
	int a1 = axis1-'X';
	int a2;
	if ('X' <= axis2 && axis2 <= 'Z' && axis1 != axis2) a2 = axis2-'X';
	else if (axis2 == '-') a2 = (a1-1+3)%3;
	else a2 = (a1+1)%3;
	int a3 = (a1+1)%3;
	if (a3==a2) a3=(a2+1)%3;
	a = a1==0 ? ex : (a1==1 ? ey : ez);
	b = a2==0 ? ex : (a2==1 ? ey : ez);
	c = a3==0 ? ex : (a3==1 ? ey : ez);
	if (wt == D3DRMWRAP_FLAT)
		{
		af.Ex() = c.unit();
		af.Ey() = b.unit();
		af.Ez() = a.unit();
        if (c.norm()>0) texS.X() = 1/c.norm()*TEXGAPMAG;
		if (b.norm()>0) texS.Y() = 1/b.norm()*TEXGAPMAG;
		}
	else if (wt == D3DRMWRAP_CYLINDER)
		{
		af.Ex() = b.unit();
		af.Ey() = c.unit();
		af.Ez() = a.unit();
		texS.X() = 1;
		if (a.norm()>0) texS.Y() = 1/(a.norm()*TEXGAPMAG);
		pos += b/2 + c/2;
		}
	else if (wt == D3DRMWRAP_SPHERE)
		{
		af.Ex() = b.unit();
		af.Ey() = c.unit();
		af.Ez() = a.unit();
		texS=Vec2f(1,1);
		pos += a/2 + b/2 + c*TEXGAPMAG/2; 
		}
    if (af.det() == 0) af = WXD3Affine();
    af.Pos() = af.Rot().inv() * pos;
	return true;
	}
bool WXRMMesh::Set(const WXRMTexture& tex, D3DRMWRAPTYPE wt, char axis1, char axis2, Vec2f texO)
	{
	WXD3Affine af;
	Vec2f texS;
	if (!CalcWrap(af, texS, wt, axis1, axis2)) return false;
	return Set(tex, wt, af, texO, texS);
	}
bool WXRMMesh::Set(D3DCOLOR col)
	{
	if (!CheckCreate()) return false;
	if (CKRMERR(PMeshB()->SetColor(col))) return false;
	return true;
	}
bool WXRMMesh::Set(D3DRMCOLORSOURCE colSrc)
	{
	if (!CheckCreate()) return false;
	if (CKRMERR(PMeshB()->SetColorSource(colSrc))) return false;
	return true;
	}
bool WXRMMesh::Set(WXRMMaterial::intf_type* m){
	if (!CheckCreate()) return false;
	if (CKRMERR(PMeshB()->SetMaterial(m))) return false;
	return true;
}
bool WXRMMesh::Scale(const WXD3Vec3& s)
	{
	if (!CheckCreate()) return false;
	bool rtv = CKRMERR(PMeshB()->Scale(s.X(), s.Y(), s.Z()));
	vertices.Release();
	UpdateMesh();
	return rtv;
	}
bool WXRMMesh::Pos(const WXD3Vec3& s)
	{
	if (!CheckCreate()) return false;
	bool rtv = CKRMERR(PMeshB()->Translate(s.X(), s.Y(), s.Z()));
	vertices.Release();
	UpdateMesh();
	return rtv;
	}
bool WXRMMesh::GetBox(WXD3Vec3& min, WXD3Vec3& max)
	{
	if (!CheckCreate()) return false;
	D3DRMBOX box;
	bool rtv = CKRMERR(PMeshB()->GetBox(&box));
	min.X() = box.min.x;
	min.Y() = box.min.y;
	min.Z() = box.min.z;
	max.X() = box.max.x;
	max.Y() = box.max.y;
	max.Z() = box.max.z;
	return rtv;
	}
bool WXRMMesh::Add(const WXRMFace& face)
	{
	if (!CheckCreate()) return false;
	VRASSERT(face.pFace);
	bool rv;
	if (face.pCMesh)
		{
		WXRMFace::intf_type* f=NULL;
		GUID guid;
		face.pFace->Clone(NULL, guid, (void**)&f);
		rv = !CKRMERR(PMeshB()->AddFace(f));
		if (!rv) RELEASE(f);
		}
	rv = !CKRMERR(PMeshB()->AddFace(face.pFace));
	return rv;
	}
int WXRMMesh::Add(const WXRMVertex& vtx)
	{
	if (!CheckCreate()) return false;
	int idP = PMeshB()->AddVertex(vtx.Pos().X(), vtx.Pos().Y(), vtx.Pos().Z());
	int idN = PMeshB()->AddNormal(vtx.Normal().X(), vtx.Normal().Y(), vtx.Normal().Z());
	vertices.Release();
	UpdateMesh();
	VRASSERT(idP == idN);
	return idP;
	}
const IID& WXRMMesh::InterfaceID() const
	{
	return IID_IDirect3DRMMeshBuilder;
	}
const IID& WXRMMesh::ClassID() const
	{
	return CLSID_CDirect3DRMMeshBuilder;
	}
//
//	WXRMFace
//
WXRMVertex WXRMFace::Vertices::operator[](UINT id)
	{
	VRASSERT(id < Size());
	VRASSERT(face->pCMesh);
	face->pCMesh->CheckCreate();
	int idMesh = face->pFace->GetVertexIndex(id);
	return face->pCMesh->vertices[idMesh];
	}
UINT WXRMFace::Vertices::Size()
	{
	return face->pFace->GetVertexCount();
	}

WXRMFace::WXRMFace(WXRMFace& f)
	{
	pFace = f.pFace;
	f.pFace = NULL;
	vertices.face = this;
	}
WXRMFace::WXRMFace()
	{
	pCMesh = NULL;
	pFace = NULL;
	WXRM::pD3DRM->CreateFace(&pFace);
	vertices.face = this;
	}

WXRMFace::WXRMFace(WXRMMesh& m, intf_type* pf){
	pCMesh = &m;
	pFace = pf;
	vertices.face = this;
}
WXRMFace::~WXRMFace()
	{
	RELEASE(pFace);
	}
bool WXRMFace::Set(D3DCOLOR col)
	{
	return CKRMERR(pFace->SetColor(col));
	}
bool WXRMFace::Set(WXRMMaterial::intf_type* m){
	return CKRMERR(pFace->SetMaterial(m));
}
bool WXRMFace::Set(WXRMTexture::intf_type* t){
	return CKRMERR(pFace->SetTexture(t));
}
bool WXRMFace::Add(const WXRMVertex& vtx){
	VRASSERT(pCMesh);		//	æ‚É WXRMMesh::Add(WXRMFace& f) ‚ÅƒƒbƒVƒ…‚É‰Á‚¦‚Ä‚­‚¾‚³‚¢B
	int id = pCMesh->Add(vtx);
	return Add(id);
}
bool WXRMFace::Add(int id){
	return CKRMERR(pFace->AddVertexAndNormalIndexed(id, id));
}
//
//	WXRMFaces
//
WXRMFaces::WXRMFaces()
	{
	pFaces = NULL;
	}
WXRMFaces::~WXRMFaces()
	{
	RELEASE(pFaces);
	}
void WXRMFaces::Create()
	{
	if (pFaces && pFaces->GetSize() < Size()) RELEASE(pFaces);
	pCMesh->CheckCreate();
	if (!pFaces) pCMesh->PMeshB()->GetFaces(&pFaces);
	VRASSERT(pFaces);
	}
WXRMFace WXRMFaces::operator [](UINT id){
	Create();
	VRASSERT(id <pFaces->GetSize());
	IDirect3DRMFace* pFace=NULL;
	pFaces->GetElement(id, &pFace);
	pFace->Release();
	WXRMFace::intf_type* pf;
	pFace->QueryInterface(WXRMFace::InterfaceID(), (void**)&pf);
	return WXRMFace(*pCMesh, pf);
}
UINT WXRMFaces::Size(){
	pCMesh->CheckCreate();
	return pCMesh->PMeshB()->GetFaceCount();
}
void WXRMFaces::Size(UINT s){
	while(Size() < s) Add();
}
bool WXRMFaces::Add(const WXRMFace& face)
	{
	return pCMesh->Add(face);
	}
//-----------------------------------------------------------------------------
//	WXRMVertex
//
WXRMVertex::WXRMVertex(const WXD3Vec3& p, const WXD3Vec3& n)
	{
	id = -1;
	vertices = NULL;
	pos = (D3DVECTOR&)p;
	normal = (D3DVECTOR&)n;
	}
WXRMVertex::WXRMVertex(const WXRMVertex& v)
	{
	id = v.id;
	vertices = v.vertices;
	pos = v.pos;
	normal = v.normal;
	}
const WXRMVertex WXRMVertex::operator = (const WXRMVertex& v)
	{
	pos = v.pos;
	normal = v.normal;
	if (vertices)
		{
		vertices->SetNormal(id, normal);
		vertices->SetPos(id, pos);
		}
	return *this;
	}
void WXRMVertex::Pos(const WXD3Vec3& p)
	{
	pos = (D3DVECTOR&)p;
	if (vertices) vertices->SetPos(id, pos);
	}
void WXRMVertex::Normal(const WXD3Vec3& n)
	{
	normal = (D3DVECTOR&)n;
	if (vertices) vertices->SetNormal(id, normal);
	}
bool WXRMVertex::Set(D3DCOLOR col)
	{
	if (!vertices) return false;
	vertices->pCMesh->CheckCreate();
	return CKRMERR(vertices->pCMesh->PMeshB()->SetVertexColor(id, col));
	}
//
//	WXRMVertices
//
WXRMVertices::WXRMVertices()
	{
	pPos = NULL;
	pNor = NULL;
	nPos = 0;
	nNor = 0;
	};
WXRMVertices::~WXRMVertices()
	{
	Release();
	}
void WXRMVertices::Create(){
	DWORD fc=0;
	if (!pPos){
		pCMesh->CheckCreate();
#if DIRECTDRAW_VERSION >= 0x0600
		pCMesh->PMeshB()->GetGeometry(&nPos, NULL, &nNor, NULL, &fc, NULL);
#else
		pCMesh->PMeshB()->GetVertices(&nPos, NULL, &nNor, NULL, &fc, NULL);
#endif
		pPos = new D3DVECTOR [nPos];
		pNor = new D3DVECTOR [nNor];
#if DIRECTDRAW_VERSION >= 0x0600
		pCMesh->PMeshB()->GetGeometry(&nPos, pPos, &nNor, pNor, &fc, NULL);
#else
		pCMesh->PMeshB()->GetVertices(&nPos, pPos, &nNor, pNor, &fc, NULL);
#endif
	}
}
void WXRMVertices::Release()
	{
	delete [] pPos;
	pPos = NULL;
	nPos = 0;
	delete [] pNor;
	pNor = NULL;
	nNor = 0;
	}
void WXRMVertices::SetPos(int id, const D3DVECTOR& pos)
	{
	Create();
	VRASSERT(0<=id && id<(int)nPos);
	pPos[id] = pos;
	pCMesh->PMeshB()->SetVertex(id, pos.x, pos.y, pos.z);
	pCMesh->UpdateMesh();
	}
void WXRMVertices::SetNormal(int id, const D3DVECTOR& nor)
	{
	Create();
	VRASSERT(0<=id && id<(int)nNor);
	pNor[id] = nor;
	pCMesh->PMeshB()->SetNormal(id, nor.x, nor.y, nor.z);
	}

WXRMVertex WXRMVertices::operator[](UINT id)
	{
	Create();
	WXRMVertex vtx;
	vtx.vertices = this;
	vtx.id = id;
	vtx.pos = pPos[id];
	vtx.normal = pNor[id];
	return vtx;
	}
UINT WXRMVertices::Size()
	{
	Create();
	return nPos;
	}
void WXRMVertices::Size(UINT s)
	{
	while (Size() < s) Add();
	}
int WXRMVertices::Add(const WXRMVertex& vtx)
	{
	return pCMesh->Add(vtx);
	}

}	//	namespace LVRM
