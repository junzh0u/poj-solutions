#include<iostream>
#include<algorithm>

using namespace std;

struct Platform
{
	int iLeft;
	int iRight;
	int iHeight;
	int iLeftTime;
	int iRightTime;
};

bool Compare(Platform plaLeft,Platform plaRight)
{
	return plaLeft.iHeight>plaRight.iHeight;
}

void main()
{
	int iCaseNum,iPlaNum,iMax,i,j,iTemp;
	bool bLeft,bRight;
	Platform plaData[1100];
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iPlaNum>>plaData[0].iLeft>>plaData[0].iHeight>>iMax;
		plaData[0].iRight=plaData[0].iLeft;
		plaData[0].iLeftTime=plaData[0].iRightTime=0;
		for(i=1;i<=iPlaNum;i++)
		{
			cin>>plaData[i].iLeft>>plaData[i].iRight>>plaData[i].iHeight;
			plaData[i].iLeftTime=plaData[i].iRightTime=INT_MAX;
		}
		plaData[iPlaNum+1].iHeight=0;
		plaData[iPlaNum+1].iLeftTime=plaData[iPlaNum+1].iRightTime=INT_MAX;
		sort(plaData+1,plaData+iPlaNum+1,Compare);
		for(i=0;i<=iPlaNum;i++)
		{
			if(plaData[i].iLeftTime==INT_MAX || plaData[i].iRightTime==INT_MAX)
				continue;
			bLeft=bRight=true;
			for(j=i+1;j<=iPlaNum+1 && (bLeft || bRight);j++)
			{
				if(plaData[j].iHeight<plaData[i].iHeight-iMax)
					break;
				if(j==iPlaNum+1)
					plaData[j].iLeft=plaData[j].iRight=plaData[i].iLeft;
				if(bLeft && plaData[j].iLeft<=plaData[i].iLeft && plaData[j].iRight>=plaData[i].iLeft)
				{
					iTemp=plaData[i].iHeight-plaData[j].iHeight+plaData[i].iLeft-plaData[j].iLeft+plaData[i].iLeftTime;
					if(iTemp<plaData[j].iLeftTime)
						plaData[j].iLeftTime=iTemp;
					iTemp=plaData[i].iHeight-plaData[j].iHeight+plaData[j].iRight-plaData[i].iLeft+plaData[i].iLeftTime;
					if(iTemp<plaData[j].iRightTime)
						plaData[j].iRightTime=iTemp;
					bLeft=false;
				}
				if(j==iPlaNum+1)
					plaData[j].iLeft=plaData[j].iRight=plaData[i].iRight;
				if(bRight && plaData[j].iLeft<=plaData[i].iRight && plaData[j].iRight>=plaData[i].iRight)
				{
					iTemp=plaData[i].iHeight-plaData[j].iHeight+plaData[i].iRight-plaData[j].iLeft+plaData[i].iRightTime;
					if(iTemp<plaData[j].iLeftTime)
						plaData[j].iLeftTime=iTemp;
					iTemp=plaData[i].iHeight-plaData[j].iHeight+plaData[j].iRight-plaData[i].iRight+plaData[i].iRightTime;
					if(iTemp<plaData[j].iRightTime)
						plaData[j].iRightTime=iTemp;
					bRight=false;
				}
			}
		}
		cout<<plaData[i].iLeftTime<<endl;
	}
}
