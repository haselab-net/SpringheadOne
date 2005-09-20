#ifndef PTMATRIX_TVECTOR_H
#define PTMATRIX_TVECTOR_H
/**	@file TVector.h
	テンプレートによるN次元ベクトルの定義.
	要素の型とサイズをテンプレートの引数にすることで，
	管理情報をメモリに持たずに，一般の行列を扱う．
	配列をキャストしてベクトルにすることもできる．*/
#include <math.h>
#include <assert.h>
#include <iosfwd>

#ifdef _MSC_VER
 #define for if(0); else for
#pragma warning (disable: 4786)
#endif

namespace PTM {
#ifdef _DEBUG
 #pragma optimize ("awgity", on)
 #pragma auto_inline(on)
 #pragma inline_recursion(on)
#endif

#if defined _MSC_VER || defined __BORLANDC__
template <int N> class TVectorDim{ char dim[N]; };
 #define DIMTYPE class
 #define DIMENC(n) PTM::TVectorDim<n>
 #define DIMDEC(T) sizeof(T)
 #define DIMDEF(T, t) typedef T t
 #define TYPENAME
#else
 #define DIMTYPE int
 #define DIMENC(n) n
 #define DIMDEC(n) n
 #define DIMDEF(T,t) static const int t = T;
 #define TYPENAME typename
#endif

//------------------------------------------------------------------
//	ベクトルの初期化のためのユーティリティー
//	<< で結合できる初期化値型.
template <class T, int N>
class TVectorInitializer{
public:
	typedef T element_type;
	DIMDEF(DIMENC(N), dim);
	T value[N];
	TVectorInitializer(TVectorInitializer<T, N-1>& v, const T& t){
		for(int i=0; i<N-1; ++i){ value[i] = v.value[i]; }
		value[N-1] = t;
	}
	TVectorInitializer(const T& t){
		value[0] = t;
	}
	TVectorInitializer(){}
};
#ifndef _MSC_VER
template <class T>
class TVectorInitializer<T,1>{
public:
	typedef T element_type;
	DIMDEF(DIMENC(1), dim);
	T value[1];
	TVectorInitializer(const T& t){
		value[0] = t;
	}
	TVectorInitializer(){}
};
#endif
//	初期化値型の最初の値
template <class T>
class TVectorInitializerHeadWithType{
};
class TVectorInitializerHead{
public:
	template <class	T>
	TVectorInitializerHeadWithType<T> operator()(T t) const {
		return TVectorInitializerHeadWithType<T>();
	}
};
#define VecInit TVectorInitializerHead()
template <class T>
TVectorInitializer<T,1> operator << (TVectorInitializerHead, T t){
	return TVectorInitializer<T,1>(t);
}
template <class T>
TVectorInitializer<T,1> operator << (TVectorInitializerHeadWithType<T>, T t){
	return TVectorInitializer<T,1>(t);
}
template <class T, int N>
TVectorInitializer<T,N+1> operator << (TVectorInitializer<T,N> vi, T t){
	return TVectorInitializer<T,N+1>(vi, t);
}

///	ベクトル、行列用のイタレータ.
template <DIMTYPE STRIDE=DIMENC(1), class T=float, class ZERO=T>
class TVectorIterator{
public:
	typedef T element_type;
	typedef ZERO zero;
	int stride() const { return DIMDEC(STRIDE); }
protected:
	element_type* ptr;
public:
	explicit TVectorIterator(element_type* p){ ptr = p; }
	operator element_type*() const { return ptr; }
	element_type& operator [](int pos) const { return ptr[pos*stride()]; }
	TVectorIterator& operator ++(){ ptr+=stride(); return *this; }
	TVectorIterator operator ++(int) { TVectorIterator rv = *this; ptr+=stride(); return rv; }
	TVectorIterator& operator --(){ ptr-=stride(); return *this; }
	TVectorIterator operator --(int) { TVectorIterator rv = *this; ptr-=stride(); return rv; }
	TVectorIterator& operator +=(int n) { ptr += n*stride(); return *this; }
	TVectorIterator& operator -=(int n) { ptr -= n*stride(); return *this; }
	TVectorIterator operator +(int n) const { TVectorIterator rv(*this); rv+=n; return rv; }
	TVectorIterator operator -(int n) const { TVectorIterator rv(*this); rv-=n; return rv; }
};

///	普通のイタレータと const 版のイタレータのペア.
template <DIMTYPE STRIDE=DIMENC(1), class T=float, class ZERO = T>
class TVectorIteratorPair{
public:
	typedef T element_type;
	typedef ZERO zero;
	typedef TVectorIterator<STRIDE, T, zero> iterator;
	typedef TVectorIterator<STRIDE, const T, zero> const_iterator;
};

///	ベクトル型のためのメモリ
template <int N, class T>
class TVectorBuffer{
public:
	///	要素の型
	typedef T element_type;
private:
	///	メモリの実体
	element_type value[N];
protected:
	///	メモリの先頭を返す。
	element_type* begin(){ return value; }
	///	メモリの先頭を返す。(const 版)
	const element_type* begin() const { return value; }
};

//------------------------------------------------------------------
///	ベクトル型の基本クラス。普通は派生クラスの TVector を使う。
template <DIMTYPE DIM, class ITP, class BUF>
class TVectorBase:public BUF{
public:
	///@name ベクトルのサイズ.
	//@{
	/** ベクトルの次元(要素数). DIMTYPE は処理系によって異なる。
		DIMTYPE dim = DIM;
		\begin{verbatim}
 DIMENC(10) とすると DIMTYPE 型の10ができる。
 DIMDEF(DIMENC(10), dim);	で、 DIMTYPE dim = DIMENC(10); の意味になる。
 DIMDEC(dim); でdimが表す次元数を int 型で取り出す。
		\end{verbatim}	*/
	DIMDEF(DIM, dim);
	///	ベクトルのサイズ.
	int size() const { return DIMDEC(dim); }
	//@}
	
	///@name 基本型・要素の型などの定義.
	//@{
	///	このクラスの型
	typedef				TVectorBase<DIM, ITP, BUF>			this_type;
	///	要素の型
	typedef typename	ITP::iterator::element_type			element_type;
	///	要素の型(const 版)
	typedef typename	ITP::const_iterator::element_type	const_element_type;
	///	要素のゼロを返す関数オブジェクトの型
	typedef typename	ITP::zero							zero;
	///	基本クラスの型を定義.
	typedef				BUF									base_type;
	///	要素を保持するメモリの型
	typedef				BUF									buffer_type;
	//@}

	///@name 要素のアクセスのための型の定義.
	//@{
	///	イタレータのペア(const iterator と普通のiteratorのペア)の型
	typedef ITP												iterator_pair;
	///	イタレータの型
	typedef typename	ITP::iterator						iterator;
	///	イタレータの型(const 版)
	typedef typename	ITP::const_iterator					const_iterator;
	///	ベクトルを配列で表した型(要素型[要素数] 型).
	typedef element_type array_type [DIMDEC(dim)];
	//@}

	///@name 要素のアクセス.
	//@{
	///	ベクトルの先頭要素へのイタレータを返す.
	iterator begin(){ return iterator(base_type::begin()); }
	///	ベクトルの先頭要素へのイタレータを返す(const 版).
	const_iterator begin() const { return const_iterator(base_type::begin()); }
	///	ベクトルの最後の要素の次へのイタレータを返す.
	iterator end(){ return begin() + size(); }
	///	ベクトルの最後の要素の次へのイタレータを返す(const 版).
	const_iterator end() const { return begin() + size(); }
	///	ベクトルを配列型で返す.
	operator array_type&() {return *(array_type*)(element_type*)begin();}
	///	ベクトルを配列型で返す(const 版).
	operator const array_type&() const {return *(array_type*)(element_type*)begin();}
	///	n番目の要素を返す(基数は0).
	element_type& item(int n){ return begin()[n]; }
	///	n番目の要素を返す(基数は0)(const 版).
	const element_type& item(int n) const { return begin()[n]; }
	///	[] 演算子(基数は0).
	element_type& operator[] (int n) { return item(n); }
	///	[] 演算子(基数は0) (const 版).
	const element_type& operator[] (int n) const { return item(n); }
	//@}

	///@name 基本操作.
	//@{
	/**	ベクトルの要素を全てvにする.
		@param v 要素型の値.	*/
	void clear(const element_type v=zero()){ for(iterator it = begin(); it != end(); ++it){ *it = v; } }
	/**	代入(*this = b).
		@param b 同じサイズのベクトル.	*/
	template <class M_ITP, class M_BUF>
	void assign(const TVectorBase<dim, M_ITP, M_BUF>& b) {
		typename M_ITP::const_iterator itB = b.begin();
		for(this_type::iterator it = begin(); it != end(); ++it, ++itB){
			*it = *itB;
		}
	}
	//@}

	///@ 演算.
	//@{
	/**	加算(*this += b).
		@param b 同じサイズのベクトル.	*/
	template <class M_ITP, class M_BUF>
	void add(const TVectorBase<dim, M_ITP, M_BUF>& b){
		this_type::iterator it=begin();
		for(typename M_ITP::const_iterator itB = b.begin();
		it != end();
		++itB, ++it){
			*it += *itB;
		}
	}
	/**	減算(*this -= b).
		@param b 同じサイズのベクトル.	*/
	template <class M_ITP, class M_BUF>
	void sub(const TVectorBase<dim, M_ITP, M_BUF>& b){
		this_type::iterator it=begin();
		for(typename M_ITP::const_iterator itB = b.begin(); it != end(); ++itB, ++it){
			*it -= *itB;
		}
	}
	/**	定数倍(*this *= b).
		@param b 要素型.				*/
	void multi(const element_type& b){
		for(typename this_type::iterator it=begin(); it != end(); ++it){
			*it *= b;
		}
	}
	/**	定数分の１(*this /= b).
		@param b 要素型.				*/
	void div(const element_type& b){
		for(typename this_type::iterator it=begin(); it != end(); ++it){
			*it /= b;
		}
	}
	/**	内積(return *this * b).
		@param	b	同じサイズのベクトル.
		@return		内積の値(要素型)	*/
	template <class M_ITP, class M_BUF>
	element_type dot(const TVectorBase<dim, M_ITP, M_BUF>& b) const {
		element_type rv = zero();
		const_iterator it = begin();
		for(typename M_ITP::const_iterator itB = b.begin(); it != end(); ++itB, ++it){
			rv += (*it) * (*itB);
		}
		return rv;
	}
	/**	比較(return *this == b).
		@param	b	同じサイズのベクトル.
		@return		bool値.	*/
	template <class M_ITP, class M_BUF>
	bool equal(const TVectorBase<dim, M_ITP, M_BUF>& b) const {
		bool rv = true;
		for(typename M_ITP::const_iterator it = begin(), itB = b.begin(); it != end(); ++itB, ++it){
			rv &= (*it == *itB);
		}
		return rv;
	}
	///	ベクトルのノルム(大きさ)の2乗を返す.
	element_type square() const { return dot(*this); }
	///	ベクトルのノルム(大きさ)を返す.
	element_type norm() const { return sqrt(square()); }
	///	単位ベクトルにする．
	void unitize(){ div(norm()); }
	///	クリッピングする(bとeを頂点とする矩形内に収まるようにする).
	template <DIMTYPE DIMA, class ITPA, class BUFA, DIMTYPE DIMB, class ITPB, class BUFB>
	void clip(const TVectorBase<DIMA, ITPA, BUFA>& b, const TVectorBase<DIMB, ITPB, BUFB>& e){
		for(int i=0; i<size(); ++i){
			if (item(i) < b.item(i)) item(i) = b.item(i);
			if (item(i) > e.item(i)) item(i) = e.item(i);
		}
	}
	//@}

	///@ 入出力
	//@{
	/**	表示.
		@param os 出力先ストリーム	*/
	void print(std::ostream& os) const {
		this_type::const_iterator it=begin();
		os << "(";
		if(it != end()){ os << *it; ++it; }
		for(; it != end(); ++it){ os << " " << *it; }
		os << ")";
	}
	/**	表示.
		@param is 入力元ストリーム	*/
	template <class T>
	void input(T& is) {
		char ch;
		is >> ch;
		for(this_type::iterator it=begin(); it != end(); ++it){
			is >> *it;
		}
		is >> ch;
	}
	//@}
};

/**	ベクトル型派生クラスに必要なメンバの定義.
	派生クラスを作るたびに，このマクロを使ってメンバを作る.
	デフォルトコンストラクタ， 同じサイズのベクトルによる
	初期化・代入などが定義される．
	@param	THIS	新たに宣言する派生クラスの型名.
	@see	TVector	*/
#define DEF_TVECTOR_BASIC_MEMBER(THIS)										\
	typedef THIS this_type;			/*  このクラスの型を定義しておく. */	\
	DIMDEF(base_type::dim, dim);	/*	ベクトルの次元数.	*/				\
	/*  イタレータのペア	*/												\
	typedef base_type::iterator_pair iterator_pair;							\
	/*  イタレータ	*/														\
	typedef base_type::iterator iterator;									\
	/*	イタレータ(const版)	*/												\
	typedef base_type::const_iterator const_iterator;						\
	/*	要素型.	*/															\
	typedef base_type::element_type element_type;							\
	/*	要素型(const版).	*/												\
	typedef base_type::element_type const_element_type;						\
	/*	バッファ型.	*/														\
	typedef base_type::buffer_type buffer_type;								\
	/*  ゼロを返す関数オブジェクト.	*/										\
	typedef base_type::zero zero;											\
	/*  要素数が等しい配列 p による初期化	*/								\
	THIS(element_type* p){													\
		assign( *(PTM::TVectorBase< dim,									\
			PTM::TVectorIteratorPair<DIMENC(1), element_type, zero>,		\
			PTM::TVectorBuffer<DIMDEC(dim), element_type> >*) p );			\
	}																		\
	/*  要素数が等しい配列 p の代入	*/										\
	THIS& operator =(const element_type* p){								\
		assign( *(PTM::TVectorBase< dim,									\
			PTM::TVectorIteratorPair<DIMENC(1), element_type, zero>,		\
			PTM::TVectorBuffer<DIMDEC(dim), element_type> >*) p );			\
		return *this;														\
	}																		\
	/*  次元が等しいベクトル b による初期化	*/								\
	template <class M_ITP, class M_BUF>										\
	THIS(const PTM::TVectorBase<dim, M_ITP, M_BUF>& b){ assign(b); }		\
	/*  次元が等しいベクトル b を代入	*/									\
	template <class M_ITP, class M_BUF> 									\
	THIS& operator =(const PTM::TVectorBase<dim, M_ITP, M_BUF>& b){			\
		assign(b); return *this;											\
	}																		\
	/*  << 演算子による初期化のためのメソッド	*/							\
	typedef PTM::TVectorInitializer<element_type, DIMDEC(dim)>				\
		vector_initializer_type;											\
	typedef PTM::TVectorBase<dim,											\
		PTM::TVectorIteratorPair<DIMENC(1), element_type, zero>,			\
		PTM::TVectorBuffer<DIMDEC(dim), element_type> > basic_vector_type;	\
	THIS(const vector_initializer_type& b){									\
		assign(*(basic_vector_type*) b.value);								\
	}																		\
	THIS& operator =(const vector_initializer_type& b){						\
		assign(*(basic_vector_type*) b.value);								\
		return *this;														\
	}																		\

//----------------------------------------------------------------------------
///	演算子つきのベクトル型基本クラス. 通常はこれは使わず，派生クラスの TVector を使う.
template <DIMTYPE DIM, class ITP, class BUF>
class TVectorBaseOp:public TVectorBase<DIM, ITP, BUF>{
public:
	///	基本クラスの型を定義
	typedef TVectorBase<DIM, ITP, BUF> base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVectorBaseOp);
	///	デフォルトコンストラクタ.
	TVectorBaseOp(){}

	/**	部分ベクトルを返す.
		このベクトルの0要素から S::dim-1要素までの
		部分ベクトルへの参照を返す．
		@param	S	部分ベクトルのサイズを指定するためのオブジェクト.
					S::dim が次元数を表す.
		@return	Sで指定されたサイズのベクトルへの参照を返す.	*/
	template <class S>
	TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(S){
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (element_type*) begin();
	}
	/**	部分ベクトルを返す.
		このベクトルのi要素から i+S::dim-1要素までの
		部分ベクトルへの参照を返す．
		@param	S	部分ベクトルのサイズを指定するためのオブジェクト.
					S::dim が次元数を表す.
		@param	i	部分ベクトルの0要素目が，このベクトルの何行目に当たるのかを指定.
		@return	Sで指定されたサイズのベクトルへの参照を返す.	*/
	template <class S>
	TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(int i, S){
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (element_type*) (begin()+i);
	}
	/**	部分ベクトルを返す(const 版).
		このベクトルの0要素から S::dim-1要素までの
		部分ベクトルへの参照を返す．
		@param	S	部分ベクトルのサイズを指定するためのオブジェクト.
					S::dim が次元数を表す.
		@return	Sで指定されたサイズのベクトルへの参照を返す.	*/
	template <class S>
	const TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(S) const {
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (const element_type*) begin();
	}
	/**	部分ベクトルを返す(const 版).
		このベクトルのi要素から i+S::dim-1要素までの
		部分ベクトルへの参照を返す．
		@param	S	部分ベクトルのサイズを指定するためのオブジェクト.
					S::dim が次元数を表す.
		@param	i	部分ベクトルの0要素目が，このベクトルの何行目に当たるのかを指定.
		@return	Sで指定されたサイズのベクトルへの参照を返す.	*/
	template <class S>
	const TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(int i, S) const {
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (const element_type*) (begin()+i);
	}
	
	/**	+= 演算子(*this = *this + b).
		@param b サイズが等しいベクトル型.	*/
	template <class M_ITP, class M_BUF>
	this_type operator+= (const TVectorBase<dim, M_ITP, M_BUF>& b) { add(b); return *this; }
	/**	-= 演算子(*this = *this - b)
		@param b サイズが等しいベクトル型.	*/
	template <class M_ITP, class M_BUF>
	this_type operator-= (const TVectorBase<dim, M_ITP, M_BUF>& b) { sub(b); return *this; }
	/**	*= 演算子 (*this = *this * b)
		@param b 要素型.	*/
	template <class T> this_type operator*= (T b) { multi(b); return *this; }
	/**	/= 演算子 (*this = 1/b * *this).
		@param b 要素型.	*/
	template <class T> this_type operator/= (T b) { div(b); return *this; }
	/**	- 演算子 (return -*this).	*/
	this_type operator- () const { this_type r(*this); r*=-1; return r; }
	///	単位ベクトルを返す．
	this_type unit() const { this_type r(*this); r.unitize(); return r; }
};
//----------------------------------------------------------------------------
/**	部分ベクトル型作成のためのユーティリティークラス.
	TSubVector<ベクトル型, 次元数>::type で部分行列型を取得できる。	*/
template <class VEC, int N>
class TSubVector:public TVectorBaseOp<DIMENC(N), typename VEC::iterator_pair, typename VEC::buffer_type>{
public:
	typedef TVectorBaseOp<DIMENC(N), typename VEC::iterator_pair, typename VEC::buffer_type> type;
};
/**	部分行列型作成のためのユーティリティークラス.
	TVecDim<次元数> で部分行列型を取得できる。	*/
template <int N>
class TVecDim{
public:
	DIMDEF(DIMENC(N), dim);
};

//----------------------------------------------------------------------------
/**	ベクトル型. TVector<3, float> v; のように使う
	@param	N	要素数.
	@param	T	要素の型.
	@see		TVector型の演算子
*/
template <int N, class T>
class TVector:public TVectorBaseOp<DIMENC(N), TVectorIteratorPair<DIMENC(1),T,T>, TVectorBuffer<N,T> >{
public:
	///	基本クラス型を定義
	typedef TVectorBaseOp<DIMENC(N), TVectorIteratorPair<DIMENC(1),T,T>, TVectorBuffer<N,T> > base_type;
	/**	継承されない基本的なメンバの定義.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVector);
	///	デフォルトコンストラクタ.
	TVector(){}
};

//----------------------------------------------------------------------------
//	グローバル関数・演算子
/**	ベクトルの和(return a + b).
	@param	a	ベクトル型
	@param	b	aと同じ次元数のベクトル型
	@return		aと同じ次元数のTVector型	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
TVectorBaseOp<N, ITPA, BUFA> operator + (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	TVector<DIMDEC(N), TYPENAME ITPA::element_type> rv(a); rv.add(b); return rv;
}
/**	ベクトルの差(return a - b).
	@param	a	ベクトル型
	@param	b	aと同じ次元数のベクトル型
	@return		aと同じ次元数のTVector型	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
TVectorBaseOp<N, ITPA, BUFA> operator - (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	TVector<DIMDEC(N), TYPENAME ITPA::element_type> rv(a); rv.sub(b); return rv;
}
/**	ベクトルの内積(return a * b).
	@param	a	ベクトル型
	@param	b	aと同じ次元数のベクトル型
	@return		要素型	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
typename ITPA::element_type operator * (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	return a.dot(b);
}
/**	ベクトルのスカラー倍(return a * b).
	@param	a	ベクトル型
	@param	b	要素型
	@return		aと同じ次元数のベクトル型	*/
template <DIMTYPE N, class ITPA, class BUFA>
TVectorBaseOp<N, ITPA, BUFA> operator * (const TVectorBaseOp<N, ITPA, BUFA>& a, typename ITPA::element_type b) {
	TVectorBaseOp<N, ITPA, BUFA> rv(a);
	rv.multi(b);
	return rv;
}
/**	ベクトルのスカラー倍(return a * b).
	@param	a	要素型
	@param	b	ベクトル型
	@return		bと同じ次元数のベクトル型	*/
template <DIMTYPE N, class ITPB, class BUFB>
TVectorBaseOp<N, ITPB, BUFB> operator * (typename ITPB::element_type a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	TVectorBaseOp<N, ITPB, BUFB> rv(b);
	rv.multi(a);
	return rv;
}
/**	ベクトルのスカラー分の１(return a / b).
	@param	a	ベクトル型
	@param	b	要素型
	@return		aと同じ次元数のベクトル型	*/
template <DIMTYPE N, class ITPA, class BUFA, class T>
TVectorBaseOp<N, ITPA, BUFA> operator / (const TVectorBaseOp<N, ITPA, BUFA>& a, T b) {
	TVectorBaseOp<N, ITPA, BUFA> rv(a);
	rv.div(b);
	return rv;
}
/**	比較(return a == b).
	@param	a	ベクトル型
	@param	b	要素型aと同じ次元数のベクトル型
	@return		bool	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
bool operator== (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) { return a.equal(b); }
/**	比較(return a != b).
	@param	a	ベクトル型
	@param	b	要素型aと同じ次元数のベクトル型
	@return		bool	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
bool operator!= (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) { return !a.equal(b); }
///	ストリームに書式付で出力する.
template <DIMTYPE N, class ITP, class BUF>
std::ostream& operator << (std::ostream& os, const TVectorBaseOp<N, ITP, BUF>& v){
	v.print(os);
	return os;
}
///	ストリームから書式付で入力する.
template <DIMTYPE N, class ITP, class BUF>
std::istream& operator >> (std::istream& is, TVectorBaseOp<N, ITP, BUF>& v){
	v.input(is);
	return is;
}
/**	2次元ベクトルの外積(return a x b).
	@param	a	2次元ベクトル型
	@param	b	2次元ベクトル型
	@return		要素型	*/
template <class ITPA, class BUFA, class ITPB, class BUFB>
typename ITPA::element_type operator % (const TVectorBaseOp<DIMENC(2), ITPA, BUFA>& a, const TVectorBaseOp<DIMENC(2), ITPB, BUFB>& b) {
	return a[0] * b[1] - a[1] * b[0];
}
/**	3次元ベクトルの外積(return a x b).
	@param	a	3次元ベクトル型
	@param	b	3次元ベクトル型
	@return		3次元ベクトル型	*/
template <class ITPA, class BUFA, class ITPB, class BUFB>
TVector<3, TYPENAME ITPA::element_type> operator % (const TVectorBaseOp<DIMENC(3), ITPA, BUFA>& a, const TVectorBaseOp<DIMENC(3), ITPB, BUFB>& b) {
	TVector<3, TYPENAME ITPA::element_type> r;
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
	return r;
}
/**	2次元ベクトルの外積(return a x b).
	@param	a	2次元ベクトル型
	@param	b	2次元ベクトル型
	@return		要素型	*/
template <class ITPA, class BUFA, class ITPB, class BUFB>
typename ITPA::element_type operator ^ (const TVectorBaseOp<DIMENC(2), ITPA, BUFA>& a, const TVectorBaseOp<DIMENC(2), ITPB, BUFB>& b) {
	return a % b;
}
/**	3次元ベクトルの外積(return a x b).
	@param	a	3次元ベクトル型
	@param	b	3次元ベクトル型
	@return		3次元ベクトル型	*/
template <class ITPA, class BUFA, class ITPB, class BUFB>
TVector<3, TYPENAME ITPA::element_type> operator ^ (const TVectorBaseOp<DIMENC(3), ITPA, BUFA>& a, const TVectorBaseOp<DIMENC(3), ITPB, BUFB>& b) {
	return a % b;
}
#ifdef _DEBUG
 #pragma optimize ("", on)
 #pragma auto_inline(off)
 #pragma inline_recursion(off)
#endif

}	//	namespace PTM
#endif
