#include<iostream.h>

void main()
{
	int iCaseNum,i,iSum;
	cin>>iCaseNum;
	for(i=1;i<=iCaseNum;i++)
	{
		cin>>iSum;
		cout<<i<<" "<<iSum/25<<" QUARTER(S), ";
		iSum%=25;
		cout<<iSum/10<<" DIME(S), ";
		iSum%=10;
		cout<<iSum/5<<" NICKEL(S), ";
		iSum%=5;
		cout<<iSum<<" PENNY(S)"<<endl;
	}
}