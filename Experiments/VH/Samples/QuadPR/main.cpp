
#include "QDPRSolver.h"

#define TIMES 20

int main(){

	Spr::QDPRSolver qd_pr_solver;
	qd_pr_solver.Init();
	qd_pr_solver.Calc();
	qd_pr_solver.DataOut();
	
	/*
	qd_pr_solver.Init();
	qd_pr_solver.ParamOut();
	for(int k = 0; k < TIMES; k++){
		qd_pr_solver.Calc();
		printf("***************** ‘æ %d@‰ñ–Ú***************\n",k+1);
		qd_pr_solver.Out();
		qd_pr_solver.DataOut(k);
		qd_pr_solver.Set(k);
		//if(k ==4)
			//qd_pr_solver.ParamOut();
	}
	*/
	return 0;
}
