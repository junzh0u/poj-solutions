#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>

struct Crosspoint{
	bool exist;
	double x,y;
};

struct Sphere{
	double x,y,r;
};

struct Line{
	double x,y,dx,dy;
	Crosspoint intersect(const Sphere& sphere){
		Crosspoint crosspoint;
		crosspoint.exist=false;
		double sx=sphere.x,sy=sphere.y,r=sphere.r;
		double a=dx*dx+dy*dy;
		double b=-2.0*dx*sx-2.0*dy*sy+2.0*dx*x+2.0*dy*y;
		double c=-r*r+sx*sx+sy*sy-2*sx*x+x*x-2*sy*y+y*y;
		double delta=b*b-4.0*a*c;
		if(delta<0)
			return crosspoint;
		//double k1=(-b+sqrt(delta))/(2.0*a);
		//double k2=(-b-sqrt(delta))/(2.0*a);
		double k=(-b-sqrt(delta))/(2.0*a);
		if(k<0)
			return crosspoint;
		crosspoint.x=x+dx*k;
		crosspoint.y=y+dy*k;
		crosspoint.exist=true;
		return crosspoint;
	}
	void hit(const Sphere& sphere){
		Crosspoint crosspoint=intersect(sphere);
		double rx=crosspoint.x-sphere.x,ry=crosspoint.y-sphere.y;
		double dx2=(-dx*rx*rx-2.0*dy*rx*ry+dx*ry*ry)/(rx*rx+ry*ry);
		double dy2=(dy*rx*rx-2.0*dx*rx*ry-dy*ry*ry)/(rx*rx+ry*ry);
		x=crosspoint.x;
		y=crosspoint.y;
		dx=dx2;
		dy=dy2;
	}
};

const int SPHERE_NUM_MAX=30,HIT_TIME_SHOW_MAX=10,HIT_TIME_MAX=HIT_TIME_SHOW_MAX+1;
int sphereNum;
Sphere spheres[SPHERE_NUM_MAX];
Line ray;
int hitTime,hitSequence[HIT_TIME_MAX];

bool input(){
	scanf("%d",&sphereNum);
	if(!sphereNum)
		return false;
	for(int i=0;i<sphereNum;i++)
		scanf("%lf%lf%lf",&spheres[i].x,&spheres[i].y,&spheres[i].r);
	scanf("%lf%lf%lf%lf",&ray.x,&ray.y,&ray.dx,&ray.dy);
	return true;
}

void run(){
	hitTime=0;
	while(hitTime<HIT_TIME_MAX){
		int hitIndex=-1;
		double distanceMin;
		for(int i=0;i<sphereNum;i++){
			Crosspoint crosspoint=ray.intersect(spheres[i]);
			if(crosspoint.exist){
				double dx=crosspoint.x-ray.x;
				double dy=crosspoint.y-ray.y;
				double distance=sqrt(dx*dx+dy*dy);
				if(hitIndex<0 || distanceMin>distance){
					hitIndex=i;
					distanceMin=distance;
				}
			}
		}
		if(hitIndex<0)
			break;
		ray.hit(spheres[hitIndex]);
		hitSequence[hitTime++]=hitIndex;
	}
}

void output(){
	for(int i=0;i<hitTime && i<HIT_TIME_SHOW_MAX;i++)
		printf("%d ",hitSequence[i]+1);
	if(hitTime>HIT_TIME_SHOW_MAX)
		printf("...\n");
	else
		printf("inf\n");
}

int main(){
	int sceneId=1;
	while(input()){
		run();
		printf("Scene %d\n",sceneId++);
		output();
		printf("\n");
	}
	return 0;
}