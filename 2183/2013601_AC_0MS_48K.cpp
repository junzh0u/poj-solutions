#include<stdio.h>
#include<stdlib.h>

#define MAX 40

void main()
{
	long iTable[MAX],i,j;
	scanf("%ld",&iTable[0]);
	for(i=1;i<MAX;i++)
	{
		iTable[i]=((iTable[i-1]%100000)/10)*((iTable[i-1]%100000)/10)%1000000;
		for(j=0;j<i;j++)
			if(iTable[j]==iTable[i])
			{
				printf("%ld %ld %ld\n",iTable[i],i-j,i);
				exit(0);
			}
	}
}