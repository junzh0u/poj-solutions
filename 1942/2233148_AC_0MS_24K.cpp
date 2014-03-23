#include<stdio.h>

__int64 Choose(__int64 iTotal,__int64 iNum)
{return (iNum>0)?(Choose(iTotal-1,iNum-1)*iTotal/iNum):1;}

void main()
{
	__int64 iRow,iCol;
	while(scanf("%I64d%I64d",&iRow,&iCol) && (iRow!=0 || iCol!=0))
		printf("%I64d\n",Choose(iRow+iCol,(iRow<iCol)?iRow:iCol));
}