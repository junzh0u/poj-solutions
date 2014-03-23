#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<set>
using namespace std;

int main(){
	int pointNum;
	scanf("%d",&pointNum);
	set<pair<int,int>> points;
	for(int i=0;i<pointNum;i++){
		pair<int,int> point;
		scanf("%d%d",&point.first,&point.second);
		points.insert(point);
	}
	int squareNum=0;
	for(set<pair<int,int>>::iterator a=points.begin();a!=points.end();a++){
		for(set<pair<int,int>>::iterator b=a;b!=points.end();b++){
			if(a==b)
				continue;
			pair<int,int> c(b->first+b->second-a->second,b->second+a->first-b->first);
			if(points.find(c)==points.end())
				continue;
			pair<int,int> d(a->first+b->second-a->second,a->second+a->first-b->first);
			if(points.find(d)==points.end())
				continue;
			squareNum++;
		}
	}
	squareNum/=2;
	printf("%d\n",squareNum);
	return 0;
}