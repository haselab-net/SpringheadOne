#ifndef HASE_WXD3D_HWXD3DIM_H
#define HASE_WXD3D_HWXD3DIM_H
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "WXD3Dev.h"
#include "WXD3Dev.h"

#define IMERRMSG(n, msg)		error.DXERRMSG(n, msg)
#define CKIMERR(n)				error.CKDXERR(n)
#define CKIMERRMSG(n,msg)		error.CKDXERRMSG(n, msg)

namespace LVRM {
inline D3DCOLORVALUE WXD3DCV(REAL r, REAL g, REAL b, REAL a=1)
	{
	D3DCOLORVALUE c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
	}
inline D3DVERTEX WXD3DVERTEX(D3DVECTOR pos, D3DVECTOR n, REAL u=0, REAL v=1)
	{
	D3DVERTEX vtx;
	vtx.dvX = pos.x;
	vtx.dvY = pos.y;
	vtx.dvZ = pos.z;
	vtx.dvNX = n.x;
	vtx.dvNY = n.y;
	vtx.dvNZ = n.z;
	vtx.dvTU = u;
	vtx.dvTV = v;
	return vtx;
	}
#define WXD3DInst(op, dat, count)	WXD3DInstFunc_(op, sizeof(dat), count)
#define WXD3DInstN(op, count)		WXD3DInstFunc_(op, 0, count)
inline D3DINSTRUCTION WXD3DInstFunc_(D3DOPCODE op, size_t sz, WORD count)
	{
	D3DINSTRUCTION inst;
	inst.bOpcode = op;
	inst.bSize = sz;
	inst.wCount = count;
	return inst;
	}
inline D3DSTATE WXD3DSTTrans(D3DTRANSFORMSTATETYPE tt, DWORD hMat)
	{
	D3DSTATE st;
	st.dtstTransformStateType = tt;
	st.dwArg[0] = hMat;
	return st;
	}
inline D3DSTATE WXD3DSTLight(D3DLIGHTSTATETYPE lt, DWORD dw)
	{
	D3DSTATE st;
	st.dlstLightStateType = lt;
	st.dwArg[0] = dw;
	return st;
	}
inline D3DSTATE WXD3DSTLight(D3DLIGHTSTATETYPE lt, D3DFOGMODE dw)
	{
	return WXD3DSTLight(lt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTLight(D3DLIGHTSTATETYPE lt, D3DVALUE dv)
	{
	D3DSTATE st;
	st.dlstLightStateType = lt;
	st.dvArg[0] = dv;
	return st;
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, DWORD dw)
	{
	D3DSTATE st;
	st.drstRenderStateType = rt;
	st.dwArg[0] = dw;
	return st;
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DTEXTUREADDRESS dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DFILLMODE dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DSHADEMODE dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DLINEPATTERN dw)
	{
	return WXD3DSTRender(rt, *(DWORD*)&dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DTEXTUREFILTER dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DBLEND dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DTEXTUREBLEND dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DCULL dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DCMPFUNC dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DFOGMODE dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, bool dw)
	{
	return WXD3DSTRender(rt, (DWORD)dw);
	}
inline D3DSTATE WXD3DSTRender(D3DRENDERSTATETYPE rt, D3DVALUE dv)
	{
	D3DSTATE st;
	st.drstRenderStateType = rt;
	st.dvArg[0] = dv;
	return st;
	}
inline D3DPROCESSVERTICES WXD3DProcVtx(DWORD flg, WORD num, WORD start=0, WORD dest=0)
	{
	D3DPROCESSVERTICES pv;
	pv.dwFlags = flg;
	pv.wStart  = start;           /* First source vertex */
	pv.wDest   = dest;
	pv.dwCount = num;
	pv.dwReserved = 0;
	return pv;
	}
inline D3DTRIANGLE WXD3DTRIANGLE(WORD v1, WORD v2, WORD v3, WORD flg)
	{
	D3DTRIANGLE tri;
	tri.v1 = v1;
	tri.v2 = v2;
	tri.v3 = v3;
	tri.wFlags = flg;
	return tri;
	}
class CIMObject:public WXUnknown{
	protected:
	WXD3IM* pIM;
	DWORD handle;
	bool bCreated;

	public:
	CIMObject();
	CIMObject(CIMObject& o);
	CIMObject(IUnknown* o);
	virtual void AfterDevCreate(){}
	virtual void BeforeDevRelease(){handle = NULL;}
	void CheckIM();
	operator DWORD()
		{
		return handle;
		}
	virtual bool Create();
	virtual void Release();
	};
class CIMMaterial:public CIMObject, public D3DMATERIAL
	{
	protected:
	IDirect3DMaterial*& PMaterial() const
		{
		return (IDirect3DMaterial*&) intf;
		}
	virtual void AfterDevCreate();
	public:
	CIMMaterial(IDirect3DMaterial* m=NULL);
	CIMMaterial(const CIMMaterial& m);
	//							拡散					環境				反射			ハイライト		ハイライトの鋭さ
	CIMMaterial(D3DCOLORVALUE diffuse, D3DCOLORVALUE ambient, D3DCOLORVALUE specular, D3DCOLORVALUE emmisive, REAL power);
	//				物体の色	ハイライトの鋭さ
	CIMMaterial(D3DCOLORVALUE c, REAL power);
	//						拡散					環境				反射			ハイライト		ハイライトの鋭さ
	virtual ~CIMMaterial();
	void Set(D3DCOLORVALUE diffuse, D3DCOLORVALUE ambient, D3DCOLORVALUE specular, D3DCOLORVALUE emmisive, REAL power);
	//				物体の色	ハイライトの鋭さ
	void Set(D3DCOLORVALUE c, REAL power);
	bool Set();
	bool Get();
	virtual bool Create();
	void Release();
	operator IDirect3DMaterial* () const
		{
		return (IDirect3DMaterial*) intf;
		}
	operator DWORD ()
		{
		Create();
		return handle;
		}
	};
class CIMMatrix: public CIMObject
	{
	protected:
	void AfterDevCreate();

	public:
//	WXD3Affine mat;
	CIMMatrix(REAL front, REAL back, WXD3Vec2 center=WXD3Vec2(0,0), WXD3Vec2 size=WXD3Vec2(1,1));
	CIMMatrix(const WXD3Affine& af);
	CIMMatrix(const CIMMatrix& m);
	CIMMatrix();
	~CIMMatrix();
	bool Create();
	void Release();
	operator WXD3Affine();
	CIMMatrix operator = (const WXD3Affine& af);
	CIMMatrix operator = (const CIMMatrix& m);
	bool Get();
	bool Set();
	};
class CIMLight:public CIMObject, public D3DLIGHT
	{
	protected:
	IDirect3DLight*& PLight() const
		{
		return (IDirect3DLight*&) intf;
		}
	virtual void AfterDevCreate();
	public:
	CIMLight(const CIMLight& l);
	CIMLight(IDirect3DLight* l=NULL);
	~CIMLight();
	bool Create();
	void Release();
	bool Set();
	bool Get();
	operator IDirect3DLight*() const
		{
		return (IDirect3DLight*)intf;
		}
	};

class CIMExecuteBuffer:public CIMObject
	{
	protected:
	bool bLock;
	BYTE* pCur;
	D3DEXECUTEDATA data;
	D3DEXECUTEBUFFERDESC desc, descOld;
	IDirect3DExecuteBuffer*& PBuf()
		{
		return (IDirect3DExecuteBuffer*&)intf;
		}
	void AfterDevCreate();
	void BeforeDevRelease();

	public:
	CIMExecuteBuffer();
	bool Create();
	void Release();
	operator IDirect3DExecuteBuffer*() const
		{
		return (IDirect3DExecuteBuffer*)intf;
		}
	DWORD Size();
	DWORD Caps();
	void Size(size_t sz);
	void Caps(DWORD flags);
	bool Lock();
	void Unlock();
	void EndVertex();
	void EndInst();
	//	vertex
	D3DVERTEX*& PVertex()
		{
		return (D3DVERTEX*&) pCur;
		}
	//	instruction
	D3DINSTRUCTION*& PInst()
		{
		return (D3DINSTRUCTION*&) pCur;
		}
	//	op code
	D3DPOINT*& PPoint()
		{
		return (D3DPOINT*&) pCur;
		}
	D3DLINE*& PLine()
		{
		return (D3DLINE*&) pCur;
		}
	D3DTRIANGLE*& PTri()
		{
		return (D3DTRIANGLE*&) pCur;
		}
	D3DMATRIXLOAD*& PMatLoad()
		{
		return (D3DMATRIXLOAD*&) pCur;
		}
	D3DMATRIXMULTIPLY*& PMatMulti()
		{
		return (D3DMATRIXMULTIPLY*&) pCur;
		}
	D3DSTATE*& PState()
		{
		return (D3DSTATE*&) pCur;
		}
	D3DPROCESSVERTICES*& PProcVtx()
		{
		return (D3DPROCESSVERTICES*&) pCur;
		}
	D3DTEXTURELOAD*& PTexLoad()
		{
		return (D3DTEXTURELOAD*&) pCur;
		}
	D3DBRANCH*& PBrach()
		{
		return (D3DBRANCH*&) pCur;
		}
	D3DSPAN*& PSpan()
		{
		return (D3DSPAN*&) pCur;
		}
	D3DSTATUS*& PStatus()
		{
		return (D3DSTATUS*&) pCur;
		}
	};
class WXD3IM
	{
	protected:
	static WXD3IM* last;
	WXD3Device* pD3Dev;
	IDirect3DViewport* pView;
	static void CreateCallback(WXDDSurface& surf, void*);
	static void ReleaseCallback(WXDDSurface& surf, void*);
	static void DeleteCallback(WXDDSurface& surf, void*);
	bool UpdateViewport();
	public:
	WXDDError error;
	class CIMObjects:public CDXObjects
		{
		public:
			void Add(CIMObject& o){CDXObjects::Add(o);}
			void Del(CIMObject& o){CDXObjects::Del(o);}
			void AfterDevCreate();
			void BeforeDevRelease();
		}objs;			//	pDDrawの開放前に開放するオブジェクト
	CIMExecuteBuffer exbuf;

	WXD3IM();
	~WXD3IM();
	void Surf(WXD3Device& surf);
	bool Create();
	void Release();
	bool Clear(DWORD mtr);
	bool Execute(DWORD dwClip);
	bool BeginScene();
	bool EndScene();
	
	WXDDraw*				PWXDD(){return pD3Dev->PWXDD();}
	WXD3Device*			PCDev(){return pD3Dev;}
	IDirect3D*			PWXD3D(){return pD3Dev->PWXDD()->pD3D;}
	IDirect3DDevice*	PDev(){return pD3Dev->pD3DDev;}
	IDirect3DViewport*	PView(){return pView;}
	static WXD3IM& GetLastCreate(){return *last;}
	};
}	//	namespace LVRM
#endif
