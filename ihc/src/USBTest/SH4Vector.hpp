///////////////////////////////////////////////////////////////////////////////
//
// SH4Vector.hpp
//
// Katsuhito AKAHANE (2002/12/27)
//
// Vector計算用のファイル
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _SH4_VECTOR_HPP_
#define _SH4_VECTOR_HPP_
#include <math.h>
//#include "SH4DebugPrint.hpp"

//extern SH4Print* pPrinter;

#ifndef WIN32
//#define SH4_EXTENSION  //SH4 拡張命令 使用
//#include <machine.h>
#include "builtin.inl"
#else
#include <stdio.h>
#endif

#ifdef SH4_EXTENSION
#define CONST 
#else
#define CONST const
#endif

///////////////////////////////////////////////////////////////////////////////
// inline 関数

template <class SC>
inline SC Det2(SC a,SC b,SC c,SC d){
	return ((a)*(d) - (b)*(c));
}

template <class SC>
inline SC Square(SC x){
	return x*x;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Vec3f
class Vec3f{
public:
	float data[4];
public:
	float* Data(){ return data;}

	const float& X() const { return data[0];}
	const float& Y() const { return data[1];}
	const float& Z() const { return data[2];}
	float& X(){ return data[0];}
	float& Y(){ return data[1];}
	float& Z(){ return data[2];}

	void add(const Vec3f& b);
	void sub(const Vec3f& b);
	void multi(const float b);
	void div(const float b);

	float dot(const Vec3f& b) CONST;
	bool equal(const Vec3f& b) const;
	float square() CONST;
	float norm() CONST;
	void unitize();

public:
	Vec3f();
	~Vec3f();
	Vec3f(float ix,float iy,float iz);

	Vec3f& operator =(const Vec3f& b);
	Vec3f& operator +=(Vec3f& b);
	Vec3f& operator -=(Vec3f& b);
	Vec3f& operator *=(float b);
	Vec3f& operator /=(float b);
	Vec3f operator -() const;

	Vec3f unit() const;
	void Print();
	void Print(char* str);
};

// 演算子のグローバル宣言
extern Vec3f operator + (const Vec3f& a,const Vec3f& b);
extern Vec3f operator - (const Vec3f& a,const Vec3f& b);
extern float operator * (const Vec3f& a,const Vec3f& b);
extern Vec3f operator * (const Vec3f& a, float b);
extern Vec3f operator * (float b, const Vec3f& a);
extern Vec3f operator / (const Vec3f& a, float b);
extern bool operator == (const Vec3f& a,const Vec3f& b);
extern bool operator != (const Vec3f& a,const Vec3f& b);

#endif //_SH4_VECTOR_HPP_
