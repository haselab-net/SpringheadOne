#ifndef AFFINE_H
#define AFFINE_H
#include "TMatrix.h"
#include "TMatrixUtility.h"

/** @page TAffine アフィン行列クラスライブラリ
	@section intro はじめに
		このドキュメントはTAffine行列クラスライブラリのドキュメントです．
		TAffine行列クラスライブラリは，3Dシミュレーションに必須な
		TAffine行列をC++のクラスにしたものです．
	@section usage 使い方
	TAffine 行列クラスライブラリは，ヘッダファイルだけからなる
	クラスライブラリなので, TAffine.h, TMatrix.h, TMatrixUtility.h, TVector.h
	を同じフォルダに入れておき，.cppファイルからヘッダをインクルードするだけで
	使用できます．
	@subsection sample サンプル
	@verbatim
#include "TAffine.h"		//	TAffine行列ライブラリのインクルードする．
#include <iostream>
using namespace LVRM;	//	TAffine行列クラスはLVRM名前空間の中で宣言されている．
void main(){
	Affinef af(Rad(30), 'z');	//	要素がfloatなTAffine行列を宣言. 
								//	z軸回り30度回転行列に初期化
	Vec3f vec(1,0,0);			//	要素がfloatな3次元のベクトルを宣言
	std::cout << af;
	std::cout << vec << std::endl;
	std::cout << af * vec << std::endl;
}@endverbatim
	@subsection af_vec TAffine行列・ベクトルの機能
	普通に演算ができます．
	<ul>
	<li> +:和, -:差, *:積/内積, /:定数分の1
	<li> ==:比較, =:代入
	<li> <<:出力, >>:入力
	<li> %:ベクトルの外積
	</ul>
	TAffine変換は，
	@verbatim
	TAffine af; TVec3 v, af_v;
	af_v = af * v;@endverbatim
	とすればできます．また，次のようにTAffine行列の部分を取り出すことができます．
	<ul>
		<li> af.Ex():	X軸基底ベクトル．(3次元ベクトル)．
		<li> af.Ey():	X軸基底ベクトル．(3次元ベクトル)．
		<li> af.Ez():	X軸基底ベクトル．(3次元ベクトル)．
		<li> af.Trn():	平行移動部分．(3次元ベクトル)．
		<li> af.Rot():	回転変換部分．(3×３行列)．
	</ul>
	部分への代入などもできます．
	@verbatim
	TAffine af;
	af.Pos() = Vec3f(10,0,0);
	af.Rot() = TAffine(Rad(30), 'x').Rot() * af.Rot();@endverbatim
	ベクトルは次のようなメンバ関数を持ちます．
	<ul>
	<li> unit(): 向きが等しい単位ベクトルを返す．
	<li> norm(): ベクトルの大きさ(ノルム)を返す．
	</ul>
	@subsection affConstract TAffine行列の初期化機能
	TAffine行列(LVRM::TAffine)には便利なコンストラクタや初期化関数を用意しました．
	いくつかをここで紹介します．
	<ul>
		<li> Affinef(float x, float y, float z):		平行移動
		<li> Affinef(Vec3f ex, Vec3f ey, Vec3f pos):	直接代入,ez=ex%ey
		<li> Affinef(float rad, char axis):				回転行列 axisは，'x', 'y', 'z'
		<li> Affinef(float rad, Vec3f axis):			回転行列
		<li> Affinef(Vec3f screen, Vec2f size, float f, float b): 射影行列として初期化．
		<li> void InitProjectionGL(Vec3f screen, Vec2f size, float f, float b):<br>
			OpenGL用射影行列として初期化(-Zが前)．
			<ul>
				<li> screen   カメラから見たビューポートの中心の位置  
				<li> size   ビューポートのサイズ  
				<li> f      手前のクリッピング平面とカメラの距離  
				<li> b      奥のクリッピング平面とカメラの距離  
			</ul>
		<li> void LookAtGL(Vec3f posz, Vec3f posy):位置はそのままで，poszに-Ez(), posy に Ey()
			が向くようなTAffine行列
	</ul>
	
	@section thanks 謝辞
	LU分解，逆行列，ガウス消去法などの行列計算アルゴリズムは，<br>
    「『Ｃ言語による最新アルゴリズム事典』全ソースコード」<br>
    ftp://ftp.matsusaka-u.ac.jp/pub/algorithms<br>
	奥村 晴彦 Haruhiko Okumura<br>
	を改変して流用させていただきました．
	自由にコードを使わせてくださって，ありがとうございます．
	
*/
/**	@file TAffine.h
	2/3次元アフィン行列と2/3次元ベクトル．*/
namespace LVRM {
#ifdef _DEBUG
 #pragma optimize ("awgity", on)
 #pragma auto_inline(on)
 #pragma inline_recursion(on)
#endif

/**@defgroup TAffine	2/3次元ベクトル・TAffine行列	*/
//@{
#undef M_PI
///	円周率π
#ifdef __BORLANDC__
#define M_PI 3.14159265358979323846
#else
const double M_PI = 3.14159265358979323846;
#endif

#undef abs
///	絶対値
template <class T> T abs(T t){
	return t > T()  ?  t  :  -t;
}

#undef sign
///	符号(正なら1, 負なら -1 を返す)
template <class T> T sign(T t){
	return t > T()  ?  1  :  -1;
}
///	度をラジアンに変換
inline double Rad(double deg){
	return ((double)deg/360*2*M_PI);
}
///	ラジアンを度に変換
inline double Deg(double rad){
	return (rad/(2*M_PI)) * 360;
}
///	2乗
template <class SC>
inline SC Square(SC x){
	return x*x;
}
///	2x2行列の行列式
template <class SC>
inline SC Det2(SC a, SC b, SC c, SC d){
	return ((a)*(d) - (b)*(c));
}

//-----------------------------------------------------------------------------
//		TVec2
/**	2次元ベクトルクラス	*/
template<class T>
class TVec2:public PTM::TVector<2, T>{
public:
	typedef PTM::TVector<2, T> base_type;
	///	基本的なメンバの定義 @see ::DEF_TVECTOR_BASIC_MEMBER
	DEF_TVECTOR_BASIC_MEMBER(TVec2);
	///@name 構築・初期化
	//@{
	///	コンストラクタ1
	TVec2(){ clear(); }
	///	コンストラクタ2
	TVec2(element_type xi, element_type yi){ X() = xi; Y() = yi; }
	//@}
	
	///@name 変数アクセス
	//@{
	///	ベクトルのx成分
	const element_type& X() const { return item(0); }
	///	ベクトルのy成分
	const element_type& Y() const { return item(1); }
	///	ベクトルのx成分
	element_type& X() { return item(0); }
	///	ベクトルのy成分
	element_type& Y() { return item(1); }
	//@}
};

//-----------------------------------------------------------------------------
//		TMatrix2
/**	2x2行列クラス.	*/
template <class T>
class TMatrix2:public PTM::TMatrixCol<2,2,T>{
public:
	typedef PTM::TMatrixCol<2,2,T> base_type;
	///	基本的なメンバの定義 @see ::DEF_TMATRIX_BASIC_MEMBER
	DEF_TMATRIX_BASIC_MEMBER(TMatrix2);
	/**@name	基底ベクトルへのアクセス	*/
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
	
	///@name 初期化と構築
	//@{
	///	コンストラクタ(単位行列)
	TMatrix2(){
		Ex().X() = 1, Ey().X()= 0 , Ex().Y() = 0 , Ey().Y() = 1;
	}
	///	コンストラクタ(回転行列)
	TMatrix2(element_type rad){
        element_type s = sin(rad);
        element_type c = cos(rad);
		Ex().X() = c , Ey().X()= s , Ex().Y() = -s , Ey().Y() = c;
	}
	///	コンストラクタ(直接指定)
	template <class TX, class TY>
	TMatrix2(TVec2<TX> exi, TVec2<TY> eyi){
		EX().X() = exi.X();
		Ex().Y() = exi.Y();
		Ey().X() = eyi.X();
		Ey().Y() = eyi.Y();
	}
	//@}

	/**@name 要素へのアクセス */
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
	///@name 演算
	//@{
	///	行列式
	element_type det(){ return EX().X() * Ey().Y() - Ey().X() * Ex().Y();};
	///	逆行列を返す．
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

/**	TAffine2行列(回転,拡大,平行移動を表す)行列のクラス.	*/
template <class T>
class TAffine2:public PTM::TMatrixCol<3,3,T>{
public:
	typedef PTM::TMatrixCol<3,3,T> base_type;
	///	基本的なメンバの定義 @see ::DEF_TMATRIX_BASIC_MEMBER
	DEF_TMATRIX_BASIC_MEMBER(TAffine2);

	/**@name	基底ベクトルへのアクセス	*/
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

	/**@name	要素へのアクセス	*/
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
	///	TrnX()の別名
	element_type& PosX() {return Trn().X();}
	///	TrnY()の別名
	element_type& PosY() {return Trn().Y();}
	//@}

	/**@name	初期化と構築	*/
	///	単位行列
	void InitUnit(){ PTM::init_unit(*this); }
	///	平行移動
	void InitTrn(element_type px, element_type py){
		InitUnit();
		Trn().X() = px;
		Trn().Y() = py;
	}
	///	x軸, 位置 を指定
	template <class IX, class BX, class IP, class BP>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& exi,  const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		PTM::init_direct(Rot(), exi, 'x');
		Pos() = posi;
		item(2,2) = 0;
	}
	///	x/y軸, 位置 を指定
	template <class IX, class BX, class IP, class BP>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& a, char axis, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		PTM::init_direct(Rot(), a, ch);
		Pos() = posi;
		item(2,0) = 0; item(2,1) = 0;	item(2,2) = 1;
	}
	///	x軸, y軸, 位置 を指定
	template <class IX, class BX, class IY, class BY, class IP, class BP>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& exi,
					const PTM::TVectorBaseOp<DIMENC(2), IY, BY>& eyi,
					const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		Ex() = exi;
		Ey() = eyi;
		Pos() = posi;
		item(2,0) = 0; item(2,1) = 0;	item(2,2) = 1;
	}
	///	回転
	template <class IP, class BP>
	void InitRot(element_type th, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		PTM::init_rot(Rot(), th);
		Pos() = posi;
		item(2,0) = 0; item(2,1) = 0;	item(2,2) = 1;
	}
	///	単位行列
	TAffine2(){InitUnit();}
	///	平行移動
	TAffine2(element_type px, element_type py){InitTrn(px, py);}
	///	基底ベクトルを直接代入
	template <class IX, class BX, class IY, class BY, class IP, class BP>
	TAffine2(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& exi,
			const PTM::TVectorBaseOp<DIMENC(2), IY, BY>& eyi,
			const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){ InitDirect(exi, eyi, posi); }
	/**	基底ベクトルを直接代入.
		@param a	axis軸の基底ベクトル
		@param axis	aの表す軸. 'x', 'y' のどれか
	*/
	template <class IX, class BX, class IP, class BP>
	TAffine2(const PTM::TVectorBaseOp<DIMENC(2), IX, BX>& a, char axis, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		InitDirect(a, axis, posi);
	}
	/**	回転.
		@param th	回転角(ラジアン)	*/
	///	回転
	template <class IP, class BP>
	TAffine2(element_type th, const PTM::TVectorBaseOp<DIMENC(2), IP, BP>& posi){
		InitRot(th, posi);
	}
	///	2x2行列で初期化
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

	///	回転拡大変換部を取り出す.
	PTM::TSubMatrix<this_type, 2,2> Rot() { return sub_matrix(PTM::TMatDim<2,2>()); }
	///	回転拡大変換部を取り出す (const版).
	const PTM::TSubMatrix<this_type, 2,2> Rot() const { return sub_matrix(PTM::TMatDim<2,2>()); }
};

//-----------------------------------------------------------------------------
//		TVec3
///	3次元ベクトルクラス.
template <class T>
class TVec3 :public PTM::TVector<3,T>{
public:
	typedef PTM::TVector<3,T> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec3);

	///@name 変数アクセス
	//@{
	///	x成分
	const element_type& X() const { return item(0); }
	///	y成分
	const element_type& Y() const { return item(1); }
	///	z成分
	const element_type& Z() const { return item(2); }
	///	x成分
	element_type& X(){ return item(0); }
	///	y成分
	element_type& Y(){ return item(1); }
	///	z成分
	element_type& Z(){ return item(2); }
	/// 
	//@}
	
	///@name 初期化・構築
	//@{
	///	コンストラクタ
	TVec3 (){ clear(); }
	///	コンストラクタ
	TVec3 (element_type xi, element_type yi, element_type zi){ X() = xi; Y() = yi; Z() = zi; }
	//@}
};

//-----------------------------------------------------------------------------
//	TMatrix3

///	3x3行列クラス.
template  <class T>
class TMatrix3 :public PTM::TMatrixCol<3,3,T>{
public:
	typedef PTM::TMatrixCol<3,3,T> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrix3);

	/**@name	基底ベクトルへのアクセス	*/
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
	/**@name 要素へのアクセス */
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
	/**@name	初期化と構築	*/
	//@{
	///	単位行列
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
	/**	任意軸まわり回転
		+																	   +
		|u^2+(1-u^2)cos(th)      uv(1-cos(th))-wsin(th)  wu(1-cos(th))+vsin(th)|
	R =	|uv(1-cos(th))+wsin(th)  v^2+(1-v^2)cos(th)      vw(1-cos(th))-usin(th)|
		|wu(1-cos(th))-vsin(th)  vw(1-cos(th))+usin(th)  w^2+(1-w^2)cos(th)    |
		+																	   +
	*/
	template <class I, class B>
		TMatrix3(element_type th, const PTM::TVectorBaseOp<DIMENC(3), I, B>& axis){ PTM::init_rot(*this, th, axis.unit()); }
	///	Qutaternionから回転行列を初期化
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
	
	/**@name 計算	*/
	//@{
	///	行列式
	element_type det() const {
		return
		( Ex().X() * Ey().Y() * Ez().Z()  +  Ex().Y() * Ey().Z() * Ez().X()  +  Ex().Z() * Ey().X() * Ez().Y() ) -
		( Ex().Z() * Ey().Y() * Ez().X()  +  Ex().X() * Ey().Z() * Ez().Y()  +  Ex().Y() * Ey().X() * Ez().Z() );
	}
	///	逆行列
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
///	4次元ベクトルクラス.
template <class T>
class TVec4 :public PTM::TVector<4,T>{
public:
	typedef PTM::TVector<4,T> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec4);

	///@name 変数アクセス
	//@{
	///	x成分
	const element_type& X() const { return item(0); }
	///	y成分
	const element_type& Y() const { return item(1); }
	///	z成分
	const element_type& Z() const { return item(2); }
	///	w成分
	const element_type& W() const { return item(3); }
	///	x成分
	element_type& X(){ return item(0); }
	///	y成分
	element_type& Y(){ return item(1); }
	///	z成分
	element_type& Z(){ return item(2); }
	///	z成分
	element_type& W(){ return item(3); }
	/// 
	//@}
	
	///@name 初期化・構築
	//@{
	///	コンストラクタ
	TVec4 (){ clear(); }
	///	コンストラクタ
	TVec4 (element_type xi, element_type yi, element_type zi, element_type wi){ X() = xi; Y() = yi; Z() = zi; W() = wi;}
	//@}
};
//-----------------------------------------------------------------------------
//		TQuaternion
///		4元数クラス.
template <class T>
class TQuaternion: public PTM::TVector<4,T>
{
public:
	///	基本クラスの型を定義
	typedef PTM::TVector<4,T> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TQuaternion);
    ///	3次元の部分ベクトル
	typedef PTM::TVectorBaseOp<DIMENC(3), iterator_pair, buffer_type> vector_type;
	///@name 変数アクセス
	//@{
	///	w成分
	const element_type& W() const { return item(0); }
	///	x成分
	const element_type& X() const { return item(1); }
	///	y成分
	const element_type& Y() const { return item(2); }
	///	z成分
	const element_type& Z() const { return item(3); }
	///
	const vector_type& V() const {return sub_vector(1, vector_type());}

	///	z成分
	element_type& W(){ return item(0); }
	///	x成分
	element_type& X(){ return item(1); }
	///	y成分
	element_type& Y(){ return item(2); }
	///	z成分
	element_type& Z(){ return item(3); }
	/// 
	vector_type& V() {return sub_vector(1, vector_type());}
	/// 
	vector_type angle() {
		//	W() =cos(theta/2) なので
		double a = acos(W());
		if (a) return 2*acos(W()) * sub_vector(1, vector_type()).unit();
		vector_type r;
		r.clear();
		return r;
	}
	//@}

	///@name 初期化・構築
	//@{
	///	コンストラクタ
	TQuaternion(){ W() = 1; X() = 0; Y() = 0; Z() = 0;}
	///	コンストラクタ
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
	///共役
	void conjugate() { V() = -V(); }
	///	
	TQuaternion conjugated() const { TQuaternion rv(*this); rv.conjugate(); return rv;}
	///逆
	TQuaternion inv() const { return conjugated() / square(); }

	///回転行列変換
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

	///lhsを回転してrhsに一致させるクウォータニオン
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

	///オイラー角で指定
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
///	TQuaternion 同士の掛け算．回転変換としては，合成になる．
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
///	TQuaternionでベクトルを回転． TQuaternion * vector * TQuaternion.conjugated() と同じ．
template <class T, class ITP, class BUF>
inline PTM::TVectorBaseOp<DIMENC(3), ITP, BUF> operator*(const TQuaternion<T>& q, const PTM::TVectorBaseOp<DIMENC(3), ITP, BUF>& v){
	TQuaternion<T> qv(1, v[0], v[1], v[2]);
	return (q * qv * q.conjugated()).sub_vector(1, TVec3<T>());
}
///	TQuaternion の内積．
template <class T1, class T2>
inline T1 dot(const TQuaternion<T1>& q1, const TQuaternion<T2>& q2) {
	return q1.X() * q2.X() + q1.Y() * q2.Y() + q1.Z() * q2.Z() + q1.W() * q2.W();
}
//-----------------------------------------------------------------------------
//	TAffine
///	TAffine行列(回転,拡大,平行移動を表す)行列のもとになるクラス.
template <class T>
class TAffine :public PTM::TMatrixCol<4,4,T>{
public:
	typedef PTM::TMatrixCol<4,4,T> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TAffine);

	/**@name	基底ベクトルへのアクセス	*/
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
	///	平行移動成分(Trn()の別名)
	TVec3<element_type>& Pos() {return Trn();}
	///	平行移動成分(Trn()の別名,const 版)
	const TVec3<element_type>& Pos() const {return Trn();}
	//@}

	/**@name	要素へのアクセス	*/
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
	///	TrnX()の別名
	element_type& PosX() {return TrnX();}
	const element_type& PosX() const {return TrnX();}
	///	TrnY()の別名
	element_type& PosY() {return TrnY();}
	const element_type& PosY() const {return TrnY();}
	///	TrnZ()の別名
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

	/**@name	初期化と構築	*/
	///	単位行列
	void InitUnit(){ PTM::init_unitize(*this); }
	///	平行移動
	void InitTrn(element_type px, element_type py, element_type pz){
		InitUnit();
		Pos().X() = px;	Pos().Y() = py;	Pos().Z() = pz;
	}
	///	x軸, y軸, 位置 を指定
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
	///	x/y/z軸, y/z/x軸, 位置 を指定
	template <class ITPX, class BUFX, class ITPY, class BUFY, class ITPZ, class BUFZ>
	void InitDirect(const PTM::TVectorBaseOp<DIMENC(3),ITPX, BUFX>& a,
					const PTM::TVectorBaseOp<DIMENC(3),ITPY, BUFY>& b,
					char axis,
					const PTM::TVectorBaseOp<DIMENC(3),ITPZ, BUFZ>& posi){
		PTM::init_direct(Rot(), a, b, axis);
		Pos() = posi;
		ExW() = 0; EyW() = 0; EzW() = 0; PosW() = 1;
	}
	///	x/y/z軸まわり回転
	template <class ITPP, class BUFP>
	void InitRot(element_type th, char axis, const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		PTM::init_rot(Rot(), th, axis);
		Pos() = posi;
		ExW() = 0; EyW() = 0; EzW() = 0; PosW() = 1;
	}
	/**	任意軸まわり回転
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
	///	回転ベクトルと位置で初期化
	template <class ITPA, class BUFA, class ITPP, class BUFP>
	void InitRot(	const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& rvec,
					const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		element_type size=rvec.Size();
		InitRot(size, size > 0 ? rvec/size : TVec3<typename ITPA::element_type>(1,0,0), posi);
	}
	///	単位行列
	TAffine(){InitUnit();}
	///	平行移動
	TAffine(element_type px, element_type py, element_type pz){InitTrn(px, py, pz);}
	///	基底ベクトルを直接代入
	template <class ITPX, class BUFX, class ITPY, class BUFY>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPX, BUFX>& exi,
			const PTM::TVectorBaseOp<DIMENC(3),ITPY, BUFY>& eyi){
		InitDirect(exi, eyi, TVec3<element_type>());
	}
	///	基底ベクトルを直接代入
	template <class ITPX, class BUFX, class ITPY, class BUFY, class ITPP, class BUFP>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPX, BUFX>& exi,
			const PTM::TVectorBaseOp<DIMENC(3),ITPY, BUFY>& eyi,
			const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		InitDirect(exi, eyi, posi);
	}
	/**	基底ベクトルを直接代入.
		@param a	axis軸の基底ベクトル
		@param b	(axis+1)%3 軸の基底ベクトル
		@param axis	aの表す軸. 'x', 'y', 'z' のどれか	*/
	template <class ITPA, class BUFA, class ITPB, class BUFB>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& a,
			const PTM::TVectorBaseOp<DIMENC(3),ITPB, BUFB>& b,
			char axis){ InitDirect(a, b, axis, TVec3<element_type>()); }
	/**	基底ベクトルを直接代入.
		@param a	axis軸の基底ベクトル
		@param b	(axis+1)%3 軸の基底ベクトル
		@param axis	aの表す軸. 'x', 'y', 'z' のどれか
		@param posi	平行移動の量	*/
	template <class ITPA, class BUFA, class ITPB, class BUFB, class ITPP, class BUFP>
	TAffine(	const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& a,
			const PTM::TVectorBaseOp<DIMENC(3),ITPB, BUFB>& b,
			char axis,
			const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){ InitDirect(a, b, axis, posi); }
	/**	軸周り回転.
		@param th	回転角(ラジアン)
		@param axis	回転軸. 'x', 'y', 'z' のどれか	*/
	TAffine(element_type th, char axis) {InitRot(th, axis, TVec3<element_type>());}
	/**	軸周り回転.
		@param th	回転角(ラジアン)
		@param axis	回転軸. 'x', 'y', 'z' のどれか
		@param posi	平行移動の量	*/
	template <class ITP, class BUF>
	TAffine(element_type th, char axis, const PTM::TVectorBaseOp<DIMENC(3),ITP, BUF>& posi) {InitRot(th, axis , posi);}
	/**	軸周り回転.
		@param th	回転角(ラジアン)
		@param axis	回転軸.	*/
	template <class ITPA, class BUFA>
	TAffine(element_type th, const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& axis){
		InitRot(th, axis.unit(), Vec3f());
	}
	/**	軸周り回転.
		@param th	回転角(ラジアン)
		@param axis	回転軸.	*/
	template <class ITPA, class BUFA, class ITPP, class BUFP>
	TAffine(element_type th, const PTM::TVectorBaseOp<DIMENC(3),ITPA, BUFA>& axis , const PTM::TVectorBaseOp<DIMENC(3),ITPP, BUFP>& posi){
		InitRot(th, axis.unit(), posi);
	}
	/**	OpenGLの射影行列として初期化
		@param screen	カメラから見たビューポートの中心の位置
		@param size		ビューポートのサイズ
		@param front	手前のクリッピング平面とカメラの距離
		@param back		奥のクリッピング平面とカメラの距離	*/
	template <class ITPS, class BUFS, class ITPZ, class BUFZ>
	void InitProjectionGL(
		const PTM::TVectorBaseOp<DIMENC(3),ITPS, BUFS>& screen,
		const PTM::TVectorBaseOp<DIMENC(2),ITPZ, BUFZ>& size,
		element_type front=1.0f, element_type back=10000.0f){
		PTM::init_projection_gl(*this, screen, size, front, back);
	}
	/**	Direct3Dの射影行列として初期化
		@param screen	カメラから見たビューポートの中心の位置
		@param size		ビューポートのサイズ
		@param front	手前のクリッピング平面とカメラの距離
		@param back		奥のクリッピング平面とカメラの距離	*/
	template <class ITPS, class BUFS, class ITPZ, class BUFZ>
	void InitProjectionD3D(const PTM::TVectorBaseOp<DIMENC(3),ITPS, BUFS>& screen,
	const PTM::TVectorBaseOp<DIMENC(2),ITPZ, BUFZ>& size,
		element_type front=1.0f, element_type back=10000.0f){
		PTM::init_projection_d3d(*this, screen, size, front, back);
	}
	/**	射影行列として初期化
		@param screen	カメラから見たビューポートの中心の位置
		@param size		ビューポートのサイズ
		@param front	手前のクリッピング平面とカメラの距離
		@param back		奥のクリッピング平面とカメラの距離	*/
	template <class ITPS, class BUFS, class ITPZ, class BUFZ>
	TAffine(const PTM::TVectorBaseOp<DIMENC(3),ITPS, BUFS>& screen, const PTM::TVectorBaseOp<DIMENC(2),ITPZ, BUFZ>& size, element_type front=1.0f, element_type back=10000.0f){
		InitProjectionGL(screen, size, front, back);
	}
	///	3x3行列で初期化
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
	///	回転拡大変換部への参照を返す.
	PTM::TSubMatrix<base_type, 3,3>::type& Rot(){ return sub_matrix(PTM::TMatDim<3,3>()); }
	///	回転拡大変換部への参照を返す (const版).
	const PTM::TSubMatrix<base_type, 3,3>::type& Rot() const { return sub_matrix(PTM::TMatDim<3,3>()); }
	
	///	posiにEz()が向いたTAffine行列
	template <class ITP, class BUF>
	void LookAt(const PTM::TVectorBaseOp<DIMENC(3), ITP, BUF>& posi){
		PTM::init_look_at(*this, posi);
	}
	///	poszにEz(), posy に Ey() が向いたTAffine行列
	template <class ITPZ, class BUFZ, class ITPY, class BUFY>
	void LookAt(const PTM::TVectorBaseOp<DIMENC(3), ITPZ, BUFZ>& posz, const PTM::TVectorBaseOp<DIMENC(3), ITPY, BUFY>& posy){
		PTM::init_look_at(*this, posz, posy);
	}
	///	posiに-Ez()が向いたTAffine行列
	template <class ITP, class BUF>
	void LookAtGL(const PTM::TVectorBaseOp<DIMENC(3), ITP, BUF>& posi){
		PTM::init_look_at_gl(*this, posi);
	}
	///	poszに-Ez(), posy に Ey() が向いたTAffine行列
	template <class ITPZ, class BUFZ, class ITPY, class BUFY>
	void LookAtGL(const PTM::TVectorBaseOp<DIMENC(3), ITPZ, BUFZ>& posz, const PTM::TVectorBaseOp<DIMENC(3), ITPY, BUFY>& posy){
		PTM::init_look_at_gl(*this, posz, posy);
	}
};

///	TAffineとベクトルの掛け算
template <class ITP_ROW, class ITP_COL, class BUF, class ITP_V, class BUFV>
PTM::TVector<3, typename ITP_COL::element_type> operator * (
	const PTM::TMatrixBaseOp<DIMENC(4), DIMENC(4), ITP_ROW, ITP_COL, BUF>& a,
	const PTM::TVectorBaseOp<DIMENC(3), ITP_V, BUFV>& b){
	PTM::TVector<3, TYPENAME ITP_COL::element_type> r;
	r = a.sub_matrix(PTM::TMatDim<3,3>()) * b;
	r += a.col(3).sub_vector(PTM::TVecDim<3>());
	return r;
}

///	float版2次元ベクトル
typedef TVec2<float> Vec2f;
///	double版2次元ベクトル
typedef TVec2<double> Vec2d;
///	float版3次元ベクトル
typedef TVec3<float> Vec3f;
///	double版3次元ベクトル
typedef TVec3<double> Vec3d;
///	float版4次元ベクトル
typedef TVec4<float> Vec4f;
///	double版4次元ベクトル
typedef TVec4<double> Vec4d;

///	float版2×2行列．
typedef TMatrix2<float> Matrix2f;
///	double版2×2行列．
typedef TMatrix2<double> Matrix2d;
///	float版3×3行列．
typedef TMatrix3<float> Matrix3f;
///	double版3×3行列．
typedef TMatrix3<double> Matrix3d;
///	float版2次元あふぃん行列.
typedef TAffine2<float> Affine2f;
///	double版2次元あふぃん行列.
typedef TAffine2<double> Affine2d;
///	float版3次元あふぃん行列.
typedef TAffine<float> Affinef;
///	double版3次元あふぃん行列.
typedef TAffine<double> Affined;
///	float版TQuaternion.
typedef TQuaternion<float> Quaternionf;
///	double版TQuaternion.
typedef TQuaternion<double> Quaterniond;
//@}

#ifdef _DEBUG
 #pragma optimize ("", on)
 #pragma auto_inline(off)
 #pragma inline_recursion(off)
#endif

}	//	namespace LVRM

#endif
