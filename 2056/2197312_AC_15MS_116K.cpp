#include<iostream.h>

char cGrid[200][201];
int iRowNum,iColNum;

bool Avail(int iRow,int iCol)
{
	return iRow>=0 && iRow<iRowNum && iCol>=0 && iCol<iColNum;
}

bool IsBoard(int iCol,int iBegin,int iEnd)
{
	//if(Avail(iBegin-1,iCol-1) && cGrid[iBegin-1][iCol-1]!='S')
	//	return false;
	for(int i=iBegin;i<iEnd;i++)
		if(cGrid[i][iCol-1]!='S')
			return false;
	//if(Avail(iEnd,iCol-1) && cGrid[iEnd][iCol-1]!='S')
	//	return false;
	return true;
}

void Improve(int iCol,int iBegin,int iEnd)
{
	for(int i=iBegin;i<iEnd;i++)
		cGrid[i][iCol]='n';
}

int SetNewBoard()
{
	int iCount=0,i,j,k;
	for(i=0;i<iRowNum;i++)
		for(j=1;j<iColNum;j++)
			if(cGrid[i][j]=='B')
				for(k=-1;k<=1;k++)
					if(Avail(i+k,j-1) && cGrid[i+k][j-1]!='B' && cGrid[i+k][j-1]!='N')
					{
						cGrid[i+k][j-1]='N';
						iCount++;
					}
	return iCount;
}

void main()
{
	int i,j,k;
	while(cin>>iRowNum>>iColNum && iRowNum!=0)
	{
		for(i=0;i<iRowNum;i++)
			cin>>cGrid[i];
		for(i=1;i<iColNum-1;i++)
		{
			for(j=0;j<iRowNum;)
			{
				for(;j<iRowNum && cGrid[j][i]!='B';j++);
				for(k=j+1;k<iRowNum && cGrid[k][i]=='B';k++);
				if(IsBoard(i,j,k))Improve(i,j,k);
				j=k+1;
			}
		}
		cout<<SetNewBoard()<<endl;
		//for(i=0;i<iRowNum;i++)
		//	cout<<cGrid[i]<<endl;
	}
}