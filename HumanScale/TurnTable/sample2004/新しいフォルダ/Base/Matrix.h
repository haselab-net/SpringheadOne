#ifndef	MATRIX_H
#define	MATRIX_H
/**	@file Matrix.h n x m 行列演算，n,mが可変．
*/

#include "Debug.h"
#include <vector>

namespace LVRM {

#define SCALAR float
class Matrix 
	{
	int		m;						//	行列の行の数
	int		n;						//　行列の列の数
	static	SCALAR	rZero;			//　初期化用変数
	std::vector<SCALAR> v;			//	行列の内容
public:
	Matrix( void );					//　コンストラクタ
	Matrix( int , int );			//　コンストラクタ
	~Matrix( void );				//　デストラクタ

	void	Size(int m, int n);		//　行列の大きさの指定
	Matrix	Size();					//	行列の大きさ
	int		Width() const;			//　行列の列数
	int		Height() const;			//　行列の行数
	void	InsLine(int l);			//	行の挿入
	void	InsColmn(int c);		//	列の挿入
	void	DelLine(int l);			//	行の削除
	void	DelColmn(int c);		//	列の削除
	void	Set(int l,int c, Matrix m);
									//	行列の部分代入
	Matrix	Get(int l,int c, int m, int n);
									//	部分行列の取得
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

	void	Zero();					//　零行列
	void	Unit();					//　単位行列
	Matrix	Trans() const;			//　転置行列
	Matrix	Gauss() const;			//　ガウスの消去法
	Matrix	Inv() const;			//　逆行列
	Matrix	PInv() const;			//　疑似逆行列
	SCALAR	Det() const;			//　行列式
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
