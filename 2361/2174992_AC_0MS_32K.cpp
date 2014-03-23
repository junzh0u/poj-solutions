#include<iostream.h>

char cBoard[3][4];
const int LINE[8][3][2]={	{{0,0},{0,1},{0,2}},{{1,0},{1,1},{1,2}},{{2,0},{2,1},{2,2}},
							{{0,0},{1,0},{2,0}},{{0,1},{1,1},{2,1}},{{0,2},{1,2},{2,2}},
							{{0,0},{1,1},{2,2}},{{0,2},{1,1},{2,0}}						};

bool LineWin(int iLine)
{return cBoard[LINE[iLine][0][0]][LINE[iLine][0][1]]!='.' && cBoard[LINE[iLine][0][0]][LINE[iLine][0][1]]==cBoard[LINE[iLine][1][0]][LINE[iLine][1][1]] &&  cBoard[LINE[iLine][0][0]][LINE[iLine][0][1]]==cBoard[LINE[iLine][2][0]][LINE[iLine][2][1]];}

bool Win(){for(int i=0;i<8;i++)if(LineWin(i))return true;return false;}

bool CaseSolve()
{
	int iNumX=0,iNumO=0,i,j;char cLast;
	for(i=0;i<3;i++)
	{
		cin>>cBoard[i];
		for(j=0;j<3;j++)
		{
			if(cBoard[i][j]=='X')iNumX++;
			if(cBoard[i][j]=='O')iNumO++;
		}
	}
	if(iNumX!=iNumO && iNumX!=iNumO+1)return false;
	cLast=iNumX==iNumO?'O':'X';
	if(!Win())return true;
	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if(cBoard[i][j]==cLast)
			{
				cBoard[i][j]='.';
				if(!Win())return true;
				cBoard[i][j]=cLast;
			}
	return false;
}

void main()
{
	int iCaseNum;
	cin>>iCaseNum;
	while(iCaseNum--){cout<<(CaseSolve()?"yes":"no")<<endl;}
}