#define _CRT_SECURE_NO_WARNINGS

#undef _DEBUG

#include<cstdio>
#include<cstring>

const int WORD_NUM_MAX=200,WORD_LEN_MAX=50;
const char END[]="#";

struct Text{
	int wordNum;
	char words[WORD_NUM_MAX+1][WORD_LEN_MAX+1];
	bool read(){
		wordNum=0;
		while(true){
			if(scanf("%s",words[wordNum])==EOF)
				return false;
			if(!strcmp(words[wordNum],END))
				return true;
			wordNum++;
		}
	}
};

Text textA,textB;
int commonLen[WORD_LEN_MAX+1][WORD_LEN_MAX+1];
int state[WORD_LEN_MAX+1][WORD_LEN_MAX+1];

int main(){
	while(true){
		if(!textA.read())
			return 0;
		textB.read();
		for(int endA=0;endA<=textA.wordNum;endA++)
			commonLen[endA][0]=0;
		for(int endB=0;endB<=textB.wordNum;endB++)
			commonLen[0][endB]=0;
		for(int endA=1;endA<=textA.wordNum;endA++){
			for(int endB=1;endB<=textB.wordNum;endB++){
				if(strcmp(textA.words[endA-1],textB.words[endB-1])){
					commonLen[endA][endB]=commonLen[endA-1][endB-1];
					state[endA][endB]=0;
				}
				else{
					commonLen[endA][endB]=commonLen[endA-1][endB-1]+1;
					state[endA][endB]=1;
				}
				if(commonLen[endA][endB]<commonLen[endA-1][endB]){
					commonLen[endA][endB]=commonLen[endA-1][endB];
					state[endA][endB]=2;
				}
				if(commonLen[endA][endB]<commonLen[endA][endB-1]){
					commonLen[endA][endB]=commonLen[endA][endB-1];
					state[endA][endB]=3;
				}
			}
		}
		char commonSequence[WORD_NUM_MAX][WORD_LEN_MAX+1];
		int len=0,endA=textA.wordNum,endB=textB.wordNum;
		while(endA && endB){
			switch(state[endA][endB]){
			case 0:
				endA--;endB--;break;
			case 1:
				strcpy(commonSequence[len++],textA.words[endA-1]);
				endA--;endB--;break;
			case 2:
				endA--;break;
			case 3:
				endB--;break;
			}
		}
		for(int i=len-1;i>0;i--)
			printf("%s ",commonSequence[i]);
		printf("%s\n",commonSequence[0]);
	}
}