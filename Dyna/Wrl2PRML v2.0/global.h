
#include "CalcPenalty.h"
#include <Base/Affine.h>
using namespace LVRM;

#define NOW_PEN
//#define PAST_FRIC
//#define NON_FRIC

#ifdef PAST_FRIC
	const double FRI_DAM_STF = 0.01f;
#else
	const double FRI_DAM_STF = 0.0001f;
#endif

#ifdef NOW_PEN
	const double TIM_MULT = 9.0f;
	const double PEN_DAM_STF = 0.011f;
#else
	const double TIM_MULT = 8.0f;
	const double PEN_DAM_STF = 0.01f;
#endif

#ifdef NON_FRIC
	const double DYN_MYU = 0.0f;
#else
	const double DYN_MYU = 0.5f;
#endif

Vec3d CalcInterior(TCoords vertices);
Vec3d CalcNormal(Vec3d a, Vec3d b, Vec3d c);
