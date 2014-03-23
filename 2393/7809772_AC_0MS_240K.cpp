#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<climits>

const int WEEK_NUM_MAX=10000;


int main(){
	int weekNum,fee;
	scanf("%d%d",&weekNum,&fee);
	int cost[WEEK_NUM_MAX],deliveries[WEEK_NUM_MAX];
	for(int i=0;i<weekNum;i++)
		scanf("%d%d",cost+i,deliveries+i);
	int min=INT_MAX;
	long long costSum=0;
	for(int i=0;i<weekNum;i++){
		if(min>cost[i]-i*fee)
			min=cost[i]-i*fee;
		costSum+=(min+i*fee)*deliveries[i];
	}
	printf("%lld\n",costSum);
	return 0;
}