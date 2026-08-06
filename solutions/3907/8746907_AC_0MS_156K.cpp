#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<cmath>

int main(){
	while(true){
		int vectorNum;
		scanf("%d",&vectorNum);
		if(!vectorNum)
			return 0;
		double *x=new double[vectorNum],*y=new double[vectorNum];
		for(int i=0;i<vectorNum;i++)
			scanf("%lf%lf",x+i,y+i);
		double area=0.0;
		for(int i=0;i<vectorNum;i++){
			int next=(i+1)%vectorNum;
			area+=x[i]*y[next]-y[i]*x[next];
		}
		area=0.5*fabs(area);
		printf("%d\n",(int)(area+0.5));
		delete[] x;
		delete[] y;
	}
}