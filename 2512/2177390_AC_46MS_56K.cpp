#include<stdio.h>
#include<ctype.h>
#include<string.h>

bool bSafe[8][8],bOccupy[8][8];
const int HORSE[8][2]={2,1,1,2,-1,2,-2,1,-2,-1,-1,-2,1,-2,2,-1},
		KING[8][2]={1,0,1,1,0,1,-1,1,-1,0,-1,-1,0,-1,1,-1},
		DIAGONAL[4][2]={{-1,-1},{-1,1},{1,-1},{1,1}},
		BEELINE[4][2]={1,0,-1,0,0,-1,0,1};

bool Avail(int iRow,int iCol){return iRow>=0 && iRow<8 && iCol>=0 && iCol<8;}

void FillDiagonal(int iRow,int iCol)
{
	int i,j;
	for(j=0;j<4;j++)
		for(i=1;Avail(iRow+i*DIAGONAL[j][0],iCol+i*DIAGONAL[j][1]);i++)
		{
			if(bOccupy[iRow+i*DIAGONAL[j][0]][iCol+i*DIAGONAL[j][1]])break;
			bSafe[iRow+i*DIAGONAL[j][0]][iCol+i*DIAGONAL[j][1]]=false;
		}
}

void FillBeeline(int iRow,int iCol)
{
	int i,j;
	for(j=0;j<4;j++)
		for(i=1;Avail(iRow+i*BEELINE[j][0],iCol+i*BEELINE[j][1]);i++)
		{
			if(bOccupy[iRow+i*BEELINE[j][0]][iCol+i*BEELINE[j][1]])break;
			bSafe[iRow+i*BEELINE[j][0]][iCol+i*BEELINE[j][1]]=false;
		}
}

void main()
{
	char cLine[100],cDes[8][10],cBoard[8][10];int iPos,i,j,k,iCount,iHead,iEnd;
	while(gets(cLine))
	{
		iHead=iEnd=0;
		for(i=0;i<8;i++)
		{
			for(iEnd=iHead+1;cLine[iEnd]!='/' && cLine[iEnd]!='\0';iEnd++);
			cLine[iEnd]='\0';strcpy(cDes[i],cLine+iHead);iHead=iEnd+1;
		}
		memset(bSafe,true,sizeof(bSafe));memset(bOccupy,false,sizeof(bOccupy));
		memset(cBoard,'.',sizeof(cBoard));
		for(i=0;i<8;i++)
		{
			iPos=0;
			for(j=0;cDes[i][j]!='\0';j++)
			{
				if(isdigit(cDes[i][j]))iPos+=cDes[i][j]-'0';
				else{cBoard[i][iPos]=cDes[i][j];bOccupy[i][iPos++]=true;}
			}
			cBoard[i][8]='\0';
		}
		for(i=0;i<8;i++)
			for(j=0;j<8;j++)
			{
				if(cBoard[i][j]=='p')
				{
					if(Avail(i+1,j-1))bSafe[i+1][j-1]=false;
					if(Avail(i+1,j+1))bSafe[i+1][j+1]=false;
				}
				else if(cBoard[i][j]=='P')
				{
					if(Avail(i-1,j-1))bSafe[i-1][j-1]=false;
					if(Avail(i-1,j+1))bSafe[i-1][j+1]=false;
				}
				else if(tolower(cBoard[i][j])=='n')
					for(k=0;k<8;k++)
					{
						if(Avail(i+HORSE[k][0],j+HORSE[k][1]))
							bSafe[i+HORSE[k][0]][j+HORSE[k][1]]=false;
					}
				else if(tolower(cBoard[i][j])=='b')FillDiagonal(i,j);
				else if(tolower(cBoard[i][j])=='r')FillBeeline(i,j);
				else if(tolower(cBoard[i][j])=='q')
					{FillDiagonal(i,j);FillBeeline(i,j);}
				else if(tolower(cBoard[i][j])=='k')
					for(k=0;k<8;k++)
					{
						if(Avail(i+KING[k][0],j+KING[k][1]))
							bSafe[i+KING[k][0]][j+KING[k][1]]=false;
					}
			}
		iCount=0;
		for(i=0;i<8;i++)for(j=0;j<8;j++)if(bSafe[i][j] && !bOccupy[i][j])iCount++;
		printf("%d\n",iCount);
	}
}