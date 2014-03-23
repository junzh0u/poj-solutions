#include<iostream>

using namespace std;

void main()
{
	const double DISTANCE=4.5;
	int iSpeed,iStartTime,iRiderNum,iTempTime,iMinTime,i;
	double dTempTime;
	while(1)
	{
		iMinTime=INT_MAX;
		cin>>iRiderNum;
		if(iRiderNum==0)
			break;
		for(i=0;i<iRiderNum;i++)
		{
			cin>>iSpeed>>iStartTime;
			if(iStartTime>=0)
			{
				dTempTime=iStartTime+3600.0*DISTANCE/iSpeed;
				iTempTime=dTempTime;
				if(dTempTime-iTempTime!=0.0)
					iTempTime++;
				if(iTempTime<iMinTime)
					iMinTime=iTempTime;
			}
		}
		cout<<iMinTime<<endl;
	}
}