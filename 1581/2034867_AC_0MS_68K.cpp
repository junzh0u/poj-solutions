#include<iostream.h>

#define MAX 100

void main()
{
	char cTeamName[MAX][50];
	int iTeamNum,iSub,iTime,iProNum[MAX],iPoint[MAX],i,j,iWinner;
	cin>>iTeamNum;
	iProNum[0]=iWinner=0;
	for(i=1;i<=iTeamNum;i++)
	{
		cin>>cTeamName[i];
		iProNum[i]=iPoint[i]=0;
		for(j=0;j<4;j++)
		{
			cin>>iSub>>iTime;
			if(iTime>0)
			{
				iProNum[i]++;
				iPoint[i]+=iTime+(iSub-1)*20;
			}
			if(iProNum[i]>iProNum[iWinner] || (iProNum[i]==iProNum[iWinner] && iPoint[i]<iPoint[iWinner]))
				iWinner=i;
		}
	}
	cout<<cTeamName[iWinner]<<" "<<iProNum[iWinner]<<" "<<iPoint[iWinner]<<endl;
}