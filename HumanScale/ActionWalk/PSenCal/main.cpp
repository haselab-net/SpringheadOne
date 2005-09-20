#include "PSensor.h"

#include <stdio.h>
using namespace LVRM;
using namespace std;

int main(int argc, char* argv[])
{
	int number,calibNum; //à≥óÕÉZÉìÉTÇÃêî
	FILE *fp;
	printf("Calibration For TurnTable ! (^^)/\n\n");
	printf("How Many Sensors Do You Use?\n");
	scanf("%d",&number);
	printf("%d\n\n",number);
	PSensor psen = PSensor(number);
	if(!psen.exist){
		return 0;
	}
	printf("How Many Times Will You Caliblate?\n");
	scanf("%d",&calibNum);
	psen.Calibration(calibNum);
	if(fp = fopen("calib","w")){
		psen.PrintFile(fp);
	}else{
		printf("Can't open the file");
	}
	fclose(fp);
	return 0;
}
