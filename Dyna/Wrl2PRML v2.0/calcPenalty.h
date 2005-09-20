#ifndef CALCPENALTY_H
#define CALCPENALTY_H

#if defined(__cplusplus)
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <qhull.h>
#include <mem.h>
#include <qset.h>
#include <geom.h>
#include <merge.h>
#include <poly.h>
#include <io.h>
#include <stat.h>
//char qh_version[] = "qad 2000/8/1";  /* used for error messages 
#if defined(__cplusplus)
}
#endif

#include <Base/Affine.h>
#include <iostream>
#include <vector>

using namespace LVRM;

class TCoords:public std::vector<Vec3d>{
public:
	const Affined& posture;
	TCoords(const Affined& a):posture(a){}
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
	/// 法線ベクトル(外部方向)
	Vec3d normal;

	Vec3d GNormal(Affined posture) { return posture.Rot() *  normal; }
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
	double ratio;		//	内分の比率
	TMidPoint(const TCoords* c=NULL, TFace* f=NULL, int v1=-1, int v2=-1, double r=0.0f){
		coords=c;
		face = f;
		vtx[0]=v1;
		vtx[1]=v2;
		ratio=r;
	}
	Vec3d Coord() const{
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

class TGeoNode;
class TCalcPenalty{
	std::vector<Vec3d> vertexVct;
	TFaces commonArea;
	Vec3d interior;
	TGeoNode* frame1;
	TGeoNode* frame2;
	TPolytope* tope1;
	TPolytope* tope2;
public:
	Vec3d normal;
	//	計算結果
	Vec3d penalty;
	Vec3d penaltyMoment;
	Vec3d velocity;
	Vec3d velocityMoment;
	double area;
	
	TCalcPenalty(Vec3d inter, Vec3d n): interior(inter), normal(n){
		penalty = Vec3d();
		penaltyMoment = Vec3d();
		area = 0.0;
	};
	~TCalcPenalty(){
		commonArea.clear();
		vertexVct.clear();
		interior = Vec3d();
		normal = Vec3d();
		penalty = Vec3d();
		penaltyMoment = Vec3d();
		area = 0.0;
	};
	void Draw();
	void Preprocessor();
	void CalcCommonArea();
	void CalcPenalty(TGeoNode* f1, TPolytope* tope1, TGeoNode* f2, TPolytope* tope2);
	void CalcTriPenaltyVelocity(TFace* face);
	void IntegratePenalty(Vec3d& pen, Vec3d& penMom, Vec3d* coord, double* depth);
	void IntegrateVelocity(Vec3d& intVel, Vec3d& intVelMom, Vec3d vel, Vec3d angVel, Vec3d* coord);
	void OutputQhalf();
	void OutputIntersections(FILE *fp, int format, facetT *facetlist, setT *facets, boolT printall);
	void OutputIntersection(FILE *fp, int format, facetT *facet, boolT printall);
	void OutputCoord(FILE *fp, char *string, pointT* point);
};

class TCalcPenaltySub{
public:
	enum PenaltyType{
		NONE, VERTEX, EDGE, FACE
	} type;
	TCollideInterface poly1;
	TCollideInterface poly2;
	Vec3d normal;
	Vec3d penaltyMoment;
	Vec3d penalty;
	double area;
	
	TCalcPenaltySub(TCoords& c1, TCoords& c2): poly1(c1), poly2(c2), area(0){}
	void Draw();
	void CalcPenalty();
	void CalcTriPenalty(const T2DIsects& tri, TFace* face1, const Vec3d& normal);
	void CalcEdgePenalty(const T2DIsect& p1, const T2DIsect& p2, const Vec3d& normal, double sign);
	void CalcPointPenalty(Vec3d p1, Vec3d p2, const Vec3d& normal);
	void Integrate(Vec3d& f, Vec3d& t, double ra, double rb, Vec3d* coord, double* depth, const Vec3d& normal);
	void FindCommonLine(T2DIsects& commonArea, const TCoords& vtxs1, TFace& edge1, const TCoords& vtxs2, TFace& face2, const Vec3d& n);
	void FindCommonArea(T2DIsects& commonArea, const TCoords& vtxs1, TFace& face1, const TCoords& vtxs2, TFace& face2, const Vec3d& n);
	bool InsideCheck(const Vec3d& p, const Vec3d& q1, const Vec3d& q2, const Vec3d& n);
	bool FindIntersection(double& r1, const Vec3d& p1, const Vec3d&	q1,	double& r2, const Vec3d& p2, const Vec3d& q2, const Vec3d& n);
};

#endif
