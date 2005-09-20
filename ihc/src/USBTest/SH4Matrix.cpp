///////////////////////////////////////////////////////////////////////////////
//
// SH4Matrix.cpp
//
// Katsuhito AKAHANE (2002/12/29)
//
// �s��v�Z�p�̃t�@�C��
//
///////////////////////////////////////////////////////////////////////////////
#include "SH4Matrix.hpp"
//#include "IHCPrinter.h"
#include <cyg/libc/stdio/stdio.h>
// �f�t�H���g�R���X�g���N�^
Matrix3f::Matrix3f(){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			data[i][j] = 0.0f;
		}
	}
}

// �R���X�g���N�^
Matrix3f::Matrix3f(Vec3f iex, Vec3f iey, Vec3f iez){
	ExX() = iex.X();
	ExY() = iex.Y();
	ExZ() = iex.Z();

	EyX() = iey.X();
	EyY() = iey.Y();
	EyZ() = iey.Z();

	EzX() = iez.X();
	EzY() = iez.Y();
	EzZ() = iez.Z();

	int i;
	for(i=0;i<4;i++){
		data[i][3] = data[3][i] = 0.0f;
	}
}

// �f�X�g���N�^
Matrix3f::~Matrix3f(){
}

// �]�u�s��
Matrix3f& Matrix3f::transSelf(){
	float temp;
	temp = ExY(); ExY() = EyX(); EyX() = temp;
	temp = ExZ(); ExZ() = EzX(); EzX() = temp;
	temp = EyZ(); EyZ() = EzY(); EzY() = temp;
	return *this;
}
// �]�u�s��
Matrix3f Matrix3f::trans(){
	Matrix3f aMat(*this);
	float temp;
	temp = aMat.ExY(); aMat.ExY() = aMat.EyX(); aMat.EyX() = temp;
	temp = aMat.ExZ(); aMat.ExZ() = aMat.EzX(); aMat.EzX() = temp;
	temp = aMat.EyZ(); aMat.EyZ() = aMat.EzY(); aMat.EzY() = temp;
	return aMat;
}

// �s��
float Matrix3f::det(){
	return
	(ExX() * EyY() * EzZ() + EyX() * EzY() * ExZ() + EzX() * ExY() * EyZ()) -
	(EzX() * EyY() * ExZ() + ExX() * EzY() * EyZ() + EyX() * ExY() * EzZ());
}

// �t�s��
Matrix3f Matrix3f::inv(){
	Matrix3f rtv;
	float det_ = 1/det();
	rtv.ExX() = Det2(EyY(), EzY(), EyZ(), EzZ()) * det_;
	rtv.ExY() = Det2(EzY(), ExY(), EzZ(), ExZ()) * det_;
	rtv.ExZ() = Det2(ExY(), EyY(), ExZ(), EyZ()) * det_;
	
	rtv.EyX() = Det2(EyZ(), EzZ(), EyX(), EzX()) * det_;
	rtv.EyY() = Det2(EzZ(), ExZ(), EzX(), ExX()) * det_;
	rtv.EyZ() = Det2(ExZ(), EyZ(), ExX(), EyX()) * det_;
	
	rtv.EzX() = Det2(EyX(), EzX(), EyY(), EzY()) * det_;
	rtv.EzY() = Det2(EzX(), ExX(), EzY(), ExY()) * det_;
	rtv.EzZ() = Det2(ExX(), EyX(), ExY(), EyY()) * det_;
	
	return rtv;
}

// *this += b
void Matrix3f::add(const Matrix3f& b){
#ifdef SH4_EXTENSION
	add4(Data()[0],const_cast<Matrix3f&>(b).Data()[0],Data()[0]);
	add4(Data()[1],const_cast<Matrix3f&>(b).Data()[1],Data()[1]);
	add4(Data()[2],const_cast<Matrix3f&>(b).Data()[2],Data()[2]);
#else
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			data[i][j] += b.data[i][j];
		}
	}
#endif //SH4_EXTENSION
}

// *this -= b
void Matrix3f::sub(const Matrix3f& b){
#ifdef SH4_EXTENSION
	sub4(Data()[0],const_cast<Matrix3f&>(b).Data()[0],Data()[0]);
	sub4(Data()[1],const_cast<Matrix3f&>(b).Data()[1],Data()[1]);
	sub4(Data()[2],const_cast<Matrix3f&>(b).Data()[2],Data()[2]);
#else
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			data[i][j] -= b.data[i][j];
		}
	}
#endif //SH4_EXTENSION
}

// *this == b
bool Matrix3f::equal(const Matrix3f& b) const{
	bool rv = true;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			rv &= (data[i][j] == b.data[i][j]);
		}
	}
	return rv;
}

Matrix3f Matrix3f::operator - (){
	Matrix3f r(*this);
	r*=-1;
	return r;
}

// ������Z�q
Matrix3f& Matrix3f::operator = (const Matrix3f& b){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			data[i][j] = b.data[i][j];
		}
	}
	return *this;
}
Matrix3f& Matrix3f::operator += (const Matrix3f& b){
	add(b);
	return *this;
}
Matrix3f& Matrix3f::operator -= (const Matrix3f& b){
	sub(b);
	return *this;
}
Matrix3f& Matrix3f::operator*=(float b){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			data[i][j] *= b;
		}
	}
	return *this;
}
Matrix3f& Matrix3f::operator/=(float b){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			data[i][j] /= b;
		}
	}
	return *this;
}

// �o��
void Matrix3f::Print(){ this->Print(""); }
void Matrix3f::Print(char* ch){
	printf("Matrix3f:%s\n\r(%f,%f,%f)\n\r(%f,%f,%f)\n\r(%f,%f,%f)\n\r",ch,
		ExX(),EyX(),EzX(),
		ExY(),EyY(),EzY(),
		ExZ(),EyZ(),EzZ()
		);
}


// ���Z�q�̃O���[�o����`
bool operator == (const Matrix3f&a, const Matrix3f&b){
	return a.equal(b);
}
bool operator != (const Matrix3f&a, const Matrix3f&b){
	return !a.equal(b);
}
Matrix3f operator +(const Matrix3f&a, const Matrix3f&b){
	Matrix3f r(a);
	r += b;
	return r;
}
Matrix3f operator -(const Matrix3f&a, const Matrix3f&b){
	Matrix3f r(a);
	r -= b;
	return r;
}

//
// SH4�s�񉉎Z���C�u�����g�p�@�s�񓯎m�̊|���Z
//
Matrix3f multi(const Matrix3f& b, const Matrix3f& a){ //�s��̊|���Z b*a
	Matrix3f r;
#ifdef SH4_EXTENSION
	//int maskdata = get_imask();
	//set_imask(15);
	ld_ext((float(*)[4]) const_cast<Matrix3f&>(a).Data());
	mtrx4mul((float(*)[4])const_cast<Matrix3f&>(b).Data(),(float(*)[4])r.Data());
	//set_imask(maskdata);
#else
	r.ExX() = a.ExX()*b.ExX() + a.EyX()*b.ExY() + a.EzX()*b.ExY();
	r.ExY() = a.ExY()*b.ExX() + a.EyY()*b.ExY() + a.EzY()*b.ExY();
	r.ExZ() = a.ExZ()*b.ExX() + a.EyZ()*b.ExY() + a.EzZ()*b.ExY();

	r.EyX() = a.ExX()*b.EyX() + a.EyX()*b.EyY() + a.EzX()*b.EyY();
	r.EyY() = a.ExY()*b.EyX() + a.EyY()*b.EyY() + a.EzY()*b.EyY();
	r.EyZ() = a.ExZ()*b.EyX() + a.EyZ()*b.EyY() + a.EzZ()*b.EyY();

	r.EzX() = a.ExX()*b.EzX() + a.EyX()*b.EzY() + a.EzX()*b.EzY();
	r.EzY() = a.ExY()*b.EzX() + a.EyY()*b.EzY() + a.EzY()*b.EzY();
	r.EzZ() = a.ExZ()*b.EzX() + a.EyZ()*b.EzY() + a.EzZ()*b.EzY();
#endif //SH4_EXTENSION
	return r;
}

//
// SH4�s�񉉎Z���C�u�����g�p�@�x�N�g���ƍs��̊|���Z
//
Vec3f multi(const Vec3f& b, const Matrix3f& a){ //�x�N�g���ƍs��̐� b*a
	Vec3f r;
#ifdef SH4_EXTENSION
	//int maskdata = get_imask();
	//set_imask(15);
	ld_ext((float(*)[4]) const_cast<Matrix3f&>(a).Data());
	ftrv(const_cast<Vec3f&>(b).Data(),r.Data());
	//set_imask(maskdata);
#else
	/*
	r.X() = a.ExX()*b.X() + a.EyX()*b.Y() + a.EzX()*b.Z();
	r.Y() = a.ExY()*b.X() + a.EyY()*b.Y() + a.EzY()*b.Z();
	r.Z() = a.ExZ()*b.X() + a.EyZ()*b.Y() + a.EzZ()*b.Z();
	*/
	r.X() = a.ExX()*b.X() + a.ExY()*b.Y() + a.ExZ()*b.Z();
	r.Y() = a.EyX()*b.X() + a.EyY()*b.Y() + a.EyZ()*b.Z();
	r.Z() = a.EzX()*b.X() + a.EzY()*b.Y() + a.EzZ()*b.Z();
#endif //SH4_EXTENSION
	return r;
}
// A*B
Matrix3f operator * (const Matrix3f& a, const Matrix3f& b){
	return multi(a,b);
	//return multi(b,a);
}

// a*A
Matrix3f operator * (const float a, const Matrix3f& b){
	Matrix3f r(b);
	r *= a;
	return r;
}

// A*v
Vec3f operator * (const Matrix3f& a, const Vec3f& b){
	//return multi(const_cast<Matrix3f&>(a).trans(),b);
	return multi(b,const_cast<Matrix3f&>(a).trans());
}
//v*A
Vec3f operator * (const Vec3f& a, const Matrix3f& b){
	/*
#ifdef SH4_EXTENSION
	Vec3f r;
	ld_ext((float(*)[4]) const_cast<Matrix3f&>(b).Data());
	ftrv(const_cast<Vec3f&>(a).Data(),r.Data());
	return r;
#else
	*/
	return multi(a,b);
//#endif
}
