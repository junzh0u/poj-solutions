#include<stdio.h>

void main()
{
	int iCaseNum,iFloorNum,iFloor[11000],iUp,iDown,iSum,iAdd,i,iMin;
	__int64 iLeft[11000],iRight[11000];
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d%d%d",&iFloorNum,&iDown,&iUp);
		for(i=0;i<iFloorNum;i++)
			scanf("%d",&iFloor[i]);
		iLeft[0]=iAdd=0;
		iSum=iFloor[0];
		for(i=1;i<iFloorNum;i++)
		{
			iLeft[i]=iLeft[i-1]+iUp*iSum+iAdd;
			iAdd+=iSum;
			iSum+=iFloor[i];
		}
		iRight[iFloorNum-1]=iAdd=0;
		iSum=iFloor[iFloorNum-1];
		for(i=iFloorNum-2;i>=0;i--)
		{
			iRight[i]=iRight[i+1]+iDown*iSum+iAdd;
			iAdd+=iSum;
			iSum+=iFloor[i];
		}
		iMin=0;
		for(i=1;i<iFloorNum;i++)
			if(iLeft[i]+iRight[i]<iLeft[iMin]+iRight[iMin])
				iMin=i;
		printf("%d\n",iMin+1);
	}
}