#include<iostream.h>

void main()
{
	int iCoinNum;
	while(cin>>iCoinNum)
	{
		if(iCoinNum==0)
			break;
		if(iCoinNum%2==0)
			cout<<"No Solution!"<<endl;
		else
			cout<<iCoinNum-1<<endl;
	}
}