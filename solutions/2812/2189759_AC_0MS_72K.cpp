#include<stdio.h>

void main()
{
	double dVertice[20][2],dArea,dVolumn;int iVerticeNum,i;
	while(scanf("%d",&iVerticeNum) && iVerticeNum!=0)
	{
		for(i=0;i<iVerticeNum;i++)scanf("%lf%lf",&dVertice[i][0],&dVertice[i][1]);
		scanf("%lf",&dVolumn);
		for(i=0,dArea=0.0;i<iVerticeNum;i++)
			dArea+=dVertice[(i+1)%iVerticeNum][0]*dVertice[i%iVerticeNum][1]-dVertice[(i+1)%iVerticeNum][1]*dVertice[i%iVerticeNum][0];
		printf("BAR LENGTH: %.2lf\n",2.0*dVolumn/dArea);
	}
}