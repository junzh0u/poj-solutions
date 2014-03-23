#include<stdio.h>

void main()
{
	int iN,i;double dTemp,dResult;
	scanf("%d%lf",&iN,&dTemp);
	dResult=iN*dTemp;//printf("%.2lf\n",dResult);
	scanf("%lf",&dTemp);
	dResult+=dTemp;//printf("%.2lf\n",dResult);
	for(i=iN;i>0;i--)
	{
		scanf("%lf",&dTemp);
		dResult-=2*i*dTemp;
	}
	printf("%.2lf\n",dResult/(iN+1));
}