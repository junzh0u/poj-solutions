#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cmath>

const int NONE=-1;

struct Coord{
	int x,y;
};

double length(Coord coord){
	return sqrt((double)(coord.x*coord.x+coord.y*coord.y));
}

double angleCos(Coord left,Coord right){
	return (left.x*right.x+left.y*right.y)/(length(left)*length(right));
}

int cross(Coord left,Coord right){
	return left.x*right.y-left.y*right.x;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int plantNum;
		scanf("%d",&plantNum);
		Coord *plants=new Coord[plantNum];
		bool *visited=new bool[plantNum];
		for(int i=0;i<plantNum;i++){
			int index;
			scanf("%d",&index);
			scanf("%d%d",&plants[index-1].x,&plants[index-1].y);
			visited[index-1]=false;
		}
		int current=NONE;
		for(int index=0;index<plantNum;index++)
			if(current==NONE || plants[current].y>plants[index].y)
				current=index;
		int *path=new int[plantNum];
		path[0]=current;
		visited[current]=true;
		Coord vectorPrev={1,0};
		for(int i=1;i<plantNum;i++){
			int next=NONE;
			double cosMax,distanceMin;
			for(int index=0;index<plantNum;index++){
				if(visited[index])
					continue;
				Coord vector={plants[index].x-plants[current].x,plants[index].y-plants[current].y};
				if(cross(vectorPrev,vector)<0)
					continue;
				double cos=angleCos(vectorPrev,vector);
				double distance=length(vector);
				if(next==NONE || cosMax<cos || (cosMax==cos && distanceMin>distance)){
					next=index;
					cosMax=cos;
					distanceMin=distance;
				}
			}
			if(next==NONE)
				break;
			path[i]=next;
			visited[next]=true;
			vectorPrev.x=plants[next].x-plants[current].x;
			vectorPrev.y=plants[next].y-plants[current].y;
			current=next;
		}
		printf("%d",plantNum);
		for(int i=0;i<plantNum;i++)
			printf(" %d",path[i]+1);
		printf("\n");
		delete[] path;
		delete[] visited;
		delete[] plants;
	}
	return 0;
}