#include<iostream.h>
#include<string.h>
#define MAX 300

struct Point{int iX,iY;} pDeque[MAX*MAX];
int iBoard[MAX][MAX],iSize;
const int MOVE[8][2]={{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};

bool Avail(int iX,int iY)
{
	return iX>=0 && iX<iSize && iY>=0 && iY<iSize && iBoard[iX][iY]==-1;
}

void main()
{
	int iCaseNum,iCur,iEnd,i;
	Point pEnd;
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		memset(iBoard,-1,sizeof(int)*MAX*MAX);iCur=0;iEnd=1;
		cin>>iSize>>pDeque[0].iX>>pDeque[0].iY>>pEnd.iX>>pEnd.iY;
		iBoard[pDeque[0].iX][pDeque[0].iY]=0;
		while(iBoard[pEnd.iX][pEnd.iY]==-1)
		{
			for(i=0;i<8;i++)
				if(Avail(pDeque[iCur].iX+MOVE[i][0],pDeque[iCur].iY+MOVE[i][1]))
				{
					pDeque[iEnd].iX=pDeque[iCur].iX+MOVE[i][0];
					pDeque[iEnd].iY=pDeque[iCur].iY+MOVE[i][1];
					iBoard[pDeque[iEnd].iX][pDeque[iEnd].iY]=iBoard[pDeque[iCur].iX][pDeque[iCur].iY]+1;
					iEnd++;
				}
			iCur++;
		}
		cout<<iBoard[pEnd.iX][pEnd.iY]<<endl;
	}
}