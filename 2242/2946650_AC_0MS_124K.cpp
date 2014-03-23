#include<cstdio>
#include<cmath>

int main(){
	const double PI=2.0*asin(1.0);
	double x[3],y[3];
	while(scanf("%lf%lf%lf%lf%lf%lf",&x[0],&y[0],&x[1],&y[1],&x[2],&y[2])!=EOF){
		double length[3],halfPerimeter=0.0;
		for(int i=0;i<3;i++){
			length[i]=sqrt(pow(x[i%3]-x[(i+1)%3],2)+pow(y[i%3]-y[(i+1)%3],2));
			halfPerimeter+=length[i]/2.0;
		}
		double area=halfPerimeter;
		for(i=0;i<3;i++)
			area*=halfPerimeter-length[i];
		area=sqrt(area);
		double radius=0.25/area;
		for(i=0;i<3;i++)
			radius*=length[i];
		printf("%.2lf\n",2.0*PI*radius);
	}
	return 0;
}