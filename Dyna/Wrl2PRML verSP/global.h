
#include "CalcPenalty.h"
#include <Base/Affine.h>
using namespace LVRM;

//#define PAST
#ifdef PAST
	const double TIM_MULT = 8.0f;
#endif
#ifndef PAST
	const double TIM_MULT = 8.0f;
#endif
const double PEN_DAM_STF = 0.01f;
const double FRI_DAM_STF = 0.01f;
const double DAM_STF = 0.01f;
const double DYN_MYU = 0.2f;

Vec3d CalcInterior(TCoords vertices);
Vec3d CalcNormal(Vec3d a, Vec3d b, Vec3d c);
