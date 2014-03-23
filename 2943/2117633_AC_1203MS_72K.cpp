#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Material{int iDetectNum;bool bDetect[300];};
Material mTest[200];
int iDiseaseNum,iTestNum;
bool bTotal[300],bUsed[200];

int MaterialCompare(const void* pLeft,const void* pRight)
{return ((Material*)pLeft)->iDetectNum-((Material*)pRight)->iDetectNum;}

void Insert(int iNumber)
{
	int i;
	bUsed[iNumber]=true;
	for(i=0;i<iDiseaseNum;i++)
		if(mTest[iNumber].bDetect[i])
			bTotal[i]=true;
}

bool Eliminate(int iNumber)
{
	int i,j;
	bool bTestMix[200],bMix[300];
	for(i=0;i<iDiseaseNum;i++)
		if(mTest[iNumber].bDetect[i] && !bTotal[i])
		{
			Insert(iNumber);
			return false;
		}
	memcpy(bTestMix,bUsed,sizeof(bool)*iTestNum);
	for(i=0;i<iDiseaseNum;i++)
		if(!mTest[iNumber].bDetect[i])
			for(j=0;j<iNumber;j++)
				if(bTestMix[j] && mTest[j].bDetect[i])
					bTestMix[j]=false;
	memset(bMix,false,sizeof(bool)*iDiseaseNum);
	for(i=0;i<iDiseaseNum;i++)
		if(mTest[iNumber].bDetect[i])
		{
			for(j=0;j<iTestNum;j++)
				if(bTestMix[j] && mTest[j].bDetect[i])
				{
					bMix[i]=true;
					break;
				}
			if(!bMix[i])
			{
				Insert(iNumber);
				return false;
			}
		}
	return true;
}

void main()
{
	int i,j,iTemp,iCount;
	while(scanf("%d%d",&iDiseaseNum,&iTestNum))
	{
		if(iDiseaseNum==0)break;
		for(i=0;i<iTestNum;i++)
		{
			scanf("%d",&(mTest[i].iDetectNum));
			memset(mTest[i].bDetect,false,sizeof(bool)*iDiseaseNum);
			for(j=0;j<mTest[i].iDetectNum;j++)
			{
				scanf("%d",&iTemp);
				mTest[i].bDetect[iTemp-1]=true;
			}
		}
		qsort(mTest,iTestNum,sizeof(Material),MaterialCompare);
		memset(bTotal,false,sizeof(bool)*iDiseaseNum);
		memset(bUsed,false,sizeof(bool)*iTestNum);
		iCount=0;
		for(i=0;i<iTestNum;i++)if(Eliminate(i))iCount++;
		printf("%d\n",iCount);
	}
}