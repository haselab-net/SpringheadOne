#include <Solid/SOLID.h>
#include <VertexBase.h>
#include <Polyhedron.h>
#include <Object.h>
#include "WalkThroughTools.h"

TReflectNormalCalculator::TReflectNormalCalculator(){
	for(int i=0; i<sizeof(idxs)/sizeof(idxs[0]); ++i) idxs[i] = i;
	normals.push_back(Vec3d());			//	Œ´“_‚ð‰Á‚¦‚Ä‚¨‚­
}
void TReflectNormalCalculator::Add(Vec3f nn){
	Vec3f n = nn * 1000;
	for(std::vector<Vec3d>::iterator it = normals.begin(); it != normals.end(); ++it){
		if ((*it - n).square() < 1E-6) return;
	}
	normals.push_back(n);
}
Vec3f TReflectNormalCalculator::Calculate(Vec3f vel){
	//	”½ŽË–Ê‚Ì–@ü‚ð‹‚ß‚é
	Vec3f normal;	//	”½ŽË–Ê‚Ì–@ü
	if(normals.size() > 2){
		VertexBase vbase((Point*)normals.begin());
		Vec3d reflect = -vel.unit();
		Polyhedron pointPoly((Point*)&reflect, 1, idxs);
		Polyhedron normalPoly(vbase, normals.size(), idxs);
		Transform tr;
		tr.setIdentity();
		Point pa, pb;
		closest_points(normalPoly, pointPoly, tr, tr, pa, pb);
		normal = (Vec3d&)pa;
		if (normal.square() > 0){
			normal = normal.unit();
		}else{
			normal = Vec3f();
		}
	}else if(normals.size() == 2){
		normal = (normals[1]).unit();
	}
	return normal;
}
void TReflectNormalCalculator::Clear(){
	normals.clear();
	normals.push_back(Vec3f());
}
Vec3f TPenaltyCanceler::Calculate(Vec3f from, Vec3f n){
	for(int i=0; i<planes.size(); ++i){
		float d = (planes[i].pos - from) * n;
		if (d > 0){
			from += d * n;
		}
	}
	return from;
}
