#include<iostream.h>
#include<math.h>

void main()
{
	int iCaseNum,iDisW,iDisL,iStep[4],i,j;
	char cAdd[2][3];
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>cAdd[0]>>cAdd[1];
		iDisW=fabs(cAdd[0][0]-cAdd[1][0]);
		iDisL=fabs(cAdd[0][1]-cAdd[1][1]);
		if(iDisW==0 && iDisL==0)
		{
			cout<<"0 0 0 0"<<endl;
			continue;
		}
		iStep[0]=(iDisW>iDisL)?iDisW:iDisL;
		iStep[1]=(iDisW*iDisL==0||iDisW==iDisL)?1:2;
		iStep[2]=(iDisW*iDisL==0)?1:2;
		if(iDisW==iDisL)
			iStep[3]=1;
		else if((iDisW-iDisL)%2)
			iStep[3]=0;
		else
			iStep[3]=2;
		for(j=0;j<3;j++)
			cout<<iStep[j]<<" ";
		if(iStep[j])
			cout<<iStep[j]<<endl;
		else
			cout<<"Inf"<<endl;
	}
}