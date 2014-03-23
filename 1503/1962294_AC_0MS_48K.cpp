#include<iostream.h>
#include<string.h>

void main()
{
	char cTemp[110],cLongInt[110][110];
	int iCount,i,j,iCarry=0,iResult[110]={0};
	for(iCount=0;iCount<110;iCount++)
	{
		cin>>cTemp;
		cLongInt[iCount];
		if(strcmp(cTemp,"0")==0)
			break;
		for(i=0;i<109-strlen(cTemp);i++)
			cLongInt[iCount][i]='0';
		for(i=0;i<strlen(cTemp);i++)
			cLongInt[iCount][109-strlen(cTemp)+i]=cTemp[i];
		cLongInt[iCount][109]='\0';
	}
	//for(i=0;i<iCount;i++)
	//	cout<<cLongInt[i]<<endl;
	for(i=108;i>=0;i--)
	{
		iResult[i]=iCarry;
		for(j=0;j<iCount;j++)
			iResult[i]+=cLongInt[j][i]-'0';
		iCarry=iResult[i]/10;
		iResult[i]%=10;
	}
	for(i=0;i<109;i++)
		if(iResult[i])
			break;
	for(i;i<109;i++)
		cout<<iResult[i];
	cout<<endl;
}