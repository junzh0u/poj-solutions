#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<cstring>

const int WORD_LEN_MAX=10;

char word[WORD_LEN_MAX+1];
int len;

int combine(int total,int part){
	if(total<part)
		return 0;
	if(total-part<part)
		return combine(total,total-part);
	int result=1;
	for(int i=0;i<part;i++)
		result=result*(total-i)/(i+1);
	return result;
}

int count(char first,int len){
	return combine('z'-first,len-1);
}

bool legal(){
	for(int i=0;i+1<len;i++)
		for(int j=i+1;j<len;j++)
			if(word[i]>=word[j])
				return false;
	return true;
}

int main(){
	scanf("%s",word);
	len=strlen(word);
	if(!legal()){
		printf("0\n");
		return 0;
	}
	int code=1;
	char last='a';
	for(int i=0;i<len;i++){
		for(char first=last;first<word[i];first++)
			code+=count(first,len-i);
		last=word[i]+1;
	}
	for(int i=1;i<len;i++)
		code+=combine(26,i);
	printf("%d\n",code);
	return 0;
}