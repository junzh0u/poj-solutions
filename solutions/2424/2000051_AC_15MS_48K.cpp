#include<stdio.h>
#include<string.h>

#define TIME_MAX 2000

bool CheckTable(int* iLeaveTime,int iTableNum,int iMin)
{
	int i,iFirstLeave=0;
	for(i=0;i<iTableNum;i++)
	{
		if(iLeaveTime[i]<iLeaveTime[iFirstLeave])
			iFirstLeave=i;
		if(iLeaveTime[i]<=iMin)
		{
			iLeaveTime[i]=iMin+30;
			return true;
		}
	}
	if(iLeaveTime[iFirstLeave]<=iMin+30)
	{
		iLeaveTime[iFirstLeave]+=30;
		return true;
	}
	return false;
}

void main()
{
	int iTwo,iFour,iSix,iTwoTime[100],iFourTime[100],iSixTime[100],iHour,iMin,iCome,iDiner;
	char cTemp;
	while(scanf("%d%d%d",&iTwo,&iFour,&iSix))
	{
		if(!iTwo)
			break;
		memset(iTwoTime,0,sizeof(iTwoTime));
		memset(iFourTime,0,sizeof(iFourTime));
		memset(iSixTime,0,sizeof(iSixTime));
		iDiner=0;
		while(scanf("%d:%d %d",&iHour,&iMin,&iCome))
			if((iCome<=2 && CheckTable(iTwoTime,iTwo,iHour*60+iMin)) || (iCome>2 && iCome<=4 && CheckTable(iFourTime,iFour,iHour*60+iMin)) || (iCome>4 && CheckTable(iSixTime,iSix,iHour*60+iMin)))
				iDiner+=iCome;
		printf("%d\n",iDiner);
		scanf("%c",&cTemp);
	}
}