#include<stdio.h>
#include<string.h>

char War(char cMap[][101],int iX,int iY,const int iRow,const int iCol)
{
	char cWin;
	cWin=(cMap[iX][iY]=='R')?'P':((cMap[iX][iY]=='S')?'R':'S');
	if((iX>0 && cMap[iX-1][iY]==cWin) || (iY>0 && cMap[iX][iY-1]==cWin) || (iX<iRow-1 && cMap[iX+1][iY]==cWin) || (iY<iCol-1 && cMap[iX][iY+1]==cWin))
		return cWin;
	return cMap[iX][iY];
}

void main()
{
	int iCaseNum,iRow,iCol,iDay,i,j;
	char cMap[100][101],cTemp[100][101];
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d %d %d",&iRow,&iCol,&iDay);
		for(i=0;i<iRow;i++)
			scanf("%s",cMap[i]);
		while(iDay--)
		{
			for(i=0;i<iRow;i++)
				for(j=0;j<iCol;j++)
					cTemp[i][j]=War(cMap,i,j,iRow,iCol);
			memcpy(cMap,cTemp,sizeof(cMap));
		}
		for(i=0;i<iRow;i++)
		{
			for(j=0;j<iCol;j++)
				printf("%c",cMap[i][j]);
			printf("\n");
		}
		printf("\n");
	}
}