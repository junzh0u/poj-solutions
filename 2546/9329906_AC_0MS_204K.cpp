#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<algorithm>
#include<cmath>
using std::min;

const double pi=acos(-1.0);

int main(){
	double x1,y1,r1,x2,y2,r2;
	scanf("%lf%lf%lf%lf%lf%lf",&x1,&y1,&r1,&x2,&y2,&r2);
	double area;
	double d=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	if(d<=fabs(r1-r2))
		area=pi*min(r1,r2)*min(r1,r2);
	else if(d>=r1+r2)
		area=0.0;
	else{
		double a1=acos((r1*r1+d*d-r2*r2)/(2.0*r1*d));
		double a2=acos((r2*r2+d*d-r1*r1)/(2.0*r2*d));
		area=r1*r1*a1+r2*r2*a2-r1*d*sin(a1);
	}
	printf("%.3f\n",area);
	return 0;
}