#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

int powMod(int base,int exp,int mod){
	__int64 powRemainder[32],pos=0,remainder=1;
	for(int pos=0;exp;pos++){
		if(pos==0)
			powRemainder[pos]=base%mod;
		else
			powRemainder[pos]=(powRemainder[pos-1]*powRemainder[pos-1])%mod;
		if(exp&1)
			remainder=(remainder*powRemainder[pos])%mod;
		exp>>=1;
	}
	return (int)remainder;
}

bool isPrime(int n){
	int primes[25]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
	for(int i=0;i<25 && primes[i]<n;i++){
		int a=powMod(primes[i],n-1,n);
		if(a!=1)
			return false;
	}
	return true;
}

bool isPseudoprime(int base,int exp){
	if(powMod(base,exp,exp)!=base%exp)
		return false;
	return !isPrime(exp);
}

int main(){
	int exp,base;
	while(true){
		scanf("%d%d",&exp,&base);
		if(!exp && !base)
			return 0;
		printf(isPseudoprime(base,exp)?"yes\n":"no\n");
	}
}