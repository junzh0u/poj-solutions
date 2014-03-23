#include<iostream.h>

void main()
{
	int iCaseNum,iN,i;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iN;
		cout<<(iN/2)*((iN-1)/2)<<endl;
	}
}