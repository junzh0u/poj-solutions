#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<ctype.h>

int XDigit(char cLetter)
{
	if(cLetter>='0' && cLetter<='9')
		return cLetter-'0';
	else
		return cLetter-'A'+10;
}

void main()
{
	char cTemp[100],cIn[50],cOut[50];
	long iNum;
	int iBaseIn,iBaseOut,i;
	while(gets(cTemp))
	{
		sscanf(cTemp,"%s %d %d",cIn,&iBaseIn,&iBaseOut);
		iNum=0;
		for(i=strlen(cIn)-1;i>=0 && cIn[i]!=' ';i--)
			iNum+=XDigit(cIn[i])*pow(iBaseIn,strlen(cIn)-1-i);
		_ltoa(iNum,cOut,iBaseOut);
		if(strlen(cOut)>7)
			printf("  ERROR\n");
		else
		{
			for(i=0;i<strlen(cOut);i++)
				if(islower(cOut[i]))
					cOut[i]=toupper(cOut[i]);
			printf("%7s\n",cOut);
		}
	}
}