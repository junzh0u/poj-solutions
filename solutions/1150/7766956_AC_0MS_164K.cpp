#define _CRT_SECURE_NO_WARNINGS
#define DEBUG

#include<cstdio>

int countFactor(int last,int factor){
	if(last==0)
		return 0;
	return last/factor+countFactor(last/factor,factor);
}

int countLastDigitInOdd(int last,int factor){
	if(last==0)
		return 0;
	return last/10+(last%10>=factor)+countLastDigitInOdd(last/5,factor);
}

int countLastDigit(int last,int factor){
	if(last==0)
		return 0;
	return countLastDigit(last/2,factor)+countLastDigitInOdd(last,factor);
}

int pow3mod10(int exp){
	static const int table[4]={1,3,9,7};
	return table[exp%4];
}

int pow7mod10(int exp){
	static const int table[4]={1,7,9,3};
	return table[exp%4];
}

int pow9mod10(int exp){
	static const int table[2]={1,9};
	return table[exp%2];
}

int pow2mod10(int exp){
	if(exp==0)
		return 1;
	static const int table[4]={6,2,4,8};
	return table[exp%4];
}

int main(){
	int n,m;
	while(scanf("%d%d",&n,&m)!=EOF){
		int num2=countFactor(n,2)-countFactor(n-m,2);
		int num5=countFactor(n,5)-countFactor(n-m,5);
		if(num5>num2){
			printf("5\n");
			continue;
		}
		int num3=countLastDigit(n,3)-countLastDigit(n-m,3);
		int num7=countLastDigit(n,7)-countLastDigit(n-m,7);
		int num9=countLastDigit(n,9)-countLastDigit(n-m,9);
		int lastDigit=(pow2mod10(num2-num5)*pow3mod10(num3)*pow7mod10(num7)*pow9mod10(num9))%10;
		printf("%d\n",lastDigit);
	}
	return 0;
}