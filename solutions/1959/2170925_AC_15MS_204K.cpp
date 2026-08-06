#include<iostream.h>
#include<stdlib.h>
#include<string.h>

int iMemory[181][4][62];
int iScore[62];

int IntCmp(const void* pLeft,const void* pRight)
{return *(int*)pRight-*(int*)pLeft;}

int Count(int iTotal,int iLeft,int iLimit)
{
	int i;
	if(iMemory[iTotal][iLeft][iLimit]>=0)
		return iMemory[iTotal][iLeft][iLimit];
	if(iTotal==0)
		return 1;
	if(i==62 || iLeft==0)
		return (iTotal==0)?1:0;
	iMemory[iTotal][iLeft][iLimit]=0;
	for(i=iLimit;i<62;i++)
		if(iTotal>=iScore[i])
			iMemory[iTotal][iLeft][iLimit]+=Count(iTotal-iScore[i],iLeft-1,i);
	return iMemory[iTotal][iLeft][iLimit];
}

void main()
{
	int i,j,iCaseNum,iTotal;
	iScore[0]=25;iScore[61]=50;
	for(i=1;i<=20;i++)
	{
		iScore[i]=i;
		iScore[i+20]=2*i;
		iScore[i+40]=3*i;
	}
	qsort(iScore,62,sizeof(int),IntCmp);
	memset(iMemory,-1,sizeof(iMemory));
	//for(i=1;i<181;i++)
	//	cout<<Count(i,3,0)<<endl;

	cin>>iCaseNum;
	for(i=1;i<=iCaseNum;i++)
	{
		cin>>iTotal;
		cout<<"Scenario #"<<i<<":\n"<<Count(iTotal,3,0)<<"\n"<<endl;
	}
}