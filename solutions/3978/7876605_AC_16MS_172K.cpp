#define _CRT_SECURE_NO_WARNINGS

#include<cstdio>
#include<algorithm>

const int N_MAX=10000,PRIME_NUM_MAX=10000;

int primeNum,primes[PRIME_NUM_MAX];
bool isPrime[N_MAX+1];
int countBefore[N_MAX+1];

void primeSieve(){
	for(int i=2;i<=N_MAX;i++)
		isPrime[i]=true;
	primeNum=0;

	for(int i=2;i<=N_MAX;i++){
		if(isPrime[i])
			primes[primeNum++]=i;
		for(int j=0;j<primeNum;j++){
			if(primes[j]*i>N_MAX)
				break;
			isPrime[primes[j]*i]=false;
			if(i%primes[j]==0)
				break;
		}
	}
}

int firstNoLessThan(int n,int begin,int end){
	if(end==begin+1){
		if(primes[begin]>=n)
			return begin;
		return end;
	}
	int mid=(begin+end)/2;
	if(primes[mid]>=n)
		return firstNoLessThan(n,begin,mid);
	return firstNoLessThan(n,mid,end);
}

int firstGreaterThan(int n,int begin,int end){
	if(end==begin+1){
		if(primes[begin]>n)
			return begin;
		return end;
	}
	int mid=(begin+end)/2;
	if(primes[mid]>n)
		return firstGreaterThan(n,begin,mid);
	return firstGreaterThan(n,mid,end);
}

int main(){
	primeSieve();
	while(true){
		int first,last;
		scanf("%d%d",&first,&last);
		if(first==-1 && last==-1)
			return 0;
		int end=firstGreaterThan(last,0,primeNum);
		int begin=firstNoLessThan(first,0,primeNum);
		printf("%d\n",end-begin);
	}
}