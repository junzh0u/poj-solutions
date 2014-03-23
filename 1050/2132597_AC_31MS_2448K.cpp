#include<stdio.h>
#include<string.h>
#define MAX 100

int iSize,iArray[MAX][MAX],iColumn[MAX][MAX][MAX];

void main()
{
	int i,j,k,iMax,iPartMax;
	scanf("%d",&iSize);
	//memset(iColumn[0][0],0,sizeof(int)*MAX);
	for(i=0;i<iSize;i++)
	{
		for(j=0;j<iSize;j++)
		{
			scanf("%d",&iArray[i][j]);
			for(k=0;k<i;k++)
				iColumn[k][i][j]=iColumn[k][i-1][j]+iArray[i][j];
			iColumn[i][i][j]=iArray[i][j];
		}
	}
	iMax=iArray[0][0];
	for(i=0;i<iSize;i++)
		for(j=i;j<iSize;j++)
		{
			iPartMax=iColumn[i][j][0];
			for(k=1;k<iSize;k++)
				if(iColumn[i][j][k-1]>0)
				{
					iColumn[i][j][k]+=iColumn[i][j][k-1];
					if(iColumn[i][j][k]>iPartMax)
						iPartMax=iColumn[i][j][k];
				}
			if(iPartMax>iMax)
				iMax=iPartMax;
		}
	printf("%d\n",iMax);
}