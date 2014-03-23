#include<iostream.h>
#define SHIP '#'
#define WATER '.'
#define MAX 1000

int iHeight,iWidth;char cBoard[MAX][MAX+1];

bool Avail(int iRow,int iCol){return iRow>=0 && iRow<iHeight && iCol>=0 && iCol<iWidth;}

bool Fill(int iBeginRow,int iBeginCol)
{
	int iEndRow,iEndCol,i,j;
	for(iEndRow=iBeginRow+1;iEndRow<=iHeight && cBoard[iEndRow][iBeginCol]==SHIP;iEndRow++);
	for(iEndCol=iBeginCol+1;iEndCol<=iWidth && cBoard[iBeginRow][iEndCol]==SHIP;iEndCol++);
	for(i=iBeginRow;i<iEndRow;i++)for(j=iBeginCol;j<iEndCol;j++)
		{if(cBoard[i][j]!=SHIP)return false;cBoard[i][j]=WATER;}
	for(i=iBeginRow-1;i<=iEndRow;i++)
		if((Avail(i,iBeginCol-1) && cBoard[i][iBeginCol-1]==SHIP) || (Avail(i,iEndCol) && cBoard[i][iEndCol]==SHIP))
			return false;
	for(i=iBeginCol-1;i<=iEndCol;i++)
		if((Avail(iBeginRow-1,i) && cBoard[iBeginRow-1][i]==SHIP) || (Avail(iEndRow,i) && cBoard[iEndRow][i]==SHIP))
			return false;
	return true;
}

int Judge()
{
	int i,j,iCount;
	for(i=0,iCount=0;i<iHeight;i++)for(j=0;j<iWidth;j++)
		if(cBoard[i][j]==SHIP){if(!Fill(i,j))return -1;iCount++;}
	return iCount;
}

void main()
{
	int i,iCount;
	while(cin>>iHeight>>iWidth && iHeight>0)
	{
		for(i=0;i<iHeight;i++)cin>>cBoard[i];
		if((iCount=Judge())>=0)cout<<"There are "<<iCount<<" ships."<<endl;
		else cout<<"Bad placement."<<endl;
	}
}