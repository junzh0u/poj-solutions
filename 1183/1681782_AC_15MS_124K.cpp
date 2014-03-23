#include<iostream>

using namespace std;

void main()
{
	unsigned long iA,iB,iSum=0;
	cin>>iA;
	for(iB=iA+1;iB<=iA*2;iB+=1)
		if(!((iA*iB+1)%(iB-iA)) && (iB+((iA*iB+1)/(iB-iA))<iSum || !iSum))
		{
			iSum=iB+((iA*iB+1)/(iB-iA));
			//cout<<iB<<"+"<<((iA*iB+1)/(iB-iA))<<"="<<iSum<<endl;
		}
	cout<<iSum<<endl;
}