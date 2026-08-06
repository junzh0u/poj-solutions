#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int JURY_NUM_MAX=100,PROBLEM_NUM_MAX=100;

int main(){
	int juryNum,problemNum;
	scanf("%d%d",&juryNum,&problemNum);
	int simplestCount[PROBLEM_NUM_MAX],hardestCount[PROBLEM_NUM_MAX];
	std::fill(simplestCount,simplestCount+PROBLEM_NUM_MAX,0);
	std::fill(hardestCount,hardestCount+PROBLEM_NUM_MAX,0);
	for(int jury=0;jury<juryNum;jury++){
		int complexity[PROBLEM_NUM_MAX];
		for(int problem=0;problem<problemNum;problem++)
			scanf("%d",complexity+problem);
		int complexityMin=*std::min_element(complexity,complexity+problemNum);
		int complexityMax=*std::max_element(complexity,complexity+problemNum);
		for(int problem=0;problem<problemNum;problem++){
			if(complexity[problem]==complexityMin)
				simplestCount[problem]++;
			if(complexity[problem]==complexityMax)
				hardestCount[problem]++;
		}
	}
	bool exist=false;
	for(int problem=0;problem<problemNum;problem++)
		if(!hardestCount[problem] && simplestCount[problem]>juryNum/2){
			printf("%d ",problem+1);
			exist=true;
		}
	if(!exist)
		printf("0");
	printf("\n");
	return 0;
}