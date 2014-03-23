#include<iostream.h>
#include<string.h>
#define INF 1000000

int iFlowerNum,iVaseNum,iValue[100][100],iMemory[100][100];

int Max(int iLeftFlower,int iLeftVase)
{
	int iSum=0,i,iTemp;
	if(iLeftFlower==0)
		return 0;
	if(iMemory[iLeftFlower][iLeftVase]<INF)
		return iMemory[iLeftFlower][iLeftVase];
	if(iLeftFlower==iLeftVase)
		for(i=1;i<=iLeftFlower;i++)
			iSum+=iValue[iFlowerNum-i][iVaseNum-i];
	else
	{
		iSum=iValue[iFlowerNum-iLeftFlower][iVaseNum-iLeftVase]+Max(iLeftFlower-1,iLeftVase-1);
		iTemp=Max(iLeftFlower,iLeftVase-1);
		if(iSum<iTemp)iSum=iTemp;
	}
	return iMemory[iLeftFlower][iLeftVase]=iSum;
}

void main()
{
	int i,j;
	cin>>iFlowerNum>>iVaseNum;
	for(i=0;i<iFlowerNum;i++)
		for(j=0;j<iVaseNum;j++)
			cin>>iValue[i][j];
	memset(iMemory,INF,sizeof(iMemory));
	cout<<Max(iFlowerNum,iVaseNum)<<endl;
}