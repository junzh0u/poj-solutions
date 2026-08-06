#include<iostream>
#include<algorithm>
#include<cmath>
#include<iomanip>

using namespace std;

void main()
{
	int iStripyNum,i;
	double dStripy[100];
	cin>>iStripyNum;
	for(i=0;i<iStripyNum;i++)
		cin>>dStripy[i];
	if(iStripyNum!=1)
	{
		sort(dStripy,dStripy+iStripyNum);
		for(i=iStripyNum-2;i>=0;i--)
			dStripy[i]=2*sqrt(dStripy[i]*dStripy[i+1]);
	}
	cout<<setiosflags(ios::fixed)<<setprecision(3)<<dStripy[0]<<endl;
}