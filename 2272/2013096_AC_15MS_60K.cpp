#include<stdio.h>
#include<stdlib.h>

int DartPoint(double dX,double dY)
{
	if(dX*dX+dY*dY<=9.0)
		return 100;
	if(dX*dX+dY*dY<=36.0)
		return 80;
	if(dX*dX+dY*dY<=81.0)
		return 60;
	if(dX*dX+dY*dY<=144.0)
		return 40;
	if(dX*dX+dY*dY<=225.0)
		return 20;
	return 0;
}

void main()
{
	double dX,dY;
	int iPointA,iPointB,i;
	while(1)
	{
		iPointA=iPointB=0;
		for(i=0;i<3;i++)
		{
			scanf("%lf %lf",&dX,&dY);
			if(!i && dX==-100.0)
				exit(0);
			iPointA+=DartPoint(dX,dY);
		}
		for(i=0;i<3;i++)
		{
			scanf("%lf %lf",&dX,&dY);
			iPointB+=DartPoint(dX,dY);
		}
		printf("SCORE: %d to %d, ",iPointA,iPointB);
		if(iPointA==iPointB)
			printf("TIE.\n");
		else
			printf("PLAYER %d WINS.\n",(iPointA>iPointB)?1:2);
	}
}