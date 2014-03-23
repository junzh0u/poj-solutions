#include<iostream.h>

void main()
{
	int iCaseNum,i,j,iN,iResult;
	cin>>iCaseNum;
	for(i=1;i<=iCaseNum;i++)
	{
		cin>>iN;
		iResult=0;
		for(j=1;j<=iN;j++)
			iResult+=j*(j+2)*(j+1)/2;
		cout<<i<<" "<<iN<<" "<<iResult<<endl;
	}
}