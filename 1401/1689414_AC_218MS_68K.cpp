#include<iostream.h>

void main()
{
	int iCaseNum,i,iN,iZ;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iN;
		iZ=0;
		while(1)
		{
			iN/=5;
			if(!iN)
				break;
			iZ+=iN;
		}
		cout<<iZ<<endl;
	}
}