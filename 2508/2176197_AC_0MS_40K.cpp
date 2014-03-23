#include<stdio.h>
#include<math.h>
#define PI 3.1415926

void main()
{
	double dRadius,dHeight,dDistA,dAngleA,dDistB,dAngleB,dRatio,dSectorAngle;
	while(scanf("%lf%lf%lf%lf%lf%lf",&dRadius,&dHeight,&dDistA,&dAngleA,&dDistB,&dAngleB)!=EOF)
	{
		dRatio=dRadius/sqrt(dRadius*dRadius+dHeight*dHeight);
		dSectorAngle=fabs(dAngleA-dAngleB);
		if(dSectorAngle>180)dSectorAngle=360-dSectorAngle;
		dSectorAngle*=PI*dRatio/180.0;
		printf("%.2lf\n",sqrt(dDistA*dDistA+dDistB*dDistB-2*dDistA*dDistB*cos(dSectorAngle)));
	}
}