#include <Base/Env.h>

#include <Solid/SOLID.h>

#include <strstream>
#include <float.h>
#include <Base/Affine.h>

#include "CalcPenalty.h"
#include "GeoNode.h"
#include "GLView.h"

using namespace std;
using namespace LVRM;

class TCalcCollisionMotion{
//	double myu;
	double k;
public:
	TCalcCollisionMotion(){
//		myu = 0.0f;
	}
	Vec3d CalcApplicationPoint(Vec3d force, Vec3d torque, Vec3d normal, Vec3d interior);
	void CalcCollisionMotion(Vec3d pen, Vec3d penMom, Vec3d vel, Vec3d velMom,
		Vec3d normal, Vec3d interior, TPenaltyDetector& pc, TGeoNode* g1, TGeoNode* g2, double dt);
};
	
