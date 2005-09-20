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
	Vec3f SprPoint(Vec3f& force, Vec3f& torque);
	void CalcFriction(Vec3f force, Vec3f torque, Vec3f normal, TPenaltyDetector& pc, 
					  TGeoNode* g1, TGeoNode* g2, float dt);
};
	
