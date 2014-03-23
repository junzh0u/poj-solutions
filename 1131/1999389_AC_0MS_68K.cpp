#include<iostream.h>
#include<string.h>

#define LIMIT 15

void main()
{
	char cOct[LIMIT+2];
	int iDigit[3*LIMIT],iDec[3*LIMIT],i,j,iOutDigit;
	while(cin>>cOct)
	{
		if(!strcmp(cOct,"0") || !strcmp(cOct,"1"))
		{
			cout<<cOct<<" [8] = "<<cOct<<" [10]"<<endl;
			continue;
		}
		memset(iDigit,0,sizeof(iDigit));
		memset(iDec,0,sizeof(iDec));
		iDigit[0]=1;iDigit[1]=2;iDigit[2]=5;
		for(i=2;cOct[i]!='\0';i++)
		{
			for(j=0;j<3*i-3;j++)
				iDec[j]+=(cOct[i]-'0')*iDigit[j];
			for(j=3*i-4;j>=0;j--)
			{
				iDigit[j+3]+=iDigit[j]*5;
				iDigit[j+2]+=iDigit[j]*2;
				iDigit[j+1]+=iDigit[j];
				iDigit[j]=0;
			}
			for(j=3*i-1;j>0;j--)
				if(iDigit[j]>=10)
				{
					iDigit[j-1]+=iDigit[j]/10;
					iDigit[j]%=10;
				}
		}
		for(i=3*strlen(cOct)-7;i>0;i--)
			if(iDec[i]>=10)
			{
				iDec[i-1]+=iDec[i]/10;
				iDec[i]%=10;
			}
		cout<<cOct<<" [8] = 0.";
		for(iOutDigit=3*LIMIT-1;iDec[iOutDigit]==0;iOutDigit--);
		for(i=0;i<=iOutDigit;i++)
			cout<<iDec[i];
		cout<<" [10]"<<endl;
	}
}