#ifndef PTMATRIX_TVECTOR_H
#define PTMATRIX_TVECTOR_H
/**	@file TVector.h
	�e���v���[�g�ɂ��N�����x�N�g���̒�`.
	�v�f�̌^�ƃT�C�Y���e���v���[�g�̈����ɂ��邱�ƂŁC
	�Ǘ������������Ɏ������ɁC��ʂ̍s��������D
	�z����L���X�g���ăx�N�g���ɂ��邱�Ƃ��ł���D*/
#include <algorithm>
#include <iosfwd>
#include <math.h>
#include <assert.h>
#include <stddef.h>
#ifdef _MSC_VER
 #define for if(0); else for
 #pragma warning (disable: 4786)
 #pragma warning (disable: 4200)
#endif

#ifdef __BORLANDC__
 #pragma warn -8027
#endif


namespace PTM {;
#pragma pack(push, 4)
#ifdef _DEBUG
 #pragma optimize ("awgity", on)
#endif
#pragma inline_recursion(on)
#pragma inline_depth(255)

#if defined _MSC_VER && _MSC_VER < 1300
/*	VisualC++�̃e���v���[�g�΍�D
	VC�͂ǂ����C�e���v���[�g�����ɓ����^�̈����𕡐�����ƁC�S���ŏ���
	�����̒l���Ƃ�炵���D����ł́Csize_t ��2�ȏ�n�����Ƃ��ł��Ȃ��D
	�����ŁC�T�C�Y��n�ɂȂ�N���X������āC sizeof �ŃT�C�Y�����o�����Ƃ�
	������ size_t �������œn���悤�ɂ���D*/
 template <size_t N> class TVectorDim{
	char dim[N];
 };
 #define DIMTYPE		class
 #define DIMENC(n)		PTM::TVectorDim<((n)+1)>
 #define DIMDEC(T)		((sizeof(T)-1))
 #define DIMDEF(T, t)	typedef T t
 #define TYPENAME

#else
 //	���̃R���p�C���ɂ͂���ȕςȖ��͂Ȃ��悤��
 #define DIMTYPE size_t
 #define DIMENC(n) (n)
 #define DIMDEC(n) (n)
 #define DIMDEF(T, t) static const size_t t = T
 #define TYPENAME typename
#endif

template <size_t N>
class TVecDim{
public:
	DIMDEF(DIMENC(N), SIZE);
	DIMDEF(DIMENC(N), STRIDE);
};

template <size_t STR, class EXP, class RET, class E, class Z=E>
class TVectorDesc{
public:
	DIMDEF(DIMENC(STR), STRIDE);	///<	stride
	typedef EXP		exp_type;		///<	����
	typedef RET		ret_type;		///<	�Ԃ�l�̌^
	typedef E		element_type;	///<	�v�f�̌^
	typedef Z		zero;			///<	zero(0) �� �v�f�̌^��0��Ԃ��^
};
template <class EXP, class RET, class E, class Z=E>
class VectorDesc{
public:
	typedef EXP		exp_type;		///<	����
	typedef RET		ret_type;		///<	�Ԃ�l�̌^
	typedef E		element_type;	///<	�v�f�̌^
	typedef Z		zero;			///<	zero(0) �� �v�f�̌^��0��Ԃ��^
};

///@name ���Z.
//@{
template <class D> class VectorImp;
template <DIMTYPE N, class D> class TVectorBase;
/**	�x�N�g���̗v�f��S��v�ɂ���.
	@param v �v�f�^�̒l.	*/
template <class D>
void clear(VectorImp<D>& a, const TYPENAME D::element_type v= D::zero(0)){
	for(size_t i=0; i<a.size(); ++i) a.item(i) = v;
}
/**	���(a = b).
	@param b �����T�C�Y�̃x�N�g��.	*/
template <class AD, class BD>
void assign(VectorImp<AD>& a, const VectorImp<BD>& b) {
	a.resize(b.size());
	assert(a.size() == b.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) = b.item(i);
}
///	���(a = b) 2�v�f��p
template <class AD, class BD>
void assign(TVectorBase<DIMENC(2), AD>& a, const TVectorBase<DIMENC(2), BD>& b) {
	assert(a.size() == b.size());
	a.item(0) = b.item(0);
	a.item(1) = b.item(1);
}
///	���(a = b) 3�v�f��p
template <class AD, class BD>
void assign(TVectorBase<DIMENC(3), AD>& a, const TVectorBase<DIMENC(3), BD>& b) {
	assert(a.size() == b.size());
        typedef TYPENAME AD::element_type ET;
	a.item(0) = ET(b.item(0));
	a.item(1) = ET(b.item(1));
	a.item(2) = ET(b.item(2));
}
///	���(a = b) 4�v�f��p
template <class AD, class BD>
void assign(TVectorBase<DIMENC(4), AD>& a, const TVectorBase<DIMENC(4), BD>& b) {
	assert(a.size() == b.size());
	a.item(0) = (TYPENAME AD::element_type)b.item(0);
	a.item(1) = (TYPENAME AD::element_type)b.item(1);
	a.item(2) = (TYPENAME AD::element_type)b.item(2);
	a.item(3) = (TYPENAME AD::element_type)b.item(3);
}
/**	���(*this = b).
	@param b �����T�C�Y�̃x�N�g��.	*/
template <class AD>
void assign(VectorImp<AD>& a, const TYPENAME AD::element_type* b) {
	for(size_t i=0; i<a.size(); ++i) a.item(i) = b[i];
}
/**	���Z(*this += b).
	@param b �����T�C�Y�̃x�N�g��.	*/
template <class AD, class BD>
void add(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() ==a. size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) += (TYPENAME AD::element_type) b.item(i);
}
/**	���Z(*this -= b).
	@param b �����T�C�Y�̃x�N�g��.	*/
template <class AD, class BD>
void sub(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) -= (TYPENAME AD::element_type) b.item(i);
}
/**	�萔�{(*this *= b).
	@param b �v�f�^.				*/
template <class AD>
void multi(VectorImp<AD>& a, const TYPENAME AD::element_type& b){
	for(size_t i=0; i<a.size(); ++i) a.item(i) *= b;
}
/**	�萔�{(*this *= b).
	@param b �v�f�^.				*/
template <class AD, class BD>
void multi_each(VectorImp<AD>& a, const VectorImp<BD>& b){
	for(size_t i=0; i<a.size(); ++i) a.item(i) *= b.item(i);
}
/**	�萔���̂P(*this /= b).
	@param b �v�f�^.				*/
template <class AD>
void div(VectorImp<AD>& a, const TYPENAME AD::element_type& b){
	for(size_t i=0; i<a.size(); ++i) a.item(i) /= b;
}
/**	����(return *this * b).
	@param	b	�����T�C�Y�̃x�N�g��.
	@return		���ς̒l(�v�f�^)	*/
template <class AD, class BD>
TYPENAME AD::element_type dot(const VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	AD::element_type rv = AD::zero(0);
	for(size_t i=0; i<a.size(); ++i) rv += a.item(i)*b.item(i);
	return rv;
}
template <class AD, class BD>
TYPENAME AD::element_type dot(const TVectorBase<DIMENC(3), AD>& a, const TVectorBase<DIMENC(3), BD>& b){
        typedef TYPENAME AD::element_type ET;
	return ET(a.item(0)*b.item(0) + a.item(1)*b.item(1) + a.item(2)*b.item(2));
}
/**	��r(return *this == b).
	@param	b	�����T�C�Y�̃x�N�g��.
	@return		bool�l.	*/
template <class AD, class BD>
bool equal(const VectorImp<AD>& a, const VectorImp<BD>& b){
	if (b.size() != a.size()) return false;
	for(size_t i=0; i<a.size(); ++i){
		if (a.item(i) != b.item(i)) return false;
	}
	return true;
}

/**	�v�f���Ƃ̍ŏ��l(min(*this, b)).
	@param b �����T�C�Y�̃x�N�g��.	*/
template <class AD, class BD>
void element_min_impl(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) = std::min(a.item(i), b.item(i));
}
template <class AD, class BD>
TYPENAME AD::ret_type element_min(const VectorImp<AD>& a, const VectorImp<BD>& b){
	AD::ret_type r(a);
	element_min_impl(r, b);
	return r;
}
/**	�v�f���Ƃ̍ő�l(max(*this, b)).
	@param b �����T�C�Y�̃x�N�g��.	*/
template <class AD, class BD>
void element_max_impl(VectorImp<AD>& a, const VectorImp<BD>& b){
	assert(b.size() == a.size());
	for(size_t i=0; i<a.size(); ++i) a.item(i) = std::max(a.item(i), b.item(i));
}
template <class AD, class BD>
TYPENAME AD::ret_type element_max(const VectorImp<AD>& a, const VectorImp<BD>& b){
	AD::ret_type r(a);
	element_max_impl(r, b);
	return r;
}
///	�N���b�s���O����(b��e�𒸓_�Ƃ����`���Ɏ��܂�悤�ɂ���).
template <class AD, class BD, class ED>
void clip(const VectorImp<AD>& a, const VectorImp<BD>& b, const VectorImp<ED>& e){
	for(size_t i=0; i<a.size(); ++i){
		if (a.item(i) < b.item(i)) a.item(i) = b.item(i);
		if (a.item(i) > e.item(i)) a.item(i) = e.item(i);
	}
}
//@}


template<size_t SZ, size_t STR, class OD> class TVectorSlice;
template<size_t SZ, class OD> class TSubVector;
template<class D> class EVectorSlice;
template<class D> class EVectorRange;
///	�x�N�g���̉��Z�Ȃǂ̎���
template <class D>
class VectorImp{
public:
	typedef D desc;
	typedef TYPENAME desc::exp_type exp_type;
	typedef TYPENAME desc::ret_type ret_type;
	typedef TYPENAME desc::element_type element_type;
	typedef TYPENAME desc::zero zero;
	///	�R���X�g���N�^
	VectorImp(){}

	///@name ��{����.
	//@{
	///	���̂̎擾
	exp_type& exp(){ return *(exp_type*)this; }
	const exp_type& exp() const { return *(const exp_type*)this; }
	///	n�Ԗڂ̗v�f��Ԃ�(���0).
	element_type& item(size_t n){ return exp().item_impl(n); }
	const element_type& item(size_t n) const { return exp().item_impl(n); }
	///	�T�C�Y�̎擾
	size_t size() const { return exp().size_impl(); }
	///	�T�C�Y�̐ݒ�
	void resize(size_t sz){ exp().resize_impl(sz); }
	///	�X�g���C�h�̎擾
	size_t stride(){ return exp().stride_impl(); }
	//@}
	
	///@name �����x�N�g��
	//@{
	/**	�X���C�X��Ԃ��D�e���v���[�g��
		���̃x�N�g����SZ::OFFSET�v�f���� SZ::OFFSET + SZ::SIZE�v�f�܂ł�
		�����x�N�g���ւ̎Q�Ƃ�Ԃ��D
		@param	SZ::OFFSET	�����x�N�g���̍ŏ��̗v�f�̈ʒu.
		@param	SZ::SIZE	�����x�N�g���̃T�C�Y.
		@return	SZ �Ŏw�肳�ꂽ�x�N�g���ւ̎Q�Ƃ�Ԃ�.	*/
	template <class SZ> TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&
	t_slice(SZ){ return (TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&)item(DIMDEC(SZ::OFFSET)); }
	template <class SZ>	const TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&
	t_slice(SZ) const { return (TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&)item(DIMDEC(SZ::OFFSET)); }
	///	�����x�N�g����Ԃ��D�e���v���[�g��
	template <class SZ> TSubVector<DIMDEC(SZ::SIZE), desc>&
	sub_vector(SZ){ return (TSubVector<DIMDEC(SZ::SIZE), desc>&)item(DIMDEC(SZ::OFFSET)); }
	template <class SZ>	const TSubVector<DIMDEC(SZ::SIZE), desc>&
	sub_vector(SZ) const { return (TSubVector<DIMDEC(SZ::SIZE), desc>&)item(DIMDEC(SZ::OFFSET)); }
	/**	�X���C�X��Ԃ��D�T�C�Y�����e���v���[�g��
		���̃x�N�g���� off �v�f���� off + SZ::SIZE�v�f�܂ł�
		�����x�N�g���ւ̎Q�Ƃ�Ԃ��D
		@param	SZ::OFFSET	�����x�N�g���̍ŏ��̗v�f�̈ʒu.
		@param	SZ::SIZE	�����x�N�g���̃T�C�Y.
		@return	SZ �Ŏw�肳�ꂽ�x�N�g���ւ̎Q�Ƃ�Ԃ�.	*/
	template <class SZ> TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&
	t_slice(size_t off, SZ){ return (TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&)item(off); }
	template <class SZ>	const TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&
	t_slice(size_t off, SZ) const { return (TVectorSlice<DIMDEC(SZ::SIZE), DIMDEC(SZ::STRIDE), desc>&)item(off); }
	///	�����x�N�g����Ԃ��D�T�C�Y�����e���v���[�g��
	template <class SZ> TSubVector<DIMDEC(SZ::SIZE), desc>&
	sub_vector(size_t off, SZ){ return (TSubVector<DIMDEC(SZ::SIZE), desc>&)item(off); }
	template <class SZ>	const TSubVector<DIMDEC(SZ::SIZE), desc>&
	sub_vector(size_t off, SZ) const { return (TSubVector<DIMDEC(SZ::SIZE), desc>&)item(off); }
	
	///	�����x�N�g����Ԃ��D�ϐ���
	EVectorSlice<element_type> v_range(size_t off, size_t sz){
		return EVectorSlice<element_type>(sz, stride(), &item(off));
	}
	EVectorSlice<element_type> v_range(size_t off, size_t sz) const {
		return EVectorSlice<element_type>(sz, stride(), &item(off));
	}
	///	�x�N�g���̃X���C�X��Ԃ��D
	EVectorSlice<element_type> v_slice(size_t off, size_t sz, size_t str){
		return EVectorSlice<element_type>(sz, stride()*str, &item(off));
	}
	EVectorSlice<element_type> v_slice(size_t off, size_t sz, size_t str) const {
		return EVectorSlice<element_type>(sz, stride()*str, &item(off));
	}
	//@}
	
	///@name ���Z.
	//@{
	/**	�x�N�g���̗v�f��S��v�ɂ���.
		@param v �v�f�^�̒l.	*/
	void clear(const element_type v=zero(0)){ PTM::clear(exp(), v);	}
	/**	���(*this = b).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class D>
		void assign(const VectorImp<D>& b) { PTM::assign(exp(), b.exp()); }
	/**	���(*this = b).
		@param b �����T�C�Y�̃x�N�g��.	*/
	void assign(const element_type* b) { PTM::assign(exp(), b); }
	/**	���Z(*this += b).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class D>
		void add(const VectorImp<D>& b){ PTM::add(exp(), b.exp()); }
	/**	���Z(*this -= b).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class D>
		void sub(const VectorImp<D>& b){ PTM::sub(exp(), b.exp()); }
	/**	�萔�{(*this *= b).
		@param b �v�f�^.				*/
	void multi(const element_type& b){ PTM::multi(exp(), b); }
	/**	�萔���̂P(*this /= b).
		@param b �v�f�^.				*/
	void div(const element_type& b){ PTM::div(exp(), b); }
	/**	����(return *this * b).
		@param	b	�����T�C�Y�̃x�N�g��.
		@return		���ς̒l(�v�f�^)	*/
	template <class D>
		element_type dot(const VectorImp<D>& b) const { return PTM::dot(exp(), b.exp()); }
	/**	��r(return *this == b).
		@param	b	�����T�C�Y�̃x�N�g��.
		@return		bool�l.	*/
	template <class D>
		bool equal(const VectorImp<D>& b) const { return PTM::equal(exp(), b.exp()); }

	/**	�v�f���Ƃ̍ŏ��l(min(*this, b)).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class D>
		void element_min(const VectorImp<D>& b){ element_min_impl(exp(), b.exp()); }
	/**	�v�f���Ƃ̍ő�l(max(*this, b)).
		@param b �����T�C�Y�̃x�N�g��.	*/
	template <class D>
		void element_max(const VectorImp<D>& b){ element_max_impl(exp(), b.exp()); }
	///	�x�N�g���̃m����(�傫��)��2���Ԃ�.
	element_type square() const { return PTM::dot(exp(),exp()); }
	///	�x�N�g���̃m����(�傫��)��Ԃ�.
	element_type norm() const { return element_type( sqrt(square()) ); }
	///	�P�ʃx�N�g���ɂ���D
	void unitize(){ div(norm()); }
	///	�N���b�s���O����(b��e�𒸓_�Ƃ����`���Ɏ��܂�悤�ɂ���).
	template <class BD, class ED>
		void clip(const VectorImp<BD>& b, const VectorImp<ED>& e){ clip(exp(), b.exp(), e.exp()); }
	//@}

	///@name ���o��
	//@{
	/**	�\��.
		@param os �o�͐�X�g���[��	*/
	void print(std::ostream& os) const {
//		os << size();
		os << "(";
		if (size()){ os.width(6); os << item(0); }
		for(size_t i=1; i<size(); ++i){ os << " "; os.width(6); os << item(i); }
		os << ")";
	}
	/**	�\��.
		@param is ���͌��X�g���[��	*/
	template <class T>
	void input(T& is) {
		char ch;
		is >> ws;
		if (is.peek() != '('){
			is.setstate(ios::badbit);
			return;
		}
		is.get();
		std::vector<element_type> v;
		while(1){
			v.push_back(element_type());
			is >> v.back();
			if (!is.good()) break;
			if (size()<v.size()) resize(v.size());
			if (size()<v.size()) break;
		}
		v.pop_back();
		for(unsigned i=0; i<v.size(); ++i){
			item(i) = v[i];
		}
		is.clear();
		is >> ws;
		if (is.peek() == ')'){
            is.get();
			return;
		}
		is.setstate(ios::badbit);
	}
	//@}

	///@name ���Z�q
	//@{
	///	[] ���Z�q(���0).
	template <class I> element_type& operator[] (I n) { return item(n); }
	///	[] ���Z�q(���0) (const ��).
	template <class I> const element_type& operator[] (I n) const { return item(n); }
	///	element_type * �ւ̕ϊ�
	operator element_type* (){ return &item(0); }
	operator const element_type* () const { return &item(0); }
	/**	�x�N�g���̃X�J���[�{(return this * b).
		@param	b	�v�f�^
		@return	this�Ɠ����x�N�g���^	*/
	ret_type operator * (element_type b) const { desc::ret_type rv(*this); rv.multi(b); return rv; }
	/**	�x�N�g���̃X�J���[���̂P(return this / b).
		@param	b	�v�f�^
		@return	this�Ɠ����x�N�g���^	*/
	ret_type operator / (element_type b) const { desc::ret_type rv(*this); rv.div(b); return rv; }
	/**	+= ���Z�q(*this = *this + b).
		@param b �T�C�Y���������x�N�g���^.	*/
	template <class D>
	ret_type operator+= (const VectorImp<D>& b) { add(b); return *this; }
	/**	-= ���Z�q(*this = *this - b)
		@param b �T�C�Y���������x�N�g���^.	*/
	template <class D>
	ret_type operator-= (const VectorImp<D>& b) { sub(b); return *this; }
	/**	*= ���Z�q (*this = *this * b)
		@param b �v�f�^.	*/
	template <class T> ret_type operator*= (T b) { multi(element_type(b)); return *this; }
	/**	/= ���Z�q (*this = 1/b * *this).
		@param b �v�f�^.	*/
	template <class T> ret_type operator/= (T b) { div(element_type(b)); return *this; }
	/**	- ���Z�q (return -*this).	*/
	ret_type operator- () const { desc::ret_type r(*this); r*=element_type(-1); return r; }
	///	�P�ʃx�N�g����Ԃ��D
	ret_type unit() const { desc::ret_type r(*this); r.unitize(); return r; }
	//@}
protected:
	///	�o�b�t�@�̏������D�������Ȃ��D
	void init_buffer(){}
	///	�f�t�H���g�R���X�g���N�^
	void set_default(){}
};

///	�������e���v���[�g�Ŏ��x�N�g���̊�{�^
template<DIMTYPE N, class D>
class TVectorBase: public VectorImp<D> {
protected:
	TVectorBase(){}
public:
	DIMDEF(N, SIZE);
	size_t size_impl() const { return DIMDEC(N); }
	void resize_impl(size_t sz) { assert(sz==DIMDEC(N)); }
};


/**	�x�N�g���^�h���N���X�ɕK�v�ȃ����o�̒�`.
	�h���N���X����邽�тɁC���̃}�N�����g���ă����o�����.
	�f�t�H���g�R���X�g���N�^�C �����T�C�Y�̃x�N�g���ɂ��
	�������E����Ȃǂ���`�����D
	@param	THIS	�V���ɐ錾����h���N���X�̌^��.
	@see	TVector	*/
#define DEF_VECTOR_BASIC_MEMBER(THIS)										\
	typedef THIS this_type;			/*  ���̃N���X�̌^���`���Ă���. */	\
	typedef TYPENAME desc::element_type element_type;						\
	typedef TYPENAME desc::ret_type ret_type;								\
	/*	�R�s�[�R���X�g���N�^	*/											\
	THIS& operator =(const THIS& b){										\
		assign(b); return *this;											\
	}																		\
	/*  �x�N�g�� b ����	*/												\
	template <class D>														\
	THIS& operator =(const PTM::VectorImp<D>& b){							\
		exp().assign(b); return *this;										\
	}																		\
	/*  �x�N�g�� b �����Z	*/												\
	template <class D>														\
	THIS& operator +=(const PTM::VectorImp<D>& b){							\
		exp().add(b); return *this;											\
	}																		\
	/*  �x�N�g�� b �����Z	*/												\
	template <class D>														\
	THIS& operator -=(const PTM::VectorImp<D>& b){							\
		exp().sub(b); return *this;											\
	}																		\
	/*  �v�f�����������z�� p �̑��	*/										\
	THIS& operator =(const element_type* p){								\
		exp().assign(p);													\
		return *this;														\
	}																		\

#define DEF_TVECTOR_BASIC_MEMBER(THIS)										\
	DEF_VECTOR_BASIC_MEMBER(THIS)											\
	/*  �x�N�g�� b �ɂ�鏉����		*/										\
	template <class D>														\
	THIS(const PTM::VectorImp<D>& b){init_buffer(); assign(b);}				\
	/*  �v�f�����������z�� p �ɂ�鏉����	*/								\
	THIS(const element_type* p){											\
		assign(p);															\
	}																		\
	/*	�f�t�H���g�R���X�g���N�^	*/										\
	THIS(){ init_buffer(); set_default(); }									\

//----------------------------------------------------------------------------
/**	�����x�N�g���^�쐬�̂��߂̃��[�e�B���e�B�[�N���X.
	TVecDim<������> �ŕ����x�N�g���^���擾�ł���B	*/
template <const size_t SOFF, const size_t SDIM>
class TSubVectorDim{
public:
	DIMDEF(DIMENC(SOFF),	OFFSET);
	DIMDEF(DIMENC(SDIM),	SIZE);
	DIMDEF(DIMENC(1),		STRIDE);
};
//----------------------------------------------------------------------------
/**	�x�N�g���^. TVector<3, float> v; �̂悤�Ɏg��
	@param	N	�v�f��.
	@param	T	�v�f�̌^.
	@see		TVector�^�̉��Z�q
*/
template <size_t N, class T>
class TVector:public TVectorBase<DIMENC(N), TVectorDesc<1, TVector<N,T>, TVector<N,T>, T> >{
public:
	DIMDEF(DIMENC(1), STRIDE);
	typedef TVectorDesc<1, TVector<N,T>, TVector<N,T>, T> desc;	///<	�^���
	typedef TVectorBase<DIMENC(N),desc> base_type;				///<	��{�N���X�^
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVector);
private:
	element_type data[N];										///<	�f�[�^
public:	
	///	�v�f�̃A�N�Z�X
	T& item_impl(size_t i){ return data[i]; }
	///	�v�f�̃A�N�Z�X
	const T& item_impl(size_t i) const { return data[i]; }
	///	�X�g���C�h
	size_t stride_impl() const { return 1; }
};

template<size_t SZ, size_t STR, class OD>
class TVectorSlice: public TVectorBase<DIMENC(SZ),
	TVectorDesc< STR*DIMDEC(OD::STRIDE), 
		TVectorSlice<SZ, STR, OD>, TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> > {
public:
	DIMDEF(DIMENC(SZ), SIZE);
	DIMDEF(DIMENC(STR), STRIDE);
	typedef void array_type;
	typedef void const_array_type;
	typedef TVectorDesc< STR*DIMDEC(OD::STRIDE), TVectorSlice<SZ, STR, OD>, TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> desc;
	typedef TVectorBase<DIMENC(SZ), desc> base_type;
	///	�p������Ȃ���{�I�ȃ����o�̒�`  @see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(TVectorSlice);
	///	�v�f�̃A�N�Z�X
	element_type& item_impl(size_t i){ return data[i][0]; }
	const element_type& item_impl(size_t i) const { return data[i][0]; }
	///	�X�g���C�h
	size_t stride_impl() const { return DIMDEC(STRIDE); }
protected:
	element_type data[DIMDEC(SIZE)][DIMDEC(STRIDE)];
};


template<size_t SZ, class OD>
class TSubVector: public TVectorBase<DIMENC(SZ),
	TVectorDesc< DIMDEC(OD::STRIDE), TSubVector<SZ, OD>, TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> > {
public:
	DIMDEF(DIMENC(SZ), SIZE);
	DIMDEF(OD::STRIDE, STRIDE);
	typedef void array_type;
	typedef void const_array_type;
	typedef TVectorDesc< DIMDEC(OD::STRIDE), TSubVector<SZ, OD>, TVector<SZ, TYPENAME OD::element_type>, TYPENAME OD::element_type, TYPENAME OD::zero> desc;
	typedef TVectorBase<DIMENC(SZ), desc> base_type;
	///	�p������Ȃ���{�I�ȃ����o�̒�`  @see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(TSubVector);
	///	�v�f�̃A�N�Z�X
	element_type& item_impl(size_t i){ return data[i][0]; }
	const element_type& item_impl(size_t i) const { return data[i][0]; }
	///	�X�g���C�h
	size_t stride_impl() const { return DIMDEC(STRIDE); }
protected:
	element_type data[DIMDEC(SIZE)][DIMDEC(STRIDE)];
};

//----------------------------------------------------------------------------

/**	�x�N�g���^. VVector<float> v; �̂悤�Ɏg��
	@param	T	�v�f�̌^.
	@see		TVector�^�̉��Z�q
*/
template <class T>
class VVector:public VectorImp<TVectorDesc<1, VVector<T>,VVector<T>,T> >{
public:
	typedef TVectorDesc<1,VVector<T>,VVector<T>,T> desc;
	typedef VectorImp<desc> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_VECTOR_BASIC_MEMBER	*/
	DEF_VECTOR_BASIC_MEMBER(VVector);
	///	��{�^�̒�`
	typedef element_type* array_type;
	typedef const element_type* const_array_type;
private:
	///	�������̎���
	array_type data;
	///
	size_t size_;
public:
	///	�f�t�H���g�R���X�g���N�^
	VVector(){
		init_buffer();
		set_default();
	}
	///	�R�s�[�R���X�g���N�^
	VVector(const VVector& s){
		init_buffer();
		resize(s.size_);
		memcpy(data, s.data, sizeof(element_type) * size_);
	}
	template <class D>
	VVector(const VectorImp<D>& b){
		init_buffer();
		assign(b);
	}
	///	
	~VVector(){
    	delete [] data;
    }
	///	�x�N�g���̃T�C�Y.
	size_t size_impl() const { return size_; }
	///	�x�N�g���̃T�C�Y�̐ݒ�
	void resize_impl(size_t s){
		if (s > size_){
			delete data;
			data = new element_type[s];
		}
		size_ = s;
	}
	///	�X�g���C�h
	size_t stride_impl() const { return 1; }
	///	�v�f�̃A�N�Z�X
	T& item_impl(size_t i){ return data[i]; }
	///	�v�f�̃A�N�Z�X
	const T& item_impl(size_t i) const { return data[i]; }
protected:
	void init_buffer(){data=0; size_=0;}
};

/**	�x�N�g���^. EVector<float> v; �̂悤�Ɏg��
	@param	T	�v�f�̌^.
	@see		Vector�^�̉��Z�q
*/
template <class T>
class ConstEVector:public VectorImp< TVectorDesc<1, ConstEVector<T>, VVector<T>,T,T> >{
public:
	typedef TVectorDesc<1, ConstEVector<T>,VVector<T>,T,T> desc;
	typedef VectorImp<desc> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_VECTOR_BASIC_MEMBER	*/
	DEF_VECTOR_BASIC_MEMBER(ConstEVector);
	typedef const element_type* array_type;
private:
	///	�������̎���
	array_type data;
	///
	size_t size_;
public:
	///
	ConstEVector(array_type v, size_t sz):data(v), size_(sz){}
	//
	ConstEVector(size_t sz, size_t str, array_type v):data(v), size_(sz){ assert(str==1);}
	///	�x�N�g���̃T�C�Y.
	size_t size_impl() const { return size_; }
	///	�x�N�g���̃T�C�Y�̐ݒ�
	void resize_impl(size_t s){
		assert(size_ == s);
	}
	///	�v�f�̃A�N�Z�X
	const T& item_impl(size_t i) const { return data[i]; }
	///	�X�g���C�h
	size_t stride_impl() const { return 1; }
};
template <class T>
class EVector:public VectorImp< TVectorDesc<1, EVector<T>, VVector<T>,T,T> >{
public:
	typedef ConstEVector<T> const_type;
	typedef TVectorDesc<1, EVector<T>,VVector<T>,T,T> desc;
	typedef VectorImp<desc> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_VECTOR_BASIC_MEMBER	*/
	DEF_VECTOR_BASIC_MEMBER(EVector);
	typedef element_type* array_type;
private:
	///	�������̎���
	array_type data;
	///
	size_t size_;
public:
	///
	EVector(size_t sz, array_type v):data(v), size_(sz){}
	EVector(size_t sz, size_t str, array_type v):data(v), size_(sz){ assert(str==1); }
	///	�x�N�g���̃T�C�Y.
	size_t size_impl() const { return size_; }
	///	�x�N�g���̃T�C�Y�̐ݒ�
	void resize_impl(size_t s){
		assert(size_ == s);
	}
	///	�v�f�̃A�N�Z�X
	const T& item_impl(size_t i) const { return data[i]; }
	T& item_impl(size_t i) { return data[i]; }
	///	�X�g���C�h
	size_t stride_impl() const { return 1; }
};
template<class T>
class ConstEVectorSlice: public VectorImp< VectorDesc<ConstEVectorSlice<T>, VVector<T>, T> >{
public:
	typedef void array_type;
	typedef void const_array_type;
	typedef VectorDesc<EVectorSlice<T>, VVector<T>, T> desc;
	typedef VectorImp<desc> base_type;
	///	�p������Ȃ���{�I�ȃ����o�̒�`.	@see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(ConstEVectorSlice);
	///
	ConstEVectorSlice(size_t sz, size_t str, const T* d):size_(sz), stride_(str), data(d){}
	///
	ConstEVectorSlice(const ConstEVectorSlice& v):size_(v.size_), stride_(v.stride_), data(v.data){}
	///	�v�f�̃A�N�Z�X
	element_type& item_impl(size_t i){ return data[index(i)]; }
	///	�v�f�̃A�N�Z�X
	const element_type& item_impl(size_t i) const { return data[index(i)]; }
	///	�T�C�Y
	size_t size_impl() const { return size_; }
	///	�T�C�Y�̐ݒ�
	void resize_impl(size_t sz) { size_ = sz; }
	///	�X�g���C�h
	size_t stride_impl() const { return stride_; }
protected:
	const element_type* data;
	size_t size_;
	size_t stride_;
	///	���̍s��ւ̃A�N�Z�X
	size_t index(size_t i) const { return i*stride_; }
};
template<class T>
class EVectorSlice: public VectorImp< VectorDesc<EVectorSlice<T>, VVector<T>, T> >{
public:
	typedef ConstEVectorSlice<T> const_type;
	typedef void array_type;
	typedef void const_array_type;
	typedef VectorDesc<EVectorSlice<T>, VVector<T>, T> desc;
	typedef VectorImp<desc> base_type;
	///	�p������Ȃ���{�I�ȃ����o�̒�`.	@see ::DEF_VECTOR_BASIC_MEMBER
	DEF_VECTOR_BASIC_MEMBER(EVectorSlice);
	///
	EVectorSlice(size_t sz, size_t str, T* d):size_(sz), stride_(str), data(d){}
	///
	EVectorSlice(const EVectorSlice& v):size_(v.size_), stride_(v.stride_), data(v.data){}
	///	�v�f�̃A�N�Z�X
	element_type& item_impl(size_t i){ return data[index(i)]; }
	///	�v�f�̃A�N�Z�X
	const element_type& item_impl(size_t i) const { return data[index(i)]; }
	///	�T�C�Y
	size_t size_impl() const { return size_; }
	///	�T�C�Y�̐ݒ�
	void resize_impl(size_t sz) { size_ = sz; }
	///	�X�g���C�h
	size_t stride_impl() const { return stride_; }
protected:
	element_type* data;
	size_t size_;
	size_t stride_;
	///	���̍s��ւ̃A�N�Z�X
	size_t index(size_t i) const { return i*stride_; }
};

//----------------------------------------------------------------------------
//	�O���[�o���֐��E���Z�q

/**	�x�N�g���̘a(return a + b).
	@param	a	�x�N�g���^
	@param	b	a�Ɠ����������̃x�N�g���^
	@return		a�Ɠ�����������TVector�^	*/
template <class AD, class BD>
TYPENAME AD::ret_type operator + (const VectorImp<AD>& a, const VectorImp<BD>& b) {
	AD::ret_type rv(a);
	rv.add(b);
	return rv;
}
/**	�x�N�g���̍�(return a - b).
	@param	a	�x�N�g���^
	@param	b	a�Ɠ����������̃x�N�g���^
	@return		a�Ɠ����������̃x�N�g���^	*/
template <class AD, class BD>
TYPENAME AD::ret_type operator - (const VectorImp<AD>& a, const VectorImp<BD>& b) {
	AD::ret_type rv(a);
	rv.sub(b);
	return rv;
}
/**	�X�J���[�ƃx�N�g���̐�(return a * b).
	@param	a	�x�N�g���^
	@param	b	�v�f�^
	@return		�x�N�g���^	*/
template <class BD>
typename BD::ret_type operator * (const TYPENAME BD::element_type& a, const VectorImp<BD>& b) {
	BD::ret_type r(b);
	r.multi(a);
	return r;
}

/**	�x�N�g���̓���(return a * b).
	@param	a	�x�N�g���^
	@param	b	a�Ɠ����������̃x�N�g���^
	@return		�v�f�^	*/
template <class AD, class BD>
typename AD::element_type operator * (const VectorImp<AD>& a, const VectorImp<BD>& b) {
	return a.dot(b);
}

/**	��r(return a == b).
	@param	a	�x�N�g���^
	@param	b	�v�f�^a�Ɠ����������̃x�N�g���^
	@return		bool	*/
template <class AD, class BD>
bool operator == (const VectorImp<AD>& a, const VectorImp<BD>& b) { return a.equal(b); }
/**	��r(return a != b).
	@param	a	�x�N�g���^
	@param	b	�v�f�^a�Ɠ����������̃x�N�g���^
	@return		bool	*/
template <class AD, class BD>
bool operator != (const VectorImp<AD>& a, const VectorImp<BD>& b) { return !a.equal(b); }

///	�X�g���[���ɏ����t�ŏo�͂���.
template <class BD>
std::ostream& operator << (std::ostream& os, const VectorImp<BD>& v){
	v.print(os);
	return os;
}
///	�X�g���[�����珑���t�œ��͂���.
template <class BD>
std::istream& operator >> (std::istream& is, VectorImp<BD>& v){
	v.input(is);
	return is;
}
/**	2�����x�N�g���̊O��(return a x b).
	@param	a	2�����x�N�g���^
	@param	b	2�����x�N�g���^
	@return		�v�f�^	*/
template <class	AD, class BD>
typename AD::element_type cross (const TVectorBase<DIMENC(2), AD>& a, const TVectorBase<DIMENC(2), BD>& b) {
	return a[0] * b[1] - a[1] * b[0];
}
template <class	AD, class BD>
typename AD::element_type operator % (const TVectorBase<DIMENC(2), AD>& a, const TVectorBase<DIMENC(2), BD>& b) {
	return cross(a, b);
}
template <class	AD, class BD>
typename AD::element_type operator ^ (const TVectorBase<DIMENC(2), AD>& a, const TVectorBase<DIMENC(2), BD>& b) {
	return cross(a, b);
}

/**	3�����x�N�g���̊O��(return a x b).
	@param	a	3�����x�N�g���^
	@param	b	3�����x�N�g���^
	@return		3�����x�N�g���^	*/
template <class	AD, class BD>
TYPENAME AD::ret_type cross (const TVectorBase<DIMENC(3), AD>& a, const TVectorBase<DIMENC(3), BD>& b) {
	AD::ret_type r;
	r[0] = AD::element_type(a[1] * b[2] - a[2] * b[1]);
	r[1] = AD::element_type(a[2] * b[0] - a[0] * b[2]);
	r[2] = AD::element_type(a[0] * b[1] - a[1] * b[0]);
	return r;
}

template <class	AD, class BD>
TYPENAME AD::ret_type operator % (const TVectorBase<DIMENC(3), AD>& a, const TVectorBase<DIMENC(3), BD>& b) {
	return cross(a, b);
}
template <class	AD, class BD>
TYPENAME AD::ret_type operator ^ (const TVectorBase<DIMENC(3), AD>& a, const TVectorBase<DIMENC(3), BD>& b) {
	return cross(a, b);
}


#ifdef _DEBUG
 #pragma optimize ("", on)
 #pragma auto_inline(off)
 #pragma inline_recursion(off)
#endif
#pragma pack(pop)

}	//	namespace PTM
#endif

