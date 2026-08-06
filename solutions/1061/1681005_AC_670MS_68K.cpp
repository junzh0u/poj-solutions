#include<iostream.h>

void main()
{
	unsigned long iX,iY,iM,iN,iL,iChange,iIniDis,iDis,iTimes;
	cin>>iX>>iY>>iM>>iN>>iL;
	if(iM==iN && iX!=iY)
		cout<<"Impossible"<<endl;
	else 
	{
		if(iM>iN)
		{
			iChange=iM-iN;
			iIniDis=(iY-iX+iL)%iL;
		}
		else
		{
			iChange=iN-iM;
			iIniDis=(iX-iY+iL)%iL;
		}
		iTimes=iIniDis/iChange;
		iIniDis%=iChange;
		iDis=iIniDis;
		while(1)
		{
			if(iDis==0)
			{
				cout<<iTimes<<endl;
				break;
			}
			iDis+=iL;
			iTimes+=iDis/iChange;
			iDis%=iChange;
			if(iDis==iIniDis)
			{
				cout<<"Impossible"<<endl;
				break;
			}
		}
	}
}