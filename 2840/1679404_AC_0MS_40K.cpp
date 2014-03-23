#include<iostream.h>

void main()
{
	int iHour,iCaseNum,i,j;
	char cString[6];
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>cString;
		for(j=0;cString[j]!=':';j++);
		if(cString[j+1]=='0' && cString[j+2]=='0')
		{
			if(j==1)
				iHour=cString[0]-'0';
			else
				iHour=(cString[0]-'0')*10+cString[1]-'0';
			cout<<((iHour>12)?(iHour-12):(iHour+12))<<endl;
		}
		else
			cout<<0<<endl;
	}
}