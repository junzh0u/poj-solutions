#include<stdio.h>
#include<math.h>

int Count(double dSize,double* dArea,int iPieNum)
{
	int iReturn=0,i;
	for(i=0;i<iPieNum;i++)iReturn+=dArea[i]/dSize;
	return iReturn;
}

void main()
{
	int iCaseNum,iPieNum,iFriendNum,i;double dArea[10000],dMax,dMin,dMid;
	const double PI=acos(-1);
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d %d",&iPieNum,&iFriendNum);
		for(i=0;i<iPieNum;i++){scanf("%lf",&dArea[i]);dArea[i]*=dArea[i];}
		dMax=1e8;dMin=0.0;
		while(dMax-dMin>1e-6)
		{
			dMid=(dMax+dMin)/2.0;
			if(Count(dMid,dArea,iPieNum)>=iFriendNum+1)dMin=dMid;
			else dMax=dMid;
		}
		printf("%.4lf\n",(dMax+dMin)/2.0*PI);
	}
}