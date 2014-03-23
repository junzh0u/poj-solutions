#include<iostream.h>
#include<string.h>

int Max(int iLeft,int iRight){return (iLeft>iRight)?iLeft:iRight;}
int Min(int iLeft,int iRight){return (iLeft<iRight)?iLeft:iRight;}

void Advance(char* cSection)
{
	char cTemp[300];
	strcpy(cTemp,cSection);
	strcpy(cSection+1,cTemp);
	cSection[0]='0';
}

bool Check(char* cBottom,char* cTop)
{
	int i;
	for(i=0;i<strlen(cBottom) && i<strlen(cTop);i++)
		if(cBottom[i]+cTop[i]>'1'+'2')
			return false;
	return true;
}

int MinimalLength(char* cBottom,char* cTop)
{
	int i;
	char cTopTemp[300];
	strcpy(cTopTemp,cTop);
	for(i=0;i<strlen(cBottom);i++)
	{
		if(Check(cBottom,cTopTemp))
			return Max(strlen(cBottom),strlen(cTop)+i);
		Advance(cTopTemp);
	}
	return strlen(cBottom)+strlen(cTop);
}

void main()
{
	char cBottom[300],cTop[300];
	cin>>cBottom>>cTop;
	cout<<Min(MinimalLength(cBottom,cTop),MinimalLength(cTop,cBottom))<<endl;
}