#ifndef PTMATRIX_TMATRIX_H
#define PTMATRIX_TMATRIX_H
/** 
	@page PTM ポータブル テンプレート 行列クラスライブラリ
	
	@author 長谷川 晶一

	@date 2001年6月10日,10月6日更新

	@section introM はじめに
		このドキュメントはポータブル テンプレート 行列クラスライブラリ
		のドキュメントです．
		ポータブル テンプレート 行列クラスライブラリは，
		テンプレートによるM×N行列のクラスライブラリです．
		
	@section specM このライブラリの特徴
	@subsection tmpM テンプレートで解決
		全てをテンプレートで静的に解決しています．
		行列のサイズやポインタなどを保持するための管理領域を持ちません．
		そのため，
		<ul>
			<li> 行列の一部(部分行列，ベクトル，行ベクトル，列ベクトル)などを
				直接参照することができる(代入も可能)．
			<li> 配列をキャストして行列として使用することも可能．
			<li> 行列の掛け算などで，行列のサイズが合わない場合，コンパイル時に
				コンパイラがエラーを出力する．
			<li> 行列のサイズを動的に変更することができない．
		</ul>
		といった特徴をもちます．
	@subsection portM 移植性
		処理系依存部分の多いテンプレート機能を活用していながら，全機能が，
		3つのコンパイラで使用できます．サポートしているコンパイラは，
		<ul>
			<li> CL (MS Visual C++ 6.0)
			<li> bcc32(Borland C++ 5.5.1)
			<li> gcc(GNU c compiler 2.95.3-5)
		</ul>
		です．
	@subsection why 新たにクラスライブラリを作成した理由
		すでに多くの行列ライブラリがありながら，新たに作成した理由は，
		- TNT，MTL の行列は管理領域をメモリに持つため，配列をキャストして
			行列として使用することができない．
		- Blitz は TinyMatrix, TinyVector を持つが，Visual C++ で
			使用できない．
		- 私の知る限り，部分行列，部分ベクトルへの参照を返す行列ライブラリ
			は存在しない．

	からです．
	
	@section usage< 使い方
	ポータブル テンプレート 行列クラスライブラリは，ヘッダファイルだけからなる
	クラスライブラリなので, TMatrix.h, TMatrixUtility.h, TVector.h
	を同じフォルダに入れておき，.cppファイルからヘッダをインクルードするだけで
	使用できます．
	@subsection sampleM サンプル
		簡単なサンプルです．適当なファイル名(たとえば sample.cpp) で保存して
		コンパイルしてください．コンパイルするためには，
		<DL>
		<DT> visual C++ の場合
		<DD> cl -GX sample.cpp
		<DT> gccの場合
		<DD> gcc sample.cpp -lstdc++
		<DT> bcc の場合
		<DD> bcc32 sample.cpp
		</DL>
	としてください．
	@verbatim
#include "TMatrix.h"    //  行列ライブラリのインクルードする．
#include <iostream>
using namespace PTM;    //  行列クラスはPTM名前空間の中で宣言されている．
void main(){
    TMatrix<2,2,float> mat;     //  2行2列の行列を宣言
    mat[0][0] = 1;  mat[0][1] = 2;
    mat[1][0] = 3;  mat[1][1] = 4;
    TVector<2,float> vec;       //  2次元のベクトルを宣言
    vec[0] = 1; vec[1] = 0;
    std::cout << mat;
    std::cout << vec << std::endl;
    std::cout << mat * vec << std::endl;	//	掛け算
    std::cout << mat + mat << std::endl;	//	足し算
    std::cout << mat - mat << std::endl;	//	引き算
    std::cout << mat.trans() << std::endl;	//	転置
    std::cout << mat.inv() << std::endl;	//	逆行列
}
@endverbatim
	@subsection vecfunc ベクトルの機能
	次の演算ができます．
	<ul>
	<li> +:和, -:差, *:内積/定数倍, /:定数分の1
	<li> ==:比較, =:代入
	<li> <<:出力, >>:入力
	<li> %:外積(2・3次元のみ)
	</ul>
	次のメンバ関数を持ちます．
	<ul>
	<li> unit(): 向きが等しい単位ベクトルを返す．
	<li> norm(): ベクトルの大きさ(ノルム)を返す．
	<li> sub_vector(): 部分ベクトルへの参照を返す．
	</ul>
	
	@subsection matfunc 行列の機能
	次の演算ができます．
	<ul>
		<li> +:和, -:差, *:積/定数倍, /:定数分の1
		<li> ==:比較, =:代入
		<li> <<:出力, >>:入力
	</ul>
	次のメンバ関数を持ちます．
	<ul>
		<li> det(): 行列式を返す．
		<li> inv(): 逆行列を返す．
		<li> gauss(): ガウスの消去法で方程式を解く．
		<li> sub_matrix(): 部分行列への参照を返す．
		<li> row(): 行ベクトルへの参照を返す．
		<li> col(): 列ベクトルへの参照を返す．
	</ul>
		sub_matrix()やrow()には値を代入することもできます．
		@verbatim
	TMatrix<3,3,float> mat; TVector<3, float> vec;
	mat.row() = vec;@endverbatim
	@section pub 再配布の条件
	再配布する場合は，原著作者の署名・連絡先を改変・削除しないでください．
	閉鎖的な会社でソフトウエアを書く人にも自由に使用できるようにしたい
	ので，GPL・LGPLにしませんでした．
	もちろんGPL・LGPLに改変して再配布してくださっても結構です．
	@section support サポート
	バグ，不具合，意味不明なコンパイルエラーなどを見つけた場合は，
	長谷川 晶一 (hase@hi.pi.titech.ac.jp) までご連絡ください．
	できる限りサポートし，より良いライブラリにしていくつもりです．<br>
	特にこのライブラリはテンプレートクラスライブラリなので，使用時に，
	内部の動作が分らないと意味のわからないコンパイルエラーに出会うことも
	あると思います．そのような問題にはFAQを作って対応するつもりですので，
	まずはご連絡ください．
	@section thanksM 謝辞
	LU分解，逆行列，ガウス消去法などの行列計算アルゴリズムは，<br>
    「『Ｃ言語による最新アルゴリズム事典』全ソースコード」<br>
    ftp://ftp.matsusaka-u.ac.jp/pub/algorithms<br>
	奥村 晴彦 Haruhiko Okumura<br>
	を改変して流用させていただきました．
	自由にコードを使わせてくださって，ありがとうございます．	*/
//-----------------------------------------------------------------------------
/**	@file TMatrix.h
	テンプレートによるN×M行列型の定義.
	要素の型とサイズをテンプレートの引数にすることで，
	管理情報をメモリに持たずに，一般の行列を扱う．
	配列をキャストして行列にすることもできる．
	部分行列や行や列を行列やベクトルとして取り出すこともできる．
	sparse matrix には対応していない．										*/
//------------------------------------------------------------------------------

#include "TVector.h"

///	PTMatrixライブラリの名前空間
namespace PTM{

#ifdef _DEBUG
 #pragma optimize ("awgity", on)
 #pragma auto_inline(on)
 #pragma inline_recursion(on)
#endif
//----------------------------------------------------------------------------
///	行列型の基本クラス。普通は派生クラスの TMatrix を使う。
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
class TMatrixBase:public BUF{
public:
	///@name 行列のサイズ
	//@{
	///	行列の行数
	int nrow() const { return DIMDEC(dim_row); }
	///	行列の列数
	int ncol() const { return DIMDEC(dim_col); }
	///	行列の行数. DIMTYPE dim_row = NROW;
	DIMDEF(NROW, dim_row);
	///	行列の列数. DIMTYPE dim_col = NCOL;
	DIMDEF(NCOL, dim_col);
#if !defined __BORLANDC__ && !defined _MSC_VER
	static const int dim_row_col = DIMDEC(dim_row)*DIMDEC(dim_col);
#endif
	//@}

	///@name 基本クラス、要素などの型.
	//@{
	///	メモリを持つ型
	typedef BUF									buffer_type;
	///	基本クラスの型
	typedef BUF									base_type;
	///	このクラスの型
	typedef TMatrixBase							this_type;
	///	要素の型
	typedef typename ITP_COL::element_type		element_type;
	///	要素型の0を返す関数オブジェクトの型
	typedef typename ITP_COL::zero				zero;
	//@}
	
	///@name イタレータ、行・列ベクトルなどアクセスのための型.
	//@{
	///	行のイタレータのペア型
	typedef ITP_ROW								row_iterator_pair;
	///	行のイタレータ
	typedef typename ITP_ROW::iterator			row_iterator;
	///	行のイタレータ(const 版)
	typedef typename ITP_ROW::const_iterator	const_row_iterator;
	
	///	列のイタレータのペア型
	typedef ITP_COL								col_iterator_pair;
	///	列のイタレータ
	typedef typename ITP_COL::iterator			col_iterator;
	///	列のイタレータ(const 版)
	typedef typename ITP_COL::const_iterator	const_col_iterator;

	///	行ベクトルの型
	typedef TVectorBaseOp<dim_col, ITP_COL, BUF>	row_vector;
	///	列ベクトルの型
	typedef TVectorBaseOp<dim_row, ITP_ROW, BUF>	col_vector;

#if !defined __BORLANDC__ && !defined _MSC_VER
	typedef element_type array_type [dim_row_col];
#else
	///	行列を1次元配列で表したときの型
	typedef element_type array_type [DIMDEC(dim_row)*DIMDEC(dim_col)];
#endif
	///	行列を2次元配列で表したときの型
	typedef element_type array_2D_type[DIMDEC(dim_row)][DIMDEC(dim_col)];
	//@}

private:
	///	絶対値を返す関数.
	template <class element_type> element_type abs(const element_type& t){
		return t < zero() ? -t : t;
	}

public:

	///@name 要素のアクセス
	//@{
	///	行列の[0][0]要素へのポインタ
	element_type*		begin()					{ return base_type::begin(); }
	///	行列の[0][0]要素へのポインタ(const版)
	const element_type*	begin() const			{ return base_type::begin(); }
	///	行方向へ進む、[0][0]要素を指す、イタレータを返す。
	row_iterator		row_begin()				{ return row_iterator(begin()); }
	///	行方向へ進む、[0][0]要素を指す、イタレータを返す。(const版)
	const_row_iterator	row_begin() const		{ return const_row_iterator(begin()); }
	///	行方向へ進む、[0][c]要素を指す、イタレータを返す。
	row_iterator		row_begin(int c)		{ return row_iterator(col_begin()+c); }
	///	行方向へ進む、[0][c]要素を指す、イタレータを返す。(const版)
	const_row_iterator	row_begin(int c) const	{ return const_row_iterator(col_begin()+c); }

	///	列方向へ進む、[0][0]要素を指す、イタレータを返す。
	col_iterator		col_begin()				{ return col_iterator(begin()); }
	///	列方向へ進む、[0][0]要素を指す、イタレータを返す。(const版)
	const_col_iterator	col_begin() const		{ return const_col_iterator(begin()); }
	///	列方向へ進む、[r][0]要素を指す、イタレータを返す。
	col_iterator		col_begin(int r)		{ return col_iterator(row_begin()+r); }
	///	列方向へ進む、[r][0]要素を指す、イタレータを返す。(const版)
	const_col_iterator	col_begin(int r) const	{ return const_col_iterator(row_begin()+r); }

	///	行方向へ進む、[行数][c]要素を指す、イタレータを返す。
	row_iterator		row_end(int c=0)		{ return row_begin(c)+nrow();}
	///	行方向へ進む、[行数][c]要素を指す、イタレータを返す。(const版)
	const_row_iterator	row_end(int c=0) const	{ return row_begin(c)+nrow(); }

	///	列方向へ進む、[r][列数]要素を指す、イタレータを返す。
	col_iterator		col_end(int n=0)		{ return col_begin(n)+ncol();}
	///	列方向へ進む、[r][列数]要素を指す、イタレータを返す。(const版)
	const_col_iterator	col_end(int n=0) const	{ return col_begin(n)+ncol(); }

	///	n行m列要素([n][m]要素)への参照を返す。
	element_type& item(int n, int m)			{ return col_begin(n)[m]; }
	///	n行m列要素([n][m]要素)への参照を返す。(const 版)
	const element_type& item(int n, int m) const{ return col_begin(n)[m]; }
	
	///	r行ベクトル([r])への参照を返す。
	row_vector&			row(int r)				{ return *(row_vector*) (element_type*) col_begin(r); }
	///	r行ベクトル([r])への参照を返す。(const版)
	const row_vector&	row(int r) const		{ return *(const row_vector*) (const element_type*) col_begin(r); }
	///	r行ベクトル([r])への参照を返す。
	row_vector&			operator [](int r)		{ return row(r); }
	///	r行ベクトル([r])への参照を返す。(const版)
	const row_vector&	operator [](int r) const{ return row(r); }

	///	c列ベクトル([][c])への参照を返す。
	col_vector&			col(int c)				{ return *(col_vector*) (element_type*) row_begin(c); }
	///	c列ベクトル([][c])への参照を返す。(const版)
	const col_vector&	col(int c) const		{ return *(const col_vector*) (const element_type*) row_begin(c); }

	///	行列を1次元配列で返す
	operator array_type&() {return *(array_type*)begin();}
	///	行列を1次元配列で返す
	operator const array_type&() const {return *(array_type*)begin();}

	///	行列を2次元配列で返す
	operator const array_2D_type&() const {return *(array_2D_type*)begin();}
	///	行列を2次元配列で返す
	operator array_2D_type&() {return *(array_2D_type*)begin();}
	//@}

	///	@name 基本操作
	//@{
	///	ゼロクリア
	void clear(){
		for(int r = 0; r < nrow(); ++r) row(r).clear(zero());
	}
	///	ある値でクリア
	template <class M_ITP_ROW>
	void clear(typename M_ITP_ROW::elemet_type t){
		for(int r = 0; r < nrow(); ++r) row(r).clear(t);
	}
	///	代入
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	void assign(const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		for(int r = 0; r < nrow(); ++r) row(r).assign(b.row(r));
	}
	//@}
	///	@name 計算
	//@{
	///	加算
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	void add(const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		for(int r = 0; r < nrow(); ++r) row(r).add(b.row(r));
	}
	///	減算
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	void sub(const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		for(int r = 0; r < nrow(); ++r) row(r).sub(b.row(r));
	}
	///	LU分解を行う。thisを書き換える。行列式を返す。
	element_type lu(int (&ip)[DIMDEC(dim_row)]){
		assert(nrow() == ncol());
		this_type& a = *this;
		int i, j, k, ii, ik;
		int n = nrow();
		element_type t, u, det;
		TVector<DIMDEC(dim_row), element_type> weight;
		
		det = 0;                   // 行列式
		for (k = 0; k < n; k++) {  // 各行について
			ip[k] = k;             // 行交換情報の初期値
			u = 0;                 // その行の絶対値最大の要素を求める
			for (j = 0; j < n; j++) {
				t = abs(a[k][j]);  if (t > u) u = t;
			}
			if (u == 0) goto EXIT; // 0 なら行列はLU分解できない
			weight[k] = 1 / u;     // 最大絶対値の逆数
		}
		det = 1;                   // 行列式の初期値
		for (k = 0; k < n; k++) {  // 各行について
			u = -1;
			for (i = k; i < n; i++) {  // より下の各行について
				ii = ip[i];            // 重み×絶対値 が最大の行を見つける
				t = abs(a[ii][k]) * weight[ii];
				if (t > u) {  u = t;  j = i;  }
			}
			ik = ip[j];
			if (j != k) {
				ip[j] = ip[k];  ip[k] = ik;  // 行番号を交換
				det = -det;  // 行を交換すれば行列式の符号が変わる
			}
			u = a[ik][k];  det *= u;  // 対角成分
			if (u == 0) goto EXIT;    // 0 なら行列はLU分解できない
			for (i = k + 1; i < n; i++) {  // Gauss消去法
				ii = ip[i];
				t = (a[ii][k] /= u);
				for (j = k + 1; j < n; j++)
					a[ii][j] -= t * a[ik][j];
			}
		}
EXIT:
		return det;           // 戻り値は行列式
	}
	///	行列式を返す．
	element_type det(){
		return calc_matrix_det(*this);
	}
	///	(*this) x + b = 0 の1次方程式を解く
	template <class ITPX, class BUFX, class ITPB, class BUFB>
	void solve(TVectorBase<dim_row, ITPX, BUFX>& x, const TVectorBase<dim_row, ITPB, BUFB>& b, int (&ip)[DIMDEC(dim_row)]){
		this_type& a = *this;
		int i, j, ii;
		element_type t;
		const int n = nrow();
		for (i = 0; i < n; i++) {       // Gauss消去法の残り
			ii = ip[i];  t = b[ii];
			for (j = 0; j < i; j++) t -= a[ii][j] * x[j];
			x[i] = t;
		}
		for (i = n - 1; i >= 0; i--) {  // 後退代入
			t = x[i];  ii = ip[i];
			for (j = i + 1; j < n; j++) t -= a[ii][j] * x[j];
			x[i] = t / a[ii][i];
		}
	}
	///	ガウスの消去法
	template <class ITPX, class BUFX, class ITPB, class BUFB>
	element_type gauss(TVectorBase<dim_row, ITPX, BUFX>& x, const TVectorBase<dim_row, ITPB, BUFB>& b){
		this_type& a = *this;
		element_type det;				// 行列式
		int ip[DIMDEC(dim_row)];		// 行交換の情報

		det = lu(ip);					// LU分解
		if (det != 0) solve(x, b, ip);	// LU分解の結果を使って連立方程式を解く
		return det;						// 戻り値は行列式
	}
	///	逆行列を求める。
	template <class ITP_ROWI, class ITP_COLI, class BUFI>
	element_type inv(TMatrixBase<dim_row, dim_col, ITP_ROWI, ITP_COLI, BUFI>& a_inv){
		int i, j, k, ii;
		int n = nrow();
		element_type t, det;
		int ip[DIMDEC(dim_row)];   // 行交換の情報
		this_type& a = *this;
		
		det = a.lu(ip);
		if (det != 0){
			for (k = 0; k < n; k++) {
				for (i = 0; i < n; i++) {
					ii = ip[i];  t = (ii == k);
					for (j = 0; j < i; j++)
						t -= a[ii][j] * a_inv[j][k];
					a_inv[i][k] = t;
				}
				for (i = n - 1; i >= 0; i--) {
					t = a_inv[i][k];  ii = ip[i];
					for (j = i + 1; j < n; j++)
						t -= a[ii][j] * a_inv[j][k];
					a_inv[i][k] = t / a[ii][i];
				}
			}
		}
		return det;
	}
	//@}
	///	表示
	void print(std::ostream& os) const {
		int w = os.width();
		os.width(0);
		for(int i=0; i<nrow(); ++i){
			for(int j=0; j<w; ++j) os << " ";
			if (i == 0) os << "(";
			else os << " ";
			row(i).print(os);
			if (i==nrow()-1) os << ")";
			os << std::endl;
		}
		os.width(w);
	}
	///	入力
	void input(std::istream& is){
		char ch;
		is >> ch;
		for(int i=0; i<nrow(); ++i) is >> row(i);
		is >> ch;
	}
};
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
TYPENAME ITP_COL::element_type calc_matrix_det(const TMatrixBase<NROW,NCOL,ITP_ROW,ITP_COL,BUF>& mat){
	int ip[DIMDEC(NROW)];
	TMatrixBase<NROW,NCOL,ITP_ROW,ITP_COL,BUF> tmp(mat);
	return tmp.lu(ip);
}
template <class ITP_ROW, class ITP_COL, class BUF>
TYPENAME ITP_COL::element_type calc_matrix_det(const TMatrixBase<DIMENC(2), DIMENC(2),ITP_ROW,ITP_COL,BUF>& mat){
	return mat.item(0,0)*mat.item(1,1) - mat.item(1,0)*mat.item(0,1);
}
	
/**	TMatrix型派生クラスに必要なメンバの定義.
	派生クラスを作るたびに，このマクロを使ってメンバを作る.
	@param	THIS	新たに宣言する派生クラスの型名.
	@see	TMatrix	*/
#define DEF_TMATRIX_BASIC_MEMBER(THIS)										\
    typedef THIS this_type;             /*  このクラスの型を定義しておく.*/	\
	typedef base_type::buffer_type buffer_type;								\
										/*	行列のメモリ型.	*/				\
    DIMDEF(base_type::dim_row, dim_row);/*  行列の行数.	*/					\
    DIMDEF(base_type::dim_col, dim_col);/*  行列の列数.	*/					\
	typedef base_type::row_iterator_pair row_iterator_pair;					\
                                        /*  行イタレータのペア	*/			\
    typedef base_type::row_iterator row_iterator;							\
                                        /*  行イタレータ	*/				\
    typedef base_type::const_row_iterator const_row_iterator;				\
                                        /*  行イタレータ(const版)	*/		\
	typedef base_type::col_iterator_pair col_iterator_pair;					\
                                        /*  列イタレータのペア		*/		\
    typedef base_type::col_iterator col_iterator;							\
                                        /*  列イタレータ	*/				\
    typedef base_type::const_col_iterator const_col_iterator;				\
                                        /*  列イタレータ(const版)	*/		\
    typedef base_type::element_type element_type;							\
                                        /*  要素型.	*/						\
    typedef base_type::element_type const_element_type;						\
                                        /*  要素型(const版).	*/			\
    typedef base_type::zero zero;											\
                                        /*  ゼロを返す関数オブジェクト.	*/	\
	/*	TMatrixBaseオブジェクトによる初期化	*/								\
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>				\
	THIS(const PTM::TMatrixBaseOp<dim_row, dim_col,							\
		M_ITP_ROW, M_ITP_COL, M_BUF>& b){ assign(b); }						\
	/*	代入演算子. サイズが等しい行列 b を代入する.	*/					\
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>				\
	this_type& operator =(const PTM::TMatrixBase<dim_row, dim_col,			\
		M_ITP_ROW, M_ITP_COL, M_BUF>& b){ assign(b); return *this; }		\
	/*	<< 演算子による初期化	*/											\
	template <class ROWINIT, int M_R>										\
	THIS(const PTM::TVectorInitializer<ROWINIT, M_R>& v){					\
		for(int r=0; r<nrow(); ++r){										\
			row(r) = *(PTM::TVector<DIMDEC(ROWINIT::dim),					\
				TYPENAME ROWINIT::element_type> *)v.value[r].value;			\
		}																	\
	}																		\
	/*	<< 演算子による初期化のための = 演算子.	*/							\
	template <class ROWINIT, int M_R>										\
	this_type& operator =(const PTM::TVectorInitializer<ROWINIT, M_R>& v){	\
		for(int r=0; r<nrow(); ++r){										\
			row(r) = *(PTM::TVector<DIMDEC(ROWINIT::dim),					\
				TYPENAME ROWINIT::element_type> *)v.value[r].value;			\
		}																	\
		return *this;														\
	}																		\

//----------------------------------------------------------------------------
///	演算子つきの行列
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
class TMatrixBaseOp: public TMatrixBase <NROW,NCOL,ITP_ROW,ITP_COL,BUF>{
public:
	///	基本クラス型の定義.
	typedef TMatrixBase<NROW,NCOL,ITP_ROW,ITP_COL,BUF>	base_type;
	/**	継承されない基本的なメンバの定義.
		@see	::DEF_TMATRIX_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrixBaseOp);
	///	デフォルトコンストラクタ.
	TMatrixBaseOp(){}
	///	転置行列.
	TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>& trans(){
		return *(TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>*)this;
	}
	///	転置行列(const 版).
	const TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>& trans() const {
		return *(TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>*)this;
	}
	///	逆行列の計算.
	template <class ITP_ROWI, class ITP_COLI, class BUFI>
	element_type inv(TMatrixBase<dim_row, dim_col, ITP_ROWI, ITP_COLI, BUFI>& a_inv){ return base_type::inv(a_inv); }
	///	逆行列を返す.
	this_type inv() const {
		this_type a(*this), a_inv;
		a.inv(a_inv);
		return a_inv;
	}
	/**	部分行列を返す.
		この行列の(0,0)要素から (S::nrow()-1, S::ncol()-1)要素までの
		部分行列への参照を返す．
		@param	S	部分行列のサイズを指定するためのオブジェクト.
					S::dim_row が行数. S::dim_colが列数を表す.
		@return	Sで指定されたサイズの行列への参照を返す.	*/
	template <class S>
	TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(S){
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (element_type*)(col_begin());
	}
	/**	部分行列を返す.
		この行列の(r,c)要素から (r+S::nrow()-1, c+S::ncol()-1)要素までの
		部分行列への参照を返す．
		@param	S	部分行列のサイズを指定するためのオブジェクト.
					S::dim_row が行数. S::dim_colが列数を表す.
		@param	r	部分行列の0行目が，この行列の何行目に当たるのかを指定.
		@param	c	部分行列の0列目が，この行列の何列目に当たるのかを指定.
		@return	Sで指定されたサイズの行列への参照を返す.	*/
	template <class S>
	TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(int r, int c, S){
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (element_type*)(col_begin(r)+c);
	}
	/**	部分行列を返す(const 版).
		この行列の(0,0)要素から (S::nrow()-1, S::ncol()-1)要素までの
		部分行列への参照を返す．
		@param	S	部分行列のサイズを指定するためのオブジェクト.
					S::dim_row が行数. S::dim_colが列数を表す.
		@return	Sで指定されたサイズの行列への参照を返す.	*/
	template <class S>
	const TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(S) const {
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (const element_type*)(col_begin());
	}
	/**	部分行列を返す.(const 版)
		この行列の(r,c)要素から (r+S::nrow()-1, c+S::ncol()-1)要素までの
		部分行列への参照を返す．
		@param	S	部分行列のサイズを指定するためのオブジェクト.
					S::dim_row が行数. S::dim_colが列数を表す.
		@param	r	部分行列の0行目が，この行列の何行目に当たるのかを指定.
		@param	c	部分行列の0列目が，この行列の何列目に当たるのかを指定.
		@return	Sで指定されたサイズの行列への参照を返す.	*/
	template <class S>
	const TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(int r, int c, S) const {
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (element_type*)(col_begin(r)+c);
	}
	/**	+=演算子(*this = *this + b).
		@param	b	次元が等しい行列	*/
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	this_type operator+= (const TMatrixBaseOp<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		add(b); return *this; 
	}
	/**	-=演算子(*this = *this - b).
		@param	b	次元が等しい行列	*/
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	this_type operator-= (const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		sub(b); return *this;
	}
	/**	- 演算子 (return -*this).	*/
	this_type operator- () { this_type r(*this); r*=-1; return r; }
	/**	*=演算子(*this = b * *this).
		@param	b	要素型	*/
	this_type operator*= (element_type b){
		for(int i=0; i<nrow(); ++i) row(i) *= b; return *this;
	}
	/**	/=演算子(*this = *this / b).
		@param	b	要素型	*/
	this_type operator/= (element_type b){
		for(int i=0; i<nrow(); ++i) row(i) /= b; return *this;
	}
};
//----------------------------------------------------------------------------
/**	行列型. 列が隣合う，行ベクトルが密なベクトルとしてに取り出せる行列.
	TMatrix<行数, 列数, 要素の型> が行列型になる。	*/
template <int R, int C, class T, class ZERO=T>
class TMatrix:public TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(C),T,ZERO>,
	TVectorIteratorPair<DIMENC(1),T,ZERO>, TVectorBuffer<R*C,T> >{
public:
	/**	基本クラス型の定義	*/
	typedef TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(C),T,ZERO>, 
		TVectorIteratorPair<DIMENC(1),T,ZERO>, TVectorBuffer<R*C,T> > base_type;
	/**	継承されない基本的なメンバの定義.
		@see	::DEF_TMATRIX_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrix);
	///	デフォルトコンストラクタ.
	TMatrix(){}
};
/**	行列型. 行が隣合う，列ベクトルが密なベクトルとしてに取り出せる行列.
	TMatrix<行数, 列数, 要素の型> が行列型になる。	*/
template <int R, int C, class T, class ZERO=T>
class TMatrixCol:public TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(1),T,ZERO>,
	TVectorIteratorPair<DIMENC(R),T,ZERO>, TVectorBuffer<R*C,T> >{
public:
	/**	基本クラス型の定義	*/
	typedef TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(1),T,ZERO>,
		TVectorIteratorPair<DIMENC(R),T,ZERO>, TVectorBuffer<R*C,T> > base_type;
	/**	継承されない基本的なメンバの定義.
		@see	::DEF_TMATRIX_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrixCol);
	///	デフォルトコンストラクタ.
	TMatrixCol(){}
};

//----------------------------------------------------------------------------
/**	部分行列型作成のためのユーティリティークラス.
	TSubMatrix<行列型, 行数, 列数>::type で部分行列型を取得できる。	*/
template <class MAT, int R, int C>
class TSubMatrix:public TMatrixBaseOp<DIMENC(R), DIMENC(C), typename MAT::row_iterator_pair, typename MAT::col_iterator_pair, typename MAT::buffer_type>{
public:
	typedef TMatrixBaseOp<DIMENC(R), DIMENC(C), typename MAT::row_iterator_pair, typename MAT::col_iterator_pair, typename MAT::buffer_type> type;
};
/**	部分行列型作成のためのユーティリティークラス.
	TMatDim<行数, 列数> で部分行列型を取得できる。	*/
template <int R, int C>
class TMatDim{
public:
	DIMDEF(DIMENC(R), dim_row);
	DIMDEF(DIMENC(C), dim_col);
};


//----------------------------------------------------------------------------
//	外部関数の定義

/**	比較(return a == b).
	@param	a	行列型
	@param	b	要素型aと同じ次元数のベクトル型
	@return		bool	*/
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
bool operator == (
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b) {
	return a.equal(b);
}
/**	比較(return a != b).
	@param	a	行列型
	@param	b	要素型aと同じ次元数のベクトル型
	@return		bool	*/
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
bool operator != (
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b) {
	return !a.equal(b);
}

///	行列の足し算
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator + (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	r += b;
	return r;
}
///	行列の引き算
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator - (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	r -= b;
	return r;
}
///	行列の掛け算
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	DIMTYPE NCOLB, class ITP_ROWB, class ITP_COLB, class BUFB>
	TMatrixBaseOp<NROW, NCOLB, ITP_ROW, ITP_COL, BUF> multi(
		const TMatrixBase<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
		const TMatrixBase<NCOL, NCOLB, ITP_ROWB, ITP_COLB, BUFB>& b){
	TMatrixBaseOp<NROW, NCOLB, ITP_ROW, ITP_COL, BUF> r;
	for(int n=0; n<r.nrow(); ++n){
		for(int m=0; m<r.ncol(); ++m) {
			r.item(n,m) = a.row(n) * b.col(m);
		}
	}
	return r;
}
///	行列とベクトルの掛け算
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF, class ITP_V, class BUFV>
TVector<DIMDEC(NROW), typename ITP_COL::element_type> multi (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TVectorBaseOp<NCOL, ITP_V, BUFV>& b){
	TVector<DIMDEC(NROW), ITP_COL::element_type> r;
	for(int n=0; n<r.size(); ++n){
		r.item(n) = a.row(n) * b;
	}
	return r;
}
///	行列の掛け算
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	DIMTYPE NCOLB, class ITP_ROWB, class ITP_COLB, class BUFB>
	TMatrixBaseOp<NROW, NCOLB, ITP_ROW, ITP_COL, BUF> operator * (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TMatrixBaseOp<NCOL, NCOLB, ITP_ROWB, ITP_COLB, BUFB>& b){
	return multi(a, b);
}
///	行列とベクトルの掛け算
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF, class ITP_V, class BUFV>
TVector<DIMDEC(NROW), typename ITP_COL::element_type> operator * (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TVectorBaseOp<NCOL, ITP_V, BUFV>& b){
	return multi(a, b);
}
///	ベクトルと行列の掛け算
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF, class ITP_V, class BUFV>
TVector<DIMDEC(NCOL), typename ITP_COL::element_type> operator * (
	const TVectorBaseOp<NROW, ITP_V, BUFV>& a,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& b){
	return multi(b.trans(), a);
}
///	行列の定数倍
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator * (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	TYPENAME ITP_COL::element_type b){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	for(int i=0; i<r.nrow(); ++i) r.row(i) *= b;
	return r;
}
///	行列の定数倍
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator * (
	TYPENAME ITP_COL::element_type b,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	for(int i=0; i<r.nrow(); ++i) r.row(i) *= b;
	return r;
}
///	表示
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
std::ostream& operator << (std::ostream& os, const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& m){
	m.print(os);
	return os;
}
///	入力
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
std::istream& operator >> (std::istream& is, TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& m){
	m.input(is);
	return is;
}
#ifdef _DEBUG
 #pragma optimize ("", on)
 #pragma auto_inline(off)
 #pragma inline_recursion(off)
#endif
}	//	namespace PTM
#endif
