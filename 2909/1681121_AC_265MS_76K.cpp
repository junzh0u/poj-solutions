#include<iostream.h>
#include<math.h>

bool isPrime(int);

void main()
{
	int iN,i,iCount;
	while(1)
	{
		cin>>iN;
		if(!iN)
			break;
		iCount=0;
		for(i=2;i<=iN/2;i++)
			if(isPrime(i) && isPrime(iN-i))
				iCount++;
		cout<<iCount<<endl;
	}
}

bool isPrime(int iNum)
{
	int i;
	for(i=2;i<=sqrt(iNum);i++)
		if(iNum%i==0)
			return false;
	return true;
}