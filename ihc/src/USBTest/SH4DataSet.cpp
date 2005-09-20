// SH4DataSet.cpp: CSH4DataSet クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SH4DataSet.hpp"

#include <stdio.h>
#ifndef WIN23
//#include "IHCPrinter.h"
#include <cyg/libc/stdio/stdio.h>
#endif

#include <string.h>

//#define BIG_TO_LITTLE
#ifdef BIG_TO_LITTLE
void BIG_TO_LITTLE_S(short* val){CSH4DataSet::bigToLittles(val);}
void BIG_TO_LITTLE_F(float* val){CSH4DataSet::bigToLittlef(val);}
#else
void BIG_TO_LITTLE_S(short* val)  {}
void BIG_TO_LITTLE_F(float* val)  {}
#endif //BIG_TO_LITTLE

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CSH4DataSet::CSH4DataSet()
{
	Reset();
}

CSH4DataSet::~CSH4DataSet()
{

}

void CSH4DataSet::SetAnalysisWords(char *words)
{
	int i;
	int sid,eid,size;
	Reset();
	printf("--------------------------------------------------------------\n\r");
	while(words[0] != '\0'){
		for(i=0;i<CSH4DATA_S8_SIZE;i++){
			if(strncmp(words,CSH4Data::s8_key[i],strlen(CSH4Data::s8_key[i])) == 0){
				words+=strlen(CSH4Data::s8_key[i]);
				sscanf(words,"[%d:%d][%d]",&sid,&eid,&size);
				s8_sid[i] = sid;
				s8_eid[i] = eid;
				s8_size[i] = size;
				// DEBUG
				printf("%12s %3d:%3d[%3d] %3d %3d |%3d\n\r",
					CSH4Data::s8_key[i],
					s8_sid[i],s8_eid[i],s8_eid[i] - s8_sid[i]+1,
					s8_size[i],
					sizeof(short),
					(s8_eid[i] - s8_sid[i]+1)*s8_size[i]*sizeof(short)
					);
				words = strchr(words,']')+1;
				words = strchr(words,']')+1;
				while(words[0] == ' ') words++;
				if(words[0] == '\0') return;
			}
		}
		for(i=0;i<CSH4DATA_F8_SIZE;i++){
			if(strncmp(words,CSH4Data::f8_key[i],strlen(CSH4Data::f8_key[i])) == 0){
				words+=strlen(CSH4Data::f8_key[i]);
				sscanf(words,"[%d:%d][%d]",&sid,&eid,&size);
				f8_sid[i] = sid;
				f8_eid[i] = eid;
				f8_size[i] = size;
				// DEBUG
				printf("%12s %3d:%3d[%3d] %3d %3d |%3d\n\r",
					CSH4Data::f8_key[i],
					f8_sid[i],f8_eid[i],f8_eid[i] - f8_sid[i]+1,
					f8_size[i],
					sizeof(float),
					(f8_eid[i] - f8_sid[i]+1)*f8_size[i]*sizeof(float));
				words = strchr(words,']')+1;
				words = strchr(words,']')+1;
				while(words[0] == ' ') words++;
				if(words[0] == '\0') return;
			}
		}
		for(i=0;i<CSH4DATA_F4_SIZE;i++){
			if(strncmp(words,CSH4Data::f4_key[i],strlen(CSH4Data::f4_key[i])) == 0){
				words+=strlen(CSH4Data::f4_key[i]);
				sscanf(words,"[%d:%d][%d]",&sid,&eid,&size);
				f4_sid[i] = sid;
				f4_eid[i] = eid;
				f4_size[i] = size;
				// DEBUG
				printf("%12s %3d:%3d[%3d] %3d %3d |%3d\n\r",
					CSH4Data::f4_key[i],
					f4_sid[i],f4_eid[i],f4_eid[i] - f4_sid[i]+1,
					f4_size[i],
					sizeof(float),
					(f4_eid[i] - f4_sid[i]+1)*f4_size[i]*sizeof(float));
				words = strchr(words,']')+1;
				words = strchr(words,']')+1;
				while(words[0] == ' ') words++;
				if(words[0] == '\0') return;
			}
		}
		words++;
	}
	printf("--------------------------------------------------------------\n\r");
}

void CSH4DataSet::SetData(char *buf)
{
	int i,j,k;
	for(i=CSH4DATA_S8_SIZE-1;i>=0;i--){
		for(j=s8_size[i]-1;j>=0;j--){
			for(k=s8_sid[i];k<=s8_eid[i];k++){
				memcpy(buf,&s8_data[i][k][j],sizeof(short));
				buf+=sizeof(short);
			}
		}
	}
	for(i=CSH4DATA_F8_SIZE-1;i>=0;i--){
		for(j=f8_size[i]-1;j>=0;j--){
			for(k=f8_sid[i];k<=f8_eid[i];k++){
				memcpy(buf,&f8_data[i][k][j],sizeof(float));
				buf+=sizeof(float);
			}
		}
	}
	for(i=CSH4DATA_F4_SIZE-1;i>=0;i--){
		for(j=f4_size[i]-1;j>=0;j--){
			for(k=f4_sid[i];k<=f4_eid[i];k++){
				memcpy(buf,&f4_data[i][k][j],sizeof(float));
				buf+=sizeof(float);
			}
		}
	}
}

int CSH4DataSet::CheckDataSize(int maxSize)
{
	int i,j,k;
	int data_size=0;
	for(i=0;i<CSH4DATA_S8_SIZE;i++){
		for(j=0;j<s8_size[i];j++){
			for(k=s8_sid[i];k<=s8_eid[i];k++){
				data_size+=sizeof(short);
			}
		}
	}
	for(i=0;i<CSH4DATA_F8_SIZE;i++){
		for(j=0;j<f8_size[i];j++){
			for(k=f8_sid[i];k<=f8_eid[i];k++){
				data_size+=sizeof(float);
			}
		}
	}
	for(i=0;i<CSH4DATA_F4_SIZE;i++){
		for(j=0;j<f4_size[i];j++){
			for(k=f4_sid[i];k<=f4_eid[i];k++){
				data_size+=sizeof(float);
			}
		}
	}

	if(data_size <= maxSize){
		printf("size(%d/%d Bytes)[%d Bytes left]\n\r\n\r",data_size,maxSize,maxSize-data_size);
		return 1;
	}
	else{
		printf("size(%d/%d Bytes)[%d Bytes over!!]\n\r\n\r",data_size,maxSize,maxSize-data_size);
		return 0;
	}
}

void CSH4DataSet::Reset()
{
	int i;
	for(i=0;i<CSH4DATA_S8_SIZE;i++){
		s8_size[i] = s8_eid[i] = s8_sid[i] =0;
	}
	for(i=0;i<CSH4DATA_F8_SIZE;i++){
		f8_size[i] = f8_eid[i] = f8_sid[i] =0;
	}
	for(i=0;i<CSH4DATA_F4_SIZE;i++){
		f4_size[i] = f4_eid[i] = f4_sid[i] =0;
	}
}

void CSH4DataSet::GetData(char *buf)
{
	int i,j,k;
	for(i=CSH4DATA_S8_SIZE-1;i>=0;i--){
		for(j=s8_size[i]-1;j>=0;j--){
			for(k=s8_sid[i];k<=s8_eid[i];k++){
				memcpy(&s8_data[i][k][j],buf,sizeof(short));
				BIG_TO_LITTLE_S(&s8_data[i][k][j]);
				buf+=sizeof(short);
			}
		}
	}
	for(i=CSH4DATA_F8_SIZE-1;i>=0;i--){
		for(j=f8_size[i]-1;j>=0;j--){
			for(k=f8_sid[i];k<=f8_eid[i];k++){
				memcpy(&f8_data[i][k][j],buf,sizeof(float));
				BIG_TO_LITTLE_F(&f8_data[i][k][j]);
				buf+=sizeof(float);
			}
		}
	}
	for(i=CSH4DATA_F4_SIZE-1;i>=0;i--){
		for(j=f4_size[i]-1;j>=0;j--){
			for(k=f4_sid[i];k<=f4_eid[i];k++){
				memcpy(&f4_data[i][k][j],buf,sizeof(float));
				BIG_TO_LITTLE_F(&f4_data[i][k][j]);
				buf+=sizeof(float);
			}
		}
	}
}

#ifdef WIN32
void CSH4DataSet::bigToLittlef(float *val)
{
	char* cval;
	char temp;
	cval = (char*)val;
	temp = cval[0];
	cval[0] = cval[3];
	cval[3] = temp;

	temp = cval[1];
	cval[1] = cval[2];
	cval[2] = temp;
}

void CSH4DataSet::bigToLittles(short *val)
{
	char* cval;
	char temp;
	cval = (char*)val;
	temp = cval[0];
	cval[0] = cval[1];
	cval[1] = temp;
}
#endif //#ifdef WIN32


#ifdef _DEBUG
int CSH4DataSet::CHKTestVector(CSH4DataSet& ds2)
{
	int i,j,k;
	int retVal = 1;
	printf("test vector check.\n\r");
	for(i=0;i<CSH4DATA_S8_SIZE;i++){
		for(j=0;j<s8_size[i];j++){
			for(k=s8_sid[i];k<=s8_eid[i];k++){
				if(s8_data[i][k][j] != ds2.s8_data[i][k][j]){
					retVal = 0;
				}
			}
		}
	}
	for(i=0;i<CSH4DATA_F8_SIZE;i++){
		for(j=0;j<f8_size[i];j++){
			for(k=f8_sid[i];k<=f8_eid[i];k++){
				if(f8_data[i][k][j] != ds2.f8_data[i][k][j]){
					retVal = 0;
				}
			}
		}
	}
	for(i=0;i<CSH4DATA_F4_SIZE;i++){
		for(j=0;j<f4_size[i];j++){
			for(k=f4_sid[i];k<=f4_eid[i];k++){
				if(f4_data[i][k][j] != ds2.f4_data[i][k][j]){
					retVal = 0;
				}
			}
		}
	}

	if(retVal == 0){
		printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r");
	}
	else{
		printf("ok\n\r");
	}
	return retVal;
}
#endif //#ifdef _DEBUG
