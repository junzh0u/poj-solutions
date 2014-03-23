#include<iostream.h>
#define MAX 1100
#define INF 200000000

void main()
{
	int iCaseNum,iClassNum,iNum[MAX],iSum[MAX],iPrice[MAX],iBest[MAX],i,j;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iClassNum;
		iSum[0]=0;
		for(i=1;i<=iClassNum;i++)
		{
			cin>>iNum[i]>>iPrice[i];
			iSum[i]=iSum[i-1]+iNum[i];
		}
		iBest[0]=0;
		for(i=1;i<=iClassNum;i++)
		{
			iBest[i]=INF;
			for(j=0;j<i;j++)
				if(iBest[i]>iBest[j]+(iSum[i]-iSum[j]+10)*iPrice[i])
					iBest[i]=iBest[j]+(iSum[i]-iSum[j]+10)*iPrice[i];
		}
		cout<<iBest[iClassNum]<<endl;
	}
}