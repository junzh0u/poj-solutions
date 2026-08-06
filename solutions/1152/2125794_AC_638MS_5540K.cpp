#include<stdio.h>
#include<ctype.h>
#include<string.h>

short REMAINDER[70][40000];
void Initialize()
{
	int i,j;
	for(i=0;i<70;i++)
	{
		REMAINDER[i][0]=1;
		for(j=1;j<40000;j++)
			REMAINDER[i][j]=(REMAINDER[i][j-1]*(i+2))%(i+1);
	}
}

void ToNumber(char* cSymbol,int iLen)
{
	int i;
	for(i=0;i<iLen;i++)
	{
		if(isdigit(cSymbol[i]))cSymbol[i]-='0';
		else if(isupper(cSymbol[i]))cSymbol[i]-='A'-10;
		else cSymbol[i]-='a'-36;
	}
}

void main()
{
	char cInteger[40000];
	int iBase,iRemainder,i,iLen;
	Initialize();
	while(scanf("%s",cInteger)!=EOF)
	{
		iBase=2;iLen=strlen(cInteger);
		/*for(i=0;i<iLen;i++)
			printf("%d ",cInteger[i]);
		printf("\n");*/
		ToNumber(cInteger,iLen);
		/*for(i=0;i<iLen;i++)
			printf("%d ",cInteger[i]);
		printf("\n");*/
		for(i=0;i<iLen;i++)
			if(cInteger[i]>=iBase)
				iBase=cInteger[i]+1;
		for(;iBase<=62;iBase++)
		{
			iRemainder=0;
			for(i=0;i<iLen;i++)
				iRemainder+=cInteger[i]*REMAINDER[iBase-2][iLen-i-1];
			if(iRemainder%(iBase-1)==0)
				break;
		}
		if(iBase==63)printf("such number is impossible!\n");
		else printf("%d\n",iBase);
	}
}
