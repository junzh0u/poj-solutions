#include<cstdio>
#include<string>
#include<algorithm>
using namespace std;

bool IsSubSequence(char* stringBegin,char* stringEnd,char* subBegin,char* subEnd)
{
	if(stringEnd-stringBegin<subEnd-subBegin)
		return false;
	if(subBegin==subEnd)
		return true;
	while(*stringBegin!=*subBegin && stringBegin!=stringEnd)
		stringBegin++;
	return IsSubSequence(stringBegin+1,stringEnd,subBegin+1,subEnd);
}

void main()
{
	int caseNum;
	const int LEN=100;
	scanf("%d",&caseNum);
	while(caseNum--)
	{
		char string[LEN+1],sub[LEN+1];
		scanf("%s %s",string,sub);
		bool isSubOrReverseSub=IsSubSequence(string,string+strlen(string),sub,sub+strlen(sub));
		reverse(sub,sub+strlen(sub));
		isSubOrReverseSub=isSubOrReverseSub||IsSubSequence(string,string+strlen(string),sub,sub+strlen(sub));
		printf("%s\n",isSubOrReverseSub?"YES":"NO");
	}
}