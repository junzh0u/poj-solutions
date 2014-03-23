#include<iostream.h>
#include<string.h>
#include<math.h>

long Transform(char* cDigit,long iBase)
{
	long iRet=0,i;
	for(i=0;i<strlen(cDigit);i++)
	{
		if(cDigit[i]-'0'>=iBase)
			return -1;
		iRet+=(cDigit[i]-'0')*pow(iBase,strlen(cDigit)-i-1);
	}
	return iRet;
}

void main()
{
	long iCaseNum,iBase;
	char cLeft[10],cRight[10],cPro[10];
	cin>>iCaseNum;
	while(iCaseNum--)
	{
		cin>>cLeft>>cRight>>cPro;
		for(iBase=2;iBase<=16;iBase++)
			if(Transform(cLeft,iBase)!=-1 && Transform(cRight,iBase)!=-1 && Transform(cPro,iBase)!=-1 && Transform(cLeft,iBase)*Transform(cRight,iBase)==Transform(cPro,iBase))
				break;
		if(iBase==17)
			iBase=0;
		cout<<iBase<<endl;
	}
}