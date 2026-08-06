#include<iostream.h>
#include<string.h>
#include<ctype.h>

char Reverse(char cChar)
{
	const char LETTER_REVERSE[27]={"A   3  HIL JM O   2TUVWXY5"},DIGIT_REVERSE[10]={"1SE Z  8 "};
	if(isupper(cChar))
		return LETTER_REVERSE[cChar-'A'];
	else
		return DIGIT_REVERSE[cChar-'1'];
}

bool IsPalin(char* cString)
{
	int i,j;
	for(i=0,j=strlen(cString)-1;i<j;i++,j--)
		if(cString[i]!=cString[j])
			return false;
	return true;
}

bool IsMirror(char* cString)
{
	int i,j;
	for(i=0,j=strlen(cString)-1;i<=j;i++,j--)
		if(Reverse(cString[i])==' ' || Reverse(cString[i])!=cString[j])
			return false;
	return true;
}

void main()
{
	char cString[30];
	while(cin>>cString)
	{
		cout<<cString<<" -- is ";
		if(IsPalin(cString))
		{
			if(IsMirror(cString))
				cout<<"a mirrored palindrome.";
			else
				cout<<"a regular palindrome.";
		}
		else
		{
			if(IsMirror(cString))
				cout<<"a mirrored string.";
			else
				cout<<"not a palindrome.";
		}
		cout<<"\n"<<endl;
	}
}