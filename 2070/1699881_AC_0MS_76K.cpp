#include<iostream.h>

void main()
{
	double dSpeed,dWeight,dStrength;
	bool bCanPlay;
	while(1)
	{
		cin>>dSpeed>>dWeight>>dStrength;
		if(!dSpeed)
			break;
		bCanPlay=false;
		if(dSpeed<=4.5 && dWeight>=150 && dStrength>=200)
		{
			bCanPlay=true;
			cout<<"Wide Receiver ";
		}
		if(dSpeed<=6.0 && dWeight>=300 && dStrength>=500)
		{
			bCanPlay=true;
			cout<<"Lineman ";
		}
		if(dSpeed<=5.0 && dWeight>=200 && dStrength>=300)
		{
			bCanPlay=true;
			cout<<"Quarterback ";
		}
		if(!bCanPlay)
			cout<<"No positions";
		cout<<endl;
	}
}