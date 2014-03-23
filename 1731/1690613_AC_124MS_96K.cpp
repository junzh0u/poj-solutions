#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;

void main()
{
	char cOrder[220];
	cin>>cOrder;
	sort(cOrder,cOrder+strlen(cOrder));
	//cout<<cOrder<<endl;
	do
	{
		cout<<cOrder<<endl;
	}while(next_permutation(cOrder,cOrder+strlen(cOrder)));
}