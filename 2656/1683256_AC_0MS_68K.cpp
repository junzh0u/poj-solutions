#include<iostream.h>

void main()
{
	int iDayNum,i,iSchHour,iSupHour,iMax,iResult;
	while(1)
	{
		cin>>iDayNum;
		if(!iDayNum)
			break;
		iResult=iMax=0;
		for(i=0;i<iDayNum;i++)
		{
			cin>>iSchHour>>iSupHour;
			if(iSchHour+iSupHour-8>iMax)
			{
				iMax=iSchHour+iSupHour-8;
				iResult=i+1;
			}
		}
		cout<<iResult<<endl;
	}
}