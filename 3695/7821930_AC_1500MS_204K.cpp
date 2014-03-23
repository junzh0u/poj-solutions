#define _CRT_SECURE_NO_WARNINGS
//#undef _DEBUG

#include<cstdio>
#include<algorithm>
#include<set>

const int RECT_NUM_MAX=30;
const int X_NUM_MAX=2*RECT_NUM_MAX,Y_NUM_MAX=2*RECT_NUM_MAX;

int rectNum,queryNum;
int xs[2*RECT_NUM_MAX],xNum,ys[2*RECT_NUM_MAX],yNum;

struct Rect{
	int xBegin,yBegin,xEnd,yEnd;
	void read(){
		scanf("%d%d%d%d",&xBegin,&yBegin,&xEnd,&yEnd);
		xs[xNum++]=xBegin;
		xs[xNum++]=xEnd;
		ys[yNum++]=yBegin;
		ys[yNum++]=yEnd;
	}
	bool include(const Rect &a){
		return xBegin<=a.xBegin && xEnd>=a.xEnd && yBegin<=a.yBegin && yEnd>=a.yEnd;
	}
	int area(){
		return (xEnd-xBegin)*(yEnd-yBegin);
	}
#ifdef _DEBUG
	void print(){
		printf("{ x: %d->%d , y: %d->%d }\n",xBegin,xEnd,yBegin,yEnd);
	}
#endif
};
Rect rects[RECT_NUM_MAX];

void sortAndSingle(int *array,int &size){
	std::set<int> temp;
	for(int i=0;i<size;i++)
		temp.insert(array[i]);
	size=0;
	for(std::set<int>::iterator iter=temp.begin();iter!=temp.end();iter++)
		array[size++]=*iter;
}

int belongMask[X_NUM_MAX-1][Y_NUM_MAX-1];
int area[X_NUM_MAX-1][Y_NUM_MAX-1];

int main(){
	int caseID=0;
	while(true){
		scanf("%d%d",&rectNum,&queryNum);
		if(!rectNum && !queryNum)
			return 0;
		xNum=yNum=0;
		for(int i=0;i<rectNum;i++)
			rects[i].read();
#ifdef _DEBUG
		printf("# Rectangles:\n");
		for(int i=0;i<rectNum;i++)
			rects[i].print();
#endif
		sortAndSingle(xs,xNum);
		sortAndSingle(ys,yNum);
#ifdef _DEBUG
		printf("# Xs:\n");
		for(int i=0;i<xNum;i++)
			printf("%d ",xs[i]);
		printf("\n# Ys:\n");
		for(int i=0;i<yNum;i++)
			printf("%d ",ys[i]);
		printf("\n");
#endif
		for(int xIndex=0;xIndex+1<xNum;xIndex++)
			for(int yIndex=0;yIndex+1<yNum;yIndex++){
				belongMask[xIndex][yIndex]=0;
				Rect small={xs[xIndex],ys[yIndex],xs[xIndex+1],ys[yIndex+1]};
#ifdef _DEBUG
				printf("# Grid[%d,%d]: ",xIndex,yIndex);
				small.print();
#endif
				for(int rectIndex=0;rectIndex<rectNum;rectIndex++){
					if(rects[rectIndex].include(small)){
						belongMask[xIndex][yIndex]|=(1<<rectIndex);
#ifdef _DEBUG
						printf("\tBelongs to: ");
						rects[rectIndex].print();
#endif
					}}
				area[xIndex][yIndex]=small.area();
#ifdef _DEBUG
				printf("# Belong mask = %d\n",belongMask[xIndex][yIndex]);
				printf("# Area = %d\n",area[xIndex][yIndex]);
#endif
			}

			printf("Case %d:\n",++caseID);
			for(int queryID=1;queryID<=queryNum;queryID++){
				int fillNum,fillMask=0;
				scanf("%d",&fillNum);
				while(fillNum--){
					int rectIndex;
					scanf("%d",&rectIndex);
					fillMask|=(1<<(rectIndex-1));
				}
#ifdef _DEBUG
				printf("# Fill mask = %d\n",fillMask);
#endif
				int areaSum=0;
				for(int xIndex=0;xIndex+1<xNum;xIndex++){
					for(int yIndex=0;yIndex+1<yNum;yIndex++){
						if(belongMask[xIndex][yIndex]&fillMask){
							areaSum+=area[xIndex][yIndex];
#ifdef _DEBUG
							printf("# Include Grid[%d,%d]\n",xIndex,yIndex);
#endif
						}}}
				printf("Query %d: %d\n",queryID,areaSum);
			}
			printf("\n");
	}
}