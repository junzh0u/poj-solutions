#include<iostream.h>

void main()
{
	int iCaseNum,iSum,iDif,i;
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>iSum>>iDif;
		if(iSum<iDif || (iSum+iDif)%2)
			cout<<"impossible"<<endl;
		else
			cout<<(iSum+iDif)/2<<" "<<(iSum-iDif)/2<<endl;
	}
}