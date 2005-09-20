///////////////////////////////////////////////////////////////////////////////
//
// SH4Vector.cpp
//
// Katsuhito AKAHANE (2002/12/27)
//
// Vector計算用のファイル
//
///////////////////////////////////////////////////////////////////////////////
#include "SH4Vector.hpp"
//#include "IHCPrinter.h"
#include <cyg/libc/stdio/stdio.h>
#ifndef WIN32
//#include <mathf.h>
#include <math.h>
#endif

// デフォルトコンストラクタ
Vec3f::Vec3f(){
	data[0] = data[1] = data[2] = data[3] = 0.0f;
}

// コンストラクタ
Vec3f::Vec3f(float ix,float iy,float iz){
	X() = ix;
	Y() = iy;
	Z() = iz;
	data[3] = 0.0f;
}

// デストラクタ
Vec3f::~Vec3f(){}

// 和 *this += b
void Vec3f::add(const Vec3f& b){
#ifdef SH4_EXTENSION
	add4(Data(),const_cast<Vec3f&>(b).Data(),Data());
#else 
	X() += b.X();
	Y() += b.Y();
	Z() += b.Z();
#endif //SH4_EXTENSION
}

// 差 *this -= b
void Vec3f::sub(const Vec3f& b){
#ifdef SH4_EXTENSION
	sub4(Data(),const_cast<Vec3f&>(b).Data(),Data());
#else
	X() -= b.X();
	Y() -= b.Y();
	Z() -= b.Z();
#endif //SH4_EXTENSION
}

// ノルムの掛け算 *this *= b
void Vec3f::multi(const float b){
	X() *= b;
	Y() *= b;
	Z() *= b;
}

// ノルムの割り算 *this /= b
void Vec3f::div(const float b){
	X() /= b;
	Y() /= b;
	Z() /= b;
}

// 内積
float Vec3f::dot(const Vec3f& b) CONST{ // 内積
	float rv;
#ifdef SH4_EXTENSION
	rv = fipr(Data(),const_cast<Vec3f&>(b).Data());
#else
	rv = X()*b.X() + Y()*b.Y() + Z()*b.Z();
#endif //SH4_EXTENSION
	return rv;
}

// 等号
bool Vec3f::equal(const Vec3f& b) const{
	bool rv = true;
	rv = ((X() == b.X()) && (Y() == b.Y()) && (Z() == b.Z()));
	return rv;
}

// 二乗和
float Vec3f::square() CONST {
	return dot(*this);
}

// 距離
float Vec3f::norm() CONST {
	return (float)sqrt(square());
}

// 単位ベクトル化
void Vec3f::unitize(){ div(norm());}

//
// operator
//
Vec3f& Vec3f::operator =(const Vec3f& b){
	X() = b.X();
	Y() = b.Y();
	Z() = b.Z();
	return *this;
}
Vec3f& Vec3f::operator +=(Vec3f& b){
	add(b);
	return *this;
}
Vec3f& Vec3f::operator -=(Vec3f& b){
	sub(b);
	return *this;
}
Vec3f& Vec3f::operator *=(float b){
	multi(b);
	return *this;
}
Vec3f& Vec3f::operator /=(float b){
	div(b);
	return *this;
}
Vec3f Vec3f::operator -() const{
	Vec3f r(*this);
	//r = *this;
	r*=-1;
	return r;
}
Vec3f Vec3f::unit() const{
	Vec3f r(*this);
	//r = *this;
	r.unitize();
	return r;
}

void Vec3f::Print(){
	this->Print("");
}
void Vec3f::Print(char* str){
	printf("Vec3f:%s (%f,%f,%f)\n\r",str,X(),Y(),Z());
}



// 演算子のグローバル定義
Vec3f operator + (const Vec3f& a,const Vec3f& b){
	Vec3f rv(a);
	rv.add(b);
	return rv;
}
Vec3f operator - (const Vec3f& a,const Vec3f& b){
	Vec3f rv(a);
	rv.sub(b);
	return rv;
}
float operator * (const Vec3f& a,const Vec3f& b){ //内積
	return const_cast<Vec3f&>(a).dot(b);
}
Vec3f operator * (const Vec3f& a, float b){
	Vec3f rv(a);
	rv.multi(b);
	return rv;
}

Vec3f operator * (float b, const Vec3f& a){
	Vec3f rv(a);
	rv.multi(b);
	return rv;
}

Vec3f operator / (const Vec3f& a, float b){
	Vec3f rv(a);
	rv.div(b);
	return rv;
}
bool operator == (const Vec3f& a,const Vec3f& b){
	return a.equal(b);
}
bool operator != (const Vec3f& a,const Vec3f& b){
	return !a.equal(b);
}
