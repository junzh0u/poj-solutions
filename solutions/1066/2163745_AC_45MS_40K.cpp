#include<iostream.h>
#include<math.h>
#include<stdlib.h>

struct Point{double dX,dY;};
struct Wall{Point pBegin,pEnd;} wList[30],wTemp;

int Sign(double dValue)
{
	if(fabs(dValue)<1e-6)return 0;
	else return (dValue<0)?(-1):1;
}

double Det(double dLeftX,double dLeftY,double dRightX,double dRightY)
{return dLeftX*dRightY-dLeftY*dRightX;}

double Area(Point pA,Point pB,Point pC)
{return Det(pB.dX-pA.dX,pB.dY-pA.dY,pC.dX-pA.dX,pC.dY-pA.dY);}

bool Cut(Wall wLeft,Wall wRight)
{
	return Sign(Area(wLeft.pBegin,wLeft.pEnd,wRight.pBegin))*Sign(Area(wLeft.pBegin,wLeft.pEnd,wRight.pEnd))<0 
		&& Sign(Area(wRight.pBegin,wRight.pEnd,wLeft.pBegin))*Sign(Area(wRight.pBegin,wRight.pEnd,wLeft.pEnd))<0;
}

int DoubleComp(const void* pLeft,const void* pRight)
{return Sign(*((double*)pLeft)-*((double*)pRight));}

void main()
{
	int iWallNum,i,j,iMin=100,iCut,iLeftCount=1,iTopCount=1,iBottomCount=1,iRightCount=1;
	double dTop[32]={0.0},dBottom[32]={0.0},dLeft[32]={0.0},dRight[32]={0.0};
	cin>>iWallNum;
	for(i=0;i<iWallNum;i++)
	{
		cin>>wList[i].pBegin.dX>>wList[i].pBegin.dY>>wList[i].pEnd.dX>>wList[i].pEnd.dY;
		if(wList[i].pBegin.dX==0)dLeft[iLeftCount++]=wList[i].pBegin.dY;
		else if(wList[i].pBegin.dX==100)dRight[iRightCount++]=wList[i].pBegin.dY;
		if(wList[i].pBegin.dY==0)dBottom[iBottomCount++]=wList[i].pBegin.dX;
		else if(wList[i].pBegin.dY==100)dTop[iTopCount++]=wList[i].pBegin.dX;
		if(wList[i].pEnd.dX==0)dLeft[iLeftCount++]=wList[i].pEnd.dY;
		else if(wList[i].pEnd.dX==100)dRight[iRightCount++]=wList[i].pEnd.dY;
		if(wList[i].pEnd.dY==0)dBottom[iBottomCount++]=wList[i].pEnd.dX;
		else if(wList[i].pEnd.dY==100)dTop[iTopCount++]=wList[i].pEnd.dX;
	}
	dLeft[iLeftCount++]=dRight[iRightCount++]=dBottom[iBottomCount++]=dTop[iTopCount++]=100.0;
	cin>>wTemp.pBegin.dX>>wTemp.pBegin.dY;
	qsort(dLeft,iLeftCount,sizeof(double),DoubleComp);wTemp.pEnd.dX=0;
	for(i=1;i<iLeftCount;i++)
	{
		wTemp.pEnd.dY=(dLeft[i]+dLeft[i-1])/2.0;iCut=0;
		for(j=0;j<iWallNum;j++)if(Cut(wTemp,wList[j]))iCut++;
		if(iCut<iMin)iMin=iCut;
	}
	qsort(dRight,iRightCount,sizeof(double),DoubleComp);wTemp.pEnd.dX=100;
	for(i=1;i<iRightCount;i++)
	{
		wTemp.pEnd.dY=(dRight[i]+dRight[i-1])/2.0;iCut=0;
		for(j=0;j<iWallNum;j++)if(Cut(wTemp,wList[j]))iCut++;
		if(iCut<iMin)iMin=iCut;
	}
	qsort(dTop,iTopCount,sizeof(double),DoubleComp);wTemp.pEnd.dY=100;
	for(i=1;i<iTopCount;i++)
	{
		wTemp.pEnd.dX=(dTop[i]+dTop[i-1])/2.0;iCut=0;
		for(j=0;j<iWallNum;j++)if(Cut(wTemp,wList[j]))iCut++;
		if(iCut<iMin)iMin=iCut;
	}
	qsort(dBottom,iBottomCount,sizeof(double),DoubleComp);wTemp.pEnd.dY=0;
	for(i=1;i<iBottomCount;i++)
	{
		wTemp.pEnd.dX=(dBottom[i]+dBottom[i-1])/2.0;iCut=0;
		for(j=0;j<iWallNum;j++)if(Cut(wTemp,wList[j]))iCut++;
		if(iCut<iMin)iMin=iCut;
	}
	cout<<"Number of doors = "<<iMin+1<<endl;
}