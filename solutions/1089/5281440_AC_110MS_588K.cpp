#include<cstdio>
#include<algorithm>
#include<cmath>
using namespace std;

struct Interval{
	int lower;
	int higher;
};

bool IntervalCompare(const Interval& left,const Interval& right){
	if(left.lower!=right.lower)
		return left.lower<right.lower;
	return left.higher<right.higher;
}

int main(){
	int intervalNum;
	scanf("%d",&intervalNum);
	Interval *intervals=new Interval[intervalNum];
	for(int i=0;i<intervalNum;i++)
		scanf("%d%d",&intervals[i].lower,&intervals[i].higher);
	sort(intervals,intervals+intervalNum,IntervalCompare);
	for(int i=0;i<intervalNum;){
		int next=i+1;
		while(next<intervalNum && intervals[i].higher>=intervals[next].lower){
			if(intervals[i].higher<intervals[next].higher)
				intervals[i].higher=intervals[next].higher;
			next++;
		}
		printf("%d %d\n",intervals[i].lower,intervals[i].higher);
		i=next;
	}
	delete[] intervals;
	return 0;
}