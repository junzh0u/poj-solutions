#include<iostream.h>

void main()
{
	int iS,i;
	cin>>iS;
	for(i=0;;i++)
		if(i*(i+1)/2>iS && !((i*(i+1)/2-iS)%2))
			break;
	cout<<i<<endl;
}