/***********************************************************************/
/*                                                                     */
/*  FILE        :qp_prog02.h                                           */
/*  DATE        :2002/04/01                                            */
/*  DESCRIPTION :Quadratic Programming (Active Set Method)             */
/*                                                                     */
/*  Katsuhito AKAHANE  (kakahane@hi.pi.titech.ac.jp)                   */
/*                    modified by Shoichi HASEGAWA                     */
/*                                                                     */
/***********************************************************************/
#ifndef QUADPROGRAM_H
#define QUADPROGRAM_H
#include <Base/TMatrix.h>
using namespace PTM;
/*
�񎟌v��@
                                                          2003/02/04�@�ԉH ���m

1.�ړI
	�{�b�N�X���������(minX<=x<=maxX)�ŁA���������K�͂̓񎟌v����������v���O�����ł��B
	SPIDAR�p�ɊJ������Ă��܂����A�{�b�N�X����������Ȃ�΁A��ʂ̓񎟌v������������Ƃ��ł��܂��B
	��̓I�ɂ́A�ȉ��̒ʂ�
	
	f(x) = (1/2)x'Qx - c'x   -> minimize
	minX <= x <= maxX
	
	��L������@(Active Set Strategy Methods)��p���ĉ����Ă��܂��B
*/
/**	Quadratic Programming (C++ template)
	@param MATNN	n�sn��̍s��^
	@param VECN		n�s�̃x�N�g���^
	@param VEC2N	2n�s�̃x�N�g���^
	@param VECNI	int�^��n�s�̃x�N�g���^	*/
template <typename MATNN, typename VECN, typename VEC2N, typename VECNI>
class QuadProgramImp{
public:
	MATNN matQ;				///<	�ړI�֐���2���̌W���s��
	VECN vecC;				///<	�ړI�֐���1���̌W���x�N�g��
	VECN vecX;				///<	��
	int Dim(){ return vecX.size(); }
	typedef TYPENAME VECN::element_type T;

protected:
	MATNN matA;
	VECN minX;				
	VECN maxX;

	MATNN matR;
	VECN vecL;

	VEC2N vecXYNext;
	VECN vecD;
	VECNI isActiveSet;

public:
	QuadProgramImp(){}
	~QuadProgramImp(){}

	//
	// QP Initialize Function
	// minT <= T <= maxT
	void Init(VECN minT, VECN maxT){
		int i,j;
		minX = -minT;
		maxX = maxT;

		for(i=0; i<Dim(); i++){
			for(j=0;j<Dim();j++){
				matA[i][j] = 0;
			}
			vecX[i] = minT[i];
			isActiveSet[i] = -1;
			matA[i][i] = 1;
		}
	}

	//
	// QP Main Function
	int Solve(){
		int i = 0;
		//while(1){
		for(;i<10;i++){
			MakeCalcMat();
			CalcMatRXL(matR,vecXYNext,vecL);
			if(isVecX_VecXNext()){
				if(CalcLambda()) return i;
			}
			else{
				CalcAlpha();
			}
			i++;
		}
		return -1;
	}

protected:
	void MakeCalcMat(){
		int i,j;
		for(i=0;i<Dim();i++){
			vecL[i] = vecC[i];
			for(j=0;j<Dim();j++){
				if(isActiveSet[j] > 0){
					vecL[i] -= matQ[i][j] * maxX[i];
					matR[i][j] = matA[i][j];
				}
				else if(isActiveSet[j] < 0){
					vecL[i] += matQ[i][j] * minX[i];
					matR[i][j] = -matA[i][j];
				}
				else{
					matR[i][j] = matQ[i][j];
				}
			}
		}

	}

	void CalcMatRXL(MATNN& a, VEC2N& x, VECN& b){
		int i,j,k;
		T p,q,s;
		// �A���P��������������
		//�i�K�E�X�̏����@ �Ȋw�Z�p�v�Z�n���h�u�b�N���j���P�̗]�n����
		for ( k=0 ; k<Dim()-1 ; ++k )
		{
			p=a[k][k];
			for ( j=k+1 ; j<Dim() ; ++j )
				a[k][j]/=p;
			b[k]/=p;
			for ( i=k+1 ; i<Dim() ; ++i )
			{
				q=a[i][k];
				for ( j=k+1 ; j<Dim() ; ++j )
					a[i][j]-=q*a[k][j];
				b[i]-=q*b[k];
			} 
		} 
		x[Dim()-1]=b[Dim()-1]/a[(Dim()-1)][(Dim()-1)];
		for ( k=Dim()-2 ; k>=0 ; --k )
		{
			s=b[k];
			for ( j=k+1 ; j<Dim() ; ++j )
				s-=a[k][j]*x[j];
			x[k]=s;
		}  

		// ���ʂ��i�[
		for(i=0;i<Dim();i++){
			if(isActiveSet[i] > 0){
				x[Dim()+i] = x[i];
				x[i] = maxX[i];
			}
			else if(isActiveSet[i] < 0){
				x[Dim()+i] = x[i];
				x[i] = -minX[i];
			}
			else{
				x[Dim()+i] = 0.0f;
				//xout[i] = x[i];
			}
		}

	}

	int isVecX_VecXNext(){
		int i;
		for(i=0;i<Dim();i++){
			if(vecX[i] != vecXYNext[i]) return 0;
		}
		return 1;
	}

	int CalcLambda(){
		int i,bval = 1;
		for(i=0;i<Dim();i++){
			if(isActiveSet[i]){
				if(vecXYNext[Dim()+i] < 0){
					isActiveSet[i] = 0;
					bval = 0;
				}
			}
		}
		return bval;
	}

	void CalcAlpha(){
		int i,minIndex = -1,bval;
		T val,alpha;
		T minAlpha = 1;

		for(i=0;i<Dim();i++){
			if(!isActiveSet[i]){
				val = vecD[i] = vecXYNext[i] - vecX[i];
				if(val < 0){
					alpha = -(minX[i] + vecX[i]) / val;
					if(alpha > 0 && minAlpha > alpha){
						minAlpha = alpha;
						minIndex = i;
						bval = -1;
					}
					else if(alpha <= 0){
						isActiveSet[i] = -1;
					}
				}
				else if(val > 0){
					alpha = (maxX[i] - vecX[i]) / val;
					if(alpha > 0 && minAlpha > alpha){
						minAlpha = alpha;
						minIndex = i;
						bval = 1;
					}
					else if(alpha <= 0){
						isActiveSet[i] = 1;
					}
				}
			}
		}
		if(minIndex >= 0){
			isActiveSet[minIndex] = bval;
			for(i=0;i<Dim();i++){
				if(!isActiveSet[i]){
					vecX[i] += minAlpha * vecD[i];
				}
			}
		}
		else{
			for(i=0;i<Dim();i++){
				vecX[i] = vecXYNext[i];
			}
		}
	}
};

/**	Quadratic Programming (C++ template)
	@param T	�^��
	@param N	������						*/
template <typename T, int N>
class TQuadProgram:public QuadProgramImp< TMatrixRow<N, N, T>, TVector<N, T>, TVector<2*N, T>, TVector<N, int> >{
};
/**	Quadratic Programming (C++ template)
	@param T	�^��	*/
template <class T>
class VQuadProgram:public QuadProgramImp< VMatrixRow<T>, VVector<T>, VVector<T>, VVector<int> >{
public:
#ifdef __GNUC__
	typedef typename VQuadProgram::T ET;
#else
	typedef T ET;
#endif
	typedef QuadProgramImp< VMatrixRow<ET>, VVector<ET>, VVector<ET>, VVector<int> > base_type;
	///	������ݒ�
	void SetDim(int n){
		matQ.resize(n,n);
		vecC.resize(n);
		vecX.resize(n);
		matA.resize(n, n);
		minX.resize(n);	
		maxX.resize(n);

		matR.resize(n, n);
		vecL.resize(n);

		vecXYNext.resize(2*n);
		vecD.resize(n);
		isActiveSet.resize(n);
	}
	///	�������CminT�̎����ŁC������ݒ�
	void Init(VVector<ET> minT, VVector<ET> maxT){
		SetDim(minT.size());
		base_type::Init(minT, maxT);
	}
};


#endif //QUADPROGRAM_H
