#include<iostream.h>
#include<string.h>

char cBoard[10];

bool IsSame(int iA,int iB,int iC)
{
	if(cBoard[iA]!='.' && cBoard[iB]==cBoard[iA] && cBoard[iC]==cBoard[iA])
		return true;
	return false;
}

void main()
{
	const char END[10]="end";
	char cWin;
	int iX,iO,i,j,iWinCount,iPointWin[9];
	bool bValid;
	while(1)
	{
		cin>>cBoard;
		if(!strcmp(cBoard,END))
			break;
		iX=iO=0;
		for(i=0;i<9;i++)
		{
			if(cBoard[i]=='X')
				iX++;
			else if(cBoard[i]=='O')
				iO++;
		}
		if(iX!=iO && iX!=iO+1)
			bValid=false;
		else if(iX<3)
			bValid=false;
		else
		{
			iWinCount=0;
			for(i=0;i<9;i++)
				iPointWin[i]=0;
			for(i=0;i<3;i++)
				if(IsSame(3*i,3*i+1,3*i+2))
				{
					iPointWin[3*i]++;
					iPointWin[3*i+1]++;
					iPointWin[3*i+2]++;
					cWin=cBoard[3*i];
					iWinCount++;
				}
			for(i=0;i<3;i++)
				if(IsSame(i,i+3,i+6))
				{
					iPointWin[i]++;
					iPointWin[i+3]++;
					iPointWin[i+6]++;
					cWin=cBoard[i];
					iWinCount++;
				}
			if(IsSame(0,4,8))
			{
				iPointWin[0]++;
				iPointWin[4]++;
				iPointWin[8]++;
				cWin=cBoard[0];
				iWinCount++;
			}
			if(IsSame(2,4,6))
			{
				iPointWin[2]++;
				iPointWin[4]++;
				iPointWin[6]++;
				cWin=cBoard[2];
				iWinCount++;
			}
			if(iWinCount==0)
			{
				if(iX==5 && iO==4)
					bValid=true;
				else
					bValid=false;
			}
			else if(iWinCount==1)
			{
				if((cWin=='X' && iX==iO+1) || (cWin=='O' && iX==iO))
					bValid=true;
				else
					bValid=false;
			}
			else
			{
				bValid=false;
				for(i=0;i<9;i++)
					if(iPointWin[i]==iWinCount)
					{
						bValid=true;
						cWin=cBoard[i];
						break;
					}
				if(bValid)
					if(!((cWin=='X' && iX==iO+1) || (cWin=='O' && iX==iO)))
						bValid=false;
			}
		}
		cout<<(bValid?("valid"):("invalid"))<<endl;
	}
}