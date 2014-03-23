#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int StringCompare(const void* pLeft,const void* pRight){return strcmp((char*)pLeft,(char*)pRight);}

void main()
{
	char cSequence[20000][21];
	int iPeopleNum,iLength,iCopy[20000],iCount,i;
	while(scanf("%d%d",&iPeopleNum,&iLength))
	{
		if(iPeopleNum==0)break;
		for(i=0;i<iPeopleNum;i++)scanf("%s",cSequence[i]);
		qsort(cSequence,iPeopleNum,sizeof(char)*21,StringCompare);
		iCount=0;
		memset(iCopy,0,sizeof(int)*iPeopleNum);
		for(i=1;i<iPeopleNum;i++)
		{
			if(strcmp(cSequence[i],cSequence[i-1])==0)iCount++;
			else
			{
				iCopy[iCount]++;
				iCount=0;
			}
		}
		iCopy[iCount]++;
		for(i=0;i<iPeopleNum;i++)printf("%d\n",iCopy[i]);
	}
}