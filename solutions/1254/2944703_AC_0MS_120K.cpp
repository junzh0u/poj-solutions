#include<cstdio>
#include<cmath>

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum-->0){
		int markerX[2],markerY[2],degree[2];
		scanf("%d%d%d",&markerX[0],&markerY[0],&degree[0]);
		scanf("%d%d%d",&markerX[1],&markerY[1],&degree[1]);
		double radian[2]={degree[0]*asin(1.0)/90.0,degree[1]*asin(1.0)/90.0};
		double x=(cos(radian[0])*sin(radian[1])*markerX[0]-sin(radian[0])*cos(radian[1])*markerX[1]+
			sin(radian[0])*sin(radian[1])*(markerY[1]-markerY[0]))/sin(radian[1]-radian[0]),y;
		if(degree[0]%180!=0)
			y=(x-markerX[0])/tan(radian[0])+markerY[0];
		else
			y=(x-markerX[1])/tan(radian[1])+markerY[1];
		printf("%.4lf %.4lf\n",x,y);
	}
	return 0;
}