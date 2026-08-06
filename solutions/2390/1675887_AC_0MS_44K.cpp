#include<iostream.h>

void main()
{
	int iInterest,iYear,iIniMoney,i;
	double dMoney;
	cin>>iInterest>>iIniMoney>>iYear;
	dMoney=iIniMoney;
	for(i=0;i<iYear;i++)
		dMoney*=(1.0+iInterest/100.0);
	cout<<(int)dMoney<<endl;
}