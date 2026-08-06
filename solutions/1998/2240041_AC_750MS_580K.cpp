#include<iostream>
using namespace std;
#define MAX 1000
#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3
#define INVALID -1

struct Position{int iRow,iCol;} pList[MAX*MAX];int iHeight,iWidth,iBoard[MAX][MAX];
const char STRING[4][9]={"doprava","doleva","nahoru","dolu"};
const int DIRECTION[4][2]={{0,1},{0,-1},{-1,0},{1,0}};

bool Avail(int iRow,int iCol){return iRow>=0 && iRow<iHeight && iCol>=0 && iCol<iWidth;}

int Move(int iTile)
{
	int iDir;
	for(iDir=0;iDir<4;iDir++)
		if(Avail(pList[iTile].iRow+DIRECTION[iDir][0],pList[iTile].iCol+DIRECTION[iDir][1]) && iBoard[pList[iTile].iRow+DIRECTION[iDir][0]][pList[iTile].iCol+DIRECTION[iDir][1]]==0)
		{
			swap(iBoard[pList[iTile].iRow+DIRECTION[iDir][0]][pList[iTile].iCol+DIRECTION[iDir][1]],iBoard[pList[iTile].iRow][pList[iTile].iCol]);
			swap(pList[iBoard[pList[iTile].iRow+DIRECTION[iDir][0]][pList[iTile].iCol+DIRECTION[iDir][1]]],pList[iBoard[pList[iTile].iRow][pList[iTile].iCol]]);
			break;
		}
	if(iDir>=4)return INVALID;
	return iDir;
}

void main()
{
	int iCaseNum,i,j,k,iTile,iSign;
	cin>>iCaseNum;
	for(i=1;i<=iCaseNum;i++)
	{
		cout<<"Skladacka cislo "<<i<<":"<<endl;
		cin>>iHeight>>iWidth;
		for(j=0;j<iHeight;j++)for(k=0;k<iWidth;k++)
			{cin>>iBoard[j][k];pList[iBoard[j][k]].iRow=j;pList[iBoard[j][k]].iCol=k;}
		while(cin>>iTile && iTile!=0)
		{
			if((iSign=Move(iTile))==INVALID)cout<<"Neplatny tah kamenem "<<iTile<<"."<<endl;
			else cout<<"Kamen "<<iTile<<" presunut "<<STRING[iSign]<<"."<<endl;
		}
		for(j=0;j<iHeight;j++){for(k=0;k<iWidth;k++)cout<<iBoard[j][k]<<" ";cout<<endl;}cout<<endl;
	}
}