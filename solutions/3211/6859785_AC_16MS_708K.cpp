#include<cstdio>
#include<cstring>
#include<memory>
#include<algorithm>

const int COLOR_NUM_MAX=10,CLOTH_NUM_MAX=100,COLOR_NAME_LEN_MAX=10,TIME_MAX=1000;

int queryTable[CLOTH_NUM_MAX*TIME_MAX+1][CLOTH_NUM_MAX+1];

int timeUsedMax(int timeLimit,const int clothNum,const int clothTimes[]){
	for(int limit=0;limit<=timeLimit;limit++)
		queryTable[limit][0]=0;
	for(int end=0;end<=clothNum;end++)
		queryTable[0][end]=0;
	for(int end=1;end<=clothNum;end++){
		for(int limit=1;limit<=timeLimit;limit++){
			queryTable[limit][end]=queryTable[limit][end-1];
			if(limit>=clothTimes[end-1]){
				int temp=queryTable[limit-clothTimes[end-1]][end-1]+clothTimes[end-1];
				if(queryTable[limit][end]<temp)
					queryTable[limit][end]=temp;
			}
		}
	}
	return queryTable[timeLimit][clothNum];
}

int timeNeeded(int clothNum,int clothTimes[]){
	int timeSum=0;
	for(int index=0;index<clothNum;index++)
		timeSum+=clothTimes[index];
	//memset(queryTable,-1,sizeof(int)*CLOTH_NUM_MAX*TIME_MAX*CLOTH_NUM_MAX);
	int girlfriendTime=timeUsedMax(timeSum/2,clothNum,clothTimes);
	return timeSum-girlfriendTime;
}

int main(){
	int colorNum,clothNum;
	while(scanf("%d %d",&colorNum,&clothNum)){
		if(colorNum==0 && clothNum==0)
			return 0;
		char colorNames[COLOR_NUM_MAX][COLOR_NAME_LEN_MAX+1];
		for(int colorIndex=0;colorIndex<colorNum;colorIndex++)
			scanf("%s",colorNames+colorIndex);
		int colorClothNum[COLOR_NUM_MAX]={0};
		int colorClothTimes[COLOR_NUM_MAX][CLOTH_NUM_MAX];
		for(int clothIndex=0;clothIndex<clothNum;clothIndex++){
			int time;
			char colorName[COLOR_NAME_LEN_MAX+1];
			scanf("%d %s",&time,colorName);
			for(int colorIndex=0;colorIndex<colorNum;colorIndex++)
				if(strcmp(colorName,colorNames[colorIndex])==0){
					colorClothTimes[colorIndex][colorClothNum[colorIndex]++]=time;
					break;
				}
		}
		int timeNeededTotal=0;
		for(int colorIndex=0;colorIndex<colorNum;colorIndex++)
			timeNeededTotal+=timeNeeded(colorClothNum[colorIndex],colorClothTimes[colorIndex]);
		printf("%d\n",timeNeededTotal);
	}
}