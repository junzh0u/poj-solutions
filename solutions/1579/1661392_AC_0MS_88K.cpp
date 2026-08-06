#include<stdio.h>

void main()
{
	int iA,iB,iC,i,j,k;
	int iW[21][21][21];
	for(i=0;i<21;i++)
		for(j=0;j<21;j++)
			iW[0][i][j]=iW[i][0][j]=iW[i][j][0]=1;
	for(i=1;i<21;i++)
		for(j=1;j<21;j++)
			for(k=1;k<21;k++)
			{
				if(i<j && j<k)
					iW[i][j][k]=iW[i][j][k-1]+iW[i][j-1][k-1]-iW[i][j-1][k];
				else
					iW[i][j][k]=iW[i-1][j][k]+iW[i-1][j-1][k]+iW[i-1][j][k-1]-iW[i-1][j-1][k-1];
			}
	while(1)
	{
		scanf("%d%d%d",&iA,&iB,&iC);
		if(iA==-1 && iB==-1 &&iC==-1)
			break;
		else if(iA<=0 || iB<=0 || iC<=0)
			printf("w(%d, %d, %d) = %d\n",iA,iB,iC,iW[0][0][0]);
		else if(iA>20 || iB>20 || iC>20)
			printf("w(%d, %d, %d) = %d\n",iA,iB,iC,iW[20][20][20]);
		else
			printf("w(%d, %d, %d) = %d\n",iA,iB,iC,iW[iA][iB][iC]);
	}
}
