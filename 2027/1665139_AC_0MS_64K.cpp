#include<iostream.h>

void main()
{
	int iCaseNum,i,iX,iY;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iX>>iY;
		cout<<((iX<iY)?("NO BRAINS"):("MMM BRAINS"))<<endl;
	}
}