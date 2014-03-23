#include<iostream>
#include<cmath>

using namespace std;

void main()
{
	int iTarget[16][3],iToMap[3],i,j,iTemp,iMinDis,iChosen;
	for(i=0;i<16;i++)
		for(j=0;j<3;j++)
			cin>>iTarget[i][j];
	for(i=0;i<3;i++)
		cin>>iToMap[i];
	while(iToMap[0]!=-1 &&  iToMap[1]!=-1 && iToMap[2]!=-1)
	{
		iTemp=0;
		iMinDis=INT_MAX;
		for(i=0;i<16;i++)
		{
			for(j=0;j<3;j++)
				iTemp+=pow((iToMap[j]-iTarget[i][j]),2);
			iTemp=sqrt(iTemp);
			if(iTemp<iMinDis)
			{
				iMinDis=iTemp;
				iChosen=i;
				if(iMinDis==0)
					break;
			}
		}
		cout<<"("<<iToMap[0]<<","<<iToMap[1]<<","<<iToMap[2]<<") maps to (";
		cout<<iTarget[iChosen][0]<<","<<iTarget[iChosen][1]<<","<<iTarget[iChosen][2]<<")"<<endl;
		for(i=0;i<3;i++)
			cin>>iToMap[i];
	}
}