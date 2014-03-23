#include<iostream.h>

char cMap[110][110];
int iRow,iCol;

void FillLake(int,int);

void main()
{
	int i,j,iCount;
	while(1)
	{
		cin>>iRow>>iCol;
		if(!iRow)
			break;
		for(i=0;i<iRow;i++)
			for(j=0;j<iCol;j++)
				cin>>cMap[i][j];
		iCount=0;
		for(i=0;i<iRow;i++)
			for(j=0;j<iCol;j++)
				if(cMap[i][j]=='@')
				{
					iCount++;
					FillLake(i,j);
				}
		cout<<iCount<<endl;
	}
}

void FillLake(int iX,int iY)
{
	cMap[iX][iY]='*';
	if(iX>0 && iY>0 && cMap[iX-1][iY-1]=='@')
		FillLake(iX-1,iY-1);
	if(iX>0 && cMap[iX-1][iY]=='@')
		FillLake(iX-1,iY);
	if(iX>0 && iY<iCol && cMap[iX-1][iY+1]=='@')
		FillLake(iX-1,iY+1);
	if(iY>0 && cMap[iX][iY-1]=='@')
		FillLake(iX,iY-1);
	if(iY<iCol-1 && cMap[iX][iY+1]=='@')
		FillLake(iX,iY+1);
	if(iX<iRow-1 && iY>0 && cMap[iX+1][iY-1]=='@')
		FillLake(iX+1,iY-1);
	if(iX<iRow-1 && cMap[iX+1][iY]=='@')
		FillLake(iX+1,iY);
	if(iX<iRow-1 && iY<iCol-1 && cMap[iX+1][iY+1]=='@')
		FillLake(iX+1,iY+1);
	return;
}