#include<iostream.h>
#include<string.h>

#define MAX 10000
bool PRIME[MAX+1];
void Initialize()
{
	int i,j;
	memset(PRIME,true,sizeof(bool)*(MAX+1));
	PRIME[0]=PRIME[1]=false;
	for(i=2;i<=MAX;i++)
		if(PRIME[i])
			for(j=2;i*j<=MAX;j++)
				PRIME[i*j]=false;
}

bool IsPrime(int iNum)
{
	int i;
	if(iNum<=MAX)
		return PRIME[iNum];
	for(i=2;i*i<=iNum;i++)
		if(PRIME[i] && iNum%i==0)
			return false;
	return true;
}

int DigitSum(int iNum)
{
	int iSum=0;
	while(iNum!=0)
	{
		iSum+=iNum%10;
		iNum/=10;
	}
	return iSum;
}

bool IsSmith(int iNum)
{
	int iDigitSum,i;
	iDigitSum=DigitSum(iNum);
	while(!IsPrime(iNum))
		for(i=2;i*i<=iNum;i++)
			if(PRIME[i] && iNum%i==0)
			{
				iDigitSum-=DigitSum(i);
				iNum/=i;
				break;
			}
	iDigitSum-=DigitSum(iNum);
	return iDigitSum==0;
}

void main()
{
	Initialize();
	int iFloor;
	while(cin>>iFloor)
	{
		if(iFloor==0)
			break;
		for(iFloor=iFloor+1;;iFloor++)
		{
			if(IsPrime(iFloor))
				continue;
			if(IsSmith(iFloor))
				break;
		}
		cout<<iFloor<<endl;
	}
}