#include<cstdio>
#include<algorithm>

//#define DEBUG

struct Point{
	int x,y;
};

bool moreClockwise(const Point &left,const Point &right){
	return left.x*right.y-left.y*right.x>0;
}

bool turnAnticlockwise(const Point &first,const Point &second,const Point &third){
	int x1=second.x-first.x,y1=second.y-first.y;
	int x2=third.x-second.x,y2=third.y-second.y;
	return x1*y2-x2*y1>0;
}

int main(){
	int pointNum;
	scanf("%d",&pointNum);

	Point *points=new Point[pointNum];
	for(int index=0;index<pointNum;index++)
		scanf("%d%d",&points[index].x,&points[index].y);
	
	int minXIndex=0;
	for(int index=1;index<pointNum;index++)
		if(points[index].x<points[minXIndex].x)
			minXIndex=index;
	std::swap(points[0],points[minXIndex]);
	for(int index=1;index<pointNum;index++){
		points[index].x-=points[0].x;
		points[index].y-=points[0].y;
	}
	points[0].x=points[0].y=0;

	std::sort(points+1,points+pointNum,moreClockwise);
	
#ifdef DEBUG
	printf("Points:\n");
	for(int index=0;index<pointNum;index++)
		printf("( %3d, %3d )\n",points[index].x,points[index].y);
#endif
	
	bool *convex=new bool[pointNum];
	std::fill(convex,convex+pointNum,true);
	int *prev=new int[pointNum];
	for(int index=0;index<pointNum;index++)
		prev[index]=(index+pointNum-1)%pointNum;

	for(int index=2;index<pointNum;index++){
		while(true){
			int prevIndex=prev[index];
			int prevIndex2=prev[prevIndex];
			if(turnAnticlockwise(points[prevIndex2],points[prevIndex],points[index]))
				break;
			convex[prevIndex]=false;
			prev[index]=prevIndex2;
		}
	}
	
#ifdef DEBUG
	printf("Points on convex:\n");
	for(int index=0;index<pointNum;index++)
		if(convex[index])
			printf("( %3d, %3d )\n",points[index].x,points[index].y);
#endif
	
	int area=0;
	for(int index=0;index<pointNum;index++)
		if(convex[index])
			area+=(points[index].x-points[prev[index]].x)*(points[index].y+points[prev[index]].y);
	if(area<0)
		area=0-area;
	
#ifdef DEBUG
	printf("Area:\t%d\n",area);
#endif
	
	printf("%d\n",area/100);

	delete[] prev;
	delete[] convex;
	delete[] points;

	return 0;
}