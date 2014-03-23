#include<stdio.h>

#define DOWN -1
#define UP 1
#define LIMIT 20

_int64 iDown[LIMIT+1][LIMIT+1]={0},iUp[LIMIT+1][LIMIT+1]={0};
int iSeq[LIMIT+1];

void NextPla(int iStart,_int64 iCode,int iDir,int iPlaNum,int iIniPla);

void main()
{
	_int64 iCata;
	int iN,iI,iCaseNum,iPlaNum,i,iIniDir,iFirst;
	iUp[1][1]=iDown[1][1]=1;
	for(iN=2;iN<=LIMIT;iN++)
		for(iI=2;iI<=iN;iI++)
			iDown[iI][iN]=iDown[iI-1][iN]+iDown[iN+1-iI][iN-1];
	for(iN=1;iN<=LIMIT;iN++)
		for(iI=1;iI<=iN;iI++)
			iUp[iI][iN]=iDown[iN+1-iI][iN];
	iDown[1][1]=0;
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d%I64d",&iPlaNum,&iCata);
		for(i=0;i<=iPlaNum;i++)
			iSeq[i]=i;
		for(i;i<=LIMIT;i++)
			iSeq[i]=0;
		iFirst=0;
		while(iCata>0 && iFirst<iPlaNum)
		{
			iFirst++;
			iCata-=iDown[iFirst][iPlaNum];
			iCata-=iUp[iFirst][iPlaNum];
		}
		iCata+=iUp[iFirst][iPlaNum];
		if(iCata>0)
		{
			if(iFirst==iPlaNum)
				iIniDir=DOWN;
			else
				iIniDir=UP;
		}
		else
		{
			iCata+=iDown[iFirst][iPlaNum];
			iIniDir=DOWN;
		}
		iSeq[iFirst]=0;
		for(i=iFirst+1;i<=iPlaNum;i++)
			iSeq[i]--;
		printf("%d ",iFirst);
		if(iPlaNum>1)
			NextPla(iFirst,iCata,iIniDir*(-1),iPlaNum-1,iPlaNum);
		printf("\n");
	}
}

void NextPla(int iStart,_int64 iCode,int iDir,int iPlaNum,int iIniPla)
{
	int iNext,i;
	if(iPlaNum==1)
		for(iNext=1;iNext<=iIniPla;iNext++)
			if(iSeq[iNext])
			{
				printf("%d ",iNext);
				return;
			}
	if(iDir==DOWN)
		iNext=iStart;
	else
		iNext=0;
	while(iCode>0)
	{
		do
		{
			iNext++;
		}while(!iSeq[iNext]);
		iCode-=((iDir==UP)?(iUp[iSeq[iNext]][iPlaNum]):(iDown[iSeq[iNext]][iPlaNum]));
	}
	iCode+=((iDir==UP)?(iUp[iSeq[iNext]][iPlaNum]):(iDown[iSeq[iNext]][iPlaNum]));
	iSeq[iNext]=0;
	for(i=iNext+1;i<=iIniPla;i++)
		if(iSeq[i])
			iSeq[i]--;
	printf("%d ",iNext);
	NextPla(iNext,iCode,iDir*(-1),iPlaNum-1,iIniPla);
}