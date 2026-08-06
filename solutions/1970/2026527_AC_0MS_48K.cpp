#include<stdio.h>

bool Judge(int iRow,int iCol,int iMap[][19],int iSame)
{
	int i;
	if(iRow>=iSame-1 && iCol<=19-iSame)
	{
		for(i=1;i<=iSame-1;i++)
			if(iMap[iRow-i][iCol+i]!=iMap[iRow][iCol])
				break;
		if(i==iSame && (iRow==iSame-1 || iCol==19-iSame || iMap[iRow-iSame][iCol+iSame]!=iMap[iRow][iCol]) && (iRow==18 || iCol==0 || iMap[iRow+1][iCol-1]!=iMap[iRow][iCol]))
			return true;
	}
	if(iCol<=19-iSame)
	{
		for(i=1;i<=iSame-1;i++)
			if(iMap[iRow][iCol+i]!=iMap[iRow][iCol])
				break;
		if(i==iSame && (iCol==19-iSame || iMap[iRow][iCol+iSame]!=iMap[iRow][iCol]) && (iCol==0 || iMap[iRow][iCol-1]!=iMap[iRow][iCol]))
			return true;
	}
	if(iRow<=19-iSame && iCol<=19-iSame)
	{
		for(i=1;i<=iSame-1;i++)
			if(iMap[iRow+i][iCol+i]!=iMap[iRow][iCol])
				break;
		if(i==iSame && (iRow==19-iSame || iCol==19-iSame || iMap[iRow+iSame][iCol+iSame]!=iMap[iRow][iCol]) && (iRow==0 || iCol==0 || iMap[iRow-1][iCol-1]!=iMap[iRow][iCol]))
			return true;
	}
	if(iRow<=19-iSame)
	{
		for(i=1;i<=iSame-1;i++)
			if(iMap[iRow+i][iCol]!=iMap[iRow][iCol])
				break;
		if(i==iSame && (iRow==19-iSame || iMap[iRow+iSame][iCol]!=iMap[iRow][iCol]) && (iRow==0 || iMap[iRow-1][iCol]!=iMap[iRow][iCol]))
			return true;
	}
	return false;
}

void main()
{
	int iCaseNum,iMap[19][19],i,j,iRow,iCol;
	bool bJudge;
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		for(i=0;i<19;i++)
			for(j=0;j<19;j++)
				scanf("%d",&iMap[i][j]);
		bJudge=false;
		for(i=0;i<19 && !bJudge;i++)
			for(j=0;j<19 && !bJudge;j++)
				if(iMap[j][i] && Judge(j,i,iMap,5))
				{
					iRow=j;iCol=i;
					bJudge=true;
				}
		if(!bJudge)
			printf("0\n");
		else
			printf("%d\n%d %d\n",iMap[iRow][iCol],iRow+1,iCol+1);
	}
}