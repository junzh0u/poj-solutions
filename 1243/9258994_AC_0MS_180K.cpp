#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int GUESS_MAX=30,LIFE_MAX=30;

int maxprices[GUESS_MAX+1][LIFE_MAX+1];

void initialize(){
	for(int life=0;life<=LIFE_MAX;life++)
		maxprices[0][life]=0;
	for(int guess=0;guess<=GUESS_MAX;guess++)
		maxprices[guess][0]=guess;
	for(int guess=1;guess<=GUESS_MAX;guess++)
		for(int life=1;life<=GUESS_MAX;life++)
			maxprices[guess][life]=maxprices[guess-1][life-1]+1+maxprices[guess-1][life];
}

int main(){
	initialize();
	for(int caseId=1;;caseId++){
		int guess,life;
		scanf("%d%d",&guess,&life);
		if(!guess && !life)
			return 0;
		printf("Case %d: %d\n",caseId,maxprices[guess][life]);
	}
}