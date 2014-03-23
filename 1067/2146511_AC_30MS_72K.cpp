#include<iostream.h>
#include<math.h>
void main()
{
	const double ALPHA=(1.0+sqrt(5.0))/2.0,BETA=ALPHA+1.0;
	int iLeft,iRight,iTemp;
	while(cin>>iLeft>>iRight)
	{
		if(iLeft<iRight){iTemp=iLeft;iLeft=iRight;iRight=iTemp;}
		iTemp=ceil(iLeft/BETA);
		cout<<((iLeft==int(BETA*iTemp) && iRight==int(ALPHA*iTemp))?0:1)<<endl;
	}
}