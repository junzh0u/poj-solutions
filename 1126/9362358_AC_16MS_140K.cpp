#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>
#include<locale>

const int SENTENCE_LEN_MAX=256;

bool isSentence(char *begin,char *end);
bool isTwoSentence(char *begin,char *end);

int main(){
	char sentence[SENTENCE_LEN_MAX+1];
	while(scanf("%s",sentence)!=EOF)
		printf(isSentence(sentence,sentence+strlen(sentence))?"YES\n":"NO\n");
	return 0;
}

bool isSentence(char *begin,char *end){
	if(begin==end)
		return false;
	if(*begin>='p' && *begin<='z')
		return begin+1==end;
	if(*begin=='N')
		return isSentence(begin+1,end);
	if(*begin=='C' || *begin=='D' || *begin=='E' || *begin=='I')
		return isTwoSentence(begin+1,end);
	return false;
}

bool isTwoSentence(char *begin,char *end){
	for(char *mid=begin+1;mid<end;mid++)
		if(isSentence(begin,mid) && isSentence(mid,end))
			return true;
	return false;
}