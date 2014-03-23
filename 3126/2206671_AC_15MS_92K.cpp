#include<iostream.h>
#include<string.h>
#include<stdlib.h>
#define MAX 2000

bool ISPRIME[10000];
void Initialize()
{
	int i,j;
	memset(ISPRIME,true,sizeof(ISPRIME));ISPRIME[0]=ISPRIME[1]=false;
	for(i=2;i<10000;i++)if(ISPRIME[i])for(j=2;j*i<10000;j++)ISPRIME[j*i]=false;
}

void main()
{
	Initialize();
	int iCaseNum,iCur,iLast,iMemory[10000],i,j,iTemp;char cDeque[MAX][5],cEnd[5];
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>cDeque[0]>>cEnd;
		memset(iMemory,-1,sizeof(iMemory));iMemory[atoi(cDeque[0])]=0;
		iCur=0;iLast=1;
		while(iMemory[atoi(cEnd)]==-1)
		{
			iTemp=iMemory[atoi(cDeque[iCur])];
			for(i=0;i<4;i++)
				for(j=0;j<10;j++)
				{
					cDeque[iCur][i]=(cDeque[iCur][i]-'0'+1)%10+'0';
					if(cDeque[iCur][0]=='0')continue;
					if(ISPRIME[atoi(cDeque[iCur])] && iMemory[atoi(cDeque[iCur])]==-1)
					{
						strcpy(cDeque[iLast++],cDeque[iCur]);
						iMemory[atoi(cDeque[iCur])]=iTemp+1;
					}
				}
			iCur++;
		}
		cout<<iMemory[atoi(cEnd)]<<endl;
	}
}