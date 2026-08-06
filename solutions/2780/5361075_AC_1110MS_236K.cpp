#include<cstdio>
#include<algorithm>
using namespace std;

struct Point{
	int x,y;
};

int main(){
	int pointNum;
	while(scanf("%d",&pointNum)!=EOF){
		Point* points=new Point[pointNum];
		for(int i=0;i<pointNum;i++)
			scanf("%d%d",&points[i].x,&points[i].y);
		double* slopes=new double[pointNum];
		int max=0;
		for(int i=0;i<pointNum;i++){
			int count=0,slopeCount=0;
			for(int j=0;j<pointNum;j++){
				if(points[i].x==points[j].x)
					count++;
				else
					slopes[slopeCount++]=(points[j].y-points[i].y)/(double)(points[j].x-points[i].x);
			}
			sort(slopes,slopes+slopeCount);
			for(int begin=0;begin<slopeCount;){
				int end=begin;
				while(end<slopeCount && slopes[end]-slopes[begin]<1e-10)
					end++;
				if(count<end-begin+1)
					count=end-begin+1;
				begin=end;
			}
			if(max<count)
				max=count;
		}
		printf("%d\n",max);
		delete[] slopes;
		delete[] points;
	}
	return 0;
}