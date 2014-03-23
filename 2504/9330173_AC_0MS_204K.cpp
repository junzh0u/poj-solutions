#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cmath>
#include<algorithm>
using namespace std;

const double pi=acos(-1.0);

int main(){
	for(int id=1;;id++){
		int vertexNum;
		scanf("%d",&vertexNum);
		if(!vertexNum)
			return 0;
		double x1,y1,x2,y2,x3,y3;
		scanf("%lf%lf%lf%lf%lf%lf",&x1,&y1,&x2,&y2,&x3,&y3);
		double xc=(x3*x3*(-y1+y2)+x2*x2*(y1-y3)-(x1*x1+(y1-y2)*(y1-y3))*(y2-y3))/
			(2.0*(x3*(-y1+y2)+x2*(y1-y3)+x1*(-y2+y3)));
		double yc=(-(x2*x2*x3)+x1*x1*(-x2+x3)+x3*(y1*y1-y2*y2)+x1*(x2*x2-x3*x3+y2*y2-y3*y3)+x2*(x3*x3-y1*y1+y3*y3))/
			(2.0*(x3*(y1-y2)+x1*(y2-y3)+x2*(-y1+y3)));
		double r=sqrt((x1-xc)*(x1-xc)+(y1-yc)*(y1-yc));
		double angle=acos((x1-xc)/r);
		if(y1<yc)
			angle=-angle;
		double xmin,xmax,ymin,ymax;
		xmin=xmax=xc;
		ymin=ymax=yc;
		for(int i=0;i<vertexNum;i++){
			double x=xc+r*cos(angle);
			xmin=min(xmin,x);
			xmax=max(xmax,x);
			double y=yc+r*sin(angle);
			ymin=min(ymin,y);
			ymax=max(ymax,y);
			angle+=2.0*pi/vertexNum;
		}
		printf("Polygon %d: %.3f\n",id,(xmax-xmin)*(ymax-ymin));
	}
}