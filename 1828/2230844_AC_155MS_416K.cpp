#include<stdio.h>
#include<stdlib.h>
#define MAX 50000

struct Location{int iX,iY;} lList[MAX];

int LocCmp(const void* pLeft,const void* pRight)
{
	if(((Location*)pLeft)->iX!=((Location*)pRight)->iX)
		return ((Location*)pRight)->iX-((Location*)pLeft)->iX;
	else return ((Location*)pRight)->iY-((Location*)pLeft)->iY;
}

void main()
{
	int iMonkeyNum,i,j,iCount=0,iMax;
	while(scanf("%d",&iMonkeyNum) && iMonkeyNum>0)
	{
		for(i=0;i<iMonkeyNum;i++)
			scanf("%d%d",&lList[i].iX,&lList[i].iY);
		qsort(lList,iMonkeyNum,sizeof(Location),LocCmp);
		for(i=1,iMax=lList[0].iY,iCount=1;i<iMonkeyNum;i++)
			if(lList[i].iY>iMax)
			{
				iCount++;
				iMax=lList[i].iY;
			}
		printf("%d\n",iCount);
	}
}