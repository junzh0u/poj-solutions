#include<cstdio>

const int SIZE_MAX=1000,PRIME_NUM_MAX=200;

int main(){
	bool isPrime[SIZE_MAX+1]={false};
	for(int i=2;i<=SIZE_MAX;i++)
		isPrime[i]=true;
	for(int factor=2;factor<=SIZE_MAX;factor++)
		if(isPrime[factor])
			for(int multiple=2;multiple*factor<=SIZE_MAX;multiple++)
				isPrime[multiple*factor]=false;
	int primes[PRIME_NUM_MAX],primeNum=0;
	for(int i=2;i<=SIZE_MAX;i++)
		if(isPrime[i])
			primes[primeNum++]=i;

	int caseNum;
	scanf("%d",&caseNum);
	for(int caseID=1;caseID<=caseNum;caseID++){
		int size;
		scanf("%d",&size);
		int visiblePointNum=0;
		for(int x=2;x<=size;x++){
			int visibleYNum;
			if(isPrime[x])
				visibleYNum=x-1;
			else{
				visibleYNum=x;
				for(int primeIndex=0;primeIndex<primeNum;primeIndex++){
					int prime=primes[primeIndex];
					if(prime>x)
						break;
					if(x%prime==0)
						visibleYNum-=visibleYNum/prime;
				}
			}
			visiblePointNum+=visibleYNum;
		}
		visiblePointNum=2*visiblePointNum+3;
		printf("%d %d %d\n",caseID,size,visiblePointNum);
	}
	return 0;
}