#include<cstdio>
#include<cstring>
#include<algorithm>
#include<locale>

const int WORD_LEN_MAX=120;
const int WORD_NUM_MAX=WORD_LEN_MAX;

char identifier[WORD_LEN_MAX+1];
char words[WORD_NUM_MAX][WORD_LEN_MAX+1];
int wordNum;

bool splitInCppStyle(){
	wordNum=0;
	char *identifierEnd=identifier+strlen(identifier);
	char *wordBegin=identifier;
	while(true){
		if(wordBegin>=identifierEnd)
			return false;

		char *wordEnd=std::find(wordBegin+1,identifierEnd,'_');
		int wordLen=wordEnd-wordBegin;
		char word[WORD_LEN_MAX];
		strncpy(word,wordBegin,wordLen);
		word[wordLen]='\0';
		for(int index=0;index<wordLen;index++)
			if(!islower(word[index]))
				return false;
		
		strcpy(words[wordNum++],word);

		if(wordEnd==identifierEnd)
			return true;
		wordBegin=wordEnd+1;
	}
	return true;
}

bool splitInJavaStyle(){
	wordNum=0;
	char *identifierEnd=identifier+strlen(identifier);
	char *wordBegin=identifier;
	while(true){
		char *wordEnd=std::find_if(wordBegin+1,identifierEnd,&isupper);
		int wordLen=wordEnd-wordBegin;
		char word[WORD_LEN_MAX];
		strncpy(word,wordBegin,wordLen);
		word[wordLen]='\0';
		if(wordNum!=0){
			if(!isupper(word[0]))
				return false;
			word[0]=tolower(word[0]);
		}
		for(int index=0;index<wordLen;index++)
			if(!islower(word[index]))
				return false;
		
		strcpy(words[wordNum++],word);

		if(wordEnd==identifierEnd)
			return true;
		wordBegin=wordEnd;
	}
	return true;
}

void printInCppStyle(){
	for(int wordIndex=0;wordIndex<wordNum;wordIndex++){
		if(wordIndex!=0)
			printf("_");
		printf("%s",words[wordIndex]);
	}
	printf("\n");
}

void printInJavaStyle(){
	for(int wordIndex=0;wordIndex<wordNum;wordIndex++){
		if(wordIndex==0)
			printf("%c",words[wordIndex][0]);
		else
			printf("%c",toupper(words[wordIndex][0]));
		printf("%s",words[wordIndex]+1);
	}
	printf("\n");
}

int main(){
	scanf("%s",identifier);
	if(splitInCppStyle())
		printInJavaStyle();
	else if(splitInJavaStyle())
		printInCppStyle();
	else
		printf("Error!\n");
	return 0;
}