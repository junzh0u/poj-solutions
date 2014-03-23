//#include<iostream.h>
#include<stdio.h>
#include<math.h>
#include<string.h>

char cMap[750][750];

void DrawMap(int iDegree,int iOriX,int iOriY)
{
	if(iDegree==1)
		cMap[iOriX][iOriY]='X';
	else
	{
		DrawMap(iDegree-1,iOriX,iOriY);
		DrawMap(iDegree-1,iOriX+2*pow(3,iDegree-2),iOriY);
		DrawMap(iDegree-1,iOriX+pow(3,iDegree-2),iOriY+pow(3,iDegree-2));
		DrawMap(iDegree-1,iOriX,iOriY+2*pow(3,iDegree-2));
		DrawMap(iDegree-1,iOriX+2*pow(3,iDegree-2),iOriY+2*pow(3,iDegree-2));
	}
}

void main()
{
	int iDegree,i,j;
	while(scanf("%d",&iDegree))
	{
		if(iDegree==-1)
			break;
		memset(cMap,' ',sizeof(cMap));
		DrawMap(iDegree,0,0);
		for(i=0;i<pow(3,iDegree-1);i++)
		{
			for(j=0;j<pow(3,iDegree-1);j++)
				printf("%c",cMap[i][j]);
			printf("\n");
		}
		printf("-\n");
	}
}