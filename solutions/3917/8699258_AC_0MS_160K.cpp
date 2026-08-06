#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int codeFromSymbol(char symbol){
	switch(symbol){
	case 'R':
		return 0;
	case 'P':
		return 1;
	case 'S':
		return 2;
	default:
		return -1;
	}
}

int JUDGE_TABLE[3][3]={{0,-1,1},{1,0,-1},{-1,1,0}};

int judge(char a,char b){
	return JUDGE_TABLE[codeFromSymbol(a)][codeFromSymbol(b)];
}

int main(){
	char symbols[2][80];
	while(true){
		gets(symbols[0]);
		gets(symbols[1]);
		if(symbols[0][0]=='E')
			return 0;
		int win[2]={0,0};
		for(int i=0;symbols[0][i]!='\0';i++){
			switch(judge(symbols[0][i],symbols[1][i])){
			case 1:
				win[0]++;
				break;
			case -1:
				win[1]++;
				break;
			}
		}
		printf("P1: %d\nP2: %d\n",win[0],win[1]);
	}
}