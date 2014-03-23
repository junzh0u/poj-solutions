#include<iostream.h>
#include<ctype.h>
#include<stdlib.h>

void main()
{
	long iCaseNum,iHour,iMinute,iSecond,iDay,iMonth,iYear,i,iDayMinute=64,iDayHour=8;
	long iYearForm[400],iYearSum,iMonthForm[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	char cTime[9],cDate[11],*pcBegin;
	iYearSum=0;
	for(i=0;i<400;i++)
	{
		if((i%4==0 && i%100) || i%400==0)
			iYearForm[i]=366;
		else
			iYearForm[i]=365;
		iYearSum+=iYearForm[i];
	}
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>cTime>>cDate;
		pcBegin=cTime;
		iHour=atoi(pcBegin);
		while(isdigit(*pcBegin))
			pcBegin++;
		pcBegin++;
		iMinute=atoi(pcBegin);
		while(isdigit(*pcBegin))
			pcBegin++;
		pcBegin++;
		iSecond=atoi(pcBegin);
		pcBegin=cDate;
		iDay=atoi(pcBegin);
		while(isdigit(*pcBegin))
			pcBegin++;
		pcBegin++;
		iMonth=atoi(pcBegin);
		while(isdigit(*pcBegin))
			pcBegin++;
		pcBegin++;
		iYear=atoi(pcBegin)-2000;
		iSecond+=iHour*3600+iMinute*60;
		iSecond=iSecond*(10.0*100.0*100.0/(24.0*3600.0));
		iMinute=iSecond/100;
		iSecond%=100;
		iHour=iMinute/100;
		iMinute%=100;
		iDay+=iYear/400*iYearSum;
		iYear%=400;
		iMonthForm[1]=iYearForm[iYear]-337;
		for(i=0;i<iYear;i++)
			iDay+=iYearForm[i];
		for(i=0;i<iMonth-1;i++)
			iDay+=iMonthForm[i];
		iMonth=(iDay-1)/100+1;
		iDay=(iDay-1)%100+1;
		iYear=(iMonth-1)/10;
		iMonth=(iMonth-1)%10+1;
		cout<<iHour<<":"<<iMinute<<":"<<iSecond<<" "<<iDay<<"."<<iMonth<<"."<<iYear<<endl;
	}
}