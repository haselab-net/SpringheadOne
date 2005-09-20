// SH4Data.cpp: CSH4Data クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SH4Data.hpp"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
const char CSH4Data::s8_key[CSH4DATA_S8_SIZE][CSH4DATA_CHAR_SIZE]={
	"AD",
	"DA",
	"ENC"
};
const char CSH4Data::f8_key[CSH4DATA_F8_SIZE][CSH4DATA_CHAR_SIZE]={
	"AMP",
	"DAMP",
	"VOLT"
};
const char CSH4Data::f4_key[CSH4DATA_F4_SIZE][CSH4DATA_CHAR_SIZE]={
	"POS_ENC",
	"ORI_ENC",
	"VEL_ENC",
	"ORI_VEL_ENC",
	"ACC_ENC",
	"ORI_ACC_ENC",

	"POS_AD",
	"ORI_AD",
	"VEL_AD",
	"ORI_VEL_AD",
	"ACC_AD",
	"ORI_ACC_AD",
	
	"POS",
	"ORI",
	"VEL",
	"ORI_VEL",
	"ACC",
	"ORI_ACC",
	
	"FORCE",
	"MOMENT",
	"DISP_FORCE",
	"DISP_MOMENT",
	"IMPULSE"
};

CSH4Data::CSH4Data()
{

}

CSH4Data::~CSH4Data()
{

}

#ifdef _DEBUG
void CSH4Data::testVector()
{
	int i,j,k;
	for(i=CSH4DATA_S8_SIZE-1;i>=0;i--){
		for(j=CSH4DATA_BUFFER_SIZE-1;j>=0;j--){
			for(k=0;k<=8;k++){
				s8_data[i][k][j] = rand();
			}
		}
	}
	for(i=CSH4DATA_F8_SIZE-1;i>=0;i--){
		for(j=CSH4DATA_BUFFER_SIZE-1;j>=0;j--){
			for(k=0;k<=8;k++){
				f8_data[i][k][j] = (float)rand();
			}
		}
	}
	for(i=CSH4DATA_F4_SIZE-1;i>=0;i--){
		for(j=CSH4DATA_BUFFER_SIZE-1;j>=0;j--){
			for(k=0;k<=4;k++){
				f4_data[i][k][j] = (float)rand();
			}
		}
	}

}
#endif //#ifdef _DEBUG
