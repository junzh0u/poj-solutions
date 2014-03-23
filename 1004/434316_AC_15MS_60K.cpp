#include<iostream.h>

int main()
{
	int i;
	double dDataIn,dAver=0;
	for(i=0;i<12;i++)
	{
		cin>>dDataIn;
		dAver+=dDataIn;
	}
	dAver/=12;
	cout<<"$"<<dAver<<endl;
	return 0;
}