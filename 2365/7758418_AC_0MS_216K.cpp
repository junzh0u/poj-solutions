/*
 * main.cpp
 *
 *  Created on: 2010-10-18
 *      Author: junzhou
 */

//#define DEBUG

#include<cstdio>
#include<cmath>

const int NAIL_NUM_MAX=100;
const double PI=acos(-1.0);

int nailNum;
double nailRadius,length=0.0;

struct Coordinate{
	double x,y;
};
Coordinate nails[NAIL_NUM_MAX];

double distance(Coordinate left,Coordinate right){
#ifdef DEBUG
	printf("%f\n",sqrt(pow((left.x-right.x),2.0)+pow((left.y-right.y),2.0)));
#endif
	return sqrt(pow((left.x-right.x),2.0)+pow((left.y-right.y),2.0));
}

int main(){
	scanf("%d%lf",&nailNum,&nailRadius);
	for(int i=0;i<nailNum;i++)
		scanf("%lf%lf",&nails[i].x,&nails[i].y);
	for(int i=0;i<nailNum;i++)
		length+=distance(nails[i],nails[(i+1)%nailNum]);
	length+=2*PI*nailRadius;
	printf("%.2lf\n",length);
	return 0;
}
