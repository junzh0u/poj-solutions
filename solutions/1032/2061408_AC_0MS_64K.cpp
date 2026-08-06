#include<iostream.h>

void main()
{
	int iTotal,iSum=0,iMax,i;
	cin>>iTotal;
	for(iMax=2;iSum<=iTotal;iMax++)
		iSum+=iMax;
	if(iSum==iTotal+1)
	{
		for(i=3;i<iMax-1;i++)
			cout<<i<<" ";
		cout<<iMax<<endl;
	}
	else
	{
		for(i=2;i<iSum-iTotal;i++)
			cout<<i<<" ";
		for(i;i<iMax-1;i++)
			cout<<i+1<<" ";
		cout<<endl;
	}
}