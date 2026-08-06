#include<iostream.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define REF_ID 1
#define TEXT_ID 2
#define EMPTY 3

struct Refer
{
	int iSeq,iLine;
	char cNote[3][90];
};

void main()
{
	Refer rTable[1000];
	int iCorList[1000],iAppCor[1000],iAppSeq[1000],iAppNum=0,iApp,iRefNum=0,iTextLine=0,ID=EMPTY,i,j;
	bool bAppMark[1000]={false};
	char cTemp[90],*cPtr,*cPtr2;
	char *cText[40000];
	for(i=0;i<40000;i++)
		cText[i]=new char [90];
	while(gets(cTemp))
	{
		for(cPtr=cTemp;*cPtr==' ';cPtr++);
		if(*cPtr=='\0')
		{
			if(ID==REF_ID)
			{
				ID=EMPTY;
				iRefNum++;
			}
			if(ID==TEXT_ID)
			{
				cText[iTextLine][0]='\0';
				ID=EMPTY;
				iTextLine++;
			}
		}
		else 
		{
			if(ID==REF_ID)
			{
				strcpy(rTable[iRefNum].cNote[rTable[iRefNum].iLine],cTemp);
				rTable[iRefNum].iLine++;
			}
			else if(ID==TEXT_ID)
			{
				strcpy(cText[iTextLine],cTemp);
				iTextLine++;
			}
			else
			{
				if(sscanf(cTemp,"[%d]",&(rTable[iRefNum].iSeq)))
				{
					for(cPtr=cTemp;*cPtr!=']';cPtr++);
					strcpy(rTable[iRefNum].cNote[0],cPtr+1);
					rTable[iRefNum].iLine=1;
					iCorList[rTable[iRefNum].iSeq]=iRefNum;
					ID=REF_ID;
				}
				else
				{
					strcpy(cText[iTextLine],cTemp);
					iTextLine++;
					ID=TEXT_ID;
				}
			}
		}
	}
	for(i=0;i<iTextLine;i++)
		for(cPtr=cText[i];*cPtr!='\0';cPtr++)
			if(*cPtr=='[')
			{
				sscanf(cPtr,"[%d]",&iApp);
				if(!bAppMark[iApp])
				{
					iAppSeq[iAppNum]=iApp;
					iAppNum++;
					bAppMark[iApp]=true;
					iAppCor[iApp]=iAppNum;
				}
				for(cPtr2=cPtr;*cPtr2!=']';cPtr2++);
				strcpy(cTemp,cPtr2+1);
				_itoa(iAppCor[iApp],cPtr+1,10);
				for(cPtr2=cPtr;*cPtr2!='\0';cPtr2++);
				*cPtr2=']';
				strcpy(cPtr2+1,cTemp);
				cPtr=cPtr2;
			}
	for(i=iTextLine-1;cText[i][0]=='\0';i--);
	iTextLine=i+1;
	for(i=0;i<iTextLine;i++)
		printf("%s\n",cText[i]);
	for(i=0;i<iAppNum;i++)
	{
		printf("\n[%d]",i+1);
		for(j=0;j<rTable[iCorList[iAppSeq[i]]].iLine;j++)
			printf("%s\n",rTable[iCorList[iAppSeq[i]]].cNote[j]);
	}
	printf("\n");
}