#include<iostream.h>
#include<iomanip.h>
#include<math.h>

void main()
{
	const double PI=3.1415926;
	double dR,dN;
	int iCaseNum,i;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>dR>>dN;
		cout<<"Scenario #"<<i+1<<":\n"<<setiosflags(ios::fixed)<<setprecision(3)<<dR/(1.0+1.0/sin(PI/dN))<<"\n"<<endl;

	}
}