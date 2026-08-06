#include<iostream.h>

void main()
{
	int iDecimal,iResult,iTemp,iDigit;
	while(1)
	{
		cin>>iDecimal;
		if(!iDecimal)
			break;
		iTemp=iDecimal;
		iResult=0;
		iDigit=1;
		while(iTemp)
		{
			iResult+=((iTemp%10>4)?(iTemp%10-1):(iTemp%10))*iDigit;
			iTemp/=10;
			iDigit*=9;
		}
		cout<<iDecimal<<": "<<iResult<<endl;
	}
}