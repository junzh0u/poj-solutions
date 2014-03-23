#include<stdio.h>
#include<string.h>

#define MAX 1000

void Multiply(int* iSour,int iMul)
{
	int i;
	for(i=MAX-1;i>0;i--)
		iSour[i]*=iMul;
	for(i=MAX-1;i>0;i--)
		if(iSour[i]>=10)
		{
			iSour[i-1]+=iSour[i]/10;
			iSour[i]%=10;
		}
	//iSour[0]%=10;
}

void Statistic(int* iSour,int* iFreq)
{
	int i;
	for(i=0;iSour[i]==0;i++);
	for(;i<MAX;i++)
		iFreq[iSour[i]]++;
}

void main()
{
	int iFact[MAX],i,iFreq[370][10],iDesire;
	memset(iFact,0,sizeof(iFact));
	memset(iFreq,0,sizeof(iFreq));
	iFact[MAX-1]=1;
	for(i=1;i<370;i++)
	{
		Multiply(iFact,i);
		Statistic(iFact,iFreq[i]);
	}
	while(scanf("%d",&iDesire))
	{
		if(iDesire==0)
			break;
		printf("%d! --\n",iDesire);
		for(i=0;i<10;i++)
		{
			if(i%5!=0)
				printf(" ");
			printf("   (%d)",i);
			printf("%5d",iFreq[iDesire][i]);
			if(i%5==4)
				printf("\n");
		}
	}
}