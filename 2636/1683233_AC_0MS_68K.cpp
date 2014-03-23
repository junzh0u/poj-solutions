#include<iostream.h>

void main()
{
	int iCaseNum,iStripNum,iTemp,iSum,i,j;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iStripNum;
		iSum=1;
		for(j=0;j<iStripNum;j++)
		{
			cin>>iTemp;
			iSum+=iTemp-1;
		}
		cout<<iSum<<endl;
	}
}