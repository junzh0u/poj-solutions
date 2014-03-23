#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int COUNT_MAX=15,LAST_MAX=100000000,FACTOR_NUM_MAX=10;
int count,last,factorNum,factors[FACTOR_NUM_MAX],factorsChosen[FACTOR_NUM_MAX];
long long cardNum;

inline long long pow(int base,int exp){
	long long power=1;
	while(exp--)
		power*=base;
	return power;
}

void chooseFactor(int target,int chosen){
	if(chosen==target){
		int exclude=last;
		for(int i=0;i<chosen;i++)
			exclude/=factorsChosen[i];
		cardNum+=(target%2?-1:1)*pow(exclude,count);
		return;
	}
	for(int i=0;i<factorNum;i++)
		if(chosen==0 || factors[i]>factorsChosen[chosen-1]){
			factorsChosen[chosen]=factors[i];
			chooseFactor(target,chosen+1);
		}
}

void analyzeFactor(int integer){
	factorNum=0;
	for(int factor=2;factor*factor<=integer;factor++){
		if(integer%factor==0){
			while(integer%factor==0)
				integer/=factor;
			factors[factorNum++]=factor;
		}
	}
	if(integer!=1)
		factors[factorNum++]=integer;
}

int main(){
	scanf("%d%d",&count,&last);
	analyzeFactor(last);
	cardNum=pow(last,count);
	for(int i=1;i<=factorNum;i++)
		chooseFactor(i,0);
	printf("%lld\n",cardNum);
	return 0;
}