#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<cctype>

using namespace std;

int COMPARE(char,char);

void main()
{
	int iCaseNum,i;
	char cWord[15];
	cin>>iCaseNum;
	for(i=0;i<iCaseNum;i++)
	{
		cin>>cWord;
		sort(cWord,cWord+strlen(cWord),COMPARE);
		do
		{
			cout<<cWord<<endl;
		}while(next_permutation(cWord,cWord+strlen(cWord),COMPARE));
	}
}

int COMPARE(char cLeft,char cRight)
{
	if(tolower(cLeft)==tolower(cRight))
		return cLeft<cRight;
	return tolower(cLeft)<tolower(cRight);
}