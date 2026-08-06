#include<stdio.h>
#include<string.h>
#include<ctype.h>
#define MAX 10000

void ToNumber(int* iNumber,char* cSymbol,int iLen)
{
	for(int i=0;i<iLen;i++)
	{
		if(isdigit(cSymbol[i]))iNumber[iLen-i-1]=cSymbol[i]-'0';
		else if(isupper(cSymbol[i]))iNumber[iLen-i-1]=cSymbol[i]-'A'+10;
		else iNumber[iLen-i-1]=cSymbol[i]-'a'+36;
	}
}

void ToSymbol(char* cSymbol,int* iNumber,int iLen)
{
	for(int i=0;i<iLen;i++)
	{
		if(iNumber[i]<10)cSymbol[iLen-i-1]=iNumber[i]+'0';
		else if(iNumber[i]<36)cSymbol[iLen-i-1]=iNumber[i]-10+'A';
		else cSymbol[iLen-i-1]=iNumber[i]-36+'a';
	}
	cSymbol[iLen]='\0';
}

void Plus(int* iDes,int* iSour,int iLen,int iMul)
{for(int i=0;i<iLen;i++)iDes[i]+=iSour[i]*iMul;}

void Print(int* iArray,int iLen)
{for(int i=0;i<iLen;i++)printf("%d",iArray[i]);printf("\n");}

void main()
{
	char cIn[MAX],cOut[MAX];
	int iCaseNum,iInBase,iInTemp[MAX],iOutBase,iOutTemp[MAX],iRoot[MAX],iRootBit,iInLen,iOutLen,i,j;
	scanf("%d",&iCaseNum);
	while(iCaseNum--)
	{
		scanf("%d %d %s",&iInBase,&iOutBase,cIn);
		iInLen=strlen(cIn);
		ToNumber(iInTemp,cIn,iInLen);

		//printf("In:%s\nInTemp:",cIn);Print(iInTemp,iInLen);

		memset(iOutTemp,0,sizeof(iOutTemp));memset(iRoot,0,sizeof(iRoot));
		iRoot[0]=1;iRootBit=1;
		for(i=0;i<iInLen;i++)
		{
			Plus(iOutTemp,iRoot,iRootBit,iInTemp[i]);
			iOutLen=iRootBit;
			for(j=0;j<iRootBit;j++)
				iRoot[j]*=iInBase;
			for(j=0;j<iRootBit;j++)
			{
				if(iRoot[j]>=iOutBase)
				{
					iRoot[j+1]+=iRoot[j]/iOutBase;
					iRoot[j]%=iOutBase;
				}
			}
			while(iRoot[iRootBit]!=0)
			{
				if(iRoot[iRootBit]>=iOutBase)
				{
					iRoot[iRootBit+1]=iRoot[iRootBit]/iOutBase;
					iRoot[iRootBit]%=iOutBase;
				}
				iRootBit++;
			}
		}
		for(i=0;i<iOutLen;i++)
		{
			if(iOutTemp[i]>=iOutBase)
			{
				iOutTemp[i+1]+=iOutTemp[i]/iOutBase;
				iOutTemp[i]%=iOutBase;
			}
		}
		while(iOutTemp[iOutLen]!=0)
		{
			if(iOutTemp[iOutLen]>=iOutBase)
			{
				iOutTemp[iOutLen+1]+=iOutTemp[iOutLen]/iOutBase;
				iOutTemp[iOutLen]%=iOutBase;
			}
			iOutLen++;
		}
		ToSymbol(cOut,iOutTemp,iOutLen);

		//printf("OutTemp:");Print(iOutTemp,iOutLen);printf("Out:%s\n",cOut);
		printf("%d %s\n%d %s\n\n",iInBase,cIn,iOutBase,cOut);
	}
}