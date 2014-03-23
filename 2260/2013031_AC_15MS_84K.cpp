#include<stdio.h>
#include<string.h>

#define UNSET -1
#define FALSE -2

void main()
{
	int iSize,iMatrix[100][100],iRowSum[100],iOddRow,iColSum[100],iOddCol,i,j;
	while(scanf("%d",&iSize))
	{
		if(!iSize)
			break;
		memset(iRowSum,0,sizeof(iRowSum));
		memset(iColSum,0,sizeof(iColSum));
		for(i=0;i<iSize;i++)
			for(j=0;j<iSize;j++)
			{
				scanf("%d",&iMatrix[i][j]);
				iRowSum[i]+=iMatrix[i][j];
				iColSum[j]+=iMatrix[i][j];
			}
		iOddRow=iOddCol=UNSET;
		for(i=0;i<iSize;i++)
		{
			if(iRowSum[i]%2)
			{
				if(iOddRow==UNSET)
					iOddRow=i;
				else
					iOddRow=FALSE;
			}
			if(iColSum[i]%2)
			{
				if(iOddCol==UNSET)
					iOddCol=i;
				else
					iOddCol=FALSE;
			}
		}
		if(iOddRow==UNSET && iOddCol==UNSET)
			printf("OK\n");
		else if(iOddRow!=UNSET && iOddCol!=UNSET && iOddRow!=FALSE && iOddCol!=FALSE)
			printf("Change bit (%d,%d)\n",iOddRow+1,iOddCol+1);
		else
			printf("Corrupt\n");
	}
}