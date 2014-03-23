#include<cstdio>
#include<cmath>
#include<algorithm>
using namespace std;

//#define DEBUG

struct Interval{
	double begin,end;
};

bool IntervalLess(const Interval &left,const Interval &right){
	return left.begin<right.begin;
}

int main(){
	int caseID=0;
	while(true){
		int islandNum,radarRange;
		scanf("%d%d",&islandNum,&radarRange);
		if(islandNum==0 && radarRange==0)
			break;
		caseID++;
		bool possible=true;
		Interval *intervals=new Interval[islandNum];
		for(int i=0;i<islandNum;i++){
			int x,y;
			scanf("%d%d",&x,&y);
			if(possible){
				//y=abs(y);
				if(y>radarRange)
					possible=false;
				else{
					double width=sqrt((double)radarRange*radarRange-y*y);
					intervals[i].begin=x-width;
					intervals[i].end=x+width;
				}
			}
		}
		if(!possible){
			printf("Case %d: -1\n",caseID);
			continue;
		}
		sort(intervals,intervals+islandNum,IntervalLess);
#ifdef DEBUG
		for(int i=0;i<islandNum;i++)
			printf("(%d,%d),",intervals[i].begin,intervals[i].end);
		printf("\n");
#endif
		double radarX=intervals[0].end;
		int radarCount=1;
		for(int i=1;i<islandNum;i++){
			if(radarX<intervals[i].begin){
#ifdef DEBUG
				printf("%d,",radarX);
#endif
				radarCount++;
				radarX=intervals[i].end;
			}
			else if(radarX>intervals[i].end)
				radarX=intervals[i].end;
		}
#ifdef DEBUG
		printf("%d\n",radarX);
#endif
		printf("Case %d: %d\n",caseID,radarCount);
	}
	return 0;
}