#include<iostream.h>
#include<string.h>

#define BLACK 1
#define WHITE -1
#define NONE 0

struct Position
{
	int iColor;
	bool bReachable,bJoin[8];
}pMap[25][25];
const int Knight[8][2]={-1,-2,-2,-1,-2,1,-1,2,1,2,2,1,2,-1,1,-2};
int iSide;

inline int Min(int iLeft,int iRight)
{
	return (iLeft<iRight)?iLeft:iRight;
}

inline int Max(int iLeft,int iRight)
{
	return (iLeft>iRight)?iLeft:iRight;
}

bool CheckCut(int xa,int ya,int xb,int yb,int xc,int yc,int xd,int yd)
{
	return ((xb-xa)*(yc-ya)-(xc-xa)*(yb-ya))*((xb-xa)*(yd-ya)-(xd-xa)*(yb-ya))<0 && ((xd-xc)*(ya-yc)-(xa-xc)*(yd-yc))*((xd-xc)*(yb-yc)-(xb-xc)*(yd-yc))<0;
}

bool CheckLine(int iSouX,int iSouY,int iDesX,int iDesY)
{
	int i,j,k;
	for(i=Min(iSouX,iDesX);i<=Max(iSouX,iDesX);i++)
		for(j=Min(iSouY,iDesY);j<=Max(iSouY,iDesY);j++)
			if(pMap[i][j].iColor!=NONE)
				for(k=0;k<8;k++)
					if(pMap[i][j].bJoin[k] && CheckCut(iSouX,iSouY,iDesX,iDesY,i,j,i+Knight[k][0],j+Knight[k][1]))
						return false;
	return true;
}

void SetJoint(int iSouX,int iSouY)
{
	int i;
	pMap[iSouX][iSouY].bReachable=true;
	for(i=0;i<8;i++)
		if(pMap[iSouX][iSouY].bJoin[i] && !pMap[iSouX+Knight[i][0]][iSouY+Knight[i][1]].bReachable)
			SetJoint(iSouX+Knight[i][0],iSouY+Knight[i][1]);
}

bool CheckWin(int iWinColor)
{
	int i,j;
	for(i=0;i<=iSide;i++)
		for(j=0;j<=iSide;j++)
			pMap[i][j].bReachable=false;
	for(i=0;i<=iSide;i++)
		if(pMap[0][i].iColor==iWinColor && !pMap[0][i].bReachable)
		{
			SetJoint(0,i);
			for(j=0;j<=iSide;j++)
				if(pMap[iSide][j].bReachable)
					return true;
		}
	return false;
}

void main()
{
	int iMove,i,j,iCorX,iCorY;
	while(cin>>iSide>>iMove)
	{
		if(!iSide)
			break;
		memset(pMap,0,sizeof(pMap));
		for(i=0;i<iMove;i++)
		{
			cin>>iCorX>>iCorY;
			pMap[iCorX][iCorY].iColor=(i%2)?WHITE:BLACK;
			for(j=0;j<8;j++)
				if(iCorX+Knight[j][0]>=0 && iCorX+Knight[j][0]<=iSide && iCorY+Knight[j][1]>=0 && iCorY+Knight[j][1]<=iSide)
					if(pMap[iCorX+Knight[j][0]][iCorY+Knight[j][1]].iColor==pMap[iCorX][iCorY].iColor)
					{
						if(CheckLine(iCorX,iCorY,iCorX+Knight[j][0],iCorY+Knight[j][1]))
						{
							pMap[iCorX][iCorY].bJoin[j]=pMap[iCorX+Knight[j][0]][iCorY+Knight[j][1]].bJoin[(j+4)%8]=true;
						}
					}
		}
		cout<<(CheckWin(BLACK)?"yes":"no")<<endl;
	}
}