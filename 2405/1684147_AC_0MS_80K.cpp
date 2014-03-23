#include<iostream.h>
#include<math.h>
#include<iomanip.h>

void main()
{
	double dDia,dVol;
	const double PI=3.1415926535;
	while(1)
	{
		cin>>dDia>>dVol;
		if(!dDia)
			break;
		cout<<setiosflags(ios::fixed)<<setprecision(3)<<pow(dDia*dDia*dDia-6*dVol/PI,1.0/3.0)<<endl;
	}
}