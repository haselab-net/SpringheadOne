#ifndef _MYMATH
#define _MYMATH

#include <math.h>

//Physics game for developer 2001
///////////////////////////////////////////////////////////////////////////////
//
// tQuaternion.h : tQuaternion System structure definition file
//
// Purpose:	tQuaternion Conversion and Evaluation Functions
// Sources:
//	Shoemake, Ken, "Animating Rotations with tQuaternion Curves"
//		Computer Graphics 85, pp. 245-254
//	Watt and Watt, Advanced Animation and Rendering Techniques
//		Addison Wesley, pp. 360-368
//  Shoemake, Graphic Gems II.
// // Updated by 2002.11 Jeong
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------//
// Misc. Constants
//------------------------------------------------------------------------//

double	const	g	= 32.174f;		// -acceleration due to gravity, ft/s^2
double	const	rho = 0.0023769f;	// desity of air at sea level, slugs/ft^3
double	const	tol = 0.0000000001f;		// double type tolerance 
#define M_PI        3.14159265358979323846f
#define HALF_PI	    1.57079632679489661923f

typedef struct {
	double Matrix[4][4];
} MRot;

#define copyMatrix(src, dest)    memcpy((dest), (src), sizeof(double)*16)

//------------------------------------------------------------------------//
// Misc. Functions
//------------------------------------------------------------------------//
inline	double	DegreesToRadians(double deg);
inline	double	RadiansToDegrees(double rad);

inline	double	DegreesToRadians(double deg)
{
	return deg * M_PI / 180.0f;
}

inline	double	RadiansToDegrees(double rad)
{	
	return rad * 180.0f / M_PI;
}

//------------------------------------------------------------------------//
// MyVector Class and MyVector functions
//------------------------------------------------------------------------//
class MyVector {
public:
	double x;
	double y;
	double z;

	MyVector(void);
	MyVector(double xi, double yi, double zi);

	double Magnitude(void);
	void  Normalize(void);
	void  Reverse(void);

	MyVector& operator+=(MyVector u);	// MyVector addition
	MyVector& operator-=(MyVector u);	// MyVector subtraction
	MyVector& operator*=(double s);	// scalar multiply
	MyVector& operator/=(double s);	// scalar divide

	MyVector operator-(void);

};

inline	MyVector operator+(MyVector u, MyVector v);
inline	MyVector operator-(MyVector u, MyVector v);
inline	MyVector operator^(MyVector u, MyVector v);
inline	double operator*(MyVector u, MyVector v);
inline	MyVector operator*(double s, MyVector u);
inline	MyVector operator*(MyVector u, double s);
inline	MyVector operator/(MyVector u, double s);
inline	double TripleScalarProduct(MyVector u, MyVector v, MyVector w);
inline  double MyVectorDistance(MyVector v1, MyVector v2);

inline MyVector::MyVector(void)
{
	x = 0;
	y = 0;
	z = 0;
}

inline MyVector::MyVector(double xi, double yi, double zi)
{
	x = xi;
	y = yi;
	z = zi;
}

inline	double MyVector::Magnitude(void)
{
	return (double) sqrt(x*x + y*y + z*z);
}

inline	void  MyVector::Normalize(void)
{
	double m = (double) sqrt(x*x + y*y + z*z);
	if(m <= tol) m = 1;
	x /= m;
	y /= m;
	z /= m;	

	if (fabs(x) < tol) x = 0.0f;
	if (fabs(y) < tol) y = 0.0f;
	if (fabs(z) < tol) z = 0.0f;
}

inline	void  MyVector::Reverse(void)
{
	x = -x;
	y = -y;
	z = -z;
}

inline MyVector& MyVector::operator+=(MyVector u)
{
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

inline	MyVector& MyVector::operator-=(MyVector u)
{
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

inline	MyVector& MyVector::operator*=(double s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline	MyVector& MyVector::operator/=(double s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline	MyVector MyVector::operator-(void)
{
	return MyVector(-x, -y, -z);
}


inline	MyVector operator+(MyVector u, MyVector v)
{
	return MyVector(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline	MyVector operator-(MyVector u, MyVector v)
{
	return MyVector(u.x - v.x, u.y - v.y, u.z - v.z);
}

// MyVector cross product (u cross v)
inline	MyVector operator^(MyVector u, MyVector v)
{
	return MyVector(	u.y*v.z - u.z*v.y,
					-u.x*v.z + u.z*v.x,
					u.x*v.y - u.y*v.x );
}

// MyVector dot product
inline	double operator*(MyVector u, MyVector v)
{
	return (u.x*v.x + u.y*v.y + u.z*v.z);
}

inline	MyVector operator*(double s, MyVector u)
{
	return MyVector(u.x*s, u.y*s, u.z*s);
}

inline	MyVector operator*(MyVector u, double s)
{
	return MyVector(u.x*s, u.y*s, u.z*s);
}

inline	MyVector operator/(MyVector u, double s)
{
	return MyVector(u.x/s, u.y/s, u.z/s);
}

// triple scalar product (u dot (v cross w))
inline	double TripleScalarProduct(MyVector u, MyVector v, MyVector w)
{
	return double(	(u.x * (v.y*w.z - v.z*w.y)) +
					(u.y * (-v.x*w.z + v.z*w.x)) +
					(u.z * (v.x*w.y - v.y*w.x)) );
	//return u*(v^w);

}

inline double MyVectorDistance(MyVector v1, MyVector v2) 
{
	return(	sqrt(((v1.x - v2.x) * (v1.x - v2.x)) + 
			((v1.y - v2.y) * (v1.y - v2.y)) + 	
			((v1.z - v2.z) * (v1.z - v2.z)))); 	
}

//------------------------------------------------------------------------//
// Matrix Class and matrix functions
//------------------------------------------------------------------------//

class Matrix3x3 {
public:
	// elements eij: i -> row, j -> column
	double	e11, e12, e13, e21, e22, e23, e31, e32, e33;	

	Matrix3x3(void);
	Matrix3x3(	double r1c1, double r1c2, double r1c3, 
				double r2c1, double r2c2, double r2c3, 
				double r3c1, double r3c2, double r3c3 );

	double	det(void);
	Matrix3x3	Transpose(void);
	Matrix3x3	Inverse(void);

	Matrix3x3& operator+=(Matrix3x3 m);
	Matrix3x3& operator-=(Matrix3x3 m);
	Matrix3x3& operator*=(double s);
	Matrix3x3& operator/=(double s);
};

inline	Matrix3x3 operator+(Matrix3x3 m1, Matrix3x3 m2);
inline	Matrix3x3 operator-(Matrix3x3 m1, Matrix3x3 m2);
inline	Matrix3x3 operator/(Matrix3x3 m, double s);
inline	Matrix3x3 operator*(Matrix3x3 m1, Matrix3x3 m2);
inline	Matrix3x3 operator*(Matrix3x3 m, double s);
inline	Matrix3x3 operator*(double s, Matrix3x3 m);
inline	MyVector operator*(Matrix3x3 m, MyVector u);
inline	MyVector operator*(MyVector u, Matrix3x3 m);


inline	Matrix3x3::Matrix3x3(void)
{
	e11 = 0;
	e12 = 0;
	e13 = 0;
	e21 = 0;
	e22 = 0;
	e23 = 0;
	e31 = 0;
	e32 = 0;
	e33 = 0;
}

inline	Matrix3x3::Matrix3x3(	double r1c1, double r1c2, double r1c3, 
								double r2c1, double r2c2, double r2c3, 
								double r3c1, double r3c2, double r3c3 )
{
	e11 = r1c1;
	e12 = r1c2;
	e13 = r1c3;
	e21 = r2c1;
	e22 = r2c2;
	e23 = r2c3;
	e31 = r3c1;
	e32 = r3c2;
	e33 = r3c3;
}

inline	double	Matrix3x3::det(void)
{
	return	e11*e22*e33 - 
			e11*e32*e23 + 
			e21*e32*e13 - 
			e21*e12*e33 + 
			e31*e12*e23 - 
			e31*e22*e13;	
}

inline	Matrix3x3	Matrix3x3::Transpose(void)
{
	return Matrix3x3(e11,e21,e31,e12,e22,e32,e13,e23,e33);
}

inline	Matrix3x3	Matrix3x3::Inverse(void)
{
	double	d = e11*e22*e33 - 
				e11*e32*e23 + 
				e21*e32*e13 - 
				e21*e12*e33 + 
				e31*e12*e23 - 
				e31*e22*e13;

	if (d == 0) d = 1;

	return	Matrix3x3(	(e22*e33-e23*e32)/d,
						-(e12*e33-e13*e32)/d,
						(e12*e23-e13*e22)/d,
						-(e21*e33-e23*e31)/d,
						(e11*e33-e13*e31)/d,
						-(e11*e23-e13*e21)/d,
						(e21*e32-e22*e31)/d,
						-(e11*e32-e12*e31)/d,
						(e11*e22-e12*e21)/d );	
}

inline	Matrix3x3& Matrix3x3::operator+=(Matrix3x3 m)
{
	e11 += m.e11;
	e12 += m.e12;
	e13 += m.e13;
	e21 += m.e21;
	e22 += m.e22;
	e23 += m.e23;
	e31 += m.e31;
	e32 += m.e32;
	e33 += m.e33;
	return *this;
}

inline	Matrix3x3& Matrix3x3::operator-=(Matrix3x3 m)
{
	e11 -= m.e11;
	e12 -= m.e12;
	e13 -= m.e13;
	e21 -= m.e21;
	e22 -= m.e22;
	e23 -= m.e23;
	e31 -= m.e31;
	e32 -= m.e32;
	e33 -= m.e33;
	return *this;
}

inline	Matrix3x3& Matrix3x3::operator*=(double s)
{
	e11 *= s;
	e12 *= s;
	e13 *= s;
	e21 *= s;
	e22 *= s;
	e23 *= s;
	e31 *= s;
	e32 *= s;
	e33 *= s;
	return *this;
}

inline	Matrix3x3& Matrix3x3::operator/=(double s)
{
	e11 /= s;
	e12 /= s;
	e13 /= s;
	e21 /= s;
	e22 /= s;
	e23 /= s;
	e31 /= s;
	e32 /= s;
	e33 /= s;
	return *this;
}

inline	Matrix3x3 operator+(Matrix3x3 m1, Matrix3x3 m2)
{
	return	Matrix3x3(	m1.e11+m2.e11,
						m1.e12+m2.e12,
						m1.e13+m2.e13,
						m1.e21+m2.e21,
						m1.e22+m2.e22,
						m1.e23+m2.e23,
						m1.e31+m2.e31,
						m1.e32+m2.e32,
						m1.e33+m2.e33);
}

inline	Matrix3x3 operator-(Matrix3x3 m1, Matrix3x3 m2)
{
	return	Matrix3x3(	m1.e11-m2.e11,
						m1.e12-m2.e12,
						m1.e13-m2.e13,
						m1.e21-m2.e21,
						m1.e22-m2.e22,
						m1.e23-m2.e23,
						m1.e31-m2.e31,
						m1.e32-m2.e32,
						m1.e33-m2.e33);
}

inline	Matrix3x3 operator/(Matrix3x3 m, double s)
{	
	return	Matrix3x3(	m.e11/s,
						m.e12/s,
						m.e13/s,
						m.e21/s,
						m.e22/s,
						m.e23/s,
						m.e31/s,
						m.e32/s,
						m.e33/s);
}

inline	Matrix3x3 operator*(Matrix3x3 m1, Matrix3x3 m2)
{
	return Matrix3x3(	m1.e11*m2.e11 + m1.e12*m2.e21 + m1.e13*m2.e31,
						m1.e11*m2.e12 + m1.e12*m2.e22 + m1.e13*m2.e32,
						m1.e11*m2.e13 + m1.e12*m2.e23 + m1.e13*m2.e33,
						m1.e21*m2.e11 + m1.e22*m2.e21 + m1.e23*m2.e31,
						m1.e21*m2.e12 + m1.e22*m2.e22 + m1.e23*m2.e32,
						m1.e21*m2.e13 + m1.e22*m2.e23 + m1.e23*m2.e33,
						m1.e31*m2.e11 + m1.e32*m2.e21 + m1.e33*m2.e31,
						m1.e31*m2.e12 + m1.e32*m2.e22 + m1.e33*m2.e32,
						m1.e31*m2.e13 + m1.e32*m2.e23 + m1.e33*m2.e33 );
}

inline	Matrix3x3 operator*(Matrix3x3 m, double s)
{
	return	Matrix3x3(	m.e11*s,
						m.e12*s,
						m.e13*s,
						m.e21*s,
						m.e22*s,
						m.e23*s,
						m.e31*s,
						m.e32*s,
						m.e33*s);
}

inline	Matrix3x3 operator*(double s, Matrix3x3 m)
{
	return	Matrix3x3(	m.e11*s,
						m.e12*s,
						m.e13*s,
						m.e21*s,
						m.e22*s,
						m.e23*s,
						m.e31*s,
						m.e32*s,
						m.e33*s);
}

inline	MyVector operator*(Matrix3x3 m, MyVector u)
{
	return MyVector(	m.e11*u.x + m.e12*u.y + m.e13*u.z,
					m.e21*u.x + m.e22*u.y + m.e23*u.z,
					m.e31*u.x + m.e32*u.y + m.e33*u.z);					
}

inline	MyVector operator*(MyVector u, Matrix3x3 m)
{
	return MyVector(	u.x*m.e11 + u.y*m.e21 + u.z*m.e31,
					u.x*m.e12 + u.y*m.e22 + u.z*m.e32,
					u.x*m.e13 + u.y*m.e23 + u.z*m.e33);
}

//------------------------------------------------------------------------//
// tQuaternion Class and tQuaternion functions
//------------------------------------------------------------------------//

class tQuaternion {
public:
	double	n;	// number (scalar) part
	MyVector	v;	// MyVector part: v.x, v.y, v.z

	tQuaternion(void);
	tQuaternion(double e0, double e1, double e2, double e3);

	double	Magnitude(void);
	MyVector	GetMyVector(void);
	double	GetScalar(void);
	tQuaternion	operator+=(tQuaternion q);
	tQuaternion	operator-=(tQuaternion q);
	tQuaternion operator*=(double s);
	tQuaternion operator/=(double s);
	tQuaternion	operator~(void) const { return tQuaternion(n, -v.x, -v.y, -v.z);}
};

inline	tQuaternion operator+(tQuaternion q1, tQuaternion q2);
inline	tQuaternion operator-(tQuaternion q1, tQuaternion q2);
inline	tQuaternion operator*(tQuaternion q1, tQuaternion q2);
inline	tQuaternion operator*(tQuaternion q, double s);
inline	tQuaternion operator*(double s, tQuaternion q);
inline	tQuaternion operator*(tQuaternion q, MyVector v);
inline	tQuaternion operator*(MyVector v, tQuaternion q);
inline	tQuaternion operator/(tQuaternion q, double s);
inline	double QGetAngle(tQuaternion q);
inline	MyVector QGetAxis(tQuaternion q);
inline	tQuaternion QRotate(tQuaternion q1, tQuaternion q2);
inline	MyVector	QVRotate(tQuaternion q, MyVector v);
inline	tQuaternion	MakeQFromEulerAngles(double x, double y, double z);
inline	MyVector	MakeEulerAnglesFromQ(tQuaternion q);


inline	tQuaternion::tQuaternion(void)
{
	n = 0;
	v.x = 0;
	v.y =  0;
	v.z = 0;
}

inline	tQuaternion::tQuaternion(double e0, double e1, double e2, double e3)
{
	n = e0;
	v.x = e1;
	v.y = e2;
	v.z = e3;
}

inline	double	tQuaternion::Magnitude(void)
{
	return (double) sqrt(n*n + v.x*v.x + v.y*v.y + v.z*v.z);
}

inline	MyVector	tQuaternion::GetMyVector(void)
{
	return MyVector(v.x, v.y, v.z);
}

inline	double	tQuaternion::GetScalar(void)
{
	return n;
}

inline	tQuaternion	tQuaternion::operator+=(tQuaternion q)
{
	n += q.n;
	v.x += q.v.x;
	v.y += q.v.y;
	v.z += q.v.z;
	return *this;
}

inline	tQuaternion	tQuaternion::operator-=(tQuaternion q)
{
	n -= q.n;
	v.x -= q.v.x;
	v.y -= q.v.y;
	v.z -= q.v.z;
	return *this;
}

inline	tQuaternion tQuaternion::operator*=(double s)
{
	n *= s;
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return *this;
}

inline	tQuaternion tQuaternion::operator/=(double s)
{
	n /= s;
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return *this;
}

/*inline	tQuaternion	tQuaternion::operator~()
{
	return tQuaternion(n, -v.x, -v.y, -v.z);
}*/

inline	tQuaternion operator+(tQuaternion q1, tQuaternion q2)
{
	return	tQuaternion(	q1.n + q2.n,
							q1.v.x + q2.v.x,
							q1.v.y + q2.v.y,
							q1.v.z + q2.v.z);
}

inline	tQuaternion operator-(tQuaternion q1, tQuaternion q2)
{
	return	tQuaternion(	q1.n - q2.n,
							q1.v.x - q2.v.x,
							q1.v.y - q2.v.y,
							q1.v.z - q2.v.z);
}

inline	tQuaternion operator*(tQuaternion q1, tQuaternion q2)
{
	return	tQuaternion(	q1.n*q2.n - q1.v.x*q2.v.x - q1.v.y*q2.v.y - q1.v.z*q2.v.z,
							q1.n*q2.v.x + q1.v.x*q2.n + q1.v.y*q2.v.z - q1.v.z*q2.v.y,
							q1.n*q2.v.y + q1.v.y*q2.n + q1.v.z*q2.v.x - q1.v.x*q2.v.z,
							q1.n*q2.v.z + q1.v.z*q2.n + q1.v.x*q2.v.y - q1.v.y*q2.v.x);							
}

inline	tQuaternion operator*(tQuaternion q, double s)
{
	return	tQuaternion(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
}

inline	tQuaternion operator*(double s, tQuaternion q)
{
	return	tQuaternion(q.n*s, q.v.x*s, q.v.y*s, q.v.z*s);
}

inline	tQuaternion operator*(tQuaternion q, MyVector v)
{
	return	tQuaternion(	-(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
							q.n*v.x + q.v.y*v.z - q.v.z*v.y,
							q.n*v.y + q.v.z*v.x - q.v.x*v.z,
							q.n*v.z + q.v.x*v.y - q.v.y*v.x);
}

inline	tQuaternion operator*(MyVector v, tQuaternion q)
{
	return	tQuaternion(	-(q.v.x*v.x + q.v.y*v.y + q.v.z*v.z),
							q.n*v.x + q.v.z*v.y - q.v.y*v.z,
							q.n*v.y + q.v.x*v.z - q.v.z*v.x,
							q.n*v.z + q.v.y*v.x - q.v.x*v.y);
}

inline	tQuaternion operator/(tQuaternion q, double s)
{
	return	tQuaternion(q.n/s, q.v.x/s, q.v.y/s, q.v.z/s);
}

inline	double QGetAngle(tQuaternion q)
{
	return	(double) (2*acos(q.n));
}

inline	MyVector QGetAxis(tQuaternion q)
{
	MyVector v;
	double m;

	v = q.GetMyVector();
	m = v.Magnitude();
	
	if (m <= tol)
		return MyVector();
	else
		return v/m;	
}

inline	tQuaternion QRotate(tQuaternion q1, tQuaternion q2)
{
	return	q1*q2*(~q1);
}

inline	MyVector	QVRotate(tQuaternion q, MyVector v)
{
	tQuaternion t;


	t = q*v*(~q);

	return	t.GetMyVector();
}

inline	tQuaternion	MakeQFromEulerAngles(double roll, double pitch, double yaw)
{
	tQuaternion	q;
//	double	roll = DegreesToRadians(x);
//	double	pitch = DegreesToRadians(y);
//	double	yaw = DegreesToRadians(z);
	
	double	cyaw, cpitch, croll, syaw, spitch, sroll;
	double	cc, cs, sc, ss;

	cyaw = cos(0.5f * yaw);
	cpitch = cos(0.5f * pitch);
	croll = cos(0.5f * roll);
	syaw = sin(0.5f * yaw);
	spitch = sin(0.5f * pitch);
	sroll = sin(0.5f * roll);
/*
	cyawcpitch = cyaw*cpitch;
	syawspitch = syaw*spitch;
	cyawspitch = cyaw*spitch;
	syawcpitch = syaw*cpitch;

	q.n = (double) (cyawcpitch * croll + syawspitch * sroll);
	q.v.x = (double) (cyawcpitch * sroll - syawspitch * croll); 
	q.v.y = (double) (cyawspitch * croll + syawcpitch * sroll);
	q.v.z = (double) (syawcpitch * croll - cyawspitch * sroll);
*/
	// Rot z-> x-> y
	cc = cpitch * cyaw;
	cs = cpitch * syaw;
	sc = spitch * cyaw;
	ss = spitch * syaw;

	q.n = (sroll * cc) - (croll * ss);
	q.v.x = (croll * sc) + (sroll * cs);
	q.v.y = (sroll * sc) + (sroll * cs);
	q.v.z = (croll * cc) - (sroll * ss);

	return q;
}

inline	MyVector	MakeEulerAnglesFromQ(tQuaternion q)
{
	double	r11, r21, r31, r32, r33, r12, r13;
	double	q00, q11, q22, q33;
	double	tmp;
	MyVector	u;

	q00 = q.n * q.n;
	q11 = q.v.x * q.v.x;
	q22 = q.v.y * q.v.y;
	q33 = q.v.z * q.v.z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (q.v.x*q.v.y + q.n*q.v.z);
	r31 = 2 * (q.v.x*q.v.z - q.n*q.v.y);
	r32 = 2 * (q.v.y*q.v.z + q.n*q.v.x);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if(tmp > 0.999999)
	{
		r12 = 2 * (q.v.x*q.v.y - q.n*q.v.z);
		r13 = 2 * (q.v.x*q.v.z + q.n*q.v.y);

		u.x = RadiansToDegrees(0.0f); //roll
		u.y = RadiansToDegrees((double) (-(M_PI/2) * r31/tmp)); // pitch
		u.z = RadiansToDegrees((double) atan2(-r12, -r31*r13)); // yaw
		return u;
	}
	u.x = RadiansToDegrees((double) atan2(r32, r33)); // roll
	u.y = RadiansToDegrees((double) asin(-r31));		 // pitch
	u.z = RadiansToDegrees((double) atan2(r21, r11)); // yaw
	return u;
	

}

#define DELTA	0.0001		// DIFFERENCE AT WHICH TO LERP INSTEAD OF SLERP
///////////////////////////////////////////////////////////////////////////////
// Function:	SlerpQuat
// Purpose:		Spherical Linear Interpolation Between two tQuaternions
// Arguments:	Two tQuaternions, blend factor, result tQuaternion
///////////////////////////////////////////////////////////////////////////////
inline tQuaternion SlerpQuat2(tQuaternion q1,tQuaternion q2,double slerp)
{
/// Local Variables ///////////////////////////////////////////////////////////
	tQuaternion quat1b,result;
	double omega,cosom,sinom,scale0,scale1;
///////////////////////////////////////////////////////////////////////////////
	// USE THE DOT PRODUCT TO GET THE COSINE OF THE ANGLE BETWEEN THE
	// tQuaternionS
	cosom = q1.n * q2.n + 
			q1.v.x * q2.v.x + 
			q1.v.y * q2.v.y + 
			q1.v.z * q2.v.z; 

	// MAKE SURE WE ARE TRAVELING ALONG THE SHORTER PATH
	if (cosom < 0.0)
	{
		// IF WE ARE NOT, REVERSE ONE OF THE tQuaternionS
		cosom = -cosom;
		quat1b.n = - q1.n;
		quat1b.v.x = - q1.v.x;
		quat1b.v.y = - q1.v.y;
		quat1b.v.z = - q1.v.z;
	} else {
		quat1b.n = q1.n;
		quat1b.v.x = q1.v.x;
		quat1b.v.y = q1.v.y;
		quat1b.v.z = q1.v.z;
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

	result.n = scale0 * q1.n + scale1 * q2.n;
	result.v.x = scale0 * q1.v.x + scale1 * q2.v.x;
	result.v.y = scale0 * q1.v.y + scale1 * q2.v.y;
	result.v.z = scale0 * q1.v.z + scale1 * q2.v.z;

	return result;
}
// SlerpQuat  /////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function:	SlerpQuat
// Purpose:		Spherical Linear Interpolation Between two tQuaternions
// Arguments:	Two tQuaternions, blend factor, result tQuaternion
// Notes:		The comments explain the handling of the special cases.
//				The comments in the magazine were wrong.  Adjust the
//				DELTA constant to play with the LERP vs. SLERP level
///////////////////////////////////////////////////////////////////////////////
inline tQuaternion SlerpQuat(tQuaternion q1,tQuaternion q2,double slerp)
{
/// Local Variables ///////////////////////////////////////////////////////////
	tQuaternion result;
	double omega,cosom,sinom,scale0,scale1;
///////////////////////////////////////////////////////////////////////////////
	// USE THE DOT PRODUCT TO GET THE COSINE OF THE ANGLE BETWEEN THE
	// tQuaternionS
     cosom = q1.n * q2.n + 
			q1.v.x * q2.v.x + 
			q1.v.y * q2.v.y + 
			q1.v.z * q2.v.z; 
	
	// CHECK A COUPLE OF SPECIAL CASES. 
	// MAKE SURE THE TWO tQuaternionS ARE NOT EXACTLY OPPOSITE? (WITHIN A LITTLE SLOP)
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
		result.n = scale0 * q1.n + scale1 * q2.n;
		result.v.x = scale0 * q1.v.x + scale1 * q2.v.x;
		result.v.y = scale0 * q1.v.y + scale1 * q2.v.y;
		result.v.z = scale0 * q1.v.z + scale1 * q2.v.z;
	} else {
		// THE tQuaternionS ARE NEARLY OPPOSITE SO TO AVOID A DIVIDED BY ZERO ERROR
		// CALCULATE A PERPENDICULAR tQuaternion AND SLERP THAT DIRECTION
		result.n = -q2.v.x;
		result.v.x = q2.n;
		result.v.y = -q2.v.z;
		result.v.z = q2.v.y;
		scale0 = sin((1.0 - slerp) * (double)HALF_PI);
		scale1 = sin(slerp * (double)HALF_PI);
		result.n = scale0 * q1.n + scale1 * result.n;
		result.v.x = scale0 * q1.v.x + scale1 * result.v.x;
		result.v.y = scale0 * q1.v.y + scale1 * result.v.y;
		result.v.z = scale0 * q1.v.z + scale1 * result.v.z;
	
	}

}

//// MultVectorByMatrix //////////////////////////////////////////////////////
inline void multMatrixMyVector(double M[4][4], double V[4], double destV[4])
{
    int i, j;
	memset(destV, 0, sizeof(double)*4);
  
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			destV[i] += M[i][j]*V[j];
}

inline void multMatrix(double src1[4][4], double src2[4][4], double dest[4][4])
{
	int i, k, j;
	memset(dest, 0, sizeof(double)*16);
  
	for (i = 0; i < 4; i++)
		for ( k = 0; k < 4; k++)
			for (j = 0; j < 4; j++)
				dest[i][k] += src1[i][j]*src2[j][k];
}

// shadow 
inline void CalcShadowMat(GLfloat mat[4][4], tQuaternion Gn, MyVector vL)
{
   GLfloat kk;

   kk= Gn.n * vL.x + Gn.v.x*vL.y + Gn.v.y * vL.z;

   mat[0][0] = kk - vL.x*Gn.n;
   mat[1][0] = 0 - vL.x*Gn.v.x;
   mat[2][0] = 0 - vL.x*Gn.v.y;
   mat[3][0] = 0 - vL.x*Gn.v.z;

   mat[0][1] = 0 - vL.y*Gn.n;
   mat[1][1] = kk - vL.y*Gn.v.x;
   mat[2][1] = 0 - vL.y*Gn.v.y;
   mat[3][1] = 0 - vL.y*Gn.v.z;

   mat[0][2] = 0 - vL.z*Gn.n;
   mat[1][2] = 0 - vL.z*Gn.v.x;
   mat[2][2] = kk- vL.z*Gn.v.y;
   mat[3][2] = 0 - vL.z*Gn.v.z;
   
   mat[0][3] = 0;
   mat[1][3] = 0;
   mat[2][3] = 0;
   mat[3][3] = kk;
}

inline MyVector B_spline(MyVector pos)
{
	GLfloat b_mat[4][4]= {-1,3,-3,1, 3,-6,0,4, -3, 3, 3, 1, 1, 0,0,0};///6
 
}
#endif