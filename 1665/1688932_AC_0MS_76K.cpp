#include<iostream.h>
#include<iomanip.h>

void main()
{
	const double PI=3.1415927,INCINMIL=5280*12,SECINHOU=60*60;
	double dDia,dRev,dTime;
	int iCount=0;
	while(1)
	{
		cin>>dDia>>dRev>>dTime;
		if(dRev==0.0)
			break;
		iCount++;
		cout<<"Trip #"<<iCount<<": "<<setiosflags(ios::fixed)<<setprecision(2)<<PI*dDia*dRev/INCINMIL<<" "<<setprecision(2)<<PI*dDia*dRev*SECINHOU/(dTime*INCINMIL)<<endl;
	}
}
