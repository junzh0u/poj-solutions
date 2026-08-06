#include<iostream.h>

void main()
{
	double dPrice[10001]={0.0},dPriceM[10001],dPriceN[10001],dSum;
	int iM,iN,iPriceNum,i;
	bool bBull=false;
	cin>>iM>>iN>>iPriceNum;
	for(i=1;i<=iPriceNum;i++)cin>>dPrice[i];
	dSum=0.0;for(i=0;i<iM;i++)dSum+=dPrice[i];
	for(i=iM;i<=iPriceNum;i++)
	{
		dSum+=dPrice[i]-dPrice[i-iM];
		dPriceM[i]=dSum/iM;
	}
	dSum=0.0;for(i=0;i<iN;i++)dSum+=dPrice[i];
	for(i=iN;i<=iPriceNum;i++)
	{
		dSum+=dPrice[i]-dPrice[i-iN];
		dPriceN[i]=dSum/iN;
	}
	bBull=(dPriceM[iN]>dPriceN[iN]);
	cout<<(bBull?"BUY":"SELL")<<" ON DAY "<<iN<<endl;
	for(i=iN+1;i<=iPriceNum;i++)
		if((dPriceM[i]>dPriceN[i])!=bBull)
		{
			bBull=!bBull;
			cout<<(bBull?"BUY":"SELL")<<" ON DAY "<<i<<endl;
		}
}
