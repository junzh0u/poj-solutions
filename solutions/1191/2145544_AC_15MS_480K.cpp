#include<stdio.h>
#include<math.h>

#define INF 10000000.0
#define UNSET -1

int iBoard[8][8];
double dAverage=0.0,dMemory[8][8][8][8][13],dVariance[8][8][8][8];

void main()
{
	int iPart,i,j,k,iBeginRow,iBeginCol,iEndRow,iEndCol;double iTemp;
	scanf("%d",&iPart);
	for(i=0;i<8;i++)for(j=0;j<8;j++){scanf("%d",&iBoard[i][j]);dAverage+=iBoard[i][j];}
	dAverage/=iPart;
	for(iBeginRow=0;iBeginRow<8;iBeginRow++)
		for(iBeginCol=0;iBeginCol<8;iBeginCol++)
			for(iEndRow=iBeginRow;iEndRow<8;iEndRow++)
				for(iEndCol=iBeginCol;iEndCol<8;iEndCol++)
				{
					dVariance[iBeginRow][iBeginCol][iEndRow][iEndCol]=0.0;
					for(i=iBeginRow;i<=iEndRow;i++)
						for(j=iBeginCol;j<=iEndCol;j++)
							dVariance[iBeginRow][iBeginCol][iEndRow][iEndCol]+=iBoard[i][j];
					dVariance[iBeginRow][iBeginCol][iEndRow][iEndCol]-=dAverage;
					dVariance[iBeginRow][iBeginCol][iEndRow][iEndCol]*=dVariance[iBeginRow][iBeginCol][iEndRow][iEndCol];
					dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][0]=dVariance[iBeginRow][iBeginCol][iEndRow][iEndCol];
				}
	for(k=1;k<iPart;k++)
		for(iBeginRow=0;iBeginRow<8;iBeginRow++)
			for(iBeginCol=0;iBeginCol<8;iBeginCol++)
				for(iEndRow=iBeginRow;iEndRow<8;iEndRow++)
					for(iEndCol=iBeginCol;iEndCol<8;iEndCol++)
					{
						dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]=INF;
						for(i=iBeginRow;i<iEndRow;i++)
						{
							iTemp=dVariance[iBeginRow][iBeginCol][i][iEndCol]+dMemory[i+1][iBeginCol][iEndRow][iEndCol][k-1];
							if(dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]>iTemp)
								dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]=iTemp;
							iTemp=dVariance[i+1][iBeginCol][iEndRow][iEndCol]+dMemory[iBeginRow][iBeginCol][i][iEndCol][k-1];
							if(dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]>iTemp)
								dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]=iTemp;
						}
						for(i=iBeginCol;i<iEndCol;i++)
						{
							iTemp=dVariance[iBeginRow][iBeginCol][iEndRow][i]+dMemory[iBeginRow][i+1][iEndRow][iEndCol][k-1];
							if(dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]>iTemp)
								dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]=iTemp;
							iTemp=dVariance[iBeginRow][i+1][iEndRow][iEndCol]+dMemory[iBeginRow][iBeginCol][iEndRow][i][k-1];
							if(dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]>iTemp)
								dMemory[iBeginRow][iBeginCol][iEndRow][iEndCol][k]=iTemp;
						}
					}
	printf("%.3f\n",sqrt(dMemory[0][0][7][7][iPart-1]/iPart));
}