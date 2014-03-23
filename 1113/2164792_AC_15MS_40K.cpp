#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define PI 3.1415926

struct Point{int iX,iY;bool bUsed;} pVertices[1000];

int PointCmp(const void* pLeft,const void* pRight)
{
	if(((Point*)pLeft)->iX!=((Point*)pRight)->iX)
		return (((Point*)pLeft)->iX-((Point*)pRight)->iX);
	else return ((Point*)pLeft)->iY-((Point*)pRight)->iY;
}

int Det(int iLeftX,int iLeftY,int iRightX,int iRightY)
{return iLeftX*iRightY-iLeftY*iRightX;}

int Area(Point pA,Point pB,Point pC)
{return Det(pB.iX-pA.iX,pB.iY-pA.iY,pC.iX-pA.iX,pC.iY-pA.iY);}

double Distance(Point pA,Point pB)
{return sqrt((pA.iX-pB.iX)*(pA.iX-pB.iX)+(pA.iY-pB.iY)*(pA.iY-pB.iY));}

void main()
{
	int iVertNum,iRadius,i,iLeftDeque[1000],iLeftCount,iRightDeque[1000],iRightCount;
	double dLength=0.0;
	scanf("%d%d",&iVertNum,&iRadius);
	for(i=0;i<iVertNum;i++)
	{
		scanf("%d%d",&pVertices[i].iX,&pVertices[i].iY);
		pVertices[i].bUsed=false;
	}
	qsort(pVertices,iVertNum,sizeof(Point),PointCmp);
	iLeftDeque[0]=0;iLeftCount=1;i=1;
	while(i<iVertNum)
	{
		if(iLeftCount==1 || Area(pVertices[iLeftDeque[iLeftCount-2]],pVertices[iLeftDeque[iLeftCount-1]],pVertices[i])>=0)
		{
			iLeftDeque[iLeftCount++]=i;
			pVertices[i++].bUsed=true;
		}
		else pVertices[iLeftDeque[--iLeftCount]].bUsed=false;
	}
	iRightDeque[0]=iVertNum-1;iRightCount=1;i=iVertNum-2;
	while(i>=0)
	{
		if(pVertices[i].bUsed)i--;
		else if(iRightCount==1 || Area(pVertices[iRightDeque[iRightCount-2]],pVertices[iRightDeque[iRightCount-1]],pVertices[i])>=0)
			iRightDeque[iRightCount++]=i--;
		else iRightCount--;
	}
	for(i=1;i<iLeftCount;i++)
		dLength+=Distance(pVertices[iLeftDeque[i-1]],pVertices[iLeftDeque[i]]);
	for(i=1;i<iRightCount;i++)
		dLength+=Distance(pVertices[iRightDeque[i-1]],pVertices[iRightDeque[i]]);
	dLength+=2*PI*iRadius;
	printf("%.0lf\n",dLength);
}