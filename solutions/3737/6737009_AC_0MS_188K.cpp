#include<cmath>
#include<cstdio>

int main(){
	const double pi=acos(-1.0);
	double area;
	while(scanf("%lf",&area)!=EOF){
		double height=sqrt(2.0*area/pi);
		double radius=sqrt(area*area/(pi*pi*height*height+2*area*pi));
		double volume=pi*radius*radius*height/3.0;
		printf("%.2f\n%.2f\n%.2f\n",volume,height,radius);
	}
	return 0;
}