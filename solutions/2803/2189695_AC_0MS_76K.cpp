#include<iostream.h>
#include<string.h>

void ExpandSuffix(char* cWord)
{
	int iLen=strlen(cWord);
	if(cWord[iLen-2]=='e' && cWord[iLen-1]=='r')
	{
		cWord[iLen-2]='\0';
		cout<<"one who "<<cWord<<"s";
	}
	else if(cWord[iLen-3]=='i' && cWord[iLen-2]=='n' && cWord[iLen-1]=='g')
	{
		cWord[iLen-3]='\0';
		cout<<"to actively "<<cWord;
	}
	else if(cWord[iLen-3]=='i' && cWord[iLen-2]=='z' && cWord[iLen-1]=='e')
	{
		cWord[iLen-3]='\0';
		cout<<"change into "<<cWord;
	}
	else if(cWord[iLen-1]=='s')
	{
		cWord[iLen-1]='\0';
		cout<<"multiple instances of "<<cWord;
	}
	else if(cWord[iLen-4]=='t' && cWord[iLen-3]=='i' && cWord[iLen-2]=='o' && cWord[iLen-1]=='n')
	{
		cWord[iLen-4]='\0';
		cout<<"the process of "<<cWord<<"ing";
	}
	else
		cout<<cWord;
}

void ExpandPrefix(char* cWord)
{
	if(cWord[0]=='a' && cWord[1]=='n' && cWord[2]=='t' && cWord[3]=='i')
	{
		cout<<"against ";
		ExpandSuffix(cWord+4);
	}
	else if(cWord[0]=='p' && cWord[1]=='o' && cWord[2]=='s' && cWord[3]=='t')
	{
		cout<<"after ";
		ExpandSuffix(cWord+4);
	}
	else if(cWord[0]=='p' && cWord[1]=='r' && cWord[2]=='e')
	{
		cout<<"before ";
		ExpandSuffix(cWord+3);
	}
	else if(cWord[0]=='r' && cWord[1]=='e')
	{
		ExpandSuffix(cWord+2);
		cout<<" again";
	}
	else if(cWord[0]=='u' && cWord[1]=='n')
	{
		cout<<"not ";
		ExpandSuffix(cWord+2);
	}
	else
		ExpandSuffix(cWord);
}

void main()
{
	int iWordNum;char cWord[110];
	cin>>iWordNum;
	while(iWordNum--)
	{
		cin>>cWord;
		ExpandPrefix(cWord);
		cout<<endl;
	}
}