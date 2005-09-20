#include "CalcPenalty.h"
#include <Base/Affine.h>
using namespace LVRM;

const int K_DIV = 100;

Vec3d CalcInterior(TCoords vertices);
Vec3d CalcNormal(Vec3d a, Vec3d b, Vec3d c);