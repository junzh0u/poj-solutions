#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

struct Coord{
	double x,y;
};

int main(){
	int num;
	while(scanf("%d",&num)!=EOF){
		Coord *middles=new Coord[num];
		for(int i=0;i<num;i++)
			scanf("%lf%lf",&middles[i].x,&middles[i].y);
		Coord *vertices=new Coord[num];
		vertices[0]=middles[0];
		for(int i=1;i<num;i+=2){
			vertices[0].x+=(middles[i+1].x-middles[i].x);
			vertices[0].y+=(middles[i+1].y-middles[i].y);
		}
		for(int i=1;i<num;i++){
			vertices[i].x=2*middles[i-1].x-vertices[i-1].x;
			vertices[i].y=2*middles[i-1].y-vertices[i-1].y;
		}
		printf("%d",num);
		for(int i=0;i<num;i++)
			printf(" %.6f %.6f",vertices[i].x,vertices[i].y);
		printf("\n");
		delete[] vertices;
		delete[] middles;
	}
	return 0;
}