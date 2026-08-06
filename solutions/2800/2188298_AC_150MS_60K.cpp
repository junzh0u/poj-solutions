#include<stdio.h>
#define LIMIT 10

__int64 iN,iK;

__int64 Sum(__int64 iPre)
{
	__int64 iSum=0,iQuot=iK/iPre,i;
	while(iK/iQuot-iK/(iQuot+1)>LIMIT)
	{
		iSum+=(iK%(iK/(iQuot+1)+1)+iK%iPre)*(iPre-iK/(iQuot+1))/2;
		iPre=iK/((iQuot++)+1);;
	}
	for(i=2;i<=iPre;i++)iSum+=iK%i;
	return iSum;
}

void main()
{
	scanf("%I64d%I64d",&iN,&iK);
	printf("%I64d\n",(iN<=iK)?Sum(iN):(Sum(iK)+(iN-iK)*iK));
}