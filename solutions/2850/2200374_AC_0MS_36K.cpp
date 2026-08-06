#include<stdio.h>
#include<math.h>

struct Point{double dX,dY;} pCentre[20];

Point Stack(Point pLeft,Point pRight)
{
	Point pTop;double dDistX,dDistY,dHalf,dHeight,dAngle;
	dDistX=pRight.dX-pLeft.dX;
	dDistY=pRight.dY-pLeft.dY;
	dAngle=atan(dDistY/dDistX);
	dHalf=sqrt(dDistX*dDistX+dDistY*dDistY)/2.0;
	dHeight=sqrt(4.0-dHalf*dHalf);
	pTop.dX=pLeft.dX+dHalf*cos(dAngle)-dHeight*sin(dAngle);
	pTop.dY=pLeft.dY+dHalf*sin(dAngle)+dHeight*cos(dAngle);
	return pTop;
}

void main()
{
	int iCaseNum,iLevel,i,j;
	scanf("%d",&iCaseNum);
	for(j=1;j<=iCaseNum;j++)
	{
		scanf("%d",&iLevel);
		for(i=0;i<iLevel;i++)
		{
			scanf("%lf",&pCentre[i].dX);
			pCentre[i].dY=1.0;
		}
		while((iLevel--)>1)
		{
			for(i=0;i<iLevel;i++)
				pCentre[i]=Stack(pCentre[i],pCentre[i+1]);
		}
		printf("%d: %.4lf %.4lf\n",j,pCentre[0].dX,pCentre[0].dY);
	}
}
