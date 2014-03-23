#include<iostream.h>
#include<string.h>

char cString[90];

bool IsUnique(int);

void main()
{
	bool bSurp;
	int i;
	while(1)
	{
		cin.getline(cString,89);
		if(cString[0]=='*')
			break;
		bSurp=true;
		for(i=0;i<(int)strlen(cString)-2;i++)
			if(!IsUnique(i))
			{
				bSurp=false;
				break;
			}
		cout<<cString<<" is"<<(bSurp?(" "):(" NOT "))<<"surprising."<<endl;
	}
}

bool IsUnique(int iDis)
{
	char cPair[90][3];
	int i,j;
	for(i=0;i<(int)strlen(cString)-iDis-1;i++)
	{
		cPair[i][0]=cString[i];
		cPair[i][1]=cString[i+iDis+1];
		cPair[i][2]='\0';
		for(j=0;j<i;j++)
			if(!strcmp(cPair[j],cPair[i]))
				return false;
	}
	return true;
}