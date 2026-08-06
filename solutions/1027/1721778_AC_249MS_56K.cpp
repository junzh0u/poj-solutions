#include<stdio.h>

struct Pair
{
	int iX;
	int iY;
};

char cMap[10][16];
bool bVisit[10][15];
int iTotal;
Pair pTemp[160],pSame[160];

int CalBall(int,int,char,bool);
void RemoveBall(int,int,char,int);

int main()
{
	int iCaseNum,i,j,k,l,iRemove,iScore,iRemX,iRemY,iCount,iRemain,iTemp;
	char cRemCol;
	scanf("%d",&iCaseNum);
	for(i=0;i<iCaseNum;i++)
	{
		for(j=0;j<10;j++)
			scanf("%s",cMap[j]);
		iScore=0;
		iCount=0;
		iRemain=150;
		printf("Game %d:\n\n",i+1);
		while(1)
		{
			iRemove=0;
			for(j=0;j<10;j++)
				for(k=0;k<15;k++)
					bVisit[j][k]=false;
			for(k=0;k<15;k++)
				for(j=9;j>=0;j--)
				{
					if(cMap[j][k]==' ' || bVisit[j][k])
						continue;
					iTemp=CalBall(j,k,cMap[j][k],true);
					if(iTemp>iRemove)
					{
						iRemove=iTemp;
						iRemX=j;
						iRemY=k;
						cRemCol=cMap[j][k];
						for(l=0;l<iRemove;l++)
						{
							pSame[l].iX=pTemp[l].iX;
							pSame[l].iY=pTemp[l].iY;
						}
					}
				}
			if(iRemove<2)
				break;
			RemoveBall(iRemX,iRemY,cRemCol,iRemove);
			iCount++;
			printf("Move %d at (%d,%d): removed %d balls of color %c, got %d points.\n",iCount,10-iRemX,iRemY+1,iRemove,cRemCol,(iRemove-2)*(iRemove-2));
			iScore+=(iRemove-2)*(iRemove-2);
			iRemain-=iRemove;
			if(iRemain<2)
				break;
		}
		if(!iRemain)
			iScore+=1000;
		printf("Final score: %d, with %d balls remaining.\n\n",iScore,iRemain);
	}
	return 0;
}

int CalBall(int iX,int iY,char cCol,bool bIni)
{
	//int i;
	if(bIni)
		iTotal=0;
	//cMap[iX][iY]='*';
	bVisit[iX][iY]=true;
	pTemp[iTotal].iX=iX;
	pTemp[iTotal].iY=iY;
	iTotal++;
	if(iX>0 && cMap[iX-1][iY]==cCol && !bVisit[iX-1][iY])
		CalBall(iX-1,iY,cCol,false);
	if(iY>0 && cMap[iX][iY-1]==cCol && !bVisit[iX][iY-1])
		CalBall(iX,iY-1,cCol,false);
	if(iX<9 && cMap[iX+1][iY]==cCol && !bVisit[iX+1][iY])
		CalBall(iX+1,iY,cCol,false);
	if(iY<14 && cMap[iX][iY+1]==cCol && !bVisit[iX][iY+1])
		CalBall(iX,iY+1,cCol,false);
	/*if(bIni)
		for(i=0;i<iTotal;i++)
			cMap[pTemp[i].iX][pTemp[i].iY]=cCol;*/
	return iTotal;
}

void RemoveBall(int iX,int iY,char cCol,int iNum)
{
	int i,j,k;
	bool bCol[15];
	for(i=0;i<iNum;i++)
		cMap[pSame[i].iX][pSame[i].iY]=' ';
	for(i=0;i<15;i++)
		for(j=9;j>0;j--)
			if(cMap[j][i]==' ')
				for(k=j-1;k>=0;k--)
					if(cMap[k][i]!=' ')
					{
						cMap[j][i]=cMap[k][i];
						cMap[k][i]=' ';
						break;
					}
	for(i=0;i<15;i++)
	{
		for(j=0;j<10;j++)
			if(cMap[j][i]!=' ')
				break;
		if(j==10)
			bCol[i]=false;
		else
			bCol[i]=true;
	}
	for(i=0;i<15;i++)
	{
		if(!bCol[i])
			for(j=i+1;j<15;j++)
				if(bCol[j])
				{
					for(k=0;k<10;k++)
					{
						cMap[k][i]=cMap[k][j];
						cMap[k][j]=' ';
					}
					bCol[i]=true;
					bCol[j]=false;
					break;
				}
	}
}