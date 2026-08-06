#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int WORD_LEN_MAX=10;

int main(){
	char word[WORD_LEN_MAX+1];
	char group[WORD_LEN_MAX+1]="";
	while(gets(word)!=NULL){
		for(int match=0;;match++){
			if(word[match]=='\0'){
				group[match]=word[match];
				break;
			}
			else if(group[match]!=word[match]){
				group[match]=word[match];
				group[match+1]='\0';
				break;
			}
			putchar(' ');
		}
		puts(word);
	}
	return 0;
}