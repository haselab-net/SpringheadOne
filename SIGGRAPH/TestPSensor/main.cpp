#include "PSensor.h"

#include <stdio.h>
using namespace LVRM;
using namespace std;

int main(int argc, char* argv[])
{
	int number,peopleNumber; //ˆ³—ÍƒZƒ“ƒT‚Ì”
	FILE *fp;
	printf("Test Program ! (^^)/\n\n");
	printf("How Many Sensors Do You Use?\n");
	scanf("%d",&number);
	printf("%d\n\n",number);
	PSensor psen = PSensor(number);
	if(!psen.exist){
		return 0;
	}
	if(fp = fopen("calib","r")){
		psen.ReadFile(fp);
	}else{
		printf("Can't open the file");
		return 0;
	}
	fclose(fp);
	printf("How Many People Will You Test?\n");
	scanf("%d",&peopleNumber);
	printf("%d\n\n",peopleNumber);
	for(int i = 0; i < peopleNumber; i++){
		float weigh;
		printf("How much is your weigh?\n");
		scanf("%f",&weigh);
		printf("Weigh = %f\n",psen.TestWeigh());
	}
	return 0;
}
