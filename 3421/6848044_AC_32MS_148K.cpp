#include<cstdio>
#include<algorithm>

const int PRIME_MAX=1<<10,PRIME_NUM=200,FACTOR_NUM_MAX=400;

int primes[PRIME_NUM],primeNum;

void initialize(){
	bool isPrime[PRIME_MAX+1];
	std::fill(isPrime,isPrime+PRIME_MAX+1,true);
	for(int factor=2;factor*factor<=PRIME_MAX;factor++)
		if(isPrime[factor])
			for(int product=2*factor;product<=PRIME_MAX;product+=factor)
				isPrime[product]=false;
	primeNum=0;
	for(int number=2;number<=PRIME_MAX;number++)
		if(isPrime[number])
			primes[primeNum++]=number;
}

long long permutation(int number){
	if(number==1)
		return 1;
	return permutation(number-1)*number;
}

int main(){
	initialize();
	int last;
	while(scanf("%d",&last)!=EOF){
		if(last==1){
			printf("1 1\n");
			continue;
		}
		int factorNum=0,factorCount[FACTOR_NUM_MAX];
		for(int index=0;index<primeNum;index++){
			int prime=primes[index];
			if(prime>last)
				break;
			if(last%prime==0){
				factorCount[factorNum]=0;
				while(last%prime==0){
					factorCount[factorNum]++;
					last/=prime;
				}
				factorNum++;
			}
		}
		if(last>1)
			factorCount[factorNum++]=1;
		int factorCountSum=0;
		for(int index=0;index<factorNum;index++)
			factorCountSum+=factorCount[index];
		long long chainNum=permutation(factorCountSum);
		for(int index=0;index<factorNum;index++)
			chainNum/=permutation(factorCount[index]);
		printf("%d %lld\n",factorCountSum,chainNum);
	}
	return 0;
}