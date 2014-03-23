#include<cstdio>
#include<cstring>

const int WORD_LEN_MAX=10000,TEXT_LEN_MAX=1000000;
char word[WORD_LEN_MAX+1],text[TEXT_LEN_MAX+1];
int wordLen,textLen;
int shift[WORD_LEN_MAX];

void computeShift(){
	shift[0]=0;
	int matchLen=0;
	for(int index=1;index<wordLen;index++){
		while(matchLen>0 && word[matchLen]!=word[index])
			matchLen=shift[matchLen-1];
		if(word[matchLen]==word[index])
			matchLen++;
		shift[index]=matchLen;
	}
}

int kmpMatch(){
	int matchLen=0,occurrenceCount=0;
	for(int index=0;index<textLen;index++){
		while(matchLen>0 && word[matchLen]!=text[index])
			matchLen=shift[matchLen-1];
		if(word[matchLen]==text[index])
			matchLen++;
		if(matchLen==wordLen){
			occurrenceCount++;
			matchLen=shift[matchLen-1];
		}
	}
	return occurrenceCount;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%s%s",word,text);
		wordLen=strlen(word);
		textLen=strlen(text);
		computeShift();
		printf("%d\n",kmpMatch());
	}
	return 0;
}