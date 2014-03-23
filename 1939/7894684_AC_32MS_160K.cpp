#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

struct Pos{
	double x,y;
};

int main(){
	int cityNum;
	while(scanf("%d",&cityNum)!=EOF){
		printf("%d ",cityNum);
		Pos first;
		scanf("%lf%lf",&first.x,&first.y);
		Pos prev=first,current;
		while(cityNum-->1){
			scanf("%lf%lf",&current.x,&current.y);
			printf("%.6f %.6f ",(prev.x+current.x)/2.0,(prev.y+current.y)/2.0);
			prev=current;
		}
		printf("%.6f %.6f\n",(prev.x+first.x)/2.0,(prev.y+first.y)/2.0);
	}
	return 0;
}