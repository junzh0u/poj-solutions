#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int LEN_MAX=100;
const char JOKER='*';

char seq[LEN_MAX+1];
char card[2][LEN_MAX+1];

bool match(char target,int pos){
	if(card[0][pos]==JOKER)
		return true;
	return card[0][pos]==target || card[1][pos]==target;
}

int findMatch(char target,int begin){
	for(int pos=begin;card[0][pos]!='\0';pos++)
		if(match(target,pos))
			return pos;
	return -1;
}

bool win(){
	int begin=0;
	for(int i=0;seq[i]!='\0';i++){
		int pos=findMatch(seq[i],begin);
		if(pos==-1)
			return false;
		begin=pos+1;
	}
	return true;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		scanf("%s%s%s",seq,card[0],card[1]);
		printf(win()?"win\n":"lose\n");
	}
	return 0;
}