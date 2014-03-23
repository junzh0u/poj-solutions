//Numbers That Count

#include<iostream.h>
#include<string.h>

int judgeStr(char[]);
void invenStr(char*,char[]);
void printRes(int,char[]);

int main()
{
	char cInStr[81];
	int i;
	while(1)
	{
		cin>>cInStr;
		while(1)
		{
			if(cInStr[0]=='0' && cInStr[1]!='\0')
			{
				for(i=0;i<(int)strlen(cInStr);i++)
					cInStr[i]=cInStr[i+1];
			}
			else
				break;
		}
		if(cInStr[0]=='-')
			break;
		else
			printRes(judgeStr(cInStr),cInStr);
	}
	return 0;
}

int judgeStr(char cIniStr[])
{
	int i,j,iBreTru=0;
	char cInvStr[16][81];
	strcpy(cInvStr[0],cIniStr);
	i=0;
	while(i<15)
	{
		invenStr(cInvStr[i+1],cInvStr[i]);
		for(j=0;j<=i;j++)
		{
			if(!strcmp(cInvStr[j],cInvStr[i+1]))
			{
				iBreTru=1;
				break;
			}
		}
		if(iBreTru)
			break;
		i++;
	}
	if(i==15)
		return 0;
	else if(i==j)
	{
		if(i==0)
			return 1;
		else
			return (-1)*i;
	}
	else
		return i+1-j;
}
/*
1:   n is self-inventorying 
<0:  n is self-inventorying after j steps 
>1:  n enters an inventory loop of length k 
0:   n can not be classified after 15 iterations
*/

void invenStr(char* cRec,char cOff[])
{
	int i,iWriP=0,iCount[10]={0};
	for(i=0;i<(int)strlen(cOff);i++)
		iCount[cOff[i]-'0']++;
	for(i=0;i<10;i++)
	{
		if(iCount[i]>0)
		{
			if(iCount[i]<10)
			{
				cRec[iWriP]=iCount[i]+'0';
				iWriP++;
			}
			else if(iCount[i]>=10)
			{
				cRec[iWriP]=iCount[i]/10+'0';
				iWriP++;
				cRec[iWriP]=iCount[i]%10+'0';
				iWriP++;
			}
			cRec[iWriP]=i+'0';
			iWriP++;
		}
	}
	cRec[iWriP]='\0'; 
}

void printRes(int iVal,char cIniStr[])
{
	if(iVal==0)
		cout<<cIniStr<<" can not be classified after 15 iterations"<<endl;
	else if(iVal==1)
		cout<<cIniStr<<" is self-inventorying"<<endl;
	else if(iVal>1)
		cout<<cIniStr<<" enters an inventory loop of length "<<iVal<<endl;
	else
		cout<<cIniStr<<" is self-inventorying after "<<(-1)*iVal<<((iVal==(-1))?" step":" steps")<<endl;
}
