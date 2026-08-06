#define _CRT_SECURE_NO_WARNINGS
//#define DEBUG

#include<cstdio>

const int TOTAL_MAX=1001;
const double UNDEF=-1.0;

double answerMemory[TOTAL_MAX+1][TOTAL_MAX+1];
double probability(int color,int total,int remain){
	if(remain>color || remain>total || remain<0)
		return 0.0;
	if((total+remain)%2==1)
		return 0.0;
	if(total==0 || total==1)
		return 1.0;
	if(answerMemory[total][remain]==UNDEF)
		answerMemory[total][remain]=probability(color,total-1,remain+1)*(remain+1.0)/color+
									probability(color,total-1,remain-1)*(1.0-(remain-1.0)/color);
#ifdef DEBUG
	printf("# probability(%d,%d,%d) = %f\n",color,total,remain,answerMemory[total][remain]);
#endif
	return answerMemory[total][remain];
}

int main(){
	int color,total,remain;
	while(true){
		scanf("%d",&color);
		if(color==0)
			return 0;
		scanf("%d%d",&total,&remain);
		if(total>1000)
			total=1000+total%2;
		for(int i=0;i<=total;i++)
			for(int j=0;j<=total;j++)
				answerMemory[i][j]=UNDEF;
		printf("%.3f\n",probability(color,total,remain));
	}
}