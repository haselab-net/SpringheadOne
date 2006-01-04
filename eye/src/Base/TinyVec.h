#ifndef TINYVEC_H
#define TINYVEC_H

#include <Base/TVector.h>

#ifndef PTM_PACK	//	�P�̂Ŏg�p����ꍇ�́Cnamespace �ɓ���Ȃ�
namespace Spr{;
#endif
#ifdef _WIN32
 #pragma pack(push, 4)
#endif

//-----------------------------------------------------------------------------
//		TVec2
template <class EXP, class T> class TVecDesc: public PTM::TVectorDesc<1, EXP, EXP, T>{};
/**	2�����x�N�g���N���X	*/
template<class T>
class TVec2:public PTM::TVectorBase<DIMENC(2), TVecDesc<TVec2<T>, T> >{
public:
	typedef TVecDesc<TVec2<T>, T> desc;
	typedef PTM::TVectorBase<DIMENC(2), desc> base_type;
	///	��{�I�ȃ����o�̒�` @see ::DEF_TVECTOR_BASIC_MEMBER
	DEF_TVECTOR_BASIC_MEMBER(TVec2);

	union{
		T data[2];
		struct{
			T x,y;
		};
	};
	///	�v�f�̃A�N�Z�X
	T& item_impl(size_t i){ return data[i]; }
	///	�v�f�̃A�N�Z�X
	const T& item_impl(size_t i) const { return data[i]; }
	///	�X�g���C�h
	size_t stride_impl() const { return 1; }
	///@name �\�z�E������
	//@{
	///	�R���X�g���N�^
	template <class TX, class TY>
		TVec2(TX xi, TY yi){ X() = element_type(xi); Y() = element_type(yi); }
	//@}

	///@name �������֐�
	//@{
	/// ��x�N�g��
	static TVec2<T> Zero(){TVec2<T> v; v.clear(); return v;}
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
	///	�x�N�g����x����
	const element_type& U() const { return item(0); }
	///	�x�N�g����y����
	const element_type& V() const { return item(1); }
	///	�x�N�g����x����
	element_type& U() { return item(0); }
	///	�x�N�g����y����
	element_type& V() { return item(1); }
	//@}
protected:
	void set_default(){clear();}
};

//-----------------------------------------------------------------------------
//		TVec3
///	3�����x�N�g���N���X.
template<class T>
class TVec3:public PTM::TVectorBase<DIMENC(3), TVecDesc<TVec3<T>, T> >{
public:
	typedef TVecDesc<TVec3<T>, T> desc;
	typedef PTM::TVectorBase<DIMENC(3), desc> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec3);
	union{
		T data[3];
		struct{
			T x,y,z;
		};
	};
	///	�v�f�̃A�N�Z�X
	T& item_impl(size_t i){ return data[i]; }
	///	�v�f�̃A�N�Z�X
	const T& item_impl(size_t i) const { return data[i]; }
	///	�X�g���C�h
	size_t stride_impl() const { return 1; }

	///@name �������֐�
	//@{
	/// ��x�N�g��
	static TVec3<T> Zero(){TVec3<T> v; v.clear(); return v;}
	//@}
	
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

	const element_type& R() const { return item(0); }
	const element_type& G() const { return item(1); }
	const element_type& B() const { return item(2); }
	element_type& R(){ return item(0); }
	element_type& G(){ return item(1); }
	element_type& B(){ return item(2); }
	//@}
	
	///@name �������E�\�z
	//@{
	///	�R���X�g���N�^
	template <class TX, class TY, class TZ> TVec3 (TX xi, TY yi, TZ zi){ X() = element_type(xi); Y() = element_type(yi); Z() = element_type(zi); }
	//@}
	/**	����(return *this * b).
		@param	b	�����T�C�Y�̃x�N�g��.
		@return		���ς̒l(�v�f�^)	*/
	template <class D>
	element_type dot(const PTM::VectorImp<D>& b) const {
		assert(b.size() == size());
		return item(0)*b[0] + item(1)*b[1] + item(2)*b[2];
	}
protected:
	void set_default(){clear();}
};

//-----------------------------------------------------------------------------
//		TVec4
///	4�����x�N�g���N���X.
template<class T>
class TVec4:public PTM::TVectorBase<DIMENC(4), TVecDesc<TVec4<T>, T> >{
public:
	typedef TVecDesc<TVec4<T>, T> desc;
	typedef PTM::TVectorBase<DIMENC(4), desc> base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see ::DEF_TVECTOR_BASIC_MEMBER	*/
	DEF_TVECTOR_BASIC_MEMBER(TVec4);
	union{
		T data[4];
		struct{
			T x,y,z,w;
		};
	};
	///	�v�f�̃A�N�Z�X
	T& item_impl(size_t i){ return data[i]; }
	///	�v�f�̃A�N�Z�X
	const T& item_impl(size_t i) const { return data[i]; }
	///	�X�g���C�h
	size_t stride_impl() const { return 1; }

	///@name �������֐�
	//@{
	/// ��x�N�g��
	static TVec4<T> Zero(){TVec4<T> v; v.clear(); return v;}
	//@}

	///@name �ϐ��A�N�Z�X
	//@{
	///	x����
	const element_type& X() const { return x; }
	///	y����
	const element_type& Y() const { return y; }
	///	z����
	const element_type& Z() const { return z; }
	///	w����
	const element_type& W() const { return w; }
	///	x����
	element_type& X(){ return x; }
	///	y����
	element_type& Y(){ return y; }
	///	z����
	element_type& Z(){ return z; }
	///	w����
	element_type& W(){ return w; }

	const element_type& R() const { return x; }
	const element_type& G() const { return y; }
	const element_type& B() const { return z; }
	const element_type& A() const { return w; }
	element_type& R(){ return x; }
	element_type& G(){ return y; }
	element_type& B(){ return z; }
	element_type& A(){ return w; }
	//@}
	
	///@name �������E�\�z
	//@{
	///	�R���X�g���N�^
	template <class TX, class TY, class TZ, class TW>
	TVec4 (TX xi, TY yi, TZ zi, TW wi){ X() = element_type(xi); Y() = element_type(yi); Z() = element_type(zi); W() = element_type(wi);}
	//@}
protected:
	void set_default(){clear();}
};

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

#ifdef _WIN32
#pragma pack(pop)
#endif

#ifndef PTM_PACK	//	�P�̂Ŏg�p����ꍇ�́Cnamespace �ɓ���Ȃ�
}
#endif

#endif
