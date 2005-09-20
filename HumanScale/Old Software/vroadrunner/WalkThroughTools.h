#ifndef WALKTHROUGHTOOLS_H
#define WALKTHROUGHTOOLS_H
#include <Base/Affine.h>
#include <vector>
using namespace LVRM;

class TReflectNormalCalculator{
	unsigned int idxs[1024];
public:
	std::vector<Vec3d> normals;
	TReflectNormalCalculator();
	Vec3f Calculate(Vec3f vel);
	void Clear();
	void Add(Vec3f n);
};
class TPenaltyCanceler{
public:
	struct TPlane{
		Vec3f pos;
		Vec3f normal;
		TPlane(Vec3f p, Vec3f n):pos(p), normal(n){}
	};
	class TPlanes:public std::vector<TPlane>{
	};
	TPlanes planes;
	Vec3f Calculate(Vec3f from, Vec3f n);
	void Add(Vec3f p, Vec3f n){
		planes.push_back(TPlane(p,n));
	}
	void Clear(){
		planes.clear();
	}
};
#endif
