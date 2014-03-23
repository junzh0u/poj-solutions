#include<iostream.h>
#include<iomanip.h>

void main()
{
	int iYearForm[8000],iMonthForm[12]={31,28,31,30,31,30,31,31,30,31,30,31},i,iDay,iYear,iMonth,iWeek;
	char iWeekDayName[7][10]={"Saturday","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday"};
	for(i=0;i<8000;i++)
		if((i%4==0 && i%100) || i%400==0)
			iYearForm[i]=366;
		else
			iYearForm[i]=365;
	while(cin>>iDay)
	{
		if(iDay==-1)
			break;
		iYear=0;
		iWeek=iDay%7;
		iDay++;
		while(iDay>iYearForm[iYear])
		{
			iDay-=iYearForm[iYear];
			iYear++;
		}
		iMonthForm[1]=iYearForm[iYear]-337;
		iMonth=0;
		while(iDay>iMonthForm[iMonth])
		{
			iDay-=iMonthForm[iMonth];
			iMonth++;
		}
		cout<<setfill('0')<<iYear+2000<<"-"<<setw(2)<<iMonth+1<<"-"<<setw(2)<<iDay<<" "<<iWeekDayName[iWeek]<<endl;
	}
}