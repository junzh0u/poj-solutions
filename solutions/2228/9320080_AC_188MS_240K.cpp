#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>

const int TIME_TOTAL_MAX=3830;
const int TIME_BED_MAX=TIME_TOTAL_MAX-1;
int timeTotal,timeBed,utilities[TIME_TOTAL_MAX];
int utilityTotalWithLast[2][TIME_BED_MAX+1];
int utilityTotalWithoutLast[2][TIME_BED_MAX+1];

void initialize(){
	for(int total=0;total<2;total++)
		for(int bed=0;bed<=timeBed;bed++)
			utilityTotalWithLast[total][bed]=utilityTotalWithoutLast[total][bed]=0;
}

void dp(){
	for(int total=1;total<timeTotal;total++){
		utilityTotalWithLast[total%2][0]=utilityTotalWithLast[total%2][1]=0;
		for(int bed=2;bed<=total+1 && bed<=timeBed;bed++)
			utilityTotalWithLast[total%2][bed]=std::max(utilityTotalWithLast[(total-1)%2][bed-1]+utilities[total],utilityTotalWithoutLast[(total-1)%2][bed-1]);
		utilityTotalWithoutLast[total%2][0]=0;
		for(int bed=1;bed<=total && bed<=timeBed;bed++)
			utilityTotalWithoutLast[total%2][bed]=std::max(utilityTotalWithLast[(total-1)%2][bed],utilityTotalWithoutLast[(total-1)%2][bed]);
	}
}

int sleepInLastHour(){
	initialize();
	utilityTotalWithLast[0][1]=utilities[0];
	dp();
	return utilityTotalWithLast[(timeTotal-1)%2][timeBed];
}

int dontSleepInLastHour(){
	initialize();
	utilityTotalWithLast[0][1]=0;
	dp();
	return utilityTotalWithoutLast[(timeTotal-1)%2][timeBed];
}

int main(){
	scanf("%d%d",&timeTotal,&timeBed);
	for(int time=0;time<timeTotal;time++)
		scanf("%d",utilities+time);
	printf("%d\n",std::max(sleepInLastHour(),dontSleepInLastHour()));
	return 0;
}