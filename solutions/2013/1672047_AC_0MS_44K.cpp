#include<iostream.h>

void main()
{
	int iStringNum,iCaseCode=0,i;
	char cString[15][26];
	while(1)
	{
		cin>>iStringNum;
		if(iStringNum==0)
			break;
		iCaseCode++;
		for(i=0;i<iStringNum;i++)
		{
			if(i%2==0)
				cin>>cString[i/2];
			else
				cin>>cString[iStringNum-i/2-1];
		}
		cout<<"SET "<<iCaseCode<<endl;
		for(i=0;i<iStringNum;i++)
		{
				cout<<cString[i];
			cout<<endl;
		}
	}
}