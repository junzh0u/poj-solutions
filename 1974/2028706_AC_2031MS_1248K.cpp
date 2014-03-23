#include<stdio.h>
#include<stdlib.h>

int iRowMax,iColMax;

struct Stone
{
	int iRow,iCol;
};

int RowCmp(const void* psLeft,const void* psRight)
{
	if((((Stone*)psLeft)->iRow)!=(((Stone*)psRight)->iRow))
		return (((Stone*)psLeft)->iRow)-(((Stone*)psRight)->iRow);
	else
		return (((Stone*)psLeft)->iCol)-(((Stone*)psRight)->iCol);
}

int ColCmp(const void* psLeft,const void* psRight)
{
	if((((Stone*)psLeft)->iCol)!=(((Stone*)psRight)->iCol))
		return (((Stone*)psLeft)->iCol)-(((Stone*)psRight)->iCol);
	else
		return (((Stone*)psLeft)->iRow)-(((Stone*)psRight)->iRow);
}

int RowCount(const Stone* psBegin,const Stone* psEnd)
{
	if(psBegin->iRow==psEnd->iRow)
		return (psEnd->iCol)-(psBegin->iCol)>2;
	else
		return ((psBegin->iCol)<=(iColMax-2))+((psEnd->iCol)>2)+(psEnd->iRow)-(psBegin->iRow)-1;
}

int ColCount(const Stone* psBegin,const Stone* psEnd)
{
	if(psBegin->iCol==psEnd->iCol)
		return (psEnd->iRow)-(psBegin->iRow)>2;
	else
		return ((psBegin->iRow)<=(iRowMax-2))+((psEnd->iRow)>2)+(psEnd->iCol)-(psBegin->iCol)-1;
}

void main()
{
	int iCaseNum,iStoneNum,i,iSum;
	Stone stList[150000];
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d %d %d",&iRowMax,&iColMax,&iStoneNum);
		for(i=1;i<=iStoneNum;i++)
			scanf("%d %d",&stList[i].iRow,&stList[i].iCol);
		iSum=0;
		stList[0].iRow=1;stList[0].iCol=0;
		stList[iStoneNum+1].iRow=iRowMax;stList[iStoneNum+1].iCol=iColMax+1;
		qsort(stList+1,iStoneNum,sizeof(Stone),RowCmp);
		for(i=1;i<=iStoneNum+1;i++)
			iSum+=RowCount(stList+i-1,stList+i);
		stList[0].iRow=0;stList[0].iCol=1;
		stList[iStoneNum+1].iRow=iRowMax+1;stList[iStoneNum+1].iCol=iColMax;
		qsort(stList+1,iStoneNum,sizeof(Stone),ColCmp);
		for(i=1;i<=iStoneNum+1;i++)
			iSum+=ColCount(stList+i-1,stList+i);
		printf("%d\n",iSum);
	}
}