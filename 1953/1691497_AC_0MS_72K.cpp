#include<iostream.h>

void main()
{
	int iResult[45],iCaseNum,i,iLength;
	iResult[0]=2;
	iResult[1]=3;
	for(i=2;i<45;i++)
		iResult[i]=iResult[i-1]+iResult[i-2];
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iLength;
		cout<<"Scenario #"<<i+1<<":\n"<<iResult[iLength-1]<<"\n"<<endl;
	}
}