#include<iostream.h>

int PowerModulus(int iBase,int iExponent,int iDivisor)
{
	int iRemainder=1,iTemp[40]={iBase%iDivisor},i;
	if(iExponent&1>0)iRemainder=iTemp[0];iExponent>>=1;
	for(i=1;iExponent>0;i++,iExponent>>=1)
	{
		iTemp[i]=(iTemp[i-1]*iTemp[i-1])%iDivisor;
		if(iExponent&1>0)iRemainder=(iRemainder*iTemp[i])%iDivisor;
	}
	return iRemainder;
}

void main()
{
	int iCaseNum,iDivisor,iPlayerNum,iBase,iExponent,iRemainder;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iDivisor>>iPlayerNum;iRemainder=0;
		while(iPlayerNum--)
		{
			cin>>iBase>>iExponent;
			iRemainder=(iRemainder+PowerModulus(iBase,iExponent,iDivisor))%iDivisor;
		}
		cout<<iRemainder<<endl;
	}
}