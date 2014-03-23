#include<iostream.h>

void main()
{
	int iCaseNum,i,iX,iY;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iX>>iY;
		if(iX==iY)
			cout<<(iX/2)*4+iX%2<<endl;
		else if(iX-iY==2)
			cout<<(iX/2)*4+iX%2-2<<endl;
		else
			cout<<"No Number"<<endl;
	}
}