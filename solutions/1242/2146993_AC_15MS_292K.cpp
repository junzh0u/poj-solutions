#include<stdio.h>
#include<stdlib.h>

int iSize,iPlug[10000][2],iSocket[10000][2],iPairNum,iPair[10000][2];

int Connect()
{
	int i,iLength=0;
	for(i=0;i<iPairNum;i++)
		iLength+=abs(iPlug[iPair[i][0]-1][0]-iSocket[iPair[i][1]-1][0])+abs(iPlug[iPair[i][0]-1][1]-iSocket[iPair[i][1]-1][1]);
	return iLength;
}

void Initialize()
{
	for(int i=0;i<iSize*iSize;i++)
	{iPlug[i][0]=iSocket[i][0]=i/iSize;iPlug[i][1]=iSocket[i][1]=i%iSize;}
}

void Rotate()
{
	int i,iTemp;
	for(i=0;i<iSize*iSize;i++)
	{iTemp=iPlug[i][0];iPlug[i][0]=iPlug[i][1];iPlug[i][1]=iSize-1-iTemp;}
}

void Flip(){for(int i=0;i<iSize*iSize;i++)iPlug[i][1]=iSize-1-iPlug[i][1];}

void main()
{
	int i,iLength,iCaseCount=0;
	double dAverage;
	while(scanf("%d",&iSize))
	{
		if(iSize==0)break;
		scanf("%d",&iPairNum);
		for(i=0;i<iPairNum;i++)scanf("%d%d",&iPair[i][0],&iPair[i][1]);
		dAverage=10000000;Initialize();
		for(i=0;i<4;i++){iLength=Connect();if(dAverage>iLength)dAverage=iLength;Rotate();}
		Flip();
		for(i=0;i<4;i++){iLength=Connect();if(dAverage>iLength)dAverage=iLength;Rotate();}
		printf("Scenario %d: smallest average = %.4lf\n\n",++iCaseCount,dAverage/iPairNum+1.0);
	}
}