#include<iostream>
using namespace std;

void main()
{
	int iNum,iArray[50001],i,j;
	cin>>iNum;
	for(i=1;i<=iNum;i++)
		iArray[i]=i;
	for(i=2;i<iNum;i++)
	{
		swap(iArray[i],iArray[1]);
		for(j=i;j>1;j/=2)
			swap(iArray[j],iArray[j/2]);
	}
	swap(iArray[iNum],iArray[1]);
	for(i=1;i<=iNum;i++)
		cout<<iArray[i]<<" ";
	cout<<endl;
}