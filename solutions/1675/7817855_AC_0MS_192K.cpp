#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<cmath>

const double PI=acos(-1.0);

bool valid(int *x,int *y){
	for(int i=0;i<3;i++)
		if(x[i]==0 && y[i]==0)
			return false;
	double angles[3];
	double angleMax=-2.0*PI;
	double angleMin=2.0*PI;
	for(int i=0;i<3;i++){
		angles[i]=atan2((double)y[i],(double)x[i]);
		if(angleMax<angles[i])
			angleMax=angles[i];
		if(angleMin>angles[i])
			angleMin=angles[i];
	}
	return angleMax-angleMin>PI*2.0/3.0;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int r,x[3],y[3];
		scanf("%d",&r);
		for(int i=0;i<3;i++)
			scanf("%d%d",x+i,y+i);
		printf(valid(x,y)?"Yes\n":"No\n");
	}
	return 0;
}