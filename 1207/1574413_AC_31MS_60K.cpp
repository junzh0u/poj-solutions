#include<iostream.h>

int calCycle(int);

void main()
{
	int iA,iB,iLow,iHigh,iMax,i;
	while(cin>>iA>>iB)
	{
		if(iA<=iB)
		{
			iLow=iA;
			iHigh=iB;
		}
		else
		{
			iLow=iB;
			iHigh=iA;
		}
		iMax=0;
		for(i=iLow;i<=iHigh;i++)
		{
			if(iMax<calCycle(i))
				iMax=calCycle(i);
		}
		cout<<iA<<" "<<iB<<" "<<iMax<<endl;
	}
}

int calCycle(int iN)
{
	int iStep=1;
	while(iN!=1)
	{
		if(iN%2)
			iN=iN*3+1;
		else
			iN/=2;
		iStep++;
	};
	return iStep;
}