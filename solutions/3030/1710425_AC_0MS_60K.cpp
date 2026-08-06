#include<iostream.h>

void main()
{
	int iCaseNum,i,iBefore,iAfter,iAdv;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iBefore>>iAfter>>iAdv;
		if(iAfter>iBefore+iAdv)
			cout<<"advertise"<<endl;
		else if(iAfter==iBefore+iAdv)
			cout<<"does not matter"<<endl;
		else
			cout<<"do not advertise"<<endl;
	}
}
