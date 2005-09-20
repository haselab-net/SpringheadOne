#include "Matrix.h"
#include <math.h>
#include <iostream>

namespace LVRM {
SCALAR	Matrix::rZero = 0.0;

//　エラーメッセージの出力　//

static void error( char* err = "" )
	{
#ifdef USEMFC
	TRACE("Can't calculate. %s\n", err);
#endif
	VRASSERT(false);
	}

Matrix::Matrix( void )
	{
	n = m = 0;
	}

Matrix::Matrix( int sm , int sn )
	{
	Size(sm, sn);
	}

Matrix::~Matrix()
	{
	}

void Matrix::Size(int sm , int sn)
	{
	m = sm;
	n = sn;
	v.clear();
	for(int i=0; i<m*n; i++) v.push_back(rZero);
	}
Matrix Matrix::Size()
	{
	Matrix mat(1,2);
	mat[0][0] = m;
	mat[0][1] = n;
	return mat;
	}
void Matrix::InsLine(int l)
	{
	VRASSERT(l<=m);
	v.insert(v.begin() + l*n, n, rZero);
	m ++;
	}
void Matrix::InsColmn(int c)
	{
	VRASSERT(c<=n);
	v.reserve(m*(n+1));
	for(int i=m-1; i>=0; i--) v.insert(v.begin()+m*n+c, rZero);
	n ++;
	}
void Matrix::DelLine(int l)
	{
	VRASSERT(l<m);
	v.erase(v.begin() + l*n, v.begin() + (l+1)*n);
	m --;
	}
void Matrix::DelColmn(int c)
	{
	VRASSERT(c<n);
	for(int i=m-1; i>=0; i--) v.erase(v.begin()+m*n+c);
	n --;
	}
void Matrix::Set(int l,int c, Matrix s)
	{
	VRASSERT(l + s.Height() <= Height());
	VRASSERT(c + s.Width() <= Width());
	for(int i=0; i<s.Height(); i++)
		for(int j=0; j<s.Width(); j++) (*this)[l+i][c+j] = s[i][j];
	}
Matrix Matrix::Get(int l,int c, int h, int w)
	{
	VRASSERT(l + h <= Height());
	VRASSERT(c + w <= Width());
	Matrix s(h,w);
	for(int i=0; i<h; i++)
		for(int j=0; j<w; j++) s[i][j] = (*this)[l+i][c+j];
	return s;
	}

Matrix Matrix::operator+() const
	{
	return( *this );
	}

Matrix Matrix::operator-()  const
	{
	Matrix	c( m , n );
	for (int i=0; i<m; i++ )
		{
		for (int j=0; j<n; j++ )
			c[i][j] = -( *this )[i][j];
		}
	return c;
	}

Matrix Matrix::operator+( const Matrix& a )  const
	{
	if ( m != a.m || n != a.n ) error( "+" );
	Matrix c( m , n );
	for (int i=0; i<m; i++)
		{
		for (int j=0; j<n; j++)
			c[i][j] = ( *this )[i][j] + a[i][j];
		}	
	return c;
	}

Matrix Matrix::operator-( const Matrix& a )  const
	{
	if ( m != a.m || n != a.n ) error( "-" );
	Matrix c( m , n );
	for ( int i = 0 ; i < m ; i++ )
		{
		for ( int j = 0 ; j < n ; j++ )
			c[i][j] = ( *this )[i][j] - a[i][j];
		}
	return( c );
	}

Matrix Matrix::operator*( const Matrix& a )  const
	{
	if ( n != a.m ) error( "*" );
	Matrix c( m , a.n );
	for ( int i = 0 ; i < m ; i++ )
		{
		for ( int j = 0 ; j < a.n ; j++ )
			{
			SCALAR	sum = 0.0;
			for ( int k = 0 ; k < n ; k++ )
				sum += ( *this )[i][k] * a[k][j];
			c[i][j] = sum;
			}
		}
	return c;
	}

Matrix operator*( SCALAR c , const Matrix& a )
	{
	return( a * c );
	}

Matrix Matrix::operator*( SCALAR c ) const
	{
	Matrix a( m , n );
	for ( int i = 0 ; i < m ; i++ )
		{
		for ( int j = 0 ; j < n ; j++ )
			a[i][j] = ( *this )[i][j] * c;
		}
	return( a );
	}

Matrix Matrix::operator/( SCALAR c ) const
	{
	if (0 <= c && c < SCALAR(1E-10)) c = SCALAR(1E-10);
	if (0 > c && c > -SCALAR(1E-10)) c = -SCALAR(1E-10);
	Matrix a( m , n );
	for ( int i = 0 ; i < m ; i++ )
		{
		for ( int j = 0 ; j < n ; j++ )
			a[i][j] = ( *this )[i][j] / c;
		}
	return( a );
	}

Matrix Matrix::operator+=( const Matrix& a )
	{
	*this = *this + a;
	return( *this );
	}

Matrix Matrix::operator-=( const Matrix& a )
	{
	*this = *this - a;
	return( *this );
	}

Matrix Matrix::operator*=( const Matrix& a )
{
	*this = *this * a;
	return( *this );
}

Matrix Matrix::operator*=( SCALAR c )
{
	*this = *this * c;
	return( *this );
}

Matrix Matrix::operator/=( SCALAR c )
{
	if (0 <= c && c < SCALAR(1E-10)) c = SCALAR(1E-10);
	if (0 > c && c > -SCALAR(1E-10)) c = -SCALAR(1E-10);
	*this = *this / c;
	return( *this );
}

//　零行列　//
void Matrix::Zero()
	{
	for(int i=0; i<m*n; i++) v[i] = rZero;
	}

//　単位行列　//
void Matrix::Unit()
	{
	if ( m != n ) error( "Unit" );
	Zero();
	for (int i=0; i<m; i++) (*this)[i][i] = SCALAR(1.0);
	}

//　転置行列　//
Matrix Matrix::Trans( void ) const
{
	Matrix a( n , m );
	for ( int i = 0 ; i < m ; i++ ){
		for ( int j = 0 ; j < n ; j ++ )
			a[j][i] = ( *this )[i][j];
	}
	return( a );
}

//　値の入れ替え　//
inline void swap( SCALAR& a , SCALAR& b )
{
	SCALAR	tmp = a;
	a = b;
	b = tmp;
}

//　ガウスの消去法　//
Matrix Matrix::Gauss( void ) const
{
	if ( n != m+1 )
		error( "gauss" );
	Matrix a = *this;
	int k;
	for (k = 0 ; k < n-2 ; k++ ){
		int mx = k;
		int i;
		for (i = k+1 ; i < n-1 ; i++ )
			if ( fabs( a[i][k] ) > fabs( a[mx][k] ) )
				mx = i;
		if ( mx != k ){
			for ( i = k ; i < n ; i++)
				swap( a[k][i] , a[mx][i] );
		}
		for( i = k+1 ; i < n-1 ; i++ ){
			SCALAR	tmp = a[i][k] / a[k][k];
			for ( int j = k+1 ; j < n ; j++ )
				a[i][j] -= tmp * a[k][j];
		}
	}
	for ( k = n-2 ; k >= 0; k-- ){
		a[k][n] /= a[k][k];
		for( int i = k-1 ; i >= 0 ; i-- )
			a[i][n] -=a[k][n] * a[i][k];
	}
	Matrix x( n-1 , 1 );
	for( k = 0 ; k < n-1 ; k++ )
		x[k][1] = a[k][n];
	return( x );
}

//　行列式　//
SCALAR Matrix::Det( void ) const
{
	if ( n != m )
		error( "det" );
	Matrix a = *this;
	int cnt = 0;
	int k;
	for (k = 0 ; k < n-1 ; k++ ){
		int mx = k;
        int i;
		for (i = k+1 ; i < n ; i++ )
			if ( fabs( a[i][k] ) > fabs( a[mx][k] ))
				mx = i;
		if ( mx != k ){
			cnt++;
			for ( i = k ; i < n ; i++ )
				swap( a[k][i] , a[mx][i] );
		}
		for ( i = k+1 ; i < n ; i++ ){
			if (0 <= a[k][k] && a[k][k] < SCALAR(1E-10)) a[k][k] = SCALAR(1E-10);
			if (0 > a[k][k] && a[k][k] > -SCALAR(1E-10)) a[k][k] = -SCALAR(1E-10);
			SCALAR	tmp = a[i][k] / a[k][k];
			for ( int j = k+1 ; j < n ; j++ )
				a[i][j] -= tmp * a[k][j];
		}
	}
	SCALAR	det_ = 1.0;
	for ( k = 0 ; k < n ; k++ )
		det_ *= a[k][k];
	if ( cnt%2 )
		det_ = -det_;
	return ( det_ );
}

//　逆行列　//
Matrix Matrix::Inv( void ) const
{
	if ( n != m )
		error( "Inv" );
	Matrix a( n , n ) , b( n , n);
	a = *this;
	b.Unit();
    int k;
	for (k = 0 ; k < n-1 ; k++ ){
		int mx = k;
        int i;
		for (i = k+1 ; i < n ; i++ )
			if ( fabs( a[i][k] ) > fabs( a[mx][k] ))
				mx = i;
		if ( mx != k ){
			for ( i = k ; i < n ; i++ )
				swap( a[k][i] , a[mx][i] );
			for ( i = 1 ; i < n ; i++ )
				swap( b[k][i] , b[mx][i] );
		}
		if (0 <= a[k][k] && a[k][k] < SCALAR(1E-10)) a[k][k] = SCALAR(1E-10);
		if (0 > a[k][k] && a[k][k] > -SCALAR(1E-10)) a[k][k] = -SCALAR(1E-10);
		for ( i = k+1 ; i < n ; i++ ){
			SCALAR	tmp = a[i][k] / a[k][k];
			for ( int j = 1 ; j < n ; j++ ){
				a[i][j] -= tmp * a[k][j];
				b[i][j] -= tmp * b[k][j];
			}
		}
	}
	for ( k = n-1 ; k >= 0 ; k-- ){
		if (0 <= a[k][k] && a[k][k] < SCALAR(1E-10)) a[k][k] = SCALAR(1E-10);
		if (0 > a[k][k] && a[k][k] > -SCALAR(1E-10)) a[k][k] = -SCALAR(1E-10);
        int i;
		for ( i = 0 ; i < n ; i ++ )
			b[k][i] /= a[k][k];
		for ( i = k-1 ; i >= 0 ; i-- ){
			for ( int j = 0 ; j < n ; j++ )
			b[i][j] -= b[k][j] * a[i][k];
		}
	}
	return( b );
}

//	疑似逆行列
Matrix Matrix::PInv() const
	{
	if ( n == m ) return Inv();
	if (m<n) return Trans() * ((*this) * Trans()).Inv();
	else return (Trans() * (*this)).Inv() * Trans();
	}
void Matrix::Load(std::istream& istr)
	{
	int h=0,w=0;
	char ch;
	istr >> ch;
	VRASSERT(ch == '(');
	istr >> h;
	istr >> ch;
	VRASSERT(ch == ',');
	istr >> w;
	istr >> ch;
	VRASSERT(ch == ')');
	Size(h,w);
	for(int i=0; i<Height(); i++)
		for(int j=0; j<Width(); j++)
			istr >> (*this)[i][j];
	}
void Matrix::Save(std::ostream& ostr) const
	{
	ostr << "(" << Height() << "," << Width() << ")" << "\n";
	for(int i=0; i<Height(); i++)
		{
		for(int j=0; j<Width(); j++)
			{
			ostr.width(8);
			ostr << (*this)[i][j] << " ";
			}
		ostr << "\n";
		}
	}

std::ostream& operator << (std::ostream& ostr, const Matrix& mat)
	{
	ostr << "(" << mat.Height() << "," << mat.Width() << ")" << "\n";
	for(int i=0; i<mat.Height(); i++)
		{
		for(int j=0; j<mat.Width(); j++)
			{
			ostr.width(5);
			ostr.precision(2);
			ostr << mat[i][j] << " ";
			}
		ostr << "\n";
		}
	return ostr;
	}
std::istream& operator >> (std::istream& istr, Matrix& m)
	{
	m.Load(istr);
	return istr;
 	}
}	//	namespace LVRM
