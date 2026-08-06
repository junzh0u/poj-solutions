#include<iostream.h>
#include<math.h>

bool isPrime(int);

void main()
{
	int iSum,iB;
	while(1)
	{
		cin>>iSum;
		if(!iSum)
			break;
		for(iB=3;iB<=iSum/2;iB+=2)
			if(isPrime(iB) && isPrime(iSum-iB))
			{
				cout<<iSum<<" = "<<iB<<" + "<<iSum-iB<<endl;
				break;
			}
	}
}

bool isPrime(int iNum)
{
	int i;
	for(i=3;i<=sqrt(iNum);i+=2)
		if(!(iNum%i))
			return false;
	return true;
}