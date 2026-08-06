#include<iostream.h>
#include<math.h>
#include<iomanip.h>

void main()
{
	double dVs,dR,dC,dW;
	int iCaseNum,i;
	cin>>dVs>>dR>>dC>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>dW;
		cout<<setiosflags(ios::fixed)<<setprecision(3)<<(dR*dC*dW*dVs)/sqrt(1+dR*dR*dR*dC*dW*dW)<<endl;
	}
}