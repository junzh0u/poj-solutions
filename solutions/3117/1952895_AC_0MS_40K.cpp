#include<iostream.h>

void main()
{
	int iTeamNum,iMatchNum,iPoint,iTotalPoint;
	char cTeamName[12];
	while(1)
	{
		cin>>iTeamNum>>iMatchNum;
		if(!iTeamNum)
			break;
		iTotalPoint=0;
		while(iTeamNum--)
		{
			cin>>cTeamName>>iPoint;
			iTotalPoint+=iPoint;
		}
		cout<<3*iMatchNum-iTotalPoint<<endl;
	}
}