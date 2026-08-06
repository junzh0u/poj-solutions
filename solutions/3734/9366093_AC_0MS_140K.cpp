#define _CRT_SECURE_NO_WARNINGS
#include<cstdio>

const int MODULUS=10007;

int powerMod(int n){
	if(n==0)
		return 1;
	if(n&1)
		return (powerMod(n-1)*2)%MODULUS;
	int temp=powerMod(n>>1);
	return (temp*temp)%MODULUS;
}

int main(){
	int caseNum;
	scanf("%d",&caseNum);
	while(caseNum--){
		int n;
		scanf("%d",&n);
		n=n%(MODULUS-1);
		int temp=powerMod(n-1);
		printf("%d\n",(temp*(temp+1))%MODULUS);
	}
	return 0;
}