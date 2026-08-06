#include<iostream>

using namespace std;

void main()
{
	int iStuNum,iDimX,iDimY,iDimZ,i,iMinClay,iMaxClay,j;
	char cMinName[10],cMaxName[10],cTemp[10];
	while(1)
	{
		cin>>iStuNum;
		if(iStuNum==-1)
			break;
		iMinClay=INT_MAX;
		iMaxClay=0;
		for(i=0;i<iStuNum;i++)
		{
			cin>>iDimX>>iDimY>>iDimZ>>cTemp;
			if(iDimX*iDimY*iDimZ<iMinClay)
			{
				iMinClay=iDimX*iDimY*iDimZ;
				for(j=0;;j++)
				{
					cMinName[j]=cTemp[j];
					if(cMinName[j]=='\0')
						break;
				}
			}
			if(iDimX*iDimY*iDimZ>iMaxClay)
			{
				iMaxClay=iDimX*iDimY*iDimZ;
				for(j=0;;j++)
				{
					cMaxName[j]=cTemp[j];
					if(cMaxName[j]=='\0')
						break;
				}
			}
		}
		cout<<cMaxName<<" took clay from "<<cMinName<<"."<<endl;
	}
}