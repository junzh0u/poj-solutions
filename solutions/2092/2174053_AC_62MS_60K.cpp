#include<stdio.h>
#include<string.h>

void Swap(int* piLeft,int* piRight)
{
	int iTemp=*piLeft;
	*piLeft=*piRight;
	*piRight=iTemp;
}

void main()
{
	int iRankNum,iPlayerNum,iCode,iCount[10000],i,j,iMax,iSecond;
	while(scanf("%d%d",&iRankNum,&iPlayerNum) && iRankNum!=0)
	{
		memset(iCount,0,sizeof(iCount));
		for(i=0;i<iRankNum;i++)
			for(j=0;j<iPlayerNum;j++)
			{
				scanf("%d",&iCode);iCount[iCode-1]++;
			}
		/*iMax=iCount[0];
		for(i=0;i<10000;i++)
			if(iCount[i]>iMax)
				iMax=iCount[i];
		iSecond=0;
		for(i=0;i<10000;i++)
			if(iCount[i]!=iMax && iCount[i]>iSecond)
				iSecond=iCount[i];*/
		iMax=iCount[0];iSecond=iCount[1];
		if(iMax<iSecond)Swap(&iMax,&iSecond);
		for(i=2;i<10000;i++)
		{
			if(iCount[i]>iMax)
			{
				iSecond=iMax;
				iMax=iCount[i];
			}
			else if(iCount[i]>iSecond)
				iSecond=iCount[i];
		}
		for(i=0;i<10000;i++)
			if(iCount[i]==iSecond)
				printf("%d ",i+1);
		printf("\n");
	}
}