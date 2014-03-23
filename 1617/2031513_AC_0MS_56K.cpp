#include<iostream.h>
#include<stdlib.h>
#include<string.h>

void main()
{
	char cKey[20],cCipher[200],cTable[200][20],c;
	int i,j,iPtr;
	while(cin>>cKey)
	{
		if(strcmp(cKey,"THEEND")==0)
			break;
		cin>>cCipher;
		iPtr=0;
		for(c='A';c<='Z';c++)
			for(i=0;cKey[i]!='\0';i++)
				if(cKey[i]==c)
					for(j=0;j<strlen(cCipher)/strlen(cKey);j++,iPtr++)
						cTable[j][i]=cCipher[iPtr];
		for(i=0;i<strlen(cCipher)/strlen(cKey);i++)
			for(j=0;j<strlen(cKey);j++)
				cout<<cTable[i][j];
		cout<<endl;
	}
}