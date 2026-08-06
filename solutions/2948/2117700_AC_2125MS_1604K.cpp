#include<stdio.h>
#include<string.h>

int iHeight,iWidth,iYeye[400][400],iBlog[400][400],iMemory[400][400];

int RowSum(int iRow,int iCol)
{
	int iSum=0;
	for(;iCol<iWidth;iCol++)iSum+=iBlog[iRow][iCol];
	return iSum;
}

int ColSum(int iRow,int iCol)
{
	int iSum=0;
	for(;iRow<iHeight;iRow++)iSum+=iYeye[iRow][iCol];
	return iSum;
}

int Mine(int iRow,int iCol)
{
	int iNorth,iWest;
	if(iRow==iHeight || iCol==iWidth)return 0;
	if(iMemory[iRow][iCol]!=-1)return iMemory[iRow][iCol];
	iNorth=RowSum(iRow,iCol)+Mine(iRow+1,iCol);
	iWest=ColSum(iRow,iCol)+Mine(iRow,iCol+1);
	iMemory[iRow][iCol]=(iNorth>iWest)?iNorth:iWest;
	return iMemory[iRow][iCol];
}

void main()
{
	int i,j;
	while(scanf("%d%d",&iHeight,&iWidth))
	{
		if(iHeight==0)break;
		for(i=0;i<iHeight;i++)for(j=0;j<iWidth;j++)scanf("%d",&iYeye[i][j]);
		for(i=0;i<iHeight;i++)for(j=0;j<iWidth;j++)scanf("%d",&iBlog[i][j]);
		memset(iMemory,-1,sizeof(iMemory));
		printf("%d\n",Mine(0,0));
	}
}