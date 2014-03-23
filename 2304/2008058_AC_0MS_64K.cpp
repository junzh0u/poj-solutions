#include<iostream.h>

void main()
{
	int iPos,iFir,iSec,iThi;
	while(cin>>iPos>>iFir>>iSec>>iThi)
	{
		if(!iPos && !iFir && !iSec && !iThi)
			break;
		cout<<((iPos-iFir+40)%40+(iSec-iFir+40)%40+(iSec-iThi+40)%40)*9+1080<<endl;
	}
}