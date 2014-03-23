#include<iostream.h>
#include<math.h>
#include<iomanip.h>

#define LIMIT 21

void main()
{
	int i,iN[LIMIT]={3},iTempN,iYear;
	double dBitTemp[LIMIT],dBit[LIMIT]={4.0};
	for(i=1;i<LIMIT;i++)
		dBit[i]=2.0*dBit[i-1];
	dBitTemp[0]=log(1)/log(2)+log(2)/log(2)+log(3)/log(2);
	for(i=1;i<LIMIT;i++)
	{
		dBitTemp[i]=dBitTemp[i-1];
		iTempN=iN[i-1];
		while(1)
		{
			iTempN++;
			dBitTemp[i]+=log(iTempN)/log(2);
			if(dBitTemp[i]>=dBit[i])
			{
				dBitTemp[i]-=log(iTempN)/log(2);
				iTempN--;
				iN[i]=iTempN;
				break;
			}
		}
	}
	while(1)
	{
		cin>>iYear;
		if(!iYear)
			break;
		cout<<iN[iYear/10-196]<<endl;
	}
}
