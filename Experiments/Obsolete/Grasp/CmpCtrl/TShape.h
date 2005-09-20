#ifndef TSHAPE_H
#define TSHAPE_H
#include <Base/utilities.h>
#include <Base/Affine.h>
#include <vector>

namespace LVRM{

class TShape:public UTRefCount{
protected:
	float friction;
public:
	TShape(){ friction = 1.0f; }
	virtual TShape* CalcProxyPos(Vec3f& newPrx, Vec3f& normal, const Vec3f& prx, const Vec3f& dev)=0;
	virtual void Draw()=0;
	float Friction() { return friction; }
};
class TPlane:public TShape{
public:
	virtual TShape* CalcProxyPos(Vec3f& newPrx, Vec3f& normal, const Vec3f& prx, const Vec3f& dev);
	void Draw(){}
};
class TConvexPolygon:public TShape{
protected:
	struct TVtx{
		Vec3f pos;
		Vec3f normal;
	};
	typedef std::vector<TVtx> TVtxs;
	TVtxs vtxs;
	Vec3f normal;
public:
	void Draw();
	void Clear(){ vtxs.clear(); }
	virtual TShape* CalcProxyPos(Vec3f& newPrx, Vec3f& normalRet, const Vec3f& prx, const Vec3f& dev);
	void AddVertex(Vec3f p){
		TVtx vtx;
		vtx.pos = p;
		vtxs.push_back(vtx);
	}
	void Calc(){
		if (vtxs.size() < 3) return;
		normal = ((vtxs[1].pos - vtxs[0].pos) ^ (vtxs[2].pos - vtxs[0].pos)).unit();
		int i=0;
		for(; i<vtxs.size()-1; ++i){
			vtxs[i].normal = normal ^ (vtxs[i+1].pos - vtxs[i].pos);
		}
		vtxs[i].normal = (normal ^ (vtxs[0].pos - vtxs[i].pos)).unit();
	}
};
class TCube:public TShape{
	TConvexPolygon faces[6];
	Vec3f extent;
public:
	void Draw(){
		for(int i=0; i<6; ++i){
			faces[i].Draw();
		}
	}
	Vec3f Extent(){ return extent; }
	void Extent(Vec3f e){
		extent = e;
		MakeFace(0, 0, 1, 2);
		MakeFace(2, 1, 2, 0);
		MakeFace(4, 2, 0, 1);
	}
	virtual TShape* CalcProxyPos(Vec3f& newPrx, Vec3f& normal,  const Vec3f& prx, const Vec3f& dev){
		for(int i=0; i<6; i++){
			TShape* rv = faces[i].CalcProxyPos(newPrx, normal, prx, dev);
			if (rv) return rv;
		}
		return NULL;
	}
protected:
	void MakeFace(int f, int x, int y, int z){
		Vec3f vtx;
		faces[f].Clear();
		faces[f].AddVertex(vtx);
		vtx[y] = extent[y];
		faces[f].AddVertex(vtx);
		vtx[x] = extent[x];
		faces[f].AddVertex(vtx);
		vtx[y] = 0;
		faces[f].AddVertex(vtx);
		faces[f].Calc();

		f++;
		faces[f].Clear();
		vtx=Vec3f(0,0,0);
		vtx[z] = extent[z];
		faces[f].AddVertex(vtx);
		vtx[x] = extent[x];
		faces[f].AddVertex(vtx);
		vtx[y] = extent[y];
		faces[f].AddVertex(vtx);
		vtx[x] = 0;
		faces[f].AddVertex(vtx);
		faces[f].Calc();
	}
};

}
#endif
