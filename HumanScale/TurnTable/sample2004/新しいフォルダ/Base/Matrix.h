#ifndef	MATRIX_H
#define	MATRIX_H
/**	@file Matrix.h n x m �s�񉉎Z�Cn,m���ρD
*/

#include "Debug.h"
#include <vector>

namespace LVRM {

#define SCALAR float
class Matrix 
	{
	int		m;						//	�s��̍s�̐�
	int		n;						//�@�s��̗�̐�
	static	SCALAR	rZero;			//�@�������p�ϐ�
	std::vector<SCALAR> v;			//	�s��̓��e
public:
	Matrix( void );					//�@�R���X�g���N�^
	Matrix( int , int );			//�@�R���X�g���N�^
	~Matrix( void );				//�@�f�X�g���N�^

	void	Size(int m, int n);		//�@�s��̑傫���̎w��
	Matrix	Size();					//	�s��̑傫��
	int		Width() const;			//�@�s��̗�
	int		Height() const;			//�@�s��̍s��
	void	InsLine(int l);			//	�s�̑}��
	void	InsColmn(int c);		//	��̑}��
	void	DelLine(int l);			//	�s�̍폜
	void	DelColmn(int c);		//	��̍폜
	void	Set(int l,int c, Matrix m);
									//	�s��̕������
	Matrix	Get(int l,int c, int m, int n);
									//	�����s��̎擾
	const SCALAR* operator[]( int ) const;
	SCALAR*		operator[]( int );
	Matrix		operator+=( const Matrix& );
	Matrix		operator-=( const Matrix& );
	Matrix		operator*=( const Matrix& );
	Matrix		operator*=( SCALAR );
	Matrix		operator/=( SCALAR );
	Matrix		operator+() const;
	Matrix		operator-() const;
	Matrix		operator+( const Matrix& ) const;
	Matrix		operator-( const Matrix& ) const;
	Matrix		operator*( const Matrix& ) const;
	Matrix		operator*( SCALAR ) const;
	Matrix		operator/( SCALAR ) const;
	
	friend	Matrix	operator*( SCALAR , const Matrix& );

	void	Zero();					//�@��s��
	void	Unit();					//�@�P�ʍs��
	Matrix	Trans() const;			//�@�]�u�s��
	Matrix	Gauss() const;			//�@�K�E�X�̏����@
	Matrix	Inv() const;			//�@�t�s��
	Matrix	PInv() const;			//�@�^���t�s��
	SCALAR	Det() const;			//�@�s��
	void	Load(std::istream& istr);
	void	Save(std::ostream& ostr) const;
	};

inline int Matrix::Height() const
	{
	return m;
	}
inline int Matrix::Width() const
	{
	return n;
	}
#ifdef DEBUG
class REALARRAY
	{
	int width;
	SCALAR* ptr;
	REALARRAY(SCALAR* p, int w)
		{
		ptr = p;
		width = w;
		}
	SCALAR& operator[](int sn) const
		{
		VRASSERT(0 <= sn && sn < width);
		return ptr[sn];
		}
	};
inline REALARRAY Matrix::operator[]( int sm ) const
	{
	VRASSERT(0 <= sm && sm < Height());
	REALARRAY ra( (NAMESPACE(std) vector<SCALAR>::iterator)v.begin() + (sm * n), Width());
	}
#else
inline const SCALAR* Matrix::operator[]( int sm ) const
	{
	VRASSERT(0 <= sm && sm < Height());
	return v.begin() + (sm * n);
	}
inline SCALAR* Matrix::operator[]( int sm )
	{
	VRASSERT(0 <= sm && sm < Height());
	return v.begin() + (sm * n);
	}
#endif
std::ostream& operator << (std::ostream& ostr, const Matrix& m);
std::istream& operator >> (std::istream& istr,Matrix& m);

}	//	namespace LVRM

#endif
