#include<iostream>

using namespace std;

struct WALL
{
	int iLeftX;
	int iRightX;
	int iY;
	bool bRemoved;
};

bool WallLess(WALL,WALL);

void main()
{
	int iCaseNum,iWallNum,iPassNum,iRightMax,iWallList[100],iListCode,iRemoveNum,i,j,k,l,iTemp,iTempXA,iTempXB;
	WALL wWall[100],wTemp;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iWallNum>>iPassNum;
		for(j=0;j<iWallNum;j++)
			wWall[j].bRemoved=false;
		for(j=0;j<iWallNum;j++)
		{
			cin>>iTempXA>>wWall[j].iY>>iTempXB>>wWall[j].iY;
			if(iTempXA<iTempXB)
			{
				wWall[j].iLeftX=iTempXA;
				wWall[j].iRightX=iTempXB;
			}
			else
			{
				wWall[j].iLeftX=iTempXB;
				wWall[j].iRightX=iTempXA;
			}
		}
		for(j=0;j<iWallNum;j++)
			for(k=j+1;k<iWallNum;k++)
				if(wWall[j].iRightX>wWall[k].iRightX)
				{
					wTemp=wWall[j];
					wWall[j]=wWall[k];
					wWall[k]=wTemp;
				}
		iRightMax=0;
		for(j=0;j<iWallNum;j++)
			if(wWall[j].iRightX>iRightMax)
				iRightMax=wWall[j].iRightX;
		iRemoveNum=0;
		for(j=0;j<=iRightMax;j++)
		{
			iListCode=0;
			for(k=0;k<100;k++)
				iWallList[k]=0;
			for(k=0;k<iWallNum;k++)
				if(!(wWall[k].iLeftX>j || wWall[k].iRightX<j || wWall[k].bRemoved))
				{
					iWallList[iListCode]=k;
					iListCode++;
				}
			if(iListCode>iPassNum)
			{
				for(k=0;k<iListCode-iPassNum;k++)
				{
					wWall[iWallList[iListCode-1-k]].bRemoved=true;
					iRemoveNum++;
				}
			}
		}
		cout<<iRemoveNum<<endl;
	}
}
