#include<iostream>

using namespace std;

void main()
{
	int iCaseNum,iDataNum,iData[1001],i,j,iCount;
	int iTemp;
	bool bFound;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iDataNum;
		for(j=0;j<iDataNum;j++)
			cin>>iData[j];
		iCount=0;
		bFound=true;
		while(bFound)
		{
			bFound=false;
			for(j=0;j<iDataNum-1;j++)
				if(iData[j+1]<iData[j])
				{
					iTemp=iData[j];
					iData[j]=iData[j+1];
					iData[j+1]=iTemp;
					iCount++;
					bFound=true;
				}
		}
		cout<<"Scenario #"<<i+1<<":\n"<<iCount<<"\n"<<endl;
	}
}
