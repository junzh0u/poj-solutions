#include<iostream.h>

void main()
{
	int iCoinNum,iCaseNum,iPlaceNum,i,j,k,iLeft[1000],iRight[1000],iJudge,iFalseCoin,iResultNum=0;
	bool bCoin[3][2000],bFound;
	char cJudge;
	cin>>iCoinNum>>iCaseNum;
	for(i=0;i<3;i++)
		for(j=0;j<iCoinNum;j++)
			bCoin[i][j]=false;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iPlaceNum;
		for(j=0;j<iPlaceNum;j++)
			cin>>iLeft[j];
		for(j=0;j<iPlaceNum;j++)
			cin>>iRight[j];
		cin>>cJudge;
		if(cJudge=='>')
		{
			for(j=0;j<iCoinNum;j++)
			{
				bFound=false;
				for(k=0;k<iPlaceNum;k++)
					if(j==iLeft[k]-1 || j==iRight[k]-1)
					{
						bFound=true;
						break;
					}
				if(!bFound)
					bCoin[1][j]=true;
			}
			iJudge=-1;
		}
		else if(cJudge=='=')
			iJudge=0;
		else
		{
			for(j=0;j<iCoinNum;j++)
			{
				bFound=false;
				for(k=0;k<iPlaceNum;k++)
					if(j==iLeft[k]-1 || j==iRight[k]-1)
					{
						bFound=true;
						break;
					}
				if(!bFound)
					bCoin[1][j]=true;
			}
			iJudge=1;
		}
		for(j=0;j<iPlaceNum;j++)
		{
			bCoin[1+iJudge][iLeft[j]-1]=true;
			bCoin[1-iJudge][iRight[j]-1]=true;
		}
	}
	for(i=0;i<iCoinNum;i++)
		if(!bCoin[1][i] && !( bCoin[0][i] && bCoin[2][i] ) )
		{
			iFalseCoin=i+1;
			iResultNum++;
		}
	if(iResultNum==1)
		cout<<iFalseCoin<<endl;
	else
		cout<<0<<endl;
}