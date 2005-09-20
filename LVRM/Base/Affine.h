#ifndef AFFINE_H
#define AFFINE_H
#include "TMatrix.h"
#include "TMatrixUtility.h"

/** @page TAffine �A�t�B���s��N���X���C�u����
	@section intro �͂��߂�
		���̃h�L�������g��TAffine�s��N���X���C�u�����̃h�L�������g�ł��D
		TAffine�s��N���X���C�u�����́C3D�V�~�����[�V�����ɕK�{��
		TAffine�s���C++�̃N���X�ɂ������̂ł��D
	@section usage �g����
	TAffine �s��N���X���C�u�����́C�w�b�_�t�@�C����������Ȃ�
	�N���X���C�u�����Ȃ̂�, TAffine.h, TMatrix.h, TMatrixUtility.h, TVector.h
	�𓯂��t�H���_�ɓ���Ă����C.cpp�t�@�C������w�b�_���C���N���[�h���邾����
	�g�p�ł��܂��D
	@subsection sample �T���v��
	@verbatim
#include "TAffine.h"		//	TAffine�s�񃉃C�u�����̃C���N���[�h����D
#include <iostream>
using namespace LVRM;	//	TAffine�s��N���X��LVRM���O��Ԃ̒��Ő錾����Ă���D
void main(){
	Affinef af(Rad(30), 'z');	//	�v�f��float��TAffine�s���錾. 
								//	z�����30�x��]�s��ɏ�����
	Vec3f vec(1,0,0);			//	�v�f��float��3�����̃x�N�g����錾
	std::cout << af;
	std::cout << vec << std::endl;
	std::cout << af * vec << std::endl;
}@endverbatim
	@subsection af_vec TAffine�s��E�x�N�g���̋@�\
	���ʂɉ��Z���ł��܂��D
	<ul>
	<li> +:�a, -:��, *:��/����, /:�萔����1
	<li> ==:��r, =:���
	<li> <<:�o��, >>:����
	<li> %:�x�N�g���̊O��
	</ul>
	TAffine�ϊ��́C
	@verbatim
	TAffine af; TVec3 v, af_v;
	af_v = af * v;@endverbatim
	�Ƃ���΂ł��܂��D�܂��C���̂悤��TAffine�s��̕��������o�����Ƃ��ł��܂��D
	<ul>
		<li> af.Ex():	X�����x�N�g���D(3�����x�N�g��)�D
		<li> af.Ey():	X�����x�N�g���D(3�����x�N�g��)�D
		<li> af.Ez():	X�����x�N�g���D(3�����x�N�g��)�D
		<li> af.Trn():	���s�ړ������D(3�����x�N�g��)�D
		<li> af.Rot():	��]�ϊ������D(3�~�R�s��)�D
	</ul>
	�����ւ̑���Ȃǂ��ł��܂��D
	@verbatim
	TAffine af;
	af.Pos() = Vec3f(10,0,0);
	af.Rot() = TAffine(Rad(30), 'x').Rot() * af.Rot();@endverbatim
	�x�N�g���͎��̂悤�ȃ����o�֐��������܂��D
	<ul>
	<li> unit(): �������������P�ʃx�N�g����Ԃ��D
	<li> norm(): �x�N�g���̑傫��(�m����)��Ԃ��D
	</ul>
	@subsection affConstract TAffine�s��̏������@�\
	TAffine�s��(LVRM::TAffine)�ɂ͕֗��ȃR���X�g���N�^�⏉�����֐���p�ӂ��܂����D
	�������������ŏЉ�܂��D
	<ul>
		<li> Affinef(float x, float y, float z):		���s�ړ�
		<li> Affinef(Vec3f ex, Vec3f ey, Vec3f pos):	���ڑ��,ez=ex%ey
		<li> Affinef(float rad, char axis):				��]�s�� axis�́C'x', 'y', 'z'
		<li> Affinef(float rad, Vec3f axis):			��]�s��
		<li> Affinef(Vec3f screen, Vec2f size, float f, float b): �ˉe�s��Ƃ��ď������D
		<li> void InitProjectionGL(Vec3f screen, Vec2f size, float f, float b):<br>
			OpenGL�p�ˉe�s��Ƃ��ď�����(-Z���O)�D
			<ul>
				<li> screen   �J�������猩���r���[�|�[�g�̒��S�̈ʒu  
				<li> size   �r���[�|�[�g�̃T�C�Y  
				<li> f      ��O�̃N���b�s���O���ʂƃJ�����̋���  
				<li> b      ���̃N���b�s���O���ʂƃJ�����̋���  
			</ul>
		<li> void LookAtGL(Vec3f posz, Vec3f posy):�ʒu�͂��̂܂܂ŁCposz��-Ez(), posy �� Ey()
			�������悤��TAffine�s��
	</ul>
	
	@section thanks �ӎ�
	LU�����C�t�s��C�K�E�X�����@�Ȃǂ̍s��v�Z�A���S���Y���́C<br>
    �u�w�b����ɂ��ŐV�A���S���Y�����T�x�S�\�[�X�R�[�h�v<br>
    ftp://ftp.matsusaka-u.ac.jp/pub/algorithms<br>
	���� ���F Haruhiko Okumura<br>
	�����ς��ė��p�����Ă��������܂����D
	���R�ɃR�[�h���g�킹�Ă��������āC���肪�Ƃ��������܂��D
	
*/
/**	@file TAffine.h
	2/3�����A�t�B���s���2/3�����x�N�g���D*/
namespace LVRM {
#ifdef _DEBUG
 #pragma optimize ("awgity", on)
 #pragma auto_inline(on)
 #pragma inline_recursion(on)
#endif

/**@defgroup TAffine	2/3�����x�N�g���ETAffine�s��	*/
//@{
#undef M_PI
///	�~������
#ifdef __BORLANDC__
#define M_PI 3.14159265358979323846
#else
const double M_PI = 3.14159265358979323846;
#endif

#undef abs
///	��Βl
template <class T> T abs(T t){
	return t > T()  ?  t  :  -t;
}

#undef sign
///	����(���Ȃ�1, ���Ȃ� -1 ��Ԃ�)
template <class T> T sign(T t){
	return t > T()  ?  1  :  -1;
}
///	�x�����W�A���ɕϊ�
inline double Rad(double deg){
	return ((double)deg/360*2*M_PI);
}
///	���W�A����x�ɕϊ�
inline double Deg(double rad){
	return (rad/(2*M_PI)) * 360;
}
///	2��
template <class SC>
inline SC Square(SC x){
	return x*x;
}
///	2x2�s��̍s��
template <class SC>
inline SC Det2(SC a, SC b, SC c, SC d){
	return ((a)*(d) - (b)*(c));
}

//-----------------------------------------------------------------------------
//		TVec2
/**	2�����x�N�g���N���X	*/
template<class T>
class TVec2:public PTM::TVector<2, T>{
public:
	typedef PTM::TVector<2, T> base_type;
	///	��{�I�ȃ����o�̒�` @see ::DEF_TVECTOR_BASIC_MEMBER
	DEF_TVECTOR_BASIC_MEMBER(TVec2);
	///@name �\�z�E������
	//@{
	///	�R���X�g���N�^1
	TVec2(){ clear(); }
	///	�R���X�g���N�^2
	TVec2(element_type xi, element_type yi){ X() = xi; Y() = yi; }
	//@}
	
	///@name �ϐ��A�N�Z�X
	//@{
	///	�x�N�g����x����
	const element_type& X() const { return item(0); }
	///	�x�N�g����y����
	const element_type& Y() const { return item(1); }
	///	�x�N�g����x����
	element_type& X() { return item(0); }
	///	�x�N�g����y����
	element_type& Y() { return item(1); }
	//@}
};

//-----------------------------------------------------------------------------
//		TMatrix2
/**	2x2�s��N���X.	*/
template <class T>
class TMatrix2:public PTM::TMatrixCol<2,2,T>{
public:
	typedef PTM::TMatrixCol<2,2,T> base_type;
	///	��{�I�ȃ����o�̒�` @see ::DEF_TMATRIX_BASIC_MEMBER
	DEF_TMATRIX_BASIC_MEMBER(TMatrix2);
	/**@name	���x�N�g���ւ̃A�N�Z�X	*/
	//@{
	/// 
	TVec2<element_type>& Ex(){ return *(TVec2<element_type>*)&col(0); }
	/// 
	const TVec2<element_type>& Ex() const{ return *(TVec2<element_type>*)&col(0); }
	/// 
	/// 
	TVec2<element_type>& Ey(){ return *(TVec2<element_type>*)&col(1); }
	/// 
	const TVec2<element_type>& Ey() const{ return *(TVec2<element_type>*)&col(1); }
	//@}
	
	///@name �������ƍ\�z
	//@{
	///	�R���X�g���N�^(�P�ʍs��)
	TMatrix2(){
		Ex().X() = 1, Ey().X()= 0 , Ex().Y() = 0 , Ey().Y() = 1;
	}
	///	�R���X�g���N�^(��]�s��)
	TMatrix2(element_type rad){
        element_type s = sin(rad);
        element_type c = cos(rad);
		Ex().X() = c , Ey().X()= s , Ex().Y() = -s , Ey().Y() = c;
	}
	///	�R���X�g���N�^(���ڎw��)
	template <class TX, class TY>
	TMatrix2(TVec2<TX> exi, TVec2<TY> eyi){
		EX().X() = exi.X();
		Ex().Y() = exi.Y();
		Ey().X() = eyi.X();
		Ey().Y() = eyi.Y();
	}
	//@}

	/**@name �v�f�ւ̃A�N�Z�X */
	//@{
	/// 
	const element_type& ExX() const {return EX().X();}
	element_type& ExX() {return EX().X();}
	/// 
	const element_type& ExY() const {return Ex().Y();}
	element_type& ExY() {return Ex().Y();}
	/// 
	const element_type& EyX() const {return Ey().X();}
	element_type& EyX() {return Ey().X();}
	/// 
	const element_type& EyY() const {return Ey().Y();}
	element_type& EyY() {return Ey().Y();}
	//@}

	//---------------------------------
	//@}
	///@name ���Z
	//@{
	///	�s��
	element_type det(){ return EX().X() * Ey().Y() - Ey().X() * Ex().Y();};
	///	�t�s���Ԃ��D
	TMatrix2 inv() const{
		element_type d = det();
		TMatrix2 rv;
		rv.EX().X() = Ey().Y() / d;
		rv.Ey().X() = -Ey().X() / d;
		rv.Ex().Y() = -Ex().Y() / d;
		rv.Ey().Y() = EX().X() / d;
		return rv;
	}
	//@}
};

//-----------------------------------------------------------------------------
//	TAffine2

/**	TAffine2�s��(��],�g��,���s�ړ���\��)�s��̃N���X.	*/
template <class T>
class TAffine2:public PTM::TMatrixCol<3,3,T>{
public:
	typedef PTM::TMatrixCol<3,3,T> base_type;
	///	��{�I�ȃ����o�̒�` @see ::DEF_TMATRIX_BASIC_MEMBER
	DEF_TMATRIX_BASIC_MEMBER(TAffine2);

	/**@name	���x�N�g���ւ̃A�N�Z�X	*/
	//@{
	/// 
	TVec2<element_type>& Ex(){
		return *(TVec2<element_type>*) &item(0,0);
	}
	/// 
	const TVec2<element_type>& Ex() const{
		return *(TVec2<element_type>*) &item(0,0);
	}
	/// 
	TVec2<element_type>& Ey(){
		return *(TVec2<element_type>*) &item(0,1);
	}
	/// 
	const TVec2<element_type>& Ey() const{
		return *(TVec2<element_type>*) &item(0,1);
	}
	/// 
	TVec2<element_type>& Trn(){
		return *(TVec2<element_type>*) &item(0,2);
	}
	/// 
	const TVec2<element_type>& Trn() const{
		return *(TVec2<element_type>*) &item(0,2);
	}
	/// 
	TVec2<element_type>& Pos(){ return Trn(); }
	/// 
	const TVec2<element_type>& Pos() const { return Trn(); }
	//@}

	/**@name	�v�f�ւ̃A�N�Z�X	*/
	//@{
	/// 
	element_type& ExX() {return Ex().X();}
	/// 
	element_type& ExY() {return Ex().Y();}
	/// 
	element_type& EyX() {return Ey().X();}
	/// 
	element_type& EyY() {return Ey().Y();}
	/// 
	element_type& TrnX() {return Trn().X();}
	/// 
	element_type& TrnY() {return Trn().Y();}
	///	TrnX()�̕ʖ�
	element_type& PosX() {return Trn().X();}
	///	TrnY()�̕ʖ�
	element_type& PosY() {return Trn().Y();}
	//@}

	/**@name	�������ƍ\�z	*/
	///	�P�ʍs��
	void InitUnit(){ PTM::init_unit(*this); }
	///	���s�ړ�
	void InitTrn(element_type px, element_type py){
		InitUnit();
		Trn().X() = px;
		Trn().Y() = py;
	}
	///	x��, �ʒu ���w��
	template <class IX, class BX, class IP, class BP>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& exi,  const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		PTM::init_direct(Rot(), exi, 'x');
		Pos() = posi;
		item(2,2) = 0;
	}
	///	x/y��, �ʒu ���w��
	template <class IX, class BX, class IP, class BP>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& a, char axis, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		PTM::init_direct(Rot(), a, ch);
		Pos() = posi;
		item(2,0) = 0; item(2,1) = 0;	item(2,2) = 1;
	}
	///	x��, y��, �ʒu ���w��
	template <class IX, class BX, class IY, class BY, class IP, class BP>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& exi,
					const PTM::TVectorBaseOp<DIMENC(2), IY, BY>& eyi,
					const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		Ex() = exi;
		Ey() = eyi;
		Pos() = posi;
		item(2,0) = 0; item(2,1) = 0;	item(2,2) = 1;
	}
	///	��]
	template <class IP, class BP>
	void InitRot(element_type th, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		PTM::init_rot(Rot(), th);
		Pos() = posi;
		item(2,0) = 0; item(2,1) = 0;	item(2,2) = 1;
	}
	///	�P�ʍs��
	TAffine2(){InitUnit();}
	///	���s�ړ�
	TAffine2(element_type px, element_type py){InitTrn(px, py);}
	///	���x�N�g���𒼐ڑ��
	template <class IX, class BX, class IY, class BY, class IP, class BP>
	TAffine2(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& exi,
			const PTM::TVectorBaseOp<DIMENC(2), IY, BY>& eyi,
			const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){ InitDirect(exi, eyi, posi); }
	/**	���x�N�g���𒼐ڑ��.
		@param a	axis���̊��x�N�g��
		@param axis	a�̕\����. 'x', 'y' �̂ǂꂩ
	*/
	template <class IX, class BX, class IP, class BP>
	TAffine2(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& a, char axis, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		InitDirect(a, axis, posi);
	}
	/**	��].
		@param th	��]�p(���W�A��)	*/
	///	��]
	template <class IP, class BP>
	TAffine2(element_type th, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		InitRot(th, posi);
	}
	///	2x2�s��ŏ�����
	template <class ITP_R, class ITP_C, class BUF, class ITPV, class BUFV>
	TAffine2(
		const PTM::TMatrixBaseOp<DIMENC(2), DIMENC(2), ITP_R, ITP_C, BUF>& m,
		const PTM::TVectorBaseOp<DIMENC(2), ITPV, BUFV> posi){
		sub_matrix(PTM::TMatDim<2,2>()) = m;
		Trn() = posi;
		sub_matrix(2,0,PTM::TMatDim<1,2>()).clear();
		item(2,2) = 1;
	}
	//@}

	///	��]�g��ϊ��������o��.
	PTM::TSubMatrix<this_type, 2,2> Rot() { return sub_matrix(PTM::TMatDim<2,2>()); }
	///	��]�g��ϊ��������o�� (const��).
	const PTM::TSubMatrix<this_type, 2,2> Rot() const { return sub_matrix(PTM::TMatDim<2,2>()); }
};

//-----------------------------------------------------------------------------
//		TVec3
///	3�����x�N�g���N���X.
template <class T>
class TVec3 :public PTM::TVector<3,T>{
public:
	typedef PTM::TVector<3,T> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec3);

	///@name �ϐ��A�N�Z�X
	//@{
	///	x����
	const element_type& X() const { return item(0); }
	///	y����
	const element_type& Y() const { return item(1); }
	///	z����
	const element_type& Z() const { return item(2); }
	///	x����
	element_type& X(){ return item(0); }
	///	y����
	element_type& Y(){ return item(1); }
	///	z����
	element_type& Z(){ return item(2); }
	/// 
	//@}
	
	///@name �������E�\�z
	//@{
	///	�R���X�g���N�^
	TVec3 (){ clear(); }
	///	�R���X�g���N�^
	TVec3 (element_type xi, element_type yi, element_type zi){ X() = xi; Y() = yi; Z() = zi; }
	//@}
};

//-----------------------------------------------------------------------------
//	TMatrix3

///	3x3�s��N���X.
template  <class T>
class TMatrix3 :public PTM::TMatrixCol<3,3,T>{
public:
	typedef PTM::TMatrixCol<3,3,T> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrix3);

	/**@name	���x�N�g���ւ̃A�N�Z�X	*/
	//@{
	/// 
	TVec3<element_type>& Ex(){ return *(TVec3<element_type>*) &item(0,0); }
	///
	const TVec3<element_type>& Ex() const { return *(TVec3<element_type>*) &item(0,0); }
	/// 
	TVec3<element_type>& Ey(){ return *(TVec3<element_type>*) &item(0,1); }
	///
	const TVec3<element_type>& Ey() const { return *(TVec3<element_type>*) &item(0,1); }
	/// 
	TVec3<element_type>& Ez(){ return *(TVec3<element_type>*) &item(0,2); }
	///
	const TVec3<element_type>& Ez() const { return *(TVec3<element_type>*) &item(0,2); }
	//@}
	/**@name �v�f�ւ̃A�N�Z�X */
	//@{
	/// 
	const element_type& ExX() const {return Ex().X();}
	element_type& ExX() {return Ex().X();}
	/// 
	const element_type& ExY() const {return Ex().Y();}
	element_type& ExY() {return Ex().Y();}
	/// 
	const element_type& ExZ() const {return Ex().Z();}
	element_type& ExZ() {return Ex().Z();}
	/// 
	const element_type& EyX() const {return Ey().X();}
	element_type& EyX() {return Ey().X();}
	/// 
	const element_type& EyY() const {return Ey().Y();}
	element_type& EyY() {return Ey().Y();}
	/// 
	const element_type& EyZ() const {return Ey().Z();}
	element_type& EyZ() {return Ey().Z();}
	/// 
	const element_type& EzX() const {return Ez().X();}
	element_type& EzX() {return Ez().X();}
	/// 
	const element_type& EzY() const {return Ez().Y();}
	element_type& EzY() {return Ez().Y();}
	/// 
	const element_type& EzZ() const {return Ez().Z();}
	element_type& EzZ() {return Ez().Z();}
	//@}
	
	//---------------------------------
	/**@name	�������ƍ\�z	*/
	//@{
	///	�P�ʍs��
	TMatrix3(){ PTM::init_unitize(*this); }
	/// 
	template <class IX, class BX, class IY, class BY>
	TMatrix3(const PTM::TVectorBaseOp<DIMENC(3),IX,BX>& exi, const PTM::TVectorBaseOp<DIMENC(3),IY,BY>&eyi){
		PTM::init_direct(*this, exi, eyi, 'x');
	}
	/// 
	template <class IX, class BX, class IY, class BY, class IZ, class BZ>
	TMatrix3(const PTM::TVectorBaseOp<DIMENC(3),IX,BX>& exi, const PTM::TVectorBaseOp<DIMENC(3),IY,BY>&eyi, const PTM::TVectorBaseOp<DIMENC(3),IZ,BZ>&ezi){
		Ex() = exi; Ey() = eyi; Ez() = ezi;
	}
	///
	template <class IA, class BA, class IB, class BB>
	TMatrix3(const PTM::TVectorBaseOp<DIMENC(3),IA,BA>& a, const PTM::TVectorBaseOp<DIMENC(3),IB,BB>&b, char axis) {
		PTM::init_direct(*this, a, b, axis);
	}
	/// 
	TMatrix3(element_type th, char axis) { PTM::init_rot(*this, th, axis); }
	/**	�C�ӎ��܂���]
		+																	   +
		|u^2+(1-u^2)cos(th)      uv(1-cos(th))-wsin(th)  wu(1-cos(th))+vsin(th)|
	R =	|uv(1-cos(th))+wsin(th)  v^2+(1-v^2)cos(th)      vw(1-cos(th))-usin(th)|
		|wu(1-cos(th))-vsin(th)  vw(1-cos(th))+usin(th)  w^2+(1-w^2)cos(th)    |
		+																	   +
	*/
	template <class I, class B>
		TMatrix3(element_type th, const PTM::TVectorBaseOp<DIMENC(3), I, B>& axis){ PTM::init_rot(*this, th, axis.unit()); }
	///	Qutaternion�����]�s���������
	template <class I, class B>
		TMatrix3(const PTM::TVectorBaseOp<DIMENC(4), I, B>& q){ PTM::init_rot(*this, q); }
	/// 
	template <class I, class B>
	static TMatrix3 Cross(const PTM::TVectorBaseOp<DIMENC(3), I, B>& v){
		TMatrix3 rv;
		PTM::init_cross(rv, v);
		return rv;
	}
	//
	template <class I, class B>
	void InitCross(const PTM::TVectorBaseOp<DIMENC(3), I, B>& v){ PTM::init_cross(*this, v); }
	/// 
	void InitZero(){ clear(); }
	//@}
	
	/**@name �v�Z	*/
	//@{
	///	�s��
	element_type det() const {
		return
		( Ex().X() * Ey().Y() * Ez().Z()  +  Ex().Y() * Ey().Z() * Ez().X()  +  Ex().Z() * Ey().X() * Ez().Y() ) -
		( Ex().Z() * Ey().Y() * Ez().X()  +  Ex().X() * Ey().Z() * Ez().Y()  +  Ex().Y() * Ey().X() * Ez().Z() );
	}
	///	�t�s��
	TMatrix3 inv() const {
		TMatrix3 rtv;
		element_type det_ = 1 / det();
		rtv.Ex().X() = Det2(Ey().Y(), Ez().Y(), Ey().Z(), Ez().Z()) * det_;
		rtv.Ex().Y() = Det2(Ez().Y(), Ex().Y(), Ez().Z(), Ex().Z()) * det_;
		rtv.Ex().Z() = Det2(Ex().Y(), Ey().Y(), Ex().Z(), Ey().Z()) * det_;
			
		rtv.Ey().X() = Det2(Ey().Z(), Ez().Z(), Ey().X(), Ez().X()) * det_;
		rtv.Ey().Y() = Det2(Ez().Z(), Ex().Z(), Ez().X(), Ex().X()) * det_;
		rtv.Ey().Z() = Det2(Ex().Z(), Ey().Z(), Ex().X(), Ey().X()) * det_;
		
		rtv.Ez().X() = Det2(Ey().X(), Ez().X(), Ey().Y(), Ez().Y()) * det_;
		rtv.Ez().Y() = Det2(Ez().X(), Ex().X(), Ez().Y(), Ex().Y()) * det_;
		rtv.Ez().Z() = Det2(Ex().X(), Ey().X(), Ex().Y(), Ey().Y()) * det_;
		return rtv;
	}
};

//-----------------------------------------------------------------------------
//		TVec4
///	4�����x�N�g���N���X.
template <class T>
class TVec4 :public PTM::TVector<4,T>{
public:
	typedef PTM::TVector<4,T> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec4);

	///@name �ϐ��A�N�Z�X
	//@{
	///	x����
	const element_type& X() const { return item(0); }
	///	y����
	const element_type& Y() const { return item(1); }
	///	z����
	const element_type& Z() const { return item(2); }
	///	w����
	const element_type& W() const { return item(3); }
	///	x����
	element_type& X(){ return item(0); }
	///	y����
	element_type& Y(){ return item(1); }
	///	z����
	element_type& Z(){ return item(2); }
	///	z����
	element_type& W(){ return item(3); }
	/// 
	//@}
	
	///@name �������E�\�z
	//@{
	///	�R���X�g���N�^
	TVec4 (){ clear(); }
	///	�R���X�g���N�^
	TVec4 (element_type xi, element_type yi, element_type zi, element_type wi){ X() = xi; Y() = yi; Z() = zi; W() = wi;}
	//@}
};
//-----------------------------------------------------------------------------
//		TQuaternion
///		4�����N���X.
template <class T>
class TQuaternion: public PTM::TVector<4,T>
{
public:
	///	��{�N���X�̌^���`
	typedef PTM::TVector<4,T> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TQuaternion);
    ///	3�����̕����x�N�g��
	typedef PTM::TVectorBaseOp<DIMENC(3), iterator_pair, buffer_type> vector_type;
	///@name �ϐ��A�N�Z�X
	//@{
	///	w����
	const element_type& W() const { return item(0); }
	///	x����
	const element_type& X() const { return item(1); }
	///	y����
	const element_type& Y() const { return item(2); }
	///	z����
	const element_type& Z() const { return item(3); }
	///
	const vector_type& V() const {return sub_vector(1, vector_type());}

	///	z����
	element_type& W(){ return item(0); }
	///	x����
	element_type& X(){ return item(1); }
	///	y����
	element_type& Y(){ return item(2); }
	///	z����
	element_type& Z(){ return item(3); }
	/// 
	vector_type& V() {return sub_vector(1, vector_type());}
	/// 
	vector_type angle() {
		//	W() =cos(theta/2) �Ȃ̂�
		double a = acos(W());
		if (a) return 2*acos(W()) * sub_vector(1, vector_type()).unit();
		vector_type r;
		r.clear();
		return r;
	}
	//@}

	///@name �������E�\�z
	//@{
	///	�R���X�g���N�^
	TQuaternion(){ W() = 1; X() = 0; Y() = 0; Z() = 0;}
	///	�R���X�g���N�^
	TQuaternion(element_type wi, element_type xi, element_type yi, element_type zi){ W() = wi; X() = xi; Y() = yi; Z() = zi;}
	template <class I, class B>
	void InitDirect(element_type a, const PTM::TVectorBaseOp<DIMENC(3), I, B> v){
		W() = a; V() = v;
	}
	template <class I, class B>
	void InitDirect(element_type a, const PTM::TVectorBaseOp<DIMENC(4), I, B> v){
		W() = v[0]; X() = v[1]; Y() = v[2]; Z() = v[3];
	}
	static TQuaternion<T> Rot(element_type angle, const TVec3<element_type>& axis){
		TQuaternion<T> quat;
		PTM::init_quaternion(quat, angle, axis);
		return quat;
	}
	static TQuaternion<T> Rot(element_type angle, char axis){
		TQuaternion<T> quat;
		PTM::init_quaternion(quat, angle, axis);
		return quat;
	}
	//@}
	///����
	void conjugate() { V() = -V(); }
	///	
	TQuaternion conjugated() const { TQuaternion rv(*this); rv.conjugate(); return rv;}
	///�t
	TQuaternion inv() const { return conjugated() / square(); }

	///��]�s��ϊ�
	void from_matrix(const TMatrix3<T>& m)
	{
		W() = sqrt(m[0][0] + m[1][1] + m[2][2] + 1) / 2.0;
		if(abs(W()) >= 0.5){
			X() = (m[1][2] - m[2][1]) / (4 * W());
			Y() = (m[2][0] - m[0][2]) / (4 * W());
			Z() = (m[0][1] - m[1][0]) / (4 * W());
			return;
		}
		if(m[0][0] > m[1][1] && m[0][0] > m[2][2]){
			X() = sqrt(m[0][0] - m[1][1] - m[2][2] + 1) / 2.0;
			Y() = (m[0][1] + m[1][0]) / (4 * X());
			Z() = (m[0][2] + m[2][0]) / (4 * X());
			W() = (m[1][2] - m[2][1]) / (4 * X());
			return;
		}
		if(m[1][1] > m[0][0] && m[1][1] > m[2][2]){
			Y() = sqrt(m[1][1] - m[0][0] - m[2][2] + 1) / 2.0;
			Z() = (m[1][2] + m[2][1]) / (4 * Y());
			X() = (m[1][0] + m[0][1]) / (4 * Y());
			W() = (m[2][0] - m[0][2]) / (4 * Y());
			return;
		}
		Z() = sqrt(m[2][2] - m[0][0] - m[1][1] + 1) / 2.0;
		X() = (m[2][0] + m[0][2]) / (4 * Z());
		Y() = (m[2][1] + m[1][2]) / (4 * Z());
		W() = (m[0][1] - m[1][0]) / (4 * Z());
	}
	void to_matrix(TMatrix3<T>& mat)
	{
		mat[0][0] = 2.0 * (W() * W() + X() * X()) - 1.0;
		mat[1][0] = 2.0 * (X() * Y() + W() * Z());
		mat[2][0] = 2.0 * (X() * Z() - W() * Y());
		mat[0][1] = 2.0 * (Y() * X() - W() * Z());
		mat[1][1] = 2.0 * (W() * W() + Y() * Y()) -1.0;
		mat[2][1] = 2.0 * (Y() * Z() + W() * X());
		mat[0][2] = 2.0 * (Z() * X() + W() * Y());
		mat[1][2] = 2.0 * (Z() * Y() - W() * X());
		mat[2][2] = 2.0 * (W() * W() + Z() * Z()) -1.0;	
	}

	///lhs����]����rhs�Ɉ�v������N�E�H�[�^�j�I��
	void rotation_arc(const TVec3<T>& lhs, const TVec3<T>& rhs)
	{
		TVec3<T> v0, v1, c;
		T d, s;
		v0 = lhs.unit();
		v1 = rhs.unit();
		c = PTM::cross(v0, v1);
		d = PTM::dot(v0, v1);
		s = sqrt((1.0 + d) * 2.0);
		set(c / s, s / 2.0);
	}

	///�I�C���[�p�Ŏw��
	void euler(T yaw, T pitch, T roll) {
		T cosYaw	= std::cos(yaw / 2);
		T sinYaw	= std::sin(yaw / 2);
		T cosPitch	= std::cos(pitch / 2);
		T sinPitch	= std::sin(pitch / 2);
		T cosRoll	= std::cos(roll / 2);
		T sinRoll	= std::sin(roll / 2);
		set(cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
			cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
			sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
			cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw);
	}

};
///	TQuaternion ���m�̊|���Z�D��]�ϊ��Ƃ��ẮC�����ɂȂ�D
template <class T>
inline TQuaternion<T> operator*(const TQuaternion<T>& q1, const TQuaternion<T>& q2){
	return 
    TQuaternion<T>(	
				q1.W() * q2.W() - q1.X() * q2.X() - q1.Y() * q2.Y() - q1.Z() * q2.Z(),
				q1.W() * q2.X() + q1.X() * q2.W() + q1.Y() * q2.Z() - q1.Z() * q2.Y(),
				q1.W() * q2.Y() + q1.Y() * q2.W() + q1.Z() * q2.X() - q1.X() * q2.Z(),
				q1.W() * q2.Z() + q1.Z() * q2.W() + q1.X() * q2.Y() - q1.Y() * q2.X()
				); 
}
///	TQuaternion�Ńx�N�g������]�D TQuaternion * vector * TQuaternion.conjugated() �Ɠ����D
template <class T, class ITP, class BUF>
inline PTM::TVectorBaseOp<DIMENC(3), ITP, BUF> operator*(const TQuaternion<T>& q, const PTM::TVectorBaseOp<DIMENC(3), ITP, BUF>& v){
	TQuaternion<T> qv(1, v[0], v[1], v[2]);
	return (q * qv * q.conjugated()).sub_vector(1, TVec3<T>());
}
///	TQuaternion �̓��ρD
template <class T1, class T2>
inline T1 dot(const TQuaternion<T1>& q1, const TQuaternion<T2>& q2) {
	return q1.X() * q2.X() + q1.Y() * q2.Y() + q1.Z() * q2.Z() + q1.W() * q2.W();
}
//-----------------------------------------------------------------------------
//	TAffine
///	TAffine�s��(��],�g��,���s�ړ���\��)�s��̂��ƂɂȂ�N���X.
template <class T>
class TAffine :public PTM::TMatrixCol<4,4,T>{
public:
	typedef PTM::TMatrixCol<4,4,T> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TAffine);

	/**@name	���x�N�g���ւ̃A�N�Z�X	*/
	//@{
	/// 
	TVec3<element_type>& Ex() { return *(TVec3<element_type>*) &item(0,0); }
	/// 
	const TVec3<element_type>& Ex() const { return *(TVec3<element_type>*) &item(0,0); }
	/// 
	TVec3<element_type>& Ey() { return *(TVec3<element_type>*) &item(0,1); }
	/// 
	const TVec3<element_type>& Ey() const { return *(TVec3<element_type>*) &item(0,1); }
	/// 
	TVec3<element_type>& Ez() { return *(TVec3<element_type>*) &item(0,2); }
	/// 
	const TVec3<element_type>& Ez() const { return *(TVec3<element_type>*) &item(0,2); }
	/// 
	TVec3<element_type>& Trn() { return *(TVec3<element_type>*) &item(0,3); }
	/// 
	const TVec3<element_type>& Trn() const { return *(TVec3<element_type>*) &item(0,3); }
	///	���s�ړ�����(Trn()�̕ʖ�)
	TVec3<element_type>& Pos() {return Trn();}
	///	���s�ړ�����(Trn()�̕ʖ�,const ��)
	const TVec3<element_type>& Pos() const {return Trn();}
	//@}

	/**@name	�v�f�ւ̃A�N�Z�X	*/
	//@{
	/// 
	element_type& ExX() {return Ex().X();}
	const element_type& ExX() const {return Ex().X();}
	/// 
	element_type& ExY() {return Ex().Y();}
	const element_type& ExY() const {return Ex().Y();}
	/// 
	element_type& ExZ() {return Ex().Z();}
	const element_type& ExZ() const {return Ex().Z();}
	/// 
	element_type& EyX() {return Ey().X();}
	const element_type& EyX() const {return Ey().X();}
	/// 
	element_type& EyY() {return Ey().Y();}
	const element_type& EyY() const {return Ey().Y();}
	/// 
	element_type& EyZ() {return Ey().Z();}
	const element_type& EyZ() const {return Ey().Z();}
	/// 
	element_type& EzX() {return Ez().X();}
	const element_type& EzX() const {return Ez().X();}
	/// 
	element_type& EzY() {return Ez().Y();}
	const element_type& EzY() const {return Ez().Y();}
	/// 
	element_type& EzZ() {return Ez().Z();}
	const element_type& EzZ() const {return Ez().Z();}
	/// 
	element_type& TrnX() {return Trn().X();}
	const element_type& TrnX() const {return Trn().X();}
	/// 
	element_type& TrnY() {return Trn().Y();}
	const element_type& TrnY() const {return Trn().Y();}
	/// 
	element_type& TrnZ() {return Trn().Z();}
	const element_type& TrnZ() const {return Trn().Z();}
	///	TrnX()�̕ʖ�
	element_type& PosX() {return TrnX();}
	const element_type& PosX() const {return TrnX();}
	///	TrnY()�̕ʖ�
	element_type& PosY() {return TrnY();}
	const element_type& PosY() const {return TrnY();}
	///	TrnZ()�̕ʖ�
	element_type& PosZ() {return TrnZ();}
	const element_type& PosZ() const {return TrnZ();}
	///
	element_type& ExW() {return item(3,0);}
	const element_type& ExW() const {return item(3,0);}
	///
	element_type& EyW() {return item(3,1);}
	const element_type& EyW() const {return item(3,1);}
	///
	element_type& EzW() {return item(3,2);}
	const element_type& EzW() const {return item(3,2);}
	///
	element_type& TrnW() {return item(3,3);}
	const element_type& TrnW() const {return item(3,3);}
	///
	element_type& PosW() {return item(3,3);}
	const element_type& PosW() const {return item(3,3);}
	//@}

	/**@name	�������ƍ\�z	*/
	///	�P�ʍs��
	void InitUnit(){ PTM::init_unitize(*this); }
	///	���s�ړ�
	void InitTrn(element_type px, element_type py, element_type pz){
		InitUnit();
		Pos().X() = px;	Pos().Y() = py;	Pos().Z() = pz;
	}
	///	x��, y��, �ʒu ���w��
	template <class ITPX, class BUFX, class ITPY, class BUFY, class ITPZ, class BUFZ>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(3),ITPX, BUFX>& exi,
					const PTM::TVectorBaseOp<DIMENC(3),ITPY, BUFY>& eyi,
					const PTM::TVectorBaseOp<DIMENC(3),ITPZ, BUFZ>& posi){
		Ex() = exi;
		Ex().unitize();
		Ey() = ( eyi - (eyi*Ex())*Ex() ).unit();
		Ez() = Ex() % Ey();
		Pos() = posi;
		ExW() = 0; EyW() = 0; EzW() = 0; PosW() = 1;
	}
	///	x/y/z��, y/z/x��, �ʒu ���w��
	template <class ITPX, class BUFX, class ITPY, class BUFY, class ITPZ, class BUFZ>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(3),ITPX, BUFX>& a,
					const PTM::TVectorBaseOp<DIMENC(3),ITPY, BUFY>& b,
					char axis,
					const PTM::TVectorBaseOp<DIMENC(3),ITPZ, BUFZ>& posi){
		PTM::init_direct(Rot(), a, b, axis);
		Pos() = posi;
		ExW() = 0; EyW() = 0; EzW() = 0; PosW() = 1;
	}
	///	x/y/z���܂���]
	template <class ITPP, class BUFP>
	void InitRot(element_type th, char axis, const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		PTM::init_rot(Rot(), th, axis);
		Pos() = posi;
		ExW() = 0; EyW() = 0; EzW() = 0; PosW() = 1;
	}
	/**	�C�ӎ��܂���]
		+																	   +
		|u^2+(1-u^2)cos(th)      uv(1-cos(th))-wsin(th)  wu(1-cos(th))+vsin(th)|
	R =	|uv(1-cos(th))+wsin(th)  v^2+(1-v^2)cos(th)      vw(1-cos(th))-usin(th)|
		|wu(1-cos(th))-vsin(th)  vw(1-cos(th))+usin(th)  w^2+(1-w^2)cos(th)    |
		+																	   +*/
	template <class ITPA, class BUFA, class ITPP, class BUFP>
	void InitRot(element_type th, const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& axis , const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		PTM::init_rot(Rot(), th, axis);
		Pos() = posi;
		ExW() = 0; EyW() = 0; EzW() = 0; PosW() = 1;
	}
	///	��]�x�N�g���ƈʒu�ŏ�����
	template <class ITPA, class BUFA, class ITPP, class BUFP>
	void InitRot(	const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& rvec,
					const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		element_type size=rvec.Size();
		InitRot(size, size > 0 ? rvec/size : TVec3<typename ITPA::element_type>(1,0,0), posi);
	}
	///	�P�ʍs��
	TAffine(){InitUnit();}
	///	���s�ړ�
	TAffine(element_type px, element_type py, element_type pz){InitTrn(px, py, pz);}
	///	���x�N�g���𒼐ڑ��
	template <class ITPX, class BUFX, class ITPY, class BUFY>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPX, BUFX>& exi,
			const PTM::TVectorBaseOp<DIMENC(3),ITPY, BUFY>& eyi){
		InitDirect(exi, eyi, TVec3<element_type>());
	}
	///	���x�N�g���𒼐ڑ��
	template <class ITPX, class BUFX, class ITPY, class BUFY, class ITPP, class BUFP>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPX, BUFX>& exi,
			const PTM::TVectorBaseOp<DIMENC(3),ITPY, BUFY>& eyi,
			const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		InitDirect(exi, eyi, posi);
	}
	/**	���x�N�g���𒼐ڑ��.
		@param a	axis���̊��x�N�g��
		@param b	(axis+1)%3 ���̊��x�N�g��
		@param axis	a�̕\����. 'x', 'y', 'z' �̂ǂꂩ	*/
	template <class ITPA, class BUFA, class ITPB, class BUFB>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& a,
			const PTM::TVectorBaseOp<DIMENC(3),ITPB, BUFB>& b,
			char axis){ InitDirect(a, b, axis, TVec3<element_type>()); }
	/**	���x�N�g���𒼐ڑ��.
		@param a	axis���̊��x�N�g��
		@param b	(axis+1)%3 ���̊��x�N�g��
		@param axis	a�̕\����. 'x', 'y', 'z' �̂ǂꂩ
		@param posi	���s�ړ��̗�	*/
	template <class ITPA, class BUFA, class ITPB, class BUFB, class ITPP, class BUFP>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& a,
			const PTM::TVectorBaseOp<DIMENC(3),ITPB, BUFB>& b,
			char axis,
			const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){ InitDirect(a, b, axis, posi); }
	/**	�������].
		@param th	��]�p(���W�A��)
		@param axis	��]��. 'x', 'y', 'z' �̂ǂꂩ	*/
	TAffine(element_type th, char axis) {InitRot(th, axis, TVec3<element_type>());}
	/**	�������].
		@param th	��]�p(���W�A��)
		@param axis	��]��. 'x', 'y', 'z' �̂ǂꂩ
		@param posi	���s�ړ��̗�	*/
	template <class ITP, class BUF>
	TAffine(element_type th, char axis, const PTM::TVectorBaseOp<DIMENC(3),ITP, BUF>& posi) {InitRot(th, axis , posi);}
	/**	�������].
		@param th	��]�p(���W�A��)
		@param axis	��]��.	*/
	template <class ITPA, class BUFA>
	TAffine(element_type th, const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& axis){
		InitRot(th, axis.unit(), Vec3f());
	}
	/**	�������].
		@param th	��]�p(���W�A��)
		@param axis	��]��.	*/
	template <class ITPA, class BUFA, class ITPP, class BUFP>
	TAffine(element_type th, const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& axis , const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		InitRot(th, axis.unit(), posi);
	}
	/**	OpenGL�̎ˉe�s��Ƃ��ď�����
		@param screen	�J�������猩���r���[�|�[�g�̒��S�̈ʒu
		@param size		�r���[�|�[�g�̃T�C�Y
		@param front	��O�̃N���b�s���O���ʂƃJ�����̋���
		@param back		���̃N���b�s���O���ʂƃJ�����̋���	*/
	template <class ITPS, class BUFS, class ITPZ, class BUFZ>
	void InitProjectionGL(
		const PTM::TVectorBaseOp<DIMENC(3),ITPS, BUFS>& screen,
		const PTM::TVectorBaseOp<DIMENC(2),ITPZ, BUFZ>& size,
		element_type front=1.0f, element_type back=10000.0f){
		PTM::init_projection_gl(*this, screen, size, front, back);
	}
	/**	Direct3D�̎ˉe�s��Ƃ��ď�����
		@param screen	�J�������猩���r���[�|�[�g�̒��S�̈ʒu
		@param size		�r���[�|�[�g�̃T�C�Y
		@param front	��O�̃N���b�s���O���ʂƃJ�����̋���
		@param back		���̃N���b�s���O���ʂƃJ�����̋���	*/
	template <class ITPS, class BUFS, class ITPZ, class BUFZ>
	void InitProjectionD3D(const PTM::TVectorBaseOp<DIMENC(3),ITPS, BUFS>& screen,
	const PTM::TVectorBaseOp<DIMENC(2),ITPZ, BUFZ>& size,
		element_type front=1.0f, element_type back=10000.0f){
		PTM::init_projection_d3d(*this, screen, size, front, back);
	}
	/**	�ˉe�s��Ƃ��ď�����
		@param screen	�J�������猩���r���[�|�[�g�̒��S�̈ʒu
		@param size		�r���[�|�[�g�̃T�C�Y
		@param front	��O�̃N���b�s���O���ʂƃJ�����̋���
		@param back		���̃N���b�s���O���ʂƃJ�����̋���	*/
	template <class ITPS, class BUFS, class ITPZ, class BUFZ>
	TAffine(const PTM::TVectorBaseOp<DIMENC(3),ITPS, BUFS>& screen, const PTM::TVectorBaseOp<DIMENC(2),ITPZ, BUFZ>& size, element_type front=1.0f, element_type back=10000.0f){
		InitProjectionGL(screen, size, front, back);
	}
	///	3x3�s��ŏ�����
	template <class IR, class IC, class BUF, class IV, class BUFV>
	TAffine(const PTM::TMatrixBaseOp<DIMENC(3), DIMENC(3), IR, IC, BUF>& m, const PTM::TVectorBaseOp<DIMENC(3), IV, BUFV> posi){
		Ex().X() = m.Ex().X();
		Ex().Y() = m.Ex().Y();
		Ex().Z() = m.Ex().Z();
		Ey().X() = m.Ey().X();
		Ey().Y() = m.Ey().Y();
		Ey().Z() = m.Ey().Z();
		Ez().X() = m.Ez().X();
		Ez().Y() = m.Ez().Y();
		Ez().Z() = m.Ez().Z();
		Pos().X() = posi.X();
		Pos().Y() = posi.Y();
		Pos().Z() = posi.Z();
		ExW() = 0;
		EyW() = 0;
		EzW() = 0;
		PosW() = 1;
	}
	///	��]�g��ϊ����ւ̎Q�Ƃ�Ԃ�.
	PTM::TSubMatrix<base_type, 3,3>::type& Rot(){ return sub_matrix(PTM::TMatDim<3,3>()); }
	///	��]�g��ϊ����ւ̎Q�Ƃ�Ԃ� (const��).
	const PTM::TSubMatrix<base_type, 3,3>::type& Rot() const { return sub_matrix(PTM::TMatDim<3,3>()); }
	
	///	posi��Ez()��������TAffine�s��
	template <class ITP, class BUF>
	void LookAt(const PTM::TVectorBaseOp<DIMENC(3), ITP, BUF>& posi){
		PTM::init_look_at(*this, posi);
	}
	///	posz��Ez(), posy �� Ey() ��������TAffine�s��
	template <class ITPZ, class BUFZ, class ITPY, class BUFY>
	void LookAt(const PTM::TVectorBaseOp<DIMENC(3), ITPZ, BUFZ>& posz, const PTM::TVectorBaseOp<DIMENC(3), ITPY, BUFY>& posy){
		PTM::init_look_at(*this, posz, posy);
	}
	///	posi��-Ez()��������TAffine�s��
	template <class ITP, class BUF>
	void LookAtGL(const PTM::TVectorBaseOp<DIMENC(3), ITP, BUF>& posi){
		PTM::init_look_at_gl(*this, posi);
	}
	///	posz��-Ez(), posy �� Ey() ��������TAffine�s��
	template <class ITPZ, class BUFZ, class ITPY, class BUFY>
	void LookAtGL(const PTM::TVectorBaseOp<DIMENC(3), ITPZ, BUFZ>& posz, const PTM::TVectorBaseOp<DIMENC(3), ITPY, BUFY>& posy){
		PTM::init_look_at_gl(*this, posz, posy);
	}
};

///	TAffine�ƃx�N�g���̊|���Z
template <class ITP_ROW, class ITP_COL, class BUF, class ITP_V, class BUFV>
PTM::TVector<3, typename ITP_COL::element_type> operator * (
	const PTM::TMatrixBaseOp<DIMENC(4), DIMENC(4), ITP_ROW, ITP_COL, BUF>& a,
	const PTM::TVectorBaseOp<DIMENC(3), ITP_V, BUFV>& b){
	PTM::TVector<3, TYPENAME ITP_COL::element_type> r;
	r = a.sub_matrix(PTM::TMatDim<3,3>()) * b;
	r += a.col(3).sub_vector(PTM::TVecDim<3>());
	return r;
}

///	float��2�����x�N�g��
typedef TVec2<float> Vec2f;
///	double��2�����x�N�g��
typedef TVec2<double> Vec2d;
///	float��3�����x�N�g��
typedef TVec3<float> Vec3f;
///	double��3�����x�N�g��
typedef TVec3<double> Vec3d;
///	float��4�����x�N�g��
typedef TVec4<float> Vec4f;
///	double��4�����x�N�g��
typedef TVec4<double> Vec4d;

///	float��2�~2�s��D
typedef TMatrix2<float> Matrix2f;
///	double��2�~2�s��D
typedef TMatrix2<double> Matrix2d;
///	float��3�~3�s��D
typedef TMatrix3<float> Matrix3f;
///	double��3�~3�s��D
typedef TMatrix3<double> Matrix3d;
///	float��2�������ӂ���s��.
typedef TAffine2<float> Affine2f;
///	double��2�������ӂ���s��.
typedef TAffine2<double> Affine2d;
///	float��3�������ӂ���s��.
typedef TAffine<float> Affinef;
///	double��3�������ӂ���s��.
typedef TAffine<double> Affined;
///	float��TQuaternion.
typedef TQuaternion<float> Quaternionf;
///	double��TQuaternion.
typedef TQuaternion<double> Quaterniond;
//@}

#ifdef _DEBUG
 #pragma optimize ("", on)
 #pragma auto_inline(off)
 #pragma inline_recursion(off)
#endif

}	//	namespace LVRM

#endif
