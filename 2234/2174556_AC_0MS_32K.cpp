#include<iostream.h>

void main()
{
	int iPileNum,iResult,iTemp;
	while(cin>>iPileNum)
	{
		cin>>iResult;
		while(--iPileNum)
		{
			cin>>iTemp;
			iResult^=iTemp;
		}
		cout<<(iResult!=0?"Yes":"No")<<endl;
	}
}