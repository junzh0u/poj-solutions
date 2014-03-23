#include<iostream.h>
#include<iomanip.h>

void main()
{
	int iN,i;
	double dFac,dE=1.0;
	cout<<"n e\n- -----------\n0 1"<<endl;
	for(iN=1;iN<=9;iN++)
	{
		dFac=1.0;
		for(i=1;i<=iN;i++)
			dFac*=i;
		dE+=1/dFac;
		cout<<iN<<" "<<setprecision(10)<<dE<<endl;
	}
}