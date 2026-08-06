#include<iostream.h>
#include<ctype.h>
#include<string.h>
#define LEN 100
#define DELETE '_'

const char VOWEL[6]="AEIOU";
bool IsVowel(char cLetter){for(int i=0;i<5;i++)if(cLetter==VOWEL[i])return true;return false;}

void main()
{
	bool bUse[26]={false};
	char cString[LEN];
	int iLength,i,j;
	cin.getline(cString,LEN);
	iLength=strlen(cString);
	for(i=0;i<iLength;i++)
		if(isalpha(cString[i]))
		{
			if(IsVowel(cString[i]) || bUse[cString[i]-'A'])cString[i]=DELETE;
			else bUse[cString[i]-'A']=true;
		}
	//cout<<cString<<endl;
	for(i=iLength-1;i>=0;i--)
		if(cString[i]!=DELETE && (ispunct(cString[i]) || cString[i]==' '))
		{
			//cout<<i<<endl;
			for(j=i-1;j>=0 && (cString[j]==' ' || cString[j]==DELETE);j--)
			{
				//cout<<j<<endl;
				cString[j]=DELETE;
			}
		}
	//cout<<cString<<endl;
	for(;iLength>0 && (cString[iLength-1]==DELETE || cString[iLength-1]==' ');iLength--);
	for(i=0;i<=iLength && (cString[i]==DELETE || cString[i]==' ');i++);
	//cout<<i<<" "<<iLength<<endl;
	for(;i<iLength;i++)
		if(cString[i]!=DELETE)
			cout<<cString[i];
	cout<<endl;
}