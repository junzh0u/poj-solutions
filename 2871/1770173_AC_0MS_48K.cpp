#include<iostream.h>
#include<iomanip.h>

void main()
{
	double dPre,dNext;
	cin>>dPre;
	while(1)
	{
		cin>>dNext;
		if(dNext>200)
		{
			cout<<"End of Output"<<endl;
			break;
		}
		cout<<setiosflags(ios::fixed)<<setprecision(2)<<dNext-dPre<<endl;
		dPre=dNext;
	}
}