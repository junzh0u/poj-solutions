#include<iostream.h>

void main()
{
	int iA,iB,iC,iD;
	double dPerA,dPerB;
	while(1)
	{
		cin>>iA>>iB>>iC>>iD;
		if(iA==0)
			break;
		if((iC>=iA && iD>=iB) || (iC>=iB && iD>=iA))
			cout<<"100%"<<endl;
		else
		{
			dPerA=((double)iC/iA>(double)iD/iB)?((double)iD/iB):((double)iC/iA);
			dPerB=((double)iC/iB>(double)iD/iA)?((double)iD/iA):((double)iC/iB);
			cout<<(int)(100*((dPerA>dPerB)?dPerA:dPerB))<<"%"<<endl;
		}
	}
}