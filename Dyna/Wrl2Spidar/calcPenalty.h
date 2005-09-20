#ifndef CALCPENALTY_H
#define CALCPENALTY_H
#include <Base/Affine.h>
#include <iostream>
#include <vector>
using namespace LVRM;

class TCoords:public std::vector<Vec3d>{
public:
	const Affinef& posture;
	TCoords(const Affinef& a):posture(a){}
	TCoords(const TCoords& c):posture(c.posture), std::vector<Vec3d>(c){};
	TCoords& operator=(const TCoords& c){
		TCoords::~TCoords();
		return *new(this) TCoords(c);
	}
	Vec3d GCoord(int id) const { return posture * begin()[id]; }
	Vec3d& LCoord(int id){ return begin()[id]; }
	void Print(std::ostream& os) const;
};
inline std::ostream& operator << (std::ostream& os, const TCoords& i){ i.Print(os); return os; }

class TFace:public std::vector<int>{
public:
	void Print(std::ostream& os) const;
};
inline std::ostream& operator << (std::ostream& os, const TFace& f){ f.Print(os); return os; }
class TFaces:public std::vector<TFace>{
public:
};
class Convex;
class TPolytope: public TFaces{
public:
	const TCoords& vertices;
	Convex* convex;
	TPolytope(TCoords& v):vertices(v){ convex = NULL; }
	TPolytope(const TPolytope& p):vertices(p.vertices), convex(p.convex){}
	TPolytope& operator = (const TPolytope& p){
		TPolytope::~TPolytope();
		return *new(this) TPolytope(p);
	}
	void Draw();
	void Print(std::ostream& os) const;
};
inline std::ostream& operator << (std::ostream& os, const TPolytope& f){ f.Print(os); return os; }
class TPolytopes: public std::vector<TPolytope>{
};
//	衝突の境界面の状態を表す．
class TCollideInterface: public TPolytope{
public:
	enum State{NONE, FACE, EDGE, VERTEX} state;
	Vec3d point;
	TCollideInterface(TCoords& v):TPolytope(v){ state = NONE; }
	TCollideInterface(const TPolytope& p):TPolytope(p){ state = NONE; }
};
class TMidPoint{
public:
	const TCoords* coords;	//	頂点配列へのポインタ
	TFace* face;		//	辺が属する面
	int vtx[2];			//	頂点の番号
	float ratio;		//	内分の比率
	TMidPoint(const TCoords* c=NULL, TFace* f=NULL, int v1=-1, int v2=-1, float r=0.0f){
		coords=c;
		face = f;
		vtx[0]=v1;
		vtx[1]=v2;
		ratio=r;
	}
	Vec3f Coord() const{
		if (ratio == 1.0f) return (*coords)[(*face)[vtx[0]]];
		return ratio * (*coords)[(*face)[vtx[0]]] + (1.0f-ratio) * (*coords)[(*face)[vtx[1]]];
	}
	operator bool() const{
		return vtx[0] != -1;
	}
	void Print(std::ostream& os) const ;
};
inline bool SameLine(TMidPoint& a, const TMidPoint& b){
	if (a.face != b.face) return false;
	if (a.vtx[0] == b.vtx[0] && a.vtx[1] == b.vtx[1]) return true;
	if (a.vtx[0] == b.vtx[1] && a.vtx[1] == b.vtx[0]) return true;
	return false;
}
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
	enum PenaltyType{
		NONE, VERTEX, EDGE, FACE
	} type;
	TCollideInterface poly1;
	TCollideInterface poly2;
	Vec3f normal;
	Vec3f torque;
	Vec3f force;
	float area;
	
	TCalcPenalty(TCoords& c1, TCoords& c2): poly1(c1), poly2(c2), area(0){}
	void Draw();
	void CalcPenalty();
	void CalcTriPenalty(const T2DIsects& tri, TFace* face1, const Vec3f& normal);
	void CalcEdgePenalty(const T2DIsect& p1, const T2DIsect& p2, const Vec3f& normal, float sign);
	void CalcPointPenalty(Vec3d p1, Vec3d p2, const Vec3f& normal);
	void CalcForceTorque(Vec3f& f, Vec3f& t, float ra, float rb, Vec3f* coord, float* depth, const Vec3f& normal);
	void FindCommonLine(T2DIsects& commonArea, const TCoords& vtxs1, TFace& edge1, const TCoords& vtxs2, TFace& face2, const Vec3f& n);
	void FindCommonArea(T2DIsects& commonArea, const TCoords& vtxs1, TFace& face1, const TCoords& vtxs2, TFace& face2, const Vec3f& n);
	bool InsideCheck(const Vec3f& p, const Vec3f& q1, const Vec3f& q2, const Vec3f& n);
	bool FindIntersection(float& r1, const Vec3f& p1, const Vec3f&	q1,	float& r2, const Vec3f& p2, const Vec3f& q2, const Vec3f& n);
};

#endif
