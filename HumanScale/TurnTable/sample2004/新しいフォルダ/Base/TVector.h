#ifndef PTMATRIX_TVECTOR_H
#define PTMATRIX_TVECTOR_H
/**	@file TVector.h
	�e���v���[�g�ɂ��N�����x�N�g���̒�`.
	�v�f�̌^�ƃT�C�Y���e���v���[�g�̈����ɂ��邱�ƂŁC
	�Ǘ������������Ɏ������ɁC��ʂ̍s��������D
	�z����L���X�g���ăx�N�g���ɂ��邱�Ƃ��ł���D*/
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
//	�x�N�g���̏������̂��߂̃��[�e�B���e�B�[
//	<< �Ō����ł��鏉�����l�^.
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
//	�������l�^�̍ŏ��̒l
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

///	�x�N�g���A�s��p�̃C�^���[�^.
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

///	���ʂ̃C�^���[�^�� const �ł̃C�^���[�^�̃y�A.
template <DIMTYPE STRIDE=DIMENC(1), class T=float, class ZERO = T>
class TVectorIteratorPair{
public:
	typedef T element_type;
	typedef ZERO zero;
	typedef TVectorIterator<STRIDE, T, zero> iterator;
	typedef TVectorIterator<STRIDE, const T, zero> const_iterator;
};

///	�x�N�g���^�̂��߂̃�����
template <int N, class T>
class TVectorBuffer{
public:
	///	�v�f�̌^
	typedef T element_type;
private:
	///	�������̎���
	element_type value[N];
protected:
	///	�������̐擪��Ԃ��B
	element_type* begin(){ return value; }
	///	�������̐擪��Ԃ��B(const ��)
	const element_type* begin() const { return value; }
};

//------------------------------------------------------------------
///	�x�N�g���^�̊�{�N���X�B���ʂ͔h���N���X�� TVector ���g���B
template <DIMTYPE DIM, class ITP, class BUF>
class TVectorBase:public BUF{
public:
	///@name �x�N�g���̃T�C�Y.
	//@{
	/** �x�N�g���̎���(�v�f��). DIMTYPE �͏����n�ɂ���ĈقȂ�B
		DIMTYPE dim = DIM;
		\begin{verbatim}
 DIMENC(10) �Ƃ���� DIMTYPE �^��10���ł���B
 DIMDEF(DIMENC(10), dim);	�ŁA DIMTYPE dim = DIMENC(10); �̈Ӗ��ɂȂ�B
 DIMDEC(dim); ��dim���\���������� int �^�Ŏ��o���B
		\end{verbatim}	*/
	DIMDEF(DIM, dim);
	///	�x�N�g���̃T�C�Y.
	int size() const { return DIMDEC(dim); }
	//@}
	
	///@name ��{�^�E�v�f�̌^�Ȃǂ̒�`.
	//@{
	///	���̃N���X�̌^
	typedef				TVectorBase<DIM, ITP, BUF>			this_type;
	///	�v�f�̌^
	typedef typename	ITP::iterator::element_type			element_type;
	///	�v�f�̌^(const ��)
	typedef typename	ITP::const_iterator::element_type	const_element_type;
	///	�v�f�̃[����Ԃ��֐��I�u�W�F�N�g�̌^
	typedef typename	ITP::zero							zero;
	///	��{�N���X�̌^���`.
	typedef				BUF									base_type;
	///	�v�f��ێ����郁�����̌^
	typedef				BUF									buffer_type;
	//@}

	///@name �v�f�̃A�N�Z�X�̂��߂̌^�̒�`.
	//@{
	///	�C�^���[�^�̃y�A(const iterator �ƕ��ʂ�iterator�̃y�A)�̌^
	typedef ITP												iterator_pair;
	///	�C�^���[�^�̌^
	typedef typename	ITP::iterator						iterator;
	///	�C�^���[�^�̌^(const ��)
	typedef typename	ITP::const_iterator					const_iterator;
	///	�x�N�g����z��ŕ\�����^(�v�f�^[�v�f��] �^).
	typedef element_type array_type [DIMDEC(dim)];
	//@}

	///@name �v�f�̃A�N�Z�X.
	//@{
	///	�x�N�g���̐擪�v�f�ւ̃C�^���[�^��Ԃ�.
	iterator begin(){ return iterator(base_type::begin()); }
	///	�x�N�g���̐擪�v�f�ւ̃C�^���[�^��Ԃ�(const ��).
	const_iterator begin() const { return const_iterator(base_type::begin()); }
	///	�x�N�g���̍Ō�̗v�f�̎��ւ̃C�^���[�^��Ԃ�.
	iterator end(){ return begin() + size(); }
	///	�x�N�g���̍Ō�̗v�f�̎��ւ̃C�^���[�^��Ԃ�(const ��).
	const_iterator end() const { return begin() + size(); }
	///	�x�N�g����z��^�ŕԂ�.
	operator array_type&() {return *(array_type*)(element_type*)begin();}
	///	�x�N�g����z��^�ŕԂ�(const ��).
	operator const array_type&() const {return *(array_type*)(element_type*)begin();}
	///	n�Ԗڂ̗v�f��Ԃ�(���0).
	element_type& item(int n){ return begin()[n]; }
	///	n�Ԗڂ̗v�f��Ԃ�(���0)(const ��).
	const element_type& item(int n) const { return begin()[n]; }
	///	[] ���Z�q(���0).
	element_type& operator[] (int n) { return item(n); }
	///	[] ���Z�q(���0) (const ��).
	const element_type& operator[] (int n) const { return item(n); }
	//@}

	///@name ��{����.
	//@{
	/**	�x�N�g���̗v�f��S��v�ɂ���.
		@param v �v�f�^�̒l.	*/
	void clear(const element_type v=zero()){ for(iterator it = begin(); it != end(); ++it){ *it = v; } }
	/**	���(*this = b).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class M_ITP, class M_BUF>
	void assign(const TVectorBase<dim, M_ITP, M_BUF>& b) {
		typename M_ITP::const_iterator itB = b.begin();
		for(this_type::iterator it = begin(); it != end(); ++it, ++itB){
			*it = *itB;
		}
	}
	//@}

	///@ ���Z.
	//@{
	/**	���Z(*this += b).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class M_ITP, class M_BUF>
	void add(const TVectorBase<dim, M_ITP, M_BUF>& b){
		this_type::iterator it=begin();
		for(typename M_ITP::const_iterator itB = b.begin();
		it != end();
		++itB, ++it){
			*it += *itB;
		}
	}
	/**	���Z(*this -= b).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class M_ITP, class M_BUF>
	void sub(const TVectorBase<dim, M_ITP, M_BUF>& b){
		this_type::iterator it=begin();
		for(typename M_ITP::const_iterator itB = b.begin(); it != end(); ++itB, ++it){
			*it -= *itB;
		}
	}
	/**	�萔�{(*this *= b).
		@param b �v�f�^.				*/
	void multi(const element_type& b){
		for(typename this_type::iterator it=begin(); it != end(); ++it){
			*it *= b;
		}
	}
	/**	�萔���̂P(*this /= b).
		@param b �v�f�^.				*/
	void div(const element_type& b){
		for(typename this_type::iterator it=begin(); it != end(); ++it){
			*it /= b;
		}
	}
	/**	����(return *this * b).
		@param	b	�����T�C�Y�̃x�N�g��.
		@return		���ς̒l(�v�f�^)	*/
	template <class M_ITP, class M_BUF>
	element_type dot(const TVectorBase<dim, M_ITP, M_BUF>& b) const {
		element_type rv = zero();
		const_iterator it = begin();
		for(typename M_ITP::const_iterator itB = b.begin(); it != end(); ++itB, ++it){
			rv += (*it) * (*itB);
		}
		return rv;
	}
	/**	��r(return *this == b).
		@param	b	�����T�C�Y�̃x�N�g��.
		@return		bool�l.	*/
	template <class M_ITP, class M_BUF>
	bool equal(const TVectorBase<dim, M_ITP, M_BUF>& b) const {
		bool rv = true;
		for(typename M_ITP::const_iterator it = begin(), itB = b.begin(); it != end(); ++itB, ++it){
			rv &= (*it == *itB);
		}
		return rv;
	}
	///	�x�N�g���̃m����(�傫��)��2���Ԃ�.
	element_type square() const { return dot(*this); }
	///	�x�N�g���̃m����(�傫��)��Ԃ�.
	element_type norm() const { return sqrt(square()); }
	///	�P�ʃx�N�g���ɂ���D
	void unitize(){ div(norm()); }
	///	�N���b�s���O����(b��e�𒸓_�Ƃ����`���Ɏ��܂�悤�ɂ���).
	template <DIMTYPE DIMA, class ITPA, class BUFA, DIMTYPE DIMB, class ITPB, class BUFB>
	void clip(const TVectorBase<DIMA, ITPA, BUFA>& b, const TVectorBase<DIMB, ITPB, BUFB>& e){
		for(int i=0; i<size(); ++i){
			if (item(i) < b.item(i)) item(i) = b.item(i);
			if (item(i) > e.item(i)) item(i) = e.item(i);
		}
	}
	//@}

	///@ ���o��
	//@{
	/**	�\��.
		@param os �o�͐�X�g���[��	*/
	void print(std::ostream& os) const {
		this_type::const_iterator it=begin();
		os << "(";
		if(it != end()){ os << *it; ++it; }
		for(; it != end(); ++it){ os << " " << *it; }
		os << ")";
	}
	/**	�\��.
		@param is ���͌��X�g���[��	*/
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

/**	�x�N�g���^�h���N���X�ɕK�v�ȃ����o�̒�`.
	�h���N���X����邽�тɁC���̃}�N�����g���ă����o�����.
	�f�t�H���g�R���X�g���N�^�C �����T�C�Y�̃x�N�g���ɂ��
	�������E����Ȃǂ���`�����D
	@param	THIS	�V���ɐ錾����h���N���X�̌^��.
	@see	TVector	*/
#define DEF_TVECTOR_BASIC_MEMBER(THIS)										\
	typedef THIS this_type;			/*  ���̃N���X�̌^���`���Ă���. */	\
	DIMDEF(base_type::dim, dim);	/*	�x�N�g���̎�����.	*/				\
	/*  �C�^���[�^�̃y�A	*/												\
	typedef base_type::iterator_pair iterator_pair;							\
	/*  �C�^���[�^	*/														\
	typedef base_type::iterator iterator;									\
	/*	�C�^���[�^(const��)	*/												\
	typedef base_type::const_iterator const_iterator;						\
	/*	�v�f�^.	*/															\
	typedef base_type::element_type element_type;							\
	/*	�v�f�^(const��).	*/												\
	typedef base_type::element_type const_element_type;						\
	/*	�o�b�t�@�^.	*/														\
	typedef base_type::buffer_type buffer_type;								\
	/*  �[����Ԃ��֐��I�u�W�F�N�g.	*/										\
	typedef base_type::zero zero;											\
	/*  �v�f�����������z�� p �ɂ�鏉����	*/								\
	THIS(element_type* p){													\
		assign( *(PTM::TVectorBase< dim,									\
			PTM::TVectorIteratorPair<DIMENC(1), element_type, zero>,		\
			PTM::TVectorBuffer<DIMDEC(dim), element_type> >*) p );			\
	}																		\
	/*  �v�f�����������z�� p �̑��	*/										\
	THIS& operator =(const element_type* p){								\
		assign( *(PTM::TVectorBase< dim,									\
			PTM::TVectorIteratorPair<DIMENC(1), element_type, zero>,		\
			PTM::TVectorBuffer<DIMDEC(dim), element_type> >*) p );			\
		return *this;														\
	}																		\
	/*  �������������x�N�g�� b �ɂ�鏉����	*/								\
	template <class M_ITP, class M_BUF>										\
	THIS(const PTM::TVectorBase<dim, M_ITP, M_BUF>& b){ assign(b); }		\
	/*  �������������x�N�g�� b ����	*/									\
	template <class M_ITP, class M_BUF> 									\
	THIS& operator =(const PTM::TVectorBase<dim, M_ITP, M_BUF>& b){			\
		assign(b); return *this;											\
	}																		\
	/*  << ���Z�q�ɂ�鏉�����̂��߂̃��\�b�h	*/							\
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
///	���Z�q���̃x�N�g���^��{�N���X. �ʏ�͂���͎g�킸�C�h���N���X�� TVector ���g��.
template <DIMTYPE DIM, class ITP, class BUF>
class TVectorBaseOp:public TVectorBase<DIM, ITP, BUF>{
public:
	///	��{�N���X�̌^���`
	typedef TVectorBase<DIM, ITP, BUF> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVectorBaseOp);
	///	�f�t�H���g�R���X�g���N�^.
	TVectorBaseOp(){}

	/**	�����x�N�g����Ԃ�.
		���̃x�N�g����0�v�f���� S::dim-1�v�f�܂ł�
		�����x�N�g���ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����x�N�g���̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim ����������\��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̃x�N�g���ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(S){
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (element_type*) begin();
	}
	/**	�����x�N�g����Ԃ�.
		���̃x�N�g����i�v�f���� i+S::dim-1�v�f�܂ł�
		�����x�N�g���ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����x�N�g���̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim ����������\��.
		@param	i	�����x�N�g����0�v�f�ڂ��C���̃x�N�g���̉��s�ڂɓ�����̂����w��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̃x�N�g���ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(int i, S){
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (element_type*) (begin()+i);
	}
	/**	�����x�N�g����Ԃ�(const ��).
		���̃x�N�g����0�v�f���� S::dim-1�v�f�܂ł�
		�����x�N�g���ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����x�N�g���̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim ����������\��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̃x�N�g���ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	const TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(S) const {
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (const element_type*) begin();
	}
	/**	�����x�N�g����Ԃ�(const ��).
		���̃x�N�g����i�v�f���� i+S::dim-1�v�f�܂ł�
		�����x�N�g���ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����x�N�g���̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim ����������\��.
		@param	i	�����x�N�g����0�v�f�ڂ��C���̃x�N�g���̉��s�ڂɓ�����̂����w��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̃x�N�g���ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	const TVectorBaseOp<S::dim, iterator_pair, buffer_type>& sub_vector(int i, S) const {
		return *(TVectorBaseOp<S::dim, iterator_pair, buffer_type>*) (const element_type*) (begin()+i);
	}
	
	/**	+= ���Z�q(*this = *this + b).
		@param b �T�C�Y���������x�N�g���^.	*/
	template <class M_ITP, class M_BUF>
	this_type operator+= (const TVectorBase<dim, M_ITP, M_BUF>& b) { add(b); return *this; }
	/**	-= ���Z�q(*this = *this - b)
		@param b �T�C�Y���������x�N�g���^.	*/
	template <class M_ITP, class M_BUF>
	this_type operator-= (const TVectorBase<dim, M_ITP, M_BUF>& b) { sub(b); return *this; }
	/**	*= ���Z�q (*this = *this * b)
		@param b �v�f�^.	*/
	template <class T> this_type operator*= (T b) { multi(b); return *this; }
	/**	/= ���Z�q (*this = 1/b * *this).
		@param b �v�f�^.	*/
	template <class T> this_type operator/= (T b) { div(b); return *this; }
	/**	- ���Z�q (return -*this).	*/
	this_type operator- () const { this_type r(*this); r*=-1; return r; }
	///	�P�ʃx�N�g����Ԃ��D
	this_type unit() const { this_type r(*this); r.unitize(); return r; }
};
//----------------------------------------------------------------------------
/**	�����x�N�g���^�쐬�̂��߂̃��[�e�B���e�B�[�N���X.
	TSubVector<�x�N�g���^, ������>::type �ŕ����s��^���擾�ł���B	*/
template <class VEC, int N>
class TSubVector:public TVectorBaseOp<DIMENC(N), typename VEC::iterator_pair, typename VEC::buffer_type>{
public:
	typedef TVectorBaseOp<DIMENC(N), typename VEC::iterator_pair, typename VEC::buffer_type> type;
};
/**	�����s��^�쐬�̂��߂̃��[�e�B���e�B�[�N���X.
	TVecDim<������> �ŕ����s��^���擾�ł���B	*/
template <int N>
class TVecDim{
public:
	DIMDEF(DIMENC(N), dim);
};

//----------------------------------------------------------------------------
/**	�x�N�g���^. TVector<3, float> v; �̂悤�Ɏg��
	@param	N	�v�f��.
	@param	T	�v�f�̌^.
	@see		TVector�^�̉��Z�q
*/
template <int N, class T>
class TVector:public TVectorBaseOp<DIMENC(N), TVectorIteratorPair<DIMENC(1),T,T>, TVectorBuffer<N,T> >{
public:
	///	��{�N���X�^���`
	typedef TVectorBaseOp<DIMENC(N), TVectorIteratorPair<DIMENC(1),T,T>, TVectorBuffer<N,T> > base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVector);
	///	�f�t�H���g�R���X�g���N�^.
	TVector(){}
};

//----------------------------------------------------------------------------
//	�O���[�o���֐��E���Z�q
/**	�x�N�g���̘a(return a + b).
	@param	a	�x�N�g���^
	@param	b	a�Ɠ����������̃x�N�g���^
	@return		a�Ɠ�����������TVector�^	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
TVectorBaseOp<N, ITPA, BUFA> operator + (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	TVector<DIMDEC(N), TYPENAME ITPA::element_type> rv(a); rv.add(b); return rv;
}
/**	�x�N�g���̍�(return a - b).
	@param	a	�x�N�g���^
	@param	b	a�Ɠ����������̃x�N�g���^
	@return		a�Ɠ�����������TVector�^	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
TVectorBaseOp<N, ITPA, BUFA> operator - (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	TVector<DIMDEC(N), TYPENAME ITPA::element_type> rv(a); rv.sub(b); return rv;
}
/**	�x�N�g���̓���(return a * b).
	@param	a	�x�N�g���^
	@param	b	a�Ɠ����������̃x�N�g���^
	@return		�v�f�^	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
typename ITPA::element_type operator * (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	return a.dot(b);
}
/**	�x�N�g���̃X�J���[�{(return a * b).
	@param	a	�x�N�g���^
	@param	b	�v�f�^
	@return		a�Ɠ����������̃x�N�g���^	*/
template <DIMTYPE N, class ITPA, class BUFA>
TVectorBaseOp<N, ITPA, BUFA> operator * (const TVectorBaseOp<N, ITPA, BUFA>& a, typename ITPA::element_type b) {
	TVectorBaseOp<N, ITPA, BUFA> rv(a);
	rv.multi(b);
	return rv;
}
/**	�x�N�g���̃X�J���[�{(return a * b).
	@param	a	�v�f�^
	@param	b	�x�N�g���^
	@return		b�Ɠ����������̃x�N�g���^	*/
template <DIMTYPE N, class ITPB, class BUFB>
TVectorBaseOp<N, ITPB, BUFB> operator * (typename ITPB::element_type a, const TVectorBaseOp<N, ITPB, BUFB>& b) {
	TVectorBaseOp<N, ITPB, BUFB> rv(b);
	rv.multi(a);
	return rv;
}
/**	�x�N�g���̃X�J���[���̂P(return a / b).
	@param	a	�x�N�g���^
	@param	b	�v�f�^
	@return		a�Ɠ����������̃x�N�g���^	*/
template <DIMTYPE N, class ITPA, class BUFA, class T>
TVectorBaseOp<N, ITPA, BUFA> operator / (const TVectorBaseOp<N, ITPA, BUFA>& a, T b) {
	TVectorBaseOp<N, ITPA, BUFA> rv(a);
	rv.div(b);
	return rv;
}
/**	��r(return a == b).
	@param	a	�x�N�g���^
	@param	b	�v�f�^a�Ɠ����������̃x�N�g���^
	@return		bool	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
bool operator== (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) { return a.equal(b); }
/**	��r(return a != b).
	@param	a	�x�N�g���^
	@param	b	�v�f�^a�Ɠ����������̃x�N�g���^
	@return		bool	*/
template <DIMTYPE N, class ITPA, class BUFA, class ITPB, class BUFB>
bool operator!= (const TVectorBaseOp<N, ITPA, BUFA>& a, const TVectorBaseOp<N, ITPB, BUFB>& b) { return !a.equal(b); }
///	�X�g���[���ɏ����t�ŏo�͂���.
template <DIMTYPE N, class ITP, class BUF>
std::ostream& operator << (std::ostream& os, const TVectorBaseOp<N, ITP, BUF>& v){
	v.print(os);
	return os;
}
///	�X�g���[�����珑���t�œ��͂���.
template <DIMTYPE N, class ITP, class BUF>
std::istream& operator >> (std::istream& is, TVectorBaseOp<N, ITP, BUF>& v){
	v.input(is);
	return is;
}
/**	2�����x�N�g���̊O��(return a x b).
	@param	a	2�����x�N�g���^
	@param	b	2�����x�N�g���^
	@return		�v�f�^	*/
template <class ITPA, class BUFA, class ITPB, class BUFB>
typename ITPA::element_type operator % (const TVectorBaseOp<DIMENC(2), ITPA, BUFA>& a, const TVectorBaseOp<DIMENC(2), ITPB, BUFB>& b) {
	return a[0] * b[1] - a[1] * b[0];
}
/**	3�����x�N�g���̊O��(return a x b).
	@param	a	3�����x�N�g���^
	@param	b	3�����x�N�g���^
	@return		3�����x�N�g���^	*/
template <class ITPA, class BUFA, class ITPB, class BUFB>
TVector<3, TYPENAME ITPA::element_type> operator % (const TVectorBaseOp<DIMENC(3), ITPA, BUFA>& a, const TVectorBaseOp<DIMENC(3), ITPB, BUFB>& b) {
	TVector<3, TYPENAME ITPA::element_type> r;
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
	return r;
}
/**	2�����x�N�g���̊O��(return a x b).
	@param	a	2�����x�N�g���^
	@param	b	2�����x�N�g���^
	@return		�v�f�^	*/
template <class ITPA, class BUFA, class ITPB, class BUFB>
typename ITPA::element_type operator ^ (const TVectorBaseOp<DIMENC(2), ITPA, BUFA>& a, const TVectorBaseOp<DIMENC(2), ITPB, BUFB>& b) {
	return a % b;
}
/**	3�����x�N�g���̊O��(return a x b).
	@param	a	3�����x�N�g���^
	@param	b	3�����x�N�g���^
	@return		3�����x�N�g���^	*/
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
