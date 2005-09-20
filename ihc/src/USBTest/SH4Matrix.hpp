///////////////////////////////////////////////////////////////////////////////
//
// SH4Matrix.hpp
//
// Katsuhito AKAHANE (2002/12/05)
//
// 行列計算用のファイル
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _SH4_MATRIX_HPP_
#define _SH4_MATRIX_HPP_

#include "Sh4Vector.hpp"
///////////////////////////////////////////////////////////////////////////////
// Matrix3f
class Matrix3f{
public:
	float data[4][4];
public:
	float** Data(){ return reinterpret_cast<float**>(data);}

	float& ExX() { return data[0][0];}
	float& ExY() { return data[1][0];}
	float& ExZ() { return data[2][0];}
	const float& ExX() const { return data[0][0];}
	const float& ExY() const { return data[1][0];}
	const float& ExZ() const { return data[2][0];}

	float& EyX() { return data[0][1];}
	float& EyY() { return data[1][1];}
	float& EyZ() { return data[2][1];}
	const float& EyX() const { return data[0][1];}
	const float& EyY() const { return data[1][1];}
	const float& EyZ() const { return data[2][1];}

	float& EzX() { return data[0][2];}
	float& EzY() { return data[1][2];}
	float& EzZ() { return data[2][2];}
	const float& EzX() const { return data[0][2];}
	const float& EzY() const { return data[1][2];}
	const float& EzZ() const { return data[2][2];}

public:
	Matrix3f();
	Matrix3f(Vec3f iex, Vec3f iey, Vec3f iez);
	~Matrix3f();

	Matrix3f& transSelf();
	Matrix3f trans();
	float det();
	Matrix3f inv();

	void add(const Matrix3f& b);
	void sub(const Matrix3f& b);
	bool equal(const Matrix3f& b) const;

	Matrix3f& operator =(const Matrix3f& b);
	Matrix3f& operator +=(const Matrix3f& b);
	Matrix3f& operator -=(const Matrix3f& b);
	Matrix3f& operator *=(float b);
	Matrix3f& operator /=(float b);

	Matrix3f operator - ();


	void Print();
	void Print(char* ch);
};

//
extern bool operator == (const Matrix3f&a, const Matrix3f&b);
extern bool operator != (const Matrix3f&a, const Matrix3f&b);
extern Matrix3f operator +(const Matrix3f&a, const Matrix3f&b);
extern Matrix3f operator -(const Matrix3f&a, const Matrix3f&b);
//extern Matrix3f multi(const Matrix3f& a, const Matrix3f& b);
//extern Vec3f multi(const Vec3f& , const Matrix3f& );

extern Matrix3f operator * (const Matrix3f& a, const Matrix3f& b);
extern Matrix3f operator * (const float a, const Matrix3f& b);
extern Vec3f operator * (const Matrix3f& a, const Vec3f& b);
extern Vec3f operator * (const Vec3f& b, const Matrix3f& a);

#endif //_SH4_MATRIX_HPP_


