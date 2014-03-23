#include<cstdio>
#include<memory.h>

const int EAST_NUM_MAX=1100,WEST_NUM_MAX=1100;

bool connected[EAST_NUM_MAX][WEST_NUM_MAX]={false};
int eastToWestSouth[EAST_NUM_MAX][WEST_NUM_MAX+1]={0};
int eastNorthToWestSouth[EAST_NUM_MAX+1][WEST_NUM_MAX+1]={0};

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	for(int caseID=1;caseID<=caseNum;caseID++){
		int eastNum,westNum,wayNum;
		scanf("%d%d%d",&eastNum,&westNum,&wayNum);

		memset(connected,0,sizeof(bool)*EAST_NUM_MAX*WEST_NUM_MAX);
		memset(eastToWestSouth,0,sizeof(int)*EAST_NUM_MAX*(WEST_NUM_MAX+1));
		memset(eastNorthToWestSouth,0,sizeof(int)*(EAST_NUM_MAX+1)*(WEST_NUM_MAX+1));

		while(wayNum--){
			int eastIndex,westIndex;
			scanf("%d%d",&eastIndex,&westIndex);
			eastIndex--;
			westIndex--;
			connected[eastIndex][westIndex]=true;
		}

		for(int eastIndex=0;eastIndex<eastNum;eastIndex++)
			for(int westIndex=westNum-1;westIndex>=0;westIndex--){
				eastToWestSouth[eastIndex][westIndex]=eastToWestSouth[eastIndex][westIndex+1];
				if(connected[eastIndex][westIndex])
					eastToWestSouth[eastIndex][westIndex]++;
			}
		for(int eastIndex=0;eastIndex<eastNum;eastIndex++)
			for(int westIndex=0;westIndex<westNum;westIndex++)
				eastNorthToWestSouth[eastIndex+1][westIndex]=eastNorthToWestSouth[eastIndex][westIndex]+eastToWestSouth[eastIndex][westIndex];

		__int64 crossNum=0;
		for(int eastIndex=0;eastIndex<eastNum;eastIndex++)
			for(int westIndex=0;westIndex<westNum;westIndex++)
				if(connected[eastIndex][westIndex])
					crossNum+=eastNorthToWestSouth[eastIndex][westIndex+1];
		printf("Test case %d: %I64d\n",caseID,crossNum);
	}
	return 0;
}