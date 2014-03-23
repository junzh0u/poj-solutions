#include<iostream.h>

void main()
{
	int N,M;
	while(cin>>N>>M)
	{
		cout<<((N&M)==M?1:0)<<endl;
	}
}