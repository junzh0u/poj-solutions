#include<cstdio>
#include<cmath>

struct Point{
	double x,y;
};

const double PI=2*asin(1.0);

double Angle(Point vertice){
	if(vertice.y>0)
		return acos(vertice.x/sqrt(vertice.x*vertice.x+vertice.y*vertice.y));
	return 2*PI-acos(vertice.x/sqrt(vertice.x*vertice.x+vertice.y*vertice.y));
}

double AngleInclude(Point from,Point to){
	double angle=Angle(to)-Angle(from);
	if(angle>=PI)
		angle-=2*PI;
	if(angle<=(-1.0)*PI)
		angle+=2*PI;
	return angle;
}

int main(){
	double factor,height;
	int verticeNum;
	scanf("%lf%lf%d",&factor,&height,&verticeNum);
	Point first,current;
	scanf("%lf%lf",&current.x,&current.y);
	first=current;
	double angle=0.0,angleMin=0.0,angleMax=0.0;
	while(verticeNum-->1){
		Point next;
		scanf("%lf%lf",&next.x,&next.y);
		angle+=AngleInclude(current,next);
		if(angleMin>angle)
			angleMin=angle;
		if(angleMax<angle)
			angleMax=angle;
		current=next;
	}
	angle+=AngleInclude(current,first);
	if(angleMin>angle)
		angleMin=angle;
	if(angleMax<angle)
		angleMax=angle;
	angle=angleMax-angleMin;
	if(angle>2*PI)
		angle=2*PI;
	printf("%.2lf\n",angle*factor*height);
	return 0;
}