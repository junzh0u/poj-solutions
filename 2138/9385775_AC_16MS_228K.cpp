#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>

const int DICTIONARY_SIZE_MAX=1000,WORD_LEN_MAX=80,NONE=-1;
int dictionarySize;
char dictionary[DICTIONARY_SIZE_MAX][WORD_LEN_MAX+1],*longestWord;
bool visited[DICTIONARY_SIZE_MAX];

bool possible(char *from,char *to){
	if(strlen(to)!=strlen(from)+1)
		return false;
	for(int i=0;;i++){
		if(from[i]=='\0')
			return true;
		if(from[i]==to[i])
			continue;
		return strcmp(from+i,to+i+1)==0;
	}
}

void search(char *begin){
#ifdef _DEBUG
	printf("# Search: %s\tLongest: %s\n",begin,longestWord);
#endif
	for(int i=0;i<dictionarySize;i++){
		if(visited[i])
			continue;
		if(!possible(begin,dictionary[i]))
			continue;
		if(strlen(longestWord)<strlen(dictionary[i]))
			longestWord=dictionary[i];
		visited[i]=true;
		search(dictionary[i]);
	}
}

int main(){
	char begin[WORD_LEN_MAX+1];
	scanf("%d%s",&dictionarySize,begin);
	for(int i=0;i<dictionarySize;i++){
		scanf("%s",dictionary[i]);
		visited[i]=false;
	}
	longestWord=begin;
	search(begin);
	printf("%s\n",longestWord);
	return 0;
}