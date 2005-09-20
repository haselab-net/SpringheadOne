#ifndef CALCPENALTY_H
#define CALCPENALTY_H
#include <Base/Affine.h>
#include <iostream>
#include <vector>
using namespace LVRM;

class TFace:public std::vector<int>{
public:
};
class TFaces:public std::vector<TFace>{
public:
};
class TCoords:public std::vector<Vec3f>{
public:
	char* id;
	Affinef af;
	TCoords(){ id = "a"; }
};
class TPolygons{
public:
	TCoords vtxs;
	TFaces faces;
	void Draw();
};

class TMidPoint{
public:
	TCoords* coords;	//	頂点配列へのポインタ
	TFace* face;		//	辺が属する面
	int vtx[2];			//	頂点の番号
	float ratio;		//	内分の比率
	TMidPoint(TCoords* c=NULL, TFace* f=NULL, int v1=-1, int v2=-1, float r=0.0f){
		coords=c;
		face = f;
		vtx[0]=v1;
		vtx[1]=v2;
		ratio=r;
	}
	Vec3f Coord() const{
		if (ratio == 1.0f) return (*coords)[vtx[0]];
		return ratio * (*coords)[vtx[0]] + (1.0f-ratio) * (*coords)[vtx[1]];
	}
	operator bool() const{
		return vtx[0] != -1;
	}
	void Print(std::ostream& os) const ;
};
inline bool operator == (const TMidPoint& a, const TMidPoint& b){
	if (a.face != b.face) return false;
	if (a.vtx[0] == b.vtx[0] && a.vtx[1] == b.vtx[1] && a.ratio == b.ratio) return true;
	if (a.vtx[0] == b.vtx[1] && a.vtx[1] == b.vtx[0] && a.ratio == (1.0f-b.ratio)) return true;
	return false;
}
inline std::ostream& operator << (std::ostream& os, const TMidPoint& i){ i.Print(os); return os; }

class T2DIsect{
public:
	TMidPoint mp[2];
	bool bUsed;
	T2DIsect(TMidPoint& p1, TMidPoint& p2){
		mp[0]=p1;
		mp[1]=p2;
		bUsed = false;
	}
	void Print(std::ostream& os) const {
		os << mp[0] << ", ";
		os << mp[1] << std::endl;
	}
};
inline std::ostream& operator << (std::ostream& os, const T2DIsect& i){ i.Print(os); return os; }

class T2DIsects:public std::vector<T2DIsect>{
public:
	void Print(std::ostream& os) const {
		for(const_iterator it = begin(); it != end(); ++it){
			it->Print(os);
		}
	}
};
inline std::ostream& operator << (std::ostream& os, const T2DIsects& i){ i.Print(os); return os; }
class TCalcPenalty{
public:
	TPolygons polys[2];
	Vec3f normal;
	Vec3f torque;
	Vec3f force;
	
	void Draw();
	void CalcPenalty();
	void CalcTriPenalty(Vec3f& force, Vec3f& torque, const T2DIsects& tri, TFace* face1, const Vec3f& normal);
	void CalcForceTorque(Vec3f& f, Vec3f& t, float ra, float rb, Vec3f* coord, float* depth, const Vec3f& normal);
	void FindCommonArea(T2DIsects& commonArea, TCoords& vtxs1, TFace& face1, TCoords& vtxs2, TFace& face2, const Vec3f& n);
	bool InsideCheck(const Vec3f& p, const Vec3f& q1, const Vec3f& q2, const Vec3f& n);
	bool FindIntersection(float& r1, const Vec3f& p1, const Vec3f&	q1,	float& r2, const Vec3f& p2, const Vec3f& q2, const Vec3f& n);
};

#endif
