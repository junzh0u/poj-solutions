#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

//#define DEBUG

const int COW_NUM_MAX=25000,SHIFT_NUM_MAX=1000000;
int cowNum,shiftNum;

struct Interval{
	int begin,end;
};
Interval intervals[COW_NUM_MAX];

bool intervalCompare(const Interval &left,const Interval &right){
	if(left.begin!=right.begin)
		return left.begin<right.begin;
	return left.end>right.end;
}

int hire(){
	std::sort(intervals,intervals+cowNum,intervalCompare);
#ifdef DEBUG
	printf("After sort:\n");
	for(int cowIndex=0;cowIndex<cowNum;cowIndex++)
		printf("( %3d -> %3d )\n",intervals[cowIndex].begin,intervals[cowIndex].end);
	printf("\n");
#endif
	if(intervals[0].begin!=1)
		return -1;
	Interval total=intervals[0];
#ifdef DEBUG
	printf("Hire 0:\t( %3d -> %3d )\n",intervals[0].begin,intervals[0].end);
	printf("\tTotal:\t( %3d -> %3d )\n",total.begin,total.end);
#endif
	int prev=0,count=1,current=1;
	while(total.end!=shiftNum && current<cowNum){
		int hire=-1;
		for(;current<cowNum && intervals[current].begin<=total.end+1;current++)
			if(intervals[current].end>total.end && (hire==-1 || intervals[current].end>intervals[hire].end))
				hire=current;
		if(hire==-1)
			return -1;
		total.end=intervals[hire].end;
		count++;
#ifdef DEBUG
		printf("Hire %d:\t( %3d -> %3d )\n",hire,intervals[hire].begin,intervals[hire].end);
		printf("\tTotal:\t( %3d -> %3d )\n",total.begin,total.end);
#endif
	}
	if(total.end!=shiftNum)
		return -1;
	return count;
}

int main(){
	scanf("%d%d",&cowNum,&shiftNum);
	for(int cowIndex=0;cowIndex<cowNum;cowIndex++)
		scanf("%d%d",&intervals[cowIndex].begin,&intervals[cowIndex].end);
	printf("%d\n",hire());
	return 0;
}