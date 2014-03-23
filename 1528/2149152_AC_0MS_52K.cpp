#include<stdio.h>
#include<math.h>

void main()
{
	printf("PERFECTION OUTPUT\n");
	int iIn,iSum,i;
	while(scanf("%d",&iIn))
	{
		if(iIn==0)
			break;
		printf("%5d  ",iIn);
		if(iIn==1)
		{
			printf("DEFICIENT\n");
			continue;
		}
		iSum=1;
		for(i=2;i<=sqrt(iIn);i++)
			if(iIn%i==0)
			{
				iSum+=i;
				if(i*i!=iIn)
					iSum+=iIn/i;
			}
		if(iSum==iIn)
			printf("PERFECT\n");
		else if(iSum<iIn)
			printf("DEFICIENT\n");
		else
			printf("ABUNDANT\n");
	}
	printf("END OF OUTPUT\n");
}