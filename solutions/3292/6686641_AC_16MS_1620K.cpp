#include<cstdio>
#include<algorithm>

const int NUM_MAX=1000001;
const int SIZE=(NUM_MAX+3)/4;

bool isPrime[SIZE],isSemiPrime[SIZE];
int semiPrimeNum[SIZE];

int main(){
	std::fill(isPrime,isPrime+SIZE,true);
	std::fill(isSemiPrime,isSemiPrime+SIZE,false);
	isPrime[0]=false;
	for(unsigned index=1;index<SIZE;index++)
		if(isPrime[index])
			for(unsigned multiple=3;(4*index+1)*multiple<=NUM_MAX;multiple+=2)
				if((4*index+1)*multiple%4==1)
					isPrime[((4*index+1)*multiple-1)/4]=false;
	for(unsigned left=1;left*left<SIZE;left++)
		if(isPrime[left])
			for(unsigned right=left;right<SIZE;right++){
				if(4*left*right+left+right>=SIZE)
					break;
				if(isPrime[right])
					isSemiPrime[4*left*right+left+right]=true;
			}
	std::fill(semiPrimeNum,semiPrimeNum+SIZE,0);
	semiPrimeNum[0]=0;
	for(int index=1;index<SIZE;index++){
		semiPrimeNum[index]=semiPrimeNum[index-1];
		if(isSemiPrime[index])
			semiPrimeNum[index]++;
	}
	while(true){
		int upper;
		scanf("%d",&upper);
		if(upper==0)
			return 0;
		printf("%d %d\n",upper,semiPrimeNum[(upper-1)/4]);
	}
	return 0;
}