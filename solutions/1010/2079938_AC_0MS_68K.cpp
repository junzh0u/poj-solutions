#include<iostream.h>
#include<string.h>

#define NONE 0
#define BEST 1
#define TIE 2

int TypeCount(const int* iCombination,const int iStampTypeNum)
{
	int i,iStampTypeUsedNum=0;
	for(i=0;i<4;i++)
		if(iCombination[i]!=iStampTypeNum && (i==0 || iCombination[i]!=iCombination[i-1]))
			iStampTypeUsedNum++;
	return iStampTypeUsedNum;
}

int StampCount(const int* iCombination,const int iStampTypeNum)
{
	int i,iStampNum=0;
	for(i=0;i<4;i++)
		if(iCombination[i]!=iStampTypeNum)
			iStampNum++;
	return iStampNum;
}

int HighestValue(const int* iCombination,const int* iStampValue,const int iStampTypeNum)
{
	int i,iHighestStampValue=0;
	for(i=0;i<4;i++)
		if(iStampValue[iCombination[i]]>iHighestStampValue)
			iHighestStampValue=iStampValue[iCombination[i]];
	return iHighestStampValue;
}

int Compare(const int* iLeftCombination,const int* iRightCombination,const int* iStampValue,const int iStampTypeNum)
{
	if(TypeCount(iLeftCombination,iStampTypeNum)!=TypeCount(iRightCombination,iStampTypeNum))
		return TypeCount(iLeftCombination,iStampTypeNum)-TypeCount(iRightCombination,iStampTypeNum);
	if(StampCount(iLeftCombination,iStampTypeNum)!=StampCount(iRightCombination,iStampTypeNum))
		return StampCount(iRightCombination,iStampTypeNum)-StampCount(iLeftCombination,iStampTypeNum);
	if(HighestValue(iLeftCombination,iStampValue,iStampTypeNum)!=HighestValue(iRightCombination,iStampValue,iStampTypeNum))
		return HighestValue(iLeftCombination,iStampValue,iStampTypeNum)-HighestValue(iRightCombination,iStampValue,iStampTypeNum);
	return 0;
}

void main()
{
	int iStampValue[30],iStampTypeNum,iRequest,iTry[4],iCurrentState,iCombination[4],i;
	while(cin>>iStampValue[0])
	{
		iStampTypeNum=0;
		while(iStampValue[iStampTypeNum]!=0)
			cin>>iStampValue[++iStampTypeNum];
		iStampValue[iStampTypeNum]=0;
		while(cin>>iRequest)
		{
			iCurrentState=NONE;
			if(iRequest==0)
				break;
			for(iTry[0]=0;iTry[0]<=iStampTypeNum;iTry[0]++)
				for(iTry[1]=iTry[0];iTry[1]<=iStampTypeNum;iTry[1]++)
					for(iTry[2]=iTry[1];iTry[2]<=iStampTypeNum;iTry[2]++)
						for(iTry[3]=iTry[2];iTry[3]<=iStampTypeNum;iTry[3]++)
						{
							if(iStampValue[iTry[0]]+iStampValue[iTry[1]]+iStampValue[iTry[2]]+iStampValue[iTry[3]]!=iRequest)
								continue;
							if(iCurrentState==NONE || Compare(iTry,iCombination,iStampValue,iStampTypeNum)>0)
							{
								iCurrentState=BEST;
								memcpy(iCombination,iTry,sizeof(iCombination));
							}
							else if(Compare(iTry,iCombination,iStampValue,iStampTypeNum)==0)
								iCurrentState=TIE;
						}
			cout<<iRequest<<" ";
			if(iCurrentState==NONE)
				cout<<"---- none"<<endl;
			else
			{
				cout<<"("<<TypeCount(iCombination,iStampTypeNum)<<"): ";
				if(iCurrentState==TIE)
					cout<<"tie";
				else
					for(i=0;i<4;i++)
						if(iCombination[i]!=iStampTypeNum)
							cout<<iStampValue[iCombination[i]]<<" ";
				cout<<endl;
			}
		}
	}
}