#include<cstdio>
#include<cstring>

const int WORD_NUM_MAX=600,WORD_LEN_MAX=25,MESSAGE_LEN_MAX=300;

char dictionary[WORD_NUM_MAX][WORD_LEN_MAX+1],message[MESSAGE_LEN_MAX+1];
int wordNum,messageLen;

const int UNKOWN=-1;
int charToRemove[MESSAGE_LEN_MAX+1];

int match(int begin,char *word){
	int index=begin;
	while(true){
		if(*word=='\0')
			return index;
		while(true){
			if(message[index]=='\0')
				return -1;
			if(message[index]==*word){
				index++;
				break;
			}
			index++;
		}
		word++;
	}
}

int getCharToRemove(int begin){
	if(charToRemove[begin]==UNKOWN){
		charToRemove[begin]=getCharToRemove(begin+1)+1;
		for(int wordIndex=0;wordIndex<wordNum;wordIndex++){
			int end=match(begin,dictionary[wordIndex]);
			if(end!=-1){
				int temp=getCharToRemove(end)+end-begin-strlen(dictionary[wordIndex]);
				if(charToRemove[begin]>temp)
					charToRemove[begin]=temp;
			}
		}
	}
	return charToRemove[begin];
}

int main(){
	scanf("%d%d%s",&wordNum,&messageLen,message);
	for(int index=0;index<wordNum;index++)
		scanf("%s",dictionary[index]);
	for(int index=0;index<messageLen;index++)
		charToRemove[index]=UNKOWN;
	charToRemove[messageLen]=0;
	printf("%d\n",getCharToRemove(0));
	return 0;
}