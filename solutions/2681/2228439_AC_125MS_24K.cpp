#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100

void main()
{
	int iCaseNum,i,j,k,iCount[2][26],iRemove;
	char cWord[2][MAX];
	scanf("%d",&iCaseNum);gets(cWord[0]);
	for(i=1;i<=iCaseNum;i++)
	{
		memset(iCount,0,sizeof(iCount));
		for(j=0;j<2;j++)
		{
			gets(cWord[j]);
			for(k=0;cWord[j][k]!='\0';k++)
				iCount[j][cWord[j][k]-'a']++;
		}
		for(iRemove=0,j=0;j<26;j++)
			iRemove+=abs(iCount[0][j]-iCount[1][j]);
		printf("Case #%d:  %d\n",i,iRemove);
	}
}