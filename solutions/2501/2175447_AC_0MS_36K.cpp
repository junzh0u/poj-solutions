#include<stdio.h>

void main()
{
	char cLine[20];
	double dChangeTime=0.0,dSpeed=0.0,dNewSpeed,dChangeDistance=0.0;
	int iHour,iMinute,iSecond;
	while(gets(cLine))
	{
		if(cLine[8]==' ')
		{
			sscanf(cLine,"%d:%d:%d %lf",&iHour,&iMinute,&iSecond,&dNewSpeed);
			dChangeDistance+=dSpeed*(iHour+iMinute/60.0+iSecond/3600.0-dChangeTime);
			dChangeTime=iHour+iMinute/60.0+iSecond/3600.0;
			dSpeed=dNewSpeed;
			//printf("%lf %lf %lf\n",dChangeTime,dChangeDistance,dSpeed);
		}
		else
		{
			sscanf(cLine,"%d:%d:%d",&iHour,&iMinute,&iSecond);
			//printf("%lf %lf %lf\n",
			printf("%s %.2lf km\n",cLine,(iHour+iMinute/60.0+iSecond/3600.0-dChangeTime)*dSpeed+dChangeDistance);
		}
	}
}