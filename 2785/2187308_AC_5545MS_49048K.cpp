#include<cstdio>
#include<algorithm>
using namespace std;
#define MAX 4000

int iSize,iValue[MAX][4],iLeft[MAX*MAX],iRight[MAX*MAX];

void main()
{
	int i,j,iCount=0,iCon;
	scanf("%d",&iSize);
	for(i=0;i<iSize;i++)
		for(j=0;j<4;j++)
			scanf("%d",&iValue[i][j]);
	for(i=0;i<iSize;i++)
		for(j=0;j<iSize;j++)
		{
			iLeft[i*iSize+j]=iValue[i][0]+iValue[j][1];
			iRight[i*iSize+j]=iValue[i][2]+iValue[j][3];
		}
	sort(iLeft,iLeft+iSize*iSize);
	sort(iRight,iRight+iSize*iSize);
	for(i=0,j=iSize*iSize-1;i<iSize*iSize;i++)
	{
		if(i!=0 && iLeft[i]==iLeft[i-1]){iCount+=iCon;continue;}
		iCon=0;
		while(j>=0 && iLeft[i]+iRight[j]>0)j--;
		while(j>=0 && iLeft[i]+iRight[j]==0){iCount++;iCon++;j--;}
	}
	printf("%d\n",iCount);
}