#include<iostream.h>

void main()
{
	int iA,iB,iC,iD,iE,iF,iG,iH,iI,iFi[1100],iCaseNum,i;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iA>>iB>>iC>>iD>>iE>>iF>>iG>>iH>>iI;
		iFi[0]=iA;
		iFi[1]=iB;
		iFi[2]=iC;
		for(i=3;i<=iI;i++)
		{
			if(i%2)
				iFi[i]=(((iD*iFi[i-1]+iE*iFi[i-2]-iF*iFi[i-3])%iG)+iG)%iG;
			else
				iFi[i]=(((iF*iFi[i-1]-iD*iFi[i-2]+iE*iFi[i-3])%iH)+iH)%iH;
		}
		cout<<iFi[iI]<<endl;
	}
}