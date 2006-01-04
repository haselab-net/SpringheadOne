// MatrixLoader.h: CMatrixLoader クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIXLODER_H__18B4CC68_8CCE_4963_A6B3_587A4888E7EB__INCLUDED_)
#define AFX_MATRIXLODER_H__18B4CC68_8CCE_4963_A6B3_587A4888E7EB__INCLUDED_
#include <stdio.h>
#include <string.h>

#define MAXLINE 1024*2

template<class T1, int N1>
class CMatrixLoader  
{
public:
	void printMat(int size){
		int i,j,k;
		for(i=0;i<size;i++){
			fprintf(stdout,"q: %d\n",i);
			for(j=0;j<8;j++){
				for(k=0;k<8;k++){
					fprintf(stdout,"%f ",matQ[i][j][k]);
				}
				fprintf(stdout,"\n");
			}
			fprintf(stdout,"c: %d\n",i);
			for(j=0;j<8;j++){
				fprintf(stdout,"%f ",vecC[i][j]);
			}			fprintf(stdout,"\n\n");
		}

	}

	void setFile(char* filename){
		FILE* fp;
		fp = fopen(filename,"w");
		int i,j,k;
		for(i=0;i<10;i++){
			fprintf(fp,":q %d\n",i);
			for(j=0;j<8;j++){
				for(k=0;k<8;k++){
					fprintf(fp,"%f ",(float)j*8+k+1);
				}
				fprintf(fp,"\n");
			}
			fprintf(fp,":c %d\n",i);
			for(j=0;j<8;j++){
				fprintf(fp,"%f ",(float)i*8+j+1);
			}
			fprintf(fp,"\n\n");
		}
		fclose(fp);
	}

	void loadVecC(FILE* fp, int num){
		char s[MAXLINE];
		float val[8];
		int i;
		fgets(s,MAXLINE,fp);
		//sscanf(s,"%lf %lf %lf %lf %lf %lf %lf %lf",
		sscanf(s,"%f %f %f %f %f %f %f %f",
			&val[0],
			&val[1],
			&val[2],
			&val[3],
			&val[4],
			&val[5],
			&val[6],
			&val[7]
			);
		for(i=0;i<8;i++){
			vecC[num][i] = (T1)val[i];
		}
		/*
		sscanf(s,"%f %f %f %f %f %f %f %f",
			&vecC[num][0],
			&vecC[num][1],
			&vecC[num][2],
			&vecC[num][3],
			&vecC[num][4],
			&vecC[num][5],
			&vecC[num][6],
			&vecC[num][7]
			);
			*/
	}
	void loadMatQ(FILE* fp, int num){
		char s[MAXLINE];
		int i,j;
		float val[8];
		for(i=0;i<8;i++){
			fgets(s,MAXLINE,fp);
			sscanf(s,"%f %f %f %f %f %f %f %f",
				&val[0],
				&val[1],
				&val[2],
				&val[3],
				&val[4],
				&val[5],
				&val[6],
				&val[7]
				);
			for(j=0;j<8;j++){
				matQ[num][i][j] = (T1)val[j];
			}
		}

	}

	void loadFile(char* filename, int size){
		FILE* fp;
		fp = fopen(filename,"r");
		char s[MAXLINE], s2[MAXLINE];
		int count,i=0;
		while(fgets(s,MAXLINE,fp)){
			if(s[0] == ':'){
				sscanf(s,"%s %d",s2,&count);
				if(strcmp(s2,":q") == 0){
					loadMatQ(fp,i);
				}
				else if(strcmp(s2,":c") == 0){
					loadVecC(fp,i);
					i++;
					if(i>=size) break;
				}
			}
		}
		fclose(fp);
	}
	
	CMatrixLoader(){}
	virtual ~CMatrixLoader(){}

	T1 matQ[100][8][8];
	T1 vecC[100][8];
};
#endif // !defined(AFX_MATRIXLODER_H__18B4CC68_8CCE_4963_A6B3_587A4888E7EB__INCLUDED_)
