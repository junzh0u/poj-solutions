#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int main(){
	while(true){
		int guessNum;
		scanf("%d",&guessNum);
		if(!guessNum)
			return 0;
		int prev=0;
		while(guessNum--){
			int guess;
			scanf("%d",&guess);
			if(guess!=prev)
				printf("%d ",guess);
			prev=guess;
		}
		printf("$\n");
	}
}