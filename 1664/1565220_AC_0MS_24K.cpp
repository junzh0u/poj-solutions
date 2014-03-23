#include<iostream.h>

int calK(int,int);

void main()
{
	int iDataNum,iM,iN,iK,i;
	cin>>iDataNum;
	for(i=0;i<iDataNum;i++)
	{
		cin>>iM>>iN;
		iK=calK(iM,iN);
		cout<<iK<<endl;
	}
}

int calK(int iM,int iN)
{
	if(iM<0 || iN==0)
		return 0;
	if(iM==0)
		return 1;
	return calK(iM-iN,iN)+calK(iM,iN-1);
}