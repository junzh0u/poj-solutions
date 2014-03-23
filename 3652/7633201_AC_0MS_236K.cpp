#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>
#include<memory>

const int C_MAX=65536,BIT_NUM=16;
const char UNCERTAIN='?';
int a,b,c,s;
bool appear[C_MAX];
char state[BIT_NUM];

void setState(int s){
	for(int i=0;i<BIT_NUM;i++){
		state[i]=((s&1)?'1':'0');
		s>>=1;
	}
}

void changeState(int s){
	for(int i=0;i<BIT_NUM;i++){
		if(state[i]!=((s&1)?'1':'0'))
			state[i]=UNCERTAIN;
		s>>=1;
	}
}

void printState(){
	for(int i=BIT_NUM-1;i>=0;i--)
		printf("%c",state[i]);
	printf("\n");
}
/*
bool allUncertain(){
}
*/
int main(){
	while(true){
		scanf("%d",&a);
		if(a==0)
			return 0;
		scanf("%d%d%d",&b,&c,&s);
		memset(appear,0,sizeof(bool)*c);
		setState(s);
		appear[s]=true;
		while(true){
			s=(a*s+b)%c;
			if(appear[s])
				break;
			appear[s]=true;
			changeState(s);
		}
		printState();
	}
}