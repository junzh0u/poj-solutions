#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>
#include<algorithm>

const int DIVISOR_MAX=200,LEN_MAX=20;

int mod(int binary,int divisor,int *powRemainder){
	int pos=0,remainder=0;
	while(binary){
		if(binary&1)
			remainder=(remainder+powRemainder[pos])%divisor;
		binary>>=1;
		pos++;
	}
	return remainder;
}

char result[DIVISOR_MAX+1][LEN_MAX];

void print(int binary,char *line){
	int len=0;
	while(binary){
		line[len++]=(binary&1)+'0';
		binary>>=1;
	}
	std::reverse(line,line+len);
	line[len]='\0';
}

int main(){
	for(int divisor=1;divisor<=DIVISOR_MAX;divisor++){
		int powRemainder[LEN_MAX]={1};
		for(int i=1;i<LEN_MAX;i++)
			powRemainder[i]=(powRemainder[i-1]*10)%divisor;
		for(int binary=1;binary<(1<<LEN_MAX);binary++){
			if(mod(binary,divisor,powRemainder)==0){
				print(binary,result[divisor]);
				break;
			}
		}
	}
	while(true){
		int divisor;
		scanf("%d",&divisor);
		if(divisor==0)
			return 0;
		printf("%s\n",result[divisor]);
	}
}