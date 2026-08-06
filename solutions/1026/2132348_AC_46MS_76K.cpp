#include<stdio.h>
#include<string.h>
#include<math.h>
#define MAX 210
#define EXP 15

int iLength,iKey[EXP][200];

void Initialize()
{for(int i=1;i<EXP;i++)for(int j=0;j<iLength;j++)iKey[i][j]=iKey[i-1][iKey[i-1][j]];}

void Encode(char* cMessage,int* iKey)
{
	char cTemp[MAX];
	for(int i=0;i<iLength;i++)cTemp[iKey[i]]=cMessage[i];
	cTemp[iLength]='\0';
	strcpy(cMessage,cTemp);
}

void main()
{
	int i,iTime;
	char cLine[MAX],cMessage[MAX];
	while(scanf("%d",&iLength))
	{
		if(iLength==0)break;
		for(i=0;i<iLength;i++){scanf("%d",&iKey[0][i]);iKey[0][i]--;}
		Initialize();
		gets(cLine);
		while(gets(cLine))
		{
			sscanf(cLine,"%d",&iTime);
			if(iTime==0)break;
			for(i=0;cLine[i]!=' ';i++);
			strcpy(cMessage,cLine+i+1);
			for(i=strlen(cMessage);i<iLength;i++)cMessage[i]=' ';
			cMessage[iLength]='\0';
			for(i=EXP-1;i>=0;i--)
				if(iTime>=pow(2,i)){Encode(cMessage,iKey[i]);iTime-=pow(2,i);}
			printf("%s\n",cMessage);
		}
		printf("\n");
	}
}