#include<iostream.h>

void main()
{
	int iLength,iSecNum,i,iStart,iEnd;
	while(1)
	{
		cin>>iLength>>iSecNum;
		if(!iLength)
			break;
		iLength++;
		for(i=0;i<iSecNum;i++)
		{
			cin>>iStart>>iEnd;
			iLength+=iStart-iEnd-1;
		}
		cout<<iLength<<endl;
	}
}