#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int mod(int a,int b){
	while(a>=b)
		a-=b;
	return a;
}

int main(){
	int a,b;
	while(scanf("%d%d",&a,&b)!=EOF)
		printf("%d\n",mod(a,b));
	return 0;
}