#include<iostream.h>
#include<string.h>

bool IsVowel(char cLetter)
{
	const int VOWELNUM=5;
	const char VOWEL[VOWELNUM]={'a','e','i','o','u'};
	for(int i=0;i<VOWELNUM;i++)
		if(cLetter==VOWEL[i])
			return true;
	return false;
}

bool ExistVowel(char* cString)
{
	for(int i=0;cString[i]!='\0';i++)
		if(IsVowel(cString[i]))
			return true;
	return false;
}

bool NoThreeCons(char* cString)
{
	if(strlen(cString)<3)
		return true;
	for(int i=0;cString[i+2]!='\0';i++)
		if(IsVowel(cString[i])==IsVowel(cString[i+1]) && IsVowel(cString[i])==IsVowel(cString[i+2]))
			return false;
	return true;
}

bool NoTwoConsSameVowel(char* cString)
{
	if(strlen(cString)<2)
		return true;
	for(int i=0;cString[i+1];i++)
		if(cString[i]!='e' && cString[i]!='o' && cString[i]==cString[i+1])
			return false;
	return true;
}

void main()
{
	char cPassword[30];
	while(cin>>cPassword)
	{
		if(strcmp(cPassword,"end")==0)
			break;
		cout<<"<"<<cPassword<<"> is ";
		if(ExistVowel(cPassword) && NoThreeCons(cPassword) && NoTwoConsSameVowel(cPassword))
			cout<<"acceptable."<<endl;
		else
			cout<<"not acceptable."<<endl;
	}
}