#include<iostream.h>

int iSeq[20],iStartPos[20],iReqPos[20];

void Move(int,int);

void main()
{
	int iCaseNum,iItemNum,iMoveNum,i;
	char cItem[20][10];
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>iItemNum>>iMoveNum;
		for(i=0;i<iItemNum;i++)
		{
			cin>>cItem[i];
			iSeq[i]=i;
		}
		for(i=0;i<iMoveNum;i++)
		{
			cin>>iStartPos[i]>>iReqPos[i];
			iStartPos[i]--;
			iReqPos[i]--;
		}
		Move(iMoveNum,iItemNum);
		for(i=0;i<iItemNum;i++)
			cout<<cItem[iSeq[i]]<<" ";
		cout<<endl;
	}
}

void Move(int iMoveNum,int iItemNum)
{
	int iStartTemp[20],i,j,iUnused[20],iUnusedCount=0;
	bool bUsed[20];
	for(i=0;i<iItemNum;i++)
		bUsed[i]=false;
	for(i=0;i<iMoveNum;i++)
	{
		bUsed[iStartPos[i]]=true;
		iStartTemp[i]=iSeq[iStartPos[i]];
	}
	for(i=0;i<iItemNum;i++)
		if(!bUsed[i])
		{
			iUnused[iUnusedCount]=iSeq[i];
			iUnusedCount++;
		}
	for(i=0;i<iItemNum;i++)
		bUsed[i]=false;
	for(i=0;i<iMoveNum;i++)
	{
		bUsed[iReqPos[i]]=true;
		iSeq[iReqPos[i]]=iStartTemp[i];
	}
	j=0;
	for(i=0;i<iUnusedCount;i++)
		for(j;j<iItemNum;j++)
			if(!bUsed[j])
			{
				iSeq[j]=iUnused[i];
				j++;
				break;
			}
}