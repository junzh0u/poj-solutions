#include<iostream.h>
#include<math.h>

bool IsPrime(int);

void main()
{
	bool bPrime[10001]={false};
	int i,j,iResult[10001]={0},iInput,iTemp;
	for(i=2;i<10000;i++)
		bPrime[i]=IsPrime(i);
	for(i=2;i<10001;i++)
		if(bPrime[i])
		{
			iTemp=i;
			iResult[iTemp]++;
			for(j=i+1;j<10001;j++)
				if(bPrime[j])
				{
					iTemp+=j;
					if(iTemp>10000)
						break;
					iResult[iTemp]++;
				}
		}
	while(1)
	{
		cin>>iInput;
		if(!iInput)
			break;
		cout<<iResult[iInput]<<endl;
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