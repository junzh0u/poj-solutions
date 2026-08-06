#include<cstdio>
#include<cmath>

bool IsPrime(int number)
{
	for(int factor=2;factor*factor<=number;factor++)
		if(number%factor==0)
			return false;
	return true;
}

const int UNEXIST=-1;

int MaxPrime(int upperBound)
{
	for(int i=upperBound;i>=2;i--)
		if(IsPrime(i))
			return i;
	return UNEXIST;
}

int min(int left,int right)
{
	return (left<right)?left:right;
}

void main()
{
	while(true)
	{
		int productMax,numerator,denominator;
		scanf("%d%d%d",&productMax,&numerator,&denominator);
		if(productMax==0)
			break;
		int widthMostSuitable=0,lengthMostSuitable=0;
		for(int width=2;width*width<productMax;width++)
			if(IsPrime(width))
			{
				int upperBoundA=width*denominator/numerator;
				int upperBoundB=productMax/width;
				int upperBound=min(upperBoundA,upperBoundB);
				int length=MaxPrime(upperBound);
				if(length!=UNEXIST && width*length>widthMostSuitable*lengthMostSuitable)
				{
					widthMostSuitable=width;
					lengthMostSuitable=length;
				}
			}
		printf("%d %d\n",widthMostSuitable,lengthMostSuitable);
	}
}