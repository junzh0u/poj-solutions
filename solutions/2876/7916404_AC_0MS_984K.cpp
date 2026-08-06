#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<memory>

const int POW_3[13]={1,3,9,27,81,243,729,2187,6561,19683,59049,177147,531441};

char cantor[13][531441+1]={"-"};

int main(){
	for(int i=1;i<=12;i++){
		memcpy(cantor[i],cantor[i-1],POW_3[i-1]);
		memset(cantor[i]+POW_3[i-1],' ',POW_3[i-1]);
		memcpy(cantor[i]+POW_3[i-1]*2,cantor[i-1],POW_3[i-1]);
		cantor[i][POW_3[i-1]*3]='\0';
	}
	int i;
	while(scanf("%d",&i)!=EOF)
		puts(cantor[i]);
	return 0;
}