#include<iostream.h>

void main()
{
	int iCowNum,i,iCount=1;
	cin>>iCowNum;
	for(i=2;i<=iCowNum/2;i++)
		if( (i%2==0 && iCowNum%(i/2)==0 && (iCowNum/(i/2))%2==1 && (iCowNum/(i/2))>i) || (i%2!=0 && iCowNum%i==0 && iCowNum/i>=(i+1)/2) )
			iCount++;
	cout<<iCount<<endl;
}