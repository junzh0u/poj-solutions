#include<stdio.h>
#include<string.h>

#define MAX 11000

inline int Hex(char* pcFirst)
{
	return ((*pcFirst<='9')?(*pcFirst-'0'):(*pcFirst-'A'+10))*16+((*(pcFirst+1)<='9')?(*(pcFirst+1)-'0'):(*(pcFirst+1)-'A'+10));
}

void main()
{
	char cMessage[2*MAX],cBlank[2*MAX];
	int iKey[MAX],i;
	gets(cMessage);gets(cBlank);
	iKey[0]=(' ')^Hex(cBlank);
	printf("%02X",iKey[0]);
	for(i=1;i<strlen(cBlank)/2;i++)
	{
		iKey[i]=(iKey[i-1]^Hex(cMessage+2*i-2))^Hex(cBlank+2*i);
		printf("%02X",iKey[i]);
	}
	printf("\n");
}