#include "global.h"

Vec3d CalcInterior(TCoords vertices) {
	Vec3d interior;

	for (TCoords::iterator it=vertices.begin(); it!=vertices.end(); it++)
		interior += *it;
	interior /= vertices.size();
	return interior;
}

Vec3d CalcNormal(Vec3d a, Vec3d b, Vec3d c) {
	Vec3d normal;

	normal = (a^b).unit();
	if (normal * c < 0 )
		return normal;
	else
		return -normal;
}
