#include<iostream.h>
#include<math.h>
#define ZERO 1e-12

bool IsInt(double dValue)
{
	return fabs(dValue-int(dValue))<ZERO || fabs(dValue-int(dValue)-1)<ZERO;
}

void main()
{
	double dPower;int iExp,i;
	while(cin>>dPower && fabs(dPower)>ZERO)
	{
		iExp=1;
		for(i=2;i<40;i++)
		{
			if(dPower<0 && i%2==0)continue;
			if(IsInt(pow(fabs(dPower),1.0/i)))
				iExp=i;
		}
		cout<<iExp<<endl;
	}
}