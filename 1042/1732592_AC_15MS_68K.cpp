#include<iostream.h>

#define LIMIT 30

void main()
{
	int iLakeNum,iHour,iFishIni[LIMIT],iFishDis[LIMIT],iDisTime[LIMIT];
	int iTimeSeq[LIMIT],iFishSumMax;
	int i,iLakeUse;
	int iTempFishSum,iTempFish[LIMIT],iTimeLeft,iTempTimeSeq[LIMIT],iFirstLake;
	int iFirstCode;
	bool bExistFish;
	while(1)
	{
		cin>>iLakeNum;
		if(!iLakeNum)
			break;
		cin>>iHour;
		for(i=0;i<iLakeNum;i++)
			cin>>iFishIni[i];
		for(i=0;i<iLakeNum;i++)
			cin>>iFishDis[i];
		for(i=0;i<iLakeNum-1;i++)
		{
			cin>>iDisTime[i];
			iDisTime[i]*=5;
		}
		iFishSumMax=0;
		iFirstLake=iFishIni[0];
		iTimeLeft=iHour*60;
		for(i=0;i<LIMIT;i++)
			iTimeSeq[i]=0;
		while(iTimeLeft>0)
		{
			iFishSumMax+=iFirstLake;
			iFirstLake-=iFishDis[0];
			if(iFirstLake<0)
				iFirstLake=0;
			iTimeSeq[0]+=5;;
			iTimeLeft-=5;
		}
		for(iLakeUse=2;iLakeUse<=iLakeNum;iLakeUse++)
		{
			iTempFishSum=0;
			iTimeLeft=iHour*60;
			for(i=0;i<LIMIT;i++)
				iTempTimeSeq[i]=0;
			for(i=0;i<iLakeUse;i++)
				iTempFish[i]=iFishIni[i];
			for(i=0;i<iLakeUse-1;i++)
				iTimeLeft-=iDisTime[i];
			bExistFish=true;
			while(iTimeLeft>0)
			{
				iFirstCode=0;
				for(i=0;i<iLakeUse;i++)
					if(iTempFish[i]>iTempFish[iFirstCode])
						iFirstCode=i;
				if(iTempFish[iFirstCode]==0)
				{
					bExistFish=false;
					break;
				}
				iTempFishSum+=iTempFish[iFirstCode];
				iTempFish[iFirstCode]-=iFishDis[iFirstCode];
				if(iTempFish[iFirstCode]<0)
					iTempFish[iFirstCode]=0;
				iTempTimeSeq[iFirstCode]+=5;
				iTimeLeft-=5;
			}
			if(!bExistFish)
			{
				iTempTimeSeq[0]+=iTimeLeft-(iTimeLeft%5);
				iTimeLeft%=5;
			}
			if(iTempFishSum>iFishSumMax)
			{
				iFishSumMax=iTempFishSum;
				for(i=0;i<LIMIT;i++)
					iTimeSeq[i]=iTempTimeSeq[i];
			}
		}
		for(i=0;i<iLakeNum;i++)
			cout<<(i?(", "):(""))<<iTimeSeq[i];
		cout<<"\nNumber of fish expected: "<<iFishSumMax<<"\n"<<endl;
	}
}