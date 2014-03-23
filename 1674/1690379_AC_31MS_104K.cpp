#include<iostream.h>

void main()
{
	short iCaseNum,iDataNum,iData[10001],iPos[10001],i,j,iCount;
	//short iTemp;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iDataNum;
		for(j=1;j<=iDataNum;j++)
		{
			cin>>iData[j];
			iPos[iData[j]]=j;
		}
		iCount=0;
		for(j=1;j<=iDataNum-1;j++)
			if(iData[j]!=j)
			{
				//iTemp=iData[iPos[j]];
				iData[iPos[j]]=iData[j];
				//iData[j]=iData[iPos[j]];
				iPos[iData[j]]=iPos[j];
				iCount++;
			}
		cout<<iCount<<endl;
	}
}
