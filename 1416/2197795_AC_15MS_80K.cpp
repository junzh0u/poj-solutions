#include<iostream.h>
#include<stdlib.h>
#include<string.h>

#define MAX 100
#define TRUE 1
#define FALSE 2
#define REJECT 3

char cNumber[MAX];int iLen;

int Try(int iBegin,int iTarget)
{
	if(iBegin>=iLen && iTarget==0)return TRUE;
	int iReturn=FALSE,iMin,i,iTemp;char cPart[MAX];
	for(i=iBegin+1;i<=iLen;i++)
	{
		memcpy(cPart,cNumber+iBegin,i-iBegin);cPart[i-iBegin]='\0';
		iTemp=Try(i,iTarget-atoi(cPart));
		if(iTemp==TRUE)
		{
			if(iReturn==FALSE)iReturn=TRUE;
			else return REJECT;
		}
		else if(iTemp==REJECT)return REJECT;
	}
	return iReturn;
}

void Cut(int iBegin,int iTarget)
{
	char cPart[MAX];int i;
	for(i=iBegin+1;i<=iLen;i++)
	{
		memcpy(cPart,cNumber+iBegin,i-iBegin);cPart[i-iBegin]='\0';
		if(Try(i,iTarget-atoi(cPart))==TRUE)
			{cout<<" "<<cPart;Cut(i,iTarget-atoi(cPart));}
	}
}

void main()
{
	int iTarget,iMin,i,iTemp;
	while(cin>>iTarget>>cNumber && iTarget!=0)
	{
		for(i=0,iLen=strlen(cNumber),iMin=0;i<iLen;i++)iMin+=cNumber[i]-'0';
		if(iTarget<iMin)cout<<"error"<<endl;
		for(i=iTarget;i>=iMin;i--)
		{
			iTemp=Try(0,i);
			if(iTemp==REJECT)cout<<"rejected"<<endl;
			else if(iTemp==TRUE){cout<<i;Cut(0,i);cout<<endl;}
			if(iTemp!=FALSE)break;
		}
	}
}