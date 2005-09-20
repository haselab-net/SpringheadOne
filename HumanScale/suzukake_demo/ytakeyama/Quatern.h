///////////////////////////////////////////////////////////////////////////////
//
// Quaternion.h : Quaternion System structure definition file
//
// Purpose:	Quaternion Conversion and Evaluation Functions
// Sources:
//	Shoemake, Ken, "Animating Rotations with Quaternion Curves"
//		Computer Graphics 85, pp. 245-254
//	Watt and Watt, Advanced Animation and Rendering Techniques
//		Addison Wesley, pp. 360-368
//  Shoemake, Graphic Gems II.
// // Updated by 2002.11 Jeong
///////////////////////////////////////////////////////////////////////////////

#if !defined(QUATERN_H__INCLUDED_)
#define QUATERN_H__INCLUDED_

#include <string.h>
#include <math.h>

typedef struct
{
	float x,y,z;
} tVector;

// NOT DECLARED AS float[4][4] BECAUSE OPENGL ACCESSES THIS STRANGLY
typedef struct
{
	float m[16];
} tMatrix;

/// Quaternion Definitions ////////////////////////////////////////////////////
typedef struct
{
	float x,y,z,w;
} tQuaternion;

typedef struct {
	double Matrix[4][4];
} MRot;


// for Inverse kinematics
////////Update by jeong
typedef struct {
	tVector ik_trans;
	tVector ik_rot;
	tVector ik_pos;
	tVector ik_min;
	tVector ik_max;
    tQuaternion ik_quat;
}IKBone;

#define M_PI        3.14159265358979323846f
#define HALF_PI	    1.57079632679489661923f

// Trig Macros ///////////////////////////////////////////////////////////////
#define DEGTORAD(A)	((A * M_PI) / 180.0f)
#define RADTODEG(A)	((A * 180.0f) / M_PI)
///////////////////////////////////////////////////////////////////////////////
#define copyMatrix(src, dest)    memcpy((dest), (src), sizeof(double)*16)

void MultVectorByMatrix(tMatrix *mat, tVector *v,tVector *result)
{
	result->x = (mat->m[0] * v->x) +
			   (mat->m[4] * v->y) +	
			   (mat->m[8] * v->z) +
			   mat->m[12];
	result->y = (mat->m[1] * v->x) +
			   (mat->m[5] * v->y) +	
			   (mat->m[9] * v->z) +
			   mat->m[13];
	result->z = (mat->m[2] * v->x) +
			   (mat->m[6] * v->y) +	
			   (mat->m[10] * v->z) +
			   mat->m[14];
}
//// MultVectorByMatrix //////////////////////////////////////////////////////
void multMatrixVector(double M[4][4], double V[4], double destV[4])
{
    int i, j;
	memset(destV, 0, sizeof(double)*4);
  
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			destV[i] += M[i][j]*V[j];
}

void multMatrix(double src1[4][4], double src2[4][4], double dest[4][4])
{
	int i, k, j;
	memset(dest, 0, sizeof(double)*16);
  
	for (i = 0; i < 4; i++)
		for ( k = 0; k < 4; k++)
			for (j = 0; j < 4; j++)
				dest[i][k] += src1[i][j]*src2[j][k];
}


/* returns squared length of input vector */    
double VectorSquaredLength(tVector *v) 
{
	return((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

/* returns length of input vector */
double VectorLength(tVector *v) 
{
	return(sqrt(VectorSquaredLength(v)));
}

/* destructively normalizes the input vector */
void NormalizeVector(tVector *v) 
{
	float len = (float)VectorLength(v);
    if (len != 0.0) 
	{ 
		v->x /= len;  
		v->y /= len; 
		v->z /= len; 
	}
}
/////////////////////////////////////////////////////////////////////
double DotProduct(tVector *v1, tVector *v2)
{
	return ((v1->x * v2->x) + (v1->y * v2->y) + (v1->z + v2->z));
}

/* return the cross product result = v1 cross v2 */
////////Update by jeong
void CrossProduct(tVector *v1, tVector *v2, tVector *result)
{
	result->x = (v1->y * v2->z) - (v1->z * v2->y);
	result->y = (v1->z * v2->x) - (v1->x * v2->z);
	result->z = (v1->x * v2->y) - (v1->y * v2->x);
}
///////////////////////////////////////////////////////////////////
double VectorSquaredDistance(tVector *v1, tVector *v2) 
{
	return(	((v1->x - v2->x) * (v1->x - v2->x)) + 
			((v1->y - v2->y) * (v1->y - v2->y)) + 	
			((v1->z - v2->z) * (v1->z - v2->z)) ); 	
}

double VectorDistance(tVector v1, tVector v2) 
{
	return(	sqrt(((v1.x - v2.x) * (v1.x - v2.x)) + 
			((v1.y - v2.y) * (v1.y - v2.y)) + 	
			((v1.z - v2.z) * (v1.z - v2.z)))); 	
}

void CopyVector(tVector *dest, tVector *src)
{
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
}
//// CopyVector ///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	ScaleVector
// Arguments:	pointer to vector and scale factor
///////////////////////////////////////////////////////////////////////////////
void ScaleVector(tVector *vect, float scale)
{
	vect->x *= scale;
	vect->y *= scale;
	vect->z *= scale;
}
//// ScaleVector ///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	AddVectors
// Arguments:	pointer to vectors and dest
///////////////////////////////////////////////////////////////////////////////
void AddVectors(tVector *vect1, tVector *vect2, tVector *dest)
{
	dest->x = vect1->x + vect2->x;
	dest->y = vect1->y + vect2->y;
	dest->z = vect1->z + vect2->z;
}
//// AddVectors ///////////////////////////////////////////////////////////////
// shadow
void CalcShadowMat(GLfloat mat[4][4], tQuaternion& Gn, tVector& vL)
{
   GLfloat kk;

   kk= Gn.x * vL.x + Gn.y*vL.y + Gn.z * vL.z;

   mat[0][0] = kk - vL.x*Gn.x;
   mat[1][0] = 0 - vL.x*Gn.y;
   mat[2][0] = 0 - vL.x*Gn.z;
   mat[3][0] = 0 - vL.x*Gn.w;

   mat[0][1] = 0 - vL.y*Gn.x;
   mat[1][1] = kk - vL.y*Gn.y;
   mat[2][1] = 0 - vL.y*Gn.z;
   mat[3][1] = 0 - vL.y*Gn.w;

   mat[0][2] = 0 - vL.z*Gn.x;
   mat[1][2] = 0 - vL.z*Gn.y;
   mat[2][2] = kk- vL.z*Gn.z;
   mat[3][2] = 0 - vL.z*Gn.w;
   
   mat[0][3] = 0;
   mat[1][3] = 0;
   mat[2][3] = 0;
   mat[3][3] = kk;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	MultQuaternions
// Purpose:		Computes the product of two quaternions
// Arguments:	pointer to quaternions and dest
///////////////////////////////////////////////////////////////////////////////
void MultQuaternions(tQuaternion *quat1, tQuaternion *quat2, tQuaternion *dest)
{
/// Local Variables ///////////////////////////////////////////////////////////
	tQuaternion v1,v2,v3,vf;
///////////////////////////////////////////////////////////////////////////////

	CopyVector((tVector *)&v1, (tVector *)quat1);		// COPY OFF THE VECTOR PART OF THE QUAT1
	ScaleVector((tVector *)&v1,quat2->w);	// MULTIPLY IT BY THE SCALAR PART OF QUAT2

	CopyVector((tVector *)&v2, (tVector *)quat2);		// COPY OFF THE VECTOR PART OF THE QUAT1
	ScaleVector((tVector *)&v2,quat1->w);	// MULTIPLY IT BY THE SCALAR PART OF QUAT2

	CrossProduct((tVector *)quat2,(tVector *)quat1,(tVector *)&v3);

	AddVectors((tVector *)&v1, (tVector *)&v2, (tVector *)&vf);
	AddVectors((tVector *)&v3, (tVector *)&vf, (tVector *)&vf);

	vf.w = (quat1->w * quat2->w) - DotProduct((tVector *)quat1,(tVector *)quat2);

	dest->x = vf.x;
	dest->y = vf.y;
	dest->z = vf.z;
	dest->w = vf.w;
}
//// MultQuaternions //////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Function:	MultQuaternions2
// Purpose:		Computes the product of two quaternions
// Arguments:	pointer to quaternions and dest
///////////////////////////////////////////////////////////////////////////////
void MultQuaternions2(tQuaternion *quat1, tQuaternion *quat2, tQuaternion *dest)
{
    tQuaternion tmp;
    tmp.x =		quat2->w * quat1->x + quat2->x * quat1->w +
				quat2->y * quat1->z - quat2->z * quat1->y;
    tmp.y  =	quat2->w * quat1->y + quat2->y * quat1->w +
				quat2->z * quat1->x - quat2->x * quat1->z;
    tmp.z  =	quat2->w * quat1->z + quat2->z * quat1->w +
				quat2->x * quat1->y - quat2->y * quat1->x;
    tmp.w  =	quat2->w * quat1->w - quat2->x * quat1->x -
				quat2->y * quat1->y - quat2->z * quat1->z;
    dest->x = tmp.x; dest->y = tmp.y;
    dest->z = tmp.z; dest->w = tmp.w;
}
//// MultQuaternions2 //////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Function:	NormalizeQuaternion
// Arguments:	a quaternion to set
// Discussion:  Quaternions must follow the rules of x^2 + y^2 + z^2 + w^2 = 1
//				This function insures this
///////////////////////////////////////////////////////////////////////////////
void NormalizeQuaternion(tQuaternion *quat)
{
/// Local Variables ///////////////////////////////////////////////////////////
	float magnitude;
///////////////////////////////////////////////////////////////////////////////
	// FIRST STEP, FIND THE MAGNITUDE
	magnitude = (quat->x * quat->x) + 
				(quat->y * quat->y) + 
				(quat->z * quat->z) + 
				(quat->w * quat->w);

	// DIVIDE BY THE MAGNITUDE TO NORMALIZE
	quat->x = quat->x / magnitude;
	quat->y = quat->y / magnitude;
	quat->z = quat->z / magnitude;
	quat->w = quat->w / magnitude;
}
// NormalizeQuaternion  ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Function:	EulerToQuaternion
// Purpose:		Convert a set of Euler angles to a Quaternion
// Arguments:	A rotation set of 3 angles, a quaternion to set
// Discussion:  As the order of rotations is important I am
//				using the Quantum Mechanics convention of (X,Y,Z)
//				a Yaw-Pitch-Roll (Y,X,Z) system would have to be
//				adjusted.  It is more efficient this way though.
///////////////////////////////////////////////////////////////////////////////
////////Update by jeong
void EulerToQuaternion(tVector *rot, tQuaternion *quat)
{
/// Local Variables ///////////////////////////////////////////////////////////
	float rx,ry,rz,tx,ty,tz,cx,cy,cz,sx,sy,sz,cc,cs,sc,ss;
///////////////////////////////////////////////////////////////////////////////
	// FIRST STEP, CONVERT ANGLES TO RADIANS
	rx =  (rot->x * (float)M_PI) / (360 / 2);
	ry =  (rot->y * (float)M_PI) / (360 / 2);
	rz =  (rot->z * (float)M_PI) / (360 / 2);
	// GET THE HALF ANGLES
	tx = rx * (float)0.5;
	ty = ry * (float)0.5;
	tz = rz * (float)0.5;
	cx = (float)cos(tx);
	cy = (float)cos(ty);
	cz = (float)cos(tz);
	sx = (float)sin(tx);
	sy = (float)sin(ty);
	sz = (float)sin(tz);
// Rot x-> y->z
//	cc = cx * cz;
//	cs = cx * sz;
//	sc = sx * cz;
//	ss = sx * sz;
//	quat->x = (cy * sc) - (sy * cs);
//	quat->y = (cy * ss) + (sy * cc);
//	quat->z = (cy * cs) - (sy * sc);
//	quat->w = (cy * cc) + (sy * ss);
	// Rot z-> x-> y
	cc = cy * cz;
	cs = cy * sz;
	sc = sy * cz;
	ss = sy * sz;

	quat->x = (sx * cc) - (cx * ss);
	quat->y = (cx * sc) + (sx * cs);
	quat->z = (sx * sc) + (sx * cs);
	quat->w = (cx * cc) - (sx * ss);

	// INSURE THE QUATERNION IS NORMALIZED
	NormalizeQuaternion(quat);
}
// EulerToQuaternion  /////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	EulerToQuaternion2
// Purpose:		Convert a set of Euler angles to a Quaternion
// Arguments:	A rotation set of 3 angles, a quaternion to set
// Discussion:  This is a second variation.  It creates a
//				Series of quaternions and multiplies them together
//				It would be easier to extend this for other rotation orders
///////////////////////////////////////////////////////////////////////////////
void EulerToQuaternion2(tVector *rot, tQuaternion *quat)
{
/// Local Variables ///////////////////////////////////////////////////////////
	float rx,ry,rz,ti,tj,tk;
	tQuaternion qx,qy,qz,qf;
///////////////////////////////////////////////////////////////////////////////
	// FIRST STEP, CONVERT ANGLES TO RADIANS
	rx =  (rot->x * (float)M_PI) / (360 / 2);
	ry =  (rot->y * (float)M_PI) / (360 / 2);
	rz =  (rot->z * (float)M_PI) / (360 / 2);
	// GET THE HALF ANGLES
	ti = rx * (float)0.5;
	tj = ry * (float)0.5;
	tk = rz * (float)0.5;

	qx.x = (float)sin(ti); qx.y = 0.0; qx.z = 0.0; qx.w = (float)cos(ti);
	qy.x = 0.0; qy.y = (float)sin(tj); qy.z = 0.0; qy.w = (float)cos(tj);
	qz.x = 0.0; qz.y = 0.0; qz.z = (float)sin(tk); qz.w = (float)cos(tk);
//rot x->y->z
//	MultQuaternions(&qx,&qy,&qf);
//	MultQuaternions(&qf,&qz,&qf);
//rot z->x->y
    MultQuaternions(&qz,&qx,&qf);
	MultQuaternions(&qf,&qy,&qf);

	// INSURE THE QUATERNION IS NORMALIZED
	NormalizeQuaternion(&qf);

	quat->x = qf.x;
	quat->y = qf.y;
	quat->z = qf.z;
	quat->w = qf.w;

}
// EulerToQuaternion2  /////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	QuatToAxisAngle
// Purpose:		Convert a Quaternion to Axis Angle representation
// Arguments:	A quaternion to convert, a axisAngle to set
///////////////////////////////////////////////////////////////////////////////
void QuatToAxisAngle(tQuaternion *quat,tQuaternion *axisAngle)
{
/// Local Variables ///////////////////////////////////////////////////////////
	float scale,tw;
///////////////////////////////////////////////////////////////////////////////
	if (quat->w >1)
	{
        axisAngle->w = 0.0;
		axisAngle->x = 0.0;
	    axisAngle->y = 0.0;
	    axisAngle->z = 1.0;
	}
	else
	{
	tw = (float)acos(quat->w) * 2;
	scale = (float)sin(tw / 2.0);
	
	axisAngle->x = quat->x / scale;
	axisAngle->y = quat->y / scale;
	axisAngle->z = quat->z / scale;

	// NOW CONVERT THE ANGLE OF ROTATION BACK TO DEGREES
	axisAngle->w = (tw * (360 / 2)) / (float)M_PI;
	}
}
// QuatToAxisAngle  /////////////////////////////////////////////////////////

#define DELTA	0.0001		// DIFFERENCE AT WHICH TO LERP INSTEAD OF SLERP
///////////////////////////////////////////////////////////////////////////////
// Function:	SlerpQuat
// Purpose:		Spherical Linear Interpolation Between two Quaternions
// Arguments:	Two Quaternions, blend factor, result quaternion
///////////////////////////////////////////////////////////////////////////////
void SlerpQuat2(tQuaternion *quat1,tQuaternion *quat2,float slerp, tQuaternion *result)
{
/// Local Variables ///////////////////////////////////////////////////////////
	tQuaternion quat1b;
	double omega,cosom,sinom,scale0,scale1;
///////////////////////////////////////////////////////////////////////////////
	// USE THE DOT PRODUCT TO GET THE COSINE OF THE ANGLE BETWEEN THE
	// QUATERNIONS
	cosom = quat1->x * quat2->x + 
			quat1->y * quat2->y + 
			quat1->z * quat2->z + 
			quat1->w * quat2->w; 

	// MAKE SURE WE ARE TRAVELING ALONG THE SHORTER PATH
	if (cosom < 0.0)
	{
		// IF WE ARE NOT, REVERSE ONE OF THE QUATERNIONS
		cosom = -cosom;
		quat1b.x = - quat1->x;
		quat1b.y = - quat1->y;
		quat1b.z = - quat1->z;
		quat1b.w = - quat1->w;
	} else {
		quat1b.x = quat1->x;
		quat1b.y = quat1->y;
		quat1b.z = quat1->z;
		quat1b.w = quat1->w;
	}

	
	if ((1.0 - cosom) > DELTA) {
		omega = acos(cosom);
		sinom = sin(omega);
		scale0 = sin((1 - slerp) * omega) / sinom;
		scale1 = sin(slerp * omega) / sinom;
	} else {
		scale0 = 1.0 - slerp;
		scale1 = slerp;
	}

	result->x = scale0 * quat1->x + scale1 * quat2->x;
	result->y = scale0 * quat1->y + scale1 * quat2->y;
	result->z = scale0 * quat1->z + scale1 * quat2->z;
	result->w = scale0 * quat1->w + scale1 * quat2->w;
}
// SlerpQuat  /////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	SlerpQuat
// Purpose:		Spherical Linear Interpolation Between two Quaternions
// Arguments:	Two Quaternions, blend factor, result quaternion
// Notes:		The comments explain the handling of the special cases.
//				The comments in the magazine were wrong.  Adjust the
//				DELTA constant to play with the LERP vs. SLERP level
///////////////////////////////////////////////////////////////////////////////
void SlerpQuat(tQuaternion *quat1,tQuaternion *quat2,float slerp, tQuaternion *result)
{
/// Local Variables ///////////////////////////////////////////////////////////
	double omega,cosom,sinom,scale0,scale1;
///////////////////////////////////////////////////////////////////////////////
	// USE THE DOT PRODUCT TO GET THE COSINE OF THE ANGLE BETWEEN THE
	// QUATERNIONS
	cosom = quat1->x * quat2->x + 
			quat1->y * quat2->y + 
			quat1->z * quat2->z + 
			quat1->w * quat2->w; 

	// CHECK A COUPLE OF SPECIAL CASES. 
	// MAKE SURE THE TWO QUATERNIONS ARE NOT EXACTLY OPPOSITE? (WITHIN A LITTLE SLOP)
	if ((1.0 + cosom) > DELTA)
	{
		// ARE THEY MORE THAN A LITTLE BIT DIFFERENT? AVOID A DIVIDED BY ZERO AND LERP IF NOT
		if ((1.0 - cosom) > DELTA) {
			// YES, DO A SLERP
			omega = acos(cosom);
			sinom = sin(omega);
			scale0 = sin((1.0 - slerp) * omega) / sinom;
			scale1 = sin(slerp * omega) / sinom;
		} else {
			// NOT A VERY BIG DIFFERENCE, DO A LERP
			scale0 = 1.0 - slerp;
			scale1 = slerp;
		}
		result->x = scale0 * quat1->x + scale1 * quat2->x;
		result->y = scale0 * quat1->y + scale1 * quat2->y;
		result->z = scale0 * quat1->z + scale1 * quat2->z;
		result->w = scale0 * quat1->w + scale1 * quat2->w;
	} else {
		// THE QUATERNIONS ARE NEARLY OPPOSITE SO TO AVOID A DIVIDED BY ZERO ERROR
		// CALCULATE A PERPENDICULAR QUATERNION AND SLERP THAT DIRECTION
		result->x = -quat2->y;
		result->y = quat2->x;
		result->z = -quat2->w;
		result->w = quat2->z;
		scale0 = sin((1.0 - slerp) * (float)HALF_PI);
		scale1 = sin(slerp * (float)HALF_PI);
		result->x = scale0 * quat1->x + scale1 * result->x;
		result->y = scale0 * quat1->y + scale1 * result->y;
		result->z = scale0 * quat1->z + scale1 * result->z;
		result->w = scale0 * quat1->w + scale1 * result->w;
	}

}
// SlerpQuat  /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Function:	QuatToEuler
// Purpose:		Convert a Quaternion back to Euler Angles
// Arguments:	Quaternions and target Euler vector
// Notes:		The method is to convert Quaternion to a 3x3 matrix and
//				decompose the matrix.  This is subject to the
//				ambiguities of square roots and problems with inverse trig.
//				Matrix to Euler conversion is really very ill-defined but works
//				for my purposes.
///////////////////////////////////////////////////////////////////////////////
void QuatToEuler(const tQuaternion *quat, tVector *euler)
{
/// Local Variables ///////////////////////////////////////////////////////////
	float matrix[3][3];
	float cx,sx;
	float cy,sy,yr;
	float cz,sz; //z
///////////////////////////////////////////////////////////////////////////////
	// CONVERT QUATERNION TO MATRIX - I DON'T REALLY NEED ALL OF IT
	matrix[0][0] = 1.0f - (2.0f * quat->y * quat->y) - (2.0f * quat->z * quat->z);
//	matrix[0][1] = (2.0f * quat->x * quat->y) - (2.0f * quat->w * quat->z);
//	matrix[0][2] = (2.0f * quat->x * quat->z) + (2.0f * quat->w * quat->y);
	matrix[1][0] = (2.0f * quat->x * quat->y) + (2.0f * quat->w * quat->z);
//	matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
//	matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
	matrix[2][0] = (2.0f * quat->x * quat->z) - (2.0f * quat->w * quat->y);
	matrix[2][1] = (2.0f * quat->y * quat->z) + (2.0f * quat->w * quat->x);
	matrix[2][2] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->y * quat->y);

	sy = -matrix[2][0];
	cy = sqrt(1 - (sy * sy));
	yr = (float)atan2(sy,cy);
	euler->y = (yr * 180.0f) / (float)M_PI;

	// AVOID DIVIDE BY ZERO ERROR ONLY WHERE Y= +-90 or +-270 
	// NOT CHECKING cy BECAUSE OF PRECISION ERRORS
	if (sy != 1.0f && sy != -1.0f)	
	{
		cx = matrix[2][2] / cy;
		sx = matrix[2][1] / cy;
		euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)M_PI;	// RAD TO DEG

		cz = matrix[0][0] / cy;
		sz = matrix[1][0] / cy;
		euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)M_PI;	// RAD TO DEG
	}
	else
	{
		// SINCE Cos(Y) IS 0, I AM SCREWED.  ADOPT THE STANDARD Z = 0
		// I THINK THERE IS A WAY TO FIX THIS BUT I AM NOT SURE.  EULERS SUCK
		// NEED SOME MORE OF THE MATRIX TERMS NOW
		matrix[1][1] = 1.0f - (2.0f * quat->x * quat->x) - (2.0f * quat->z * quat->z);
		matrix[1][2] = (2.0f * quat->y * quat->z) - (2.0f * quat->w * quat->x);
		cx = matrix[1][1];
		sx = -matrix[1][2];
		euler->x = ((float)atan2(sx,cx) * 180.0f) / (float)M_PI;	// RAD TO DEG

		cz = 1.0f;
		sz = 0.0f;
		euler->z = ((float)atan2(sz,cz) * 180.0f) / (float)M_PI;	// RAD TO DEG
	}
}
// QuatToEuler  ///////////////////////////////////////////////////////////////
////////Update by jeong
void AxisAngleToQuat(tVector *crossResult, double turnDeg, tQuaternion *aquat)
{
	float s;
	double turnRad = (turnDeg * M_PI) / 180.0f;

	s = sin(turnRad * 0.5);
	aquat->w = cos(turnRad * 0.5);
	aquat->x = s* (crossResult->x);
	aquat->y = s* (crossResult->y);
	aquat->z = s* (crossResult->y);

}
#endif // !defined(QUATERN_H__INCLUDED_)
