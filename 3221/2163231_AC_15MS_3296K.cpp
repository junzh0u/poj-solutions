#include<iostream.h>
#include<string.h>
#define MAX 7*7*7*7*7*7*7

struct State
{
	char cConfig[8];
	int iSpace;
};

int iMemory[MAX];
State sDeque[MAX];
const char cDir[7][4]={"246","26","013","24","035","46","015"};
const int iFactorial[7]={0,1,2,2*3,2*3*4,2*3*4*5,2*3*4*5*6};

int Index(char* cConfig)
{
	int iReturn=0,iReverse,i,j;
	for(i=1;i<7;i++)
	{
		iReverse=0;
		for(j=0;j<i;j++)
			if(cConfig[j]>cConfig[i])
				iReverse++;
		iReturn+=iFactorial[i]*iReverse;
	}
	return iReturn;
}

void main()
{
	int iCaseNum,iCur,iLast,i;
	char cTarget[8],cCurrent[8]="0123456";
	memset(iMemory,-1,sizeof(iMemory));
	iLast=1;
	strcpy(sDeque[0].cConfig,cCurrent);
	sDeque[0].iSpace=0;
	iMemory[0]=0;
	for(iCur=0;sDeque[iCur].cConfig[0]!='\0';iCur++)
	{
		for(i=0;i<strlen(cDir[sDeque[iCur].iSpace]);i++)
		{
			strcpy(cCurrent,sDeque[iCur].cConfig);
			cCurrent[sDeque[iCur].iSpace]=cCurrent[cDir[sDeque[iCur].iSpace][i]-'0'];
			cCurrent[cDir[sDeque[iCur].iSpace][i]-'0']='0';
			if(iMemory[Index(cCurrent)]==-1)
			{
				if(iLast==2538)
					cout<<iCur<<"\t"<<sDeque[iCur].cConfig<<"\t"<<sDeque[iCur].iSpace<<endl;
				strcpy(sDeque[iLast].cConfig,cCurrent);
				sDeque[iLast].iSpace=cDir[sDeque[iCur].iSpace][i]-'0';
				iMemory[Index(cCurrent)]=iMemory[Index(sDeque[iCur].cConfig)]+1;
				iLast++;
			}
		}
	}
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>cTarget;
		cout<<iMemory[Index(cTarget)]<<endl;
	}
}