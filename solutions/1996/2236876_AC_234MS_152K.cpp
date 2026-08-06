#include<iostream.h>
#include<string.h>
#define MAX 100

void main()
{
	int iCaseNum,iDegreeP,iDegreeQ,iCoefP[MAX+1],iCoefQ[MAX+1],iCoef[MAX*MAX+1],iTemp[2][MAX*MAX+1],i,j,k;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iDegreeP>>iDegreeQ;
		for(i=0;i<=iDegreeP;i++)cin>>iCoefP[i];for(i=0;i<=iDegreeQ;i++)cin>>iCoefQ[i];
		memset(iCoef,0,sizeof(iCoef));memset(iTemp,0,sizeof(iTemp));iTemp[0][0]=1;
		for(i=0;i<=iDegreeP;i++)
		{
			for(j=0;j<=i*iDegreeQ;j++)iCoef[j]+=iCoefP[i]*iTemp[0][j];
			memset(iTemp[1],0,sizeof(iTemp[1]));
			for(j=0;j<=(i+1)*iDegreeQ;j++)for(k=0;k<=iDegreeQ && k<=j;k++)
				iTemp[1][j]+=iCoefQ[k]*iTemp[0][j-k];
			memcpy(iTemp[0],iTemp[1],sizeof(iTemp[0]));
		}
		for(i=0;i<=iDegreeP*iDegreeQ;i++)cout<<iCoef[i]<<" ";
		cout<<endl;
	}
}