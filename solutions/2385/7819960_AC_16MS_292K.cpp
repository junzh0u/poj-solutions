#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int WALK_MAX=30,TIME_MAX=1000;

int timeTotal,walkTimes;
int appleSum[2][TIME_MAX+1];
int appleMax[WALK_MAX+1][TIME_MAX];

int main(){
	scanf("%d%d",&timeTotal,&walkTimes);
	appleSum[0][0]=appleSum[1][0]=0;
	for(int i=0;i<timeTotal;i++){
		int tree;
		scanf("%d",&tree);
		appleSum[tree-1][i+1]=appleSum[tree-1][i]+1;
		appleSum[2-tree][i+1]=appleSum[2-tree][i];
	}
	for(int time=0;time<=timeTotal;time++)
		appleMax[0][time]=appleSum[0][time];
	int result=0;
	for(int walk=1;walk<=walkTimes;walk++){
		for(int time=0;time<=timeTotal;time++){
			appleMax[walk][time]=0;
			for(int timeWalk=0;timeWalk<time;timeWalk++){
				int apple=appleMax[walk-1][timeWalk]+appleSum[walk%2][time]-appleSum[walk%2][timeWalk];
				if(appleMax[walk][time]<apple)
					appleMax[walk][time]=apple;
			}
		}
		if(result<appleMax[walk][timeTotal])
			result=appleMax[walk][timeTotal];
	}
	printf("%d\n",result);
	return 0;
}