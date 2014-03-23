#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>

const int N_MAX=1000000;
const int PRIME_NUM_MAX=100000;

int primeNum,primes[PRIME_NUM_MAX];
int factor[N_MAX+1];
__int64 coprimeNum[N_MAX+1];

void primeSieve(){
	for(int i=2;i<=N_MAX;i++)
		factor[i]=i;
	primeNum=0;

	for(int i=2;i<=N_MAX;i++){
		if(factor[i]==i)
			primes[primeNum++]=i;
		for(int j=0;j<primeNum;j++){
			if(primes[j]*i>N_MAX)
				break;
			factor[primes[j]*i]=primes[j];
			if(i%primes[j]==0)
				break;
		}
	}
}

void eulerCoprime(){
	coprimeNum[2]=1;
	for(int i=2;i<=N_MAX;i++){
		int f=factor[i];
		if(i==f)
			coprimeNum[i]=i-1;
		else{
			if((i/f)%f)
				coprimeNum[i]=coprimeNum[i/f]*(f-1);
			else
				coprimeNum[i]=coprimeNum[i/f]*f;
		}
	}
}

int main(){
	primeSieve();
	eulerCoprime();
	while(true){
		int n;
		scanf("%d",&n);
		if(!n)
			return 0;
		__int64 count=0;
		for(int i=2;i<=n;i++)
			count+=coprimeNum[i];
		printf("%I64d\n",count);
	}
}