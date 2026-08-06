#include<iostream.h>

bool calD(int,int[]);

void main()
{
	int iJudge[10001],i;
	for(i=1;i<=10000;i++)
		iJudge[i]=1;
	for(i=1;i<=10000;i++)
		calD(i,iJudge);
	for(i=1;i<=10000;i++)
		if(iJudge[i])
			cout<<i<<endl;
}

bool calD(int iGen,int iJudge[])
{
	int iResult,i;
	iResult=iGen;
	for(i=0;;i++)
	{
		iResult+=iGen%10;
		iGen/=10;
		if(iGen==0)
			break;
	}
	if(iResult>10000)
		return false;
	else
	{
		iJudge[iResult]=0;
		calD(iResult,iJudge);
		return true;
	}
}