#include "qp_prog01.h"
#include <string.h>

#ifdef _DEBUG
#include <stdio.h>
#define PRINT_STATUS() printStatus()
#define PRINT_INIT() printInit()
#define PRINT_XY() printXY()
#define PRINT_XYD() printXYD()
static void printStatus();
static void printInit();
static void printXY();
static void printXYD();
#else
#define PRINT_STATUS()
#define PRINT_INIT()
#define PRINT_XY()
#define PRINT_XYD()
#endif //_DEBUG

float gVecX[N];
float gMatQ[N][N];
float gVecC[N];

static float gMatA[N][N];
static float minX;
static float maxX;

static float gMatR[N][N];
static float gVecL[N];

static float gVecXYNext[2*N];
static float gVecD[N];

static int isActiveSet[N];

static int ChkOptimalSol();

// ���������� �{�b�N�X�������̐����Ȃ�
void InitQuadpr(float minTin, float maxTin){
	int i,j;
	minX = -minTin;
	maxX = maxTin;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			gMatA[i][j] = 0;
		}
		gVecX[i] = minTin;
		isActiveSet[i] = -1;
		gMatA[i][i] = 1;
	}
}

//
// �e�X�e�b�v�ŋ��߂�A���P���������̌W���s��𐶐�����
static void MakeCalcMat(){
	int i,j;
	for(i=0;i<N;i++){
		gVecL[i] = gVecC[i];
		for(j=0;j<N;j++){
			if(isActiveSet[j] > 0){
				gVecL[i] -= gMatQ[i][j] * maxX;
				gMatR[i][j] = gMatA[i][j];
			}
			else if(isActiveSet[j] < 0){
				gVecL[i] += gMatQ[i][j] * minX;
				gMatR[i][j] = -gMatA[i][j];
			}
			else{
				gMatR[i][j] = gMatQ[i][j];
			}
		}
	}
}

//
//�@N�����A���P��������������
static void CalcMatRXL(float a[][N], float xout[], float b[]){
	int	i,j,k;
	float p,q,s;
	float x[N];
#ifdef _DEBUG
	printf("Solve a %dth-degree simultaneous equations\n",N);
#endif //_DEBUG
	// �A���P��������������
	//�i�K�E�X�̏����@ �Ȋw�Z�p�v�Z�n���h�u�b�N���j���P�̗]�n����
	for ( k=0 ; k<N-1 ; ++k )
	{
		p=a[k][k];
		for ( j=k+1 ; j<N ; ++j )
			a[k][j]/=p;
		b[k]/=p;
		for ( i=k+1 ; i<N ; ++i )
		{
			q=a[i][k];
			for ( j=k+1 ; j<N ; ++j )
				a[i][j]-=q*a[k][j];
			b[i]-=q*b[k];
		} 
	} 
	x[N-1]=b[N-1]/a[N-1][N-1];
	for ( k=N-2 ; k>=0 ; --k )
	{
		s=b[k];
		for ( j=k+1 ; j<N ; ++j )
			s-=a[k][j]*x[j];
		x[k]=s;
	}  

	// ���ʂ��i�[
	for(i=0;i<N;i++){
		if(isActiveSet[i] > 0){
			xout[i] = maxX;
			xout[N+i] = x[i];
		}
		else if(isActiveSet[i] < 0){
			xout[i] = -minX;
			xout[N+i] = x[i];
		}
		else{
			xout[i] = x[i];
			xout[N+i] = 0.0f;
		}
	}
}

//
// gVecX[] == gVecXYNext[] ���`�F�b�N
static int isVecX_VecXNext(){
	int i;
	for(i=0;i<N;i++){
		if(gVecX[i] != gVecXYNext[i]) return 0;
	}
	return 1;
}

//
// �œK���������������ǂ������`�F�b�N
static int CalcLambda(){
	//int minIndex = -1;
	int i,bval = 1;
	//float minVal = 0;
#ifdef _DEBUG
	printf("Calc Lambda\n");
	printf("delete ActiveSet{");
#endif //_DEBUG
	for(i=0;i<N;i++){
		if(isActiveSet[i]){
			if(gVecXYNext[N+i] < 0){
#ifdef _DEBUG
				printf("%2d(%d -> %d),",i,isActiveSet[i],0);
#endif //_DEBUG
				isActiveSet[i] = 0;
				bval = 0;
			}
		}
	}
#ifdef _DEBUG
	printf("}\n");
#endif //_DEBUG
	return bval;
}

//
// �T�������̃X�e�b�v�������߂�
static void CalcAlpha(){
	int i,minIndex=-1,bval;
	float val,alpha;
	float minAlpha = 1.0f;
#ifdef _DEBUG
	printf("Calc Alpha\n");
	printf("add ActiveSet{");
#endif //_DEBUG
	for(i=0;i<N;i++){
		if(!isActiveSet[i]){
			val = gVecD[i] = gVecXYNext[i] - gVecX[i];
			if(val < 0){
				alpha = -(minX + gVecX[i]) / val;
				if(alpha > 0 && minAlpha > alpha){
					minAlpha = alpha;
					minIndex = i;
					bval = -1;
				}
				else if(alpha <= 0){
#ifdef _DEBUG
					printf("%2d(%d -> %d  min:alpha(%f)),",i,isActiveSet[i],-1,alpha);
				//	getchar();
#endif //_DEBUG
					isActiveSet[i] = -1;
				}
			}
			else if(val > 0){
				alpha = (maxX - gVecX[i]) / val;
				if(alpha > 0 && minAlpha > alpha){
					minAlpha = alpha;
					minIndex = i;
					bval = 1;
				}
				else if(alpha <= 0){
#ifdef _DEBUG
					printf("%2d(%d -> %d  max:alpha(%f)),",i,isActiveSet[i],1,alpha);
					getchar();
#endif //_DEBUG
					isActiveSet[i] = 1;
				}
			}
		}
	}
	if(minIndex >= 0){
#ifdef _DEBUG
		printf("%2d(%d -> %d)}\n",minIndex,isActiveSet[minIndex],bval);
		printf("alpha = %f (isActiveSet[%d]=%d)\n",minAlpha,minIndex,bval);
#endif //_DEBUG
		PRINT_XYD();
		isActiveSet[minIndex] = bval;
		for(i=0;i<N;i++){
			if(!isActiveSet[i])
				gVecX[i] += minAlpha * gVecD[i];
		}
		PRINT_XYD();
	}
	else{
#ifdef _DEBUG
		printf("}\n");
		printf("alpha = %f\n",1.0f);
#endif //_DEBUG
		PRINT_XYD();
		for(i=0;i<N;i++){
			gVecX[i] = gVecXYNext[i];
		}
	}
}

//
// �񎟌v��@���C���֐�
// �����T���_��O��̍œK���ɂ���
int quadpr(){
	int i=1;
	while(1){
		MakeCalcMat(); // �W���s��̐���
		PRINT_STATUS();
		CalcMatRXL(gMatR,gVecXYNext,gVecL); //�A���P��������������
		PRINT_XY();
		if(isVecX_VecXNext()){
			if(CalcLambda()) return i; //�œK���I�����������߂�
			PRINT_XY();
		}
		else{
			CalcAlpha(); // �X�e�b�v�������߂�
		}
#ifdef _DEBUG
		printf("in process (%d times end)",i);
		//getchar();
#endif //_DEBUG
		i++; //�A������������������
	}
}

//
//�@����Ώ� N�����A���P��������������
static void CaclMatQXC(float a[][N], float x[], float b[]){
	int	i,j,k;
	float q,s;
	for ( k=0 ; k<N-1 ; ++k )
	{
		for ( i=k+1 ; i<N ; ++i )
		{
			q=a[i][k]/a[k][k];
			for ( j=k+1 ; j<=i ; ++j )
				a[i][j]-=q*a[j][k];
			b[i]-=q*b[k];
		} 
	} 
	x[N-1]=b[N-1]/a[N-1][N-1];
	for ( k=N-2 ; k>=0 ; --k )
	{
		s=b[k];
		for ( j=k+1 ; j<N ; ++j )
			s-=a[j][k]*x[j];
		x[k]=s/a[k][k];
	}  
}

static int ChkOptimalSol(){
	int i;
	int bVal=1;
	for(i=0;i<N;i++){
		if(gVecX[i] > maxX){
			gVecX[i] = maxX;
			isActiveSet[i] = 1;
			bVal = 0;
		}
		else if(gVecX[i] < -minX){
			gVecX[i] = -minX;
			isActiveSet[i] = -1;
			bVal = 0;
		}
		else{
			isActiveSet[i] = 0;
		}
	}
	return bVal;
}


//
// �񎟌v��@���C���֐�
// �����T���_�𖳏����œK���̋ߖT�ɂ���
int quadpr2(){
	int i= 1;
	// �������œK�������߂�
	memcpy(gMatR,gMatQ,sizeof(float)*N*N);
	memcpy(gVecL,gVecC,sizeof(float)*N);
	CaclMatQXC(gMatR,gVecX,gVecL);
	if(ChkOptimalSol()) return i;
	i++;
	while(1){
		MakeCalcMat(); // �W���s��̐���
		CalcMatRXL(gMatR,gVecXYNext,gVecL); //�A���P��������������
		if(isVecX_VecXNext()){
			if(CalcLambda()) return i; //�œK���I�����������߂�
		}
		else{
			CalcAlpha(); // �X�e�b�v�������߂�
		}
		i++;//�A������������������
	}
}

//
// �f�o�b�N�o�͗p
#ifdef _DEBUG
static void printInit(){
	int i,j;
	printf("A X= B\n");
	for(i=0;i<N;i++){
		printf("|");
		for(j=0;j<N;j++){
			printf("%10f ",gMatA[i][j]);
		}
		printf("|\n");
	}
}

static void printXY(){
	int i;
	printf("X XNext Y /////////////////////////////////////////////////////////////////\n");
	for(i=0;i<8;i++){
		printf("%2d  X[%2d]=%10f   XNext[%2d]=%10f   |Y[%2d]:%10f|\n",
			isActiveSet[i],
			i,gVecX[i],
			i,gVecXYNext[i],
			i,gVecXYNext[N+i]);
	}
	printf("\n");
}
static void printXYD(){
	int i;
	printf("X XNext DeltaX Y /////////////////////////////////////////////////////////////////\n");
	for(i=0;i<8;i++){
		printf("%2d  X[%2d]=%10f   XNext[%2d]=%10f   DeltaX[%2d]=%10f   |Y[%2d]:%10f|\n",
			isActiveSet[i],
			i,gVecX[i],
			i,gVecXYNext[i],
			i,gVecD[i],
			i,gVecXYNext[N+i]);
	}
	printf("\n");
}

static void printStatus(){
	int i,j;
	float sum = 0;
	printf("Q X = C\n");
	for(i=0;i<N;i++){
		printf("|");
		sum = 0;
		for(j=0;j<N;j++){
			printf("%10f ",gMatQ[i][j]);
			sum+=gMatQ[i][j];
		}
		printf("| = |%10f| %10f %10f\n",gVecC[i],sum,gVecC[i]-sum);
	}
	printf("\n");
	printf("R(%dx%d) XY(%dx1) = L(%dx1)\n",N,N,N,N);
	for(i=0;i<N;i++){
		printf("|");
		for(j=0;j<N;j++){
			printf("%10f ",gMatR[i][j]);
		}
		if(isActiveSet[i]){
			printf("| |y[%2d]| = |%10f|\n",i,gVecL[i]);
		}
		else{
			printf("| |x[%2d]| = |%10f|\n",i,gVecL[i]);
		}
	}
	printf("\n");
	/*
	printf("X \n");
	for(i=0;i<N;i++){
		printf("x[%2d]:%10f  %2d Y[%2d]:%10f \n",i,gVecX[i],isActiveSet[i],i,gVecXYNext[N+i]);
	}
	printf("\n");
	*/
	//getchar();
}
#endif //_DEBUG
