#include<stdio.h>

void main()
{
	char cIntArray[10000];
	int i,iDigitRoot;
	long iDigitSum;
	scanf("%s",cIntArray);
	while(cIntArray[0]!='0')
	{
		iDigitSum=0;
		for(i=0;cIntArray[i]!='\0';i++)
			iDigitSum+=cIntArray[i]-'0';
		iDigitRoot=(iDigitSum%9)?(iDigitSum%9):9;
		printf("%d\n",iDigitRoot);
		scanf("%s",cIntArray);
	}
}