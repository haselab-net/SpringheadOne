#ifndef PTMATRIX_TMATRIX_H
#define PTMATRIX_TMATRIX_H
/** 
	@page PTM �|�[�^�u�� �e���v���[�g �s��N���X���C�u����
	
	@author ���J�� ����

	@date 2001�N6��10��,10��6���X�V

	@section introM �͂��߂�
		���̃h�L�������g�̓|�[�^�u�� �e���v���[�g �s��N���X���C�u����
		�̃h�L�������g�ł��D
		�|�[�^�u�� �e���v���[�g �s��N���X���C�u�����́C
		�e���v���[�g�ɂ��M�~N�s��̃N���X���C�u�����ł��D
		
	@section specM ���̃��C�u�����̓���
	@subsection tmpM �e���v���[�g�ŉ���
		�S�Ă��e���v���[�g�ŐÓI�ɉ������Ă��܂��D
		�s��̃T�C�Y��|�C���^�Ȃǂ�ێ����邽�߂̊Ǘ��̈�������܂���D
		���̂��߁C
		<ul>
			<li> �s��̈ꕔ(�����s��C�x�N�g���C�s�x�N�g���C��x�N�g��)�Ȃǂ�
				���ڎQ�Ƃ��邱�Ƃ��ł���(������\)�D
			<li> �z����L���X�g���čs��Ƃ��Ďg�p���邱�Ƃ��\�D
			<li> �s��̊|���Z�ȂǂŁC�s��̃T�C�Y������Ȃ��ꍇ�C�R���p�C������
				�R���p�C�����G���[���o�͂���D
			<li> �s��̃T�C�Y�𓮓I�ɕύX���邱�Ƃ��ł��Ȃ��D
		</ul>
		�Ƃ����������������܂��D
	@subsection portM �ڐA��
		�����n�ˑ������̑����e���v���[�g�@�\�����p���Ă��Ȃ���C�S�@�\���C
		3�̃R���p�C���Ŏg�p�ł��܂��D�T�|�[�g���Ă���R���p�C���́C
		<ul>
			<li> CL (MS Visual C++ 6.0)
			<li> bcc32(Borland C++ 5.5.1)
			<li> gcc(GNU c compiler 2.95.3-5)
		</ul>
		�ł��D
	@subsection why �V���ɃN���X���C�u�������쐬�������R
		���łɑ����̍s�񃉃C�u����������Ȃ���C�V���ɍ쐬�������R�́C
		- TNT�CMTL �̍s��͊Ǘ��̈���������Ɏ����߁C�z����L���X�g����
			�s��Ƃ��Ďg�p���邱�Ƃ��ł��Ȃ��D
		- Blitz �� TinyMatrix, TinyVector �������CVisual C++ ��
			�g�p�ł��Ȃ��D
		- ���̒m�����C�����s��C�����x�N�g���ւ̎Q�Ƃ�Ԃ��s�񃉃C�u����
			�͑��݂��Ȃ��D

	����ł��D
	
	@section usage< �g����
	�|�[�^�u�� �e���v���[�g �s��N���X���C�u�����́C�w�b�_�t�@�C����������Ȃ�
	�N���X���C�u�����Ȃ̂�, TMatrix.h, TMatrixUtility.h, TVector.h
	�𓯂��t�H���_�ɓ���Ă����C.cpp�t�@�C������w�b�_���C���N���[�h���邾����
	�g�p�ł��܂��D
	@subsection sampleM �T���v��
		�ȒP�ȃT���v���ł��D�K���ȃt�@�C����(���Ƃ��� sample.cpp) �ŕۑ�����
		�R���p�C�����Ă��������D�R���p�C�����邽�߂ɂ́C
		<DL>
		<DT> visual C++ �̏ꍇ
		<DD> cl -GX sample.cpp
		<DT> gcc�̏ꍇ
		<DD> gcc sample.cpp -lstdc++
		<DT> bcc �̏ꍇ
		<DD> bcc32 sample.cpp
		</DL>
	�Ƃ��Ă��������D
	@verbatim
#include "TMatrix.h"    //  �s�񃉃C�u�����̃C���N���[�h����D
#include <iostream>
using namespace PTM;    //  �s��N���X��PTM���O��Ԃ̒��Ő錾����Ă���D
void main(){
    TMatrix<2,2,float> mat;     //  2�s2��̍s���錾
    mat[0][0] = 1;  mat[0][1] = 2;
    mat[1][0] = 3;  mat[1][1] = 4;
    TVector<2,float> vec;       //  2�����̃x�N�g����錾
    vec[0] = 1; vec[1] = 0;
    std::cout << mat;
    std::cout << vec << std::endl;
    std::cout << mat * vec << std::endl;	//	�|���Z
    std::cout << mat + mat << std::endl;	//	�����Z
    std::cout << mat - mat << std::endl;	//	�����Z
    std::cout << mat.trans() << std::endl;	//	�]�u
    std::cout << mat.inv() << std::endl;	//	�t�s��
}
@endverbatim
	@subsection vecfunc �x�N�g���̋@�\
	���̉��Z���ł��܂��D
	<ul>
	<li> +:�a, -:��, *:����/�萔�{, /:�萔����1
	<li> ==:��r, =:���
	<li> <<:�o��, >>:����
	<li> %:�O��(2�E3�����̂�)
	</ul>
	���̃����o�֐��������܂��D
	<ul>
	<li> unit(): �������������P�ʃx�N�g����Ԃ��D
	<li> norm(): �x�N�g���̑傫��(�m����)��Ԃ��D
	<li> sub_vector(): �����x�N�g���ւ̎Q�Ƃ�Ԃ��D
	</ul>
	
	@subsection matfunc �s��̋@�\
	���̉��Z���ł��܂��D
	<ul>
		<li> +:�a, -:��, *:��/�萔�{, /:�萔����1
		<li> ==:��r, =:���
		<li> <<:�o��, >>:����
	</ul>
	���̃����o�֐��������܂��D
	<ul>
		<li> det(): �s�񎮂�Ԃ��D
		<li> inv(): �t�s���Ԃ��D
		<li> gauss(): �K�E�X�̏����@�ŕ������������D
		<li> sub_matrix(): �����s��ւ̎Q�Ƃ�Ԃ��D
		<li> row(): �s�x�N�g���ւ̎Q�Ƃ�Ԃ��D
		<li> col(): ��x�N�g���ւ̎Q�Ƃ�Ԃ��D
	</ul>
		sub_matrix()��row()�ɂ͒l�������邱�Ƃ��ł��܂��D
		@verbatim
	TMatrix<3,3,float> mat; TVector<3, float> vec;
	mat.row() = vec;@endverbatim
	@section pub �Ĕz�z�̏���
	�Ĕz�z����ꍇ�́C������҂̏����E�A��������ρE�폜���Ȃ��ł��������D
	���I�ȉ�ЂŃ\�t�g�E�G�A�������l�ɂ����R�Ɏg�p�ł���悤�ɂ�����
	�̂ŁCGPL�ELGPL�ɂ��܂���ł����D
	�������GPL�ELGPL�ɉ��ς��čĔz�z���Ă��������Ă����\�ł��D
	@section support �T�|�[�g
	�o�O�C�s��C�Ӗ��s���ȃR���p�C���G���[�Ȃǂ��������ꍇ�́C
	���J�� ���� (hase@hi.pi.titech.ac.jp) �܂ł��A�����������D
	�ł������T�|�[�g���C���ǂ����C�u�����ɂ��Ă�������ł��D<br>
	���ɂ��̃��C�u�����̓e���v���[�g�N���X���C�u�����Ȃ̂ŁC�g�p���ɁC
	�����̓��삪����Ȃ��ƈӖ��̂킩��Ȃ��R���p�C���G���[�ɏo����Ƃ�
	����Ǝv���܂��D���̂悤�Ȗ��ɂ�FAQ������đΉ��������ł��̂ŁC
	�܂��͂��A�����������D
	@section thanksM �ӎ�
	LU�����C�t�s��C�K�E�X�����@�Ȃǂ̍s��v�Z�A���S���Y���́C<br>
    �u�w�b����ɂ��ŐV�A���S���Y�����T�x�S�\�[�X�R�[�h�v<br>
    ftp://ftp.matsusaka-u.ac.jp/pub/algorithms<br>
	���� ���F Haruhiko Okumura<br>
	�����ς��ė��p�����Ă��������܂����D
	���R�ɃR�[�h���g�킹�Ă��������āC���肪�Ƃ��������܂��D	*/
//-----------------------------------------------------------------------------
/**	@file TMatrix.h
	�e���v���[�g�ɂ��N�~M�s��^�̒�`.
	�v�f�̌^�ƃT�C�Y���e���v���[�g�̈����ɂ��邱�ƂŁC
	�Ǘ������������Ɏ������ɁC��ʂ̍s��������D
	�z����L���X�g���čs��ɂ��邱�Ƃ��ł���D
	�����s���s�����s���x�N�g���Ƃ��Ď��o�����Ƃ��ł���D
	sparse matrix �ɂ͑Ή����Ă��Ȃ��D										*/
//------------------------------------------------------------------------------

#include "TVector.h"

///	PTMatrix���C�u�����̖��O���
namespace PTM{

#ifdef _DEBUG
 #pragma optimize ("awgity", on)
 #pragma auto_inline(on)
 #pragma inline_recursion(on)
#endif
//----------------------------------------------------------------------------
///	�s��^�̊�{�N���X�B���ʂ͔h���N���X�� TMatrix ���g���B
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
class TMatrixBase:public BUF{
public:
	///@name �s��̃T�C�Y
	//@{
	///	�s��̍s��
	int nrow() const { return DIMDEC(dim_row); }
	///	�s��̗�
	int ncol() const { return DIMDEC(dim_col); }
	///	�s��̍s��. DIMTYPE dim_row = NROW;
	DIMDEF(NROW, dim_row);
	///	�s��̗�. DIMTYPE dim_col = NCOL;
	DIMDEF(NCOL, dim_col);
#if !defined __BORLANDC__ && !defined _MSC_VER
	static const int dim_row_col = DIMDEC(dim_row)*DIMDEC(dim_col);
#endif
	//@}

	///@name ��{�N���X�A�v�f�Ȃǂ̌^.
	//@{
	///	�����������^
	typedef BUF									buffer_type;
	///	��{�N���X�̌^
	typedef BUF									base_type;
	///	���̃N���X�̌^
	typedef TMatrixBase							this_type;
	///	�v�f�̌^
	typedef typename ITP_COL::element_type		element_type;
	///	�v�f�^��0��Ԃ��֐��I�u�W�F�N�g�̌^
	typedef typename ITP_COL::zero				zero;
	//@}
	
	///@name �C�^���[�^�A�s�E��x�N�g���ȂǃA�N�Z�X�̂��߂̌^.
	//@{
	///	�s�̃C�^���[�^�̃y�A�^
	typedef ITP_ROW								row_iterator_pair;
	///	�s�̃C�^���[�^
	typedef typename ITP_ROW::iterator			row_iterator;
	///	�s�̃C�^���[�^(const ��)
	typedef typename ITP_ROW::const_iterator	const_row_iterator;
	
	///	��̃C�^���[�^�̃y�A�^
	typedef ITP_COL								col_iterator_pair;
	///	��̃C�^���[�^
	typedef typename ITP_COL::iterator			col_iterator;
	///	��̃C�^���[�^(const ��)
	typedef typename ITP_COL::const_iterator	const_col_iterator;

	///	�s�x�N�g���̌^
	typedef TVectorBaseOp<dim_col, ITP_COL, BUF>	row_vector;
	///	��x�N�g���̌^
	typedef TVectorBaseOp<dim_row, ITP_ROW, BUF>	col_vector;

#if !defined __BORLANDC__ && !defined _MSC_VER
	typedef element_type array_type [dim_row_col];
#else
	///	�s���1�����z��ŕ\�����Ƃ��̌^
	typedef element_type array_type [DIMDEC(dim_row)*DIMDEC(dim_col)];
#endif
	///	�s���2�����z��ŕ\�����Ƃ��̌^
	typedef element_type array_2D_type[DIMDEC(dim_row)][DIMDEC(dim_col)];
	//@}

private:
	///	��Βl��Ԃ��֐�.
	template <class element_type> element_type abs(const element_type& t){
		return t < zero() ? -t : t;
	}

public:

	///@name �v�f�̃A�N�Z�X
	//@{
	///	�s���[0][0]�v�f�ւ̃|�C���^
	element_type*		begin()					{ return base_type::begin(); }
	///	�s���[0][0]�v�f�ւ̃|�C���^(const��)
	const element_type*	begin() const			{ return base_type::begin(); }
	///	�s�����֐i�ށA[0][0]�v�f���w���A�C�^���[�^��Ԃ��B
	row_iterator		row_begin()				{ return row_iterator(begin()); }
	///	�s�����֐i�ށA[0][0]�v�f���w���A�C�^���[�^��Ԃ��B(const��)
	const_row_iterator	row_begin() const		{ return const_row_iterator(begin()); }
	///	�s�����֐i�ށA[0][c]�v�f���w���A�C�^���[�^��Ԃ��B
	row_iterator		row_begin(int c)		{ return row_iterator(col_begin()+c); }
	///	�s�����֐i�ށA[0][c]�v�f���w���A�C�^���[�^��Ԃ��B(const��)
	const_row_iterator	row_begin(int c) const	{ return const_row_iterator(col_begin()+c); }

	///	������֐i�ށA[0][0]�v�f���w���A�C�^���[�^��Ԃ��B
	col_iterator		col_begin()				{ return col_iterator(begin()); }
	///	������֐i�ށA[0][0]�v�f���w���A�C�^���[�^��Ԃ��B(const��)
	const_col_iterator	col_begin() const		{ return const_col_iterator(begin()); }
	///	������֐i�ށA[r][0]�v�f���w���A�C�^���[�^��Ԃ��B
	col_iterator		col_begin(int r)		{ return col_iterator(row_begin()+r); }
	///	������֐i�ށA[r][0]�v�f���w���A�C�^���[�^��Ԃ��B(const��)
	const_col_iterator	col_begin(int r) const	{ return const_col_iterator(row_begin()+r); }

	///	�s�����֐i�ށA[�s��][c]�v�f���w���A�C�^���[�^��Ԃ��B
	row_iterator		row_end(int c=0)		{ return row_begin(c)+nrow();}
	///	�s�����֐i�ށA[�s��][c]�v�f���w���A�C�^���[�^��Ԃ��B(const��)
	const_row_iterator	row_end(int c=0) const	{ return row_begin(c)+nrow(); }

	///	������֐i�ށA[r][��]�v�f���w���A�C�^���[�^��Ԃ��B
	col_iterator		col_end(int n=0)		{ return col_begin(n)+ncol();}
	///	������֐i�ށA[r][��]�v�f���w���A�C�^���[�^��Ԃ��B(const��)
	const_col_iterator	col_end(int n=0) const	{ return col_begin(n)+ncol(); }

	///	n�sm��v�f([n][m]�v�f)�ւ̎Q�Ƃ�Ԃ��B
	element_type& item(int n, int m)			{ return col_begin(n)[m]; }
	///	n�sm��v�f([n][m]�v�f)�ւ̎Q�Ƃ�Ԃ��B(const ��)
	const element_type& item(int n, int m) const{ return col_begin(n)[m]; }
	
	///	r�s�x�N�g��([r])�ւ̎Q�Ƃ�Ԃ��B
	row_vector&			row(int r)				{ return *(row_vector*) (element_type*) col_begin(r); }
	///	r�s�x�N�g��([r])�ւ̎Q�Ƃ�Ԃ��B(const��)
	const row_vector&	row(int r) const		{ return *(const row_vector*) (const element_type*) col_begin(r); }
	///	r�s�x�N�g��([r])�ւ̎Q�Ƃ�Ԃ��B
	row_vector&			operator [](int r)		{ return row(r); }
	///	r�s�x�N�g��([r])�ւ̎Q�Ƃ�Ԃ��B(const��)
	const row_vector&	operator [](int r) const{ return row(r); }

	///	c��x�N�g��([][c])�ւ̎Q�Ƃ�Ԃ��B
	col_vector&			col(int c)				{ return *(col_vector*) (element_type*) row_begin(c); }
	///	c��x�N�g��([][c])�ւ̎Q�Ƃ�Ԃ��B(const��)
	const col_vector&	col(int c) const		{ return *(const col_vector*) (const element_type*) row_begin(c); }

	///	�s���1�����z��ŕԂ�
	operator array_type&() {return *(array_type*)begin();}
	///	�s���1�����z��ŕԂ�
	operator const array_type&() const {return *(array_type*)begin();}

	///	�s���2�����z��ŕԂ�
	operator const array_2D_type&() const {return *(array_2D_type*)begin();}
	///	�s���2�����z��ŕԂ�
	operator array_2D_type&() {return *(array_2D_type*)begin();}
	//@}

	///	@name ��{����
	//@{
	///	�[���N���A
	void clear(){
		for(int r = 0; r < nrow(); ++r) row(r).clear(zero());
	}
	///	����l�ŃN���A
	template <class M_ITP_ROW>
	void clear(typename M_ITP_ROW::elemet_type t){
		for(int r = 0; r < nrow(); ++r) row(r).clear(t);
	}
	///	���
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	void assign(const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		for(int r = 0; r < nrow(); ++r) row(r).assign(b.row(r));
	}
	//@}
	///	@name �v�Z
	//@{
	///	���Z
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	void add(const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		for(int r = 0; r < nrow(); ++r) row(r).add(b.row(r));
	}
	///	���Z
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	void sub(const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		for(int r = 0; r < nrow(); ++r) row(r).sub(b.row(r));
	}
	///	LU�������s���Bthis������������B�s�񎮂�Ԃ��B
	element_type lu(int (&ip)[DIMDEC(dim_row)]){
		assert(nrow() == ncol());
		this_type& a = *this;
		int i, j, k, ii, ik;
		int n = nrow();
		element_type t, u, det;
		TVector<DIMDEC(dim_row), element_type> weight;
		
		det = 0;                   // �s��
		for (k = 0; k < n; k++) {  // �e�s�ɂ���
			ip[k] = k;             // �s�������̏����l
			u = 0;                 // ���̍s�̐�Βl�ő�̗v�f�����߂�
			for (j = 0; j < n; j++) {
				t = abs(a[k][j]);  if (t > u) u = t;
			}
			if (u == 0) goto EXIT; // 0 �Ȃ�s���LU�����ł��Ȃ�
			weight[k] = 1 / u;     // �ő��Βl�̋t��
		}
		det = 1;                   // �s�񎮂̏����l
		for (k = 0; k < n; k++) {  // �e�s�ɂ���
			u = -1;
			for (i = k; i < n; i++) {  // ��艺�̊e�s�ɂ���
				ii = ip[i];            // �d�݁~��Βl ���ő�̍s��������
				t = abs(a[ii][k]) * weight[ii];
				if (t > u) {  u = t;  j = i;  }
			}
			ik = ip[j];
			if (j != k) {
				ip[j] = ip[k];  ip[k] = ik;  // �s�ԍ�������
				det = -det;  // �s����������΍s�񎮂̕������ς��
			}
			u = a[ik][k];  det *= u;  // �Ίp����
			if (u == 0) goto EXIT;    // 0 �Ȃ�s���LU�����ł��Ȃ�
			for (i = k + 1; i < n; i++) {  // Gauss�����@
				ii = ip[i];
				t = (a[ii][k] /= u);
				for (j = k + 1; j < n; j++)
					a[ii][j] -= t * a[ik][j];
			}
		}
EXIT:
		return det;           // �߂�l�͍s��
	}
	///	�s�񎮂�Ԃ��D
	element_type det(){
		return calc_matrix_det(*this);
	}
	///	(*this) x + b = 0 ��1��������������
	template <class ITPX, class BUFX, class ITPB, class BUFB>
	void solve(TVectorBase<dim_row, ITPX, BUFX>& x, const TVectorBase<dim_row, ITPB, BUFB>& b, int (&ip)[DIMDEC(dim_row)]){
		this_type& a = *this;
		int i, j, ii;
		element_type t;
		const int n = nrow();
		for (i = 0; i < n; i++) {       // Gauss�����@�̎c��
			ii = ip[i];  t = b[ii];
			for (j = 0; j < i; j++) t -= a[ii][j] * x[j];
			x[i] = t;
		}
		for (i = n - 1; i >= 0; i--) {  // ��ޑ��
			t = x[i];  ii = ip[i];
			for (j = i + 1; j < n; j++) t -= a[ii][j] * x[j];
			x[i] = t / a[ii][i];
		}
	}
	///	�K�E�X�̏����@
	template <class ITPX, class BUFX, class ITPB, class BUFB>
	element_type gauss(TVectorBase<dim_row, ITPX, BUFX>& x, const TVectorBase<dim_row, ITPB, BUFB>& b){
		this_type& a = *this;
		element_type det;				// �s��
		int ip[DIMDEC(dim_row)];		// �s�����̏��

		det = lu(ip);					// LU����
		if (det != 0) solve(x, b, ip);	// LU�����̌��ʂ��g���ĘA��������������
		return det;						// �߂�l�͍s��
	}
	///	�t�s������߂�B
	template <class ITP_ROWI, class ITP_COLI, class BUFI>
	element_type inv(TMatrixBase<dim_row, dim_col, ITP_ROWI, ITP_COLI, BUFI>& a_inv){
		int i, j, k, ii;
		int n = nrow();
		element_type t, det;
		int ip[DIMDEC(dim_row)];   // �s�����̏��
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
	///	�\��
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
	///	����
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
	
/**	TMatrix�^�h���N���X�ɕK�v�ȃ����o�̒�`.
	�h���N���X����邽�тɁC���̃}�N�����g���ă����o�����.
	@param	THIS	�V���ɐ錾����h���N���X�̌^��.
	@see	TMatrix	*/
#define DEF_TMATRIX_BASIC_MEMBER(THIS)										\
    typedef THIS this_type;             /*  ���̃N���X�̌^���`���Ă���.*/	\
	typedef base_type::buffer_type buffer_type;								\
										/*	�s��̃������^.	*/				\
    DIMDEF(base_type::dim_row, dim_row);/*  �s��̍s��.	*/					\
    DIMDEF(base_type::dim_col, dim_col);/*  �s��̗�.	*/					\
	typedef base_type::row_iterator_pair row_iterator_pair;					\
                                        /*  �s�C�^���[�^�̃y�A	*/			\
    typedef base_type::row_iterator row_iterator;							\
                                        /*  �s�C�^���[�^	*/				\
    typedef base_type::const_row_iterator const_row_iterator;				\
                                        /*  �s�C�^���[�^(const��)	*/		\
	typedef base_type::col_iterator_pair col_iterator_pair;					\
                                        /*  ��C�^���[�^�̃y�A		*/		\
    typedef base_type::col_iterator col_iterator;							\
                                        /*  ��C�^���[�^	*/				\
    typedef base_type::const_col_iterator const_col_iterator;				\
                                        /*  ��C�^���[�^(const��)	*/		\
    typedef base_type::element_type element_type;							\
                                        /*  �v�f�^.	*/						\
    typedef base_type::element_type const_element_type;						\
                                        /*  �v�f�^(const��).	*/			\
    typedef base_type::zero zero;											\
                                        /*  �[����Ԃ��֐��I�u�W�F�N�g.	*/	\
	/*	TMatrixBase�I�u�W�F�N�g�ɂ�鏉����	*/								\
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>				\
	THIS(const PTM::TMatrixBaseOp<dim_row, dim_col,							\
		M_ITP_ROW, M_ITP_COL, M_BUF>& b){ assign(b); }						\
	/*	������Z�q. �T�C�Y���������s�� b ��������.	*/					\
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>				\
	this_type& operator =(const PTM::TMatrixBase<dim_row, dim_col,			\
		M_ITP_ROW, M_ITP_COL, M_BUF>& b){ assign(b); return *this; }		\
	/*	<< ���Z�q�ɂ�鏉����	*/											\
	template <class ROWINIT, int M_R>										\
	THIS(const PTM::TVectorInitializer<ROWINIT, M_R>& v){					\
		for(int r=0; r<nrow(); ++r){										\
			row(r) = *(PTM::TVector<DIMDEC(ROWINIT::dim),					\
				TYPENAME ROWINIT::element_type> *)v.value[r].value;			\
		}																	\
	}																		\
	/*	<< ���Z�q�ɂ�鏉�����̂��߂� = ���Z�q.	*/							\
	template <class ROWINIT, int M_R>										\
	this_type& operator =(const PTM::TVectorInitializer<ROWINIT, M_R>& v){	\
		for(int r=0; r<nrow(); ++r){										\
			row(r) = *(PTM::TVector<DIMDEC(ROWINIT::dim),					\
				TYPENAME ROWINIT::element_type> *)v.value[r].value;			\
		}																	\
		return *this;														\
	}																		\

//----------------------------------------------------------------------------
///	���Z�q���̍s��
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
class TMatrixBaseOp: public TMatrixBase <NROW,NCOL,ITP_ROW,ITP_COL,BUF>{
public:
	///	��{�N���X�^�̒�`.
	typedef TMatrixBase<NROW,NCOL,ITP_ROW,ITP_COL,BUF>	base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see	::DEF_TMATRIX_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrixBaseOp);
	///	�f�t�H���g�R���X�g���N�^.
	TMatrixBaseOp(){}
	///	�]�u�s��.
	TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>& trans(){
		return *(TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>*)this;
	}
	///	�]�u�s��(const ��).
	const TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>& trans() const {
		return *(TMatrixBaseOp<dim_col, dim_row, col_iterator_pair, row_iterator_pair, buffer_type>*)this;
	}
	///	�t�s��̌v�Z.
	template <class ITP_ROWI, class ITP_COLI, class BUFI>
	element_type inv(TMatrixBase<dim_row, dim_col, ITP_ROWI, ITP_COLI, BUFI>& a_inv){ return base_type::inv(a_inv); }
	///	�t�s���Ԃ�.
	this_type inv() const {
		this_type a(*this), a_inv;
		a.inv(a_inv);
		return a_inv;
	}
	/**	�����s���Ԃ�.
		���̍s���(0,0)�v�f���� (S::nrow()-1, S::ncol()-1)�v�f�܂ł�
		�����s��ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����s��̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim_row ���s��. S::dim_col���񐔂�\��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̍s��ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(S){
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (element_type*)(col_begin());
	}
	/**	�����s���Ԃ�.
		���̍s���(r,c)�v�f���� (r+S::nrow()-1, c+S::ncol()-1)�v�f�܂ł�
		�����s��ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����s��̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim_row ���s��. S::dim_col���񐔂�\��.
		@param	r	�����s���0�s�ڂ��C���̍s��̉��s�ڂɓ�����̂����w��.
		@param	c	�����s���0��ڂ��C���̍s��̉���ڂɓ�����̂����w��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̍s��ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(int r, int c, S){
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (element_type*)(col_begin(r)+c);
	}
	/**	�����s���Ԃ�(const ��).
		���̍s���(0,0)�v�f���� (S::nrow()-1, S::ncol()-1)�v�f�܂ł�
		�����s��ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����s��̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim_row ���s��. S::dim_col���񐔂�\��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̍s��ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	const TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(S) const {
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (const element_type*)(col_begin());
	}
	/**	�����s���Ԃ�.(const ��)
		���̍s���(r,c)�v�f���� (r+S::nrow()-1, c+S::ncol()-1)�v�f�܂ł�
		�����s��ւ̎Q�Ƃ�Ԃ��D
		@param	S	�����s��̃T�C�Y���w�肷�邽�߂̃I�u�W�F�N�g.
					S::dim_row ���s��. S::dim_col���񐔂�\��.
		@param	r	�����s���0�s�ڂ��C���̍s��̉��s�ڂɓ�����̂����w��.
		@param	c	�����s���0��ڂ��C���̍s��̉���ڂɓ�����̂����w��.
		@return	S�Ŏw�肳�ꂽ�T�C�Y�̍s��ւ̎Q�Ƃ�Ԃ�.	*/
	template <class S>
	const TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>& sub_matrix(int r, int c, S) const {
		return *(TMatrixBaseOp<S::dim_row, S::dim_col, row_iterator_pair, col_iterator_pair, buffer_type>*) (element_type*)(col_begin(r)+c);
	}
	/**	+=���Z�q(*this = *this + b).
		@param	b	�������������s��	*/
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	this_type operator+= (const TMatrixBaseOp<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		add(b); return *this; 
	}
	/**	-=���Z�q(*this = *this - b).
		@param	b	�������������s��	*/
	template <class M_ITP_ROW, class M_ITP_COL, class M_BUF>
	this_type operator-= (const TMatrixBase<dim_row, dim_col, M_ITP_ROW, M_ITP_COL, M_BUF>& b){
		sub(b); return *this;
	}
	/**	- ���Z�q (return -*this).	*/
	this_type operator- () { this_type r(*this); r*=-1; return r; }
	/**	*=���Z�q(*this = b * *this).
		@param	b	�v�f�^	*/
	this_type operator*= (element_type b){
		for(int i=0; i<nrow(); ++i) row(i) *= b; return *this;
	}
	/**	/=���Z�q(*this = *this / b).
		@param	b	�v�f�^	*/
	this_type operator/= (element_type b){
		for(int i=0; i<nrow(); ++i) row(i) /= b; return *this;
	}
};
//----------------------------------------------------------------------------
/**	�s��^. �񂪗׍����C�s�x�N�g�������ȃx�N�g���Ƃ��ĂɎ��o����s��.
	TMatrix<�s��, ��, �v�f�̌^> ���s��^�ɂȂ�B	*/
template <int R, int C, class T, class ZERO=T>
class TMatrix:public TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(C),T,ZERO>,
	TVectorIteratorPair<DIMENC(1),T,ZERO>, TVectorBuffer<R*C,T> >{
public:
	/**	��{�N���X�^�̒�`	*/
	typedef TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(C),T,ZERO>, 
		TVectorIteratorPair<DIMENC(1),T,ZERO>, TVectorBuffer<R*C,T> > base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see	::DEF_TMATRIX_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrix);
	///	�f�t�H���g�R���X�g���N�^.
	TMatrix(){}
};
/**	�s��^. �s���׍����C��x�N�g�������ȃx�N�g���Ƃ��ĂɎ��o����s��.
	TMatrix<�s��, ��, �v�f�̌^> ���s��^�ɂȂ�B	*/
template <int R, int C, class T, class ZERO=T>
class TMatrixCol:public TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(1),T,ZERO>,
	TVectorIteratorPair<DIMENC(R),T,ZERO>, TVectorBuffer<R*C,T> >{
public:
	/**	��{�N���X�^�̒�`	*/
	typedef TMatrixBaseOp<DIMENC(R), DIMENC(C), TVectorIteratorPair<DIMENC(1),T,ZERO>,
		TVectorIteratorPair<DIMENC(R),T,ZERO>, TVectorBuffer<R*C,T> > base_type;
	/**	�p������Ȃ���{�I�ȃ����o�̒�`.
		@see	::DEF_TMATRIX_BASIC_MEMBER	*/
	DEF_TMATRIX_BASIC_MEMBER(TMatrixCol);
	///	�f�t�H���g�R���X�g���N�^.
	TMatrixCol(){}
};

//----------------------------------------------------------------------------
/**	�����s��^�쐬�̂��߂̃��[�e�B���e�B�[�N���X.
	TSubMatrix<�s��^, �s��, ��>::type �ŕ����s��^���擾�ł���B	*/
template <class MAT, int R, int C>
class TSubMatrix:public TMatrixBaseOp<DIMENC(R), DIMENC(C), typename MAT::row_iterator_pair, typename MAT::col_iterator_pair, typename MAT::buffer_type>{
public:
	typedef TMatrixBaseOp<DIMENC(R), DIMENC(C), typename MAT::row_iterator_pair, typename MAT::col_iterator_pair, typename MAT::buffer_type> type;
};
/**	�����s��^�쐬�̂��߂̃��[�e�B���e�B�[�N���X.
	TMatDim<�s��, ��> �ŕ����s��^���擾�ł���B	*/
template <int R, int C>
class TMatDim{
public:
	DIMDEF(DIMENC(R), dim_row);
	DIMDEF(DIMENC(C), dim_col);
};


//----------------------------------------------------------------------------
//	�O���֐��̒�`

/**	��r(return a == b).
	@param	a	�s��^
	@param	b	�v�f�^a�Ɠ����������̃x�N�g���^
	@return		bool	*/
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
bool operator == (
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b) {
	return a.equal(b);
}
/**	��r(return a != b).
	@param	a	�s��^
	@param	b	�v�f�^a�Ɠ����������̃x�N�g���^
	@return		bool	*/
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
bool operator != (
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
				 const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b) {
	return !a.equal(b);
}

///	�s��̑����Z
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator + (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	r += b;
	return r;
}
///	�s��̈����Z
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	class ITP_ROWB, class ITP_COLB, class BUFB>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator - (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROWB, ITP_COLB, BUFB>& b){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	r -= b;
	return r;
}
///	�s��̊|���Z
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
///	�s��ƃx�N�g���̊|���Z
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
///	�s��̊|���Z
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF,
	DIMTYPE NCOLB, class ITP_ROWB, class ITP_COLB, class BUFB>
	TMatrixBaseOp<NROW, NCOLB, ITP_ROW, ITP_COL, BUF> operator * (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TMatrixBaseOp<NCOL, NCOLB, ITP_ROWB, ITP_COLB, BUFB>& b){
	return multi(a, b);
}
///	�s��ƃx�N�g���̊|���Z
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF, class ITP_V, class BUFV>
TVector<DIMDEC(NROW), typename ITP_COL::element_type> operator * (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	const TVectorBaseOp<NCOL, ITP_V, BUFV>& b){
	return multi(a, b);
}
///	�x�N�g���ƍs��̊|���Z
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF, class ITP_V, class BUFV>
TVector<DIMDEC(NCOL), typename ITP_COL::element_type> operator * (
	const TVectorBaseOp<NROW, ITP_V, BUFV>& a,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& b){
	return multi(b.trans(), a);
}
///	�s��̒萔�{
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator * (
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a,
	TYPENAME ITP_COL::element_type b){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	for(int i=0; i<r.nrow(); ++i) r.row(i) *= b;
	return r;
}
///	�s��̒萔�{
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> operator * (
	TYPENAME ITP_COL::element_type b,
	const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& a){
	TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF> r(a);
	for(int i=0; i<r.nrow(); ++i) r.row(i) *= b;
	return r;
}
///	�\��
template <DIMTYPE NROW, DIMTYPE NCOL, class ITP_ROW, class ITP_COL, class BUF>
std::ostream& operator << (std::ostream& os, const TMatrixBaseOp<NROW, NCOL, ITP_ROW, ITP_COL, BUF>& m){
	m.print(os);
	return os;
}
///	����
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
