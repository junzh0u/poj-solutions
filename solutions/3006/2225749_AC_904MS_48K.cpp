#include<iostream.h>
#include<math.h>

bool IsPrime(int);

void main()
{
	int iA,iD,iN,iNum,iCount,i;
	while(1)
	{
		cin>>iA>>iD>>iN;
		if(!iA && !iD && !iN)
			break;
		iNum=iA;
		iCount=0;
		for(i=0;;i++)
		{
			if(IsPrime(iNum))
				iCount++;
			if(iCount==iN)
			{
				cout<<iNum<<endl;
				break;
			}
			iNum+=iD;
		}
	}
}

bool IsPrime(int iNum)
{
	int i;
	if(iNum==1)
		return false;
	for(i=2;i<=sqrt(iNum);i++)
		if(!(iNum%i))
			return false;
	return true;
}