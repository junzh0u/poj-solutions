#include<stdio.h>
#include<string.h>

void main()
{
	char cString[1000001];
	int *iNext,iPtr,i,iLen;
	iNext=new int[1000000];
	while(gets(cString))
	{
		if(cString[0]=='.')
			break;
		iLen=strlen(cString);
		iNext[0]=iPtr=0;
		for(i=1;i<iLen;i++)
		{
			while(iPtr>0 && cString[iPtr]!=cString[i])
				iPtr=iNext[iPtr-1];
			if(cString[iPtr]==cString[i])
				iPtr++;
			iNext[i]=iPtr;
		}
		if(iLen%(iLen-iNext[iLen-1]))
			printf("1\n");
		else
			printf("%d\n",iLen/(iLen-iNext[iLen-1]));
	}
}