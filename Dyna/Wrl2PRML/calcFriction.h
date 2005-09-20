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

class TCalcFriction{
	double myu, k;
public:
	TCalcFriction(){/*myu = 90.0; k = 50.0;}/*/myu = 30.0;}
	Vec3d SprPoint(Vec3d& force, Vec3d& torque);
	void CalcFriction(Vec3d force, Vec3d torque, Vec3d normal, TPenaltyDetector& pc, 
					  TGeoNode* g1, TGeoNode* g2, double dt);
};
	
