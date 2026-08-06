#include<iostream.h>
#include<string.h>
#define MAX 300

int iTable[MAX][MAX];
inline int Max(int a,int b){return (a>b)?a:b;}

void main()
{
	char cLeft[MAX],cRight[MAX];
	int iLeftLen,iRightLen,i,j;
	while(cin>>cLeft>>cRight)
	{
		iLeftLen=strlen(cLeft);iRightLen=strlen(cRight);
		memset(iTable,0,sizeof(iTable));
		for(i=1;i<=iLeftLen;i++)
			for(j=1;j<=iRightLen;j++)
			{
				if(cLeft[i-1]==cRight[j-1])
					iTable[i][j]=iTable[i-1][j-1]+1;
				else
					iTable[i][j]=Max(iTable[i-1][j],iTable[i][j-1]);
			}
		cout<<iTable[iLeftLen][iRightLen]<<endl;
	}
}