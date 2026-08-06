#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define MAX 100

struct Point{double dX,dY;};
struct Map{Point pBegin,pEnd;} mList[MAX];

int Sign(double dValue){return (fabs(dValue)<1e-6)?0:(dValue<0)?(-1):1;}

int DoubleCmp(const void* pLeft,const void* pRight)
{return Sign(*((double*)pLeft)-*((double*)pRight));}

void SortAndRemoveSame(double* dArray,int* iNum)
{
	double dTemp[2*MAX];int iTempCount,i;
	iTempCount=*iNum;memcpy(dTemp,dArray,iTempCount*sizeof(double));
	qsort(dTemp,iTempCount,sizeof(double),DoubleCmp);dArray[0]=dTemp[0];*iNum=1;
	for(i=1;i<iTempCount;i++)if(Sign(dTemp[i]-dArray[*iNum-1])!=0)dArray[(*iNum)++]=dTemp[i];
}

void main()
{
	int iMapNum,iCaseCount=0,i,j,k,iCountX,iCountY,iTemp[4];
	double dListX[2*MAX],dListY[2*MAX],dTotal;
	bool bExplored[2*MAX][2*MAX];
	while(scanf("%d",&iMapNum))
	{
		if(iMapNum==0)break;
		printf("Test case #%d\n",++iCaseCount);iCountX=iCountY=0;
		for(i=0;i<iMapNum;i++)
		{
			scanf("%lf%lf%lf%lf",&mList[i].pBegin.dX,&mList[i].pBegin.dY,&mList[i].pEnd.dX,&mList[i].pEnd.dY);
			dListX[iCountX++]=mList[i].pBegin.dX;dListX[iCountX++]=mList[i].pEnd.dX;
			dListY[iCountY++]=mList[i].pBegin.dY;dListY[iCountY++]=mList[i].pEnd.dY;
		}
		SortAndRemoveSame(dListX,&iCountX);SortAndRemoveSame(dListY,&iCountY);
		memset(bExplored,false,sizeof(bExplored));
		for(i=0;i<iMapNum;i++)
		{
			for(j=0;j<iCountX;j++)if(Sign(mList[i].pBegin.dX-dListX[j])==0)break;iTemp[0]=j;
			for(j=iTemp[0]+1;j<iCountX;j++)if(Sign(mList[i].pEnd.dX-dListX[j])==0)break;iTemp[1]=j;
			for(j=0;j<iCountY;j++)if(Sign(mList[i].pBegin.dY-dListY[j])==0)break;iTemp[2]=j;
			for(j=iTemp[2]+1;j<iCountY;j++)if(Sign(mList[i].pEnd.dY-dListY[j])==0)break;iTemp[3]=j;
			for(j=iTemp[0];j<iTemp[1];j++)for(k=iTemp[2];k<iTemp[3];k++)bExplored[j][k]=true;
		}
		dTotal=0.0;
		for(i=0;i<iCountX-1;i++)
			for(j=0;j<iCountY-1;j++)
				if(bExplored[i][j])dTotal+=(dListX[i+1]-dListX[i])*(dListY[j+1]-dListY[j]);
		printf("Total explored area: %.2lf\n\n",dTotal);
	}
}