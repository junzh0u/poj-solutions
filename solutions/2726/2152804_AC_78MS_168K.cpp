#include<stdio.h>
#include<stdlib.h>
#define MAX 10000

struct Hotel{int iDistance,iCost,iSum;bool bCandidate;} hList[10000];

int HotelCompare(const void* pLeft,const void* pRight)
{
	if(((Hotel*)pLeft)->iDistance!=((Hotel*)pRight)->iDistance)
		return ((Hotel*)pLeft)->iDistance-((Hotel*)pRight)->iDistance;
	else
		return ((Hotel*)pLeft)->iCost-((Hotel*)pRight)->iCost;
}

void main()
{
	int iHotelNum,i,iCount,iLast;
	while(scanf("%d",&iHotelNum))
	{
		if(iHotelNum==0)break;
		for(i=0;i<iHotelNum;i++)
		{
			scanf("%d%d",&hList[i].iDistance,&hList[i].iCost);
			hList[i].iSum=hList[i].iCost+hList[i].iDistance;
			hList[i].bCandidate=true;
		}
		qsort(hList,iHotelNum,sizeof(Hotel),HotelCompare);
		iLast=0;iCount=1;
		for(i=1;i<iHotelNum;i++)
			if(hList[i].iDistance>hList[iLast].iDistance && hList[i].iCost<hList[iLast].iCost)
			{
				iLast=i;
				iCount++;
			}
		printf("%d\n",iCount);
	}
}