#include<iostream.h>
#include<string.h>

void main()
{
	int iMod,iRow,i,j;
	char cString[220];
	while(1)
	{
		cin>>iMod;
		if(!iMod)
			break;
		cin>>cString;
		iRow=(strlen(cString)+iMod-1)/iMod;
		for(i=0;i<iMod;i++)
			for(j=0;j<iRow;j++)
			{
				if(j==iRow-1 && i>=iMod+strlen(cString)-iRow*iMod)
					continue;
				if(j%2)
					cout<<cString[(j+1)*iMod-i-1];
					//cout<<(j+1)*iMod-i-1<<" ";
				else
					cout<<cString[i+j*iMod];
					//cout<<i+j*iMod<<" ";
			}
		cout<<endl;
	}
}