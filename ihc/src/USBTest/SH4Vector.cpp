///////////////////////////////////////////////////////////////////////////////
//
// SH4Vector.cpp
//
// Katsuhito AKAHANE (2002/12/27)
//
// Vector�v�Z�p�̃t�@�C��
//
///////////////////////////////////////////////////////////////////////////////
#include "SH4Vector.hpp"
//#include "IHCPrinter.h"
#include <cyg/libc/stdio/stdio.h>
#ifndef WIN32
//#include <mathf.h>
#include <math.h>
#endif

// �f�t�H���g�R���X�g���N�^
Vec3f::Vec3f(){
	data[0] = data[1] = data[2] = data[3] = 0.0f;
}

// �R���X�g���N�^
Vec3f::Vec3f(float ix,float iy,float iz){
	X() = ix;
	Y() = iy;
	Z() = iz;
	data[3] = 0.0f;
}

// �f�X�g���N�^
Vec3f::~Vec3f(){}

// �a *this += b
void Vec3f::add(const Vec3f& b){
#ifdef SH4_EXTENSION
	add4(Data(),const_cast<Vec3f&>(b).Data(),Data());
#else 
	X() += b.X();
	Y() += b.Y();
	Z() += b.Z();
#endif //SH4_EXTENSION
}

// �� *this -= b
void Vec3f::sub(const Vec3f& b){
#ifdef SH4_EXTENSION
	sub4(Data(),const_cast<Vec3f&>(b).Data(),Data());
#else
	X() -= b.X();
	Y() -= b.Y();
	Z() -= b.Z();
#endif //SH4_EXTENSION
}

// �m�����̊|���Z *this *= b
void Vec3f::multi(const float b){
	X() *= b;
	Y() *= b;
	Z() *= b;
}

// �m�����̊���Z *this /= b
void Vec3f::div(const float b){
	X() /= b;
	Y() /= b;
	Z() /= b;
}

// ����
float Vec3f::dot(const Vec3f& b) CONST{ // ����
	float rv;
#ifdef SH4_EXTENSION
	rv = fipr(Data(),const_cast<Vec3f&>(b).Data());
#else
	rv = X()*b.X() + Y()*b.Y() + Z()*b.Z();
#endif //SH4_EXTENSION
	return rv;
}

// ����
bool Vec3f::equal(const Vec3f& b) const{
	bool rv = true;
	rv = ((X() == b.X()) && (Y() == b.Y()) && (Z() == b.Z()));
	return rv;
}

// ���a
float Vec3f::square() CONST {
	return dot(*this);
}

// ����
float Vec3f::norm() CONST {
	return (float)sqrt(square());
}

// �P�ʃx�N�g����
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



// ���Z�q�̃O���[�o����`
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
float operator * (const Vec3f& a,const Vec3f& b){ //����
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
