#include<iostream.h>

void main()
{
	int iCost,iSell,iFake,iBack;
	while(1)
	{
		cin>>iCost>>iSell>>iFake>>iBack;
		if(iCost==0 && iSell==0 && iFake==0 && iBack==0)
			break;
		cout<<iCost-iSell+iFake<<endl;
	}
}